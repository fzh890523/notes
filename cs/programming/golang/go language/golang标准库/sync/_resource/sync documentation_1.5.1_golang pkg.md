...

[The Go Programming Language](https://golang.org/)

[Documents](https://golang.org/doc/) [Packages](https://golang.org/pkg/) [The Project](https://golang.org/project/) [Help](https://golang.org/help/) [Blog](https://golang.org/blog/) [Play](http://play.golang.org/) 

# Package sync




## Overview ▾

Package sync provides basic synchronization primitives such as mutual exclusion locks. Other than the Once and WaitGroup types, most are intended for use by low-level library routines. Higher-level synchronization is better done via channels and communication.

Values containing the types defined in this package should not be copied.

## Index ▾


#### Examples


#### Package files

[cond.go](https://golang.org/src/sync/cond.go) [mutex.go](https://golang.org/src/sync/mutex.go) [once.go](https://golang.org/src/sync/once.go) [pool.go](https://golang.org/src/sync/pool.go) [race0.go](https://golang.org/src/sync/race0.go) [runtime.go](https://golang.org/src/sync/runtime.go) [rwmutex.go](https://golang.org/src/sync/rwmutex.go) [waitgroup.go](https://golang.org/src/sync/waitgroup.go)

## type [Cond](https://golang.org/src/sync/cond.go?s=609:772#L12)

```
type Cond struct {
        // L is held while observing or changing the condition
        L Locker// contains filtered or unexported fields
}
```

Cond implements a condition variable, a rendezvous point for goroutines waiting for or announcing the occurrence of an event.

Each Cond has an associated Locker L (often a *Mutex or *RWMutex), which must be held when changing the condition and when calling the Wait method.

A Cond can be created as part of other structures. A Cond must not be copied after first use.

### func [NewCond](https://golang.org/src/sync/cond.go?s=819:847#L22)

```
func NewCond(l Locker) *Cond
```

NewCond returns a new Cond with Locker l.

### func (*Cond) [Broadcast](https://golang.org/src/sync/cond.go?s=1967:1993#L68)

```
func (c *Cond) Broadcast()
```

Broadcast wakes all goroutines waiting on c.

It is allowed but not required for the caller to hold c.L during the call.

### func (*Cond) [Signal](https://golang.org/src/sync/cond.go?s=1785:1808#L60)

```
func (c *Cond) Signal()
```

Signal wakes one goroutine waiting on c, if there is any.

It is allowed but not required for the caller to hold c.L during the call.

### func (*Cond) [Wait](https://golang.org/src/sync/cond.go?s=1429:1450#L42)

```
func (c *Cond) Wait()
```

Wait atomically unlocks c.L and suspends execution of the calling goroutine. After later resuming execution, Wait locks c.L before returning. Unlike in other systems, Wait cannot return unless awoken by Broadcast or Signal.

Because c.L is not locked when Wait first resumes, the caller typically cannot assume that the condition is true when Wait returns. Instead, the caller should Wait in a loop:

```
c.L.Lock()
for !condition() {
    c.Wait()
}
... make use of condition ...
c.L.Unlock()

```

## type [Locker](https://golang.org/src/sync/mutex.go?s=828:871#L17)

```
type Locker interface {
        Lock()
        Unlock()
}
```

A Locker represents an object that can be locked and unlocked.

## type [Mutex](https://golang.org/src/sync/mutex.go?s=712:760#L11)

```
type Mutex struct {
        // contains filtered or unexported fields
}
```

A Mutex is a mutual exclusion lock. Mutexes can be created as part of other structures; the zero value for a Mutex is an unlocked mutex.

### func (*Mutex) [Lock](https://golang.org/src/sync/mutex.go?s=1078:1100#L31)

```
func (m *Mutex) Lock()
```

Lock locks m. If the lock is already in use, the calling goroutine blocks until the mutex is available.

### func (*Mutex) [Unlock](https://golang.org/src/sync/mutex.go?s=2481:2505#L89)

```
func (m *Mutex) Unlock()
```

Unlock unlocks m. It is a run-time error if m is not locked on entry to Unlock.

A locked Mutex is not associated with a particular goroutine. It is allowed for one goroutine to lock a Mutex and then arrange for another goroutine to unlock it.

## type [Once](https://golang.org/src/sync/once.go?s=260:305#L2)

```
type Once struct {
        // contains filtered or unexported fields
}
```

Once is an object that will perform exactly one action.

▹ Example

### func (*Once) [Do](https://golang.org/src/sync/once.go?s=1139:1166#L25)

```
func (o *Once) Do(f func())
```

Do calls the function f if and only if Do is being called for the first time for this instance of Once. In other words, given

```
var once Once

```

if once.Do(f) is called multiple times, only the first call will invoke f, even if f has a different value in each invocation. A new instance of Once is required for each function to execute.

Do is intended for initialization that must be run exactly once. Since f is niladic, it may be necessary to use a function literal to capture the arguments to a function to be invoked by Do:

```
config.once.Do(func() { config.init(filename) })

```

Because no call to Do returns until the one call to f returns, if f causes Do to be called, it will deadlock.

If f panics, Do considers it to have returned; future calls of Do return without calling f.

## type [Pool](https://golang.org/src/sync/pool.go?s=1570:1913#L32)

```
type Pool struct {

        // New optionally specifies a function to generate// a value when Get would otherwise return nil.// It may not be changed concurrently with calls to Get.
        New func() interface{}
        // contains filtered or unexported fields
}
```

A Pool is a set of temporary objects that may be individually saved and retrieved.

Any item stored in the Pool may be removed automatically at any time without notification. If the Pool holds the only reference when this happens, the item might be deallocated.

A Pool is safe for use by multiple goroutines simultaneously.

Pool's purpose is to cache allocated but unused items for later reuse, relieving pressure on the garbage collector. That is, it makes it easy to build efficient, thread-safe free lists. However, it is not suitable for all free lists.

An appropriate use of a Pool is to manage a group of temporary items silently shared among and potentially reused by concurrent independent clients of a package. Pool provides a way to amortize allocation overhead across many clients.

An example of good use of a Pool is in the fmt package, which maintains a dynamically-sized store of temporary output buffers. The store scales under load (when many goroutines are actively printing) and shrinks when quiescent.

On the other hand, a free list maintained as part of a short-lived object is not a suitable use for a Pool, since the overhead does not amortize well in that scenario. It is more efficient to have such objects implement their own free list.

### func (*Pool) [Get](https://golang.org/src/sync/pool.go?s=3023:3055#L83)

```
func (p *Pool) Get() interface{}
```

Get selects an arbitrary item from the Pool, removes it from the Pool, and returns it to the caller. Get may choose to ignore the pool and treat it as empty. Callers should not assume any relation between values passed to Put and the values returned by Get.

If Get would otherwise return nil and p.New is non-nil, Get returns the result of calling p.New.

### func (*Pool) [Put](https://golang.org/src/sync/pool.go?s=2204:2237#L51)

```
func (p *Pool) Put(x interface{})
```

Put adds x to the pool.

## type [RWMutex](https://golang.org/src/sync/rwmutex.go?s=462:794#L8)

```
type RWMutex struct {
        // contains filtered or unexported fields
}
```

An RWMutex is a reader/writer mutual exclusion lock. The lock can be held by an arbitrary number of readers or a single writer. RWMutexes can be created as part of other structures; the zero value for a RWMutex is an unlocked mutex.

### func (*RWMutex) [Lock](https://golang.org/src/sync/rwmutex.go?s=2085:2110#L66)

```
func (rw *RWMutex) Lock()
```

Lock locks rw for writing. If the lock is already locked for reading or writing, Lock blocks until the lock is available. To ensure that the lock eventually becomes available, a blocked Lock call excludes new readers from acquiring the lock.

### func (*RWMutex) [RLock](https://golang.org/src/sync/rwmutex.go?s=862:888#L19)

```
func (rw *RWMutex) RLock()
```

RLock locks rw for reading.

### func (*RWMutex) [RLocker](https://golang.org/src/sync/rwmutex.go?s=3619:3654#L119)

```
func (rw *RWMutex) RLocker() Locker
```

RLocker returns a Locker interface that implements the Lock and Unlock methods by calling rw.RLock and rw.RUnlock.

### func (*RWMutex) [RUnlock](https://golang.org/src/sync/rwmutex.go?s=1324:1352#L38)

```
func (rw *RWMutex) RUnlock()
```

RUnlock undoes a single RLock call; it does not affect other simultaneous readers. It is a run-time error if rw is not locked for reading on entry to RUnlock.

### func (*RWMutex) [Unlock](https://golang.org/src/sync/rwmutex.go?s=2926:2953#L92)

```
func (rw *RWMutex) Unlock()
```

Unlock unlocks rw for writing. It is a run-time error if rw is not locked for writing on entry to Unlock.

As with Mutexes, a locked RWMutex is not associated with a particular goroutine. One goroutine may RLock (Lock) an RWMutex and then arrange for another goroutine to RUnlock (Unlock) it.

## type [WaitGroup](https://golang.org/src/sync/waitgroup.go?s=505:813#L7)

```
type WaitGroup struct {
        // contains filtered or unexported fields
}
```

A WaitGroup waits for a collection of goroutines to finish. The main goroutine calls Add to set the number of goroutines to wait for. Then each of the goroutines runs and calls Done when finished. At the same time, Wait can be used to block until all goroutines have finished.

▹ Example

### func (*WaitGroup) [Add](https://golang.org/src/sync/waitgroup.go?s=1772:1807#L37)

```
func (wg *WaitGroup) Add(delta int)
```

Add adds delta, which may be negative, to the WaitGroup counter. If the counter becomes zero, all goroutines blocked on Wait are released. If the counter goes negative, Add panics.

Note that calls with a positive delta that occur when the counter is zero must happen before a Wait. Calls with a negative delta, or calls with a positive delta that start when the counter is greater than zero, may happen at any time. Typically this means the calls to Add should execute before the statement creating the goroutine or other event to be waited for. If a WaitGroup is reused to wait for several independent sets of events, new Add calls must happen after all previous Wait calls have returned. See the WaitGroup example.

### func (*WaitGroup) [Done](https://golang.org/src/sync/waitgroup.go?s=3141:3168#L84)

```
func (wg *WaitGroup) Done()
```

Done decrements the WaitGroup counter.

### func (*WaitGroup) [Wait](https://golang.org/src/sync/waitgroup.go?s=3238:3265#L89)

```
func (wg *WaitGroup) Wait()
```

Wait blocks until the WaitGroup counter is zero.

## Subdirectories

| Name                                     | Synopsis                                 |
| ---------------------------------------- | ---------------------------------------- |
| [..](https://golang.org/pkg/)            |                                          |
| [atomic](https://golang.org/pkg/sync/atomic/) | Package atomic provides low-level atomic memory primitives useful for implementing synchronization algorithms. |

Build version go1.5.1.
Except as [noted](https://developers.google.com/site-policies#restrictions), the content of this page is licensed under the Creative Commons Attribution 3.0 License, and code is licensed under a [BSD license](https://golang.org/LICENSE).
[Terms of Service](https://golang.org/doc/tos.html) | [Privacy Policy](http://www.google.com/intl/en/policies/privacy/)

来源： <<https://golang.org/pkg/sync/#Cond>>