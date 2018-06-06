

## Go 1.5中值得关注的几个变化                     

​                        						                        *2015-09-02*                                                *Tony Bai*                                                [Go中国](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/efb66d7b-afa3-43b3-87a7-670b27dcc31e/)                                                                    

​                        ![img]()                    

在GopherCon2015开幕之 际，Google Go Team终于放出了Go 1.5Beta1版本的安装包。在go 1.5Beta1的发布说明中，Go Team也诚恳地承认Go 1.5将打破之前6个月一个版本的发布周期，这是因为Go 1.5变动太大，需要更多时间来准备这次发布（fix bug, Write doc）。关于Go 1.5的变化，之前Go Team staff在各种golang技术会议的slide  中暴露不少，包括：

\- 编译器和运行时由C改为Go（及少量汇编语言）重写，实现了Go的self Bootstrap(自举）
\- Garbage Collector优化，大幅降低GC延迟（Stop The World），实现Gc在单独的goroutine中与其他user goroutine并行运行。
\- 标准库变更以及一些go tools的引入。

每项变动都会让gopher激动不已。但之前也只是激动，这次beta1出来后，我们可以实际体会一下这些变动带来的“快感”了。Go 1.5beta1的发布文档目前还不全，有些地方还有“待补充”字样，可能与最终go 1.5发布时的版本有一定差异，不过大体内容应该是固定不变的了。这篇文章就想和大家一起浅显地体验一下go 1.5都给gophers们带来了哪些变化吧。

**一、语言**

**【map literal】**

go 1.5依旧兼容Go 1 language specification，但修正了之前的一个“小疏忽”。

Go 1.4及之前版本中，我们只能这么来写代码：

//testmapliteral.go
package main

import (
​    "fmt"
)

type Point struct {
​    x int
​    y int
}

func main() {
​    var sl = []Point{{3, 4}, {5, 6}}
​    var m = map[Point]string{
​        Point{3,4}:"foo1",
​        Point{5,6}:"foo2",
​    }
​    fmt.Println(sl)
​    fmt.Println(m)
}

可以看到，对于Point这个struct来说，在初始化一个slice时，slice value literal中无需显式的带上元素类型Point，即

var sl = []Point{{3, 4}, {5, 6}}

而不是

var sl = []Point{Point{3, 4}, Point{5, 6}}

但当Point作为map类型的key类型时，初始化map时则要显式带上元素类型Point。Go team承认这是当初的一个疏忽，在本次Go 1.5中将该问题fix掉了。也就是说，下面的代码在Go 1.5中可以顺利编译通过：

func main() {
​    var sl = []Point{{3, 4}, {5, 6}}
​    var m = map[Point]string{
​        {3,4}:"foo1",
​        {5,6}:"foo2",
​    }
​    fmt.Println(sl)
​    fmt.Println(m)
}

**【GOMAXPROCS】**

就像这次GopherCon2015上现任Google Go project Tech Lead的Russ Cox的开幕Keynote中所说的那样：Go目标定位于高度并发的云环境。Go 1.5中将标识并发系统线程个数的GOMAXPROCS的初始值由1改为了运行环境的CPU核数。

// testgomaxprocs.go
package main

import (
​    "fmt"
​    "runtime"
)

func main() {
​    fmt.Println(runtime.GOMAXPROCS(-1))
​    fmt.Println(runtime.NumGoroutine())
}

这个代码在Go 1.4下（Mac OS X 4核）运行结果是：

$go run testgomaxprocs.go
1
4

而在go 1.5beta1下，结果为：

$go run testgomaxprocs.go
4
4

**二、编译**

**【简化跨平台编译】**

1.5之前的版本要想实现跨平台编译，需要到$GOROOT/src下重新执行一遍make.bash，执行前设置好目标环境的环境变量(GOOS和 GOARCH)，Go 1.5大大简化这个过程，使得跨平台编译几乎与普通编译一样简单。下面是一个简单的例子：

//testcrosscompile.go
package main

import (
​    "fmt"
​    "runtime"
)

func main() {
​    fmt.Println(runtime.GOOS)
}

在我的Mac上，本地编译执行：
$go build -o testcrosscompile_darwin testcrosscompile.go
$testcrosscompile_darwin
darwin

跨平台编译linux amd64上的目标程序：

$GOOS=linux GOARCH=amd64 go build -o testcrosscompile_linux testcrosscompile.go

上传testcrosscompile_linux到ubuntu 14.04上执行：
$testcrosscompile_linux
linux

虽然从用户角度跨平台编译命令很简单，但事实是go替你做了很多事情，我们可以通过build -x -v选项来输出编译的详细过程，你会发现go会先进入到$GOROOT/src重新编译runtime.a以及一些平台相关的包。编译输出的信息 太多，这里就不贴出来了。但在1.5中这个过程非常快（10秒以内），与1.4之前版本的跨平台编译相比，完全不是一个级别，这也许就是编译器用Go重写完的好处之一吧。

除了直接使用go build，我们还可以使用go tool compile和go tool link来编译程序，实际上go build也是调用这两个工具完成编译过程的。

$go tool compile testcrosscompile.go
testcrosscompile.o
$go tool link testcrosscompile.o
a.out
$a.out
darwin

go 1.5移除了以前的6a,6l之类的编译连接工具，将这些工具整合到go tool中。并且go tool compile的输出默认改为.o文件，链接器输出默认改为了a.out。

**【动态共享库】**

个人不是很赞同Go语言增加对动态共享库的支持，.so和.dll这类十多年前的技术在如今内存、磁盘空间都“非常大”的前提下，似乎已经失去了以往的魅 力。并且动态共享库所带来的弊端："DLL hell"会让程序后续的运维痛苦不已。Docker等轻量级容器的兴起，面向不变性的架构(immutable architecture)受到更多的关注。人们更多地会在container这一层进行操作，一个纯static link的应用在部署和维护方面将会有天然优势，.so只会增加复杂性。如果单纯从与c等其他语言互操作的角度，似乎用途也不会很广泛(但游戏或ui领域 可能会用到)。不过go 1.5还是增加了对动态链接库的支持，不过从go tool compile和link的doc说明来看，目前似乎还处于实验阶段。

既然go 1.5已经支持了shared library，我们就来实验一下。我们先规划一下测试repository的目录结构：

$GOPATH
​    /src
​        /testsharedlib
​            /shlib
​                – lib.go
​        /app
​            /main.go

lib.go中的代码很简单：

//lib.go
package shlib

import "fmt"

// export Method1
func Method1() {
​    fmt.Println("shlib -Method1")
}

对于希望导出的方法，采用export标记。

我们来将这个lib.go编译成shared lib，注意目前似乎只有linux平台支持编译go shared library：

$ go build -buildmode=shared testsharedlib/shlib
\# /tmp/go-build709704006/libtestsharedlib-shlib.so
warning: unable to find runtime/cgo.a

编译ok，那个warning是何含义不是很理解。

要想.so被其他go程序使用，需要将.so安装到相关目录下。我们install一下试试：

$ go install -buildmode=shared testsharedlib/shlib
multiple
 roots /home1/tonybai/test/go/go15/pkg/linux_amd64_dynlink & 
/home1/tonybai/.bin/go15beta1/go/pkg/linux_amd64_dynlink

go工具居然纠结了，不知道选择放在哪里，一个是$GOPATH/pkg/linux_amd64_dynlink，另外一个则是$GOROOT/pkg/linux_amd64_dynlink，我不清楚这是不是一个bug。

在Google了之后，我尝试了网上的一个解决方法，先编译出runtime的动态共享库：

$go install -buildmode=shared runtime sync/atomic

编译安装后，你就会在$GOROOT/pkg下面看到多出来一个目录：linux_amd64_dynlink。这个目录下的结构如下：

$ ls -R
.:
libruntime,sync-atomic.so  runtime  runtime.a  runtime.shlibname  sync

./runtime:
cgo.a  cgo.shlibname

./sync:
atomic.a  atomic.shlibname

这里看到了之前warning提到的runtime/cgo.a，我们再来重新执行一下build，看看能不能消除warning:

$ go build -buildmode=shared testsharedlib/shlib
\# /tmp/go-build086398801/libtestsharedlib-shlib.so
/home1/tonybai/.bin/go15beta1/go/pkg/tool/linux_amd64/link: cannot implicitly include runtime/cgo in a shared library

这回连warnning都没有了，直接是一个error。这里提示：无法在一个共享库中隐式包含runtime/cgo。也就是说我们在构建 testshared/shlib这个动态共享库时，还需要显式的link到runtime/cgo，这里就需要另外一个命令行标志：- linkshared。我们再来试试：

$ go build  -linkshared -buildmode=shared testsharedlib/shlib

这回build成功！我们再来试试install：

$ go install  -linkshared -buildmode=shared testsharedlib/shlib

同样成功了。并且我们在$GOPATH/pkg/linux_amd64_dynlink下发现了共享库：

$ ls -R
.:
libtestsharedlib-shlib.so  testsharedlib

./testsharedlib:
shlib.a  shlib.shlibname

$ ldd libtestsharedlib-shlib.so
​    linux-vdso.so.1 =>  (0x00007fff93983000)
​    libruntime,sync-atomic.so => 
/home1/tonybai/.bin/go15beta1/go/pkg/linux_amd64_dynlink/libruntime,sync-atomic.so
 (0x00007fa150f1b000)
​    libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007fa150b3f000)
​    libpthread.so.0 => /lib/x86_64-linux-gnu/libpthread.so.0 (0x00007fa150921000)
​    /lib64/ld-linux-x86-64.so.2 (0x00007fa1517a7000)

