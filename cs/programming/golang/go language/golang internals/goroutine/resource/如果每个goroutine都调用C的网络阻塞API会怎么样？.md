

如果每个goroutine都调用C的网络阻塞API会怎么样？

比如，goroutine中调用了curl的C库

------

在某版之后，可以设定上限。这样load就不会飙升了。

<[bette...@gmail.com](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/bca07411-ebf8-4076-8337-2bde46baa16c/)>

> 被C阻塞的goroutine是不是最多只有GOMAXPROCS个，其它的都处于等待状态？

------

我目前看到的结果是这样的，除了网络的阻塞用epoll，

其它的阻塞最终都会生成一个线程，

所以goroutine还是不能滥用

好像cgo调用也会开goroutine，最终也是绑到一个线程中运行，cgo的效率应该也不行

------

goroutine遇到阻塞的系统调用或者cgo调用的时候会开出一个thread，这个thread是不占GOMAXPROCS的。因为阻塞而开的和正常跑goroutine的加一起总线程数默认最大是10000，见<https://golang.org/pkg/runtime/debug/#SetMaxThreads> 如果超了会抛panic整个程序直接崩。

------

是的，线程消耗系统资源也很大，调用时间开销也不小，

高并发中用cgo是个灾难，以前还想用C的正则库，

现在看来是不行了

