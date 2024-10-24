



# ref



## [zhihu-final修饰递归方法会提高效率吗？-R大回答](https://www.zhihu.com/question/66083114/answer/242241071)



Java里用final修饰符去修饰一个方法的唯一正确用途就是表达：这个方法原本是一个虚方法，现在我通过final来声明这个方法不允许在派生类中进一步被覆写（override）。它跟递归方法、提高效率什么的都没有必然联系。

Java里，非私有的成员方法默认都是虚方法，而虚方法就可以在派生类中被覆写。为了保证某个类上的某个虚方法不在派生类中被进一步覆写，就需要使用final修饰符来声明，让编译器（例如javac）与JVM共同检查并保证这个限制总是成立。

曾经有一种广为流传的说法是用final修饰方法可以让对这个方法的调用变快。这种说法在现代主流的优化JVM上都是不成立的（例如Oracle JDK / OpenJDK HotSpot VM、IBM J9 VM、Azul Systems Zing VM等）。

这是因为：能用final修饰的虚方法，其派生类中必然不可能存在对其覆写的版本，于是可以判定这个虚方法只有一个可能的调用目标；而如果此时把这个final修饰符去掉，这些先进的JVM都可以通过“类层次分析”（Class Hierarchy Analysis，CHA）来发现这个方法在派生类中没有进一步覆写的版本，于是同样可以判定这个虚方法只有一个可能的调用目标。然后两者的优化程度会一模一样，无论是从“不需要通过虚分派而可以直接调用目标”（称为“去虚化”，devirtualization）还是从“便于内联”的角度看，这两种情况都是一样的。

而如果某个类层次结构中原本某个虚方法就存在多个覆写版本的话，那么本来也不可能对这个虚方法加上final修饰，所以就算这种情况下去虚化变得困难，锅也不能让“因为没用final修饰”来背。

使用final关键字在现代主流的优化JVM上不会提升性能。用它来修饰的虚方法是否递归都一样。

至于说如果一个递归方法内联了，题主的疑问有：

1. 内联后栈帧可能变大，是否会使递归调用变慢。答案是：相反，会变快。主流JVM里实现调用栈的方式都是通过直接挪动栈指针（stack pointer）就分配和撤销了栈帧空间，分配1个32字节的栈帧跟分配1个1KB的栈帧速度没区别（这里忽略栈扩容时可能有page miss的情况）。内联会消除函数调用自身的开销，例如 foo() 嵌套调用了 bar() 的话，原本调用 foo() 会引起1次栈帧分配，再调用 bar() 会再引起1次栈帧分配；而如果 foo() 内联了对 bar() 的调用，则这串调用只会引起一次 foo() + bar() 的栈帧分配，调用开销变小了，内联后的栈帧大小也通常会比单独的 foo() 栈帧 + bar() 栈帧要小。

   在有尾递归优化的环境中，尾递归调用的函数可以被优化为循环（但也有比较傻的、不优化为循环的实现，例如早期的32位x86上的CLR…），这循环可以看作是非常特殊的内联方式，完全消除了尾递归调用的函数调用开销。不过主流JVM上并没有实现通用的尾递归优化，还是靠普通内联来做优化。

   另外很重要的是，内联后编译器能看到更多上下文，其实是更利于优化的，特别是对于不擅长做过程间分析（interprocedural analysis）的编译器来说。

2. 上面已经举例提到，其实内联后栈的使用量通常是会减小的，比不内联更难爆栈

3. 请参考本文开头。在现代主流JVM上跑Java的话，请不要出于性能优化的目的使用final修饰符来修饰虚方法。

最后放个传送门：[为什么下面程序递归计算斐波那契数列java比c++要快？ - RednaxelaFX的回答 - 知乎](https://www.zhihu.com/question/56683164/answer/150190561)



> TODO
>
> yonka：
>
> CHA 对于“后来”加载的子类怎么处理呢？
>
> 类似：
>
> ```java
> void doSth(A a) {
>    a.run();  // 第一次执行时，类层级里A没有子类，第n次执行时加载了
> }
> ```