好了，既然共享库编译出来了。我们就来用一下这个共享库。

//app/main.go

package main

import (
​    "testsharedlib/shlib"
)

func main() {
​    shlib.Method1()
}

$ go build -linkshared main.go
$ ldd main
​    linux-vdso.so.1 =>  (0x00007fff579f7000)
​    libruntime,sync-atomic.so => 
/home1/tonybai/.bin/go15beta1/go/pkg/linux_amd64_dynlink/libruntime,sync-atomic.so
 (0x00007fa8d6df2000)
libtestsharedlib-shlib.so => 
/home1/tonybai/test/go/go15/pkg/linux_amd64_dynlink/libtestsharedlib-shlib.so
 (0x00007fa8d6962000)
​    libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007fa8d6586000)
​    libpthread.so.0 => /lib/x86_64-linux-gnu/libpthread.so.0 (0x00007fa8d6369000)
​    /lib64/ld-linux-x86-64.so.2 (0x00007fa8d71ef000)

$ main
shlib -Method1

编译执行ok。从输出结果来看，我们可以清晰看到main依赖的.so以及so的路径。我们再来试试，如果将testsharedlib源码目录移除后，是否还能编译ok：

$ go build -linkshared main.go
main.go:4:2: cannot find package "testsharedlib/shlib" in any of:
​    /home1/tonybai/.bin/go15beta1/go/src/testsharedlib/shlib (from $GOROOT)
​    /home1/tonybai/test/go/go15/src/testsharedlib/shlib (from $GOPATH)

