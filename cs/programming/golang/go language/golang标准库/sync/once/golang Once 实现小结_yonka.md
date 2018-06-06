**简介**

> 好像只是 只执行一次的task，用状态位来表示执行结果...

**源码简介**

> 没什么特别的
>
> 就是 并发情况下在Lock以外判断变量的值需要用 atomic.LoadUnit32(p) 的方式来 invalidate cache ... （p为指针）
>
> 而且写回的时候也做了... atomic.StoreUint32(p, value) 

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
> )
>
> // Once is an object that will perform exactly one action.
> type Once struct {
>    m    Mutex
>    done uint32
> }
>
> // Do calls the function f if and only if Do is being called for the
> // first time for this instance of Once. In other words, given
> //     var once Once
> // if once.Do(f) is called multiple times, only the first call will invoke f,
> // even if f has a different value in each invocation.  A new instance of
> // Once is required for each function to execute.
> //
> // Do is intended for initialization that must be run exactly once.  Since f
> // is niladic, it may be necessary to use a function literal to capture the
> // arguments to a function to be invoked by Do:
> //     config.once.Do(func() { config.init(filename) })
> //
> // Because no call to Do returns until the one call to f returns, if f causes
> // Do to be called, it will deadlock.
> //
> // If f panics, Do considers it to have returned; future calls of Do return
> // without calling f.
> //
> func (o *Once) Do(f func()) {
>    if atomic.LoadUint32(&o.done) == 1 {
>       return
>    }
>    // Slow-path.
>    o.m.Lock()
>    defer o.m.Unlock()
>    if o.done == 0 {
>       defer atomic.StoreUint32(&o.done, 1)
>       f()
>    }
> }
> ```