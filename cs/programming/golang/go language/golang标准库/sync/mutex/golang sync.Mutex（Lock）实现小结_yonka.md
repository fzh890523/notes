

golang sync.Mutex（Lock）实现小结_yonka

**简介**

> ```Go
> // A Locker represents an object that can be locked and unlocked.
> type Locker interface {
>    Lock()
>    Unlock()
> }
> ```
>
> 接口定义。Mutex和RWMutex都实现了该接口。
>
> 
>
> ```Go
> // A Mutex is a mutual exclusion lock.
> // Mutexes can be created as part of other structures;
> // the zero value for a Mutex is an unlocked mutex.
> type Mutex struct {
>    state int32
>    sema  uint32
> }
> ```

> “锁”的不是sema值，而是sema的指针，值只做计数用。 指针自然不会（同时）重复，所以不会出现多个mutex重复的情况。
>
> state

> > 最低位为是否lock；
> >
> > 低第2位为是否有waiter醒来
> >
> > 除去低位的部分用于记录waiter数量，也即 state >> 2 即 waiter数量。 （这个2 由 mutexWaiterShift常量定义）

**源码简介**

> ****
>
> ![img]()****
>
> ****
>
> (m *Mutex).Lock()

> > \# iter每次从semacquire中被唤醒都会清零 --- 只是控制自旋
> >
> > \# awoke 在从semacquire中被唤醒 或者 在自旋前尝试把state的woken置1成功 时置1； 在semacquire前会置0 --- awoke的意义在于避免没有block的情况下多次给woken置1（因为即使自己设置了woken为1也可能被其他设置为0，所以不能依赖这个）
> >
> > \# waiters 在 判断“不能自旋” 后 +1，之后会进入（如果cas设置state成功）semacquire
> >
> > \# woken用来同步lock和unlock操作，unlock发现有waiter awoke（woken为1）会返回，否则会尝试awoke一个waiter； waiter则在自己第一次awoke（在自旋或者从block中恢复）时设置woken
> >
> > ```Go
> > // Lock locks m.
> > // If the lock is already in use, the calling goroutine
> > // blocks until the mutex is available.
> > func (m *Mutex) Lock() {
> >    // Fast path: grab unlocked mutex.
> >    if atomic.CompareAndSwapInt32(&m.state, 0, mutexLocked) {
> >       if raceenabled {
> >          raceAcquire(unsafe.Pointer(m))
> >       }
> >       return
> >    }
> >
> >    awoke := false
> >    iter := 0
> >    for {
> >       old := m.state
> >       new := old | mutexLocked
> >       if old&mutexLocked != 0 {
> >          if runtime_canSpin(iter) {
> >             // Active spinning makes sense.
> >             // Try to set mutexWoken flag to inform Unlock
> >             // to not wake other blocked goroutines.
> >             if !awoke && old&mutexWoken == 0 && old>>mutexWaiterShift != 0 &&
> >                atomic.CompareAndSwapInt32(&m.state, old, old|mutexWoken) {
> >                awoke = true
> >             }
> >             runtime_doSpin()
> >             iter++
> >             continue
> >          }
> >          new = old + 1<<mutexWaiterShift
> >       }
> >       if awoke {
> >          // The goroutine has been woken from sleep,
> >          // so we need to reset the flag in either case.
> >          if new&mutexWoken == 0 {
> >             panic("sync: inconsistent mutex state")
> >          }
> >          new &^= mutexWoken
> >       }
> >       if atomic.CompareAndSwapInt32(&m.state, old, new) {
> >          if old&mutexLocked == 0 {
> >             break
> >          }
> >          runtime_Semacquire(&m.sema)
> >          awoke = true
> >          iter = 0
> >       }
> >    }
> >
> >    if raceenabled {
> >       raceAcquire(unsafe.Pointer(m))
> >    }
> > }
> > ```
> >
> > 1 通过cas尝试改变状态来获得锁（state），成功则退出。失败则继续： 可能是被锁定，可能是woken，可能兼具
> >
> > 2 在循环里

