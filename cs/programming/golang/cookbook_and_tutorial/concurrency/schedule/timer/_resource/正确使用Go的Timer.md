

### 正确使用Go的Timer

06 July 2014

by [skoo](http://skoo.me/skoo.html)

我们总是会使用Timer去执行一些定时任务，最近在Go语言的定时器使用上面不小心踩到一点问题，这里记录一下。

```
go demo(input)

func demo(input chan interface{}) {
    for {
        select {
        case msg <- input:
            println(msg)

        case <-time.After(time.Second * 5):
            println("5s timer")

        case <-time.After(time.Second * 10):
            println("10s timer")
        }
    }
}

```

写出上面这段程序的目的是从 input channel 持续接收消息加以处理，同时希望每过5秒钟和每过10秒钟就分别执行一个定时任务。但是当你执行这段程序的时候，只要 input channel 中的消息来得足够快，永不间断，你会发现启动的两个定时任务都永远不会执行；即使没有消息到来，第二个10s的定时器也是永远不会执行的。原因就是 select 每次执行都会重新执行 case 条件语句，并重新注册到 select 中，因此这两个定时任务在每次执行 select 的时候，都是启动了一个新的从头开始计时的 Timer 对象，所以这两个定时任务永远不会执行。

其实，

```
select {
case msg <- input:

case <-time.After(time.Second)
}

```

这个利用 time.After() 启动 Timer 的编程手法主要是用来解决 channel 操作的 Timeout 问题，而不是执行定时任务。Go 语言采用这种方式来实现 channel 的 Timeout 究竟怎么样？这个话题暂时不在这里分析。

如何正确使用 Timer 来完成上面提到的定时任务？

```
func demo(input chan interface{}) {
    t1 := time.NewTimer(time.Second * 5)
    t2 := time.NewTimer(time.Second * 10)

    for {
        select {
        case msg <- input:
            println(msg)

        case <-t1.C:
            println("5s timer")
            t1.Reset(time.Second * 5)

        case <-t2.C:
            println("10s timer")
            t2.Reset(time.Second * 10)
        }
    }
}

```

改正后的程序，原理上是自定义两个全局的 Timer，每次执行 select 都重复使用这两个 Timer，而不是每次都生成全新的。这样才可以真正做到在接收消息的同时，还能够定时的执行相应的任务。

来源： <<http://skoo.me/go/2014/07/06/use-go-timer/>>

