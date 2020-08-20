

### channel in Go's runtime

20 September 2013

by [skoo](http://skoo.me/skoo.html)

Go语言有一个非常大的亮点就是支持语言级别的并发。语言级别提供并发编程，究竟有多重要，可能需要你亲自去体会多线程、事件+callback等常见的并发并发编程模型后才能准确的感受到。为了配合语言级别的并发支持，channel组件就是Go语言必不可少的了。官方提倡的一个编程信条——“使用通信去共享内存，而不是共享内存去通信”，这里说的”通信去共享内存”的手段就是channel。

channel的实现位于[runtime/chan.c](http://golang.org/src/pkg/runtime/chan.c)文件中。

#### channel底层结构模型

![img](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/37db0f4d-a7e2-49f8-ad8d-1b605b24c95f/index_files/d38d9111-2f9e-4c65-8798-e6985085325a.png)

每个channel都是由一个Hchan结构定义的，这个结构中有两个非常关键的字段就是recvq和sendq。recvq和sendq是两个等待队列，这个两个队列里分别保存的是等待在channel上进行读操作的goroutine和等待在channel上进行写操作的goroutine。

当我们使用make()创建一个channel后，这个channel的大概内存模型就如上图，有一个Hchan结构头部，头部后面的所有内存将被划分为一个一个的slot，每个slot将存储一个元素。slot的个数当然就是make channel时指定的缓冲大小。如果make的channel是无缓冲的，那么这里就没有slot了，就只有Hchan这个头部结构。channel的这个底层实现就是分配的一段连续内存(数组)，不是采用的链表或者其他的什么高级数据结构，事实上做这件事情也不需要高级的数据结构了。

这里的所有slot形成的数组本身在不移动内存的情况下，是无法做到FIFO的，事实上，Hchan中还有两个关键字段recvx和sendx，在它们的配合下就将slot数组构成了一个循环数组，就这样利用数组实现了一个循环队列。

这里得吐槽一小段代码，这段代码就是在make一个channel的函数中。

```
#define	MAXALIGN	7

Hcan *c;

// calculate rounded size of Hchan
n = sizeof(*c);
while(n & MAXALIGN)
	n++;

```

这里的while循环就是要将Hchan结构的大小向上补齐到8的倍数，这样后面的内存空间就是按8字节对齐了。为了完成这个向上的补齐操作，最坏情况要执行7次循环，而事实上是可以一步到位的补齐到8的倍数，完全没必要一次一次的加1进行尝试。这个细节其实在很多代码里都有，Nginx就做得很优雅。我是想说Go的部分代码还是挺奔放的，我个人很不喜欢runtime里面的一些函数/变量的命名。

#### 写channel

有了channel的底层结构模型，基本上也能想象一个元素是如何在channel进行”入队/出队”了。完成写channel操作的函数是`runtime·chansend`，这个函数同时实现了同步/异步写channel，也就是带/不带缓冲的channel的写操作都是在这个函数里实现的。同步写，还是异步写，其实就是判断是否有slot。这里叙述一下写channel的过程，不再展示代码了。

1. 加锁，锁住整个channel结构（就是上面的贴图模型）。加锁是可以理解，只是这个锁也够大的。所以，是否一定总是通过“通信来共享内存”是需要慎重考虑的。这把锁可以看出，channel很多时候不一定有直接对共享变量加锁效率高。
2. 现在已经锁住了整个channel了，可以开始干活了。判断是否有slot(是否带缓冲)，如果有就做异步写，没有就做同步写。
3. 假设第2步判断的是**同步写**，那么就试着从`recvq`等待队列里取出一个等待的goroutine，然后将要写入的元素直接交给(拷贝)这个goroutine，然后再将这个拿到元素的goroutine给设置为ready状态，就可以开始运行了。到这里并没有完，如果`recvq`里，并没有一个等待的goroutine，那么就将待写入的元素保存在当前执行写的goroutine的结构里，然后将当前goroutine入队到`sendq`中并被挂起，等待有人来读取元素后才会被唤醒。这个时候，同步写的过程就真的完成了。
4. 假设第2步判断的是**异步写**，异步写相对同步写来说，依赖的对象不再是是否有goroutine在等待读，而是缓冲区是否被写满（是否还有slot）。因此，异步写的过程和同步写大体上也是一样的。首先是判断是否还有slot可用，如果没有slot可用了，就将当前goroutine入队到`sendq`中并被挂起等待。如果有slot可用，就将元素追加到一个slot中，再从`recvq`中试着取出一个等待的goroutine开始进行读操作(如果recvq中有等待读的goroutine的话)。到这里，异步写也就完成了。

异步写和同步写在逻辑过程上基本是相同的，只是依赖的对象不一样而已。同步写依赖是否有等待读的goroutine，异步写依赖是否有可用的缓冲区。

#### 读channel

我们知道了写过程的逻辑，试着推测一下读过程其实一点也不难了。有了写，本质上就有了读了。完成读channel操作的函数是`runtime·chanrecv`, 下面简单的叙述一下读过程。

1. 同样首先加锁，锁住整个channel好干活。
2. a通过是否带缓冲来判断做同步读还是异步读, 类似写过程。
3. 假设是同步读，就试着从`sendq`队列取出一个等待写的goroutine，并把需要写入的元素拿过来(拷贝)，再将取出的goroutine给ready起来。如果`sendq`中没有等待写的goroutine，就只能把当前读的goroutine给入队到`recvq`并被挂起了。
4. 假设是异步读，这个时候就是判断缓冲区中是否有一个元素，没的话，就是将当前读goroutine给入队到`recvq`并被挂起等待。如果有元素的话，当然就是取出最前面的元素，同时试着从`sendq`中取出一个等待写的goroutine唤醒它。

通过读写过程可以看出，读和写是心心相惜的，里面有一个非常重要的细节——读需要去”唤醒”写的goroutine，写的时候需要去“唤醒”读的goroutine。所以这里的读写过程其实是成对出现，配合完成工作的，缺少一个都不行。(我好像在说废话)

#### 无限大channel的实现

有同事提到如何实现一个不限制缓冲区大小的channel，同时还支持select操作。select的实现，放下一次讨论了。不管用什么语言，要实现一个无限制大小的channel，应该都不难。在目前channel的基础如何实现一个无限制大小的channel，在这里我大概说一下我的想法，抛砖引玉。

现在的channel其实就一个数组而已，为了避免内存拷贝，可以在目前的基础上加一层链表结构。这样一来，只要缓冲区用完后，就可以分配一个新的slot数组，并且和老的数组给链起来构成一个更大的缓冲区。这里代码上最复杂的应该是元素被读走后，需要将空的数组给释放掉。加入链表来构造无限制的channel实现看上去是一种比较简单有效的方案。

如果channel是无限制缓冲大小的，那么写入的goroutine就永远不会被挂起等待了，也就不要`sendq`队列了。当然，没消费者或者消费者挂掉的话，这个channel最终也会导致内存爆掉。所以，无限制大小的channel是否真的有必要？？？

了解了channel的底层实现，应该可以更好选择“通信去共享内存，还是共享内存去通信”，没有什么是银弹。

注：本文是基于go1.1.2版本代码。

- **
- [go 17](http://skoo.me/categories.html#go-ref)


- **

- [Go 17](http://skoo.me/tags.html#Go-ref)

- [runtime 11](http://skoo.me/tags.html#runtime-ref)

- ​

- 来源： <<http://skoo.me/go/2013/09/20/go-runtime-channel/>>

