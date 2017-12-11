

# 状态定义



```cpp
// JavaThreadState keeps track of which part of the code a thread is executing in. This
// information is needed by the safepoint code.
//
// There are 4 essential states:
//
//  _thread_new         : Just started, but not executed init. code yet (most likely still in OS init code)
//  _thread_in_native   : In native code. This is a safepoint region, since all oops will be in jobject handles
//  _thread_in_vm       : Executing in the vm
//  _thread_in_Java     : Executing either interpreted or compiled Java code (or could be in a stub)
//
// Each state has an associated xxxx_trans state, which is an intermediate state used when a thread is in
// a transition from one state to another. These extra states makes it possible for the safepoint code to
// handle certain thread_states without having to suspend the thread - making the safepoint code faster.
//
// Given a state, the xxx_trans state can always be found by adding 1.
//
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





# 状态迁移



要点：

> 负责GC等vm操作的VM线程需要“看”到线程的准确状态，也即对该数据的访问需要是同步的





```cpp
// thread.hpp

  // Safepoint support
#ifndef PPC64
  JavaThreadState thread_state() const           { return _thread_state; }
  void set_thread_state(JavaThreadState s)       { _thread_state = s;    }
#else
  // Use membars when accessing volatile _thread_state. See
  // Threads::create_vm() for size checks.
  inline JavaThreadState thread_state() const;
  inline void set_thread_state(JavaThreadState s);
#endif

// thread.inline.hpp

#ifdef PPC64
inline JavaThreadState JavaThread::thread_state() const    {
  return (JavaThreadState) OrderAccess::load_acquire((volatile jint*)&_thread_state);
}

inline void JavaThread::set_thread_state(JavaThreadState s) {
  OrderAccess::release_store((volatile jint*)&_thread_state, (jint)s);
}
#endif
```



* 对于membar情况：
  * volatile使得编译器不重排、并且写回mem而不是register（实际WB mem写不写回另说）
  * ​





## 场景







### write场景

`void set_thread_state(JavaThreadState s)`



- attach_current_thread(`jni.cpp`)
- `void SafepointSynchronize::block(JavaThread *thread)`
- `void JavaThread::check_safepoint_and_suspend_for_native_trans(JavaThread *thread)`
- `void JavaThread::check_special_condition_for_native_trans_and_transition(JavaThread *thread)`
- `jint Threads::create_vm(JavaVMInitArgs* args, bool* canTryAgain)`
- `void JavaThread::initialize()`
- `void JvmtiExport::post_class_unload(Klass* klass)`
- `static inline void transition(JavaThread *thread, JavaThreadState from, JavaThreadState to)`
- `static inline void transition_and_fence(JavaThread *thread, JavaThreadState from, JavaThreadState to)`
- `static inline void transition_from_java(JavaThread *thread, JavaThreadState to)`
- `static inline void transition_from_native(JavaThread *thread, JavaThreadState to)`
- `void vm_exit(int code)`
- `void vm_perform_shutdown_actions()`





### read场景

`JavaThreadState thread_state() const`



- `void AsyncGetCallTrace(ASGCT_CallTrace *trace, jint depth, void* ucontext)`
- `VM_handle_<arch>_signal`
- `JvmtiThreadEventTransition(Thread *thread) : _rm(), _hm()`
- `ThreadSnapshot::ThreadSnapshot(JavaThread* thread)`
- `void SafepointSynchronize::block(JavaThread *thread)`
- `void JavaThread::check_and_handle_async_exceptions(bool check_unsafe_error)`
- `void JavaThread::check_and_handle_async_exceptions(bool check_unsafe_error)`
- `void ThreadSafepointState::examine_state_of_thread()`
- `void VM_GetMultipleStackTraces::fill_frames(jthread jt, JavaThread *thr, oop thread_oop)`
- `inline bool vframeStreamCommon::fill_from_frame()`
- `static bool is_decipherable_interpreted_frame(JavaThread* thread, frame* fr, Method** method_p, int* bci_p)`
- `bool JavaThread::is_ext_suspend_completed(bool called_by_wait, int delay, uint32_t *bits)`
- `bool ciEnv::is_in_vm()`
- `void JvmtiExport::post_class_unload(Klass* klass)`
- `void SafepointSynchronize::print_safepoint_timeout(SafepointTimeoutReason reason)`
- `int JvmtiRawMonitor::raw_enter(TRAPS)`
- `int JvmtiRawMonitor::raw_wait(jlong millis, bool interruptible, TRAPS)`
- `void ThreadProfiler::record_tick(JavaThread* thread)`
- `int VM_Exit::set_vm_exited()`
- `int VM_Exit::wait_for_threads_in_native_to_block()`





## 状态同步方式





### writer - java线程



```cpp
// interfaceSupport.hpp
// transition_and_fence and transition_from_native

    // Change to transition state (assumes total store ordering!  -Urs)
    thread->set_thread_state(_thread_in_native_trans);

    // Make sure new state is seen by VM thread (对于transition_from_native是 by GC thread)
    if (os::is_MP()) {
      if (UseMembar) {
        // Force a fence between the write above and read below
        OrderAccess::fence();
      } else {
        // Must use this rather than serialization page in particular on Windows
        InterfaceSupport::serialize_memory(thread);
      }
    }

