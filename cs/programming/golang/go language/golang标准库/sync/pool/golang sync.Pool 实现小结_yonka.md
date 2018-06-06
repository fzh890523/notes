**简介**

> ...
>
> 池化 - 避免大量分配，同时也一定程度上削除波峰带来的影响（降下来...）
>
> 池中元素随时可能被删除，如果只有池中有引用的话，可能随时会被GC
>
> goroutine安全
>
> 使用场景：
>
> 1 合适场景

> > ```Go
> > // An example of good use of a Pool is in the fmt package, which maintains a
> > // dynamically-sized store of temporary output buffers. The store scales under
> > // load (when many goroutines are actively printing) and shrinks when
> > // quiescent.
> > ```

> 2 不合适场景

> > ```Go
> > // On the other hand, a free list maintained as part of a short-lived object is
> > // not a suitable use for a Pool, since the overhead does not amortize well in
> > // that scenario. It is more efficient to have such objects implement their own
> > // free list.
> > ```

> 为了并发做的努力：

> > 每个pool其实有当前GOMAXPROCS作为length/cap的[]*poolLocal的slice，包括一个private和一个shared的[]，put和get都会首先尝试从自己p的pid对应（作为index查找）那个poolLocal的private，如果private不满足则考虑shared，shared还不行（只会发生在get）则尝试从其他p对应的poolLocal里偷... = =

**源码简介**

> ...
>
> ![img]()
>
> allPools []*Pool

> > 全局的Pool列表
> >
> > 会在poolCleanup中清除，而poolCleanup注册到 runtime/mgc/poolcleanup，该方法会在gc时执行（gc - clearpools - poolcleanup）

> allPoolsMu Mutex

> > 用来锁对allPools的操作 - pinSlow中

> indexLocal(l unsafe.Pointer, i int) *poolLocal

> > 把l（来自Pool.local）这个指针强转为 *[1000000]poolLocal，然后取第i个poolLocal

> init

> > 把poolCleanup注册到 runtime/mgc/poolcleanup

> poolCleanup

> > 清空已有pool操作
> >
> > 把allPools中每个index都置为nil。 同时把原来每个index上的Pool的local里的private置为nil，而且local的shared的每个都置为nil然后把shared置为nil，然后把local置为nil、localsize置为0。 最后把allPools初始化为 []*Pool{}
>
> 
>
> runtime_registerPoolCleanup(cleanup func())

> > 实际会链接 runtime/registerPoolCleanup，把 poolCleanup注册到...

> runtime_procPin() int

> > 链接 runtime/procPin
> >
> > 取当前g的m，将其locks++，然后返回该m的p的id
> >
> > // 好像是根据m的locks是否>0来确定是否允许争用的
> >
> > 

> runtime_procUnpin

> > 链接 runtime/procUnpin
> >
> > 取当前g的m，将其locks--

> Pool struct

> > local unsafe.Pointer，实际为 *[1000000]poolLocal  // 这个1000000无法理解，不知道意义何在...
> >
> > localSize uintptr，对前者的计数
> >
> > New func() interface{}，new出pooled item的函数...
> >
> > Get

> > > p.pin()获取到（当前p）对应的pooLocal
> >
> > > 取l.private并且把l.private置为nil，然后做unpin
> >
> > > 如果取到的不为nil则返回，否则继续。  // 优先使用各p自己的，避免竞争/加锁
> >
> > > 对l加锁（l.Lock() --- l.Mutex.Lock()）
> >
> > > 取l.shared的最后一个
> >
> > > 释放锁
> >
> > > 如果取到的不为nil则返回，否则继续。
> >
> > > 调p.getSlow()
> >
> > > ```Go
> > > // Get selects an arbitrary item from the Pool, removes it from the
> > > // Pool, and returns it to the caller.
> > > // Get may choose to ignore the pool and treat it as empty.
> > > // Callers should not assume any relation between values passed to Put and
> > > // the values returned by Get.
> > > //
> > > // If Get would otherwise return nil and p.New is non-nil, Get returns
> > > // the result of calling p.New.
> > > func (p *Pool) Get() interface{} {
> > >    if raceenabled {
> > >       if p.New != nil {
> > >          return p.New()
> > >       }
> > >       return nil
> > >    }
> > >    l := p.pin()
> > >    x := l.private
> > >    l.private = nil
> > >    runtime_procUnpin()
> > >    if x != nil {
> > >       return x
> > >    }
> > >    l.Lock()
> > >    last := len(l.shared) - 1
> > >    if last >= 0 {
> > >       x = l.shared[last]
> > >       l.shared = l.shared[:last]
> > >    }
> > >    l.Unlock()
> > >    if x != nil {
> > >       return x
> > >    }
> > >    return p.getSlow()
> > > }
> > > ```

