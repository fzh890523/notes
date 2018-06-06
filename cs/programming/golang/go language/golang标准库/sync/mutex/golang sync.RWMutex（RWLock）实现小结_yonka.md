

golang sync.RWMutex（RWLock）实现小结_yonka

**简介**

> 
>
> ```Go
> // An RWMutex is a reader/writer mutual exclusion lock.
> // The lock can be held by an arbitrary number of readers
> // or a single writer.
> // RWMutexes can be created as part of other
> // structures; the zero value for a RWMutex is
> // an unlocked mutex.
> type RWMutex struct {
>    w           Mutex  // held if there are pending writers
>    writerSem   uint32 // semaphore for writers to wait for completing readers
>    readerSem   uint32 // semaphore for readers to wait for completing writers
>    readerCount int32  // number of pending readers
>    readerWait  int32  // number of departing readers
> }
> ```
>
> rlocker类型 && RLocker函数 类似 view，使得RWLock只暴露rlock部分。
>
> RWLock也实现了Lock接口。 
>
> 最大的reader数量为 rwmutexMaxReaders 1<<30， 01000000 . . . 相反数为 10111111 . . .，
>
> readerSem： reader在RLock时发现有pending/ongoing的writer时acquire
>
> writerSem： writer在Lock时发现有ongoing的reader时acquire
>
> reader： 有机会unlock的reader都是lock成功的，也即是在 《在writer“-rwmutexMaxReader”操作之前的做了+1的readers》， 它们会拿了锁的那个writer pending，所以要对readerwait做减一操作（同样的，只有当有该pending waiter时才需要做）。 由最有一个（把readerWait减到0的那个）reader来执行唤醒writer的操作
>
> writer： 只wait在writer“-rwmutexMaxReader”操作之前的做了+1的readers，之后+1的readers其实已经没法执行read了； wait的方式是把那一刻的waiters数量加到readerWait，然后...
>
> 原子操作： 用精巧的原子操作来界定 writer需要wait的readers

**源码简介**

> ****
>
> ![img](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/c0bb5327-5b9f-4aad-ae38-eb1180d3b4ae/index_files/3c61ee83-194f-4c0a-85cf-98d7474ed8fd.png)****
>
> ****
>
> ****
>
> (rw *RWMutex) RLock()

> > ****
> >
> > 1 原子方式readercount加一
> >
> > 2 如果值仍然小于0，说明有writer在...，做semacquire（对 &rw.readerSem）
> >
> > ****

> ****
>
> (rw *RWMutex) RUnlock()

> > ****
> >
> > 1 原子方式readercount减一
> >
> > 2 如果值小于0

> > > 如果原来是0 或者 -rwmutexMaxReaders 则说明没有获得锁，panic
> > >
> > > 否则（也即原来在 (-rwmutexMaxReaders, 0) ）说明有writer pending。 此时原子操作给rw.readerWait减一，判断是否为0，是的话唤醒writer（sema为 &rw.writerSem）
> > >
> > > \# 有wait才需要减wait

> (rw *RWMutex) Lock()

> > 1 尝试获得锁 --- writer之间互斥
> >
> > \# 被唤醒，获得锁
> >
> > 2 原子操作给 rw.readerCount减去 rwmutexMaxReader来指示有pending writer --- 此时结果值肯定为负，加上rwmutexMaxReaders才会...
> >
> > 3 如果原readcount不为0（因为writer互斥，所以这个count只会是reader带来的），那么把该值加到readWaiter中，如果结果不为0，则waiter进入semacquire（sema为writerSem）

> (rw *RWMutex) Unlock

> > 1 给readerCount加上rwmutexMaxReaders（加完肯定为正了），把lock时减去的抵消，此时结果值r为真实reader数量
> >
> > 2 如果此时结果r >= rwmutexMaxReaders，说明错误，panic...
> >
> > 3 执行r次semrelease（对 &rw.readerSem）
> >
> > 4 释放锁

**源码**

