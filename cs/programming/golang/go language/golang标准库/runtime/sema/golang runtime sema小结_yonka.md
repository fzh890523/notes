

**简介**

> sema vs syncSema：

> > 共同点： 都是用队列的思想来保存waiters信息
>
> > 区别： 

> > > 1 sema用的是全局变量来保存一组固定数量的链表，也即可能会有很多sema共用一个队列； syncSema则是自己的结构里包含链表，也即队列自用
> >
> > > 2 sema的“多余”的release会“积攒”在该sema uint32值中； syncSema会“积攒”在 以执行release操作的goroutine和“多余值”组成的sudog node中
> >
> > > 3 sema在做”多余“的release时不会block执行者； 而syncSema则会（把自身...作为节点入队）
> >
> > > 4 基于3 - sema的队列只存waiters； 而syncSema的队列则可能是 waiters 或 执行多余release的“releasers”
> >
> > > 5 基于1 - sema因为多个sema共用队列和waiter计数，所以可能出现较多的没必要的遍历队列操作； syncSema则不会
> >
> > > 6 sema的release每次的效果是 release 1； 而syncSema的release则是release n，n可以>1
> >
> > > 7 严格的说，sema的链表并不是队列，因为不符合FILO； sema则是队列

**源码简介**

> ![img](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/9bc39f34-7af9-4545-89cf-c364497bbf3f/index_files/ab5eb32b-9291-459f-983c-6278d05810c4.png)
>
> ```Go
> type semaRoot struct {
>    lock  mutex
>    head  *sudog
>    tail  *sudog
>    nwait uint32 // Number of waiters. Read w/o the lock.
> }
>
> // Prime to not correlate with any user patterns.
> const semTabSize = 251
>
> var semtable [semTabSize]struct {
>    root semaRoot
>    pad  [_CacheLineSize - unsafe.Sizeof(semaRoot{})]byte
> }
> ```

> ```Go
> //go:linkname sync_runtime_Semacquire sync.runtime_Semacquire
> func sync_runtime_Semacquire(addr *uint32) {
>    semacquire(addr, true)
> }
>
> //go:linkname net_runtime_Semacquire net.runtime_Semacquire
> func net_runtime_Semacquire(addr *uint32) {
>    semacquire(addr, true)
> }
> ```
>
> ```Go
> // Known to compiler.
> // Changes here must also be made in src/cmd/internal/gc/select.go's selecttype.
> type sudog struct {
>    g           *g
>    selectdone  *uint32
>    next        *sudog
>    prev        *sudog
>    elem        unsafe.Pointer // data element
>    releasetime int64
>    nrelease    int32  // -1 for acquire
>    waitlink    *sudog // g.waiting list
> }
> ```
>
> ```Go
> // Synchronous semaphore for sync.Cond.
> type syncSema struct {
>    lock mutex
>    head *sudog
>    tail *sudog
> }
> ```
>
> 251长度的semaRoot数组
>
> semaRoot中实际是一个带锁的链表，node为 sudog的指针，sudog wrap了goroutine（g），以及要acquire的那个指针（elem）
>
> 实际acquire的都是指针，会根据指针地址右移+取模的方式取到对应的semaRoot，然后（在semaRoot锁中）先尝试通过cas获得sema，成功则释放锁退出循环，失败则把当前g+acquire的指针组成的sudog加到semaRoot的队尾然后park等待唤醒。 每次唤醒后通过cas操作尝试获得sema...，失败则重复该循环。
>
> 要注意的是：

> > 像 net_runtime_Semacquire, sync_runtime_Semacquire, net_runtime_Semrelease, sync_runtime_Semrelease 都是调的semacquire和semrelease，分别链接到 sync/runtime_Sem* 和 net/runtime_Sem*
>
> > 而 syncsem* 则链接到 sync/runtime_Sem*

> (root *semaRoot).queue(add *uint32, s *sudog)

> > 1 把当前g赋给s.g，把addr赋给s.elem
>
> > 2 然后s加到root队尾
>
> > ```Go
> > func (root *semaRoot) queue(addr *uint32, s *sudog) {
> >    s.g = getg()
> >    s.elem = unsafe.Pointer(addr)
> >    s.next = nil
> >    s.prev = root.tail
> >    if root.tail != nil {
> >       root.tail.next = s
> >    } else {
> >       root.head = s
> >    }
> >    root.tail = s
> > }
> > ```

> (root *semaRoot).dequeue(s *sudog)

