## golang sync.Mutex 设计思想与演化过程 （一）

时间:2013-06-25 00:23  浏览:791人

今天一位golang的大牛 和 我给我娓娓道来 sync.Mutex的实现。听了大牛的讲述之后，我随口问了两个问题：

1、 有没有更加简洁的实现方法？

2.   作者实现的思路是什么？

他觉得我的问题好奇怪，这是他的原话“看类库就是为了以后写程序的时候有底，知道怎么实现的，不会用错。”所以，我提的问题，他认为不重要。当然，我不想争辩什么，最好的办法就是探索这两个问题， 看看对自己有没有帮助。为了写这篇文章，我还是花了挺多的心思收集历史资料， 论坛讨论，并去golang-nuts  上咨询了一些问题。希望对大家有所帮助。



## [go sync.Mutex 设计思想与演化过程 （一）](https://www.cnblogs.com/niniwzw/p/3153955.html)		

​     go语言在云计算时代将会如日中天，还抱着.NET不放的人将会被淘汰。学习go语言和.NET完全不一样，它有非常简单的runtime 和 类库。最好的办法就是将整个源代码读一遍，这是我见过最简洁的系统类库。读了之后，你会真正体会到C#的面向对象的表达方式是有问题的，继承并不是必要的东西。相同的问题，在go中有更加简单的表达。

　　go runtime 没有提供任何的锁，只是提供了一个PV操作原语。独占锁，条件锁 都是基于这个原语实现的。如果你学习了go，那就就知道如何在windows下高效的方式实现条件锁定（windows没有自带的条件锁）。

​     我想阅读源代码，不能仅仅只看到实现了什么，还要看到作者的设计思路，还有如果你作为作者，如何实现。这些才是真正有用的东西，知识永远学不完，我们要锻炼我们的思维。

​    要写这篇文章的背景就忽略吧，我已经很久没有写博客了，主要原因是我基本上看不到能让我有所帮助的博客，更多的是我认为我也写不出能对别人有所帮助的文章。为了写这篇文章，我还是花了挺多的心思收集历史资料， 论坛讨论，并去golang-nuts  上咨询了一些问题。希望对大家有所帮助。

一. sync.Mutex 是什么？

Mutex是一种独占锁，一般操作系统都会提供这种锁。但是，操作系统的锁是针对线程的，golang里面没有线程的概念，这样操作系统的锁就用不上了。所以，你看go语言的runtime，就会发现，实际上这是一个“操作系统”。如果Mutex还不知道的话，我建议看下面的文章，其中第一篇必看。

百度百科 mutex <http://baike.baidu.com/view/1461738.htm?fromId=1889552&redirected=seachword>

信号量：<http://swtch.com/semaphore.pdf>

还可以读一下百度百科 pv 操作：<http://baike.baidu.com/view/703687.htm>

 

二. golang 最新版本的 sync.Mutex

你可以大致扫描一下最新版本的实现，如果你第一眼就看的很懂了，每步的操作？为什么这样操作？有没有更加合理的操作？那恭喜你，你的水平已经超过google实现 sync.Mutex 的程序员了，甚至是大部分的程序员，因为这个程序历经几年的演化，才到了今天的样子，你第一眼就能看的如此透彻，那真的是很了不起。下面的章节是为没有看懂的人准备的。

