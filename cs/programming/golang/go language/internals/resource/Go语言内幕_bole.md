

[伯乐在线](http://www.jobbole.com/) > [首页](http://blog.jobbole.com/) > [所有文章](http://blog.jobbole.com/all-posts/) > [Go](http://blog.jobbole.com/category/go/) > Go语言内幕（1）：主要概念与项目结构

# Go语言内幕（1）：主要概念与项目结构

​            2015/09/07 ·  [Go](http://blog.jobbole.com/category/go/), [开发](http://blog.jobbole.com/category/programmer/)                                                 ·  [Go](http://blog.jobbole.com/tag/go/), [项目结构](http://blog.jobbole.com/tag/%e9%a1%b9%e7%9b%ae%e7%bb%93%e6%9e%84/)

本文由 [伯乐在线](http://blog.jobbole.com/) - [yhx](http://www.jobbole.com/members/scbzyhx) 翻译，[唐尤华](http://www.jobbole.com/members/tangyouhua) 校稿。未经许可，禁止转载！
英文出处：[Sergey Matyukevich](http://blog.altoros.com/golang-part-1-main-concepts-and-project-structure.html)。欢迎加入[翻译组](http://group.jobbole.com/category/feedback/trans-team/)。

这个系列博客主要为那些对 Go 基本知识已经有一定了解，又希望对其内部细节进行更深一步地探索的人准备的。今天这一篇主要分析 Go 源代码的基本结构以及 Go 编译器的某些内部细节。读完这篇博客后，你会得到下面三个问题的答案:

\1. Go 源代码结构是什么样子的？
\2. Go 编译器是如何工作的？
\3. Go 语言中的结点树的基本结构是什么样的？

## 让我们开始吧

每当开始学习一门新编程语言的时候，你总是可以找到大量的 “hello world” 教程、新手指南或者关于语言的主要概念、语法甚至标准库的文档。然而，当你想找一些介绍得更加深入的资料，比如语言运行时分配的数据结构在内存中的布局，或者调用一个内置函数时到底生成了什么样的汇编代码，你就会发现这并非易事。显然，这些问题的答案都藏在源代码中。但是，以我的个人经验来看，你很可能花费数小时在源代码中摸索却最终一无所获。

我并不是打算装得自己什么都懂，也没有打算介绍得面面俱到。而是希望可以帮助你去探索 Go 语言的源代码。

在我们开始之前，我们需要自己有一份 Go 源代码的拷贝。要获得它的源代码非常容易，只需要执行如下代码:

请注意，这份代码的主分支是在不断改进中的，我在这个博客中使用的是 release-brach.go1.4 这个分支。

## 搞清楚项目结构

如果你看一下 Go 仓库的 /src 文件夹，你会看到很多文件夹。其中，大部分文件夹都是 Go 标准库的源文件。该项目使用标准命名规则，所以每一个包（pakage）都在一个独立的文件夹中，而且这个文件夹的名称与包名称相同。除了标准库以外，该目录中还有很多其它的东西。就我各人看法，其中最有用的文件中主要有：

| 文件夹                                      | 描述                                       |
| ---------------------------------------- | ---------------------------------------- |
| [/src/cmd/](https://github.com/golang/go/tree/release-branch.go1.4/src/cmd) | 包含不同的命令行工具。                              |
| [/src/cmd/go/](https://github.com/golang/go/tree/release-branch.go1.4/src/cmd/go) | 该目录下包含一个 Go 工具的源代码文件。此工具用于下载编译 Go 的源文件，以及安装 Go 语言的包。在完成上述工作中，它会收集所有源文件并调用 Go 链接器与编译器。 |
| [/src/cmd/dist/](https://github.com/golang/go/tree/release-branch.go1.4/src/cmd/dist) | 此目录下也包含一个工具。此工具用于编译生成所有其它命令行工具。同时，它会由标准库生成所有的包。要想搞明白每个工具或者包到底用到了哪些库，你就需要分析这里的源代码。 |
| [/src/cmd/gc/](https://github.com/golang/go/tree/release-branch.go1.4/src/cmd/gc) | 包含 Go 编译器与系统架构无关的部分。                     |
| [/src/cmd/ld/](https://github.com/golang/go/tree/release-branch.go1.4/src/cmd/ld) | 包含 Go 链接器与系统架构无关的部分。与系统架构相关的部分被放在以 l 开头的目录中。这些目录的命名规则与编译器部分的命名规则相同。 |
| [/src/cmd/5a/](https://github.com/golang/go/tree/release-branch.go1.4/src/cmd/5a), 6a, 8a, and 9a | 此目录下存放针对不同架构的 Go 语言汇编编译器。Go 汇编程序的语言并不能一一对应地映射到下层机器的汇编语言。不过，对于每种不同的架构都存在一个将 Go 汇编程序翻译为机器汇编程序的编译器。你可以这[这里](https://golang.org/doc/asm)找到更多内容。 |
| [/src/lib9/](https://github.com/golang/go/tree/release-branch.go1.4/src/lib9), [/src/libbio](https://github.com/golang/go/tree/release-branch.go1.4/src/libbio), [/src/liblink](https://github.com/golang/go/tree/release-branch.go1.4/src/liblink) | 在编译器、链接器、以及运行时中用到的不同库。                   |
| [/src/runtime/](https://github.com/golang/go/tree/release-branch.go1.4/src/runtime) | 这部分包含了 Go 语言最重要的包，所有程序都默认导入这些包。其中包括所有的运行时功能，比如内存管理、垃圾回收、Go 协程（goroutine）等等。 |

## Go 编译器内部机制

正如提到的那样，Go 编译器中与系统结构无关的部分被放在 /src/cmd/gc 目录下。其入口点在 lex.c 文件中。除了一些共同的部分，比如命令行参数解析，编译器还要完成如下的工作：

\1. 初始化一些通用数据结构。

\2. 遍历提供的所有 Go 源代码文件，并针对每个文件调用 yyparse 方法。该方法会完成真正的语法分析。Go 编译器使用 Bison 作为程序分析生成器。语法描述存储在文件 go.y 中（后面我会提供详细的说明）。最终，这一步会生成一个完整的分析树，其中每个结点表示编译后程序的一个元素。

\3. 递规地遍历生成的树，并做出一定修改，例如为那些应当隐式定义的节点指定类型信息、重写在运行时包中传递给函数的某些语言元素——如类型转换，以及其它一些工作。

\4. 语法解析树处理完成后，再执行真正的编译，将结点翻译成汇编代码。

5. 在磁盘上创建目标文件，并将翻译生成的汇编代码以及一些额外的数据结构，如符号表等，写入目标文件中。

## 深入 Go 语言语法

现在让我们再进一步。 go.y 文件中包含了语言的语法规则，所以这个文件是一个探索 Go 编译器的很好突破口，也是我们理解语言语法规则的关键。这个文件主要包括如下几部分：

这个声明中定义了 xfndcl 以及 fundcl 两个结点。 fundcl 结点可以有以下两种形式。第一种对应于如下的语法结构：

其第二种形式对应于下面这种语法结构：

xfndcl 结点中包含存储于常量 LFUNC 中的关键字 func，以及其后的 fndcl 与 fnbody 结点。

Bison（或者说 Yacc）语法一个十份重要的特征是，它允许将任意 C 代码放在结点定义之后。每当在源代码文件中找到匹配该结点定义的部分的时候，相应的 C 代码就会执行。这里，我们把最终结果结点定义为 `$ $`，其子结点分别为 $1，$2……

通过一个例子更加容易理解。注意下面这段简化后的代码：

首先，我们创建了一个新结点，该结点包含函数声明的类型信息。同时，此结点的参数列表引用了结点 $3，结果列表引用了结点 $5。随后创建了结果结点 `$ $`。在结果结点中存储了函数的名称和以及其类型结点。 正如你所看到的那样，在 go.y 文件中的定义与结点结构之间可能没有直接的对应关系。

## 如何理解结点

是时候看一下结点到底是什么东西了。首先，结点是一个结构体（你可以在[这里](https://github.com/golang/go/blob/release-branch.go1.4/src/cmd/gc/go.h#L245)找到其定义）。这个结构体包含了大量的属性，这是因为它需要各种不同类型的结点类型，而不同类别的结点又有着不同的属性。下面列出了一些我认为比较重要一些属性：

| 结点结构体域 | 描述                                       |
| ------ | ---------------------------------------- |
| op     | 结点操作符。每个结点都有这个域。它将不同类型的结点区分开来。在前面的例子中，该域分别是 OTFUNC（操作类型函数）与 ODCLFUNC（操作声明函数）。 |
| type   | 该域引用一个包含类型信息的结构体（有些结点没有类型信息，例如，像 if、switch、for 之类的控制流语句）。 |
| val    | 在表示常量的结点中，该域存储常量值。                       |

到目前为止，你已经明白了结点树的基本结构了，你可以去运用一下这些知识。在接下来的博文中，我们会用一个简单的 Go 应用作为实例来分析 Go 编译器到底是如何编译代码的。

来源： <<http://blog.jobbole.com/90574/>>

 

------

##                          Go语言内幕（2）：深入 Go 编译器                     

原创*2015-09-20**伯乐在线*[伯乐在线](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/ea83aec0-d60e-4122-b146-c8ca0aacb3cf/)

**(点击上方公众号，可快速关注)**

当你通过接口引用使用一个变量时，你知道 Go 运行时到底做了哪些工作吗？这个问题并不容易回答。这是因为在 Go 中，一个类型实现了一个接口，但是这个类型并没有包含任何对这个接口的引用。与上一篇博客《[Go语言内幕（1）：主要概念与项目结构](http://mp.weixin.qq.com/s?__biz=MjM5MzA0OTkwMA==&mid=212087461&idx=2&sn=439c1a6404ae94790271006e8a011950&scene=21#wechat_redirect)》一样，你可以用 Go 编译器的知识来回答这个问题。关于 Go 编译器的内容我们已经在上一篇中已经讨论过一部分了。

在这里，让我们更加深入地探索 Go 编译器：创建一个简单的 Go 程序来看一下 Go 内部在类型转换时到底做了哪些工作。通过这个例子，我会解释结点树是如何生成并被使用的。同样地，你也可以将这篇博客的知识应用到其它 Go 编译器特征的研究中。

**前言**

要完成这个实验，我们需要直接使用 Go 编译器（而不是使用 Go 工具）。你可以通过下面的命令来使用：

> go tool 6g test.go

这个命令会编译 test.go 源文件并生成目标文件。这里， 6g 是 AMD64 架构上编译器的名称。请注意，如果你在不同的架构上，请使用不同的编译器。

在直接使用编译器的时候，我们可能会用到一些命令行参数（详细内容请参考这里）。在这个实验中，我们会用到 -W 参数，这个参数会输出结点树的布局结构。

**创建一个简单的 Go 程序**

首先，我们需要先编写一个简单的 Go 程序。 我编写的程序如下：

> package main
>
> type I interface {
>
> ​    DoSomeWork()
>
> }
>
> type T struct {
>
> ​    a int
>
> }
>
> func (t *T) DoSomeWork() {
>
> }
>
> func main() {
>
>    t := &T{}
>
>    i := I(t)
>
>    print(i)
>
> }

这段代码非常简单，不是吗？其中第 17 输出了变量 i 的值，这一行代码看上去多此一举。但是，如果没有这一行代码，程序中就没有使用到变量 i，那么整个程序就不会被编译。接下来，我们将使用 -W 参数来编译我们的程序：

> go tool 6g -W test.go

完成编译后，你会看到输出中包含了程序中定义的每个方法的结点树。在我们这个例子中有 main 和 init 方法。init 方法是隐式生成的，所有的程序都会有这个方法。此处，我们暂将该方法搁置在一边。

对于每个方法，编译器都会输出两个版本的结点树。第一个是刚解析完源文件生成的原始结点树。另外一个则是完成类型检查以及一些必须的修改后的结点树。

**分析 main 方法的结点树**

让我们仔细看一下 main 方法的最初版本结点树，尽量搞清楚 Go 编译器到底做了哪些工作。

> DCL l(15)
>
> .   NAME-main.t u(1) a(1) g(1) l(15) x(0+0) class(PAUTO) f(1) ld(1) tc(1) used(1) PTR64-*main.T
>
> AS l(15) colas(1) tc(1)
>
> .   NAME-main.t u(1) a(1) g(1) l(15) x(0+0) class(PAUTO) f(1) ld(1) tc(1) used(1) PTR64-*main.T
>
> .   PTRLIT l(15) esc(no) ld(1) tc(1) PTR64-*main.T
>
> .   .   STRUCTLIT l(15) tc(1) main.T
>
> .   .   .   TYPE <S> l(15) tc(1) implicit(1) type=PTR64-*main.T PTR64-*main.T
>
> DCL l(16)
>
> .   NAME-main.i u(1) a(1) g(2) l(16) x(0+0) class(PAUTO) f(1) ld(1) tc(1) used(1) main.I
>
> AS l(16) tc(1)
>
> .   NAME-main.autotmp_0000 u(1) a(1) l(16) x(0+0) class(PAUTO) esc(N) tc(1) used(1) PTR64-*main.T
>
> .   NAME-main.t u(1) a(1) g(1) l(15) x(0+0) class(PAUTO) f(1) ld(1) tc(1) used(1) PTR64-*main.T
>
> AS l(16) colas(1) tc(1)
>
> .   NAME-main.i u(1) a(1) g(2) l(16) x(0+0) class(PAUTO) f(1) ld(1) tc(1) used(1) main.I
>
> .   CONVIFACE l(16) tc(1) main.I
>
> .   .   NAME-main.autotmp_0000 u(1) a(1) l(16) x(0+0) class(PAUTO) esc(N) tc(1) used(1) PTR64-*main.T
>
> VARKILL l(16) tc(1)
>
> .   NAME-main.autotmp_0000 u(1) a(1) l(16) x(0+0) class(PAUTO) esc(N) tc(1) used(1) PTR64-*main.T
>
> PRINT l(17) tc(1)
>
> PRINT-list
>
> .   NAME-main.i u(1) a(1) g(2) l(16) x(0+0) class(PAUTO) f(1) ld(1) tc(1) used(1) main.I

下面的分析过程中，我会删除结点树中一些不必要的信息。

第一个结点非常的简单：

> DCL l(15)
>
> .   NAME-main.t l(15) PTR64-*main.T

第一个结点是一个声明结点。 l(15) 说明这个结点的定义在源码的第 15 行。这个声明结点引用了表示 main.t 变量的名称结点。这个变量是定义在 main 包中指向 main.T 类型的一个 64 位指针。你去看一下源代码中的第 15 行就很容易就明白这个声明代表着什么了。

接下来这个结点又是一个声明结点。这一次，这个声明结点声明了一个属于 main.T 类型的变量 main.i。

> DCL l(16)
>
> .   NAME-main.i l(16) main.I

然后，编译器创建了另外一个变量 autotmp_0000, 并将变量 main.t 赋值给该变量。

> AS l(16) tc(1)
>
> .   NAME-main.autotmp_0000 l(16) PTR64-*main.T
>
> .   NAME-main.t l(15) PTR64-*main.T

最后，我们终于看到我们真正感兴趣的结点。

> AS l(16) 
>
> .   NAME-main.i l(16)main.I
>
> .   CONVIFACE l(16) main.I
>
> .   .   NAME-main.autotmp_0000 PTR64-*main.T

我们可以看到编译器将一个特殊的结点 CONVIFACE 赋值给了变量 main.i。但是，这并没有告诉我们在这个赋值背后到底发生了什么。为了搞清楚幕后真相，我们需要去分析一下修改完成后的 main 方法结点树(你可以在输出信息的 “after walk main” 这一小节中看到相关的信息)。

**编译器怎么翻译赋值结点**

下面，你将看到编译器到底是如何翻译赋值结点的：

> AS-init
>
> .   AS l(16)
>
> .   .   NAME-main.autotmp_0003 l(16) PTR64-*uint8
>
> .   .   NAME-go.itab.*"".T."".I l(16) PTR64-*uint8
>
> .   IF l(16)
>
> .   IF-test
>
> .   .   EQ l(16) bool
>
> .   .   .   NAME-main.autotmp_0003 l(16) PTR64-*uint8
>
> .   .   .   LITERAL-nil I(16) PTR64-*uint8
>
> .   IF-body
>
> .   .   AS l(16)
>
> .   .   .   NAME-main.autotmp_0003 l(16) PTR64-*uint8
>
> .   .   .   CALLFUNC l(16) PTR64-*byte
>
> .   .   .   .   NAME-runtime.typ2Itab l(2) FUNC-funcSTRUCT-(FIELD-
>
> .   .   .   .   .   NAME-runtime.typ·2 l(2) PTR64-*byte, FIELD-
>
> .   .   .   .   .   NAME-runtime.typ2·3 l(2) PTR64-*byte PTR64-*byte, FIELD-
>
> .   .   .   .   .   NAME-runtime.cache·4 l(2) PTR64-*PTR64-*byte PTR64-*PTR64-*byte) PTR64-*byte
>
> .   .   .   CALLFUNC-list
>
> .   .   .   .   AS l(16)
>
> .   .   .   .   .   INDREG-SP l(16) runtime.typ·2 G0 PTR64-*byte
>
> .   .   .   .   .   ADDR l(16) PTR64-*uint8
>
> .   .   .   .   .   .   NAME-type.*"".T l(11) uint8
>
> .   .   .   .   AS l(16)
>
> .   .   .   .   .   INDREG-SP l(16) runtime.typ2·3 G0 PTR64-*byte
>
> .   .   .   .   .   ADDR l(16) PTR64-*uint8
>
> .   .   .   .   .   .   NAME-type."".I l(16) uint8
>
> .   .   .   .   AS l(16)
>
> .   .   .   .   .   INDREG-SP l(16) runtime.cache·4 G0 PTR64-*PTR64-*byte
>
> .   .   .   .   .   ADDR l(16) PTR64-*PTR64-*uint8
>
> .   .   .   .   .   .   NAME-go.itab.*"".T."".I l(16) PTR64-*uint8
>
> AS l(16)
>
> .   NAME-main.i l(16) main.I
>
> .   EFACE l(16) main.I
>
> .   .   NAME-main.autotmp_0003 l(16) PTR64-*uint8
>
> .   .   NAME-main.autotmp_0000 l(16) PTR64-*main.T

正如在输入中看到的那样，编译器首先给赋值结点增加了一个初始化结点列表（AS-init）用以分配节点，在 AS-init 结点中，它创建一个新的变量 main.autotmp_0003，并将 go.itab.*”".T.”".I 变量的值赋给新生成的变量。随后检查这个变量是否为 nil。如果变量为 nil，编译器使用如下参数调用 runtime.type2Itab 函数：

> a pointer to the main.T type ,
>
> a pointer to the main.I interface type,
>
> and a pointer to the go.itab.*””.T.””.I variable.

从这部分代码很容易看出，这个变量是用于缓存从 main.T 转换到 main.I 的中间结果。

**getitab 方法内部**

逻辑上来说，下一步就是找到 runtime.typ2Itab 方法。下面就是这个方法：

> func typ2Itab(t *_type, inter *interfacetype, cache **itab) *itab {
>
>   tab := getitab(inter, t, false)
>
>   atomicstorep(unsafe.Pointer(cache), unsafe.Pointer(tab))
>
>   return tab
>
> }

很明显，runtime.typ2Itab 方法中第二行只是简单地创建了一个 tab 变量，所以真正的工作都是在 getitab 方法中完成的。因此，我们再去探索 getitab 方法。因为这个方法的代码量非常巨大，所以我只拷贝了其中最重要的一部分。

> m = 
>
>   (*itab)(persistentalloc(unsafe.Sizeof(itab{})+uintptr(len(inter.mhdr)-1)*ptrSize, 0,
>
>   &memstats.other_sys))
>
>   m.inter = interm._type = typ
>
> ni := len(inter.mhdr)
>
> nt := len(x.mhdr)
>
> j := 0
>
> for k := 0; k < ni; k++ {
>
>   i := &inter.mhdr[k]
>
>   iname := i.name
>
>   ipkgpath := i.pkgpath
>
>   itype := i._type
>
>   for ; j < nt; j++ {
>
> ​      t := &x.mhdr[j]
>
> ​      if t.mtyp == itype && t.name == iname && t.pkgpath == ipkgpath {
>
> ​          if m != nil {
>
> ​              *(*unsafe.Pointer)(add(unsafe.Pointer(&m.fun[0]), uintptr(k)*ptrSize)) = t.ifn
>
> ​          }
>
> ​      }
>
>   }
>
> }

首先，我们为结果分配了一段内存空间：

> (*itab)(persistentalloc(unsafe.Sizeof(itab{})+uintptr(len(inter.mhdr)-1)*ptrSize, 0, &memstats.other_sys))

为什么我们要分配内存空间而且还是以这样奇怪的方式呢？要回答这个问题，我们需要看一下 itab 结构体的定义。

> type itab struct {
>
>   inter  *interfacetype
>
>   _type  *_type
>
>   link   *itab
>
>   bad    int32
>
>   unused int32
>
>   fun    [1]uintptr
>
> // variable sized
>
> }

最后一个属性 fun 定义为一个只有一个元素的数组，但是这个数组的长度实际上是可变的。随后，我们会看到这个可变数组中存储了指向在类型中定义的方法的指针。这些方法对应于接口类型的方法。 Go 语言作者使用动态内存分配的方法为这个属性分配空间（是的，如果你使用 unsafe 包时，这么做是可行的）。分配内存的大小为接口中方法的数量乘以指针的大小再加上结构体本身的大小之和。

> unsafe.Sizeof(itab{})+uintptr(len(inter.mhdr)-1)*ptrSize

接下来，你会看到一个嵌套循环。首先，我们遍历所有接口的方法。对于接口中的每一个方法，我们都会尽力在类型中找到一个对应的方法（这些方法存储于 mhdr 集合中）。检查两个方法是否相同的方法是相当明了的。

> *(*unsafe.Pointer)(add(unsafe.Pointer(&m.fun[0]), uintptr(k)*ptrSize)) = t.ifn

这里做了一点性能上的改进：这些接口以及预置类型的方法都是以字母顺序排列的，这个嵌套循环只需要 O(n + m)  而不是O(n * m)，其中 n 和 m 分别对应于方法的数量。

你还记得赋值的最后一部分吗？

> AS l(16) 
>
> .   NAME-main.i l(16) main.I
>
> .   EFACE l(16) main.I
>
> .   .   NAME-main.autotmp_0003 l(16) PTR64-*uint8
>
> .   .   NAME-main.autotmp_0000 l(16) PTR64-*main.T

这里，我们将 EFACE 结点赋值给 main.i 变量。这个结点（EFACE）包含了对变量 main.autotmp_0003 的引用–指向由 runtime.typ2Itab 方法返回的 itab 结构的指针，还包含对 autotmp_0000 变量的引用 ， autotmp_0000 变量中包含了与 main.t 变量相同的值。以上就是我们通过接口引用调用方法所需的全部信息了。

因此，main.i 变量存储了定义在运行时包中 iface 结构体的一个实例：

> type iface struct {
>
>   tab  *itab
>
>   data unsafe.Pointer
>
> }

**下一篇讲什么？**

到目前为止，我们也只分析了 Go 编译器与 Go 运行时的一小部分代码。还有大量的有意思的内容等待我们去探索，比如目标文件、链接器、重定位等。在接下来的博客中我会来依次分析这些内容。

------

在这个信息爆炸的时代，人们已然被大量、快速并且简短的信息所包围。然而，我们相信：过多“快餐”式的阅读只会令人“虚胖”，缺乏实质的内涵。伯乐在线博客团队正试图以我们微薄的力量，把优秀的原创/译文分享给读者，为“快餐”添加一些“营养”元素。

微信号: **jobbole**

来源： <<http://mp.weixin.qq.com/s?__biz=MjM5MzA0OTkwMA==&mid=212112037&idx=1&sn=df8719ac157e674cc3e5042b00a3db24&scene=0&key=dffc561732c226517bccb210cdf93b06969bb10f822bd693562dd69bf4a0d11745da225ace1cf5f57293219a0faa90de&ascene=1&uin=MjYyMTYyNTk0MA%3D%3D&devicetype=Windows+7&version=6102002a&pass_ticket=cqaNEWlb8lDca0RlJ0R2aDeGDdJiGNeoBBmoxT24FTd1LJHANKwuQ9%2F%2F%2FmWP3sVN>>

 