> > 除了s位于队首/队尾的情况需要处理root.head/root.tail外，就是简单的 s.prev.next = s.next
> >
> > ```Go
> > func (root *semaRoot) dequeue(s *sudog) {
> >    if s.next != nil {
> >       s.next.prev = s.prev
> >    } else {
> >       root.tail = s.prev
> >    }
> >    if s.prev != nil {
> >       s.prev.next = s.next
> >    } else {
> >       root.head = s.next
> >    }
> >    s.elem = nil
> >    s.next = nil
> >    s.prev = nil
> > }
> > ```

> semacquire(addr *uint32, profile bool)

> > \# profile决定是否做profile
> >
> > 1 获取当前gp，检查 gp == gp.m.curg 是否成立，否则panic
> >
> > 2 cas方式尝试获得，成功则退出
> >
> > 3 获取s（sudog），根据addr取得对应的root（semaRoot）
> >
> > \# if profile则做...
> >
> > 4 循环里：

> > > 1 获得root的锁
> > >
> > > 2 root.nwait加一
> > >
> > > 3 再次cas方式尝试获得，成功则 root.nwait减一 - 释放锁 - 退出循环
> > >
> > > 4 把addr、s作为node加到root的队尾
> > >
> > > 5 park当前goroutine，等待被唤醒（唤醒的时候应该做了 root.nwait减一、释放root的锁）
> > >
> > > 6 被唤醒后cas方式尝试获得，成功则退出循环，否则继续循环

> > 5 if s.releasetime > 0则block一段时间 = =
> >
> > 6 释放s
> >
> > ```Go
> > // Called from runtime.
> > func semacquire(addr *uint32, profile bool) {
> >    gp := getg()
> >    if gp != gp.m.curg {
> >       throw("semacquire not on the G stack")
> >    }
> >
> >    // Easy case.
> >    if cansemacquire(addr) {
> >       return
> >    }
> >
> >    // Harder case:
> >    // increment waiter count
> >    // try cansemacquire one more time, return if succeeded
> >    // enqueue itself as a waiter
> >    // sleep
> >    // (waiter descriptor is dequeued by signaler)
> >    s := acquireSudog()
> >    root := semroot(addr)
> >    t0 := int64(0)
> >    s.releasetime = 0
> >    if profile && blockprofilerate > 0 {
> >       t0 = cputicks()
> >       s.releasetime = -1
> >    }
> >    for {
> >       lock(&root.lock)
> >       // Add ourselves to nwait to disable "easy case" in semrelease.
> >       xadd(&root.nwait, 1)
> >       // Check cansemacquire to avoid missed wakeup.
> >       if cansemacquire(addr) {
> >          xadd(&root.nwait, -1)
> >          unlock(&root.lock)
> >          break
> >       }
> >       // Any semrelease after the cansemacquire knows we're waiting
> >       // (we set nwait above), so go to sleep.
> >       root.queue(addr, s)
> >       goparkunlock(&root.lock, "semacquire", traceEvGoBlockSync, 4)
> >       if cansemacquire(addr) {
> >          break
> >       }
> >    }
> >    if s.releasetime > 0 {
> >       blockevent(int64(s.releasetime)-t0, 3)
> >    }
> >    releaseSudog(s)
> > }
> > ```

> semrelease(addr *unit32)

> > 1 根据addr获取root
> >
> > 2 把addr的值加1
> >
> > 3 查看root.nwait是否为0，是则退出（没有需要唤醒的）
> >
> > 4 获得root的锁
> >
> > 5 再次确认root.nwait是否为0，是则释放锁并退出
> >
> > 6 遍历链表找到s.elem == addr（做Pointer转换后的）的，将其从链表删除并把root的nwait减一
> >
> > 7 释放锁
> >
> > 8 如果有查找到符合的，并且其 releasetime不为0，则置位 cputicks()，然后将其（s.g）唤醒
> >
> > ```Go
> > func semrelease(addr *uint32) {
> >    root := semroot(addr)
> >    xadd(addr, 1)
> >
> >    // Easy case: no waiters?
> >    // This check must happen after the xadd, to avoid a missed wakeup
> >    // (see loop in semacquire).
> >    if atomicload(&root.nwait) == 0 {
> >       return
> >    }
> >
> >    // Harder case: search for a waiter and wake it.
> >    lock(&root.lock)
> >    if atomicload(&root.nwait) == 0 {
> >       // The count is already consumed by another goroutine,
> >       // so no need to wake up another goroutine.
> >       unlock(&root.lock)
> >       return
> >    }
> >    s := root.head
> >    for ; s != nil; s = s.next {
> >       if s.elem == unsafe.Pointer(addr) {
> >          xadd(&root.nwait, -1)
> >          root.dequeue(s)
> >          break
> >       }
> >    }
> >    unlock(&root.lock)
> >    if s != nil {
> >       if s.releasetime != 0 {
> >          s.releasetime = cputicks()
> >       }
> >       goready(s.g, 4)
> >    }
> > }
> > ```

