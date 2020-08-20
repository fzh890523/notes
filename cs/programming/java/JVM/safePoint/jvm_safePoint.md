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





#### into source



```cpp
  enum SynchronizeState {
      _not_synchronized = 0,                   // Threads not synchronized at a safepoint
                                               // Keep this value 0. See the coment in do_call_back()
      _synchronizing    = 1,                   // Synchronizing in progress
      _synchronized     = 2                    // All Java threads are stopped at a safepoint. Only VM thread is running
  };

  enum SafepointingThread {
      _null_thread  = 0,
      _vm_thread    = 1,
      _other_thread = 2
  };

  enum SafepointTimeoutReason {
    _spinning_timeout = 0,
    _blocking_timeout = 1
  };

// State class for a thread suspended at a safepoint
class ThreadSafepointState: public CHeapObj<mtInternal> {
 public:
  // These states are maintained by VM thread while threads are being brought
  // to a safepoint.  After SafepointSynchronize::end(), they are reset to
  // _running.
  enum suspend_type {
    _running                =  0, // Thread state not yet determined (i.e., not at a safepoint yet)
    _at_safepoint           =  1, // Thread at a safepoint (f.ex., when blocked on a lock)
    _call_back              =  2  // Keep executing and wait for callback (if thread is in interpreted or vm)
  };
 private:
  volatile bool _at_poll_safepoint;  // At polling page safepoint (NOT a poll return safepoint)
  // Thread has called back the safepoint code (for debugging)
  bool                           _has_called_back;

  JavaThread *                   _thread;
  volatile suspend_type          _type;
  JavaThreadState                _orig_thread_state;
  
  // ...
  
}
```









##### 进入安全点、设置、退出



