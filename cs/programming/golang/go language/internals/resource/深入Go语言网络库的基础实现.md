

### 深入Go语言网络库的基础实现

21 April 2014

by [skoo](http://skoo.me/skoo.html)

Go语言的出现，让我见到了一门语言把网络编程这件事情给做“正确”了，当然，除了Go语言以外，还有很多语言也把这件事情做”正确”了。我一直坚持着这样的理念——要做"正确"的事情，而不是"高性能"的事情；很多时候，我们在做系统设计、技术选型的时候，都被“高性能”这三个字给绑架了，当然不是说性能不重要，你懂的。

目前很多高性能的基础网络服务器都是采用的C语言开发的，比如：Nginx、Redis、memcached等，它们都是基于”事件驱动 + 事件回掉函数”的方式实现，也就是采用epoll等作为网络收发数据包的核心驱动。不少人(包括我自己)都认为“事件驱动 + 事件回掉函数”的编程方法是“反人类”的；因为大多数人都更习惯线性的处理一件事情，做完第一件事情再做第二件事情，并不习惯在N件事情之间频繁的切换干活。为了解决程序员在开发服务器时需要自己的大脑不断的“上下文切换”的问题，Go语言引入了一种用户态线程goroutine来取代编写异步的事件回掉函数，从而重新回归到多线程并发模型的线性、同步的编程方式上。

用Go语言写一个最简单的echo服务器：

```
package main

import (
	"log"
	"net"
)

func main() {
	ln, err := net.Listen("tcp", ":8080")
	if err != nil {
        	log.Println(err)
        	return
	}
	for {
        	conn, err := ln.Accept()
        	if err != nil {
            	log.Println(err)
            	continue
        	}

        	go echoFunc(conn)
	}
}

func echoFunc(c net.Conn) {
	buf := make([]byte, 1024)

	for {
        	n, err := c.Read(buf)
        	if err != nil {
            	log.Println(err)
            	return
        	}

        	c.Write(buf[:n])
	}
}

```

main函数的过程就是首先创建一个监听套接字，然后用一个for循环不断的从监听套接字上Accept新的连接，最后调用echoFunc函数在建立的连接上干活。关键代码是:

```
go echoFunc(conn)

```

每收到一个新的连接，就创建一个“线程”去服务这个连接，因此所有的业务逻辑都可以同步、顺序的编写到echoFunc函数中，再也不用去关心网络IO是否会阻塞的问题。不管业务多复杂，Go语言的并发服务器的编程模型都是长这个样子。可以肯定的是，在linux上Go语言写的网络服务器也是采用的epoll作为最底层的数据收发驱动，Go语言网络的底层实现中同样存在“上下文切换”的工作，只是这个切换工作由runtime的调度器来做了，减少了程序员的负担。

弄明白网络库的底层实现，貌似只要弄清楚echo服务器中的Listen、Accept、Read、Write四个函数的底层实现关系就可以了。本文将采用自底向上的方式来介绍，也就是从最底层到上层的方式，这也是我阅读源码的方式。底层实现涉及到的核心源码文件主要有：
[net/fd_unix.go](http://golang.org/src/pkg/net/fd_unix.go) 
[net/fd_poll_runtime.go](http://golang.org/src/pkg/net/fd_poll_runtime.go)
[runtime/netpoll.goc](http://golang.org/src/pkg/runtime/netpoll.goc) 
[runtime/netpoll_epoll.c](http://golang.org/src/pkg/runtime/netpoll_epoll.c) 
[runtime/proc.c](http://golang.org/src/pkg/runtime/proc.c) (调度器)

netpoll_epoll.c文件是Linux平台使用epoll作为网络IO多路复用的实现代码，这份代码可以了解到epoll相关的操作（比如：添加fd到epoll、从epoll删除fd等），只有4个函数，分别是runtime·netpollinit、runtime·netpollopen、runtime·netpollclose和runtime·netpoll。init函数就是创建epoll对象，open函数就是添加一个fd到epoll中，close函数就是从epoll删除一个fd，netpoll函数就是从epoll wait得到所有发生事件的fd，并将每个fd对应的goroutine(用户态线程)通过链表返回。用epoll写过程序的人应该都能理解这份代码，没什么特别之处。

```
void
runtime·netpollinit(void)
{
	epfd = runtime·epollcreate1(EPOLL_CLOEXEC);
	if(epfd >= 0)
		return;
	epfd = runtime·epollcreate(1024);
	if(epfd >= 0) {
		runtime·closeonexec(epfd);
		return;
	}
	runtime·printf("netpollinit: failed to create descriptor (%d)\n", -epfd);
	runtime·throw("netpollinit: failed to create descriptor");
}

```

runtime·netpollinit函数首先使用runtime·epollcreate1创建epoll实例，如果没有创建成功，就换用runtime·epollcreate再创建一次。这两个create函数分别等价于glibc的epoll_create1和epoll_create函数。只是因为Go语言并没有直接使用glibc，而是自己封装的系统调用，但功能是等价于glibc的。可以通过man手册查看这两个create的详细信息。

```
int32
runtime·netpollopen(uintptr fd, PollDesc *pd)
{
	EpollEvent ev;
	int32 res;
	
	ev.events = EPOLLIN|EPOLLOUT|EPOLLRDHUP|EPOLLET;
	ev.data = (uint64)pd;
	res = runtime·epollctl(epfd, EPOLL_CTL_ADD, (int32)fd, &ev);
	return -res;
}

```

添加fd到epoll中的runtime·netpollopen函数可以看到每个fd一开始都关注了读写事件，并且采用的是边缘触发，除此之外还关注了一个不常见的新事件EPOLLRDHUP，这个事件是在较新的内核版本添加的，目的是解决对端socket关闭，epoll本身并不能直接感知到这个关闭动作的问题。注意任何一个fd在添加到epoll中的时候就关注了EPOLLOUT事件的话，就立马产生一次写事件，这次事件可能是多余浪费的。

epoll操作的相关函数都会在事件驱动的抽象层中去调用，为什么需要这个抽象层呢？原因很简单，因为Go语言需要跑在不同的平台上，有Linux、Unix、Mac OS X和Windows等，所以需要靠事件驱动的抽象层来为网络库提供一致的接口，从而屏蔽事件驱动的具体平台依赖实现。runtime/netpoll.goc源文件就是整个事件驱动抽象层的实现，抽象层的核心数据结构是：

```
struct PollDesc
{
	PollDesc* link;	// in pollcache, protected by pollcache.Lock
	Lock;		// protectes the following fields
	uintptr	fd;
	bool	closing;
	uintptr	seq;	// protects from stale timers and ready notifications
	G*	rg;	// G waiting for read or READY (binary semaphore)
	Timer	rt;	// read deadline timer (set if rt.fv != nil)
	int64	rd;	// read deadline
	G*	wg;	// the same for writes
	Timer	wt;
	int64	wd;
};

```

每个添加到epoll中的fd都对应了一个PollDesc结构实例，PollDesc维护了读写此fd的goroutine这一非常重要的信息。可以大胆的推测一下，网络IO读写操作的实现应该是：当在一个fd上读写遇到EAGAIN错误的时候，就将当前goroutine存储到这个fd对应的PollDesc中，同时将goroutine给park住，直到这个fd上再此发生了读写事件后，再将此goroutine给ready激活重新运行。事实上的实现大概也是这个样子的。

事件驱动抽象层主要干的事情就是将具体的事件驱动实现（比如： epoll）通过统一的接口封装成Go接口供net库使用，主要的接口也是：*创建事件驱动实例*、*添加fd*、*删除fd*、*等待事件*以及*设置DeadLine*。`runtime_pollServerInit`负责创建事件驱动实例，`runtime_pollOpen`将分配一个PollDesc实例和fd绑定起来，然后将fd添加到epoll中，`runtime_pollClose`就是将fd从epoll中删除，同时将删除的fd绑定的PollDesc实例删除，`runtime_pollWait`接口是至关重要的，这个接口一般是在非阻塞读写发生EAGAIN错误的时候调用，作用就是park当前读写的goroutine。

runtime中的epoll事件驱动抽象层其实在进入net库后，又被封装了一次，这一次封装从代码上看主要是为了方便在纯Go语言环境进行操作，net库中的这次封装实现在net/fd_poll_runtime.go文件中，主要是通过pollDesc对象来实现的：

```
type pollDesc struct {
	runtimeCtx uintptr
}

```

注意：此处的pollDesc对象不是上文提到的runtime中的PollDesc，相反此处pollDesc对象的runtimeCtx成员才是指向的runtime的PollDesc实例。pollDesc对象主要就是将runtime的事件驱动抽象层给再封装了一次，供网络fd对象使用。

```
var serverInit sync.Once

func (pd *pollDesc) Init(fd *netFD) error {
	serverInit.Do(runtime_pollServerInit)
	ctx, errno := runtime_pollOpen(uintptr(fd.sysfd))
	if errno != 0 {
		return syscall.Errno(errno)
	}
	pd.runtimeCtx = ctx
	return nil
}

```

pollDesc对象最需要关注的就是其Init方法，这个方法通过一个sync.Once变量来调用了runtime_pollServerInit函数，也就是创建epoll实例的函数。意思就是runtime_pollServerInit函数在整个进程生命周期内只会被调用一次，也就是只会创建一次epoll实例。epoll实例被创建后，会调用runtime_pollOpen函数将fd添加到epoll中。

网络编程中的所有socket fd都是通过netFD对象实现的，netFD是对网络IO操作的抽象，linux的实现在文件net/fd_unix.go中。netFD对象实现有自己的init方法，还有完成基本IO操作的Read和Write方法，当然除了这三个方法以外，还有很多非常有用的方法供用户使用。

```
// Network file descriptor.
type netFD struct {
	// locking/lifetime of sysfd + serialize access to Read and Write methods
	fdmu fdMutex

	// immutable until Close
	sysfd       int
	family      int
	sotype      int
	isConnected bool
	net         string
	laddr       Addr
	raddr       Addr

	// wait server
	pd pollDesc
}

```

通过netFD对象的定义可以看到每个fd都关联了一个pollDesc实例，通过上文我们知道pollDesc对象最终是对epoll的封装。

```
func (fd *netFD) init() error {
	if err := fd.pd.Init(fd); err != nil {
		return err
	}
	return nil
}

```

netFD对象的init函数仅仅是调用了pollDesc实例的Init函数，作用就是将fd添加到epoll中，如果这个fd是第一个网络socket fd的话，这一次init还会担任创建epoll实例的任务。要知道在Go进程里，只会有一个epoll实例来管理所有的网络socket fd，这个epoll实例也就是在第一个网络socket fd被创建的时候所创建。

```
for {
	n, err = syscall.Read(int(fd.sysfd), p)
	if err != nil {
		n = 0
		if err == syscall.EAGAIN {
			if err = fd.pd.WaitRead(); err == nil {
				continue
			}
		}
	}
	err = chkReadErr(n, err, fd)
	break
}

```

上面代码段是从netFD的Read方法中摘取，重点关注这个for循环中的syscall.Read调用的错误处理。当有错误发生的时候，会检查这个错误是否是syscall.EAGAIN，如果是，则调用WaitRead将当前读这个fd的goroutine给park住，直到这个fd上的读事件再次发生为止。当这个socket上有新数据到来的时候，WaitRead调用返回，继续for循环的执行。这样的实现，就让调用netFD的Read的地方变成了同步“阻塞”方式编程，不再是异步非阻塞的编程方式了。netFD的Write方法和Read的实现原理是一样的，都是在碰到EAGAIN错误的时候将当前goroutine给park住直到socket再次可写为止。

本文只是将网络库的底层实现给大体上引导了一遍，知道底层代码大概实现在什么地方，方便结合源码深入理解。Go语言中的高并发、同步阻塞方式编程的关键其实是”goroutine和调度器”，针对网络IO的时候，我们需要知道EAGAIN这个非常关键的调度点，掌握了这个调度点，即使没有调度器，自己也可以在epoll的基础上配合协程等用户态线程实现网络IO操作的调度，达到同步阻塞编程的目的。

最后，为什么需要同步阻塞的方式编程？只有看多、写多了异步非阻塞代码的时候才能够深切体会到这个问题。真正的高大上绝对不是——“别人不会，我会；别人写不出来，我写得出来。”

- **
- [go 17](http://skoo.me/categories.html#go-ref)


- **

- [Go 17](http://skoo.me/tags.html#Go-ref)

- [runtime 11](http://skoo.me/tags.html#runtime-ref)

- [net 1](http://skoo.me/tags.html#net-ref)

- ​

- 来源： <<http://skoo.me/go/2014/04/21/go-net-core/>>