// thead.cpp
// check_safepoint_and_suspend_for_native_trans
//... 同上
```





```cpp
inline void OrderAccess::fence() {
  if (os::is_MP()) {
    // always use locked addl since mfence is sometimes expensive
#ifdef AMD64
    __asm__ volatile ("lock; addl $0,0(%%rsp)" : : : "cc", "memory");
#else
    __asm__ volatile ("lock; addl $0,0(%%esp)" : : : "cc", "memory");
#endif
  }
}
```

> from [Volatile从入门到放弃](http://blog.csdn.net/w329636271/article/details/54616543)
>
> `cc代表的是寄存器,memory代表是内存;这边同时用了”cc”和”memory”,来通知编译器内存或者寄存器内的内容已经发生了修改,要重新生成加载指令(不可以从缓存寄存器中取).`
>
> `read/write请求不能越过lock指令进行重排,那么所有带有lock prefix指令(lock ,xchgl等)都会构成一个天然的x86 Mfence(读写屏障),这里用lock指令作为内存屏障,然后利用asm volatile("" ::: "cc,memory")作为编译器屏障`

> \#yonka\# 即便如上，这里是怎么保证**状态同步**的？



```cpp
// interfaceSupport_linux.cpp

static inline void serialize_memory(JavaThread *thread) {
  os::write_memory_serialize_page(thread);
}

// os.hpp

  static inline void write_memory_serialize_page(JavaThread *thread) {
    uintptr_t page_offset = ((uintptr_t)thread >>
                            get_serialize_page_shift_count()) &
                            get_serialize_page_mask();
    *(volatile int32_t *)((uintptr_t)_mem_serialize_page+page_offset) = 1;
  }
```



```cpp
// os_linux_x86.cpp

// JVM_handle_linux_signal
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
```





```cpp
// This method is called from signal handler when SIGSEGV occurs while the current
// thread tries to store to the "read-only" memory serialize page during state
// transition.
void os::block_on_serialize_page_trap() {
  if (TraceSafepoint) {
    tty->print_cr("Block until the serialize page permission restored");
  }
  // When VMThread is holding the SerializePageLock during modifying the
  // access permission of the memory serialize page, the following call
  // will block until the permission of that page is restored to rw.
  // Generally, it is unsafe to manipulate locks in signal handlers, but in
  // this case, it's OK as the signal is synchronous and we know precisely when
  // it can occur.
  Thread::muxAcquire(&SerializePageLock, "set_memory_serialize_page");
  Thread::muxRelease(&SerializePageLock);
}
```



#### 结论

java线程



### reader - vm线程





```cpp
// safepoint.cpp

// SafepointSynchronize::begin
  // Flush all thread states to memory
  if (!UseMembar) {
    os::serialize_thread_states();
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
  // TODO 为什么要先禁read再禁read&write呢？
  Thread::muxRelease(&SerializePageLock);
}

// os_linux.cpp

// Set protections specified
bool os::protect_memory(char* addr, size_t bytes, ProtType prot,
                        bool is_committed) {
  unsigned int p = 0;
  switch (prot) {
  case MEM_PROT_NONE: p = PROT_NONE; break;
  case MEM_PROT_READ: p = PROT_READ; break;
  case MEM_PROT_RW:   p = PROT_READ|PROT_WRITE; break;
  case MEM_PROT_RWX:  p = PROT_READ|PROT_WRITE|PROT_EXEC; break;
  default:
    ShouldNotReachHere();
  }
  // is_committed is unused.
  return linux_mprotect(addr, bytes, p);
}
```



from `man 2 mprotect`

> If the calling process tries to access memory in a manner that violates the protection, then the kernel generates a **SIGSEGV** signal for the process.



## source code



```cpp
// Basic class for all thread transition classes.

class ThreadStateTransition : public StackObj {
 protected:
  JavaThread* _thread;
 public:
  ThreadStateTransition(JavaThread *thread) {
    _thread = thread;
    assert(thread != NULL && thread->is_Java_thread(), "must be Java thread");
  }

