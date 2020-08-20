**简介**

> ...

**源码简介**

> ![img](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/0ec30809-98b6-49e9-86eb-55b477761853/index_files/b1e2c3b1-b17b-4d4b-aafb-40a0ee925fa0.png)
>
> 组成：

> > L locker
>
> > sema syncSema
>
> > waiters uint32
>
> > checker copyCheck  # 检测是否被copy了 --- 保存了创建时的指针，如果被copy了则肯定不同

> ```Go
> // Approximation of syncSema in runtime/sema.go.
> type syncSema struct {
>    lock uintptr
>    head unsafe.Pointer
>    tail unsafe.Pointer
> }
> ```
>
> signalImpl： 实际的signal实现

> > Signal - signalImpl(false)
>
> > Broadcast - signalImpl(true)
> >
> > 区别于py的cond，golang的cond在signal的时候不需要持有cond级别的锁 --- 因为在syncSema级别有锁 = =
> >
> > 因为release不会阻塞，所以使用自旋的方式
> >
> > ```Go
> > for {
> >    old := atomic.LoadUint32(&c.waiters)
> >    if old == 0 {
> >       if raceenabled {
> >          raceEnable()
> >       }
> >       return
> >    }
> >    new := old - 1
> >    if all {
> >       new = 0
> >    }
> >    if atomic.CompareAndSwapUint32(&c.waiters, old, new) {
> >       if raceenabled {
> >          raceEnable()
> >       }
> >       runtime_Syncsemrelease(&c.sema, old-new)
> >       return
> >    }
> > }
> > ```
> >
> > 自旋失败： waiters为0，已经被其他...释放完了 --- 退出即可
> >
> > 自旋成功： CAS成功，那么通过 runtime_Syncsemrelease来执行真正的release

> > > runtime/sema.go//syncsemrelease
> > >
> > > n表示release的数量
> > >
> > > 该操作会加锁... = = 因为 syncSema.head指向的是一个链表（node中包含goroutine的g对象），对链表操作需要被锁保护
> > >
> > > ```go
> > > // syncsemrelease waits for n pairing syncsemacquire on the same semaphore s.
> > > //go:linkname syncsemrelease sync.runtime_Syncsemrelease
> > > func syncsemrelease(s *syncSema, n uint32) {
> > >    lock(&s.lock)
> > >    for n > 0 && s.head != nil && s.head.nrelease < 0 {
> > >       // Have pending acquire, satisfy it.
> > >       wake := s.head
> > >       s.head = wake.next
> > >       if s.head == nil {
> > >          s.tail = nil
> > >       }
> > >       if wake.releasetime != 0 {
> > >          wake.releasetime = cputicks()
> > >       }
> > >       wake.next = nil
> > >       goready(wake.g, 4)
> > >       n--
> > >    }
> > >    if n > 0 {
> > >       // enqueue itself
> > >       w := acquireSudog()
> > >       w.g = getg()
> > >       w.nrelease = int32(n)
> > >       w.next = nil
> > >       w.releasetime = 0
> > >       if s.tail == nil {
> > >          s.head = w
> > >       } else {
> > >          s.tail.next = w
> > >       }
> > >       s.tail = w
> > >       goparkunlock(&s.lock, "semarelease", traceEvGoBlockCond, 3)
> > >       releaseSudog(w)
> > >    } else {
> > >       unlock(&s.lock)
> > >    }
> > > }
> > > ```
> > >
> > > 这里有个很有趣的设定，如果把当前已有的nrelease<0的node都处理完后，还不够传进来的n的话。 则会以当前g构建node（nrelease为剩余值）加到链表尾，同时该g park住。
> > >
> > > --- 会在acquire时处理到该node同时消耗完nrelease时将其wake（当然也从链表中去掉）
> > >
> > > 详见《golang runtime/sema小结_yonka》
> > >
> > > 其实不太理解“多余的release”的由来，毕竟在signal的cond代码部分通过乐观锁保证了release n的n的合理性。

> > 

