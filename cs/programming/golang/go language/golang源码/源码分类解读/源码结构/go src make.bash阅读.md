

## [go/src/make.bash阅读](http://www.cnblogs.com/yjf512/archive/2013/01/15/2860660.html)

2013-01-15 09:58 by 轩脉刃, 1547 阅读, 2 评论, [收藏](http://www.cnblogs.com/yjf512/archive/2013/01/15/2860660.html#), [编辑](http://i.cnblogs.com/EditPosts.aspx?postid=2860660)

今天看到@polaris1119的这篇[分析源码安装Go的过程（初稿）](http://blog.studygolang.com/116.html)

至今还没仔细看过make.bash的内容。这篇算是阅读笔记了。

# 环境变量

首先是make.bash中用到的环境变量：

GOROOT_FINAL：Go源码的根目录，这个变量的是在gcc的时候使用的，如果你设置了这个，gcc的-D参数就是你设置的

GOHOSTARCH：Go所在的宿主机器的架构，当然这里指的是CPU的架构

GOARCH：安装包和工具所在的机器的架构。

GOOS：安装包和工具所在的机器的操作系统。

GO_GCFLAGS：是否要在编译的时候需要带上5g/6g/8g的参数

GO_LDFLAGS：是否要在链接的时候带上5l/6l/8l的参数

CGO_ENABLED：是否能使用cgo

 

对于环境变量，你可以在编译完成后使用dist工具来查看，比如：

yejianfengtekiMacBook-Air:darwin_amd64 yejianfeng$ ./dist env

GOROOT="/Users/yejianfeng/software/go"

GOBIN="/Users/yejianfeng/software/go/bin"

GOARCH="amd64"

GOOS="darwin"

GOHOSTARCH="amd64"

GOHOSTOS="darwin"

GOTOOLDIR="/Users/yejianfeng/software/go/pkg/tool/darwin_amd64"

GOCHAR="6"

 

# 为什么有GOARCH和GOHOSTARCH的分别？

下面一个情境：amd64的机器上，下载了一份源码，所以GOHOSTARCH是设置成为AMD64，但是我现在想写的是Intel X86-32的Go可执行文件，所以设置GOARCH为Intel86。这就是交叉编译的概念。

当然这两个环境变量是在编译出go工具的时候才有用，具体的编译go代码就没用了。

 

# GOTOOLDIR是什么？

go tool命令后面可以跟其他的工具，比如pprof, yacc, api等。go tool实质上只是一个转发命令给这些工具，这些工具的源代码是放在goroot/src/cmd下面，这些工具的源码编译之后生成的二进制可执行文件就放在$GOTOOLDIR里面，具体的路径就在goroot/pkg/tool/(darwin_amd64)/下。

这个变量是在这句话中设置的

eval $(./cmd/dist/dist env -p)

go_bootstrap是什么？

 

我们可以看到go_bootstrap这个工具是使用dist工具bootstrap生成的。然后再使用go_bootstrap install来编译go的包。最后又悄悄地把go_bootstrap删掉。

我们还原一下，进入GOTOOLDIR，调用./dist bootstrap重新生成go_bootstrap

运行下你会发现，原来就是bin/go嘛，但是再看看大小，只有3M多，而完整的bin/go有5.6M。

所以可以推断go_bootstrap是包含了基本的go代码编译能力的工具。

# 整个make的流程是什么样的？

其实也是几句话就能说清楚：

1 先gcc编译出dist

2 dist编译出go代码基本编译器go bootstrap

3 用go bootstrap编译出go的src中的其他go源码

4 将其他所有工具生成放在src/cmd下供bin/go使用

![Creative Commons License](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/ca7dca6f-eca8-4cd6-a58a-6259b78647d2/index_files/4bed42fd-970e-4c4f-8e5c-f1bcbfb1572d.png)

专注Web开发50年。请加群：[![Thinking in Web](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/ca7dca6f-eca8-4cd6-a58a-6259b78647d2/index_files/75704f52-39dc-477e-ade4-1027401045da.png)](http://shang.qq.com/wpa/qunwpa?idkey=017d388202803b8f514317087f69ffabc5a1340342943a0fd8d3af1148653eb5)

本文基于[署名-非商业性使用 3.0](http://creativecommons.org/licenses/by-nc/3.0/deed.zh)许可协议发布，欢迎转载，演绎，但是必须保留本文的署名[叶剑峰](http://www.cnblogs.com/yjf512/)（包含链接http://www.cnblogs.com/yjf512/），且不得用于商业目的。如您有任何疑问或者授权方面的协商，请[与我联系](mailto:jianfengye110@gmail.com)。

来源： <<http://www.cnblogs.com/yjf512/archive/2013/01/15/2860660.html>>

 

