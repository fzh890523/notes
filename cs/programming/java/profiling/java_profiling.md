# java profiling方式/工具



## JMC - java mission control 

Ref:

* [oracle - missioncontrol](http://oracle.com/missioncontrol)
* [JMC - FAQ](https://community.oracle.com/thread/2579717)
* [JMC - product](http://www.oracle.com/technetwork/java/javaseproducts/mission-control/index.html)
* [youtube - Java Mission Control demo](https://www.youtube.com/watch?v=qytuEgVmhsI)



**商业特性**



### 介绍



组成：

* JMX console

  通过JMX获取数据显示

* JFR

  收集JVM和应用数据

* 插件式的可选工具

  * heap dump分析
  * DTrace ...



### FlightRecorder

```Shell
-XX:+UnlockCommercialFeatures -XX:+FlightRecorder
```



[java-mission-control-wp-2008279](http://www.oracle.com/technetwork/java/javaseproducts/mission-control/java-mission-control-wp-2008279.pdf)里提到几个思路：

* 每个线程的数据先收到thread-local buf，大大较少并发访问
* thread-local buf满了后转移到global-buf，这一步会有竞争访问，但相对较少
* global buf满了就写入磁盘，也可以根据策略使用ring-buf丢弃老数据
* ` Java Flight Recorder plugin in Java Mission Control`JFR数据分析功能以JMC插件形式存在



# 获取 thread dump 方式



ref：

* [Profiling with JVMTI/JVMPI, SIGPROF and AsyncGetCallTrace](http://jeremymanson.blogspot.co.id/2007/05/profiling-with-jvmtijvmpi-sigprof-and.html) or [local](resource/Java Concurrency (&c)_ Profiling with JVMTI_JVMPI, SIGPROF and AsyncGetCallTrace.html)
* [lightweight-asynchronous-sampling](http://jeremymanson.blogspot.co.id/2013/07/lightweight-asynchronous-sampling.html) or [local](resource/Java Concurrency (&c)_ Lightweight Asynchronous Sampling Profiler)



- kill -3

- jstack

- JMX from inside the JVM

- JMX remote

- JPDA (remote)

- JVMTI (C API)

  总的来说，JVMTI 等C系相比java

  * `GetStackTrace` or `GetAllStackTraces` methods

    ```
    The problem is that there is no officially documented JVMTI hook that allows the user to find out exactly what the currently running thread is doing that is safe to run in a signal handler. The official way of getting a stack trace for the currently executing thread, the GetStackTrace function, isn't safe to be called in an asynchronous way -- if you try to read the stack when a timer expires, the Java stack could be in a corrupt state, or GC could be running, or any number of other things.
    ```

    噗，也就是说这个只能查看caller当前线程的吗？ = =

    另一篇文档里说，`GetStackTrace`一来开销大二来一来safepoint。

    在openJDK上的开销：

    * Stop ALL Java threads
    * Collect single/all thread call traces
    * Resume ALL stopped threads

    在zing上的开销：

    * Stop sampled Java thread
    * Collect single thread call trace
    * Resume stopped thread

    可知，开销是随着线程数增加而增大的，尤其对于openJDK（部分线程可能要等“慢”线程很久）。

  * `AsyncGetCallTrace`

    ```
    If you send a SIGPROF to your JVM and use AsyncGetCallTrace, you find out exactly what your JVM is doing at precisely the moment you sent the signal.

    The difference here is fundamentally that all of those other methods tell you what the JVM could be doing, and this one tells you what it is doing. It will even tell you if it is performing garbage collection. This sort of information can be invaluable when you want to know what is soaking up your CPU cycles.
    ```

    ​

    ```
    It turns out that the kind folks who wrote the Java virtual machine were fully aware of this, and provided an undocumented interface for this type of profiling, used by their Forte Analyzer (which now operates under the Sun Studio umbrella, I believe). Now that they've open-sourced the JVM, this is public knowledge. For those of you who like to see the source code for such things, it can be found in hotspot/src/share/prims/forte.cpp.

    In principle, AsyncGetCallTrace is fairly easy to use. This is less true in practice. Since JVMPI has been removed in JDK6, you start by having to include JVMPI structures in your headers. In JDK5 and earlier, this step in unnecessary (all covered under the GPL):
    ```

    ​

    ```c
    typedef struct {
     jint lineno;
     jmethodID method_id;
    } JVMPI_CallFrame;

    typedef struct {
     JNIEnv *env_id;
     jint num_frames;
     JVMPI_CallFrame *frames;
    } JVMPI_CallTrace;

    // Now that you have the JVMPI structures defined, you need a prototype for the undocumented call:

    extern "C"
    void AsyncGetCallTrace(JVMPI_CallTrace *trace, jint depth,
     void* ucontext)
    __attribute__ ((weak));

    // The __attribute__ ((weak)) is only for g++ users -- it tells the compiler not to look for AsyncGetCallTrace at compile or link time. People using other compilers can create a library stub that contains this method -- this is left as an exercise for the reader.

    void JNICALL OnClassLoad(jvmtiEnv *jvmti_env,
                            JNIEnv* jni_env,
                            jthread thread,
                            jclass klass) {
    }

    // ...

    jvmtiEnv *jvmti;
    vm->GetEnv((void **)&jvmti, JVMTI_VERSION);
    jvmtiEventCallbacks *callbacks =
     new jvmtiEventCallbacks();
    callbacks->ClassLoad = &OnClassLoad;
    jvmti->SetEventCallbacks(callbacks,
     sizeof(jvmtiEventCallbacks));
    jvmti->SetEventNotificationMode(JVMTI_ENABLE,
     JVMTI_EVENT_CLASS_LOAD, NULL);

    void JNICALL OnClassPrepare(jvmtiEnv *jvmti_env,
                               JNIEnv* jni_env,
                               jthread thread,
                               jclass klass) {
     // We need to do this to "prime the pump",
     // as it were -- make sure that all of the
     // methodIDs have been initialized internally,
     // for AsyncGetCallTrace.  I imagine it slows
     // down class loading a mite, but honestly,
     // how fast does class loading have to be?
     jint method_count;
     jmethodID *methods;
     jvmti_env->GetClassMethods(klass, &method_count,
       &methods);
     delete [] methods;
    }
    ```

    ​

    ```
    Edited to add: Some of the commenters below wonder why the stack trace grabber can't print out the stack traces when it gets them. The answer to this lies in what I meant when I called this function asynchronous. I was a little (deliberately) vague on what "asynchronous" means. When I say that the timer that goes off to grab a stack trace interrupts a running thread asynchronously, I mean that it can interrupt the thread at any point. The thread can be in the middle of GC, or the middle of adding a frame to the stack, or in the middle of acquiring a lock. The reader can imagine that writing something asynchronous-safe is somewhat more tricky than writing something thread-safe: not only do you have to worry about what other threads are doing, you also have to worry about what the current thread is doing.
    ```

    但**profilers_are_lying_hobbitses** ppt里提到，其实只会对被信号打断的线程做sample，而对于pthread线程实现，处理信号的线程的选取是`by a single, **arbitrarily** selected thread within the process.`，那么只能从统计角度来认为采样是（在各线程间）均匀的，同时还依赖内核的该选取算法实现。 = =

    > TODO 这一块不是很清楚。 如果其他线程都屏蔽某信号的话，那该信号是不是每次都发送给特定线程（signal handler）？

    ​

    [open-source-jvm-sampling-profiler](http://insightfullogic.com/2014/Mar/02/open-source-jvm-sampling-profiler/) 里对jeremy的该方案的评价：

    ```
    Consequently it avoids a number of profiler accuracy issues that other sampling profilers suffer from. The downside of using this method is that the code in your async callback has horrific restrictions on it, for example the inability to heap allocate memory. The proof of concept code showed some great ingenuity but, being a proof of concept, lacked useful features such as a user interface or support for viewing profiling data being updated as the program was running.
    ```

    还有：

    ```
    This profiler gets around the severe restrictions on what can be done in the async callbacks by copying the current stack trace into a non-blocking, multiple producer, single consumer circular queue which allocates a fixed size block of memory up front. These stack traces are then read by another thread which writes out the log file. The other thread also needs to look up information about useful things like method names because the JVMTI calls to lookup method name information aren't async safe.
    ```

    这段不太明白，method name怎么获取？ 可以看下作者的项目： [honest-profiler](https://github.com/jvm-profiling-tools/honest-profiler)。 看起来是基于jeremy的项目改的。

    ​



- 外部触发

  - jstack

    - `jstack -l <pid> > <file-path>`

    甚至可以：

    ```java
    import com.sun.tools.attach.VirtualMachine;
    import sun.tools.attach.HotSpotVirtualMachine;
    import java.io.InputStream;

    public class StackTrace {

        public static void main(String[] args) throws Exception {
            String pid = args[0];
            HotSpotVirtualMachine vm = (HotSpotVirtualMachine) VirtualMachine.attach(pid);

            try (InputStream in = vm.remoteDataDump()) {
                byte[] buf = new byte[8000];
                for (int bytes; (bytes = in.read(buf)) > 0; ) {
                    System.out.write(buf, 0, bytes);
                }
            } finally {
                vm.detach();
            }
        }
    }
    ```

    ​

  - **linux** kill -3

    - `kill -3 <pid>`
    - 原理
      - signal dispatch thread + ...

  - jvisualVM

    `点击进程名称对应的 Thread Dump 按钮，将会生成 dump 文件`

    - 原理

      ```
      之前研究过jVisualVM的抽样功能，原理是利用JMX的ThreadMXBean.dumpAllThreads生成stack样本，根据出现的概率分析各个方法占用时间，优点是对系统侵入很小，写个attach小工具无需对线上系统做任何修改，但是这个时间跟cpu时钟不能对等，比如流的read，锁的lock等，这些虽然占时间但并不代表消耗相应时间的cpu，想更详细的了解cpu占用，可以利用perf，perf的缺点是jvm的黑盒，不过利用一些辅助工具，如https://github.com/jrudolph/perf-map-agent 结合perf可以打通os与jvm的性能监控，不过若需要跟踪详细的栈，需要JDK1.8的-XX:+PreserveFramePointer。
      ```

      估计 attach + ThreadMXBean.dumpAllThreads 吧

  - Java Mission Control (JMC) 

  - **Windows**  (Ctrl + Break)

- 内部触发（target java process）

  - `ThreadMXBean`，`getThreadInfo` or `dumpThreadDump`

    注意，`getThreadInfo`默认版本`maxDepth`为0表示不取stacktrace，需要显示指定。 而且该方法不能获取到monitor/sync信息（为空）

    ```java
    public void dumpThreadDump() {
      ThreadMXBean threadMxBean = ManagementFactory.getThreadMXBean();
      for (ThreadInfo ti : threadMxBean.dumpAllThreads(true, true)) {
        System.out.print(ti.toString());
      }
    }
    ```

    但：

    ```
    This is the worst option actually. Java-land representation of the stacktrace ThreadInfo[] requires a large amount of heap memory and takes much time to get constructed and printed. 
    ```

  - `Thread.dumpStack`

  - new a `Throwable` and invoke its `getStacktrace`



# 问题

ref：

- [open-source-jvm-sampling-profiler](http://insightfullogic.com/2014/Mar/02/open-source-jvm-sampling-profiler/)
- [Java CPU Sampling Using hprof](http://www.brendangregg.com/blog/2014-06-09/java-cpu-sampling-using-hprof.html)
- [Why Many Profilers have Serious Problems (More on Profiling with Signals)](http://jeremymanson.blogspot.co.id/2010/07/why-many-profilers-have-serious.html)
- [Profiling with JVMTI/JVMPI, SIGPROF and AsyncGetCallTrace](http://jeremymanson.blogspot.co.id/2007/05/profiling-with-jvmtijvmpi-sigprof-and.html)

提到几个问题



## 多数采样profiling方式都依赖安全点才能爬到栈

这样的话，**固定间隔采样**的前提就不成立了，采样间隔变成跟程序行为相关的...



还有个说法是**yield point**：

```
hprof only samples on yield points
```



- ThreadMXBean依赖safepoints
- JVMTI看起来（搜索到的）也依赖



### 为什么（只在）安全点（爬栈）会影响profile

除了影响profile interval均匀性以外，

Safe points are places in the code that the VM knows it can do a whole host of things - like initiate garbage collection - safely. The location of these safe points is determined by the JIT. It often puts them in places that aren't ideal for CPU profiling. For example, there may be a hot loop in your code that the JIT decides should not be interrupted by a safe point. If you use most standard profilers, this hot loop will never get profiled! As a result, **the placement of safe points affects the sampling quality of standard sampling profiling techniques**.

甚至

(Another interesting point, as made by Todd Mytkowicz and Amer Diwan of the University of Colorado: since JIT behavior really depends on everything in the system, and most profilers are part of the system, the decision about where to put a safe point will end up depending on which profiler you are using. This can make the results of the profilers clash violently: because of *their* differing behaviors, the safe points end up in different places, and the profilers end up tracking different places. See [Mytkowicz and Diwan's recent PLDI paper](http://www-plan.cs.colorado.edu/klipto/mytkowicz-pldi10.pdf) for details.)



### 应对

there is an undocumented JVMTI call in OpenJDK that allows you to get a stack trace from a running thread, regardless of the state of that thread. - **AsyncGetCallTrace**



In Unix-like systems, you can use a **SIGPROF signal** to call this function at (semi-)regular intervals, without having to do anything to your code.



Ref:

* [signals-and-java](http://jeremymanson.blogspot.co.id/2007/06/signals-and-java.html) - java

  ```java
  // Handles SIGHUP
  Signal.handle(new Signal("HUP"), new SignalHandler() {
    // Signal handler method
    public void handle(Signal signal) {
      System.out.println("Got signal" + signal);
    }
  });

  // You can raise a signal like this:

  Signal.raise(new Signal("HUP"));
  ```

  You can even ***register a native signal handler*** with the `Signal.handle0` method

  > 这样的话可以在java code里register SIGPROF handler来调JVMTI吧

  不过这个是deprecated，小心使用。 而且因为是`sun.misc.`下面，也不能做到多JVM/JDK通用。

* [more-about-profiling-with-sigprof](http://jeremymanson.blogspot.co.id/2007/06/more-about-profiling-with-sigprof.html) - cpp

  ```cpp
  // 样例

  struct sigaction {
    void (*sa_handler)(int);
    void (*sa_sigaction)(int, siginfo_t *, void *);
    sigset_t sa_mask;
    int sa_flags;
    void (*sa_restorer)(void);
  }

  void handler(int signal) {
    printf("received signal %d", signal);
  }

    struct sigaction sa;
    sa.sa_handler = &handler;
    sigemptyset(&sa.sa_mask);  // Oh, just look it up.
    struct sigaction old_handler;
    sigaction(SIGPROF, &sa, &old_handler);

    static struct itimerval timer;
    timer.it_interval.tv_sec = 1;  // number of seconds is obviously up to you
    timer.it_interval.tv_usec = 0;  // as is number of microseconds.
    timer.it_value = timer.it_interval;
    setitimer(ITIMER_PROF, &timer, NULL);
  ```

  ​

  ```
  So I wrote a C++ handler that called AsyncGetCallTrace, and loaded it in the Agent_OnLoad part of a JVMTI agent.
  ```

  ​





## java线程状态“不准” - runnable并不表示在运行

这样不能真实体现“消耗” CPU，而是“消耗”时间，如果期望达到前者，要结合其他手段获取线程准确的状态。

参见《java_thread.md》