> > getSlow

> > > // 尝试去其他proc偷一个，如果都偷不到的话，就只能new一个了
> > >
> > > 取pid、localsize、local
> > >
> > > 遍历 index 0 - size - 1，取对应的 (pid+i+1) % int(size) 的localPool，然后加锁尝试偷最后一个，偷到则break，否则继续循环。
> > >
> > > ```Go
> > > func (p *Pool) getSlow() (x interface{}) {
> > >    // See the comment in pin regarding ordering of the loads.
> > >    size := atomic.LoadUintptr(&p.localSize) // load-acquire
> > >    local := p.local                         // load-consume
> > >    // Try to steal one element from other procs.
> > >    pid := runtime_procPin()
> > >    runtime_procUnpin()
> > >    for i := 0; i < int(size); i++ {
> > >       l := indexLocal(local, (pid+i+1)%int(size))
> > >       l.Lock()
> > >       last := len(l.shared) - 1
> > >       if last >= 0 {
> > >          x = l.shared[last]
> > >          l.shared = l.shared[:last]
> > >          l.Unlock()
> > >          break
> > >       }
> > >       l.Unlock()
> > >    }
> > >
> > >    if x == nil && p.New != nil {
> > >       x = p.New()
> > >    }
> > >    return x
> > > }
> > > ```

> > pin

> > > 把当前g pin到proc（其实是禁止对应的m被争用），获取该操作返回的p的id - pid。
> > >
> > > 读p.localSize，如果pid小于该size则直接取第pid个。否则的话调p.pinSlow()获取。
> > >
> > > \# pin返回后是pined状态
> > >
> > > ```Go
> > > // pin pins the current goroutine to P, disables preemption and returns poolLocal pool for the P.
> > > // Caller must call runtime_procUnpin() when done with the pool.
> > > func (p *Pool) pin() *poolLocal {
> > >    pid := runtime_procPin()
> > >    // In pinSlow we store to localSize and then to local, here we load in opposite order.
> > >    // Since we've disabled preemption, GC can not happen in between.
> > >    // Thus here we must observe local at least as large localSize.
> > >    // We can observe a newer/larger local, it is fine (we must observe its zero-initialized-ness).
> > >    s := atomic.LoadUintptr(&p.localSize) // load-acquire
> > >    l := p.local                          // load-consume
> > >    if uintptr(pid) < s {
> > >       return indexLocal(l, pid)
> > >    }
> > >    return p.pinSlow()
> > > }
> > > ```

> > pinSlow

> > > 做unpin操作，释放对p（m）的pin。
> > >
> > > 锁allPools
> > >
> > > 重新pin（不能先pin后锁）
> > >
> > > 再做一次pin里的尝试，如果OK则返回。 否则的话继续。
> > >
> > > 如果p.local == nil ，则把该p append到allPools。 创建size为当前GOMAXPROCS数量的[]poolLocal，然后把指针赋给p.local，同时给p.localSize赋值，然后返回第pid个。
> > >
> > > \# 其实，运行到这个，一般两种情况：  初始、GOMAXPROCS在运行时发生变化（一般是变大，才可能出现pid对应的item不存在），这里等于会做清空操作
> > >
> > > \# pinSlow返回后也是pined状态
> > >
> > > ```Go
> > > func (p *Pool) pinSlow() *poolLocal {
> > >    // Retry under the mutex.
> > >    // Can not lock the mutex while pinned.
> > >    runtime_procUnpin()
> > >    allPoolsMu.Lock()
> > >    defer allPoolsMu.Unlock()
> > >    pid := runtime_procPin()
> > >    // poolCleanup won't be called while we are pinned.
> > >    s := p.localSize
> > >    l := p.local
> > >    if uintptr(pid) < s {
> > >       return indexLocal(l, pid)
> > >    }
> > >    if p.local == nil {
> > >       allPools = append(allPools, p)
> > >    }
> > >    // If GOMAXPROCS changes between GCs, we re-allocate the array and lose the old one.
> > >    size := runtime.GOMAXPROCS(0)
> > >    local := make([]poolLocal, size)
> > >    atomic.StorePointer((*unsafe.Pointer)(&p.local), unsafe.Pointer(&local[0])) // store-release
> > >    atomic.StoreUintptr(&p.localSize, uintptr(size))                            // store-release
> > >    return &local[pid]
> > > }
> > > ```

