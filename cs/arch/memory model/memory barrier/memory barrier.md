ref：

* [Memory barrier - wikipedia](https://en.wikipedia.org/wiki/Memory_barrier) or [local](resource/Memory barrier - Wikipedia.html)



# 概述



[what-is-a-memory-fence from stackoverflow](https://stackoverflow.com/questions/286629/what-is-a-memory-fence)

```
For performance gains modern CPUs often execute instructions out of order to make maximum use of the available silicon (including memory read/writes). Because the hardware enforces instructions integrity you never notice this in a single thread of execution. However for multiple threads or environments with volatile memory (memory mapped I/O for example) this can lead to unpredictable behavior.

A memory fence/barrier is a class of instructions that mean memory read/writes occur in the order you expect. For example a 'full fence' means all read/writes before the fence are comitted before those after the fence.

Note memory fences are a hardware concept. In higher level languages we are used to dealing with mutexes and semaphores - these may well be implemented using memory fences at the low level and explicit use of memory barriers are not necessary. Use of memory barriers requires a careful study of the hardware architecture and more commonly found in device drivers than application code.

The CPU reordering is different from compiler optimisations - although the artefacts can be similar. You need to take separate measures to stop the compiler reordering your instructions if that may cause undesirable behaviour (e.g. use of the volatile keyword in C).
```



* 背景： CPU乱序执行 （**而不是编译时对指令做重排序**）

  但是，应用程序有时还是需要关注编译重排序，这时候可能用特定技术（如C的volatile）来告知编译器重排指令

* 关注： 硬件层面的概念，一般kernel/driver层面关注，应用代码一般不关注

* 概念： 一组CPU指令

* 作用： 让CPU执行指令的顺序符合预期 - 也即让CPU顺序执行

* 范围： 主要是多CPU场景



疑问：

* 单线程下，read/write之间的mem-bar是怎么保证的呢？

  如：

  ```c
  int a = 1;  // 1
  int b = a;  // 2
  a++;  // 3
  b = a + 1;  //4

  // 显然： 1和2不能乱序；2和3不能乱序；3和4不能乱序
  // 是不能理解为： 同一地址的read和write不能乱序？

  // 当然了，这个保证只能对单线程有效(不影响行为)，对多线程还是会产生紊乱
  // 而且显然，对于多线程执行流的“透明优化(乱序)”几乎是无法实现的
  ```

  这个是怎么保证的呢？CPU在“并发执行”时，自动排列完成的吗？ 还是对CPU透明在编译期加入？ 感觉前者可能性比较大。

  ​

  wikipedia里简单一句带过了：

  > When a program runs on a single-CPU machine, the hardware performs the necessary bookkeeping to ensure that the program executes as if all memory operations were performed in the order specified by the programmer (program order), so memory barriers are not necessary. 

  实际上应该是上层不用关心的意思吧。



## 为什么（多核场景下）需要....

```C
// Processor #1:

 while (f == 0);
 // Memory fence required here
 print x;


// Processor #2:

 x = 42;
 // Memory fence required here
 f = 1;
```

代码编写者肯定期望只输出42，因为从书写顺序上：

* `f != 0`时x必然已经完成`= 32`赋值

但实际上，无论是编译重排还是CPU乱序执行都可能让`f = 1`先执行，因为从single flow上来看，并不满足**限制重排**的条件。



那么，如果要保证结果同预期，则需要：

1. 告诉编译器不要对此处做重排

   > 这个是编译期确定的，真正runtime产生影响是下一条

2. 告诉CPU这里需要按序执行

   > 虽然是runtime生效，但还是编译期确定的，编译器需要根据硬件架构（内存模型、指令集）选择合适的方式...



## 乱序执行 vs 编译重排

继续前面说的，mem-bar主要针对硬件层面的乱序执行，此外还需要在适当的时候禁止编译重排，这个一般是通过**上层同步原语**来做到的。

### c/c++的volatile

本意是用于**直接访问memory-mapped I/O**，显然这些IO是需要按序的，所以 要求编译器（也即其语义）：

* 这样的（volatile）IO操作不能被编译器优化掉（比如认为无效于是干掉）

* 对同一地址的该IO操作的顺序需要保持，**但注意： 不需要保证与其他操作的顺序**

  同一约束的两个条件：

  1. 同一地址
  2. volatile

  > 那么单核情况下的顺序是可以保证（CPU自己保证，前面提到）

  如：

  ```c
  volatile int i;
  volatile int j;
  int a;

  void do() {
    i = 1;  // 1
    a = 2;  // 2
    i++;  // 3
    a = i + 2;  // 4
    j = 3;  // 5
    i = j + a;  // 6  其实分为 读j 和 写i 两步(读a先忽略)
  }

  // 能保证的顺序：
  // 1 - 3 - 4 - 6
  // 5 - 6
  ```

  ​

  > java old MM也是沿用这一点，使得volatile基本残废，新MM里加强了语义，使得volatile var的读写操作和其他所有（不只是volatile var）读写操作的顺序得以保持



**更要注意**

**volatile并不保证mem-bar，所以多核并行情况下还是会面临乱序问题**

> wikipedia里原文是 **The keyword `volatile` does not guarantee a memory barrier to enforce cache-consistency. **
>
> {yonka}但个人感觉这里缓存一致性问题倒在其次

**所以volatile并不能用于多线程共享变量**



#### 编译器行为

编译器可以根据自己的实现来选择加强volatile语义，但这是特定编译器相关的，开发者不能依赖该行为。



如：

* gcc

  对于 带有`volatile`和`"memory"`的内联汇编，不会重排其前后的操作（应该是前面的在前，后面的在后吧），如：

  * `asm volatile("" ::: "memory");`
  * `__asm__ __volatile__ ("" ::: "memory");`

  对于c11/c++11，`atomic_signal_fence(memory_order_acq_rel);`有类似的效果。



#### 语言规范



有一些语言，可能会增强volatile的语义，如

* java，从新MM（1.5）开始，volatile可以同时保证避免硬件乱序和编译重排
* c++ 11开始，atomic类型有了java volatile接近的语义



# 各硬件架构下mem-bar实现

当某架构提供了多种mem-bar时，需要根据场景选择cost尽量小的。



## x86/x64



提供了几种mem-bar指令

* full fence

  顾名思义，是 全量 “大规模杀伤性” 的，会：

  1. commit该fence之前的所有load/store操作

     > 这种commit是不是有两种含义？
     >
     > 1. 先于执行
     > 2. flush cache

  2. 然后执行该fence之后的load/store操作

  显然，这种mem-bar是代价高昂的



## itanium

```
separate "acquire" and "release" memory barriers which address the visibility of read-after-write operations from the point of view of a reader (sink) or writer (source) respectively
```





## other

```
 separate memory barriers to control ordering between different combinations of system memory and I/O memory
```



# 高层同步原语（与mem-bar的关系）











