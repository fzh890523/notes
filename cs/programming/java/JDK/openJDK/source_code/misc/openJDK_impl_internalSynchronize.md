



## choose



```cpp
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
```



## membar



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



> This instruction does nothing except memory ordering. It literally means *"atomically add zero to the value on the top of stack"*
>
> > Chapter 8.2.2 *"Memory Ordering"* of [Intel Architectures Software Developer’s Manual](http://www.intel.ru/content/www/ru/ru/processors/architectures-software-developer-manuals.html) says that locked instructions have a total order, no reads or writes can be reordered with locked instructions.



> In the past the JVM has used MFENCE but, because of latency issues on AMD processors and potential pipeline issues on modern Intel processors it appears that a LOCK:ADD of 0 to the top of stack is preferable



### OrderAccess



```cpp
// orderAccess_linux_x86.inline.hpp

/*
 * Copyright (c) 2003, 2013, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 *
 */

#ifndef OS_CPU_LINUX_X86_VM_ORDERACCESS_LINUX_X86_INLINE_HPP
#define OS_CPU_LINUX_X86_VM_ORDERACCESS_LINUX_X86_INLINE_HPP

#include "runtime/atomic.inline.hpp"
#include "runtime/orderAccess.hpp"
#include "runtime/os.hpp"
#include "vm_version_x86.hpp"

// Implementation of class OrderAccess.

inline void OrderAccess::loadload()   { acquire(); }
inline void OrderAccess::storestore() { release(); }
inline void OrderAccess::loadstore()  { acquire(); }
// yonka 可以看到 LL SS LS 因为x86 CPU本身能保证不乱序，所以只需要“告诉”编译器不要乱序就行了
// TODO SS 的操作看不懂...
inline void OrderAccess::storeload()  { fence(); }
// 通过lock prefix来禁止CPU重排

inline void OrderAccess::acquire() {
  volatile intptr_t local_dummy;
#ifdef AMD64
  __asm__ volatile ("movq 0(%%rsp), %0" : "=r" (local_dummy) : : "memory");
#else
  __asm__ volatile ("movl 0(%%esp),%0" : "=r" (local_dummy) : : "memory");
#endif // AMD64
}

inline void OrderAccess::release() {
  // Avoid hitting the same cache-line from
  // different threads.
  volatile jint local_dummy = 0;
}

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

// yonka 通过volatile来...？
inline jbyte    OrderAccess::load_acquire(volatile jbyte*   p) { return *p; }
inline jshort   OrderAccess::load_acquire(volatile jshort*  p) { return *p; }
inline jint     OrderAccess::load_acquire(volatile jint*    p) { return *p; }
inline jlong    OrderAccess::load_acquire(volatile jlong*   p) { return Atomic::load(p); }
// yonka
// AMD64 下 CPU本身能保证64位操作原子性所以： inline jlong Atomic::load(volatile jlong* src) { return *src; }；否则的话： 
//  volatile jlong dest;
//  _Atomic_move_long(src, &dest);
//  return dest;
// done-看不出来这段汇编怎么做到...
// by 雅神： 这个其实在艹浮点栈，相当于说，他一次走两个4B字，当作一个浮点进浮点栈；然后再把这两个4B字当整数凹出来，因为用了浮点上下文，所以虽然执行了两条指令，但在task级别是隔离的，并发safe
// _Atomic_move_long:
//         esp+0x04(src，指针) -> eax
//         movl     4(%esp), %eax   # src
//         装入（eax中地址处的）整数到st(0) 。ll字长，应该是8byte
//         fildll    (%eax)
//         esp+0x08(dst, 指针) -> eax
//         movl     8(%esp), %eax   # dest
//         将st(0)以整数保存到eax中地址处，然后再执行一次出栈操作
//         fistpll   (%eax)
//         ret
inline jubyte   OrderAccess::load_acquire(volatile jubyte*  p) { return *p; }
inline jushort  OrderAccess::load_acquire(volatile jushort* p) { return *p; }
inline juint    OrderAccess::load_acquire(volatile juint*   p) { return *p; }
inline julong   OrderAccess::load_acquire(volatile julong*  p) { return Atomic::load((volatile jlong*)p); }
inline jfloat   OrderAccess::load_acquire(volatile jfloat*  p) { return *p; }
inline jdouble  OrderAccess::load_acquire(volatile jdouble* p) { return jdouble_cast(Atomic::load((volatile jlong*)p)); }

inline intptr_t OrderAccess::load_ptr_acquire(volatile intptr_t*   p) { return *p; }
inline void*    OrderAccess::load_ptr_acquire(volatile void*       p) { return *(void* volatile *)p; }
inline void*    OrderAccess::load_ptr_acquire(const volatile void* p) { return *(void* const volatile *)p; }

inline void     OrderAccess::release_store(volatile jbyte*   p, jbyte   v) { *p = v; }
inline void     OrderAccess::release_store(volatile jshort*  p, jshort  v) { *p = v; }
inline void     OrderAccess::release_store(volatile jint*    p, jint    v) { *p = v; }
inline void     OrderAccess::release_store(volatile jlong*   p, jlong   v) { Atomic::store(v, p); }
inline void     OrderAccess::release_store(volatile jubyte*  p, jubyte  v) { *p = v; }
inline void     OrderAccess::release_store(volatile jushort* p, jushort v) { *p = v; }
inline void     OrderAccess::release_store(volatile juint*   p, juint   v) { *p = v; }
inline void     OrderAccess::release_store(volatile julong*  p, julong  v) { Atomic::store((jlong)v, (volatile jlong*)p); }
inline void     OrderAccess::release_store(volatile jfloat*  p, jfloat  v) { *p = v; }
inline void     OrderAccess::release_store(volatile jdouble* p, jdouble v) { release_store((volatile jlong *)p, jlong_cast(v)); }

inline void     OrderAccess::release_store_ptr(volatile intptr_t* p, intptr_t v) { *p = v; }
inline void     OrderAccess::release_store_ptr(volatile void*     p, void*    v) { *(void* volatile *)p = v; }

inline void     OrderAccess::store_fence(jbyte*  p, jbyte  v) {
  __asm__ volatile (  "xchgb (%2),%0"
                    : "=q" (v)
                    : "0" (v), "r" (p)
                    : "memory");
}
inline void     OrderAccess::store_fence(jshort* p, jshort v) {
  __asm__ volatile (  "xchgw (%2),%0"
                    : "=r" (v)
                    : "0" (v), "r" (p)
                    : "memory");
}
inline void     OrderAccess::store_fence(jint*   p, jint   v) {
  __asm__ volatile (  "xchgl (%2),%0"
                    : "=r" (v)
                    : "0" (v), "r" (p)
                    : "memory");
}

inline void     OrderAccess::store_fence(jlong*   p, jlong   v) {
#ifdef AMD64
  __asm__ __volatile__ ("xchgq (%2), %0"
                        : "=r" (v)
                        : "0" (v), "r" (p)
                        : "memory");
#else
  *p = v; fence();
#endif // AMD64
}

// AMD64 copied the bodies for the the signed version. 32bit did this. As long as the
// compiler does the inlining this is simpler.
inline void     OrderAccess::store_fence(jubyte*  p, jubyte  v) { store_fence((jbyte*)p,  (jbyte)v);  }
inline void     OrderAccess::store_fence(jushort* p, jushort v) { store_fence((jshort*)p, (jshort)v); }
inline void     OrderAccess::store_fence(juint*   p, juint   v) { store_fence((jint*)p,   (jint)v);   }
inline void     OrderAccess::store_fence(julong*  p, julong  v) { store_fence((jlong*)p,  (jlong)v);  }
inline void     OrderAccess::store_fence(jfloat*  p, jfloat  v) { *p = v; fence(); }
inline void     OrderAccess::store_fence(jdouble* p, jdouble v) { store_fence((jlong*)p, jlong_cast(v)); }

inline void     OrderAccess::store_ptr_fence(intptr_t* p, intptr_t v) {
#ifdef AMD64
  __asm__ __volatile__ ("xchgq (%2), %0"
                        : "=r" (v)
                        : "0" (v), "r" (p)
                        : "memory");
#else
  store_fence((jint*)p, (jint)v);
#endif // AMD64
}

inline void     OrderAccess::store_ptr_fence(void**    p, void*    v) {
#ifdef AMD64
  __asm__ __volatile__ ("xchgq (%2), %0"
                        : "=r" (v)
                        : "0" (v), "r" (p)
                        : "memory");
#else
  store_fence((jint*)p, (jint)v);
#endif // AMD64
}

// Must duplicate definitions instead of calling store_fence because we don't want to cast away volatile.
inline void     OrderAccess::release_store_fence(volatile jbyte*  p, jbyte  v) {
  __asm__ volatile (  "xchgb (%2),%0"
                    : "=q" (v)
                    : "0" (v), "r" (p)
                    : "memory");
}
inline void     OrderAccess::release_store_fence(volatile jshort* p, jshort v) {
  __asm__ volatile (  "xchgw (%2),%0"
                    : "=r" (v)
                    : "0" (v), "r" (p)
                    : "memory");
}
inline void     OrderAccess::release_store_fence(volatile jint*   p, jint   v) {
  __asm__ volatile (  "xchgl (%2),%0"
                    : "=r" (v)
                    : "0" (v), "r" (p)
                    : "memory");
}

inline void     OrderAccess::release_store_fence(volatile jlong*   p, jlong   v) {
#ifdef AMD64
  __asm__ __volatile__ (  "xchgq (%2), %0"
                          : "=r" (v)
                          : "0" (v), "r" (p)
                          : "memory");
#else
  release_store(p, v); fence();
#endif // AMD64
}

inline void     OrderAccess::release_store_fence(volatile jubyte*  p, jubyte  v) { release_store_fence((volatile jbyte*)p,  (jbyte)v);  }
inline void     OrderAccess::release_store_fence(volatile jushort* p, jushort v) { release_store_fence((volatile jshort*)p, (jshort)v); }
inline void     OrderAccess::release_store_fence(volatile juint*   p, juint   v) { release_store_fence((volatile jint*)p,   (jint)v);   }
inline void     OrderAccess::release_store_fence(volatile julong*  p, julong  v) { release_store_fence((volatile jlong*)p,  (jlong)v);  }

inline void     OrderAccess::release_store_fence(volatile jfloat*  p, jfloat  v) { *p = v; fence(); }
inline void     OrderAccess::release_store_fence(volatile jdouble* p, jdouble v) { release_store_fence((volatile jlong*)p, jlong_cast(v)); }

inline void     OrderAccess::release_store_ptr_fence(volatile intptr_t* p, intptr_t v) {
#ifdef AMD64
  __asm__ __volatile__ (  "xchgq (%2), %0"
                          : "=r" (v)
                          : "0" (v), "r" (p)
                          : "memory");
#else
  release_store_fence((volatile jint*)p, (jint)v);
#endif // AMD64
}
inline void     OrderAccess::release_store_ptr_fence(volatile void*     p, void*    v) {
#ifdef AMD64
  __asm__ __volatile__ (  "xchgq (%2), %0"
                          : "=r" (v)
                          : "0" (v), "r" (p)
                          : "memory");
#else
  release_store_fence((volatile jint*)p, (jint)v);
#endif // AMD64
}

#endif // OS_CPU_LINUX_X86_VM_ORDERACCESS_LINUX_X86_INLINE_HPP
```













## memory_serialize_page



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