go编译器无法找到shlib，也就说即便是动态链接，我们也要有动态共享库的源码，应用才能编译通过。

【**internal package**】

internal包不是go 1.5的原创，在go 1.4中就已经提出对internal package的支持了。但go 1.4发布时，internal package只能用于GOROOT下的go core核心包，用户层面GOPATH不支持internal package。按原计划，go 1.5中会将internal包机制工作范围全面扩大到所有repository的。我原以为1.5beta1以及将internal package机制生效了，但实际结果呢，我们来看看示例代码：

测试目录结构如下：

testinternal/src
​    mypkg/
​        /internal
​            /foo
​                foo.go
​        /pkg1
​            main.go

​    otherpkg/
​            main.go

按照internal包的原理，预期mypkg/pkg1下的代码是可以import "mypkg/internal/foo"的，otherpkg/下的代码是不能import "mypkg/internal/foo"的。

//foo.go
package foo

import "fmt"

func Foo() {
​    fmt.Println("mypkg/internal/foo")
}

//main.go
package main

import "mypkg/internal/foo"

func main() {
​    foo.Foo()
}

在pkg1和otherpkg下分别run main.go：

mypkg/pkg1$ go run main.go
mypkg/internal/foo

otherpkg$ go run main.go
mypkg/internal/foo

可以看到在otherpkg下执行时，并没有任何build error出现。看来internal机制并未生效。

我们再来试试import $GOROOT下某些internal包，看看是否可以成功：

package main

import (
​    "fmt"
​    "image/internal/imageutil"
)

func main() {
​    fmt.Println(imageutil.DrawYCbCr)
}

我们run这个代码：

$go run main.go
0x6b7f0

同样没有出现任何error。

不是很清楚为何在1.5beta1中internal依旧无效。难道非要等最终1.5 release版么？

**【Vendor】**
Vendor机制是go team为了解决go第三方包依赖和管理而引入的实验性技术。你执行以下go env：

$go env
GOARCH="amd64"
GOBIN="/Users/tony/.bin/go15beta1/go/bin"
GOEXE=""
GOHOSTARCH="amd64"
GOHOSTOS="darwin"
GOOS="darwin"
GOPATH="/Users/tony/Test/GoToolsProjects"
GORACE=""
GOROOT="/Users/tony/.bin/go15beta1/go"
GOTOOLDIR="/Users/tony/.bin/go15beta1/go/pkg/tool/darwin_amd64"
**GO15VENDOREXPERIMENT=""**
CC="clang"
GOGCCFLAGS="-fPIC -m64 -pthread -fno-caret-diagnostics -Qunused-arguments -fmessage-length=0 -fno-common"
CXX="clang++"
CGO_ENABLED="1"

从结果中你会看到新增一个GO15VENDOREXPERIMENT变量，这个就是用来控制vendor机制是否开启的环境变量，默认不开启。若要开启，可以在环境变量文件中设置或export GO15VENDOREXPERIMENT=1临时设置。

vendor机制是在go 1.5beta1发布前不长时间临时决定加入到go 1.5中的，Russ Cox在Keith Rarick之前的一个Proposal的基础上重新做了设计而成，大致机制内容：