> > > 1 取当前状态 old，定义新状态为 new := old | mutexLocked（也即只把最低位置为已锁定）
> > >
> > > 2 如果老状态已锁定（old & mutexLocked != 0）

> > > > 1 那么会尝试自旋

> > > > > 1但首先需要当前能够自旋（比如当前p没有其他任务...，不然与其自旋比如去执行其他任务）
>
> > > > > 2 如果当前goroutine没设置过woken 并且 old的woken状态位为0（没有其他woken的） 并且 old的waiters不为0，则尝试cas设置state woken状态位为1，成功则记录该goroutine 设置过woken了
>
> > > > > 3 执行自旋
> > > > >
> > > > > 4 iter++（该值决定是否能继续自旋）
> > > > >
> > > > > 5 结束本次循环，开始下一次循环

> > > > 2 将new的waiters加一： new = old + 1 << mutexWaitersShift
> > >
> > > 3 如果该线程已经设置过woken，校验一下old的woken状态位是否为1，不是的话说明状态不一致，panic... 是的话则把new的woken状态位置0
> >
> > > 4 cas尝试把new赋给state （加了waiter、woken置0、locked置1），成功则： 如果老的locked为0说明获得锁了，退出； 如果不是则说明锁已经被... 则进入 semacquire（sema为 &m.sema）
> > >
> > > 
> > >
> > > 5 记录已经awoke了，iter重置为0，开始下一次循环

> (m *Mutex).Unlock

> > ```Go
> > // Unlock unlocks m.
> > // It is a run-time error if m is not locked on entry to Unlock.
> > //
> > // A locked Mutex is not associated with a particular goroutine.
> > // It is allowed for one goroutine to lock a Mutex and then
> > // arrange for another goroutine to unlock it.
> > func (m *Mutex) Unlock() {
> >    if raceenabled {
> >       _ = m.state
> >       raceRelease(unsafe.Pointer(m))
> >    }
> >
> >    // Fast path: drop lock bit.
> >    new := atomic.AddInt32(&m.state, -mutexLocked)
> >    if (new+mutexLocked)&mutexLocked == 0 {
> >       panic("sync: unlock of unlocked mutex")
> >    }
> >
> >    old := new
> >    for {
> >       // If there are no waiters or a goroutine has already
> >       // been woken or grabbed the lock, no need to wake anyone.
> >       if old>>mutexWaiterShift == 0 || old&(mutexLocked|mutexWoken) != 0 {
> >          return
> >       }
> >       // Grab the right to wake someone.
> >       new = (old - 1<<mutexWaiterShift) | mutexWoken
> >       if atomic.CompareAndSwapInt32(&m.state, old, new) {
> >          runtime_Semrelease(&m.sema)
> >          return
> >       }
> >       old = m.state
> >    }
> > }
> > ```
> >
> > \# 按照规定，只有获得锁的才能释放锁，那么说明lock bit肯定为1
> >
> > 1 通过原子操作把 m.state 减去 mutexLocked（AddInt32(&m.state, -mutexLocked)）
> >
> > 2 判断 新值 + mutexLocked （即操作前老值） 的lock bit是否为1，如果不是则说明unlock了一个 non-locked的锁，panic --- 这里只要有多余一个（若本来就是unlocked则为0）unlock则必然panic，由1中的原子操作保证
> >
> > 3 在循环里

> > > 1 如果waiter数目为0 或者 locked 和 woken 这两个状态为有至少不为0则退出（有人获得锁 或者 有人醒来）
> > >
> > > 2 用cas尝试把state改为 waiter数减一并且woken状态位置1，如果成功的话唤醒一次（一个）waiter （看来随机性由sema实现负责）；失败说明值发生变化，读取新值开始下一个循环
> > >
> > > \# 好奇的是，这是不用 atomic的load来读值

**源码**

