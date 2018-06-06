## [Golang Http Server源码阅读](http://www.cnblogs.com/yjf512/archive/2012/08/22/2650873.html)

2012-08-22 15:04 by 轩脉刃, 13404 阅读, 2 评论, [收藏](http://www.cnblogs.com/yjf512/archive/2012/08/22/2650873.html#), [编辑](http://i.cnblogs.com/EditPosts.aspx?postid=2650873)

这篇文章出现的理由是业务上需要创建一个Web Server。创建web是所有语言出现必须实现的功能之一了。在nginx+fastcgi+php广为使用的今天，这里我们不妨使用Go来进行web服务器的搭建。

# 前言

使用Go搭建Web服务器的包有很多，大致有下面几种方法，直接使用net包，使用net.http包，使用第三方包（比如gorilla）。使用net包就需要从tcp层开始封装，耗费人力物力极大，果断舍弃。直接使用封装好的net.http和第三方包才是上策。这里我们就选择了使用官方提供的net.http包来搭建web服务。另外附带一句，gorilla的第三方包现在使用还是非常广的，文档也是比较全的，有兴趣的同学可以考虑使用一下。

 

建议看这篇文章前先看一下net/http文档 <http://golang.org/pkg/net/http/>

 

net.http包里面有很多文件，都是和http协议相关的，比如设置cookie，header等。其中最重要的一个文件就是server.go了，这里我们阅读的就是这个文件。

# 几个重要概念

ResponseWriter： 生成Response的接口

Handler： 处理请求和生成返回的接口

ServeMux： 路由，后面会说到ServeMux也是一种Handler

Conn : 网络连接

 

# 具体分析

（具体的说明直接以注释形式放在代码中）

## 几个接口：

## Handler

实现了handler接口的对象就意味着往server端添加了处理请求的逻辑。

下面是三个接口(ResponseWriter, Flusher, Hijacker)：

## ResponseWriter, Flusher, Hijacker

## response

实现这三个接口的结构是response(这个结构是http包私有的，在文档中并没有显示，需要去看源码)

 

在response中是可以看到

这么几个方法。所以说response实现了ResponseWriter,Flusher,Hijacker这三个接口

 

## HandlerFunc

handlerFunc是经常使用到的一个type

 

这里需要多回味一下了，这个HandlerFunc定义和ServeHTTP合起来是说明了什么？说明HandlerFunc的所有实例是实现了ServeHttp方法的。另，实现了ServeHttp方法就是什么？实现了接口Handler!

 

所以你以后会看到很多这样的句子：

 

请不要讶异，你明明没有写ServeHttp，怎么能调用呢？ 实际上调用ServeHttp就是调用AdminHandler。

好吧，理解这个也花了我较长时间，附带上一个play.google写的一个小例子

<http://play.golang.org/p/nSt_wcjc2u>

有兴趣继续研究的同学可以继续试验下去

 

如果你理解了HandlerFunc，你对下面两个句子一定不会讶异了

 

下面接着看Server.go

## ServerMux结构

它就是http包中的路由规则器。你可以在ServerMux中注册你的路由规则，当有请求到来的时候，根据这些路由规则来判断将请求分发到哪个处理器（Handler）。

它的结构如下：

下面看一下muxEntry

看到这两个结构就应该对请求是**如何路由**的有思路了：

### 当一个请求request进来的时候，server会依次根据ServeMux.m中的string（路由表达式）来一个一个匹配，如果找到了可以匹配的muxEntry,就取出muxEntry.h,这是个handler，调用handler中的ServeHTTP（ResponseWriter, *Request）来组装Response，并返回。

 

ServeMux定义的方法有:

 

在godoc文档中经常见到的**DefaultServeMux是http默认使用的ServeMux**

var DefaultServeMux = NewServeMux()

如果我们没有自定义ServeMux，系统默认使用这个ServeMux。

 

换句话说，http包外层（非ServeMux）中提供的几个方法：

实际上就是调用ServeMux结构内部对应的方法。

 

## Server

下面还剩下一个Server结构

Server提供的方法有：

 

当然Http包也直接提供了方法供外部使用，实际上内部就是实例化一个Server，然后调用ListenAndServe方法

 

# 具体例子分析

下面根据上面的分析，我们对一个例子我们进行阅读。这个例子搭建了一个最简易的Server服务。当调用[http://XXXX:12345/hello](http://xxxx:12345/hello)的时候页面会返回“hello world”

 

## 首先调用Http.HandleFunc

按顺序做了几件事：

1 调用了DefaultServerMux的HandleFunc

2 调用了DefaultServerMux的Handle

3 往DefaultServeMux的map[string]muxEntry中增加对应的handler和路由规则

 

## 其次调用http.ListenAndServe(":12345", nil)

按顺序做了几件事情：

1 实例化Server

2 调用Server的ListenAndServe()

3 调用net.Listen("tcp", addr)监听端口

4 启动一个for循环，在循环体中Accept请求

5 对每个请求实例化一个Conn，并且开启一个goroutine为这个请求进行服务go c.serve()

6 读取每个请求的内容w, err := c.readRequest()

7 判断header是否为空，如果没有设置handler（这个例子就没有设置handler），handler就设置为DefaultServeMux

8 调用handler的ServeHttp

9 在这个例子中，下面就进入到DefaultServerMux.ServeHttp

10 根据request选择handler，并且进入到这个handler的ServeHTTP

​       mux.handler(r).ServeHTTP(w, r)

11 选择handler：

​    A 判断是否有路由能满足这个request（循环遍历ServerMux的muxEntry）

​    B 如果有路由满足，调用这个路由handler的ServeHttp

​    C 如果没有路由满足，调用NotFoundHandler的ServeHttp

# 后记

对于net.http包中server的理解是非常重要的。理清serverMux, responseWriter, Handler, HandlerFunc等常用结构和函数是使用go web的重要一步。个人感觉由于go中文档较少，像这样有点复杂的包，看godoc的效果就远不如直接看代码来的快和清晰了。实际上在理解了http包后，才会对godoc中出现的句子有所理解。后续还会写一些文章关于使用net.http构建web server的。请期待之。

![Creative Commons License](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/1cb01c9a-e29d-4473-9fc1-69f7a86ec7fe/index_files/93c08ecf-8be8-458d-9a98-ca43b749bb5c.png)

专注Web开发50年。请加群：[![Thinking in Web](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/1cb01c9a-e29d-4473-9fc1-69f7a86ec7fe/index_files/be897649-f9c0-4911-b835-ffe45c8727fc.png)](http://shang.qq.com/wpa/qunwpa?idkey=017d388202803b8f514317087f69ffabc5a1340342943a0fd8d3af1148653eb5)

来源： <<http://www.cnblogs.com/yjf512/archive/2012/08/22/2650873.html>>