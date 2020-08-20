**源码简介**

> ![img](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/39255031-06ba-491f-86aa-2cd25c7670dc/index_files/6233afbe-f304-4f60-ab31-8a7e1bb3c717.png)

**源码**

> 主要是 doc.go和value.go，其他的都是平台相关的汇编实现等...。
>
> 在runtime包下有：
>
> doc.go
>
> ```
> // Copyright 2011 The Go Authors.  All rights reserved.
> // Use of this source code is governed by a BSD-style
> // license that can be found in the LICENSE file.
>
> // Package atomic provides low-level atomic memory primitives
> // useful for implementing synchronization algorithms.
> //
> // These functions require great care to be used correctly.
> // Except for special, low-level applications, synchronization is better
> // done with channels or the facilities of the sync package.
> // Share memory by communicating;
> // don't communicate by sharing memory.
> //
> // The swap operation, implemented by the SwapT functions, is the atomic
> // equivalent of:
> //
> // old = *addr
> // *addr = new
> // return old
> //
> // The compare-and-swap operation, implemented by the CompareAndSwapT
> // functions, is the atomic equivalent of:
> //
> // if *addr == old {
> //    *addr = new
> //    return true
> // }
> // return false
> //
> // The add operation, implemented by the AddT functions, is the atomic
> // equivalent of:
> //
> // *addr += delta
> // return *addr
> //
> // The load and store operations, implemented by the LoadT and StoreT
> // functions, are the atomic equivalents of "return *addr" and
> // "*addr = val".
> //
> package atomic
>
> import (
>    "unsafe"
> )
>
> // BUG(rsc): On x86-32, the 64-bit functions use instructions unavailable before the Pentium MMX.
> //
> // On non-Linux ARM, the 64-bit functions use instructions unavailable before the ARMv6k core.
> //
> // On both ARM and x86-32, it is the caller's responsibility to arrange for 64-bit
> // alignment of 64-bit words accessed atomically. The first word in a global
> // variable or in an allocated struct or slice can be relied upon to be
> // 64-bit aligned.
>
> // SwapInt32 atomically stores new into *addr and returns the previous *addr value.
> func SwapInt32(addr *int32, new int32) (old int32)
>
> // SwapInt64 atomically stores new into *addr and returns the previous *addr value.
> func SwapInt64(addr *int64, new int64) (old int64)
>
> // SwapUint32 atomically stores new into *addr and returns the previous *addr value.
> func SwapUint32(addr *uint32, new uint32) (old uint32)
>
> // SwapUint64 atomically stores new into *addr and returns the previous *addr value.
> func SwapUint64(addr *uint64, new uint64) (old uint64)
>
> // SwapUintptr atomically stores new into *addr and returns the previous *addr value.
> func SwapUintptr(addr *uintptr, new uintptr) (old uintptr)
>
> // SwapPointer atomically stores new into *addr and returns the previous *addr value.
> func SwapPointer(addr *unsafe.Pointer, new unsafe.Pointer) (old unsafe.Pointer)
>
> // CompareAndSwapInt32 executes the compare-and-swap operation for an int32 value.
> func CompareAndSwapInt32(addr *int32, old, new int32) (swapped bool)
>
> // CompareAndSwapInt64 executes the compare-and-swap operation for an int64 value.
> func CompareAndSwapInt64(addr *int64, old, new int64) (swapped bool)
>
> // CompareAndSwapUint32 executes the compare-and-swap operation for a uint32 value.
> func CompareAndSwapUint32(addr *uint32, old, new uint32) (swapped bool)
>
> // CompareAndSwapUint64 executes the compare-and-swap operation for a uint64 value.
> func CompareAndSwapUint64(addr *uint64, old, new uint64) (swapped bool)
>
> // CompareAndSwapUintptr executes the compare-and-swap operation for a uintptr value.
> func CompareAndSwapUintptr(addr *uintptr, old, new uintptr) (swapped bool)
>
> // CompareAndSwapPointer executes the compare-and-swap operation for a unsafe.Pointer value.
> func CompareAndSwapPointer(addr *unsafe.Pointer, old, new unsafe.Pointer) (swapped bool)
>
> // AddInt32 atomically adds delta to *addr and returns the new value.
> func AddInt32(addr *int32, delta int32) (new int32)
>
> // AddUint32 atomically adds delta to *addr and returns the new value.
> // To subtract a signed positive constant value c from x, do AddUint32(&x, ^uint32(c-1)).
> // In particular, to decrement x, do AddUint32(&x, ^uint32(0)).
> func AddUint32(addr *uint32, delta uint32) (new uint32)
>
> // AddInt64 atomically adds delta to *addr and returns the new value.
> func AddInt64(addr *int64, delta int64) (new int64)
>
> // AddUint64 atomically adds delta to *addr and returns the new value.
> // To subtract a signed positive constant value c from x, do AddUint64(&x, ^uint64(c-1)).
> // In particular, to decrement x, do AddUint64(&x, ^uint64(0)).
> func AddUint64(addr *uint64, delta uint64) (new uint64)
>
> // AddUintptr atomically adds delta to *addr and returns the new value.
> func AddUintptr(addr *uintptr, delta uintptr) (new uintptr)
>
> // LoadInt32 atomically loads *addr.
> func LoadInt32(addr *int32) (val int32)
>
> // LoadInt64 atomically loads *addr.
> func LoadInt64(addr *int64) (val int64)
>
> // LoadUint32 atomically loads *addr.
> func LoadUint32(addr *uint32) (val uint32)
>
> // LoadUint64 atomically loads *addr.
> func LoadUint64(addr *uint64) (val uint64)
>
> // LoadUintptr atomically loads *addr.
> func LoadUintptr(addr *uintptr) (val uintptr)
>
> // LoadPointer atomically loads *addr.
> func LoadPointer(addr *unsafe.Pointer) (val unsafe.Pointer)
>
> // StoreInt32 atomically stores val into *addr.
> func StoreInt32(addr *int32, val int32)
>
> // StoreInt64 atomically stores val into *addr.
> func StoreInt64(addr *int64, val int64)
>
> // StoreUint32 atomically stores val into *addr.
> func StoreUint32(addr *uint32, val uint32)
>
> // StoreUint64 atomically stores val into *addr.
> func StoreUint64(addr *uint64, val uint64)
>
> // StoreUintptr atomically stores val into *addr.
> func StoreUintptr(addr *uintptr, val uintptr)
>
> // StorePointer atomically stores val into *addr.
> func StorePointer(addr *unsafe.Pointer, val unsafe.Pointer)
>
> // Helper for ARM.  Linker will discard on other systems
> func panic64() {
>    panic("sync/atomic: broken 64-bit atomic operations (buggy QEMU)")
> }
>
> ```
>
> value.go
>
> ```go
> // Copyright 2014 The Go Authors. All rights reserved.
> // Use of this source code is governed by a BSD-style
> // license that can be found in the LICENSE file.
>
> package atomic
>
> import (
>    "unsafe"
> )
>
> // A Value provides an atomic load and store of a consistently typed value.
> // Values can be created as part of other data structures.
> // The zero value for a Value returns nil from Load.
> // Once Store has been called, a Value must not be copied.
> type Value struct {
>    v interface{}
> }
>
> // ifaceWords is interface{} internal representation.
> type ifaceWords struct {
>    typ  unsafe.Pointer
>    data unsafe.Pointer
> }
>
> // Load returns the value set by the most recent Store.
> // It returns nil if there has been no call to Store for this Value.
> func (v *Value) Load() (x interface{}) {
>    vp := (*ifaceWords)(unsafe.Pointer(v))
>    typ := LoadPointer(&vp.typ)
>    if typ == nil || uintptr(typ) == ^uintptr(0) {
>       // First store not yet completed.
>       return nil
>    }
>    data := LoadPointer(&vp.data)
>    xp := (*ifaceWords)(unsafe.Pointer(&x))
>    xp.typ = typ
>    xp.data = data
>    return
> }
>
> // Store sets the value of the Value to x.
> // All calls to Store for a given Value must use values of the same concrete type.
> // Store of an inconsistent type panics, as does Store(nil).
> func (v *Value) Store(x interface{}) {
>    if x == nil {
>       panic("sync/atomic: store of nil value into Value")
>    }
>    vp := (*ifaceWords)(unsafe.Pointer(v))
>    xp := (*ifaceWords)(unsafe.Pointer(&x))
>    for {
>       typ := LoadPointer(&vp.typ)
>       if typ == nil {
>          // Attempt to start first store.
>          // Disable preemption so that other goroutines can use
>          // active spin wait to wait for completion; and so that
>          // GC does not see the fake type accidentally.
>          runtime_procPin()
>          if !CompareAndSwapPointer(&vp.typ, nil, unsafe.Pointer(^uintptr(0))) {
>             runtime_procUnpin()
>             continue
>          }
>          // Complete first store.
>          StorePointer(&vp.data, xp.data)
>          StorePointer(&vp.typ, xp.typ)
>          runtime_procUnpin()
>          return
>       }
>       if uintptr(typ) == ^uintptr(0) {
>          // First store in progress. Wait.
>          // Since we disable preemption around the first store,
>          // we can wait with active spinning.
>          continue
>       }
>       // First store completed. Check type and overwrite data.
>       if typ != xp.typ {
>          panic("sync/atomic: store of inconsistently typed value into Value")
>       }
>       StorePointer(&vp.data, xp.data)
>       return
>    }
> }
>
> // Disable/enable preemption, implemented in runtime.
> func runtime_procPin()
> func runtime_procUnpin()
> ```