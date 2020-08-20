

* `JVM_ENTRY(void, JVM_Sleep(JNIEnv* env, jclass threadClass, jlong millis))`

  ```cpp

  ```

  ​


* `int os::sleep(Thread* thread, jlong millis, bool interruptible)`

  ```cpp
  int os::sleep(Thread* thread, jlong millis, bool interruptible) {
    assert(thread == Thread::current(),  "thread consistency check");

    ParkEvent * const slp = thread->_SleepEvent ;
    slp->reset() ;
    OrderAccess::fence() ;

    if (interruptible) {
      jlong prevtime = javaTimeNanos();

      for (;;) {
        if (os::is_interrupted(thread, true)) {
          return OS_INTRPT;
        }

        jlong newtime = javaTimeNanos();

        if (newtime - prevtime < 0) {
          // time moving backwards, should only happen if no monotonic clock
          // not a guarantee() because JVM should not abort on kernel/glibc bugs
          assert(!Linux::supports_monotonic_clock(), "time moving backwards");
        } else {
          millis -= (newtime - prevtime) / NANOSECS_PER_MILLISEC;
        }

        if(millis <= 0) {
          return OS_OK;
        }

        prevtime = newtime;

        {
          assert(thread->is_Java_thread(), "sanity check");
          JavaThread *jt = (JavaThread *) thread;
          ThreadBlockInVM tbivm(jt);
          OSThreadWaitState osts(jt->osthread(), false /* not Object.wait() */);

          jt->set_suspend_equivalent();
          // cleared by handle_special_suspend_equivalent_condition() or
          // java_suspend_self() via check_and_wait_while_suspended()

          slp->park(millis);

          // were we externally suspended while we were waiting?
          jt->check_and_wait_while_suspended();
        }
      }
    } else {
      OSThreadWaitState osts(thread->osthread(), false /* not Object.wait() */);
      jlong prevtime = javaTimeNanos();

      for (;;) {
        // It'd be nice to avoid the back-to-back javaTimeNanos() calls on
        // the 1st iteration ...
        jlong newtime = javaTimeNanos();

        if (newtime - prevtime < 0) {
          // time moving backwards, should only happen if no monotonic clock
          // not a guarantee() because JVM should not abort on kernel/glibc bugs
          assert(!Linux::supports_monotonic_clock(), "time moving backwards");
        } else {
          millis -= (newtime - prevtime) / NANOSECS_PER_MILLISEC;
        }

        if(millis <= 0) break ;

        prevtime = newtime;
        slp->park(millis);
      }
      return OS_OK ;
    }
  }
  ```


* `void Parker::park(bool isAbsolute, jlong time)`

  ```cpp
    // Enter safepoint region
    // Beware of deadlocks such as 6317397.
    // The per-thread Parker:: mutex is a classic leaf-lock.
    // In particular a thread must never block on the Threads_lock while
    // holding the Parker:: mutex.  If safepoints are pending both the
    // the ThreadBlockInVM() CTOR and DTOR may grab Threads_lock.
    ThreadBlockInVM tbivm(jt);
  ```


* `ThreadBlockInVM(JavaThread *thread)`

  ```cpp
  class ThreadBlockInVM : public ThreadStateTransition {
   public:
    ThreadBlockInVM(JavaThread *thread)
    : ThreadStateTransition(thread) {
      // Once we are blocked vm expects stack to be walkable
      thread->frame_anchor()->make_walkable(thread);
      trans_and_fence(_thread_in_vm, _thread_blocked);
    }
    ~ThreadBlockInVM() {
      trans_and_fence(_thread_blocked, _thread_in_vm);
      // We don't need to clear_walkable because it will happen automagically when we return to java
    }
  };
  ```


* `void trans_and_fence(JavaThreadState from, JavaThreadState to)`

  ```cpp
  void trans_and_fence(JavaThreadState from, JavaThreadState to) { transition_and_fence(_thread, from, to); }
  ```

* `void transition_and_fence(JavaThread *thread, JavaThreadState from, JavaThreadState to)`

  ```cpp
    // transition_and_fence must be used on any thread state transition
    // where there might not be a Java call stub on the stack, in
    // particular on Windows where the Structured Exception Handler is
    // set up in the call stub. os::write_memory_serialize_page() can
    // fault and we can't recover from it on Windows without a SEH in
    // place.
    static inline void transition_and_fence(JavaThread *thread, JavaThreadState from, JavaThreadState to) {
      assert(thread->thread_state() == from, "coming from wrong thread state");
      assert((from & 1) == 0 && (to & 1) == 0, "odd numbers are transitions states");
      // Change to transition state (assumes total store ordering!  -Urs)
      thread->set_thread_state((JavaThreadState)(from + 1));

      // Make sure new state is seen by VM thread
      if (os::is_MP()) {
        if (UseMembar) {
          // Force a fence between the write above and read below
          OrderAccess::fence();
        } else {
          // Must use this rather than serialization page in particular on Windows
          InterfaceSupport::serialize_memory(thread);
        }
      }

      if (SafepointSynchronize::do_call_back()) {
        SafepointSynchronize::block(thread);
      }
      thread->set_thread_state(to);

      CHECK_UNHANDLED_OOPS_ONLY(thread->clear_unhandled_oops();)
    }
  ```

  ​





