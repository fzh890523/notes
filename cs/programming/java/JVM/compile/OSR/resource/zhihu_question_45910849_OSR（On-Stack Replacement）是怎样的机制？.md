# question



OSR（On-Stack Replacement）是怎样的机制？

关于OSR，我差不多能明白，这是一种运行时替换栈帧的技术。但我在看V8的代码的时候，还是会感觉一头雾水。求指教。
我现在大概知道的是这样的：
\1. Hydrogen IR中会创建OsrEntry。
\2. profiler, builtins, runtime中与OnStackRelacement相关的代码。单独看好像是看明白了，但是想串成一整块，好像又与自己理解的不同。
\3. 与execution还有一些关系?
希望高手解答。



# answer：R大



On-Stack Replacement (OSR) 是一种对提升benchmark跑分非常有效的技术，而对普通的结构良好的程序…（咳咳

OSR是一种在运行时替换正在运行的函数/方法的栈帧的技术。但它是手段，不是目的——是出于某种目的需要在运行时替换栈帧。
使用OSR最常见的目的就是在一个函数/方法的执行过程中，在执行引擎的不同优化层级之间切换，可以是从低优化层级向高优化层级切换，也可以反过来。这也就隐含了一个假设——这个执行引擎有多个层级的优化，可能是

* 一个解释器与一个JIT编译器的结合，例如以前老的HotSpot JVM，或者比较早期的Chakra，或者加入了Ignition解释器之后的V8；
* 一个无优化JIT编译器与一个优化JIT编译器的结合，例如JRockit JVM、Crankshaft时代的V8；
* 或者甚至一个解释器与多个JIT编译器的结合，例如现在的HotSpot JVM、现在的SpiderMonkey、JavaScriptCore；或者一个解释器与支持多种不同优化层级的同一个JIT编译器，例如IBM J9 JVM、现在的Chakra等；
* 还可以跟AOT结合，例如一个只做（相对比较）少量优化的AOT编译器，跟一个更优化的JIT编译器结合使用，例如计划于Oracle JDK9的某个更新版推出的AOT编译器，或者IBM J9 JVM。

在多层优化的执行引擎中，OSR可以为两种目的服务：

* 从低优化向高优化迁移：为了平衡启动性能（启动速度快，所以要初始开销小的执行模式）和顶峰性能（顶峰速度快，所以要更优化的执行模式，即便优化需要较大开销）
* 从高优化向低优化迁移：这可以细分为许多情况，例如：
  * 在**高优化层级做了很激进的优化**（例如假设某个类不会有别的子类、某个引用一定不是null、某个引用所指向的对象一定是某个具体类型，等），而这个激进的假设假如失效了的话，就必须退回到没有做这些优化的“安全”的低优化层级去继续执行。这个非常重要，有了这种OSR（deoptimize）机制的支持，JIT编译器就可以对代码做非常激进的优化，性能受正确性要求的约束会得到放松，因而对常见代码模式可以生成更快的代码。
  * 高优化层级不便于**对代码做调试**，如果某个方法之前已经被JIT优化编译了，而后来有调试器动态决定调试该方法，则让它从高优化层级退回到便于调试的低优化层级（例如解释器或者无优化的JIT编译版本的代码）去执行。

OSR还有若干其它叫法。
IBM曾经在J9 JVM中把OSR叫做“dynamic loop transfer”（DLT）。这个纯属傲娇，DLT说的就是在循环中做从低优化到高优化层级的OSR。
而HotSpot VM（以及同门师弟V8）也有自己的傲娇：它把从低优化层级向高优化层级的迁移叫做OSR，而把高优化层级向低优化层级的迁移叫做“去优化”（deoptimize），也叫做“uncommon trap”。其实deoptimize也是OSR的一种情况，能理解这个就够了。
Chakra中，deoptimization的对应物叫做“bailout”，同一个东西的不同叫法而已。

