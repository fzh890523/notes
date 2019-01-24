

## [Go的pprof使用](http://www.cnblogs.com/yjf512/archive/2012/12/27/2835331.html)

2012-12-27 11:07 by 轩脉刃, 7606 阅读, 3 评论, [收藏](http://www.cnblogs.com/yjf512/archive/2012/12/27/2835331.html#), [编辑](http://i.cnblogs.com/EditPosts.aspx?postid=2835331)

go中有pprof包来做代码的性能监控，在两个地方有包：

net/http/pprof

runtime/pprof

其实net/http/pprof中只是使用runtime/pprof包来进行封装了一下，并在http端口上暴露出来

# pprof包

## web 服务器

如果你的go程序是用http包启动的web服务器，你想查看自己的web服务器的状态。这个时候就可以选择net/http/pprof。你只需要引入包_"net/http/pprof"，然后就可以在浏览器中使用<http://localhost:port/debug/pprof/>直接看到当前web服务的状态，包括CPU占用情况和内存使用情况等。具体使用情况你可以看godoc的说明。

## 服务进程

如果你的go程序不是web服务器，而是一个服务进程，那么你也可以选择使用net/http/pprof包，同样引入包net/http/pprof，然后在开启另外一个goroutine来开启端口监听。

比如：

```
go func() {
        log.Println(http.ListenAndServe("localhost:6060", nil)) 
```

}()

## 应用程序

如果你的go程序只是一个应用程序，比如计算fabonacci数列，那么你就不能使用net/http/pprof包了，你就需要使用到runtime/pprof。具体做法就是用到pprof.StartCPUProfile和pprof.StopCPUProfile。比如下面的例子：

```
var cpuprofile = flag.String("cpuprofile", "", "write cpu profile to file")

func main() {
    flag.Parse()
    if *cpuprofile != "" {
        f, err := os.Create(*cpuprofile)
        if err != nil {
            log.Fatal(err)
        }
        pprof.StartCPUProfile(f)
        defer pprof.StopCPUProfile()
    }

```

…

运行程序的时候加一个--cpuprofile参数，比如fabonacci --cpuprofile=fabonacci.prof

这样程序运行的时候的cpu信息就会记录到XXX.prof中了。

下一步就可以使用这个prof信息做出性能分析图了（需要安装graphviz）。

使用go tool pprof (应用程序) （应用程序的prof文件）

进入到pprof，使用web命令就会在/tmp下生成svg文件，svg文件是可以在浏览器下看的。像这个样子：

[![Image]()](http://images.cnitblog.com/blog/136188/201212/27110717-36f5834876b2499397331e0f8ca97db9.png)

如果你的程序非常简单，比如只有println一个语句，你用pprof.StartCPUProfile是打印不出任何东西的。

# 举例

下面拿go-tour举个例子，这是个web程序，我在代码中加入了

_ "net/http/pprof"

在浏览器中我就可以直接看prof信息了

[![Image]()](http://images.cnitblog.com/blog/136188/201212/27110719-2acaf3b25ec0480b973eacba2b5a4006.jpg)

## 生成CPU状态分析图

下面我们想要生成CPU状态分析图，调用go tool pprof http://localhost:3999/debug/pprof/profile

就会进入30秒的profile收集时间，在这段事件内猛刷新点击go-tour浏览器上的页面，尽量让cpu占用性能产生数据。

(pprof) top10

Total: 3 samples

​       1 33.3% 33.3% 1 33.3% MHeap_AllocLocked

​       1 33.3% 66.7% 1 33.3% os/exec.(*Cmd).closeDescriptors

​       1 33.3% 100.0% 1 33.3% runtime.sigprocmask

​       0 0.0% 100.0% 1 33.3% MCentral_Grow

​       0 0.0% 100.0% 2 66.7% main.Compile

​       0 0.0% 100.0% 2 66.7% main.compile

​       0 0.0% 100.0% 2 66.7% main.run

​       0 0.0% 100.0% 1 33.3% makeslice1

​       0 0.0% 100.0% 2 66.7% net/http.(*ServeMux).ServeHTTP

​       0 0.0% 100.0% 2 66.7% net/http.(*conn).serve

 

(pprof)web

[![屏幕快照 2012-12-26 下午10.32.17](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/d38f5c7f-3514-4739-a883-54c858173ace/index_files/21089581-fcff-4798-8b5f-21de7ab00b78.png)](http://images.cnitblog.com/blog/136188/201212/27110720-b2ddf4feacc64446b321282a3d2e0dea.png)

# 参考

<http://blog.golang.org/2011/06/profiling-go-programs.html>

这篇官方文章说得很仔细了，但是要注意，将里面的gopprof工具换成go tool pprof就行了

![Creative Commons License](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/d38f5c7f-3514-4739-a883-54c858173ace/index_files/5bc586e3-1de8-4ba3-be95-1626493d558d.png)

专注Web开发50年。请加群：[![Thinking in Web](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/d38f5c7f-3514-4739-a883-54c858173ace/index_files/d70bf53e-34c7-4b85-8c7c-b9fe1e90605d.png)](http://shang.qq.com/wpa/qunwpa?idkey=017d388202803b8f514317087f69ffabc5a1340342943a0fd8d3af1148653eb5)

本文基于[署名-非商业性使用 3.0](http://creativecommons.org/licenses/by-nc/3.0/deed.zh)许可协议发布，欢迎转载，演绎，但是必须保留本文的署名[叶剑峰](http://www.cnblogs.com/yjf512/)（包含链接http://www.cnblogs.com/yjf512/），且不得用于商业目的。如您有任何疑问或者授权方面的协商，请[与我联系](mailto:jianfengye110@gmail.com)。

来源： <<http://www.cnblogs.com/yjf512/archive/2012/12/27/2835331.html>>