> > Put

> > > 同样，先尝试加到private： pin取到对应的localPool、如果private == nil则赋值、unpin。 如果前面成功则 return。
> > >
> > > 加锁，加到l.shared。
> > >
> > > ```Go
> > > // Put adds x to the pool.
> > > func (p *Pool) Put(x interface{}) {
> > >    if raceenabled {
> > >       // Under race detector the Pool degenerates into no-op.
> > >       // It's conforming, simple and does not introduce excessive
> > >       // happens-before edges between unrelated goroutines.
> > >       return
> > >    }
> > >    if x == nil {
> > >       return
> > >    }
> > >    l := p.pin()
> > >    if l.private == nil {
> > >       l.private = x
> > >       x = nil
> > >    }
> > >    runtime_procUnpin()
> > >    if x == nil {
> > >       return
> > >    }
> > >    l.Lock()
> > >    l.shared = append(l.shared, x)
> > >    l.Unlock()
> > > }
> > > ```

> poolLocal struct

> > private interface{}，只能被对应的p使用
> >
> > shared []interface{}，可以被任意p使用
> >
> > Mutex，保护对shared的访问
> >
> > pad [123]byte
> >
> > 每个p一个的pool，作为Pool.local中的item

**源码**

> ```Go
> // Copyright 2013 The Go Authors. All rights reserved.
> // Use of this source code is governed by a BSD-style
> // license that can be found in the LICENSE file.
>
> package sync
>
> import (
>    "runtime"
>    "sync/atomic"
>    "unsafe"
> )
>
> // A Pool is a set of temporary objects that may be individually saved and
> // retrieved.
> //
> // Any item stored in the Pool may be removed automatically at any time without
> // notification. If the Pool holds the only reference when this happens, the
> // item might be deallocated.
> //
> // A Pool is safe for use by multiple goroutines simultaneously.
> //
> // Pool's purpose is to cache allocated but unused items for later reuse,
> // relieving pressure on the garbage collector. That is, it makes it easy to
> // build efficient, thread-safe free lists. However, it is not suitable for all
> // free lists.
> //
> // An appropriate use of a Pool is to manage a group of temporary items
> // silently shared among and potentially reused by concurrent independent
> // clients of a package. Pool provides a way to amortize allocation overhead
> // across many clients.
> //
> // An example of good use of a Pool is in the fmt package, which maintains a
> // dynamically-sized store of temporary output buffers. The store scales under
> // load (when many goroutines are actively printing) and shrinks when
> // quiescent.
> //
> // On the other hand, a free list maintained as part of a short-lived object is
> // not a suitable use for a Pool, since the overhead does not amortize well in
> // that scenario. It is more efficient to have such objects implement their own
> // free list.
> //
> type Pool struct {
>    local     unsafe.Pointer // local fixed-size per-P pool, actual type is [P]poolLocal
>    localSize uintptr        // size of the local array
>
>    // New optionally specifies a function to generate
>    // a value when Get would otherwise return nil.
>    // It may not be changed concurrently with calls to Get.
>    New func() interface{}
> }
>
> // Local per-P Pool appendix.
> type poolLocal struct {
>    private interface{}   // Can be used only by the respective P.
>    shared  []interface{} // Can be used by any P.
>    Mutex                 // Protects shared.
>    pad     [128]byte     // Prevents false sharing.
> }
>
> // Put adds x to the pool.
> func (p *Pool) Put(x interface{}) {
>    if raceenabled {
>       // Under race detector the Pool degenerates into no-op.
>       // It's conforming, simple and does not introduce excessive
>       // happens-before edges between unrelated goroutines.
>       return
>    }
>    if x == nil {
>       return
>    }
>    l := p.pin()
>    if l.private == nil {
>       l.private = x
>       x = nil
>    }
>    runtime_procUnpin()
>    if x == nil {
>       return
>    }
>    l.Lock()
>    l.shared = append(l.shared, x)
>    l.Unlock()
> }
>
> // Get selects an arbitrary item from the Pool, removes it from the
> // Pool, and returns it to the caller.
> // Get may choose to ignore the pool and treat it as empty.
> // Callers should not assume any relation between values passed to Put and
> // the values returned by Get.
> //
> // If Get would otherwise return nil and p.New is non-nil, Get returns
> // the result of calling p.New.
> func (p *Pool) Get() interface{} {
>    if raceenabled {
>       if p.New != nil {
>          return p.New()
>       }
>       return nil
>    }
>    l := p.pin()
>    x := l.private
>    l.private = nil
>    runtime_procUnpin()
>    if x != nil {
>       return x
>    }
>    l.Lock()
>    last := len(l.shared) - 1
>    if last >= 0 {
>       x = l.shared[last]
>       l.shared = l.shared[:last]
>    }
>    l.Unlock()
>    if x != nil {
>       return x
>    }
>    return p.getSlow()
> }
>
> func (p *Pool) getSlow() (x interface{}) {
>    // See the comment in pin regarding ordering of the loads.
>    size := atomic.LoadUintptr(&p.localSize) // load-acquire
>    local := p.local                         // load-consume
>    // Try to steal one element from other procs.
>    pid := runtime_procPin()
>    runtime_procUnpin()
>    for i := 0; i < int(size); i++ {
>       l := indexLocal(local, (pid+i+1)%int(size))
>       l.Lock()
>       last := len(l.shared) - 1
>       if last >= 0 {
>          x = l.shared[last]
>          l.shared = l.shared[:last]
>          l.Unlock()
>          break
>       }
>       l.Unlock()
>    }
>
>    if x == nil && p.New != nil {
>       x = p.New()
>    }
>    return x
> }
>
> // pin pins the current goroutine to P, disables preemption and returns poolLocal pool for the P.
> // Caller must call runtime_procUnpin() when done with the pool.
> func (p *Pool) pin() *poolLocal {
>    pid := runtime_procPin()
>    // In pinSlow we store to localSize and then to local, here we load in opposite order.
>    // Since we've disabled preemption, GC can not happen in between.
>    // Thus here we must observe local at least as large localSize.
>    // We can observe a newer/larger local, it is fine (we must observe its zero-initialized-ness).
>    s := atomic.LoadUintptr(&p.localSize) // load-acquire
>    l := p.local                          // load-consume
>    if uintptr(pid) < s {
>       return indexLocal(l, pid)
>    }
>    return p.pinSlow()
> }
>
> func (p *Pool) pinSlow() *poolLocal {
>    // Retry under the mutex.
>    // Can not lock the mutex while pinned.
>    runtime_procUnpin()
>    allPoolsMu.Lock()
>    defer allPoolsMu.Unlock()
>    pid := runtime_procPin()
>    // poolCleanup won't be called while we are pinned.
>    s := p.localSize
>    l := p.local
>    if uintptr(pid) < s {
>       return indexLocal(l, pid)
>    }
>    if p.local == nil {
>       allPools = append(allPools, p)
>    }
>    // If GOMAXPROCS changes between GCs, we re-allocate the array and lose the old one.
>    size := runtime.GOMAXPROCS(0)
>    local := make([]poolLocal, size)
>    atomic.StorePointer((*unsafe.Pointer)(&p.local), unsafe.Pointer(&local[0])) // store-release
>    atomic.StoreUintptr(&p.localSize, uintptr(size))                            // store-release
>    return &local[pid]
> }
>
> func poolCleanup() {
>    // This function is called with the world stopped, at the beginning of a garbage collection.
>    // It must not allocate and probably should not call any runtime functions.
>    // Defensively zero out everything, 2 reasons:
>    // 1. To prevent false retention of whole Pools.
>    // 2. If GC happens while a goroutine works with l.shared in Put/Get,
>    //    it will retain whole Pool. So next cycle memory consumption would be doubled.
>    for i, p := range allPools {
>       allPools[i] = nil
>       for i := 0; i < int(p.localSize); i++ {
>          l := indexLocal(p.local, i)
>          l.private = nil
>          for j := range l.shared {
>             l.shared[j] = nil
>          }
>          l.shared = nil
>       }
>       p.local = nil
>       p.localSize = 0
>    }
>    allPools = []*Pool{}
> }
>
> var (
>    allPoolsMu Mutex
>    allPools   []*Pool
> )
>
> func init() {
>    runtime_registerPoolCleanup(poolCleanup)
> }
>
> func indexLocal(l unsafe.Pointer, i int) *poolLocal {
>    return &(*[1000000]poolLocal)(l)[i]
> }
>
> // Implemented in runtime.
> func runtime_registerPoolCleanup(cleanup func())
> func runtime_procPin() int
> func runtime_procUnpin()
> ```