```cpp
// Roll all threads forward to a safepoint and suspend them all
void SafepointSynchronize::begin() {

  Thread* myThread = Thread::current();
  assert(myThread->is_VM_thread(), "Only VM thread may execute a safepoint");

  if (PrintSafepointStatistics || PrintSafepointStatisticsTimeout > 0) {
    _safepoint_begin_time = os::javaTimeNanos();
    _ts_of_current_safepoint = tty->time_stamp().seconds();
  }

#if INCLUDE_ALL_GCS
  if (UseConcMarkSweepGC) {
    // In the future we should investigate whether CMS can use the
    // more-general mechanism below.  DLD (01/05).
    ConcurrentMarkSweepThread::synchronize(false);
  } else if (UseG1GC) {
    SuspendibleThreadSet::synchronize();
  }
#endif // INCLUDE_ALL_GCS

  // By getting the Threads_lock, we assure that no threads are about to start or
  // exit. It is released again in SafepointSynchronize::end().
  Threads_lock->lock();

  assert( _state == _not_synchronized, "trying to safepoint synchronize with wrong state");

  int nof_threads = Threads::number_of_threads();

  if (TraceSafepoint) {
    tty->print_cr("Safepoint synchronization initiated. (%d)", nof_threads);
  }

  RuntimeService::record_safepoint_begin();

  MutexLocker mu(Safepoint_lock);

  // Reset the count of active JNI critical threads
  _current_jni_active_count = 0;

  // Set number of threads to wait for, before we initiate the callbacks
  _waiting_to_block = nof_threads;
  TryingToBlock     = 0 ;
  int still_running = nof_threads;

  // Save the starting time, so that it can be compared to see if this has taken
  // too long to complete.
  jlong safepoint_limit_time;
  timeout_error_printed = false;

  // PrintSafepointStatisticsTimeout can be specified separately. When
  // specified, PrintSafepointStatistics will be set to true in
  // deferred_initialize_stat method. The initialization has to be done
  // early enough to avoid any races. See bug 6880029 for details.
  if (PrintSafepointStatistics || PrintSafepointStatisticsTimeout > 0) {
    deferred_initialize_stat();
  }

  // Begin the process of bringing the system to a safepoint.
  // Java threads can be in several different states and are
  // stopped by different mechanisms:
  //
  //  1. Running interpreted
  //     The interpeter dispatch table is changed to force it to
  //     check for a safepoint condition between bytecodes.
  //  2. Running in native code
  //     When returning from the native code, a Java thread must check
  //     the safepoint _state to see if we must block.  If the
  //     VM thread sees a Java thread in native, it does
  //     not wait for this thread to block.  The order of the memory
  //     writes and reads of both the safepoint state and the Java
  //     threads state is critical.  In order to guarantee that the
  //     memory writes are serialized with respect to each other,
  //     the VM thread issues a memory barrier instruction
  //     (on MP systems).  In order to avoid the overhead of issuing
  //     a memory barrier for each Java thread making native calls, each Java
  //     thread performs a write to a single memory page after changing
  //     the thread state.  The VM thread performs a sequence of
  //     mprotect OS calls which forces all previous writes from all
  //     Java threads to be serialized.  This is done in the
  //     os::serialize_thread_states() call.  This has proven to be
  //     much more efficient than executing a membar instruction
  //     on every call to native code.
  //  3. Running compiled Code
  //     Compiled code reads a global (Safepoint Polling) page that
  //     is set to fault if we are trying to get to a safepoint.
  //  4. Blocked
  //     A thread which is blocked will not be allowed to return from the
  //     block condition until the safepoint operation is complete.
  //  5. In VM or Transitioning between states
  //     If a Java thread is currently running in the VM or transitioning
  //     between states, the safepointing code will wait for the thread to
  //     block itself when it attempts transitions to a new state.
  //
  _state            = _synchronizing;
  OrderAccess::fence();

  // Flush all thread states to memory
  if (!UseMembar) {
    os::serialize_thread_states();
  }

  // Make interpreter safepoint aware
  Interpreter::notice_safepoints();

  if (UseCompilerSafepoints && DeferPollingPageLoopCount < 0) {
    // Make polling safepoint aware
    guarantee (PageArmed == 0, "invariant") ;
    PageArmed = 1 ;
    os::make_polling_page_unreadable();
  }

  // Consider using active_processor_count() ... but that call is expensive.
  int ncpus = os::processor_count() ;

#ifdef ASSERT
  for (JavaThread *cur = Threads::first(); cur != NULL; cur = cur->next()) {
    assert(cur->safepoint_state()->is_running(), "Illegal initial state");
    // Clear the visited flag to ensure that the critical counts are collected properly.
    cur->set_visited_for_critical_count(false);
  }
#endif // ASSERT

  if (SafepointTimeout)
    safepoint_limit_time = os::javaTimeNanos() + (jlong)SafepointTimeoutDelay * MICROUNITS;

  // Iterate through all threads until it have been determined how to stop them all at a safepoint
  unsigned int iterations = 0;
  int steps = 0 ;
  while(still_running > 0) {
    for (JavaThread *cur = Threads::first(); cur != NULL; cur = cur->next()) {
      assert(!cur->is_ConcurrentGC_thread(), "A concurrent GC thread is unexpectly being suspended");
      ThreadSafepointState *cur_state = cur->safepoint_state();
      if (cur_state->is_running()) {
        cur_state->examine_state_of_thread();
        if (!cur_state->is_running()) {
           still_running--;
           // consider adjusting steps downward:
           //   steps = 0
           //   steps -= NNN
           //   steps >>= 1
           //   steps = MIN(steps, 2000-100)
           //   if (iterations != 0) steps -= NNN
        }
        if (TraceSafepoint && Verbose) cur_state->print();
      }
    }

    if (PrintSafepointStatistics && iterations == 0) {
      begin_statistics(nof_threads, still_running);
    }

    if (still_running > 0) {
      // Check for if it takes to long
      if (SafepointTimeout && safepoint_limit_time < os::javaTimeNanos()) {
        print_safepoint_timeout(_spinning_timeout);
      }

      // Spin to avoid context switching.
      // There's a tension between allowing the mutators to run (and rendezvous)
      // vs spinning.  As the VM thread spins, wasting cycles, it consumes CPU that
      // a mutator might otherwise use profitably to reach a safepoint.  Excessive
      // spinning by the VM thread on a saturated system can increase rendezvous latency.
      // Blocking or yielding incur their own penalties in the form of context switching
      // and the resultant loss of $ residency.
      //
      // Further complicating matters is that yield() does not work as naively expected
      // on many platforms -- yield() does not guarantee that any other ready threads
      // will run.   As such we revert yield_all() after some number of iterations.
      // Yield_all() is implemented as a short unconditional sleep on some platforms.
      // Typical operating systems round a "short" sleep period up to 10 msecs, so sleeping
      // can actually increase the time it takes the VM thread to detect that a system-wide
      // stop-the-world safepoint has been reached.  In a pathological scenario such as that
      // described in CR6415670 the VMthread may sleep just before the mutator(s) become safe.
      // In that case the mutators will be stalled waiting for the safepoint to complete and the
      // the VMthread will be sleeping, waiting for the mutators to rendezvous.  The VMthread
      // will eventually wake up and detect that all mutators are safe, at which point
      // we'll again make progress.
      //
      // Beware too that that the VMThread typically runs at elevated priority.
      // Its default priority is higher than the default mutator priority.
      // Obviously, this complicates spinning.
      //
      // Note too that on Windows XP SwitchThreadTo() has quite different behavior than Sleep(0).
      // Sleep(0) will _not yield to lower priority threads, while SwitchThreadTo() will.
      //
      // See the comments in synchronizer.cpp for additional remarks on spinning.
      //
      // In the future we might:
      // 1. Modify the safepoint scheme to avoid potentally unbounded spinning.
      //    This is tricky as the path used by a thread exiting the JVM (say on
      //    on JNI call-out) simply stores into its state field.  The burden
      //    is placed on the VM thread, which must poll (spin).
      // 2. Find something useful to do while spinning.  If the safepoint is GC-related
      //    we might aggressively scan the stacks of threads that are already safe.
      // 3. Use Solaris schedctl to examine the state of the still-running mutators.
      //    If all the mutators are ONPROC there's no reason to sleep or yield.
      // 4. YieldTo() any still-running mutators that are ready but OFFPROC.
      // 5. Check system saturation.  If the system is not fully saturated then
      //    simply spin and avoid sleep/yield.
      // 6. As still-running mutators rendezvous they could unpark the sleeping
      //    VMthread.  This works well for still-running mutators that become
      //    safe.  The VMthread must still poll for mutators that call-out.
      // 7. Drive the policy on time-since-begin instead of iterations.
      // 8. Consider making the spin duration a function of the # of CPUs:
      //    Spin = (((ncpus-1) * M) + K) + F(still_running)
      //    Alternately, instead of counting iterations of the outer loop
      //    we could count the # of threads visited in the inner loop, above.
      // 9. On windows consider using the return value from SwitchThreadTo()
      //    to drive subsequent spin/SwitchThreadTo()/Sleep(N) decisions.

      if (UseCompilerSafepoints && int(iterations) == DeferPollingPageLoopCount) {
         guarantee (PageArmed == 0, "invariant") ;
         PageArmed = 1 ;
         os::make_polling_page_unreadable();
      }

      // Instead of (ncpus > 1) consider either (still_running < (ncpus + EPSILON)) or
      // ((still_running + _waiting_to_block - TryingToBlock)) < ncpus)
      ++steps ;
      if (ncpus > 1 && steps < SafepointSpinBeforeYield) {
        SpinPause() ;     // MP-Polite spin
      } else
      if (steps < DeferThrSuspendLoopCount) {
        os::NakedYield() ;
      } else {
        os::yield_all(steps) ;
        // Alternately, the VM thread could transiently depress its scheduling priority or
        // transiently increase the priority of the tardy mutator(s).
      }

      iterations ++ ;
    }
    assert(iterations < (uint)max_jint, "We have been iterating in the safepoint loop too long");
  }
  assert(still_running == 0, "sanity check");

  if (PrintSafepointStatistics) {
    update_statistics_on_spin_end();
  }

  // wait until all threads are stopped
  while (_waiting_to_block > 0) {
    if (TraceSafepoint) tty->print_cr("Waiting for %d thread(s) to block", _waiting_to_block);
    if (!SafepointTimeout || timeout_error_printed) {
      Safepoint_lock->wait(true);  // true, means with no safepoint checks
    } else {
      // Compute remaining time
      jlong remaining_time = safepoint_limit_time - os::javaTimeNanos();

      // If there is no remaining time, then there is an error
      if (remaining_time < 0 || Safepoint_lock->wait(true, remaining_time / MICROUNITS)) {
        print_safepoint_timeout(_blocking_timeout);
      }
    }
  }
  assert(_waiting_to_block == 0, "sanity check");

#ifndef PRODUCT
  if (SafepointTimeout) {
    jlong current_time = os::javaTimeNanos();
    if (safepoint_limit_time < current_time) {
      tty->print_cr("# SafepointSynchronize: Finished after "
                    INT64_FORMAT_W(6) " ms",
                    ((current_time - safepoint_limit_time) / MICROUNITS +
                     SafepointTimeoutDelay));
    }
  }
#endif

  assert((_safepoint_counter & 0x1) == 0, "must be even");
  assert(Threads_lock->owned_by_self(), "must hold Threads_lock");
  _safepoint_counter ++;

  // Record state
  _state = _synchronized;

  OrderAccess::fence();

#ifdef ASSERT
  for (JavaThread *cur = Threads::first(); cur != NULL; cur = cur->next()) {
    // make sure all the threads were visited
    assert(cur->was_visited_for_critical_count(), "missed a thread");
  }
#endif // ASSERT

  // Update the count of active JNI critical regions
  GC_locker::set_jni_lock_count(_current_jni_active_count);

  if (TraceSafepoint) {
    VM_Operation *op = VMThread::vm_operation();
    tty->print_cr("Entering safepoint region: %s", (op != NULL) ? op->name() : "no vm operation");
  }

  RuntimeService::record_safepoint_synchronized();
  if (PrintSafepointStatistics) {
    update_statistics_on_sync_end(os::javaTimeNanos());
  }

  // Call stuff that needs to be run when a safepoint is just about to be completed
  do_cleanup_tasks();

  if (PrintSafepointStatistics) {
    // Record how much time spend on the above cleanup tasks
    update_statistics_on_cleanup_end(os::javaTimeNanos());
  }
}
```