If there is a source directory d/vendor, then,
​    when compiling a source file within the subtree rooted at d,
​    import "p" is interpreted as import "d/vendor/p" if that exists.

​    When there are multiple possible resolutions,
​    the most specific (longest) path wins.

​    The short form must always be used: no import path can
​    contain “/vendor/” explicitly.

​    Import comments are ignored in vendored packages.

下面我们来测试一下这个机制。首先我们临时开启vendor机制，export GO15VENDOREXPERIMENT=1，我们的测试目录规划如下：

testvendor
​    vendor/
​        tonybai.com/
​            foolib/
​                foo.go
​    main/
​        main.go

$GOPATH/src/tonybai.com/foolib/foo.go

//vendor/tonybai.com/foolib/foo.go
package foo

import "fmt"

func Hello() {
​    fmt.Println("foo in vendor")
}

//$GOPATH/src/tonybai.com/foolib/foo.go
package foo

import "fmt"

func Hello() {
​    fmt.Println("foo in gopath")
}

vendor和gopath下的foo.go稍有不同，主要在输出内容上，以方便后续区分。

现在我们编译执行main.go

//main/main.go
package main

import (
​    "tonybai.com/foolib"
)

func main() {
​    foo.Hello()
}

$go run main.go
foo in gopath

显然结果与预期不符，我们通过go list -json来看main.go的依赖包路径：

$go list -json
{
… …
​    "Imports": [
​        "tonybai.com/foolib"
​    ],
​    "Deps": [
​        "errors",
​        "fmt",
​        "io",
​        "math",
​        "os",
​        "reflect",
​        "runtime",
​        "strconv",
​        "sync",
​        "sync/atomic",
​        "syscall",
​        "time",
**        "tonybai.com/foolib",**
​        "unicode/utf8",
​        "unsafe"
​    ]
}

可以看出并没有看到vendor路径，main.go import的是$GOPATH下的foo。难道是go 1.5beta1的Bug？于是翻看各种资料，最后在go 1.5beta1发布前最后提交的revison的commit log中得到了帮助：

cmd/go: disable vendoredImportPath for code outside $GOPATH
It was crashing.
This fixes the build for
GO15VENDOREXPERIMENT=1 go test -short runtime

Fixes #11416.

Change-Id: I74a9114cdd8ebafcc9d2a6f40bf500db19c6e825
Reviewed-on: https://go-review.googlesource.com/11964
Reviewed-by: Russ Cox <rsc@golang.org>

从commit log来看，大致意思是$GOPATH之外的代码的vendor机制被disable了（因为某个bug）。也就是说只有$GOPATH路径下的包在 import时才会考虑vendor路径，我们的代码的确没有在$GOPATH下，我们重新设置一下$GOPATH。

$export GOPATH=~/test/go/go15
[tony@TonydeMacBook-Air-2 ~/test/go/go15/src/testvendor/main]$go list -json
{
  … …
​    "Imports": [
​        "testvendor/vendor/tonybai.com/foolib"
​    ],
​    "Deps": [
​        "errors",
​        "fmt",
​        "io",
​        "math",
​        "os",
​        "reflect",
​        "runtime",
​        "strconv",
​        "sync",
​        "sync/atomic",
​        "syscall",
​        **"testvendor/vendor/tonybai.com/foolib",**
​        "time",
​        "unicode/utf8",
​        "unsafe"
​    ]
}

这回可以看到vendor机制生效了。执行main.go:

$go run main.go
foo in vendor

这回与预期结果就相符了。

前面提到，关闭GOPATH外的vendor机制是因为一个bug，相信go 1.5正式版发布时，这块会被enable的。

**三、小结**

Go 1.5还增加了很多工具，如trace，但因文档不全，尚不知如何使用。

Go 1.5标准库也有很多小的变化，这个只有到使用时才能具体深入了解。

Go 1.5更多是Go语言骨子里的变化，也就是runtime和编译器重写。语法由于兼容Go 1，所以基本frozen，因此从外在看来，基本没啥变动了。

至于Go 1.5的性能，官方的说法是，有的程序用1.5编译后会变得慢点，有的会快些。官方bench的结果是总体比1.4快上一些。但Go 1.5在性能方面主要是为了减少gc延迟，后续版本才会在性能上做进一步优化，优化空间还较大的，这次runtime、编译器由c变go，很多地方的go 代码并非是最优的，多是自动翻译，相信经过Go team的优化后，更idiomatic的Go code会让Go整体性能更为优异。

来源： <<http://mp.weixin.qq.com/s?__biz=MjM5OTcxMzE0MQ==&mid=206869973&idx=1&sn=d9f255987a0be4eb37816f00d9146c00&scene=23&srcid=0902P37Lz8X1ZVxE3KBWnunu#rd>>

