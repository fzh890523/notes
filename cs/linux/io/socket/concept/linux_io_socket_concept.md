

## orphan socket

ref：

* [[原创]结合案例深入解析orphan socket产生与消亡（一）](https://yq.aliyun.com/articles/91966) or [local](resource/[原创]结合案例深入解析orphan socket产生与消亡（一）-博客-云栖社区-阿里云.html)
* [结合案例深入解析orphan socket产生与消亡（二）](https://yq.aliyun.com/articles/92925) or [local](resource/结合案例深入解析orphan socket产生与消亡（二）-博客-云栖社区-阿里云.html)



### 概念

### 

```
Do you have "too many" orphan sockets?
First of all: what's an orphan socket? It's simply a socket that isn't associated to a file descriptor. For instance, after you close() a socket, you no longer hold a file descriptor to reference it, but it still exists because the kernel has to keep it around for a bit more until TCP is done with it. Because orphan sockets aren't very useful to applications (since applications can't interact with them), the kernel is trying to limit the amount of memory consumed by orphans, and it does so by limiting the number of orphans that stick around. If you're running a frontend web server (or an HTTP load balancer), then you'll most likely have a sizeable number of orphans, and that's perfectly normal.

```

简要理解即orphan sockets是没有与任何文件描述符关联的socket，应用程序已经不能与此socket进行交互了，但是由于内核中TCP未完成，仍然占用TCP的内存暂时不能释放。



* fd close了，fd与socket解绑。
* 应用程序不能访问socket
* 协议层（因为协议原因）socket没有关闭



### 出现的原因

* 什么原因或条件下会导致出现这么多orphan socket ?
* orphan socket的连接处于TCP状态的那一个阶段？



![](../concept/resource/[原创]结合案例深入解析orphan socket产生与消亡（一）-博客-云栖社区-阿里云_files/10e3ea2de912329ffa09fb478e0d7a63.png)

close调用后的状态：

* server close
  * LAST-ACK

    应该比较少见，毕竟ACK是协议栈完成

    如果多的话，client不回最后一个ACK，可能是恶意或者协议实现缺陷

* client close

  * FIN-WAIT-1

    如果多的话，从client端上看，可能server hang住，因为ACK都回不了 = = （FIN会延迟ACK吗）

  * FIN-WAIT-2

    如果多的话，从client端上看，可能server端应用hang住，无法close

    对应server端应有较多CLOSE-WAIT

  * CLOSING

    这个状态应该比较少见，server端都能回FIN说明更能回ACK

    瞬态的话可能是ACK丢包； 比较多的话可能是协议栈实现缺陷





### 影响

* orphan socket过多会给线上带来什么风险？



直接影响：

* 浪费资源

  * socket（端口）

  * socket buf/mem

    up to 64K

* 触发限制

  * `tcp_max_orphans`

    参见 《linux_io_socket_usage_option&arg&conf.md》



### 应对

