```cpp
// Wake up all threads, so they are ready to resume execution after the safepoint
// operation has been carried out
void SafepointSynchronize::end() {

  assert(Threads_lock->owned_by_self(), "must hold Threads_lock");
  assert((_safepoint_counter & 0x1) == 1, "must be odd");
  _safepoint_counter ++;
  // memory fence isn't required here since an odd _safepoint_counter
  // value can do no harm and a fence is issued below anyway.

  DEBUG_ONLY(Thread* myThread = Thread::current();)
  assert(myThread->is_VM_thread(), "Only VM thread can execute a safepoint");

  if (PrintSafepointStatistics) {
    end_statistics(os::javaTimeNanos());
  }

#ifdef ASSERT
  // A pending_exception cannot be installed during a safepoint.  The threads
  // may install an async exception after they come back from a safepoint into
  // pending_exception after they unblock.  But that should happen later.
  for(JavaThread *cur = Threads::first(); cur; cur = cur->next()) {
    assert (!(cur->has_pending_exception() &&
              cur->safepoint_state()->is_at_poll_safepoint()),
            "safepoint installed a pending exception");
  }
#endif // ASSERT

  if (PageArmed) {
    // Make polling safepoint aware
    os::make_polling_page_readable();
    PageArmed = 0 ;
  }

  // Remove safepoint check from interpreter
  Interpreter::ignore_safepoints();

  {
    MutexLocker mu(Safepoint_lock);

    assert(_state == _synchronized, "must be synchronized before ending safepoint synchronization");

    // Set to not synchronized, so the threads will not go into the signal_thread_blocked method
    // when they get restarted.
    _state = _not_synchronized;
    OrderAccess::fence();

    if (TraceSafepoint) {
       tty->print_cr("Leaving safepoint region");
    }

    // Start suspended threads
    for(JavaThread *current = Threads::first(); current; current = current->next()) {
      // A problem occurring on Solaris is when attempting to restart threads
      // the first #cpus - 1 go well, but then the VMThread is preempted when we get
      // to the next one (since it has been running the longest).  We then have
      // to wait for a cpu to become available before we can continue restarting
      // threads.
      // FIXME: This causes the performance of the VM to degrade when active and with
      // large numbers of threads.  Apparently this is due to the synchronous nature
      // of suspending threads.
      //
      // TODO-FIXME: the comments above are vestigial and no longer apply.
      // Furthermore, using solaris' schedctl in this particular context confers no benefit
      if (VMThreadHintNoPreempt) {
        os::hint_no_preempt();
      }
      ThreadSafepointState* cur_state = current->safepoint_state();
      assert(cur_state->type() != ThreadSafepointState::_running, "Thread not suspended at safepoint");
      cur_state->restart();
      assert(cur_state->is_running(), "safepoint state has not been reset");
    }

    RuntimeService::record_safepoint_end();

    // Release threads lock, so threads can be created/destroyed again. It will also starts all threads
    // blocked in signal_thread_blocked
    Threads_lock->unlock();

  }
#if INCLUDE_ALL_GCS
  // If there are any concurrent GC threads resume them.
  if (UseConcMarkSweepGC) {
    ConcurrentMarkSweepThread::desynchronize(false);
  } else if (UseG1GC) {
    SuspendibleThreadSet::desynchronize();
  }
#endif // INCLUDE_ALL_GCS
  // record this time so VMThread can keep track how much time has elasped
  // since last safepoint.
  _end_of_last_safepoint = os::javaTimeMillis();
}
```



