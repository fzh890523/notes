**简介**

> ...
>
> 
>
> 作为 runtime package的alias

**源码简介**

> ****
>
> ![img](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/4c5860c6-05c3-4a59-9611-6c39d90ac17d/index_files/4fa8d3a3-1f99-48ac-9cbf-24d839b0c0a6.png)****
>
> 可以看到，这里主要是一些签名，需要链接过来。
>
> 实现在 runtime 包里。
>
> 主要是：

> > syncSema和对应的acquire、release；
> >
> > sema的acquire和release
> >
> > canSpin和doSpin（自旋）

**源码**

> ```Go
> // Copyright 2012 The Go Authors.  All rights reserved.
> // Use of this source code is governed by a BSD-style
> // license that can be found in the LICENSE file.
>
> package sync
>
> import "unsafe"
>
> // defined in package runtime
>
> // Semacquire waits until *s > 0 and then atomically decrements it.
> // It is intended as a simple sleep primitive for use by the synchronization
> // library and should not be used directly.
> func runtime_Semacquire(s *uint32)
>
> // Semrelease atomically increments *s and notifies a waiting goroutine
> // if one is blocked in Semacquire.
> // It is intended as a simple wakeup primitive for use by the synchronization
> // library and should not be used directly.
> func runtime_Semrelease(s *uint32)
>
> // Approximation of syncSema in runtime/sema.go.
> type syncSema struct {
>    lock uintptr
>    head unsafe.Pointer
>    tail unsafe.Pointer
> }
>
> // Syncsemacquire waits for a pairing Syncsemrelease on the same semaphore s.
> func runtime_Syncsemacquire(s *syncSema)
>
> // Syncsemrelease waits for n pairing Syncsemacquire on the same semaphore s.
> func runtime_Syncsemrelease(s *syncSema, n uint32)
>
> // Ensure that sync and runtime agree on size of syncSema.
> func runtime_Syncsemcheck(size uintptr)
> func init() {
>    var s syncSema
>    runtime_Syncsemcheck(unsafe.Sizeof(s))
> }
>
> // Active spinning runtime support.
> // runtime_canSpin returns true is spinning makes sense at the moment.
> func runtime_canSpin(i int) bool
>
> // runtime_doSpin does active spinning.
> func runtime_doSpin()
> ```