> > 奇怪的是 这个却需要锁
> >
> > ```go
> > // Wait atomically unlocks c.L and suspends execution
> > // of the calling goroutine.  After later resuming execution,
> > // Wait locks c.L before returning.  Unlike in other systems,
> > // Wait cannot return unless awoken by Broadcast or Signal.
> > //
> > // Because c.L is not locked when Wait first resumes, the caller
> > // typically cannot assume that the condition is true when
> > // Wait returns.  Instead, the caller should Wait in a loop:
> > //
> > //    c.L.Lock()
> > //    for !condition() {
> > //        c.Wait()
> > //    }
> > //    ... make use of condition ...
> > //    c.L.Unlock()
> > //
> > func (c *Cond) Wait() {
> >    c.checker.check()
> >    if raceenabled {
> >       raceDisable()
> >    }
> >    atomic.AddUint32(&c.waiters, 1)
> >    if raceenabled {
> >       raceEnable()
> >    }
> >    c.L.Unlock()
> >    runtime_Syncsemacquire(&c.sema)
> >    c.L.Lock()
> > }
> > ```
> >
> > ```go
> > // syncsemacquire waits for a pairing syncsemrelease on the same semaphore s.
> > //go:linkname syncsemacquire sync.runtime_Syncsemacquire
> > func syncsemacquire(s *syncSema) {
> >    lock(&s.lock)
> >    if s.head != nil && s.head.nrelease > 0 {
> >       // Have pending release, consume it.
> >       var wake *sudog
> >       s.head.nrelease--
> >       if s.head.nrelease == 0 {
> >          wake = s.head
> >          s.head = wake.next
> >          if s.head == nil {
> >             s.tail = nil
> >          }
> >       }
> >       unlock(&s.lock)
> >       if wake != nil {
> >          wake.next = nil
> >          goready(wake.g, 4)
> >       }
> >    } else {
> >       // Enqueue itself.
> >       w := acquireSudog()
> >       w.g = getg()
> >       w.nrelease = -1
> >       w.next = nil
> >       w.releasetime = 0
> >       t0 := int64(0)
> >       if blockprofilerate > 0 {
> >          t0 = cputicks()
> >          w.releasetime = -1
> >       }
> >       if s.tail == nil {
> >          s.head = w
> >       } else {
> >          s.tail.next = w
> >       }
> >       s.tail = w
> >       goparkunlock(&s.lock, "semacquire", traceEvGoBlockCond, 3)
> >       if t0 != 0 {
> >          blockevent(int64(w.releasetime)-t0, 2)
> >       }
> >       releaseSudog(w)
> >    }
> > }
> > ```
> >
> > 会去处理/消耗“多余”的release，并且是优先...，保证不会在有“多余release”存在的情况下新的acquire被加到队尾
> >
> > 如果没有“多余”的release的话，会以当前g构建node加到队尾，同时park当前g...
> >
> > 详见《golang runtime/sema小结_yonka》

**源码**

> ```go
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
> // Cond implements a condition variable, a rendezvous point
> // for goroutines waiting for or announcing the occurrence
> // of an event.
> //
> // Each Cond has an associated Locker L (often a *Mutex or *RWMutex),
> // which must be held when changing the condition and
> // when calling the Wait method.
> //
> // A Cond can be created as part of other structures.
> // A Cond must not be copied after first use.
> type Cond struct {
>    // L is held while observing or changing the condition
>    L Locker
>
>    sema    syncSema
>    waiters uint32 // number of waiters
>    checker copyChecker
> }
>
> // NewCond returns a new Cond with Locker l.
> func NewCond(l Locker) *Cond {
>    return &Cond{L: l}
> }
>
> // Wait atomically unlocks c.L and suspends execution
> // of the calling goroutine.  After later resuming execution,
> // Wait locks c.L before returning.  Unlike in other systems,
> // Wait cannot return unless awoken by Broadcast or Signal.
> //
> // Because c.L is not locked when Wait first resumes, the caller
> // typically cannot assume that the condition is true when
> // Wait returns.  Instead, the caller should Wait in a loop:
> //
> //    c.L.Lock()
> //    for !condition() {
> //        c.Wait()
> //    }
> //    ... make use of condition ...
> //    c.L.Unlock()
> //
> func (c *Cond) Wait() {
>    c.checker.check()
>    if raceenabled {
>       raceDisable()
>    }
>    atomic.AddUint32(&c.waiters, 1)
>    if raceenabled {
>       raceEnable()
>    }
>    c.L.Unlock()
>    runtime_Syncsemacquire(&c.sema)
>    c.L.Lock()
> }
>
> // Signal wakes one goroutine waiting on c, if there is any.
> //
> // It is allowed but not required for the caller to hold c.L
> // during the call.
> func (c *Cond) Signal() {
>    c.signalImpl(false)
> }
>
> // Broadcast wakes all goroutines waiting on c.
> //
> // It is allowed but not required for the caller to hold c.L
> // during the call.
> func (c *Cond) Broadcast() {
>    c.signalImpl(true)
> }
>
> func (c *Cond) signalImpl(all bool) {
>    c.checker.check()
>    if raceenabled {
>       raceDisable()
>    }
>    for {
>       old := atomic.LoadUint32(&c.waiters)
>       if old == 0 {
>          if raceenabled {
>             raceEnable()
>          }
>          return
>       }
>       new := old - 1
>       if all {
>          new = 0
>       }
>       if atomic.CompareAndSwapUint32(&c.waiters, old, new) {
>          if raceenabled {
>             raceEnable()
>          }
>          runtime_Syncsemrelease(&c.sema, old-new)
>          return
>       }
>    }
> }
>
> // copyChecker holds back pointer to itself to detect object copying.
> type copyChecker uintptr
>
> func (c *copyChecker) check() {
>    if uintptr(*c) != uintptr(unsafe.Pointer(c)) &&
>       !atomic.CompareAndSwapUintptr((*uintptr)(c), 0, uintptr(unsafe.Pointer(c))) &&
>       uintptr(*c) != uintptr(unsafe.Pointer(c)) {
>       panic("sync.Cond is copied")
>    }
> }
> ```