```
// Copyright 2009 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

// Package sync provides basic synchronization primitives such as mutual
// exclusion locks.  Other than the Once and WaitGroup types, most are intended
// for use by low-level library routines.  Higher-level synchronization is
// better done via channels and communication.
//
// Values containing the types defined in this package should not be copied.
package sync

import (
    "sync/atomic"
    "unsafe"
)

// A Mutex is a mutual exclusion lock.
// Mutexes can be created as part of other structures;
// the zero value for a Mutex is an unlocked mutex.
type Mutex struct {
    state int32
    sema  uint32
}

// A Locker represents an object that can be locked and unlocked.
type Locker interface {
    Lock()
    Unlock()
}

const (
    mutexLocked = 1 << iota // mutex is locked
    mutexWoken
    mutexWaiterShift = iota
)

// Lock locks m.
// If the lock is already in use, the calling goroutine
// blocks until the mutex is available.
func (m *Mutex) Lock() {
    // Fast path: grab unlocked mutex.
    if atomic.CompareAndSwapInt32(&m.state, 0, mutexLocked) {
        if raceenabled {
            raceAcquire(unsafe.Pointer(m))
        }
        return
    }

    awoke := false
    for {
        old := m.state
        new := old | mutexLocked
        if old&mutexLocked != 0 {
            new = old + 1<<mutexWaiterShift
        }
        if awoke {
            // The goroutine has been woken from sleep,
            // so we need to reset the flag in either case.
            new &^= mutexWoken
        }
        if atomic.CompareAndSwapInt32(&m.state, old, new) {
            if old&mutexLocked == 0 {
                break
            }
            runtime_Semacquire(&m.sema)
            awoke = true
        }
    }

    if raceenabled {
        raceAcquire(unsafe.Pointer(m))
    }
}

// Unlock unlocks m.
// It is a run-time error if m is not locked on entry to Unlock.
//
// A locked Mutex is not associated with a particular goroutine.
// It is allowed for one goroutine to lock a Mutex and then
// arrange for another goroutine to unlock it.
func (m *Mutex) Unlock() {
    if raceenabled {
        _ = m.state
        raceRelease(unsafe.Pointer(m))
    }

    // Fast path: drop lock bit.
    new := atomic.AddInt32(&m.state, -mutexLocked)
    if (new+mutexLocked)&mutexLocked == 0 {
        panic("sync: unlock of unlocked mutex")
    }

    old := new
    for {
        // If there are no waiters or a goroutine has already
        // been woken or grabbed the lock, no need to wake anyone.
        if old>>mutexWaiterShift == 0 || old&(mutexLocked|mutexWoken) != 0 {
            return
        }
        // Grab the right to wake someone.
        new = (old - 1<<mutexWaiterShift) | mutexWoken
        if atomic.CompareAndSwapInt32(&m.state, old, new) {
            runtime_Semrelease(&m.sema)
            return
        }
        old = m.state
    }
}
```

三. 有没有更加简洁的实现方法？

有点操作系统知识的都知道，独占锁是一种特殊的PV 操作，就 0 – 1 PV操作。那我想，如果不考虑任何性能问题的话，用信号量应该就可以这样实现Mutex：

```
type Mutex struct {
    sema uint32
}

func NewMutex() *Mutex {
    var mu Mutex
    mu.sema = 1
    return &mu
}

func (m *Mutex) Lock() {
        runtime_Semacquire(&m.sema)
}

func (m *Mutex2) Unlock() {
    runtime_Semrelease(&m.sema)
}
```

当然，这个实现有点不符合要求。如果有个家伙不那么靠谱，加锁了一次，但是解锁了两次。第二次解锁的时候，应该报出一个错误，而不是让错误隐藏。于是乎，我们想到用一个变量表示加锁的次数。这样就可以判断有没有多次解锁。于是乎，我就想到了下面的解决方案：

```
type Mutex struct {
        key  int32
        sema uint32
}

func (m *Mutex) Lock() {
        if atomic.AddInt32(&m.key, 1) == 1 {
                // changed from 0 to 1; we hold lock
                return
        }
        runtime_Semacquire(&m.sema)
}

func (m *Mutex) Unlock() {
        switch v := atomic.AddInt32(&m.key, -1); {
        case v == 0:
                // changed from 1 to 0; no contention
                return
        case v == -1:
                // changed from 0 to -1: wasn't locked
                // (or there are 4 billion goroutines waiting)
                panic("sync: unlock of unlocked mutex")
        }
        runtime_Semrelease(&m.sema)
}
```

 

表示多线程复杂状态，最好的办法就是抽象出 状态 和 操作，忽略掉线程，让问题变成一个状态机问题。这样的图不仅仅用于分析Mutex。我还经常用来分析复杂的多线程锁定问题，独家秘诀，今天在这里泄露了。

 

第一个程序可以抽象出这样一个图：

 

