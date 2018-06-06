golang sync.WaitGroup 实现小结_yonka

**简介**

> ...

**源码简介**

> ![img](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/5ac688df-9a28-4549-9653-90458f7edc38/index_files/4c95c65b-f54d-4b9b-8f40-b0f22d91326e.png)
>
> 组成：

> > state1 [12]byte  # 总数计数 和 wait计数
>
> > sema uint32  # 作为“锁”

> trick - 64bit对齐

> > 64bit的原子操作需要64-bit对齐，而32-bit的编译器并不保证这一点。 所以这里用了12bit来ensure it。
> >
> > ```Go
> > func (wg *WaitGroup) state() *uint64 {
> >    if uintptr(unsafe.Pointer(&wg.state1))%8 == 0 {
> >       return (*uint64)(unsafe.Pointer(&wg.state1))
> >    } else {
> >       return (*uint64)(unsafe.Pointer(&wg.state1[4]))
> >    }
> > }
> > ```
> >
> > 可以看到，如果对齐的话（指针地址为8（byte）的整数倍）则取前8个byte（index 0- 7），如果没有（则起码是32-bit对齐）则取后8个byte（index 4-11）

> state

> > 见上面...

> Add

> > ```Go
> >     // Synchronize decrements with Wait.
> >          raceReleaseMerge(unsafe.Pointer(wg))
> >       }
> >       raceDisable()
> >       defer raceEnable()
> >    }
> >    state := atomic.AddUint64(statep, uint64(delta)<<32)
> >    v := int32(state >> 32)
> >    w := uint32(state)
> >    if raceenabled {
> >       if delta > 0 && v == int32(delta) {
> >          // The first increment must be synchronized with Wait.
> >          // Need to model this as a read, because there can be
> >          // several concurrent wg.counter transitions from 0.
> >          raceRead(unsafe.Pointer(&wg.sema))
> >       }
> >    }
> >    if v < 0 {
> >       panic("sync: negative WaitGroup counter")
> >    }
> >    if w != 0 && delta > 0 && v == int32(delta) {
> >       panic("sync: WaitGroup misuse: Add called concurrently with Wait")
> >    }
> >    if v > 0 || w == 0 {
> >       return
> >    }
> >    // This goroutine has set counter to 0 when waiters > 0.
> >    // Now there can't be concurrent mutations of state:
> >    // - Adds must not happen concurrently with Wait,
> >    // - Wait does not increment waiters if it sees counter == 0.
> >    // Still do a cheap sanity check to detect WaitGroup misuse.
> >    if *statep != state {
> >       panic("sync: WaitGroup misuse: Add called concurrently with Wait")
> >    }
> >    // Reset waiters count to 0.
> >    *statep = 0
> >    for ; w != 0; w-- {
> >       runtime_Semrelease(&wg.sema)
> >    }
> > }
> > ```
> >
> > 把负数强转为uint64然后相加后再转为int64，等价于模拟了补码运算...
> >
> > add是无锁的。
> >
> > 1、根据32位还是64情况获取state指针statep
> >
> > 2、用原子操作根据statep把delta加到state并获取结果值
> >
> > 3、从state中提取counter部分（w）和waiter部分（v）
> >
> > 4、如果操作后的counter值小于0说明减多了（Done数超过总数），panic
> >
> > 5、如果delta>0（说明是Add而不是Done操作）同时又w部分 != 0（说明已经有在wait的了）同时v 等于 delta值的话，说明先wait后add了，panic
> >
> > 6、如果 v <= 0 的情况下w != 0的话，说明有“多余”的waiter，则逐个唤醒。

> Done

> > Add(-1)
> >
> > -1 ---> 11111111 11111111 11111111 11111111 11111111 11111111 11111111 11111111
> >
> > \# 强制转换为uint64后不改变bits
> >
> > ```Go
> > state := atomic.AddUint64(statep, uint64(delta)<<32)
> > ```
> >
> > 也即 把 statep的高4byte和 11111111 11111111 11111111 11111111 相加，实际溢出后为 减一 （= = ）

> Wait

> > ```Go
> > func (wg *WaitGroup) Wait() {
> >    statep := wg.state()
> >    if raceenabled {
> >       _ = *statep // trigger nil deref early
> >       raceDisable()
> >    }
> >    for {
> >       state := atomic.LoadUint64(statep)
> >       v := int32(state >> 32)
> >       w := uint32(state)
> >       if v == 0 {
> >          // Counter is 0, no need to wait.
> >          if raceenabled {
> >             raceEnable()
> >             raceAcquire(unsafe.Pointer(wg))
> >          }
> >          return
> >       }
> >       // Increment waiters count.
> >       if atomic.CompareAndSwapUint64(statep, state, state+1) {
> >          if raceenabled && w == 0 {
> >             // Wait must be synchronized with the first Add.
> >             // Need to model this is as a write to race with the read in Add.
> >             // As a consequence, can do the write only for the first waiter,
> >             // otherwise concurrent Waits will race with each other.
> >             raceWrite(unsafe.Pointer(&wg.sema))
> >          }
> >          runtime_Semacquire(&wg.sema)
> >          if *statep != 0 {
> >             panic("sync: WaitGroup is reused before previous Wait has returned")
> >          }
> >          if raceenabled {
> >             raceEnable()
> >             raceAcquire(unsafe.Pointer(wg))
> >          }
> >          return
> >       }
> >    }
> > }
> > ```
> >
> > 1、获取statep
> >
> > 2、for循环中执行下面的操作（主要是为了乐观锁）

