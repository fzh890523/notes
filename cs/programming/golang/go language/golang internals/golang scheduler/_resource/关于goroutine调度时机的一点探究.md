# 关于goroutine调度时机的一点探究

最近遇到了一点goroutine调度的问题，就去搜了一下在什么情况下会触发goroutine的调度，可惜搜到的资料不是语焉不详就是Go版本太老。于是我参考这些资料自己写了几段代码试了一下。

代码测试环境：

- Go 1.4.2
- Ubuntu 15.04

### 什么都不做会不会自动触发调度？

在SegmentFault上有个[问题](http://segmentfault.com/q/1010000000207474),是关于死循环情况没法调度的，但是这个问题也比较老了。其他有些资料里提到比较新版本的Go中加入了抢占式调度，那么前述问题被解决了吗？我写了如下代码：

```
package main
import "fmt"

func test() {
    fmt.Println("Hello")
}

func main() {
    go test()
    for {
    }
}

```

然后运行，没有任何输出，事实证明在最新版本的Go中这个问题仍没有得到解决。
那么Go中所谓的抢占式调度是个什么意思呢？让我们去Go的源码中一探究竟。
首先，在初始化的时候runtime会用`onM(newsysmon)`生成一个后台线程sysmon，通过查看源码(proc.c中的sysmon函数)可以发现，sysmon的任务主要有三个，定时启动垃圾回收、定时进行IO poll、“抢断”运行时间过长的G。而其中完成抢断的代码很短，如下：

```
// retake P's blocked in syscalls
// and preempt long running G's
if(retake(now))
    idle = 0;
else
    idle++;

```

这个retake函数的代码中与抢断相关的代码如下：

```
} else if(s == Prunning) {
    // Preempt G if it's running for more than 10ms.
    t = p->schedtick;
    if(pd->schedtick != t) {
        pd->schedtick = t;
        pd->schedwhen = now;
        continue;
    }
    if(pd->schedwhen + 10*1000*1000 > now)
        continue;
    preemptone(p);
}

```

可以看到最终一步抢断的任务落到了preemptone上。这个函数很短，全文如下：

```
static bool
preemptone(P *p)
{
    M *mp;
    G *gp;

    mp = p->m;
    if(mp == nil || mp == g->m)
        return false;
    gp = mp->curg;
    if(gp == nil || gp == mp->g0)
        return false;
    gp->preempt = true;
    // Every call in a go routine checks for stack overflow by
    // comparing the current stack pointer to gp->stackguard0.
    // Setting gp->stackguard0 to StackPreempt folds
    // preemption into the normal stack overflow check.
    gp->stackguard0 = StackPreempt;
    return true;
}

```

可以看到，完成所谓抢断的所有步骤仅仅是将运行时间过长的G中的preempt域设为true、stackguard0设为StackPreempt，并没有即时完成抢断。注释中也说的很清楚，在函数调用前会查看是否栈溢出，此时如果发现该G被标记，才会进行真正的抢断。

初步结论：Go目前版本的抢占式调度非常原始并且不完善，所谓的抢断只是对G进行了标记，如果G不进行任何函数调用那么还是不会发生调度。所以在代码编写的过程中要避免因死循环导致的全局锁死。

### 任何函数调用时都会发生调度吗？

从上文的情况可知在函数调用时应当会发生调度。那么是不是任何函数调用都会触发调度呢？我又写了一段代码进行测试：

```
package main
import "fmt"

func test() {
    fmt.Println("Hello")
} 

func preempt() {
}

func main() {
    go test()
    for {
        preempt()
    }
}

```

令我感到困惑的是，这段代码运行之后仍然没有输出，就是说抢断仍然没有发生，我感到很困惑。翻看Go的文档，发现了这么一段关于抢占的[介绍](http://golang.org/doc/go1.2#preemption):
 “The scheduler is invoked occasionally upon entry to a function. This 
means that any loop that includes a (non-inlined) function call can be 
pre-empted, allowing other goroutines to run on the same 
thread.”也就是说任何非内联函数应当会触发调度。那么哪些函数会被Go编译器进行内联优化呢？我翻看了源码，[这个文件](https://github.com/golang/go/blob/release-branch.go1.4/src/cmd/gc/inl.c)负责内联机制的实现，其中由[这个函数](https://github.com/golang/go/blob/release-branch.go1.4/src/cmd/gc/inl.c#L116)判断是否内联。关键部分如下：

```
// If fn has no body (is defined outside of Go), cannot inline it.
if(fn->nbody == nil)
    return;

if(fn->typecheck == 0)
    fatal("caninl on non-typechecked function %N", fn);

// can't handle ... args yet
if(debug['l'] < 3)
    for(t=fn->type->type->down->down->type; t; t=t->down)
        if(t->isddd)
            return;

budget = 40;  // allowed hairyness
if(ishairylist(fn->nbody, &budget))
    return;

```

第一种情况是定义在Go程序外的函数不会被内联，对于普通Go函数这个函数定义了一个budget，使用一个`ishairylist`函数来判断这个函数是否超过了budget。`ishairylist`函数内部调用了`ishairy`函数，通过查看[这个函数的源码](https://github.com/golang/go/blob/release-branch.go1.4/src/cmd/gc/inl.c#L174)可以知道具体的机制。大致就是一个比较短的，语法结构极为简单（for、select、函数调用等等都不能有，基本只能有if）的函数才会被内联。

然后是测试系统调用会不会触发调度。测试代码如下：

```
package main
import "fmt"
import "syscall"

func test() {
    fmt.Println("Hello")
} 

func preempt() {
    syscall.Getgid()
}

func main() {
    go test()
    for {
        preempt()
    }
}

```

抢占终于发生了。直接调用系统调用会触发调度。

初步结论：内联函数无法触发调度，系统调用和函数调用能触发调度。

### 对channel的操作会不会触发调度？

channel是Go语言的重要特性，也是goroutine之间通信的重要手段。我首先对无缓冲channel的操作进行了测试：

```
package main
import "fmt"

var quit chan int

func test() {
    fmt.Println("Hello")
}

func main() {
    quit = make(chan int)
    go test()
    for {
        quit <- 1
    }
}

```

运行后输出了Hello，然后提示死锁。也就是说无缓冲的channel是会触发调度的。
接下来是有缓冲的channel测试：

```
package main
import "fmt"

var quit chan int
var glo int

func test() {
    fmt.Println(glo)
}

func main() {
    glo = 0
    n := 10000
    quit = make(chan int, n)
    go test()
    for {
        quit <- 1
        glo++
    }
}

```

随着n的变化test的输出有所不同，当n比较小的时候（比如10000），test的输出和n一致，当n较大时（比如200000、1000000），test会输出一个非零但比n小的数。这说明向channel发送信息的操作是可以触发调度的。n较大时输出与n不一致就是出现了抢断的证据。

初步结论：对无缓冲和有缓冲的channel操作都能够触发调度。

### select会不会触发调度？

```
package main
import "fmt"

func test() {
    fmt.Println(i)
}

var i int

func main() {
    n := 1000000
    ch1 := make(chan int, n)
    for i = 0; i < n; i++ {
        ch1 <- i
    }
    i = 0;
    go test()
    for {
        select {
        case <- ch1:
            i++
        }
    }
}

```

出现了和前面channel一样的情况，事实上这里的select会调用channel的读操作，所以自然也能触发调度。

初步结论：select能够触发调度。

### 总结

channel、select可以放心使用，不用担心阻塞导致全局锁死的情况，系统调用会引发调度，绝大部分的函数调用也会引发调度，但是抢占式调度仍然非常原始，不能有依赖心理。

[Golang](http://cholerae.com/categories/Golang/)

[Golang](http://cholerae.com/tags/Golang/), [concurrency](http://cholerae.com/tags/concurrency/)

来源： <http://cholerae.com/2015/05/09/%E5%85%B3%E4%BA%8Egoroutine%E8%B0%83%E5%BA%A6%E6%97%B6%E6%9C%BA%E7%9A%84%E8%AF%95%E9%AA%8C/>