  // Change threadstate in a manner, so safepoint can detect changes.
  // Time-critical: called on exit from every runtime routine
  static inline void transition(JavaThread *thread, JavaThreadState from, JavaThreadState to) {
    assert(from != _thread_in_Java, "use transition_from_java");
    assert(from != _thread_in_native, "use transition_from_native");
    assert((from & 1) == 0 && (to & 1) == 0, "odd numbers are transitions states");
    assert(thread->thread_state() == from, "coming from wrong thread state");
    // Change to transition state (assumes total store ordering!  -Urs)
    thread->set_thread_state((JavaThreadState)(from + 1));

    // Make sure new state is seen by VM thread
    if (os::is_MP()) {
      if (UseMembar) {
        // Force a fence between the write above and read below
        OrderAccess::fence();
      } else {
        // store to serialize page so VM thread can do pseudo remote membar
        os::write_memory_serialize_page(thread);
      }
    }

    if (SafepointSynchronize::do_call_back()) {
      SafepointSynchronize::block(thread);
    }
    thread->set_thread_state(to);

    CHECK_UNHANDLED_OOPS_ONLY(thread->clear_unhandled_oops();)
  }

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

  // Same as above, but assumes from = _thread_in_Java. This is simpler, since we
  // never block on entry to the VM. This will break the code, since e.g. preserve arguments
  // have not been setup.
  static inline void transition_from_java(JavaThread *thread, JavaThreadState to) {
    assert(thread->thread_state() == _thread_in_Java, "coming from wrong thread state");
    thread->set_thread_state(to);
  }

  static inline void transition_from_native(JavaThread *thread, JavaThreadState to) {
    assert((to & 1) == 0, "odd numbers are transitions states");
    assert(thread->thread_state() == _thread_in_native, "coming from wrong thread state");
    // Change to transition state (assumes total store ordering!  -Urs)
    thread->set_thread_state(_thread_in_native_trans);

    // Make sure new state is seen by GC thread
    if (os::is_MP()) {
      if (UseMembar) {
        // Force a fence between the write above and read below
        OrderAccess::fence();
      } else {
        // Must use this rather than serialization page in particular on Windows
        InterfaceSupport::serialize_memory(thread);
      }
    }

    // We never install asynchronous exceptions when coming (back) in
    // to the runtime from native code because the runtime is not set
    // up to handle exceptions floating around at arbitrary points.
    if (SafepointSynchronize::do_call_back() || thread->is_suspend_after_native()) {
      JavaThread::check_safepoint_and_suspend_for_native_trans(thread);

      // Clear unhandled oops anywhere where we could block, even if we don't.
      CHECK_UNHANDLED_OOPS_ONLY(thread->clear_unhandled_oops();)
    }

    thread->set_thread_state(to);
  }
 protected:
   void trans(JavaThreadState from, JavaThreadState to)  { transition(_thread, from, to); }
   void trans_from_java(JavaThreadState to)              { transition_from_java(_thread, to); }
   void trans_from_native(JavaThreadState to)            { transition_from_native(_thread, to); }
   void trans_and_fence(JavaThreadState from, JavaThreadState to) { transition_and_fence(_thread, from, to); }
};
```



```cpp
class ThreadInVMfromJava : public ThreadStateTransition {
 public:
  ThreadInVMfromJava(JavaThread* thread) : ThreadStateTransition(thread) {
    trans_from_java(_thread_in_vm);
  }
  ~ThreadInVMfromJava()  {
    trans(_thread_in_vm, _thread_in_Java);
    // Check for pending. async. exceptions or suspends.
    if (_thread->has_special_runtime_exit_condition()) _thread->handle_special_runtime_exit_condition();
  }
};

class ThreadInVMfromNative : public ThreadStateTransition {
 public:
  ThreadInVMfromNative(JavaThread* thread) : ThreadStateTransition(thread) {
    trans_from_native(_thread_in_vm);
  }
  ~ThreadInVMfromNative() {
    trans_and_fence(_thread_in_vm, _thread_in_native);
  }
};


class ThreadToNativeFromVM : public ThreadStateTransition {
 public:
  ThreadToNativeFromVM(JavaThread *thread) : ThreadStateTransition(thread) {
    // We are leaving the VM at this point and going directly to native code.
    // Block, if we are in the middle of a safepoint synchronization.
    assert(!thread->owns_locks(), "must release all locks when leaving VM");
    thread->frame_anchor()->make_walkable(thread);
    trans_and_fence(_thread_in_vm, _thread_in_native);
    // Check for pending. async. exceptions or suspends.
    if (_thread->has_special_runtime_exit_condition()) _thread->handle_special_runtime_exit_condition(false);
  }

  ~ThreadToNativeFromVM() {
    trans_from_native(_thread_in_vm);
    // We don't need to clear_walkable because it will happen automagically when we return to java
  }
};


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



