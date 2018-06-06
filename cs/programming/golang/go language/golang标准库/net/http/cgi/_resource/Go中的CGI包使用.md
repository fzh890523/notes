

## [Go中的CGI包使用](http://www.cnblogs.com/yjf512/archive/2012/12/25/2831891.html)

2012-12-25 10:03 by 轩脉刃, 1670 阅读, 8 评论, [收藏](http://www.cnblogs.com/yjf512/archive/2012/12/25/2831891.html#), [编辑](http://i.cnblogs.com/EditPosts.aspx?postid=2831891)

Go中包含有CGI包，net/http/cgi，这篇文章就是来阅读和使用这个包。关于cgi的参数和运行，可以看这篇文章：[CGI的一些知识点](http://www.cnblogs.com/yjf512/archive/2012/12/24/2830730.html)

# CGI包阅读

cgi包的存在就告诉我们一件事情，cgi服务端和客户端完全可以使用Go来写

这个包其实很简单，只有两个文件，其他都是测试程序

child.go

host.go

 

host.go是可以直接宿主到go的web服务器上的代码，里面提供了对request和response的直接处理函数ServerHTTP, 当你是使用go的http包写了个http之后，就可以使用ServerHTTP对请求直接配置上cgi，有点像apache中自带了php-cgi

child.go则是已经进入到脚本子进程中了，如果你的CGI脚本是go代码生成的可执行脚本，那么你就会有用到这个文件里面的函数了。这个文件内提供了将命令行环境（CGI请求）转换成Go的http包中的request的方法。

host.go是cgi的启动父程序需要用到的包，child.go是子程序需要用到的包

# 先看host.go

首先是trailingPort，这个变量是cgi服务器监听的端口号，（比如在nginx中我们一般都监听9000）

然后是osDefaultInheritEnv，这个map将各个平台的共享库默认路径列出来了。为什么设置这个变量呢？这样说，由于cgi服务器执行命令的时候命令查找设置参数有的是去环境变量中获取的，因此对每个命令执行需要设置一下环境变量。而在不同的平台，动态库的路径是不一样的，所以有了这么个Map。

 

Handler是在子程序中执行cgi脚本的。里面要注意的结构是两个Env和InheritEnv两个，一个是特别设定的环境变量，另外一个是继承的环境变量。

还有Handler中的Path，就是执行文件的路径，比如/test.php

 

下面就是最重要的ServeHTTP了，这个是用来回调处理HTTP请求的，它会将HTTP请求转化为CGI请求，并且执行这个cgi脚本。

在这个函数中，能看到CGI的RFC标准参数赋值，然后可以看到拼出了env之后将env作为exec.Cmd的Env来调用cgi脚本（path）。同时也看到了当body内有content的时候，会将Body作为stdin输入，然后从stdout出来的东西逐行读取，然后读取到header和body中去。

 

看了host.go的实现就很好理解child.go的实现了。

从Serve（handler）来看，先是使用将nginx提供的cgi请求转换成为net包中的http request和response，如果你有设置handler，就用request和response来进行处理。

后续的几个操作Write，Flush都已经是简单的buffer处理了。

# CGI包使用

然后自然想到的一个问题，能不能实现

## go-web服务器 + go-cgi + cgi-script

 

这个是可以做的，而且也不复杂：

代码如下：

如果你在cgi-script中有个可运行的cgi脚本，比如test.perl

那么我们就可以在浏览器中调用[http://10.16.15.64:8989/test.](http://10.16.15.64:8989/test.go)perl

来进行脚本调用

 

然后进一步想，能不能把go代码当作是php这样的动态脚本来运行呢，这样就可以一边修改go源码，一边就可以在页面中立刻显示修改结果了。即

## go-web + go-cgi + go-cgi-script?

答案同样也是可以，但是这个时候由于xx.go并非是可执行文件，只能使用go run 来进行调用。

代码：

 

然后在cgi-script文件夹中建立test.go

这里的init()是必须打印出来的

然后就可以打印出页面了

[![Image](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/52bab643-d82c-4298-9137-08e70ece87f2/index_files/72420301-62d9-49fc-bca1-da78fd0b46b9.png)](http://images.cnitblog.com/blog/136188/201212/25100259-94c4159a567f475bb1a6852ba919516a.png)

这时候go就像php脚本一样，如果你修改了test.go就能立刻在页面上显示出来了

 

对于第二种，当然也可以使用监控文件夹的修改等，当go项目修改后就重新build出一个可执行脚本，然后cgi的handler就修改下路由指向到这个可执行脚本。

关于监控文件的项目现在已经有很多开源的了：比如这个<https://github.com/jianfengye/go-superviser>

 

Ps： 文中说的例子已经加到<http://go.funaio.com/pkg/net/http/cgi/> 的Example中了。

![Creative Commons License](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/52bab643-d82c-4298-9137-08e70ece87f2/index_files/92310d46-d273-4716-8a8a-d1f8571c349c.png)

专注Web开发50年。请加群：[![Thinking in Web](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/52bab643-d82c-4298-9137-08e70ece87f2/index_files/c3635fca-d260-4505-b02a-fa624185c805.png)](http://shang.qq.com/wpa/qunwpa?idkey=017d388202803b8f514317087f69ffabc5a1340342943a0fd8d3af1148653eb5)

本文基于[署名-非商业性使用 3.0](http://creativecommons.org/licenses/by-nc/3.0/deed.zh)许可协议发布，欢迎转载，演绎，但是必须保留本文的署名[叶剑峰](http://www.cnblogs.com/yjf512/)（包含链接http://www.cnblogs.com/yjf512/），且不得用于商业目的。如您有任何疑问或者授权方面的协商，请[与我联系](mailto:jianfengye110@gmail.com)。

来源： <<http://www.cnblogs.com/yjf512/archive/2012/12/25/2831891.html>>

 

