# Golang的演化历程

- 十月 25, 2014
- [暂无评论](http://tonybai.com/2014/10/25/golang-history/#respond)

本文来自Google的[Golang](http://golang.org/)语言设计者之一[Rob Pike](http://en.wikipedia.org/wiki/Rob_Pike)大神在GopherCon2014大会上的开幕主题演讲资料“[Hello, Gophers](http://talks.golang.org/2014/hellogophers.slide)!”。Rob大神在这次分 享中用了两个生动的例子讲述了Golang的演化历程，总结了Golang到目前为止的成功因素，值得广大Golang Programmer & Beginner学习和了解。这里也用了"Golang的演化历程"作为标题。

**1、Hello Gophers!**

package main

import "fmt"

func main() {
​    fmt.Printf("Hello, gophers!\n")
}

Rob大神的见面礼，后续会有针对这段的演化历史的陈述。

**2、历史**

这是一个历史性的时刻。

Golang已经获得了一定的成功，值得拥有属于自己的技术大会。

**3、成功**

促成这份成功的因素有许多：

​    – 功能
​    – 缺少的功能
​    – 功能的组合
​    – 设计   
​    – 人
​    – 时间

**4、案例学习：两段程序**

我们来近距离回顾两段程序。

第一个是你见过的第一个Go程序，是属于你的历史时刻。
第二个是我们见过的第一个Go程序，是属于全世界所有Gophers的历史时刻。

先看第一个“hello, world”

**5、hello.b**

main( ) {
​    extrn a, b, c;
​    putchar(a); putchar(b); putchar(c); putchar('!*n');
}
a 'hell';
b 'o, w';
c 'orld';

上面这段代码首先出现在1972年[Brian W. Kernighan](http://en.wikipedia.org/wiki/Brian_Kernighan)的B语言教程中（也有另外一说是出现在那之前的BCPL语言中）。

**6、hello.c**

main()
{
​    printf("hello, world");
}

上面这段代码出现在1974年Brian W. Kernighan编写的《Programming in C: A Tutorial》中。这份教程当时是作为Unix v5文档的一部分。

**7、hello.c**

main()
{
​    printf("hello, world\n"); //译注：与上面的hello.c相比，多了个换行符\n输出
}

这段代码首次出现在1978年Brian W. Kernighan和[Dennis M. Ritchie](http://en.wikipedia.org/wiki/Dennis_Ritchie)合著的《[The C Programming Language](http://book.douban.com/subject/4816029/)》一书中。

**8、hello.c, 标准C草案**

\#include <stdio.h> //译注：与上面hello.c相比， 多了这个头文件包含

main()
{
​    printf("hello, world\n");
}

这段代码出现在1988年Brian W. Kernighan和Dennis M. Ritchie合著的《[The C Programming Language](http://book.douban.com/subject/1236999/)》第二版一书中，基于标准C草案。

9、hello.c，标准C89

\#include <stdio.h>

main(void) //译注：与上面hello.c相比，多了个void
{
​    printf("hello, world\n");
}

这段代码出现在1988年Brian W. Kernighan和Dennis M. Ritchie合著的《The C Programming Language》第二版第二次修订中。

**10、一两代之后…**

(省略所有中间语言)

关于Golang的讨论开始于2007年年末。

第一版语言规范起草于2008年3月份。

用于实验和原型目的的编译器开发工作已经展开。

最初的编译器输出的是C代码。

语言规范一形成，我们就重写了编译器，输出本地代码（机器码）。

**11、hello.go, 2008年6月6日**

package main

func main() int {
​    print "hello, world\n";
​    return 0;
}

针对首次提交代码的测试。

内置的print已经是当时的全部实现。main函数返回一个int类型值。
注意：print后面没有括号。

**12、hello.go，2008年6月27日**

package main

func main() {
​    print "hello, world\n";
}

当main函数返回，程序调用exit(0)。

**13、hello.go，2008年8月11日**

package main

func main() {
​    print("hello, world\n");
}

print调用加上了括号，这时print是一个函数，不再是一个原语。

**14、hello.go，2008年10月24日**

package main

import "fmt"

func main() {
​    fmt.printf("hello, world\n");
}

我们熟知并喜欢的printf来了。

**15、hello.go，2009年1月15日**

package main

import "fmt"

func main() {
​    fmt.Printf("hello, world\n");
}

头母大写的函数名用作才是导出的符号。

16、hello.go, 2009年12约11日

package main

import "fmt"

func main() {
​    fmt.Printf("hello, world\n")
}

不再需要分号。

这是在2009年11月10日Golang开发发布后的一次重要改变。

这也是当前版本的hello, world

我们花了些时间到达这里（32年！）

都是历史了！

**17、不仅仅有C**

我们从"C"开始，但Go与C相比有着巨大的不同。

其他一些语言影响和贯穿于Go的设计当中。

​    C: 语句和表达式语法
​    Pascal: 声明语法
​    Modula 2, Oberon 2：包
​    CSP, Occam, Newsqueak, Limbo, Alef:  并发
​    BCPL: 分号规则
​    Smalltalk: 方法(method)
​    Newsqueak: <-, :=
​    APL: iota
等等。也有一些是全新发明的，例如defer、常量。

还有一些来自其他语言的优点和缺点：
​    C++, C#, Java, JavaScript, LISP, Python, Scala, …

**18、hello.go，Go 1版**

将我们带到了今天。

package main

import "fmt"

func main() {
​    fmt.Println("Hello, Gophers (some of whom know 中文)!")
}

我们来深入挖掘一下，把这段代码做一个拆解。

**19、Hello, World的16个tokens**

package
main
import
"fmt"
func
main
(
)
{
fmt
.
Println
(
"Hello, Gophers (some of whom know 中文)!"
)
}

**20、package**

早期设计讨论的主要话题：扩展性的关键

package是什么？来自Modula-2等语言的idea

为什么是package?

​    – 拥有编译构建所需的全部信息
​    – 没有循环依赖(import)
​    – 没有子包
​    – 包名与包路径分离
​    – 包级别可见性，而不是类型级别
​    – 在包内部，你拥有整个语言，在包外部，你只拥有包许可的东西。

**21、main**

一个C语言遗留风范尽显之处
最初是Main，原因记不得了。
主要的包，main函数
很特别，因为它是初始化树(initialization tree)的根(root)。

**22、import**

一种加载包的机制
通过编译器实现（有别于文本预处理器。译注：C语言的include是通过preprocessor实现的）
努力使其高效且线性
导入的一个包，而不是一个标识符(identifiers)集合（译注：C语言的include是将头文件里的标识符集合引入）
至于export，它曾经是一个关键字。

**23、"fmt"**

包路径(package path)只是一个字符串，并非标识符的列表。
让语言避免定义它的含义 – 适应性。(Allows the language to avoid defining what it means—adaptability)
从一开始就想要一个URL作为一个选项。（译注：类似import "github.com/go/tools/xxx）
可以应付将来的发展。

**24、func**

一个关键字，用于引入函数(类型、变量、常量），易于编译器解析。
对于函数字面量(闭包)而言，易于解析非常重要。
顺便说一下，最初这个关键字不是func，而是function。

小插曲：

Mail thread from February 6, 2008
From: Ken Thompson [](mailto:ken@google.com)  
To: gri, r
larry and sergey came by tonight. we  talked about go for more than an hour.  they both said they liked it very much.
p.s. one of larrys comments was "why isnt function spelled func?"
—
From: Rob Pike [](mailto:r@google.com)
 To: ken, gri
fine with me. seems compatible with 'var'.
anyway we can always say, "larry said to call it 'func'"

**25、main**

程序执行的起点。除非它不是。（译注：main不是起点，rob大神的意思是不是指下列情形，比如go test测试包，在google app engine上的go程序不需要main）
将初始化与正常执行分离，早期计划之中的。
初始化在哪里发生的？(译注：说的是package内的func init() {..}函数吧)
回到包设计。（译注：重温golang的package设计思想）

**26、()**

看看，没有void
main没有返回值，由运行时来处理main的返回后的事情。
没有函数参数（命令行选项通过os包获取）
没有返回值

返回值以及语法

**27、{**

用的是大括号，而不是空格（译注：估计是与python的空格缩进对比）
同样也不是方括号。
为什么在括号后放置换行符(newline)？

**28、****fmt**

所有导入的标识符均限定于其导入的包。（All imported identifiers are qualified by their import.）
每个标识符要么是包或函数的本地变量，要么被类型或导入包限定。
对代码可读性的重大影响。

为什么是fmt，而不是format？

**29、.**

句号token在Go中有多少使用？（很多）
a.B的含义需要使用到类型系统
但这对于人类来说非常清晰，读起来也非常容易。
针对指针的自动转换(没有->)。

**30、Println**

Println，不是println，头母大写才是导出符号。
知道它是反射驱动的(reflection-driven)
可变参数函数
参数类型是(…); 2010年2月1日变成(…interface{})

**31、(**

传统函数语法

**32、****"Hello, Gophers (some of whom know 中文)!"**

UTF-8编码的源码输入。字符串字面量也自动是utf8编码格式的。

但什么是字符串(string)呢？

首批写入规范的语法规则，今天很难改变了。(blog.golang.org/strings)

**33、)**

没有分号
在go发布后不久我们就去除了分号
早期曾胡闹地尝试将它们（译注：指得是括号）去掉
最终接受了BCPL的方案

**34、}**

第一轮结束。

旁白：还没有讨论到的

​    – 类型
​    – 常量
​    – 方法
​    – interface
​    – 库
​    – 内存管理
​    – 并发（接下来将讨论）
外加工具，生态系统，社区等。
语言是核心，但也只是我们故事的一部分。

**35、成功**

要素：
​    – 站在巨人的肩膀上(building on history)
​    – 经验之作(building on experience) 译注：最初的三个神级语言设计者
​    – 设计过程
​    – 早期idea提炼到最终的方案中
​    – 由一个小团队专门集中精力做
最终：承诺
​    Go 1.0锁定了语言核心与标准库。

**36、另一轮**

让我们看第二个程序的类似演化过程。

**37、问题：素数筛(Prime sieve)**

问题来自于Communicating Sequential Processes, by C. A. R. Hoare, 1978。

“问题：以升序打印所有小于10000的素数。使用一个process数组：SIEVE，其中每个process从其前驱元素输入一个素数并打印它。接下 来这个process从其前驱元素接收到一个升序数字流并将它们传给其后继元素，这个过程会剔除掉所有是最初素数整数倍的数字。

**38、解决方案**

在1978年的CSP论文中。（注意不是Eratosthenes筛）

这个优美的方案是由David Gries贡献出来的。

**39、CSP**

在Hoare的CSP论文中：

[SIEVE(i:1..100)::
​    p,mp:integer;
​    SIEVE(i - 1)?p;
​    print!p;
​    mp := p; comment mp is a multiple of p;
*[m:integer; SIEVE(i - 1)?m →
​    *[m > mp → mp := mp + p];
​    [m = mp → skip
​    ||m < mp → SIEVE(i + 1)!m
]   ]
||SIEVE(0)::print!2; n:integer; n := 3;
​    *[n < 10000 → SIEVE(1)!n; n := n + 2]
||SIEVE(101)::*[n:integer;SIEVE(100)?n → print!n]
||print::*[(i:0..101) n:integer; SIEVE(i)?n → ...]
]

没有channel。能处理的素数的个数是在程序中指定的。

**40、Newsqueak**

circa 1988。

Rob Pike语言设计，Tom Cargill和Doug McIlroy实现。

使用了channels，这样个数是可编程的。(channel这个idea从何而来？）

counter:=prog(end: int, c: chan of int)
{
​    i:int;
​    for(i=2; i<end; i++)
​        c<-=i;
};

filter:=prog(prime: int, listen: chan of int, send: chan of int)
{
​    i:int;
​    for(;;)
​        if((i=<-listen)%prime)
​            send<-=i;
};

sieve:=prog(c: chan of int)
{
​    for(;;){
​        prime:=<-c;
​        print(prime, " ");
​        newc:=mk(chan of int);
​        begin filter(prime, c, newc);
​        c=newc;
​    }
};

count:=mk(chan of int);

begin counter(10000, count);
begin sieve(count);
"";

**41、sieve.go，2008年3月5日**

使用go规范编写的第一个版本，可能是第二个由go编写的重要程序。

\>用于发送；<用于接收。Channel是指针。Main是头字母大写的。

package Main

// Send the sequence 2, 3, 4, … to channel 'ch'.
func Generate(ch *chan> int) {
​    for i := 2; ; i++ {
​        >ch = i;    // Send 'i' to channel 'ch'.
​    }
}

// Copy the values from channel 'in' to channel 'out',
// removing those divisible by 'prime'.
func Filter(in *chan< int, out *chan> int, prime int) {
​    for ; ; {
​        i := <in;    // Receive value of new variable 'i' from 'in'.
​        if i % prime != 0 {
​            >out = i;    // Send 'i' to channel 'out'.
​        }
​    }
}

// The prime sieve: Daisy-chain Filter processes together.
func Sieve() {
​    ch := new(chan int);  // Create a new channel.
​    go Generate(ch);      // Start Generate() as a subprocess.
​    for ; ; {
​        prime := <ch;
​        printf("%d\n", prime);
​        ch1 := new(chan int);
​        go Filter(ch, ch1, prime);
​        ch = ch1;
​    }
}

func Main() {
​    Sieve();
}

**42. sieve.go，2008年7月22日**

-<用于发送；-<用于接收。Channel仍然是指针。但现在main不是大写字母开头的了。

package main

// Send the sequence 2, 3, 4, … to channel 'ch'.
func Generate(ch *chan-< int) {
​    for i := 2; ; i++ {
​        ch -< i    // Send 'i' to channel 'ch'.
​    }
}

// Copy the values from channel 'in' to channel 'out',
// removing those divisible by 'prime'.
func Filter(in *chan<- int, out *chan-< int, prime int) {
​    for {
​        i := <-in;    // Receive value of new variable 'i' from 'in'.
​        if i % prime != 0 {
​            out -< i    // Send 'i' to channel 'out'.
​        }
​    }
}

// The prime sieve: Daisy-chain Filter processes together.
func Sieve() {
​    ch := new(chan int);  // Create a new channel.
​    go Generate(ch);      // Start Generate() as a subprocess.
​    for {
​        prime := <-ch;
​        printf("%d\n",    prime);
​        ch1 := new(chan int);
​        go Filter(ch, ch1, prime);
​        ch = ch1
​    }
}

func main() {
​    Sieve()
}

**43、sieve.go，2008年9月17日**

通信操作符现在是<-。channel仍然是指针。

package main

// Send the sequence 2, 3, 4, … to channel 'ch'.
func Generate(ch *chan <- int) {
​    for i := 2; ; i++ {
​        ch <- i  // Send 'i' to channel 'ch'.
​    }
}

// Copy the values from channel 'in' to channel 'out',
// removing those divisible by 'prime'.
func Filter(in *chan <- int, out *<-chan int, prime int) {
​    for {
​        i := <-in;  // Receive value of new variable 'i' from 'in'.
​        if i % prime != 0 {
​            out <- i  // Send 'i' to channel 'out'.
​        }
​    }
}

// The prime sieve: Daisy-chain Filter processes together.
func Sieve() {
​    ch := new(chan int);  // Create a new channel.
​    go Generate(ch);      // Start Generate() as a subprocess.
​    for {
​        prime := <-ch;
​        print(prime, "\n");
​        ch1 := new(chan int);
​        go Filter(ch, ch1, prime);
​        ch = ch1
​    }
}

func main() {
​    Sieve()
}

**44、sieve.go，2009年1月6日**

引入了make内置操作符。没有指针。编码错误！（有个*被留下了，错误的参数类型）

package main

// Send the sequence 2, 3, 4, … to channel 'ch'.
func Generate(ch chan <- int) {
​    for i := 2; ; i++ {
​        ch <- i  // Send 'i' to channel 'ch'.
​    }
}

// Copy the values from channel 'in' to channel 'out',
// removing those divisible by 'prime'.
func Filter(in chan <- int, out *<-chan int, prime int) {
​    for {
​        i := <-in;  // Receive value of new variable 'i' from 'in'.
​        if i % prime != 0 {
​            out <- i  // Send 'i' to channel 'out'.
​        }
​    }
}

// The prime sieve: Daisy-chain Filter processes together.
func Sieve() {
​    ch := make(chan int);  // Create a new channel.
​    go Generate(ch);       // Start Generate() as a subprocess.
​    for {
​        prime := <-ch;
​        print(prime, "\n");
​        ch1 := make(chan int);
​        go Filter(ch, ch1, prime);
​        ch = ch1
​    }
}

func main() {
​    Sieve()
}

**45、sieve.go，2009年9月25日**

第一个正确的现代版本。同样，大写头母不见了，使用了fmt。

package main

import "fmt"

// Send the sequence 2, 3, 4, … to channel 'ch'.
func generate(ch chan<- int) {
​    for i := 2; ; i++ {
​        ch <- i;    // Send 'i' to channel 'ch'.
​    }
}

// Copy the values from channel 'in' to channel 'out',
// removing those divisible by 'prime'.
func filter(src <-chan int, dst chan<- int, prime int) {
​    for i := range src {    // Loop over values received from 'src'.
​        if i%prime != 0 {
​            dst <- i;    // Send 'i' to channel 'dst'.
​        }
​    }
}

// The prime sieve: Daisy-chain filter processes together.
func sieve() {
​    ch := make(chan int);  // Create a new channel.
​    go generate(ch);       // Start generate() as a subprocess.
​    for {
​        prime := <-ch;
​        fmt.Print(prime, "\n");
​        ch1 := make(chan int);
​        go filter(ch, ch1, prime);
​        ch = ch1;
​    }
}

func main() {
​    sieve();
}

**46、sieve.go，2009年12月10日**

分号不见了。程序已经与现在一致了。

package main

import "fmt"

// Send the sequence 2, 3, 4, … to channel 'ch'.
func generate(ch chan<- int) {
​    for i := 2; ; i++ {
​        ch <- i  // Send 'i' to channel 'ch'.
​    }
}

// Copy the values from channel 'src' to channel 'dst',
// removing those divisible by 'prime'.
func filter(src <-chan int, dst chan<- int, prime int) {
​    for i := range src {  // Loop over values received from 'src'.
​        if i%prime != 0 {
​            dst <- i  // Send 'i' to channel 'dst'.
​        }
​    }
}

// The prime sieve: Daisy-chain filter processes together.
func sieve() {
​    ch := make(chan int)  // Create a new channel.
​    go generate(ch)       // Start generate() as a subprocess.
​    for {
​        prime := <-ch
​        fmt.Print(prime, "\n")
​        ch1 := make(chan int)
​        go filter(ch, ch1, prime)
​        ch = ch1
​    }
}

func main() {
​    sieve()
}

这个优美的方案来自于几十年的设计过程。

**47、旁边，没有讨论到的**

select

真实并发程序的核心连接器（connector)
最初起源于Dijkstra的守卫命令(guarded command)
在Hoare的CSP理论实现真正并发。
经过Newsqueak、Alef、Limbo和其他语言改良后

2008年3月26日出现在Go版本中。

简单，澄清，语法方面的考虑。

**48、稳定性**

Sieve程序自从2009年末就再未改变过。– 稳定！

开源系统并不总是兼容和稳定的。

但，Go是。（兼容和稳定的）

这是Go成功的一个重要原因。

**49、****趋势**

图数据展示了Go 1.0发布后Go语言的爆发。

**50、成功**

Go成功的元素：

​    显然的：功能和工具。

​    * 并发
​    * 垃圾回收
​    * 高效的实现
​    * 给人以动态类型体验的静态类型系统
​    * 丰富但规模有限的标准库
​    * 工具化
​    * gofmt
​    * 在大规模系统中的应用

​    不那么显然的：过程

​    * 始终聚焦最初的目标
​    * 在冻结后的集中开发
​    * 小核心团队易于取得一致
​    * 社区的重要贡献
​    * 丰富的生态系统
总之，开源社区共享了我们的使命，聚焦于为当今的世界设计一门语言。

© 2014, [bigwhite](http://tonybai.com/). 版权所有.

来源： <<http://tonybai.com/2014/10/25/golang-history/>>

 