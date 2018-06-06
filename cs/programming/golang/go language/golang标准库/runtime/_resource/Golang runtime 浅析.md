

## [Golang runtime 浅析](http://www.cnblogs.com/yjf512/archive/2012/07/19/2599304.html)

2012-07-19 15:05 by 轩脉刃, 8482 阅读, 1 评论, [收藏](http://www.cnblogs.com/yjf512/archive/2012/07/19/2599304.html#), [编辑](http://i.cnblogs.com/EditPosts.aspx?postid=2599304)

从Goroot的代码出发，里面有很多代码非常复杂，一点点看吧。最重要的概念就是runtime，golang的程序都是在runtime的基础上运行的（除了与底层直接交互的syscall）。

# Runtime

在$goroot/pkg/runtime/中有三个文件非常重要：

proc.c

stack.h

runtime.h

 

在runtime.h中你能看到许多的数据结构和接口

这里的数据结构就是go中的各种特定的结构对应的底层实现，比如slice：

其中还有两个重要的结构：

## G

G代表的是goroutine。开启一个goroutine实际就是实例化一个G

## M

M代表的是Machine。M中存放go程序和机器CPU交互的数据结构

比如一个双核CPU，在主routine外开启了4个goroutine，那么实际上就有2个M结构，6个G结构（1个是主routine，4个开启的routine，最后一个是闲置的routine）

runtime和C标准库起的作用是一样的。都是为了语言的跨平台性。runtime可以运行在Windows和Unix平台，可以运行在Intel或ARM处理器上。

[![clip_image001[4\]]()](http://images.cnblogs.com/cnblogs_com/yjf512/201207/201207191504357814.png)

一个go程序都附带一个Runtime，runtime负责与底层操作系统交互。

这篇文章给了一个清晰的runtime概念：<http://pastebin.com/LEsB8FVW>

# 启动流程

回到$goroot/pkg/runtime/proc.c

里面这么个注释：

// The bootstrap sequence is:

//

// call osinit

// call schedinit

// make & queue new G

// call runtime·mstart

//

// The new G calls runtime·main.

明确告诉我们go程序的启动流程是：

## 1 调用osinit,操作系统级别的初始化

## 2 调用runtime·schedinit

在这个函数内做了许多预操作

### 获取程序运行参数

### 获取程序环境变量

（主要是有一个环境变量GOMAXPROCS，你可以使用runtime.GOMAXPROCS(int) 或者直接设置环境变量$GOMAXPROCS改变程序使用的CPU数量）

## 3 调用runtime·mstart启动M

## 4 调用runtime·main

在runtime.main中有这么两行：

main·init(); //调用main包中的init函数

main·main(); //调用main包中的main函数

用gdb调试看trace看到调用栈

[![clip_image002[4\]]()](http://images.cnblogs.com/cnblogs_com/yjf512/201207/201207191504359384.png)

关于启动流程推荐一下这篇文章：<http://www.cnblogs.com/genius0101/archive/2012/04/16/2447147.html>

# go中调用C程序

go中是可以调用C程序的，有两种方法：

## 1 go程序使用import "C"

## 2 使用文件.goc(以前也叫做cgo)

### 第一种方法

例子：

运行

[![clip_image003[4\]]()](http://images.cnblogs.com/cnblogs_com/yjf512/201207/201207191504367682.png)

使用起来非常简单，import "C"之后就有一个全局变量大写C就包含了C库中的函数， include的c库作为注释放在import "C"上面

更多可以参考：

<http://golang.org/doc/articles/c_go_cgo.html>

<http://golang.org/cmd/cgo/>

### 第二种方法

直接创建goc文件，goc文件是C和go混合编写的文件

参照$goroot/src/pkg/runtime/syscall_windows.goc

[![clip_image004[4\]](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/6812c6c6-d840-4842-bbd9-de7f152461f8/index_files/d9631f4a-1fbb-49f0-ba2f-9941110d6c7f.png)](http://images.cnblogs.com/cnblogs_com/yjf512/201207/20120719150437124.png)

这种方式不允许include C的标准库，只能引用自定义的头文件。这种方式很少使用，基本只需要知道一下就好了。

# 参考文章

我之前写了文章把所有golang的底层相关文章列出来了

<http://www.cnblogs.com/yjf512/archive/2012/07/17/2595689.html>

如果有更多的好资料麻烦各位贴出来下~

![Creative Commons License](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/6812c6c6-d840-4842-bbd9-de7f152461f8/index_files/bb4b4bae-e3ce-4cb7-8699-06d7c633e4dd.png)

专注Web开发50年。请加群：[![Thinking in Web](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/6812c6c6-d840-4842-bbd9-de7f152461f8/index_files/90487532-7b10-4865-a98c-2c8346764825.png)](http://shang.qq.com/wpa/qunwpa?idkey=017d388202803b8f514317087f69ffabc5a1340342943a0fd8d3af1148653eb5)

本文基于[署名-非商业性使用 3.0](http://creativecommons.org/licenses/by-nc/3.0/deed.zh)许可协议发布，欢迎转载，演绎，但是必须保留本文的署名[叶剑峰](http://www.cnblogs.com/yjf512/)（包含链接http://www.cnblogs.com/yjf512/），且不得用于商业目的。如您有任何疑问或者授权方面的协商，请[与我联系](mailto:jianfengye110@gmail.com)。

来源： <<http://www.cnblogs.com/yjf512/archive/2012/07/19/2599304.html>>