```cpp
// os.cpp

// Serialize all thread state variables
void os::serialize_thread_states() {
  // On some platforms such as Solaris & Linux, the time duration of the page
  // permission restoration is observed to be much longer than expected  due to
  // scheduler starvation problem etc. To avoid the long synchronization
  // time and expensive page trap spinning, 'SerializePageLock' is used to block
  // the mutator thread if such case is encountered. See bug 6546278 for details.
  Thread::muxAcquire(&SerializePageLock, "serialize_thread_states");
  os::protect_memory((char *)os::get_memory_serialize_page(),
                     os::vm_page_size(), MEM_PROT_READ);
  os::protect_memory((char *)os::get_memory_serialize_page(),
                     os::vm_page_size(), MEM_PROT_RW);
  Thread::muxRelease(&SerializePageLock);
}
```



##### stuck in safepoint



* `void ThreadSafepointState::handle_polling_page_exception()`(`safepoint.cpp`)

  > // Block the thread at the safepoint poll or poll return.

* `void SafepointSynchronize::handle_polling_page_exception(JavaThread *thread)`(`safepoint.cpp`)

* `void SharedRuntime::generate_stubs()`(`sharedRuntime.cpp`)

  ```cpp
  #ifdef COMPILER2
    // Vectors are generated only by C2.
    if (is_wide_vector(MaxVectorSize)) {
      _polling_page_vectors_safepoint_handler_blob = generate_handler_blob(CAST_FROM_FN_PTR(address, SafepointSynchronize::handle_polling_page_exception), POLL_AT_VECTOR_LOOP);
    }
  #endif // COMPILER2
    _polling_page_safepoint_handler_blob = generate_handler_blob(CAST_FROM_FN_PTR(address, SafepointSynchronize::handle_polling_page_exception), POLL_AT_LOOP);
    _polling_page_return_handler_blob    = generate_handler_blob(CAST_FROM_FN_PTR(address, SafepointSynchronize::handle_polling_page_exception), POLL_AT_RETURN);
  ```

  ​



