Ref:

* [聊聊JVM（五）从JVM角度理解线程](http://blog.csdn.net/iter_zc/article/details/41843595)



# thread state



## java thread state



```java
    /**
     * A thread state.  A thread can be in one of the following states:
     * <ul>
     * <li>{@link #NEW}<br>
     *     A thread that has not yet started is in this state.
     *     </li>
     * <li>{@link #RUNNABLE}<br>
     *     A thread executing in the Java virtual machine is in this state.
     *     </li>
     * <li>{@link #BLOCKED}<br>
     *     A thread that is blocked waiting for a monitor lock
     *     is in this state.
     *     </li>
     * <li>{@link #WAITING}<br>
     *     A thread that is waiting indefinitely for another thread to
     *     perform a particular action is in this state.
     *     </li>
     * <li>{@link #TIMED_WAITING}<br>
     *     A thread that is waiting for another thread to perform an action
     *     for up to a specified waiting time is in this state.
     *     </li>
     * <li>{@link #TERMINATED}<br>
     *     A thread that has exited is in this state.
     *     </li>
     * </ul>
     *
     * <p>
     * A thread can be in only one state at a given point in time.
     * These states are virtual machine states which do not reflect
     * any operating system thread states.
     *
     * @since   1.5
     * @see #getState
     */
    public enum State {
        /**
         * Thread state for a thread which has not yet started.
         */
        NEW,

        /**
         * Thread state for a runnable thread.  A thread in the runnable
         * state is executing in the Java virtual machine but it may
         * be waiting for other resources from the operating system
         * such as processor.
         */
        RUNNABLE,

        /**
         * Thread state for a thread blocked waiting for a monitor lock.
         * A thread in the blocked state is waiting for a monitor lock
         * to enter a synchronized block/method or
         * reenter a synchronized block/method after calling
         * {@link Object#wait() Object.wait}.
         */
        BLOCKED,

        /**
         * Thread state for a waiting thread.
         * A thread is in the waiting state due to calling one of the
         * following methods:
         * <ul>
         *   <li>{@link Object#wait() Object.wait} with no timeout</li>
         *   <li>{@link #join() Thread.join} with no timeout</li>
         *   <li>{@link LockSupport#park() LockSupport.park}</li>
         * </ul>
         *
         * <p>A thread in the waiting state is waiting for another thread to
         * perform a particular action.
         *
         * For example, a thread that has called <tt>Object.wait()</tt>
         * on an object is waiting for another thread to call
         * <tt>Object.notify()</tt> or <tt>Object.notifyAll()</tt> on
         * that object. A thread that has called <tt>Thread.join()</tt>
         * is waiting for a specified thread to terminate.
         */
        WAITING,

        /**
         * Thread state for a waiting thread with a specified waiting time.
         * A thread is in the timed waiting state due to calling one of
         * the following methods with a specified positive waiting time:
         * <ul>
         *   <li>{@link #sleep Thread.sleep}</li>
         *   <li>{@link Object#wait(long) Object.wait} with timeout</li>
         *   <li>{@link #join(long) Thread.join} with timeout</li>
         *   <li>{@link LockSupport#parkNanos LockSupport.parkNanos}</li>
         *   <li>{@link LockSupport#parkUntil LockSupport.parkUntil}</li>
         * </ul>
         */
        TIMED_WAITING,

        /**
         * Thread state for a terminated thread.
         * The thread has completed execution.
         */
        TERMINATED;
    }
```



对应：

```cpp
/*
 * Java thread state support
 */
enum {
    JAVA_THREAD_STATE_NEW           = 0,
    JAVA_THREAD_STATE_RUNNABLE      = 1,
    JAVA_THREAD_STATE_BLOCKED       = 2,
    JAVA_THREAD_STATE_WAITING       = 3,
    JAVA_THREAD_STATE_TIMED_WAITING = 4,
    JAVA_THREAD_STATE_TERMINATED    = 5,
    JAVA_THREAD_STATE_COUNT         = 6
};
```





### java thread state VS kernel/OS thread state

首先，java thread 和 OS thread是1:1的关系，这样才有比较状态的意义。

其次，因为抽象对象不同，两者的状态并不等同 = =，尤其是RUNNABLE。

* NEW

  没启动，实际上应该只有java thread对象而没有对应的os thread


* RUNNABLE

  在VM中执行就是RUNNABLE，也即VM只关注线程是否因为**自己**的原因不能执行，如果是因为其他原因，比如OS调度、阻塞系统调用等，都不管，**这是大坑**，比如profile等场景。

* BLOCKED

  java doc里描述为`blocked waiting for a monitor lock`，注意是monitor lock而不是wait queue。

  常见场景有：

  * `enter a synchronized block/method`
  * `reenter a synchronized block/method after calling Object.wait`

* WAITING

  wait queue，...

  `A thread in the waiting state is waiting for another thread to perform a particular action`

  常见场景有：

  * 不带timeout的`Object.wait`
  * 不带timeout的`Thread.join`
  * `LockSupport.part`

* TIMED_WAITING

  和WAITING类似，只是... 带timeout的...

* TERMINATED

  结束运行，应该是os thread执行完退出后剩下的java thread的状态




## Jvm 角度



### JavaThreadState



```cpp
enum JavaThreadState {
  _thread_uninitialized     =  0, // should never happen (missing initialization)
  _thread_new               =  2, // just starting up, i.e., in process of being initialized
  _thread_new_trans         =  3, // corresponding transition state (not used, included for completness)
  _thread_in_native         =  4, // running in native code
  _thread_in_native_trans   =  5, // corresponding transition state
  _thread_in_vm             =  6, // running in VM
  _thread_in_vm_trans       =  7, // corresponding transition state
  _thread_in_Java           =  8, // running in Java or in stub code
  _thread_in_Java_trans     =  9, // corresponding transition state (not used, included for completness)
  _thread_blocked           = 10, // blocked in vm
  _thread_blocked_trans     = 11, // corresponding transition state
  _thread_max_state         = 12  // maximum thread state+1 - used for statistics allocation
};
```



其中主要的状态是这5种:

**_thread_new**: 新创建的线程

**_thread_in_Java**: 在运行Java代码

**_thread_in_vm**: 在运行JVM本身的代码

**_thread_in_native**: 在运行native代码

**_thread_blocked**: 线程被阻塞了，包括等待一个锁，等待一个条件，sleep，执行一个阻塞的IO等



### OSThread state

```cpp
enum ThreadState {
  ALLOCATED,                    // Memory has been allocated but not initialized
  INITIALIZED,                  // The thread has been initialized but yet started
  RUNNABLE,                     // Has been started and is runnable, but not necessarily running
  MONITOR_WAIT,                 // Waiting on a contended monitor lock
  CONDVAR_WAIT,                 // Waiting on a condition variable
  OBJECT_WAIT,                  // Waiting on an Object.wait() call
  BREAKPOINTED,                 // Suspended at breakpoint
  SLEEPING,                     // Thread.sleep()
  ZOMBIE                        // All done, but not reclaimed yet
};
```

> 对外暴露的状态



比较常见有:

**Runnable**: 可以运行或者正在运行的

**MONITOR_WAIT**: 等待锁

**OBJECT_WAIT**: 执行了Object.wait()之后在条件队列中等待的

**SLEEPING**: 执行了Thread.sleep()的



#### 





## 各状态

### RUNNABLE



Ref: [java-cpu-sampling-using-hprof](http://www.brendangregg.com/blog/2014-06-09/java-cpu-sampling-using-hprof.html)

```
JVMTI_THREAD_STATE_RUNNABLE maps to the Java RUNNABLE Thread.State, which is documented as:

A thread in the runnable state is executing in the Java virtual machine but it may be waiting for other resources from the operating system such as processor.
Runnable does not mean running. It is also not the same as what the kernel scheduler calls "runnable". Sampling Java runnable threads is not the same thing as CPU sampling, but hprof documents it as "CPU SAMPLES". It would be better described as "JVM RUNNABLE SAMPLES".

This fact doesn't seem to be well known. The best reference I found said (from 2005):

When you have Java threads that are somehow not using the CPU, but managing to stay active, then it will appear as if those stack traces are consuming large amounts of CPU time when they aren't.
```



所以经常面临的问题是： 

* 非RUNNABLE的不会是running

* RUNNABLE的也不一定是running

  而一些常见，比如profiling，希望了解到真实消耗CPU的running时间





#### 判断线程是否真实running



几种方式：

- 获取kernel （thread）state，如`/proc/${pid}/{status}`

  **但是问题**： 这里需要native thread id（nid），见**thread id**小结，可用方式比较局限。

- JVMTI

  ```c
  /* Get traces for all threads in list (traces[i]==0 if thread not running) */
  void
  trace_get_all_current(jint thread_count, jthread *threads,
  [...]
          /* If thread has frames, is runnable, and isn't suspended, we care */
          if ( always_care ||
               ( stack_info[i].frame_count > 0
                 && (stack_info[i].state & JVMTI_THREAD_STATE_RUNNABLE)!=0
                 && (stack_info[i].state & JVMTI_THREAD_STATE_SUSPENDED)==0
                 && (stack_info[i].state & JVMTI_THREAD_STATE_INTERRUPTED)==0 )
              ) {
  // [...do sampling...]
  ```

  可以直接获取到

- 迂回方式： 排除掉可能陷入阻塞的方法

  思路是： inNative + 枚举排除，如果栈顶帧是…，则认为...

  ref： [Java CPU Sampling Using hprof](http://www.brendangregg.com/blog/2014-06-09/java-cpu-sampling-using-hprof.html)

  ```python
      ignore_set = set([
          'java.net.SocketInputStream.socketRead0',
          'java.net.SocketOutputStream.socketWrite0',
          'java.net.PlainSocketImpl.socketAvailable',
          'java.net.PlainSocketImpl.socketAccept',
          'sun.nio.ch.EPollArrayWrapper.epollWait',
      ])
  ```

  ​




# thread id



java里thread相关的id有：

* `Thread.currentThread().getId()`/`Thread.getId()`

  得到的是一个递增的值，如main的为1

  > 这个实际就是java层面的thread id
  >
  > The thread ID is unique during its lifetime. **When a thread is terminated,  this thread ID may be reused**.

* jstack看到的tid

  `"main" prio=5 tid=0x00007ffe94007800 nid=0x1b03 waiting on condition [0x00007000088f6000]`

  同样的case里，可以看到main的tid是一个很大的数值

  > 这个实际是jvm层面对os thread的wrapper的c++对象的地址
  >
  > **平台相关**

  ```cpp
    // print guess for valid stack memory region (assume 4K pages); helps lock debugging
    st->print_cr("[" INTPTR_FORMAT "]", (intptr_t)last_Java_sp() & ~right_n_bits(12));
  ```

  ​

* jstack看到的nid

  这个就是java thread映射到的os thread的id，也即native nid。

  这里把nid转为十进制即是...

  **平台相关**



## thread id映射



### tid <-> threadId

gg，java层面没搜到如何映射。

其他层面暂时未知 = =。



### tid <-> nid

gg



### threadId <-> nid

目前知道的只有：

- SA - 从外部；有java API
- JVMTI - 从内部； 没有java API



# 使用



## 创建线程



有两种方式可以让用户在JVM中创建线程

1. new java.lang.Thread().start()
2. 使用JNI将一个native thread attach到JVM中



### Thread.start

只有调用start()方法的时候，才会真正的在JVM中去创建线程，主要的生命周期步骤有：

1. 创建对应的JavaThread的instance
2. 创建对应的OSThread的instance
3. 创建实际的底层操作系统的native thread
4. 准备相应的JVM状态，比如ThreadLocal存储空间分配等
5. 底层的native thread开始运行，调用java.lang.Thread生成的Object的run()方法
6. 当java.lang.Thread生成的Object的run()方法执行完毕返回后,或者抛出异常终止后，终止native thread
7. 释放JVM相关的thread的资源，清除对应的JavaThread和OSThread



### JNI attach

主要的步骤有：

1. 通过JNI call AttachCurrentThread申请连接到执行的JVM实例
2. JVM创建相应的JavaThread和OSThread对象
3. 创建相应的java.lang.Thread的对象
4. 一旦java.lang.Thread的Object创建之后，JNI就可以调用Java代码了
5. 当通过JNI call DetachCurrentThread之后，JNI就从JVM实例中断开连接
6. JVM清除相应的JavaThread, OSThread, java.lang.Thread对象





# 实现

ref：

* [聊聊JVM（五）从JVM角度理解线程](http://blog.csdn.net/iter_zc/article/details/41843595)



## java 层面

**Class hierarchy**

- Thread
  - NamedThread
    - VMThread
    - ConcurrentGCThread
    - WorkerThread
      - GangWorker
      - GCTaskThread
  - JavaThread
  - WatcherThread

另外还有一个重要的类**OSThread**不在这个继承关系里，它以组合的方式被Thread类所使用



* **java.lang.Thread:** 这个是Java语言里的线程类，由这个Java类创建的instance都会 1:1 映射到一个操作系统的osthread
* **JavaThread: **JVM中C++定义的类，一个JavaThread的instance代表了在JVM中的java.lang.Thread的instance, 它维护了线程的状态，并且维护一个指针指向java.lang.Thread创建的对象(oop)。它同时还维护了一个指针指向对应的OSThread，来获取底层操作系统创建的osthread的状态****
* **OSThread: **JVM中C++定义的类，代表了JVM中对底层操作系统的osthread的抽象，它维护着实际操作系统创建的线程句柄handle，可以获取底层osthread的状态
* **VMThread:** JVM中C++定义的类，这个类和用户创建的线程无关，是JVM本身用来进行虚拟机操作的线程，比如GC。



### VMThread

主要由几类:

VMThread: 执行JVM本身的操作

Periodic task thread: JVM内部执行定时任务的线程

GC threads: GC相关的线程，比如单线程/多线程的GC收集器使用的线程

Compiler threads: JIT用来动态编译的线程

Signal dispatcher thread: Java解释器Interceptor用来辅助safepoint操作的线程