[![image](https://images0.cnblogs.com/blog/34637/201306/24235651-d5741460384f49199c12e680023fef40.png)](https://images0.cnblogs.com/blog/34637/201306/24235651-8ed9e37d86cf46f8a770fc4ce1322408.png)

这个状态机非常简单，有两种状态(1, 0)，两个操作(Lock, Unlock)。A线程 Lock操作后，只要它不进行UnLock操作，就不可能有其他的线程能获取到锁。因为，这个状态机唯一的轨迹是：Lock –-unlock --lock --unlock。

 

第二个程序可能的状态会非常的多，不过要注意的是 程序 2 的 Lock 和 Unlock都不是原子操作，都会分成两个部分。

Lock操作分成两个部分，一个是更改锁的状态， 我们用LSt(Lock state change) 表示，一个是更改sema, LSe (Lock sema acquire)

unlock也是一样，分别用USt (unlock state change), USe (unlock sema release) 表示。

 

那就是有4个操作，n种状态在4种操作下不断的切换， 如果  线程A 加锁 -- 解锁  中，其他线程不能进行 加锁的完整操作（LSt + LSe）（可以进行部分的加锁操作，比如LSt 操作）， 那么程序就是正确的。

像这类最基础的类库，代码量也不是很多的情况下，证明正确性是非常重要的。在我开发金融交易服务器的过程中，对很多关键的代码我都进行了证明，我发现这是理解问题和发现bug的好方法。 这也是独家的秘诀，在这里就泄露了。

说句题外话，有时间的话，一定要把 《算法导论》 里面的每一个证明都看的很通透，那你的水平就可以提升一大截了。上面对代码的抽象是十分关键的技巧，这样，就可以对这个代码进行分析了。

 

程序2 图表 : 注， 0,0 表示的是 key = 0， sema = 0,

[![image](https://images0.cnblogs.com/blog/34637/201306/24235655-4dbb7431a06447798f4d0dda9ed7f5a2.png)](https://images0.cnblogs.com/blog/34637/201306/24235652-44c113a9af1047c6bc55afc9ff5153dd.png)

 

不过，我靠，貌似只是加了一个状态，图复杂了这样多，理论上，这是一个无限状态自动机了，但是实际上，同时等待的数目一般不会是无限的。其实要证明为什么这个程序是正确的，从图上应该可以看出思路了。LSE都是 向上的，USE都是向下的。所以，Lse操作后，要想再有个Lse，必须先操作一个Use。所以，证明的关键还在于sema的特性，基本上可以把状态忽略，当然， 从0，0 到 1,0 这是一个非常特殊的状态，他们和信号量无关。

如果你是golang的忠实粉丝，而且从09年就开始知道golang的话，那么你一定知道 第二个程序就是 golang类库中最初始的 Mutex版本。比现在的版本要简单很多，但是性能上要慢一点点。看类库的演化其实是一件非常有趣的事情，我比较喜欢看非常原始的版本， 而不喜欢看最新版本的源代码，因为最新版本，成熟的版本，往往包括了太多的性能优化的细节，而损失了可读性， 也难以从中得到有用的思想。

​    理解一个程序如何工作很简单，但是，作者的设计思路才是关键，我们可以不断的看源代码，看别人的实现，我们能从中学到很多知识与技巧，当遇到相同的问题的时候，我们也能解决类似的问题。

我个人觉得，作为一个天朝的程序员，不能仅仅是山寨别人的软件，学习别人的东西。还是要能进入一个新的领域，一个未知的领域，还能有所创新。

当然，作者的设计思路我们很难得知，我们看到的只是劳动的结果，但是，我们可以这样问自己，如果我是作者，我怎么思考这个问题，然后解决这个问题。我发现，用这样的思维去考虑问题，有时候能给我很多的启示。

​    还有五分钟就12点了，我必须睡觉了，今天也只能先回答半个问题了。至于为什么不是一个问题，而是半个问题，请听下回分解。

标签: [golang](http://www.cnblogs.com/niniwzw/tag/golang/), [go语言](http://www.cnblogs.com/niniwzw/tag/go%E8%AF%AD%E8%A8%80/), [Mutex](http://www.cnblogs.com/niniwzw/tag/Mutex/)