> ```Go
> // Copyright 2009 The Go Authors. All rights reserved.
> // Use of this source code is governed by a BSD-style
> // license that can be found in the LICENSE file.
>
> // Package sync provides basic synchronization primitives such as mutual
> // exclusion locks.  Other than the Once and WaitGroup types, most are intended
> // for use by low-level library routines.  Higher-level synchronization is
> // better done via channels and communication.
> //
> // Values containing the types defined in this package should not be copied.
> package sync
>
> import (
>    "sync/atomic"
>    "unsafe"
> )
>
> // A Mutex is a mutual exclusion lock.
> // Mutexes can be created as part of other structures;
> // the zero value for a Mutex is an unlocked mutex.
> type Mutex struct {
>    state int32
>    sema  uint32
> }
>
> // A Locker represents an object that can be locked and unlocked.
> type Locker interface {
>    Lock()
>    Unlock()
> }
>
> const (
>    mutexLocked = 1 << iota // mutex is locked
>    mutexWoken
>    mutexWaiterShift = iota
> )
>
> // Lock locks m.
> // If the lock is already in use, the calling goroutine
> // blocks until the mutex is available.
> func (m *Mutex) Lock() {
>    // Fast path: grab unlocked mutex.
>    if atomic.CompareAndSwapInt32(&m.state, 0, mutexLocked) {
>       if raceenabled {
>          raceAcquire(unsafe.Pointer(m))
>       }
>       return
>    }
>
>    awoke := false
>    iter := 0
>    for {
>       old := m.state
>       new := old | mutexLocked
>       if old&mutexLocked != 0 {
>          if runtime_canSpin(iter) {
>             // Active spinning makes sense.
>             // Try to set mutexWoken flag to inform Unlock
>             // to not wake other blocked goroutines.
>             if !awoke && old&mutexWoken == 0 && old>>mutexWaiterShift != 0 &&
>                atomic.CompareAndSwapInt32(&m.state, old, old|mutexWoken) {
>                awoke = true
>             }
>             runtime_doSpin()
>             iter++
>             continue
>          }
>          new = old + 1<<mutexWaiterShift
>       }
>       if awoke {
>          // The goroutine has been woken from sleep,
>          // so we need to reset the flag in either case.
>          if new&mutexWoken == 0 {
>             panic("sync: inconsistent mutex state")
>          }
>          new &^= mutexWoken
>       }
>       if atomic.CompareAndSwapInt32(&m.state, old, new) {
>          if old&mutexLocked == 0 {
>             break
>          }
>          runtime_Semacquire(&m.sema)
>          awoke = true
>          iter = 0
>       }
>    }
>
>    if raceenabled {
>       raceAcquire(unsafe.Pointer(m))
>    }
> }
>
> // Unlock unlocks m.
> // It is a run-time error if m is not locked on entry to Unlock.
> //
> // A locked Mutex is not associated with a particular goroutine.
> // It is allowed for one goroutine to lock a Mutex and then
> // arrange for another goroutine to unlock it.
> func (m *Mutex) Unlock() {
>    if raceenabled {
>       _ = m.state
>       raceRelease(unsafe.Pointer(m))
>    }
>
>    // Fast path: drop lock bit.
>    new := atomic.AddInt32(&m.state, -mutexLocked)
>    if (new+mutexLocked)&mutexLocked == 0 {
>       panic("sync: unlock of unlocked mutex")
>    }
>
>    old := new
>    for {
>       // If there are no waiters or a goroutine has already
>       // been woken or grabbed the lock, no need to wake anyone.
>       if old>>mutexWaiterShift == 0 || old&(mutexLocked|mutexWoken) != 0 {
>          return
>       }
>       // Grab the right to wake someone.
>       new = (old - 1<<mutexWaiterShift) | mutexWoken
>       if atomic.CompareAndSwapInt32(&m.state, old, new) {
>          runtime_Semrelease(&m.sema)
>          return
>       }
>       old = m.state
>    }
> }
> ```

