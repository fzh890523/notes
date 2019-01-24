

新人请问golang开发中，以下两种方式哪种更好

| biliu...@gmail.com | ![img](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABUAAAAVCAYAAACpF6WWAAAAdklEQVR42mNgGAVDHswnwCfLwP94+CSDBqgBCWh8sg1NgGruR+PD8HogLgBiBVINXI/DQHQ8nxjDYYoV0Pj48HsgNqCmS0k2GFeY7ofi+2ji94kNW1js9+OIfQGomvto6qmSTgWgQXae2jlKgJggIDeNjwIaAQBDo0wVdvRN2QAAAABJRU5ErkJggg==)3月10日 |
| ------------------ | ---------------------------------------- |
|                    |                                          |

\1. 先声明一个channel，预先启动多个goroutine从channel等待消息，然后进行处理

\2. 每新到一个请求，直接启动新的goroutine来处理，处理完goroutine自行销毁。如此，频繁新建goroutine。

这两种方式各有何优劣？有何使用场景？谢谢

------

使用方式1的话，消息得有一个固定的格式，或者进行统一封装，

如果消息格式比较随意，像go web开发中，一般都是用方式二，

每个请求开一个goroutine

其它的区别看不出来，静待高手。

------

长连接项目多用1

短连接项目多用2

------

长连接要一直保持连接。

刚好用一个GO程来为一个连接来收数据。

短连接通常是一个连接进来，就会了处理一个消息就关闭。

------

模式 1 类似 worker 模式。

模式 2 类似 fork 模式。

各有优劣，得根据业务需求来选择使用哪种。

------

那你说长连接用方式1不正好反了？

这俩处理长短连接都没问题。Go不管长短连接都是一个网络连接对应一个或两个 goroutine来处理读写。这里讨论的更多是接收到一个具体的请求之后咋处理。

重要的区别是1是有上限的并发，不管并发请求有多高，系统最高并发就那么多；2则是并发请求多少，系统并发就多高。

从这个差别就可以推断两者的应用场景。1不适合需要阻塞的请求，比如HTTP请求需要读body，那1就不合适，不然来N个请求都阻塞就DoS了。当然如果你被body读好然后一起给过去1就可行了。

我感觉一般批处理用1（新请求的等待时间取决于系统负载），需要实时响应的用2。当然这并不绝对，如果能保证worker数目总大于总并发（比如动态起新的后端），1也可以用于实时的系统，但是稍微难设计点。

至于频繁创建 goroutine 是不是有性能问题，想想 net/http 用的就是方式2。就算有性能问题也会是重点优化对象。

来源： <<https://groups.google.com/forum/#!topic/golang-china/1ElrW9Gi3B0>>



