ref：

* [Safepoints in HotSpot JVM](http://blog.ragozin.info/2012/10/safepoints-in-hotspot-jvm.html)



# 概述



safepoint：

> Hotspot JVM上实现STW的机制

> STW是语义；safepoint是（Hotspot上STW）实现。 当然，其他JVM实现上搞出个什么东西叫STW或safepoint也没关系



效果：

* all threads running java code are suspended. 

* Threads running native code may continue to run as long as they do not interact with JVM (attempt to access Java objects via JNI, call Java method or return from native to java, will suspend thread until end of safepoint).

  和JVM交互时会检查安全点，于是被...

  场景：

  * 通过JNI（从native code）访问java对象
  * 调用java方法
  * 从native返回java




## safe region



from [内存篇：JVM内存回收理论与实现](http://icyfenix.iteye.com/blog/1166660)

```
Safepoint机制保证了程序执行时，在不太长的时间内就会遇到可进入GC的Safepoint。但是，程序“不执行”的时候呢？所谓的程序不执行就是没有分配CPU时间，典型的例子就是线程处于Sleep状态或者Blocked状态，这时候线程无法响应JVM的中断请求，走到安全的地方去中断挂起，JVM也显然不太可能等待线程重新被分配CPU时间。对于这种情况，就需要安全区域（Safe Region）来解决。
　　安全区域是指在一段代码片段之中，引用关系不会发生变化。在这个区域中任意地方开始GC都是安全的。我们也可以把Safe Region看作是被扩展了的Safepoint。
　　在线程执行到Safe Region里面的代码时，首先标识自己已经进入了Safe Region，那样当这段时间里JVM要发起GC，就不用管标识自己为Safe Region状态的线程了。在线程要离开Safe Region时，它要检查系统是否已经完成了根节点枚举（或者是整个GC过程），如果完成了，那线程就继续执行，否则它就必须等待直到收到可以安全离开Safe Region的信号为止。 
```



这里`JVM的中断请求`用的是抢占式的描述手法，这时根据`抢占`的实现不同，可能有一些场景无法打断/抢占； 而更主要的协作式safepoint，本来就不可能每个指令前都check，so...



下面提到

> JNI场景下，是不用等线程back to safepoint的

那么需要明确哪些线程不用等待，可能有：

* 根据识别出JNI等操作来认为...

* 线程做JNI等操作时都mark enter safe region，然后…识别出...

  {yonka} 估计是这种吧？





# 使用



Below are few reasons for HotSpot JVM to initiate a safepoint:

- Garbage collection pauses
- Code deoptimization
- Flushing code cache
- Class redefinition (e.g. hot swap or instrumentation)
- Biased lock revocation
- Various debug operation (e.g. deadlock check or stacktrace dump)




# 实现



from [内存篇：JVM内存回收理论与实现](http://icyfenix.iteye.com/blog/1166660)

```
对于Sefepoint，另外一个需要考虑的问题是如何让GC发生时，让所有线程（这里不包括执行JNI调用的线程）都跑到最近的安全点上再停顿下来。我们有两种方案可供选择：抢先式中断（Preemptive Suspension）和主动式中断（Voluntary Suspension），抢先式中断不需要线程的执行代码主动去配合，在GC发生时，首先把所有线程全部中断，如果发现有线程中断的地方不在安全点上，就恢复线程，让它跑到安全点上。现在几乎没有虚拟机实现采用抢先式中断来暂停线程响应GC事件。
　　而主动式中断的思想是当GC需要中断线程的时候，不直接对线程操作，仅仅简单地设置一个标志，各个线程执行时主动去轮询这个标志，发现中断标志为真时就自己中断挂起。轮询标志的地方和安全点是重合的，另外再加上创建对象需要分配内存的地方。下面的代码清单2中的test指令是HotSpot生成的轮询指令，当需要暂停线程时，虚拟机把0x160100的内存页设置为不可读，那线程执行到test指令时就会停顿等待，这样一条指令便完成线程中断了。 
```

```assembly
0x01b6d627: call   0x01b2b210         ; OopMap{[60]=Oop off=460}    
                                       ;*invokeinterface size    
                                       ; - Client1::main@113 (line 23)    
                                       ;   {virtual_call}    
 0x01b6d62c: nop                       ; OopMap{[60]=Oop off=461}    
                                       ;*if_icmplt    
                                       ; - Client1::main@118 (line 23)    
 0x01b6d62d: test   %eax,0x160100      ;   {poll}    
 0x01b6d633: mov    0x50(%esp),%esi    
 0x01b6d637: cmp    %eax,%esi   
```





* 抢占式
* 主动式/协同（作）式







## Hotspot



* **协作式**

  参考go的调度实现等；

  对立的就是**抢占式**了；



### 如何“协作”

通过线程**主动**做**safepoint （status） check**来进入…。



因为执行频率较高，所以要尽可能的减小开销。



### 状态检查执行时机

* For `compiled code`, JIT inserts safepoint checks in code at certain points (usually, after **return from calls** or at **back jump of loop**).
* For `interpreted code`, JVM have two byte code dispatch tables and if safepoint is required, JVM switches tables to enable safepoint check.




**profilers_are_lying_hobbitses** ppt上看到的

Where do we see a Safepoint poll?

* Between every 2 bytecodes (interpreter)

  这个太离谱了吧

* Backedge of non-'counted' loops (C1/C2)

* Method exit (C1/C2)

* JNI call exit




### 状态修改/通知

主要是：

* `SafepointSynchronize::begin()`

  membar + mprotect

* `SafepointSynchronize::end()`



**SafepointSynchronize::begin()方法内部有如下注释**

> Begin the process of bringing the system to a safepoint.  
>
> Java threads can be in several different states and are stopped by different mechanisms:  
>
> 1. **Running interpreted  **
>
>    The interpeter dispatch table is changed to force it to check for a safepoint condition between bytecodes.  
>
> 2. **Running in native code**
>
>    When returning from the native code, a Java thread must check the safepoint _state to see if we must block.  
>
>    If the VM thread sees a Java thread in native, it does not wait for this thread to block.  
>
>    The order of the memory writes and reads of both the safepoint state and the Java threads state is critical.  
>
>    In order to guarantee that the memory writes are serialized with respect to each other,  the VM thread issues a memory barrier instruction  (on MP systems).  
>
>    In order to avoid the overhead of issuing  a mem barrier for each Java thread making native calls, each Java  thread performs a write to a single memory page after changing the thread state.  The VM thread performs a sequence of mprotect OS calls which forces all previous writes from all Java threads to be serialized.  This is done in the os::serialize_thread_states() call.  This has proven to be much more efficient than executing a membar instruction on every call to native code.  
>
> 3. **Running compiled Code** Compiled code reads a global (Safepoint Polling) page that is set to fault if we are trying to get to a safepoint.  
>
> 4. **Blocked**
>
>    A thread which is blocked will not be allowed to return from the block condition until the safepoint operation is complete.  
>
> 5. **In VM or Transitioning between states**
>
>    If a Java thread is currently running in the VM or transitioning between states, the safepointing code will wait for the thread to block itself when it attempts transitions to a new state.  

R大补充：

```
LeafInWind 写道
注意第二点的最后一句话，就是说使用serialization page的性能比membar好

因为原本Java thread和VM thread都要做membar；用了serialization page之后只有VM thread那边要做mprotect+membar（比原本慢），而Java thread只要做一个普通的serialization page read（比原本快）。VM thread只有一个而Java thread可能有很多个，这样就赚了。
```







### 状态检查

多线程操作，普通的（多线程）共享变量的保护机制需要**expensive memory barrier**，于是使用了**memory reads a barrier**的方式实现：

1. 需要设置safepoint状态（notify）时，unmap该内存（地址）

2. 检查时，如果safepoint状态被设置，则会触发page fault，然后在handler里...

   > page unmap is forcing memory barrier to processing cores

   > {yonka} TODO 不是很清楚这个 read mem-bar，跟前面的expensive mem-bar的关系




代码主要是：

* `ThreadSafepointState::examine_state_of_thread`


* `SafepointSynchronize::safepoint_safe`
* `ThreadSafepointState::roll_forward`



# 相关



## 问题排查



有时可能会怀疑safepoint时间过长，这时候需要进行排查。

- `-XX:+PrintGCApplicationStoppedTime` – this will actually report pause time for all safepoints (GC related or not). Unfortunately output from this option lacks timestamps, but it is still useful to narrow down problem to safepoints.
- `-XX:+PrintSafepointStatistics -XX:PrintSafepointStatisticsCount=1` – this two options will force JVM to report reason and timings after each safepoint (it will be reported to stdout, not GC log).



输出类似：

```
         vmop                    [threads: total initially_running wait_to_block]    [time: spin block sync cleanup vmop] page_trap_count
0.443: ThreadDump                       [      22          1              3    ]      [     0     0     0     0     0    ]  1     // PrintSafepointStatistics
Total time for which application threads were stopped: 0.0010726 seconds, Stopping threads took: 0.0002748 seconds  // PrintGCApplicationStoppedTime
         vmop                    [threads: total initially_running wait_to_block]    [time: spin block sync cleanup vmop] page_trap_count
0.486: ThreadDump                       [      22          1              3    ]      [     0     0     0     0     0    ]  1   
Total time for which application threads were stopped: 0.0002783 seconds, Stopping threads took: 0.0001089 seconds
         vmop                    [threads: total initially_running wait_to_block]    [time: spin block sync cleanup vmop] page_trap_count
0.496: ThreadDump                       [      22          1              2    ]      [     0     0     0     0     0    ]  1   
Total time for which application threads were stopped: 0.0002673 seconds, Stopping threads took: 0.0001118 seconds
         vmop                    [threads: total initially_running wait_to_block]    [time: spin block sync cleanup vmop] page_trap_count
0.504: ThreadDump                       [      22          1              2    ]      [     0     0     0     0     0    ]  1   
Total time for which application threads were stopped: 0.0001750 seconds, Stopping threads took: 0.0000793 seconds
         vmop                    [threads: total initially_running wait_to_block]    [time: spin block sync cleanup vmop] page_trap_count
0.511: ThreadDump                       [      22          0              3    ]      [     0     0     0     0     0    ]  0   
Total time for which application threads were stopped: 0.0001303 seconds, Stopping threads took: 0.0000345 seconds
         vmop                    [threads: total initially_running wait_to_block]    [time: spin block sync cleanup vmop] page_trap_count
0.520: ThreadDump                       [      22          1              3    ]      [     0     0     0     0     0    ]  1   
Total time for which application threads were stopped: 0.0002376 seconds, Stopping threads took: 0.0001378 seconds
```





## [JVM模板解释器安全点问题](http://hllvm.group.iteye.com/group/topic/45826)

```

p2p2500 2015-06-18
JVM在执行过程中，可能会在安全点出进行GC，安全点位置包括如下：
1、循环的末尾
2、方法临返回前 / 调用方法的call指令后
3、可能抛异常的位置

我的问题是：
1. JVM的解释器执行过程中，是不是在上述指令位置处，修改安全标记，将指令模板由正常版本切换到安全版本？

2. JVM模板解释器生成的 正常指令table模板与安全版本的区别是什么？

```

```
针对问题1. 答案是否定的。在hotspot中，安全标记的设置及指令模板的切换都发生在SafepointSynchronize::begin()函数中，而该函数的调用者是VMThead线程。该线程负责检查并执行各种VMOperation。如果有某个java线程在执行过程中发现内存不够，则会创建一个GC VMOperation，然后进入等待状态。而VMThread如果发现有GC VMOperation，则会设置安全标记、切换模板表、将polling page置为不可读，然后等待所有java线程进入等待状态，再开始GC。
设置安全标记是针对java线程正在执行jni代码的情况，这样的线程在从jni返回时会检查安全标记，如果安全标记置位，这让自己block。
切换模板表是针对java线程正在执行解释器代码的情况。安全版的模板表在执行字节码的正常功能前，会先检查安全标记，然后视情况让自己block。
将polling page置位不可读是针对java线程执行编译代码的情况。编译代码在循环回跳、方法返回的地方有一个对polling page的读操作。如果polling page可读，则正常回跳或者返回，如果polling page不可读，则发生段违例，进入hotspot的信号处理函数，该函数会让当前java线程block。

针对楼主的问题2，正常模板表与安全模板表的区别就是安全模板表会在执行字节码的正常功能前检查安全标记并视情况block当前java线程。 
```









