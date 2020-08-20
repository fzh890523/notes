

## [Go语言_并发篇](http://www.cnblogs.com/yjf512/archive/2012/06/06/2537712.html)

2012-06-06 09:55 by 轩脉刃, 10291 阅读, 9 评论, [收藏](http://www.cnblogs.com/yjf512/archive/2012/06/06/2537712.html#), [编辑](http://i.cnblogs.com/EditPosts.aspx?postid=2537712)

当被问到为什么用Go语言，一定不得不提的是Go语言的并发程序编写。在C语言中编写非常繁琐复杂的并发程序在Go语言中总是显得如此便捷。

Go中并发程序依靠的是两个：goroutine和channel

# 理解什么是goroutine?

对于初学者，goroutine直接理解成为线程就可以了。当对一个函数调用go，启动一个goroutine的时候，就相当于起来一个线程，执行这个函数。

实际上，一个goroutine并不相当于一个线程，goroutine的出现正是为了替代原来的线程概念成为最小的调度单位。一旦运行goroutine时，先去当先线程查找，如果线程阻塞了，则被分配到空闲的线程，如果没有空闲的线程，那么就会新建一个线程。注意的是，当goroutine执行完毕后，线程不会回收推出，而是成为了空闲的线程。

关于goroutine的理解，推荐看这个帖子：<http://groups.google.com/group/golang-china/browse_thread/thread/0e9d683ca766ec00>

# goroutine的使用

使用非常简单，在函数前增加一个go

 

f(11)

go f(11) //这个是让f()函数作为goroutine运行

 

但是go有一个缺点，主线程要等待一个goroutine结束再处理怎么办？拿《学习go语言》中的一个例子说明。

[![clip_image001]()](http://images.cnblogs.com/cnblogs_com/yjf512/201206/201206060955122621.png)

这里的第18行为什么要sleep？ 这里是为了等上面两个go ready处理完成。

 

好了，这里就出来了一个需求：一个goroutine结束后必须要向主线程传输数据，告诉主线程这个goroutine已经结束了。

 

这里就引进了channel的概念

# channel的使用

channel的意思用白话可以这么理解：主线程告诉大家你开goroutine可以，但是我在我的主线程开了一个管道，你做完了你要做的事情之后，往管道里面塞个东西告诉我你已经完成了。

上面的例子就可以改为：

[![clip_image002]()](http://images.cnblogs.com/cnblogs_com/yjf512/201206/201206060955132164.png)

从这个程序得到的几点信息：

### 1 channel只能使用make来进行创建

基本格式是 c := make(chan int)

int是说明这个管道能传输什么类型的数据

### 2 往channel中插入数据的操作

c <- 1

是不是很形象

### 3 从channel中输出数据

<- c

### 4 为什么需要输出两次（4和5两行？）

因为2和3启动了两个goroutine，每个goroutine都往管道输出一个1，因此主线程要接收两次才能说明两个goroutine都结束了

# channel的进一步理解：

<http://blog.dccmx.com/2011/05/magic-of-channel-in-go/>

<http://blog.dccmx.com/2012/03/small-problem-about-goroutine/>

 

channel分为两种：一种是有buffer的，一种是没有buffer的，默认是没有buffer的

ci := make(chan int) //无buffer

cj := make(chan int, 0) //无buffer

cs := make(chan int, 100) //有buffer

有缓冲的channel，因此要注意“放”先于“取”

无缓冲的channel，因此要注意“取”先于“放”

<http://blog.dccmx.com/2011/05/magic-of-channel-in-go/> 里面的一个例子很好：

 

同样要先输出hello world，使用有缓冲的channel和无缓冲的channel分别是这样的：

### 有缓冲的channel：

这里有个缓冲，因此放入数据的操作c<- 0先于取数据操作 <-c

### 无缓冲的channel：

由于c是无缓冲的channel，因此必须保证取操作<-c 先于放操作c<- 0

# 参考文档：

Go语言中的channel魔法

<http://blog.dccmx.com/2011/05/magic-of-channel-in-go/>

谈点对goroutine的理解

<http://comments.gmane.org/gmane.comp.lang.go.china/676>

goroutine效果测试

<http://webcache.googleusercontent.com/search?q=cache:Vwa9TGfQKtoJ:blog.weizhe.net/tag/goroutine%2520golang+&cd=6&hl=en&ct=clnk>

Effective Go

<http://golang.org/doc/effective_go.html#goroutines>

![Creative Commons License](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/59c500d0-8dd7-45f6-9e38-b607cbe0de9c/index_files/9e160376-9eb0-483a-a923-b2cfb34303dd.png)

专注Web开发50年。请加群：[![Thinking in Web](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/59c500d0-8dd7-45f6-9e38-b607cbe0de9c/index_files/1908bd2c-287b-43d2-9160-fe0e3dfc1387.png)](http://shang.qq.com/wpa/qunwpa?idkey=017d388202803b8f514317087f69ffabc5a1340342943a0fd8d3af1148653eb5)

本文基于[署名-非商业性使用 3.0](http://creativecommons.org/licenses/by-nc/3.0/deed.zh)许可协议发布，欢迎转载，演绎，但是必须保留本文的署名[叶剑峰](http://www.cnblogs.com/yjf512/)（包含链接http://www.cnblogs.com/yjf512/），且不得用于商业目的。如您有任何疑问或者授权方面的协商，请[与我联系](mailto:jianfengye110@gmail.com)。

来源： <<http://www.cnblogs.com/yjf512/archive/2012/06/06/2537712.html>>

 