> cansemacquire(addr *uint32)

> > ```Go
> > func cansemacquire(addr *uint32) bool {
> >    for {
> >       v := atomicload(addr)
> >       if v == 0 {
> >          return false
> >       }
> >       if cas(addr, v, v-1) {
> >          return true
> >       }
> >    }
> > }
> > ```
> >
> > 循环里CAS减一直到值为0 = =

> semroot(addr *uimt32) *semaRoot

> > ```Go
> > func semroot(addr *uint32) *semaRoot {
> >    return &semtable[(uintptr(unsafe.Pointer(addr))>>3)%semTabSize].root
> > }
> > ```
> >
> > 根据指针 右移 + 取模 得到index，继而...

> syncseamacquire(s *syncSema)

> > \# 大体上是入队操作
> >
> > \# 如果有“多余”的release，则进行“抵消”而无需入队
> >
> > \# sudog的 nrelease > 0 则为 releaser node； <0（只会为-1）则为 waiter node --- 不会 == 0
> >
> > \# sudog的 releasetime只有当profile时才可能不为0
> >
> > ```Go
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
> > 1 获得sema的锁
> >
> > 2 先尝试消耗“多余”的release，如果队首是“releaser”的话，则将其nrelease减一（如果为0了则出队）然后释放锁（如果有出队releaser则将其唤醒）退出
> >
> > 3 如果没有...，则自身入队

> > > 1 获得s sudog，把当前g赋给s.g，nrelease初始化为-1，releasetime初始化为0
> > >
> > > \# 如果profile，则做profile相关...
> > >
> > > 2 把s入队
> > >
> > > 3 park，等待唤醒
> > >
> > > \# 被唤醒后
> > >
> > > \# 如果有profile则...
> > >
> > > 4 释放s

> syncsemrelease(s *syncSema, n unit32)

> > ```Go
> > // syncsemrelease waits for n pairing syncsemacquire on the same semaphore s.
> > //go:linkname syncsemrelease sync.runtime_Syncsemrelease
> > func syncsemrelease(s *syncSema, n uint32) {
> >    lock(&s.lock)
> >    for n > 0 && s.head != nil && s.head.nrelease < 0 {
> >       // Have pending acquire, satisfy it.
> >       wake := s.head
> >       s.head = wake.next
> >       if s.head == nil {
> >          s.tail = nil
> >       }
> >       if wake.releasetime != 0 {
> >          wake.releasetime = cputicks()
> >       }
> >       wake.next = nil
> >       goready(wake.g, 4)
> >       n--
> >    }
> >    if n > 0 {
> >       // enqueue itself
> >       w := acquireSudog()
> >       w.g = getg()
> >       w.nrelease = int32(n)
> >       w.next = nil
> >       w.releasetime = 0
> >       if s.tail == nil {
> >          s.head = w
> >       } else {
> >          s.tail.next = w
> >       }
> >       s.tail = w
> >       goparkunlock(&s.lock, "semarelease", traceEvGoBlockCond, 3)
> >       releaseSudog(w)
> >    } else {
> >       unlock(&s.lock)
> >    }
> > }
> > ```
> >
> > 1 获得sema的锁
> >
> > 2 处理waiters，如果有则在循环里“消耗”waiter，直到waiters处理完 或者 n处理完

> > > 1 取waiter
> >
> > > 2 出队
> >
> > > 3 唤醒
> >
> > > 4 n--

> > 3 如果没有waiters了但没被“消耗”完，也即剩余n>0，则自身入队。 否则退出

> > > 1 获取s sudog
> > >
> > > 2 把自身g赋给s.g，nrelease初始化为n，releasetime初始化为0
> > >
> > > 3 s入队
> > >
> > > 4 park，等待唤醒
> > >
> > > \# 被唤醒后
> > >
> > > 5 释放s

> syncsemcheck(sz uintptr)

> > 链接到 sync.runtime__Syncsemcheck
> >
> > ```Go
> > //go:linkname syncsemcheck sync.runtime_Syncsemcheck
> > func syncsemcheck(sz uintptr) {
> >    if sz != unsafe.Sizeof(syncSema{}) {
> >       print("runtime: bad syncSema size - sync=", sz, " runtime=", unsafe.Sizeof(syncSema{}), "\n")
> >       throw("bad syncSema size")
> >    }
> > }
> > ```
> >
> > 在sync/runtime.go//init()中调了
> >
> > ```Go
> > func init() {
> >    var s syncSema
> >    runtime_Syncsemcheck(unsafe.Sizeof(s))
> > }
> > ```

**源码**

> ```Go
> // Copyright 2009 The Go Authors. All rights reserved.
> // Use of this source code is governed by a BSD-style
> // license that can be found in the LICENSE file.
>
> // Semaphore implementation exposed to Go.
> // Intended use is provide a sleep and wakeup
> // primitive that can be used in the contended case
> // of other synchronization primitives.
> // Thus it targets the same goal as Linux's futex,
> // but it has much simpler semantics.
> //
> // That is, don't think of these as semaphores.
> // Think of them as a way to implement sleep and wakeup
> // such that every sleep is paired with a single wakeup,
> // even if, due to races, the wakeup happens before the sleep.
> //
> // See Mullender and Cox, ``Semaphores in Plan 9,''
> // http://swtch.com/semaphore.pdf
>
> package runtime
>
> import "unsafe"
>
> // Asynchronous semaphore for sync.Mutex.
>
> type semaRoot struct {
>    lock  mutex
>    head  *sudog
>    tail  *sudog
>    nwait uint32 // Number of waiters. Read w/o the lock.
> }
>
> // Prime to not correlate with any user patterns.
> const semTabSize = 251
>
> var semtable [semTabSize]struct {
>    root semaRoot
>    pad  [_CacheLineSize - unsafe.Sizeof(semaRoot{})]byte
> }
>
> //go:linkname sync_runtime_Semacquire sync.runtime_Semacquire
> func sync_runtime_Semacquire(addr *uint32) {
>    semacquire(addr, true)
> }
>
> //go:linkname net_runtime_Semacquire net.runtime_Semacquire
> func net_runtime_Semacquire(addr *uint32) {
>    semacquire(addr, true)
> }
>
> //go:linkname sync_runtime_Semrelease sync.runtime_Semrelease
> func sync_runtime_Semrelease(addr *uint32) {
>    semrelease(addr)
> }
>
> //go:linkname net_runtime_Semrelease net.runtime_Semrelease
> func net_runtime_Semrelease(addr *uint32) {
>    semrelease(addr)
> }
>
> // Called from runtime.
> func semacquire(addr *uint32, profile bool) {
>    gp := getg()
>    if gp != gp.m.curg {
>       throw("semacquire not on the G stack")
>    }
>
>    // Easy case.
>    if cansemacquire(addr) {
>       return
>    }
>
>    // Harder case:
>    // increment waiter count
>    // try cansemacquire one more time, return if succeeded
>    // enqueue itself as a waiter
>    // sleep
>    // (waiter descriptor is dequeued by signaler)
>    s := acquireSudog()
>    root := semroot(addr)
>    t0 := int64(0)
>    s.releasetime = 0
>    if profile && blockprofilerate > 0 {
>       t0 = cputicks()
>       s.releasetime = -1
>    }
>    for {
>       lock(&root.lock)
>       // Add ourselves to nwait to disable "easy case" in semrelease.
>       xadd(&root.nwait, 1)
>       // Check cansemacquire to avoid missed wakeup.
>       if cansemacquire(addr) {
>          xadd(&root.nwait, -1)
>          unlock(&root.lock)
>          break
>       }
>       // Any semrelease after the cansemacquire knows we're waiting
>       // (we set nwait above), so go to sleep.
>       root.queue(addr, s)
>       goparkunlock(&root.lock, "semacquire", traceEvGoBlockSync, 4)
>       if cansemacquire(addr) {
>          break
>       }
>    }
>    if s.releasetime > 0 {
>       blockevent(int64(s.releasetime)-t0, 3)
>    }
>    releaseSudog(s)
> }
>
> func semrelease(addr *uint32) {
>    root := semroot(addr)
>    xadd(addr, 1)
>
>    // Easy case: no waiters?
>    // This check must happen after the xadd, to avoid a missed wakeup
>    // (see loop in semacquire).
>    if atomicload(&root.nwait) == 0 {
>       return
>    }
>
>    // Harder case: search for a waiter and wake it.
>    lock(&root.lock)
>    if atomicload(&root.nwait) == 0 {
>       // The count is already consumed by another goroutine,
>       // so no need to wake up another goroutine.
>       unlock(&root.lock)
>       return
>    }
>    s := root.head
>    for ; s != nil; s = s.next {
>       if s.elem == unsafe.Pointer(addr) {
>          xadd(&root.nwait, -1)
>          root.dequeue(s)
>          break
>       }
>    }
>    unlock(&root.lock)
>    if s != nil {
>       if s.releasetime != 0 {
>          s.releasetime = cputicks()
>       }
>       goready(s.g, 4)
>    }
> }
>
> func semroot(addr *uint32) *semaRoot {
>    return &semtable[(uintptr(unsafe.Pointer(addr))>>3)%semTabSize].root
> }
>
> func cansemacquire(addr *uint32) bool {
>    for {
>       v := atomicload(addr)
>       if v == 0 {
>          return false
>       }
>       if cas(addr, v, v-1) {
>          return true
>       }
>    }
> }
>
> func (root *semaRoot) queue(addr *uint32, s *sudog) {
>    s.g = getg()
>    s.elem = unsafe.Pointer(addr)
>    s.next = nil
>    s.prev = root.tail
>    if root.tail != nil {
>       root.tail.next = s
>    } else {
>       root.head = s
>    }
>    root.tail = s
> }
>
> func (root *semaRoot) dequeue(s *sudog) {
>    if s.next != nil {
>       s.next.prev = s.prev
>    } else {
>       root.tail = s.prev
>    }
>    if s.prev != nil {
>       s.prev.next = s.next
>    } else {
>       root.head = s.next
>    }
>    s.elem = nil
>    s.next = nil
>    s.prev = nil
> }
>
> // Synchronous semaphore for sync.Cond.
> type syncSema struct {
>    lock mutex
>    head *sudog
>    tail *sudog
> }
>
> // syncsemacquire waits for a pairing syncsemrelease on the same semaphore s.
> //go:linkname syncsemacquire sync.runtime_Syncsemacquire
> func syncsemacquire(s *syncSema) {
>    lock(&s.lock)
>    if s.head != nil && s.head.nrelease > 0 {
>       // Have pending release, consume it.
>       var wake *sudog
>       s.head.nrelease--
>       if s.head.nrelease == 0 {
>          wake = s.head
>          s.head = wake.next
>          if s.head == nil {
>             s.tail = nil
>          }
>       }
>       unlock(&s.lock)
>       if wake != nil {
>          wake.next = nil
>          goready(wake.g, 4)
>       }
>    } else {
>       // Enqueue itself.
>       w := acquireSudog()
>       w.g = getg()
>       w.nrelease = -1
>       w.next = nil
>       w.releasetime = 0
>       t0 := int64(0)
>       if blockprofilerate > 0 {
>          t0 = cputicks()
>          w.releasetime = -1
>       }
>       if s.tail == nil {
>          s.head = w
>       } else {
>          s.tail.next = w
>       }
>       s.tail = w
>       goparkunlock(&s.lock, "semacquire", traceEvGoBlockCond, 3)
>       if t0 != 0 {
>          blockevent(int64(w.releasetime)-t0, 2)
>       }
>       releaseSudog(w)
>    }
> }
>
> // syncsemrelease waits for n pairing syncsemacquire on the same semaphore s.
> //go:linkname syncsemrelease sync.runtime_Syncsemrelease
> func syncsemrelease(s *syncSema, n uint32) {
>    lock(&s.lock)
>    for n > 0 && s.head != nil && s.head.nrelease < 0 {
>       // Have pending acquire, satisfy it.
>       wake := s.head
>       s.head = wake.next
>       if s.head == nil {
>          s.tail = nil
>       }
>       if wake.releasetime != 0 {
>          wake.releasetime = cputicks()
>       }
>       wake.next = nil
>       goready(wake.g, 4)
>       n--
>    }
>    if n > 0 {
>       // enqueue itself
>       w := acquireSudog()
>       w.g = getg()
>       w.nrelease = int32(n)
>       w.next = nil
>       w.releasetime = 0
>       if s.tail == nil {
>          s.head = w
>       } else {
>          s.tail.next = w
>       }
>       s.tail = w
>       goparkunlock(&s.lock, "semarelease", traceEvGoBlockCond, 3)
>       releaseSudog(w)
>    } else {
>       unlock(&s.lock)
>    }
> }
>
> //go:linkname syncsemcheck sync.runtime_Syncsemcheck
> func syncsemcheck(sz uintptr) {
>    if sz != unsafe.Sizeof(syncSema{}) {
>       print("runtime: bad syncSema size - sync=", sz, " runtime=", unsafe.Sizeof(syncSema{}), "\n")
>       throw("bad syncSema size")
>    }
> }
>
> ```