> ```Go
> // Copyright 2009 The Go Authors. All rights reserved.
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
> // An RWMutex is a reader/writer mutual exclusion lock.
> // The lock can be held by an arbitrary number of readers
> // or a single writer.
> // RWMutexes can be created as part of other
> // structures; the zero value for a RWMutex is
> // an unlocked mutex.
> type RWMutex struct {
>    w           Mutex  // held if there are pending writers
>    writerSem   uint32 // semaphore for writers to wait for completing readers
>    readerSem   uint32 // semaphore for readers to wait for completing writers
>    readerCount int32  // number of pending readers
>    readerWait  int32  // number of departing readers
> }
>
> const rwmutexMaxReaders = 1 << 30
>
> // RLock locks rw for reading.
> func (rw *RWMutex) RLock() {
>    if raceenabled {
>       _ = rw.w.state
>       raceDisable()
>    }
>    if atomic.AddInt32(&rw.readerCount, 1) < 0 {
>       // A writer is pending, wait for it.
>       runtime_Semacquire(&rw.readerSem)
>    }
>    if raceenabled {
>       raceEnable()
>       raceAcquire(unsafe.Pointer(&rw.readerSem))
>    }
> }
>
> // RUnlock undoes a single RLock call;
> // it does not affect other simultaneous readers.
> // It is a run-time error if rw is not locked for reading
> // on entry to RUnlock.
> func (rw *RWMutex) RUnlock() {
>    if raceenabled {
>       _ = rw.w.state
>       raceReleaseMerge(unsafe.Pointer(&rw.writerSem))
>       raceDisable()
>    }
>    if r := atomic.AddInt32(&rw.readerCount, -1); r < 0 {
>       if r+1 == 0 || r+1 == -rwmutexMaxReaders {
>          raceEnable()
>          panic("sync: RUnlock of unlocked RWMutex")
>       }
>       // A writer is pending.
>       if atomic.AddInt32(&rw.readerWait, -1) == 0 {
>          // The last reader unblocks the writer.
>          runtime_Semrelease(&rw.writerSem)
>       }
>    }
>    if raceenabled {
>       raceEnable()
>    }
> }
>
> // Lock locks rw for writing.
> // If the lock is already locked for reading or writing,
> // Lock blocks until the lock is available.
> // To ensure that the lock eventually becomes available,
> // a blocked Lock call excludes new readers from acquiring
> // the lock.
> func (rw *RWMutex) Lock() {
>    if raceenabled {
>       _ = rw.w.state
>       raceDisable()
>    }
>    // First, resolve competition with other writers.
>    rw.w.Lock()
>    // Announce to readers there is a pending writer.
>    r := atomic.AddInt32(&rw.readerCount, -rwmutexMaxReaders) + rwmutexMaxReaders
>    // Wait for active readers.
>    if r != 0 && atomic.AddInt32(&rw.readerWait, r) != 0 {
>       runtime_Semacquire(&rw.writerSem)
>    }
>    if raceenabled {
>       raceEnable()
>       raceAcquire(unsafe.Pointer(&rw.readerSem))
>       raceAcquire(unsafe.Pointer(&rw.writerSem))
>    }
> }
>
> // Unlock unlocks rw for writing.  It is a run-time error if rw is
> // not locked for writing on entry to Unlock.
> //
> // As with Mutexes, a locked RWMutex is not associated with a particular
> // goroutine.  One goroutine may RLock (Lock) an RWMutex and then
> // arrange for another goroutine to RUnlock (Unlock) it.
> func (rw *RWMutex) Unlock() {
>    if raceenabled {
>       _ = rw.w.state
>       raceRelease(unsafe.Pointer(&rw.readerSem))
>       raceRelease(unsafe.Pointer(&rw.writerSem))
>       raceDisable()
>    }
>
>    // Announce to readers there is no active writer.
>    r := atomic.AddInt32(&rw.readerCount, rwmutexMaxReaders)
>    if r >= rwmutexMaxReaders {
>       raceEnable()
>       panic("sync: Unlock of unlocked RWMutex")
>    }
>    // Unblock blocked readers, if any.
>    for i := 0; i < int(r); i++ {
>       runtime_Semrelease(&rw.readerSem)
>    }
>    // Allow other writers to proceed.
>    rw.w.Unlock()
>    if raceenabled {
>       raceEnable()
>    }
> }
>
> // RLocker returns a Locker interface that implements
> // the Lock and Unlock methods by calling rw.RLock and rw.RUnlock.
> func (rw *RWMutex) RLocker() Locker {
>    return (*rlocker)(rw)
> }
>
> type rlocker RWMutex
>
> func (r *rlocker) Lock()   { (*RWMutex)(r).RLock() }
> func (r *rlocker) Unlock() { (*RWMutex)(r).RUnlock() }
> ```

