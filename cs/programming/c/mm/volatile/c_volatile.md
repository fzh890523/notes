

# intro



from [Volatile从入门到放弃](http://blog.csdn.net/w329636271/article/details/54616543)

c/c++volatile的第一特性：易变性。

c/c++的volatile的第二特性：不可优化性。即不要对volatile变量进行各种激进的优化，保持变量原有的语义,不能被优化掉。

c/c++的volatile的第三特性：顺序性。基于前面的两个特性，让Volatile经常被解读为一个为多线程而生的关键词。但更要命的是很多程序员往往会忽略掉顺序性，这使得c/c++ volatile很难被正确使用的重要原因。同时，**c/c+的volatile是不能完全保证顺序性的**。

> 怎么个不完全法呢？
>
> From [C/C++ Volatile关键词深度剖析](http://hedengcheng.com/?p=725)
>
> 1. C/C++ Volatile变量，与非Volatile变量之间的操作，是可能被编译器交换顺序的
>
> 2. C/C++ Volatile变量间的操作，是不会被编译器交换顺序的
>
>    > 从1、2来看，跟java的老MM一样，或者换句话说，java的老MM“沿袭”了这一缺陷
>
> 3. 哪怕将所有的变量全部都声明为volatile，哪怕杜绝了编译器的乱序优化，但是针对生成的汇编代码，CPU有可能仍旧会乱序执行指令，导致程序依赖的逻辑出错，volatile对此无能为力



# 问题



## 缓存协调问题



### 描述



```
雅神，请教个低级问题啊。

看到c里对volatile的描述，应该是确保写回内存地址。 一般编译时要求从寄存器写回内存地址。 但考虑到CPU cache，单一个mov并不能保证这点。 而我在编译出的汇编里又没看到其他“指示性”的操作，比如清掉这条cache line等等。
```





```c
#include <unistd.h>

int volatile a;

void main()
{
    for(int i=0; i<100; i++)
    {
	sleep(1);
        a += i;
    }
}
```



```assembly
        .file   "t.c"
        .comm   a,4,4
        .text
.globl main
        .type   main, @function
main:
.LFB0:
        .cfi_startproc
        pushq   %rbp
        .cfi_def_cfa_offset 16
        .cfi_offset 6, -16
        movq    %rsp, %rbp
        .cfi_def_cfa_register 6
        subq    $16, %rsp
        movl    $0, -4(%rbp)
        jmp     .L2
.L3:
        movl    $1, %edi
        call    sleep
        movl    a(%rip), %eax
        addl    -4(%rbp), %eax
        movl    %eax, a(%rip)
        addl    $1, -4(%rbp)
.L2:
        cmpl    $99, -4(%rbp)
        jle     .L3
        leave
        .cfi_def_cfa 7, 8
        ret
        .cfi_endproc
.LFE0:
        .size   main, .-main
        .ident  "GCC: (GNU) 4.4.7 20120313 (Red Hat 4.4.7-18)"
        .section        .note.GNU-stack,"",@progbits
```



### 讨论

```
抱歉昨天实在太悲伤了，没看到DD
C的volatile只承诺读写产生副作用，并不承诺缓存一致（实际上，C标准是体系无关的，也没有规定这里的内存模型）
JY
特别关注
因为实际上，volatile的用途未必限于主存访问
举例来说，一个volatile object可能是硬件映射的只读寄存器，这时候根本不存在写回和强迫缓存失效这样的语义；C把volatile的实施细节留给了实现，并且不承诺任何并发性质（原子和可见）；对实现来说，volatile能做的常见努力其实是寄存器写回主存，而不是主存在SMP核心间保持一致
09:50
XG
这块我不太了解，map到地址空间的硬件端口不会有CPU cache对吧。volatile好像也主要是用在这个场景。
但这样的话volatile的语义就好弱啊，之前只知道C volatile只保证“可见”而没有“happens-before”语义... 现在来说，连“可见”都保证不了了...
已读
JY
特别关注
C的内存模型本来就是一团乱麻
JY
特别关注
如果需要保证写回的话，至少加fence或者开__atomic_add_fetch(&a, i, __ATOMIC_RELAXED)，不过这两种都需要gcc扩展
XG
嗯
已读
XG
对了，我对mem-bar理解的不太清楚，有相关的文档/书可以推荐一下吗？
比如用full-fence来“禁止” CPU 乱序的做法，“内存屏障”为什么能用来影响”CPU"行为，等等。
已读
XG
我的猜想是，CPU实现里有特殊“约定”指令，预取多个指令时会保证该指令之前和之后的指令的相对顺序。
才只是猜想。
已读
JY
特别关注
fence往往不是特别的指令
JY
特别关注
单纯的fence很可能是副作用，比如一个无意义的、带lock前缀的指令，要求无效化缓存
XG
CPU实现部分我感觉很难，看csapp的时候就跳过去那章了。 所以不太清楚CPU的“指令并发”机制。
已读
JY
特别关注
这时候如果多个核心都在访问同一位置，单纯靠缓存一致性协议已经不足以解决问题，应该会出一个总线锁信号，强迫串行
XG
嗯，但无效化缓存为什么能起到禁止CPU乱序的效果呢？
已读
XG
这里的乱序应该是单core啊。
已读
JY
特别关注
单核乱序是保证结果正确的
JY
特别关注
但如果指令的副作用导致锁总线，甚至排空流水，那么两方面都可以确保
XG
其实我还猜想过，CPU在“指令并发”时，会保证对同一地址的读写的操作的相对顺序。 当然，依然只是猜想。
已读
XG
但禁止重排是另一个场景。
已读
JY
特别关注
CPU乱序算法里面有对数据依赖的基本保证
JY
特别关注
但无依赖的重排需要两个粒度的确保：其一，编译器吐出目标代码的时候，可能需要优化障壁防止后端手工重排；其二就是memory barriers
JY
特别关注
btw x86其实真的有fence指令
XG
对对。 我就是这个意思。
我之前看mm/volatile的文章后的理解就是，乱序（最终指令和书写顺序）不一致可能会有编译重排和CPU乱序两个阶段（vm语言就需要vm一层层保证语义了，先不管）。 

我找个说一下。
已读
XG

1
  public void writer() {
2
    x = 42;
3
    v = true;
4
  }
5
  
6
  public void reader() {
7
    if (v == true) {
8
      //uses x - guaranteed to see 42.
9
    }
10
  }
已读
XG
随便找了个java的代码，就当是C的吧。
已读
XG
如果希望程序行为跟预期一样的话，要禁止 x=42 和 v=true这两个操作的乱序。
而因为不是同一个地址，CPU本身默认肯定不管的。

c里边，如果要保证这点，需要加入什么呢？ 
已读
XG
java里：
对于老MM，需要x和v都加volatile；
对于新MM，只需要v加volatile。
已读
JY
特别关注
#3 __atomic_store_n(&v, TRUE, __ATOMIC_RELEASE);

#7 if (__atomic_load_n(&v, __ATOMIC_ACQUIRE)) {
JY
特别关注
新jmm的v相当于自带acquire-release障壁
XG
对，__atomic_store_n/__atomic_load_n 最后实现上不是“特殊”CPU指令吗？
已读
JY
特别关注
很可能出lock前缀
XG
那么问题又回去了。 “锁总线” 这个操作（至少从名字上）好像是跟禁止乱序无关啊。
已读
XG
感觉上只是SMP下多核同步时用。
已读
JY
特别关注
简单地说，其实它类似一个事务，迫使主存操作可见
XG
即使 lock mov ... 会强制写回内存。
但不只是可见性的问题呢。
已读
JY
特别关注
但CPU乱序是不必在意的，因为对单条指令来说，它或者执行了、或者执行完了
XG
    x = 42;
    v = true;
如果CPU乱序的结果不是这个，而是
    v = true;
    x = 42;

那么...
已读
JY
特别关注
CPU不会搞出这个结果的
JY
特别关注
除非编译器搞出了这样的东西
XG
why？ 我认为默认cpu不会保证这个，不然CPU 指令并发几乎没空间了。
已读
JY
特别关注
这里应该是个优化障壁
JY
特别关注
x = 42和v = true这两个东西的求值序由编译器按需保障
编译器可以在目标代码里穿插求值，或者任意地安排顺序
但假如编译器认定x = 42 happens before v = true, 那么目标代码上x的写回一定早于v，无论之间做的穿插是什么样的
JY
特别关注
最终的目标代码只承诺“若看见v = true写回，则必然能看见x = 42写回”
JY
特别关注
为了实现这一承诺，就需要进一步地利用CPU的能力限制乱序
XG
我的意思是编译器只能保证生成的二进制代码（指令）是按序的； 但不能保证CPU的执行是按序的 --- 这里就回到： 默认情况下，CPU会不会对连续两个内存写操作乱序处理了。
如果前面一行成立的话，问题就来了： 要如何告诉CPU不做这个乱序，保证两个指令先后执行。
已读
XG
漏了，是 连续两个不同地址内存写操作 。
已读
JY
特别关注
以intel为例，lock实际上是有一个栅栏的
JY
特别关注
表面上的文档指出，读写操作不能越过lock, 但我对硬件协议不熟，不确认这个表述里是否有坑
XG
开会啊前后继续
已读
10:44
XG
如果“读写操作不能越过lock” lock确实有这个语义的话，那确实解释了我的疑惑。
从哪看到的啊？ 
已读
JY
特别关注
intel手册里应该有类似的描述，不过我现在手头没有那套砖……
XG
我去找找。 好像有电子版。
已读
JY
特别关注
btw 特指intel的话，实际上还有一个更严厉的限制：强内存模型仅仅不保证Store-Load顺序
XG
噢。 那也是... 

list a lot of memory ordering guarantees, among them:

Loads are not reordered with other loads.
Stores are not reordered with other stores.
Stores are not reordered with older loads.
In a multiprocessor system, memory ordering obeys causality (memory ordering respects transitive visibility).
In a multiprocessor system, stores to the same location have a total order.
In a multiprocessor system, locked instructions have a total order.
Loads and stores are not reordered with locked instructions.

没去细看过这些，如果这里指的是普通load/store而非限定于同一内存地址的话，那确实是CPU就提供了比较强的MM。
已读
JY
特别关注
同一地址上的RAW/WAR/WAW已经产生数据依赖了，必须保证行为在结果上串行化
XG
嗯，同一地址的这个我也想到，应该是底线 。
已读
XG
《Intel 64 Architecture Memory Ordering White Paper》

感觉这篇里估计会解答我的疑惑。 
已读
JY
特别关注
"Intel 64 memory ordering ensures that all processors agree on a single execution order of all locked instructions, including those that access different locations."
我也是头一次看到这么狠的说法……
XG
但只是这些locked指令之间吧，中间穿插的non-locked指令就不管了我猜
已读
JY
特别关注
2.7节给出的例子指出了在有依赖的情况下，lock波及其他核非lock的情况
JY
特别关注
xchg是自带lock的
XG
嗯嗯，我看看先。
野路子知识碎片化真是严重
已读
JY
特别关注
CPU这方面我也是野路子出来的，实际痛苦
```



### 结论

* **C volatile甚至不保证可见性**，原先确实误解了
* 所以C volatile基本只应该用于硬件相关（无CPU cache）
* CPU乱序保证要看CPU手册，如`lock prefix`是否有额外的语义等等