* `_polling_page_safepoint_handler_blob`(`sharedRuntime.hpp`)

* `SafepointBlob* polling_page_safepoint_handler_blob()`(`sharedRuntime.hpp`)

* `address SharedRuntime::get_poll_stub(address pc)`(`sharedRuntime.cpp`)

* `JVM_handle_linux_signal(int sig, siginfo_t* info, void* ucVoid, int abort_if_unrecognized)`(`os_linux_x86.cpp`)

  > 对于linux

  ```cpp
  // os_linux_x86.cpp

      if (thread->thread_state() == _thread_in_Java) {
        // Java thread running in Java code => find exception handler if any
        // a fault inside compiled code, the interpreter, or a stub

        if (sig == SIGSEGV && os::is_poll_address((address)info->si_addr)) {
          stub = SharedRuntime::get_poll_stub(pc);
          // ...
      // Check to see if we caught the safepoint code in the
      // process of write protecting the memory serialization page.
      // It write enables the page immediately after protecting it
      // so we can just return to retry the write.
      if ((sig == SIGSEGV) &&
          os::is_memory_serialize_page(thread, (address) info->si_addr)) {
        // Block current thread until the memory serialize page permission restored.
        os::block_on_serialize_page_trap();
        return true;
      } 
      // ...
  ```

* `void signalHandler(int sig, siginfo_t* info, void* uc)`(`os_linux.cpp`)



* `void SafepointSynchronize::block(JavaThread *thread)`

  * `void JavaThread::check_safepoint_and_suspend_for_native_trans(JavaThread *thread)`

    ```cpp
      if (SafepointSynchronize::do_call_back()) {
        // If we are safepointing, then block the caller which may not be
        // the same as the target thread (see above).
        SafepointSynchronize::block(curJT);
      }
    ```

  * `void ThreadSafepointState::handle_polling_page_exception()`

  * `static inline void transition(JavaThread *thread, JavaThreadState from, JavaThreadState to)`

    ```cpp
        if (SafepointSynchronize::do_call_back()) {
          SafepointSynchronize::block(thread);
        }
        thread->set_thread_state(to);
    ```

  * `static inline void transition_and_fence(JavaThread *thread, JavaThreadState from, JavaThreadState to)`





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




1. `get_memory_serialize_page`(`os.hpp`)
2. .
   * `serialize_memory`(`MacroAssembler_x86.cpp`)
     3. .
        * `InterpreterGenerator::generate_native_entry`(`cppInterpreter_x86.cpp`)
        * ...
   * `serialize_thread_states`(`os.cpp`)
     3. `SafepointSynchronize::begin`(`safepoint.cpp`)



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









