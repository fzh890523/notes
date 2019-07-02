# [Golang 程序调优](http://name5566.com/5345.html)

发布于 [2015 年 08 月 01 日2015 年 09 月 27 日](http://name5566.com/5345.html) 作者 [name5566](http://name5566.com/author/name5566)

本文为以下文章的读书笔记：
<http://blog.golang.org/profiling-go-programs>
<https://golang.org/pkg/runtime/pprof>

#### CPU Profiling

Golang 提供了 pprof 包（runtime/pprof）用于输出运行时的 profiling 数据，这些数据可以被 [pprof](http://code.google.com/p/google-perftools/)工具（或者 go tool pprof，其为 pprof 的变种）使用。通常我们这样来使用 pprof 包：

```
// 定义 flag cpuprofile
var cpuprofile = flag.String("cpuprofile", "", "write cpu profile to file")
 
func main() {
    flag.Parse()
 
    // 如果命令行设置了 cpuprofile
    if *cpuprofile != "" {
        // 根据命令行指定文件名创建 profile 文件
        f, err := os.Create(*cpuprofile)
        if err != nil {
            log.Fatal(err)
        }
        // 开启 CPU profiling
        pprof.StartCPUProfile(f)
        defer pprof.StopCPUProfile()
    }
    ...
```

假定我们编写的一个程序 mytest 中加入了上述代码则可以执行并生成 profile 文件：

```
./mytest -cpuprofile=mytest.prof
```

这里，我们生成了 mytest.prof profile 文件。有了 profile 文件就可以使用 go tool pprof 程序来解析此文件：

```
go tool pprof mytest mytest.prof
```

pprof 程序中最重要的命令就是 topN，此命令用于显示 profile 文件中的最靠前的 N 个样本（samples），例如（此例为 <http://blog.golang.org/profiling-go-programs> 中的例子）：

```
(pprof) top10
Total: 2525 samples
     298  11.8%  11.8%      345  13.7% runtime.mapaccess1_fast64
     268  10.6%  22.4%     2124  84.1% main.FindLoops
     251   9.9%  32.4%      451  17.9% scanblock
     178   7.0%  39.4%      351  13.9% hash_insert
     131   5.2%  44.6%      158   6.3% sweepspan
     119   4.7%  49.3%      350  13.9% main.DFS
      96   3.8%  53.1%       98   3.9% flushptrbuf
      95   3.8%  56.9%       95   3.8% runtime.aeshash64
      95   3.8%  60.6%      101   4.0% runtime.settype_flush
      88   3.5%  64.1%      988  39.1% runtime.mallocgc
```

开启 CPU profiling 后，Golang 程序在 1 秒钟会停顿 100 次，每次停顿都会记录 1 个样本。上例中，前两列表示运行的函数的样本数量（the number of samples in which the function was running）和占总样本数的百分比，例如说 runtime.mapaccess1_fast64 函数在 298 次采样中（占总采样数量的 11.8%）正在运行。第三列表示前几行样本数量总和占总样本数的百分比（第二行 22.4% 为 11.8% + 10.6%）。第四、五列表示出现的函数的样本数量（the number of samples in which the function appeared）和占总样本数的百分比，这里“出现的函数”指的是在采样中正在运行或者等待某个被调用函数返回的函数，换句话就是采样中那些位于调用栈上的函数。我们可以使用 -cum（cumulative 的缩写）flag 来以第四、五列为标准排序。需要注意的是，每次采样只会包括最底下的 100 个栈帧（stack frames）。

使用 web 命令能够以图形化的方式（SVG 格式）显示函数调用关系。例如（图片来源于<http://blog.golang.org/profiling-go-programs>）：
![profiling-go-programs_havlak1a-75](https://kshttps0.wiz.cn/ks/note/view/89077880-eff4-11e0-a402-00237def97cc/c78e7499-1940-446e-a53a-ca829e4f3b02/index_files/fa084dbc-5747-463b-88e2-09a2f9995660.png)
这里每个方块的大小由运行的函数的样本数量决定（这样就能方便的一眼看到热点函数）。箭头表示的是调用关系，箭头上的数字表示的是采样到的调用次数。web 命令还可以指定显示特定的函数，例如：

```
(pprof) web mapaccess1
```

当我们有大致的想法（也就是确定热点函数）后，就可以深入特定的函数。我们使用 list 命令（此例为<http://blog.golang.org/profiling-go-programs> 中的例子）：

```
(pprof) list DFS
Total: 2525 samples
ROUTINE ====================== main.DFS in /home/rsc/g/benchgraffiti/havlak/havlak1.go
   119    697 Total samples (flat / cumulative)
     3      3  240: func DFS(currentNode *BasicBlock, nodes []*UnionFindNode, number map[*BasicBlock]int, last []int, current int) int {
     1      1  241:     nodes[current].Init(currentNode, current)
     1     37  242:     number[currentNode] = current
     .      .  243:
     1      1  244:     lastid := current
    89     89  245:     for _, target := range currentNode.OutEdges {
     9    152  246:             if number[target] == unvisited {
     7    354  247:                     lastid = DFS(target, nodes, number, last, lastid+1)
     .      .  248:             }
     .      .  249:     }
     7     59  250:     last[number[currentNode]] = lastid
     1      1  251:     return lastid
```

上例中，第一列为运行到此行时的样本数，第二列为运行到此行或从此行调用的样本数，第三列为行号。如果需要显示汇编，可以使用命令 disasm（使用命令 weblist 可以同时显示源码和汇编代码，[这里](http://benchgraffiti.googlecode.com/hg/havlak/havlak1.html)有一个范例）。通过样本数，我们可以定位到热点行，然后考虑适合的优化策略。

#### pprof 包

pprof 包进行 profiling 有两种方式：

1. 采样。CPU Profiling 需要不断采样，（如上所述）pprof 包提供了一套特殊的 API（StartCPUProfile / StopCPUProfile）
2. 快照。下面详细谈这种方式（同样可以使用 go tool pprof 程序来解析输出的 profile 文件）

pprof 包预先定义了（还可以自己扩展）4 种快照模式：

1. goroutine，当前所有 goroutines 的 stack traces
2. heap，所有的堆内存分配（为降低开销仅获取一个近似值，To reduce overhead, the memory profiler only records information for approximately one block per half megabyte allocated (the “1-in-524288 sampling rate”), so these are approximations to the actual counts）
3. threadcreate，致使新系统线程创建的 stack traces
4. block，致使在同步原语上阻塞的 stack traces

相关 API 具体用法如下：

```
// 根据名字查找 Profile
p := pprof.Lookup("heap")
// 将一个 pprof（程序）格式的快照写入 
wp.WriteTo(w, 0)
```

这里的 WriteTo 方法原型为：

```
func (p *Profile) WriteTo(w io.Writer, debug int) error
```

其中 debug 参数：

1. 为 0 时，仅仅输出 pprof（程序）需要的十六进制地址
2. 为 1 时，输出时增加函数名和行号，这样无需工具也可以阅读此 profile
3. 为 2 时，并且当输出 goroutine profile 时，输出的 goroutine 栈的格式为未 recovered panic 时的格式

#### memory profiling

以 <https://blog.golang.org/profiling-go-programs> 中的例子为例：

```
// 定义 flag memprofile
var memprofile = flag.String("memprofile", "", "write memory profile to this file")
...
 
    // 需要 profiling 的函数
    FindHavlakLoops(cfgraph, lsgraph)
    if *memprofile != "" {
        f, err := os.Create(*memprofile)
        if err != nil {
            log.Fatal(err)
        }
        // WriteHeapProfile 等价于 Lookup("heap").WriteTo(w, 0)
        pprof.WriteHeapProfile(f)
        // 关闭文件
        f.Close()
        return
    }
```

使用 go tool pprof 程序打开生成的 profile 文件：

```
(pprof) top5
Total: 82.4 MB
    56.3  68.4%  68.4%     56.3  68.4% main.FindLoops
    17.6  21.3%  89.7%     17.6  21.3% main.(*CFG).CreateNode
     8.0   9.7%  99.4%     25.6  31.0% main.NewBasicBlockEdge
     0.5   0.6% 100.0%      0.5   0.6% itab
     0.0   0.0% 100.0%      0.5   0.6% fmt.init
```

这里显示了函数当前大致分配的内存。类似 CPU profiling，通过 list 命令查看函数具体的内存分配情况：

```
(pprof) list FindLoops
Total: 82.4 MB
ROUTINE ====================== main.FindLoops in /home/rsc/g/benchgraffiti/havlak/havlak3.go
  56.3   56.3 Total MB (flat / cumulative)
...
   1.9    1.9  268:     nonBackPreds := make([]map[int]bool, size)
   5.8    5.8  269:     backPreds := make([][]int, size)
     .      .  270:
   1.9    1.9  271:     number := make([]int, size)
   1.9    1.9  272:     header := make([]int, size, size)
   1.9    1.9  273:     types := make([]int, size, size)
   1.9    1.9  274:     last := make([]int, size, size)
   1.9    1.9  275:     nodes := make([]*UnionFindNode, size, size)
     .      .  276:
     .      .  277:     for i := 0; i < size; i++ {
   9.5    9.5  278:             nodes[i] = new(UnionFindNode)
     .      .  279:     }
...
     .      .  286:     for i, bb := range cfgraph.Blocks {
     .      .  287:             number[bb.Name] = unvisited
  29.5   29.5  288:             nonBackPreds[i] = make(map[int]bool)
```

有了这些信息，我们就可以着手进行优化。

标签：[Computer Science](http://name5566.com/tag/computer-science), [Golang](http://name5566.com/tag/golang)

分类：未分类

来源： <<http://name5566.com/5345.html>>

 