

# ET VS LT

ref：

* [服务器编程】EPOLL的LT和ET模式的区别和理解](http://blog.csdn.net/jammg/article/details/51854436)

* [epoll LT/ET 深度剖析](http://www.cnblogs.com/dongfuye/p/5274544.html) or [local](resource/epoll LT_ET 深度剖析 - dongfuye - 博客园.html)

* [彻底学会使用epoll（二）——ET和LT的触发方式](http://blog.chinaunix.net/xmlrpc.php?r=blog/article&uid=28541347&id=4285054)

* [epoll的ET和LT模式详解](http://blog.csdn.net/eroswang/article/details/4481521)




## 概念

* Level Triggered (LT) 水平触发
  * socket接收缓冲区不为空 有数据可读 读事件一直触发
  * socket发送缓冲区不满 可以继续写入数据 写事件一直触发
  * 符合思维习惯，epoll_wait返回的事件就是socket的状态
* Edge Triggered (ET) 边沿触发
  * socket的接收缓冲区状态变化时触发读事件，即空的接收缓冲区刚接收到数据时触发读事件
  * socket的发送缓冲区状态变化时触发写事件，即满的缓冲区刚空出空间时触发读事件
  * 仅在状态变化时触发事件



### 使用细节

* LT的处理过程：

  * accept一个连接，添加到epoll中监听`EPOLLIN`事件

  * 当`EPOLLIN`事件到达时，read fd中的数据并处理

  * 当需要写出数据时，把数据write到fd中；如果数据较大，无法一次性写出，那么在epoll中监听`EPOLLOUT`事件

    > {yonka} 听起来像是第一次write时是直接write而不是event-driven = = - **对的**

  * 当`EPOLLOUT`事件到达时，继续把数据write到fd中；如果数据写出完毕，那么在epoll中关闭`EPOLLOUT`事件

* ET的处理过程：

  * accept一个一个连接，添加到epoll中监听`EPOLLIN|EPOLLOUT`事件
  * 当`EPOLLIN`事件到达时，read fd中的数据并处理，read需要一直读，直到返回`EAGAIN`为止
  * 当需要写出数据时，把数据write到fd中，直到数据全部写完，或者write返回`EAGAIN`
  * 当`EPOLLOUT`事件到达时，继续把数据write到fd中，直到数据全部写完，或者write返回`EAGAIN`

从ET的处理过程中可以看到，ET的要求是需要一直读写，直到返回EAGAIN，否则就会遗漏事件。而LT的处理过程中，直到返回EAGAIN不是硬性要求，但通常的处理过程都会读写直到返回EAGAIN，但LT比ET多了一个开关EPOLLOUT事件的步骤



#### {yonka} 为什么ET/LT在使用上有这么大差别呢？

* ET下需要**一直**写入/读出直到**用完这次事件带来的额度（meet EAGAIN）**，因为kernel不会保留**这次事件的剩余额度**所以不这样做的话就会**丢失事件（实际是丢失额度吧）**

  那么问题来了： 如果当前额度确实用不完该怎么办呢？

  > 参见下面的…，用不完其实也不浪费，因为下批次write时（第一次write操作时）是不需要OUT的，而是先write到EAGAIN再...

* LT则只需要保证当前要读/写的内容都完成，至于**额度是不是使用完**则不用关心，kernel会帮忙保留

  只有当当前额度不够用时，才需要注册事件。 （IN和OUT有差别，一般情况下需要输出的内容都已经是准备好了的，只看能不能写； 而输入的内容则依赖外部，所有IN事件是一直注册 - 一般网络服务系统实际都是 input-driven）



#### 为什么LT要开关EPOLLOUT？

* 因为LT下kernel会保持信号；
* 虽然应用层无可写内容，但TCP层（等）可以是可写状态
* 那么如果不关EPOLLOUT的话，每次都会poll到EPOLLOUT信号，浪费sys/usr CPU
* 所以LT需要在不用（写）时关EPOLLOUT而在需要（写）时打开



而ET

* TCP可写而应用层无可写内容时poll到EPOLLOUT的话，不处理

* kernel不会保留事件

* （如果没有新的事件到来）下次就不会poll到该事件，那么也就不会浪费...

  > {yonka} 问题来了： 等应用层又有内容可写时怎么办？ = =
  >
  > 参考： [epoll-et example frol yedf](https://raw.githubusercontent.com/yedf/handy/master/raw-examples/epoll-et.cc) or [local](resource/epoll-et_example_from_yedf.cc) 可以看到：
  >
  > > 处理方式是： 直接write到EAGAIN，然后下次来了OUT事件继续write



#### 并发情况下怎么处理 - 这里指多线程

不是说同一socket/fd上的io，而是指多个线程的epoll_wait。

1. thread t1刚poll到了fd1上可读

2. t1去读fd1

3. thread t2又poll到fd1可读

   > 尤其是LT

4. t2也去读fd1

5. 那么问题就来了...



可行的应对方式： 被t1 poll到的事件（fd 或者 fd+type）在t1（处理完）释放前不会被poll到。

实际实现上的支持： EPOLLONESHOT

```c
event.events = event.events | EPOLLONESHOT;
```

然后在处理完后做一次重置（重复epoll_ctl）。



可以知道，类似的做法会降低一些效率，所以更合适的做法是不要让多个线程都去poll。





## 理解过程

**请教雅神**



```
​```
@XG 
正因为ET只做触发变化通知，坑就在于如果你认真去一次性处理完请求，理论上会导致饥饿（但只是理论上，如果服务端处理能力够好，看上去饿不死）
LT的通知是按状态的，只要就绪就会通知；ET只有从非就绪变化到就绪才会通知；而这个点痛不痛，实际上在于用户是否在意一次陷入（i.e. 真的调了系统调用）的性能开销

还是不太清楚，ET坑应该是在于没有一次处理完buf里的东西后事件又没了，但如果都一次处理完的话，ET/LT的差别在哪呢？
所以还是回到这个问题：
ET很大程度降低了触发次数（难道所谓的降低触发次数就是‘强逼’了程序员必须一次处理完所有请求？）
已读
XG
或者说这个”触发次数“是指内核为epoll-fd准备事件？
已读
JY
一次处理完的话，ET和LT是没差别的，因为真的都只触发了一次
JY
但问题其实是，一般传输不会只处理一次的，很多缓冲链/矢量缓冲
XG
竞争读的情况会怎样啊？ ET这种的话。

嗯，你说的情况应该是发现pending bytes不足所以”等会儿再读”吧？ 这种情况下ET就gg了。
已读
JY
竞争读？不会是并发去读一个fd吧……如果是这样/即使是这样，无关epoll触发，因为IO是客户代码完成的
XG
嗯，并发读一个fd应该比较少见。
我只是看到有些代码里有epoll到可读然后代码里会有对“未读到内容”的情况的处理，所以当时就猜想是不是被别的线程/进程读走了。
已读
JY
未读到内容是正常现象，因为IO出状况就是正常现象
一个fd可读之后，只代表它在epoll检测的时候可读了；但读的时候可能出种种问题，这里也没有异常抛出来（因为是系统调用）——另外EPOLLIN的触发未必止于可读（比如对端强杀连接，被动端会吃epollin+epollhup）
JY
刚才说的坑实际上也包括这些，epoll标志位的触发不是完全正交的
XG
GET。 可读 -》 不可读 确实没想到。
已读
XG
谢谢。
已读
JY
客气了……
​```
```



## 小结



### 使用上

* LT的编程与poll/select接近，符合一直以来的习惯，不易出错
* ET的编程可以做到更加简洁，某些场景下更加高效，但另一方面容易遗漏事件，容易产生bug



其他：

```
* 在eventloop类型(包括各类fiber/coroutine)的程序中, 处理逻辑和epoll_wait都在一个线程, ET相比LT没有太大的差别. 反而由于LT醒的更频繁, 可能时效性更好些. 
* 在老式的多线程RPC实现中, 消息的读取分割和epoll_wait在同一个线程中运行, 类似上面的原因, ET和LT的区别不大.
* 但在更高并发的RPC实现中, 为了对大消息的反序列化也可以并行, 消息的读取和分割可能运行和epoll_wait不同的线程中, 这时ET是必须的, 否则在读完数据前, epoll_wait会不停地无谓醒来.

作者：gejun
链接：https://www.zhihu.com/question/20502870/answer/142303523
来源：知乎
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
```







### 区别/差别



#### 性能

[epoll LT/ET 深度剖析](http://www.cnblogs.com/dongfuye/p/5274544.html) or [local](resource/epoll LT_ET 深度剖析 - dongfuye - 博客园.html)里提到：

* 对于`容易触发LT开关EPOLLOUT事件的情景`，ET性能好于LT（1x%）
* 开关EPOLLOUT更加频繁时，性能差别更明显（2x%）



### 应用

* ET
  * nginx
* LT
  * 其他通用网络库很多都是LT





### poll到IN却可能没读到东西的情况的解释

> 一个fd可读之后，只代表它在epoll检测的时候可读了；但读的时候可能出种种问题，这里也没有异常抛出来（因为是系统调用）——另外EPOLLIN的触发未必止于可读（比如对端强杀连接，被动端会吃epollin+epollhup）



### 为什么说ET会比LT “ET很大程度降低了触发次数”



就目前的理解（[【服务器编程】EPOLL的LT和ET模式的区别和理解](http://blog.csdn.net/jammg/article/details/51854436)里类似看法）：

只是“迫使”使用者一次读/写完 = =







