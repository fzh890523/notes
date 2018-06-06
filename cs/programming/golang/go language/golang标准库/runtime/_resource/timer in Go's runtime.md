

### timer in Go's runtime

12 September 2013

by [skoo](http://skoo.me/skoo.html)

我们总是使用sleep()类函数来让线程暂停一段时间，在Go语言里，也是使用Sleep()来暂停goroutine。 那么Go语言的sleep究竟是如何现实的呢？当然你翻看标准库中的time包里面的sleep.go源码时， 你可能会觉得看不明白，因为支持sleep功能的真正实现是在runtime里面。不难想到sleep功能是根据定时器来实现的， 因此接下来看看runtime中的timer究竟长什么样子。

timer的实现主要位于[runtime/time.goc](http://golang.org/src/pkg/runtime/time.goc)文件中。

#### 主要数据结构

```
struct  Timers
{
    Lock;
    G       *timerproc;
    bool    sleeping;
    bool    rescheduling;
    Note    waitnote;
    Timer   **t;
    int32   len;
    int32   cap;
};

struct  Timer
{
    int32   i;      // heap index

    // Timer wakes up at when, and then at when+period, ... (period > 0 only)
    // each time calling f(now, arg) in the timer goroutine, so f must be
    // a well-behaved function and not block.
    int64   when;
    int64   period;
    FuncVal *fv;
    Eface   arg;
};

```

*这两个结构是定义在runtime.h文件中。*

调用一次sleep其实就是生成一个`Timer`，然后添加到`Timers`中。可以看出来Timers就是维护所有Timer的一个集合。除了可以向Timers中添加Timer外，还要从Timers中删除超时的Timer。所以，Timers采用小顶堆来维护，小顶堆是常用来管理定时器的结构，有的地方也使用红黑树。

**Timers**

- Timers结构中有一个`Lock`, 大概猜测一下就知道是用来保护`添加/删除`Timer的，实际上也是干这件事的。
- `timerproc`指针维护的是一个goroutine，这个goroutine的主要功能就是检查小顶堆中的Timer是否超时。当然，超时就是删除Timer，并且执行Timer对应的动作。
- `t`显然就是存储所有Timer的堆了。

省略几个字段放到下文再介绍。

**Timer**

- `when`就是定时器超时的时间
- `fv`和`arg`挂载的是Timer超时后需要执行的方法。

到此，Go语言的定时器大概模型就能想象出来了。其实，所有定时器的实现都大同小异，长得都差不多。

#### timerproc goroutine

上文提到timerproc维护的是一个goroutine，这个goroutine就做一件事情——不断的循环检查堆，删除掉那些超时的Timer，并执行Timer。下面精简一下代码，看个大概主干就足够明白了。

```
static void
timerproc(void)
{
    for(;;) {
        for(;;) {
            // 判断Timer是否超时
            t = timers.t[0];
            delta = t->when - now;
            if(delta > 0)
                break;

            // TODO: 删除Timer, 代码被删除

            // 这里的f调用就是执行Timer了
            f(now, arg);
        }

        // 这个过程是，堆中没有任何Timer的时候，就把这个goroutine给挂起，不运行。
        // 添加Timer的时候才会让它ready。
        if(delta < 0) {
            // No timers left - put goroutine to sleep.
            timers.rescheduling = true;
            runtime·park(runtime·unlock, &timers, "timer goroutine (idle)");
            continue;
        }

        // 这里干的时候就让这个goroutine也sleep, 等待最近的Timer超时，再开始执行上面的循环检查。当然，这里的sleep不是用本文的定时器来实现的，而是futex锁实现。
        // At least one timer pending.  Sleep until then.
        timers.sleeping = true;
        runtime·notetsleep(&timers.waitnote, delta);
        }
    }
}

```

这里一定要记住，`timerproc`是在一个独立的goroutine中执行的。梳理一下上面代码的过程：

1. 判断堆中是否有Timer? 如果没有就将`Timers`的`rescheduling`设置为true的状态，true就代表timerproc goroutine被挂起，需要重新调度。这个重新调度的时刻就是在添加一个Timer进来的时候，会ready这个goroutine。这里挂起goroutine使用的是runtime·park()函数。
2. 如果堆中有Timer存在，就取出堆顶的一个Timer，判断是否超时。超时后，就删除Timer，执行Timer中挂载的方法。这一步是循环检查堆，直到堆中没有Timer或者没有超时的Timer为止。
3. 在堆中的Timer还没超时之前，这个goroutine将处于sleep状态，也就是设置`Timers`的`sleeping`为true状态。这个地方是通过runtime·notesleep()函数来完成的，其实现是依赖futex锁。这里，goroutine将sleep多久呢？它将sleep到最近一个Timer超时的时候，就开始执行。

维护Timers超时的goroutine干的所有事情也就这么一点，这里除了堆的维护外，就是goroutine的调度了。

#### 添加一个定时器

另外一个重要的过程就是如何完成一个Timer的添加? 同样精简掉代码，最好是对照完整的源码看。

```
static void
addtimer(Timer *t)
{
    if(timers.len >= timers.cap) {
        // TODO 这里是堆没有剩余的空间了，需要分配一个更大的堆来完成添加Timer。
    }

    // 这里添加Timer到堆中.
    t->i = timers.len++;
    timers.t[t->i] = t;
    siftup(t->i);

    // 这个地方比较重要，这是发生在添加的Timer直接位于堆顶的时候，堆顶位置就代表最近的一个超时Timer.
    if(t->i == 0) {
        // siftup moved to top: new earliest deadline.
        if(timers.sleeping) {
            timers.sleeping = false;
            runtime·notewakeup(&timers.waitnote);
        }
        if(timers.rescheduling) {
            timers.rescheduling = false;
            runtime·ready(timers.timerproc);
        }
    }
}

```

从代码可以看到新添加的Timer如果是堆顶的话，会检查`Timers`的sleeping和rescheduling两个状态。上文已经提过了，这两个状态代表timeproc goroutine的状态，如果处于sleeping，那就wakeup它; 如果是rescheduling就ready它。这么做的原因就是通知那个wait的goroutine——”堆中有一个Timer了”或者”堆顶的Timer易主了”，你赶紧来检查一下它是否超时。

添加一个Timer的过程实在太简单了，关键之处就是最后的Timers状态检查逻辑。

#### Sleep()的实现

上面的内容阐述了runtime的定时器是如何运行的，那么Go语言又是如何在定时器的基础上实现Sleep()呢？

Go程序中调用time.Sleep()后将进入runtime，执行下面的代码：

```
void
runtime·tsleep(int64 ns, int8 *reason)
{
    Timer t;

    if(ns <= 0)
        return;

    t.when = runtime·nanotime() + ns;
    t.period = 0;
    t.fv = &readyv;
    t.arg.data = g;
    runtime·lock(&timers);
    addtimer(&t);
    runtime·park(runtime·unlock, &timers, reason);
}

```

sleep原来就是创建一个Timer，添加到Timers中去，最后调用runtime·park()将当前调用Sleep()的goroutine给挂起就完事了。

关键是，goroutine被挂起后，如何在超时后被唤醒继续运行呢？这里就是Timer中`fv`和`arg`两个字段挂载的东西来完成的了。此处，fv挂载了`&readyv`，看一下readyv的定义:

```
static void
ready(int64 now, Eface e)
{
    USED(now);

    runtime·ready(e.data);
}

static FuncVal readyv = {(void(*)(void))ready};

```

readyv其实就是指向了ready函数，这个ready函数就是在Timer超时的时候将会被执行，它将ready被挂起的goroutine。`t.arg.data = g;` 这行代码就是在保存当前goroutine了。

Sleep()实现总结起来就三大步:

1. 创建一个Timer添加到Timers中
2. 挂起当前goroutine
3. Timer超时ready当前goroutine

Go语言的定时器实现还是比较清晰的，没有什么繁琐的逻辑。相比，其他地方(如:Nginx)的实现来说，这里可能就是多了goroutine的调度逻辑。

看一个东西的实现，重要的是知道作者为何要这样做。只有弄明白了why, how才有价值。

- **
- [go 17](http://skoo.me/categories.html#go-ref)


- **

- [Go 17](http://skoo.me/tags.html#Go-ref)

- [runtime 11](http://skoo.me/tags.html#runtime-ref)

- ​

- 来源： <<http://skoo.me/go/2013/09/12/go-runtime-timer/>>