> > > 1 load state值，取 counter、waiter部分
> > >
> > > 2 如果v == 0则无需再等待； 否则尝试给waiter+1（原子操作CompareAndSwap到statep），失败则自旋（开始下一个循环），成功则尝试等待sema（semaacquire）

**源码**

> ```Go
> // Copyright 2011 The Go Authors. All rights reserved.
> // Use of this source code is governed by a BSD-style
> // license that can be found in the LICENSE file.
>
> package sync
>
> import (
>    "sync/atomic"
>    "unsafe"
> )
>
> // A WaitGroup waits for a collection of goroutines to finish.
> // The main goroutine calls Add to set the number of
> // goroutines to wait for.  Then each of the goroutines
> // runs and calls Done when finished.  At the same time,
> // Wait can be used to block until all goroutines have finished.
> type WaitGroup struct {
>    // 64-bit value: high 32 bits are counter, low 32 bits are waiter count.
>    // 64-bit atomic operations require 64-bit alignment, but 32-bit
>    // compilers do not ensure it. So we allocate 12 bytes and then use
>    // the aligned 8 bytes in them as state.
>    state1 [12]byte
>    sema   uint32
> }
>
> func (wg *WaitGroup) state() *uint64 {
>    if uintptr(unsafe.Pointer(&wg.state1))%8 == 0 {
>       return (*uint64)(unsafe.Pointer(&wg.state1))
>    } else {
>       return (*uint64)(unsafe.Pointer(&wg.state1[4]))
>    }
> }
>
> // Add adds delta, which may be negative, to the WaitGroup counter.
> // If the counter becomes zero, all goroutines blocked on Wait are released.
> // If the counter goes negative, Add panics.
> //
> // Note that calls with a positive delta that occur when the counter is zero
> // must happen before a Wait. Calls with a negative delta, or calls with a
> // positive delta that start when the counter is greater than zero, may happen
> // at any time.
> // Typically this means the calls to Add should execute before the statement
> // creating the goroutine or other event to be waited for.
> // If a WaitGroup is reused to wait for several independent sets of events,
> // new Add calls must happen after all previous Wait calls have returned.
> // See the WaitGroup example.
> func (wg *WaitGroup) Add(delta int) {
>    statep := wg.state()
>    if raceenabled {
>       _ = *statep // trigger nil deref early
>       if delta < 0 {
>          // Synchronize decrements with Wait.
>          raceReleaseMerge(unsafe.Pointer(wg))
>       }
>       raceDisable()
>       defer raceEnable()
>    }
>    state := atomic.AddUint64(statep, uint64(delta)<<32)
>    v := int32(state >> 32)
>    w := uint32(state)
>    if raceenabled {
>       if delta > 0 && v == int32(delta) {
>          // The first increment must be synchronized with Wait.
>          // Need to model this as a read, because there can be
>          // several concurrent wg.counter transitions from 0.
>          raceRead(unsafe.Pointer(&wg.sema))
>       }
>    }
>    if v < 0 {
>       panic("sync: negative WaitGroup counter")
>    }
>    if w != 0 && delta > 0 && v == int32(delta) {
>       panic("sync: WaitGroup misuse: Add called concurrently with Wait")
>    }
>    if v > 0 || w == 0 {
>       return
>    }
>    // This goroutine has set counter to 0 when waiters > 0.
>    // Now there can't be concurrent mutations of state:
>    // - Adds must not happen concurrently with Wait,
>    // - Wait does not increment waiters if it sees counter == 0.
>    // Still do a cheap sanity check to detect WaitGroup misuse.
>    if *statep != state {
>       panic("sync: WaitGroup misuse: Add called concurrently with Wait")
>    }
>    // Reset waiters count to 0.
>    *statep = 0
>    for ; w != 0; w-- {
>       runtime_Semrelease(&wg.sema)
>    }
> }
>
> // Done decrements the WaitGroup counter.
> func (wg *WaitGroup) Done() {
>    wg.Add(-1)
> }
>
> // Wait blocks until the WaitGroup counter is zero.
> func (wg *WaitGroup) Wait() {
>    statep := wg.state()
>    if raceenabled {
>       _ = *statep // trigger nil deref early
>       raceDisable()
>    }
>    for {
>       state := atomic.LoadUint64(statep)
>       v := int32(state >> 32)
>       w := uint32(state)
>       if v == 0 {
>          // Counter is 0, no need to wait.
>          if raceenabled {
>             raceEnable()
>             raceAcquire(unsafe.Pointer(wg))
>          }
>          return
>       }
>       // Increment waiters count.
>       if atomic.CompareAndSwapUint64(statep, state, state+1) {
>          if raceenabled && w == 0 {
>             // Wait must be synchronized with the first Add.
>             // Need to model this is as a write to race with the read in Add.
>             // As a consequence, can do the write only for the first waiter,
>             // otherwise concurrent Waits will race with each other.
>             raceWrite(unsafe.Pointer(&wg.sema))
>          }
>          runtime_Semacquire(&wg.sema)
>          if *statep != 0 {
>             panic("sync: WaitGroup is reused before previous Wait has returned")
>          }
>          if raceenabled {
>             raceEnable()
>             raceAcquire(unsafe.Pointer(wg))
>          }
>          return
>       }
>    }
> }
> ```