正好题主问的另外一个跟V8相关的问题也跟OSR（deoptimize）有关，放个传送门：[HSimulate这条Hydrogen里的instruction到底是什么意思？ - RednaxelaFX 的回答](https://www.zhihu.com/question/44153482/answer/96977278)

把OSR机制推广开来，所谓generalized on-stacking replacement，其实就是trace-based compilation中会用到的一种基础技术。Trace编译后，进入trace和离开trace其实都要做OSR。
在trace-based compilation的上下文中，离开trace的OSR的某些情况会叫做“side-exit”，其实就跟上面提到的“deoptimize”是一个道理。

tl;dr：**“执行引擎有多个不同优化程度的层级，一个函数正在执行的过程中可以在不同优化层级之间迁移”就是OSR的重点**。
知道这个之后，下面都是废话。不怕我啰嗦的话请继续看下去…===========================================

OSR是在1980年代末1990年代初的Self VM就发展成熟的技术，在JVM上得到了广泛应用，现在各大JavaScript引擎上也普遍采用了该技术。
有好几篇Self VM的论文重点提到了OSR的思路：

* [Debugging Optimized Code with Dynamic Deoptimization](https://link.zhihu.com/?target=http%3A//www.cs.ucsb.edu/%7Eurs/oocsb/papers/pldi92.pdf), Urs Hölzle, Craig Chambers, David Ungar, PLDI'92
* [Reconciling Responsiveness with Performance in Pure Object-Oriented Languages](https://link.zhihu.com/?target=https%3A//www.cs.ucsb.edu/%7Eurs/oocsb/papers/toplas96.pdf), Urs Hölzle, David Ungar

要追根溯源的话请务必细读这些Self VM的论文。

V8 是 Self VM -> Strongtalk VM 血缘下的嫡系，跟 HotSpot JVM 是同门兄弟，而V8 Crankshaft中的优化JIT编译器更是从HotSpot VM的Client Compiler（C1）移植而来（美其名曰“深受影响”），V8 Crankshaft中的OSR机制与HotSpot VM的OSR有非常紧密的联系。

所以下面先用Java来举例，然后再回到JavaScript（V8）的情况。

试想一个microbenchmark，我们想测试Java的Math.sin()方法的速度，要怎么测？
入门级程序员（或者从C/C++之类的通常不动态编译的语言转到Java的程序员）会这样写：

```java
public class BadMicrobenchmark {
  public static void main(String[] args) {
    long startTime = System.nanoTime();
    for (int i = 0; i < 10000000; i++) {
      Math.sin(i);
    }
    long endTime = System.nanoTime();
    System.out.println("duration (ns): " + (endTime - startTime));
  }
}
```


*在主流环境中*，JVM拿到手的是含有Java字节码的Class文件，并不能直接在硬件上执行，而需要JVM要么解释执行之，要么做JIT编译后执行。为了平衡启动性能与顶峰性能的需求，现代主流JVM都引入了多层编译机制，在刚开始的时候采用比较低优化的层级来执行，等某块代码热了之后再使用较高优化的层级来执行。
这大背景请参考下面俩传送门：

* [HotSpot是较新的Java虚拟机技术，用来代替JIT技术,那么HotSpot和JIT是共存的吗？ - RednaxelaFX 的回答](https://www.zhihu.com/question/26913901/answer/35303563)
* [JIT编译，动态编译与自适应动态编译 - 编程语言与高级语言虚拟机杂谈（仮） - 知乎专栏](https://zhuanlan.zhihu.com/p/19977592)

但是问题就来了：这些主流JVM通常是以“方法”（或者笼统的说，“函数”）为单位来JIT编译的。一个方法新JIT编译后的版本，在编译好之后，要等到下一次该方法被调用时才能用上，而无法顾及当前正在执行的方法的情况。
以上面的microbenchmark例子看，这个main()方法在整个程序的执行过程中只会被调用一次，就算JVM知道它很热而把它给JIT编译了，也没有机会等到它第二次被调用的时候跳进JIT编译的版本里。这样跑benchmark不就废了么？

于是OSR机制就来救场了！与其编译整个方法，我们可以在发现某个方法里有循环很热的时候，选择只编译方法里的某个循环，或者是从某个循环开始的代码。编译好之后，执行引擎便在仍在执行该方法的情况下，从原本的层级跳转到这个新JIT编译好的版本的代码去。
还是以上面的代码为例，我们可以只编译循环的这部分（情况A）：

```java
    startTime = ???; // not used in this compilation, but used by deoptimization
    i = ???;
    for ( ; i < 10000000; i++) {
      Math.sin(i);
    }
    Runtime.deoptimize(startTime, i); // tail call: deoptimize and go back to interpreter
```


或者只编译从这个循环开始的部分（情况B）：

```java
    startTime = ???;
    i = ???;
    for ( ; i < 10000000; i++) {
      Math.sin(i);
    }
    long endTime = System.nanoTime();
    System.out.println("duration (ns): " + (endTime - startTime));
    return; // end of method, normal return
```

注意：我们要发现一个循环很热，肯定是已经执行了该循环很多次了，并且在触发该循环的JIT编译时该循环还没执行完。
所以我们JIT编译的并不是完整的该循环（用上例说就是 i = 0 开始），而是该循环可能已经执行了很多次之后再进入的该循环（所以上面就用 i = ??? 表示）。

于是我们就需要从原本该方法所执行的层级的栈帧中，把需要的状态找出来，然后迁移到新的优化层级的栈帧去。在上面的情况B中，我们显然需要从原本的栈帧中找出局部变量 startTime 与 i 的值，并将其迁移到新编译的版本的代码的栈帧去。
假如我们通过一个叫做“OSR buffer”的地方来从低优化层级向高优化层级传递值，那么上面情况B所编译的代码的样子就会是这样的：（伪代码）

```java
  public static void main$osr_at_bci$21(OsrBuffer osrbuf) {
    // OSR entry
    startTime = osrbuf.startTime; // osrbuf.slots[1]
    i         = osrbuf.i;         // osrbuf.slots[3]
    // actual body
    for ( ; i < 10000000; i++) {
      Math.sin(i);
    }
    long endTime = System.nanoTime();
    System.out.println("duration (ns): " + (endTime - startTime));
    return; // end of method, normal return
  }
```

假如我们是在“解释器+JIT编译器”的配置下实现情况B的这种OSR编译，那么实际执行的时候，就可能有这样的时间轴：

* main()方法开始被解释器执行。解释器每当遇到从前往后的跳转（例如从循环末尾跳回到循环开头）时就让VM内跟该方法相关联的“循环计数器”加一；
* 执行到 i = 5000 的时候，循环计数器达到了触发“OSR编译”的阈值，回跳的这条字节码指令的位置上异步触发JIT编译器在后台进行情况B的编译；与此同时解释器继续执行main()方法；
* 执行到 i = 5100 的时候，JIT编译器对main()的OSR编译完成，解释器在回跳的这跳字节码指令的位置上把自己的栈帧状态打包成一个OsrBuffer，调用OSR版代码并传入OsrBuffer，也就是上面例子中的main$osr_at_bci$21()；
* main()的剩余部分就在OSR编译后的代码里执行到底。

而情况A版的代码，末尾有个奇怪的东西：Runtime.deoptimize()这个伪代码。这是干嘛的呢？
情况A中，我们只编译了当前正在执行的这个循环，循环前和循环后的代码都没有编译。那么如果跳到该版本的代码去跑，跑到循环结束后怎么办？简单，回到低优化层级（例如解释器）去执行就好了嘛。
所以这个Runtime.deoptimize()所代表的意思就是，把低优化层级的执行所需要的状态打包起来传递下去，然后回到低优化层级去继续执行。例子说到这里想必题主已经获得足够信息来把V8 Crankshaft的OSR机制串起来了。码字太累，先写到这里…
***（待续）***

后面还可以写几个方面：

* OSR在不同层级之间迁移的协议是怎样的？有了协议后，实际是如何“替换”栈帧的？
* V8 Crankshaft的实现特点

Andy Wingo大大以前写过一篇博文讲解当时V8 Crankshaft的OSR机制。等不及我码字的同学请跳传送门：[on-stack replacement in v8 -- wingolog](https://link.zhihu.com/?target=https%3A//wingolog.org/archives/2011/06/20/on-stack-replacement-in-v8)

















