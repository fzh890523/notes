# Golang 1.3 sync.Mutex 源码解析

Posted on [2014年10月11日](http://dev.cmcm.com/archives/22)  by  [毛, 剑](http://dev.cmcm.com/)		 | [6条评论](http://dev.cmcm.com/archives/22#comments)		

[sync](http://godoc.org/sync)包给我们方便的提供了一组同步语意相关的标准库，下面我们重点看下sync.Mutex互斥是如何实现的。 我本机源码安装路径在/usr/local/go，这sync.Mutex（**golang 1.3版本**）涉及到的相关代码主要有：

> /usr/local/go/src/pkg/sync/mutex.go
>
> /usr/local/go/src/pkg/sync/runtime.go
>
> /usr/local/go/src/pkg/runtime/sema.goc

首先是mutex.go：

```Go
// A Mutex is a mutual exclusion lock.                                          // Mutexes can be created as part of other structures;                          // the zero value for a Mutex is an unlocked mutex.                             
type Mutex struct {                                                             
    state int32                                                                 
    sema  uint32                                                                
}
```

首先我们看到Mutex是由state和sema两个整形组成，我们不难推测出，mutex内部实现依赖的是信号量用于goroutine的唤醒操作，state就是对锁抢占者的统计，其实这种方式是采用E.W.Dijkstra在1965年提出的一种方法，用整形变量累计唤醒计数。见论文：[semaphore](http://swtch.com/semaphore.pdf)（之后我单写文章分析）。

```Go
const (                                                                         
    mutexLocked = 1 << iota // mutex is locked                                  
    mutexWoken                                                                  
    mutexWaiterShift = iota                                                     
)  
```

mutexLocked ＝ 1（二进制是1）：表示mutex处于锁状态。

mutexWoken ＝ 2（二进制是10）：表示mutex处于唤醒状态。

mutexWaiterShift = 2（二进制10）：表示等待持有锁需要累计计数的左移位。

接下来是核心的Lock：

```Go
// Lock locks m.                                                                // If the lock is already in use, the calling goroutine                         // blocks until the mutex is available.                                         
func (m *Mutex) Lock() {// Fast path: grab unlocked mutex.                                          if atomic.CompareAndSwapInt32(&m.state, 0, mutexLocked) {if raceenabled {                                                        
            raceAcquire(unsafe.Pointer(m))}return}  
```

raceenabled相关的代码全部忽略，这是golang内部使用[thread-sanitizer](https://code.google.com/p/thread-sanitizer/)用于扫描线程安全问题的诊断代码。

首先代码使用cpu的CAS指令，修改state值，如果值为0的时候，那么置state为mutexLocked状态（即为：1），如果成功，表示锁争用成功，直接return。

```Go
awoke := false  
```

awoke表示由于信号量release导致的goroutine唤醒，在for循环的底部我们看到runtime_Semacquire返回以后会被设置成true。

```Go
for {                                                                       
    old := m.state                                                          
    new := old | mutexLocked                                                
    if old&mutexLocked != 0 {new = old + 1<<mutexWaiterShift                                     
    }         
```

进入for循环开始一直争用，直到成功。对于争用失败的，即state!=0，old存储当前state值，new存储old和mutexLocked或运算，因为state可能被立马释放掉，因此需要先把locked bit位给设置上，也在后续retry lock的时候new会被直接CAS到state中去。

old&mutexLocked!=0表示，如果还存在锁未释放，new值需要新增抢占者计数（1<<2其实就是+4操作）。

```Go
    if awoke {// The goroutine has been woken from sleep,                         // so we need to reset the flag in either case.                     new &^= mutexWoken                                                  
    }  
```

如果是被唤醒的操作，我们需要把mutexWoken的bit位给抹去，这里使用一个异或操作和与操作来完成（先对new和mutexWoken进行异或操作再和new进行与操作）。

```Go
    if atomic.CompareAndSwapInt32(&m.state, old, new) {if old&mutexLocked == 0 {break}                                                                   
        runtime_Semacquire(&m.sema)                                         
        awoke = true}}
```

之后到了retry lock的步骤，前面说了，因为可能锁持有者立马就释放了锁，因此做一步retry操作可以尽可能少的使用信号量来sleep和wakeup的开销；另外唤醒操作也需要重新进行新一轮的CAS判断。

如果当前state和old相等表示没有其他争用者修改state值（有的话重新来过），而old&mutexLocked ＝ 0 意味着锁其实已经被释放，那么上一步的CAS又把锁设置成locked状态，函数break，持锁成功，否则进行信号量的DOWN操作。

semaphore的DOWN操作，检测sema值是否大于0，如果大于0，原子减一，goroutine进入ready状态，继续争用锁；否则goroutine进入sleep等待唤醒状态。

再看看对应的Unlock操作：

```Go
// Unlock unlocks m.                                                            // It is a run-time error if m is not locked on entry to Unlock.                //                                                                              // A locked Mutex is not associated with a particular goroutine.                // It is allowed for one goroutine to lock a Mutex and then                     // arrange for another goroutine to unlock it.                                  
func (m *Mutex) Unlock() {if raceenabled {                                                            
        _ = m.state                                                             
        raceRelease(unsafe.Pointer(m))}// Fast path: drop lock bit.                                                new := atomic.AddInt32(&m.state, -mutexLocked)if (new+mutexLocked)&mutexLocked == 0 {                                     
        panic("sync: unlock of unlocked mutex")}  
```

第一步是原子操作把locked bit位给抹除赋值给new，为了避免多次解锁操作使用new重新置位locked和mutexLocked进行与判断来校验，但是似乎这里有[ABA](http://en.wikipedia.org/wiki/ABA_problem)的BUG，因为有可能连续N次的Unlock，虽然有一部分panic，但是另外一个人会成功。

```Go
old := newfor {// If there are no waiters or a goroutine has already                   // been woken or grabbed the lock, no need to wake anyone.              if old>>mutexWaiterShift == 0 || old&(mutexLocked|mutexWoken) != 0 {return}// Grab the right to wake someone.                                      new = (old - 1<<mutexWaiterShift) | mutexWoken                          
    if atomic.CompareAndSwapInt32(&m.state, old, new) {                     
        runtime_Semrelease(&m.sema)return}                                                                       
    old = m.state                                                           
} 
```

old保存new的值，进入for循环，为了避免不必要的唤醒操作，这样判断了是否有等着唤醒的人以及是否存在争用和已经唤醒的情况。

old>>mutexWaiterShift＝＝0：表示old当前已经没有任何waiters。

old&(mutexLocked|mutexWorken)!=0：表示存在争用和已经触发了唤醒，考虑下执行完new := atomic.AddInt32(&m.state, -mutexLocked)之后，突然有人尝试加锁，那么Lock中的new = old + 4被赋值给state，此时已经争用者立马进入retry lock的逻辑，最终会加锁成功（注意此时只有一个ready状态的goroutine）

```go
new = (old - 1<<mutexWaiterShift) | mutexWoken
```

表示new值先减去一个waiter的计数，然后把woken置位。

而还有一种情况是执行完atomic.CompareAndSwapInt32(&m.state, old, new)操作失败，state处于争用状态，也就无需重复唤醒了，因为争用者可能已经抢到锁了（可见作者这块考虑的相当的完善）。

而如果CAS成功，最终会调用runtime_Semrelease来进行信号量操作唤醒goroutine，而原子操作失败的话只能重头来过了。

整个的mutex加锁和解锁分析完毕，后续我会重点讲一下runtime_Semacquire和runtime_Semrelease在golang runtime中是如何调度goroutine的。

Posted in [Golang](http://dev.cmcm.com/archives/category/golang).| Tagged [Golang](http://dev.cmcm.com/archives/tag/golang), [Mutex](http://dev.cmcm.com/archives/tag/mutex).		| 113 views

来源： <<http://dev.cmcm.com/archives/22>>