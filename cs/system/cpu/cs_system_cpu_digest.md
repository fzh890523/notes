# cache line

ref: 

- [lwn - What every programmer should know about memory, Part 1](https://lwn.net/Articles/250967/)

- [lwn - Memory part 2: CPU caches](https://lwn.net/Articles/252125/)

- [Linux Cache 机制](http://www.cnblogs.com/liloke/archive/2011/11/20/2255737.html) or [local](resource/Linux Cache 机制 - 李洛克 - 博客园.html)

  下文有大量摘抄于此文。

## 介绍

### 概述

```
cache，中译名高速缓冲存储器，其作用是为了更好的利用局部性原理，减少CPU访问主存的次数。简单地说，CPU正在访问的指令和数据，其可能会被以后多次访问到，或者是该指令和数据附近的内存区域，也可能会被多次访问。因此，第一次访问这一块区域时，将其复制到cache中，以后访问该区域的指令或者数据时，就不用再从主存中取出。
```

### 结构

假设内存容量为M，内存地址为m位：那么寻址范围为000…00~FFF…F(m位)

倘若把内存地址分为以下三个区间：

[![截图01](http://images.cnblogs.com/cnblogs_com/liloke/201111/201111200431313054.png)](http://images.cnblogs.com/cnblogs_com/liloke/201111/201111200431306217.png)《深入理解计算机系统》p305 英文版 beta draft  

 

tag, set index, block offset三个区间有什么用呢？再来看看Cache的逻辑结构吧：

[![截图02](http://images.cnblogs.com/cnblogs_com/liloke/201111/201111200431344883.png)](http://images.cnblogs.com/cnblogs_com/liloke/201111/201111200431331493.png) 

将此图与上图做对比，可以得出各参数如下：

B = 2^b

S = 2^s

现在来解释一下各个参数的意义：

> 一个cache被分为S个组，每个组有E个cacheline，而一个cacheline中，有B个存储单元，现代处理器中，这个存储单元一般是以字节(通常8个位)为单位的，也是最小的寻址单元。

> 因此，在一个内存地址中，中间的s位决定了该单元被映射到哪一组，而最低的b位决定了该单元在cacheline中的偏移量。valid通常是一位，代表该cacheline是否是有效的(当该cacheline不存在内存映射时，当然是无效的)。tag就是内存地址的高t位，因为可能会有多个内存地址映射到同一个cacheline中，所以该位是用来校验该cacheline是否是CPU要访问的内存单元。

> * 当tag和valid校验成功是，我们称为cache命中，这时只要将cache中的单元取出，放入CPU寄存器中即可。
> * 当tag或valid校验失败的时候，就说明要访问的内存单元(也可能是连续的一些单元，如int占4个字节，double占8个字节)并不在cache中，这时就需要去内存中取了，这就是cache不命中的情况(cache miss)。当不命中的情况发生时，系统就会从内存中取得该单元，将其装入cache中，与此同时也放入CPU寄存器中，等待下一步处理。



#### 标志位

* valid - 1bit

* tag - 内存地址高t位（其实不算标志位了）

* modified - 

  当modified=1时，表明cacheline被CPU写过。这说明，该cacheline中的内容可能已经被CPU修改过了，这样就与内存中相应的那些存储单元不一致了

### 访问

```
    当从内存中取单元到cache中时，会一次取一个cacheline大小的内存区域到cache中，然后存进相应的cacheline中。

    例如：我们要取地址 (t, s, b) 内存单元，发生了cache miss，那么系统会取 (t, s, 00…000) 到 (t, s, FF…FFF)的内存单元，将其放入相应的cacheline中。
```



### cache映射

cache的映射机制：

> * 当E=1时， 每组只有一个cacheline。那么相隔2^(s+b)个单元的2个内存单元，会被映射到同一个cacheline中。(好好想想为什么?)
> * 当1<E<C/B时，每组有E个cacheline，不同的地址，只要中间s位相同，那么就会被映射到同一组中，同一组中被映射到哪个cacheline中是依赖于替换算法的。
> * 当E=C/B，此时S=1，每个内存单元都能映射到任意的cacheline。带有这样cache的处理器几乎没有，因为这种映射机制需要昂贵复杂的硬件来支持。



不管哪种映射，只要发生了cache miss，那么必定会有一个cacheline大小的内存区域，被取到cache中相应的cacheline。

#### 地址映射

物理地址 - **而不是VA**

> 可想而知，如果是VA就乱套了... 每个task的VA都独立



做地址转换后才尝试访问... （cache - mem）



### 一致性

如果cacheline被写过（modified == 1），那么我们就应该将该cacheline中的内容写回到内存中，以保持数据的一致性。

#### 时机

当然不会是每当modified位被置1就写，这样会极大降低cache的性能，因为每次都要进行内存读写操作。事实上，大多数系统都会在这样的情况下将cacheline中的内容写回到内存：

> 当该cacheline被置换出去时，且modified位为1。

那么问题来了，**cacheline的置换时机是**？



* 访问的地址对应的cacheline已有但为modified时，先回写再...



## cache line操作

### 写回

#### 什么时候写回

我们什么时候写回到内存中呢？当然不会是每当modified位被置1就写，这样会极大降低cache的性能，因为每次都要进行内存读写操作。事实上，大多数系统都会在这样的情况下将cacheline中的内容写回到内存：

> 当该cacheline被置换出去时，且modified位为1。



### cache line flush

```
At least partial clearing the cache is necessary if the page table tree of a process changes. It might be possible to avoid a complete flush if the processor has an instruction which specifies the virtual address range which has changed.
```

### cache line invalidate



## 其他



### 多核情况下的问题

* 共享问题

  多个task（在不同CPU上）同时访问map到同一个地址（在同一个cacheline）的内容（进程内访问共享内存 或者 线程间），如同时修改一个bit却造成整个cacheline的频繁刷新

  > 典型case是遍历char[]写入

* 伪共享问题

  同时访问的虽然不是一个地址，但在同一个cacheline内，这种属于“无谓“的cacheline flush

  ​

  处理办法：

  1. 增大数组元素的间隔使得由不同线程存取的元素位于不同的cache line上。典型的空间换时间 
  2. 在每个线程中创建全局数组各个元素的本地拷贝，然后结束后再写回全局数组



```
系统是双核的，即为有2个CPU，CPU(例如Intel Pentium处理器)L1 cache是专有的，对于其他CPU不可见，每个cacheline有8个储存单元。

我们的程序中，有一个 char arr[8] 的数组，这个数组当然会被映射到CPU L1 cache中相同的cacheline，因为映射机制是硬件实现的，相同的内存都会被映射到同一个cacheline。

2个线程分别对这个数组进行写操作。当0号线程和1号线程分别运行于0号CPU和1号CPU时，假设运行序列如下：

开始CPU 0对arr[0]写；

随后CPU 1对arr[1]写；

随后CPU 0对arr[2]写；

……

CPU 1对arr[7]写；

根据多处理器中cache一致性的协议:

当CPU 0对arr[0]写时，8个char单元的数组被加载到CPU 0的某一个cacheline中，该cacheline的modified位已经被置1了；

当CPU 1对arr[1]写时，该数组应该也被加载到CPU 1的某个cacheline中，但是该数组在cpu0的cache中已经被改变，所以cpu0首先将cacheline中的内容写回到内存，然后再从内存中加载该数组到CPU 1中的cacheline中。CPU 1的写操作会让CPU 0对应的cacheline变为invalid状态注意，由于相同的映射机制，cpu1 中的 cacheline 和cpu0 中的cacheline在逻辑上是同一行(直接映射机制中是同一行，组相联映射中则是同一组)

当CPU 0对arr[2]写时，该cacheline是invalid状态，故CPU 1需要将cacheline中的数组数据传送给CPU 0，CPU 0在对其cacheline写时，又会将CPU 1中相应的cacheline置为invalid状态

……

如此往复，cache的性能遭到了极大的损伤！此程序在多核处理器下的性能还不如在单核处理器下的性能高。
```






### DMA带来的cache line数据不一致问题

```
经过DMA操作，cache缓存对应的内存数据已经被修改了，而CPU本身不知道（DMA传输是不通过CPU的），它仍然认为cache中的数 据就是内存中的数据，以后访问Cache映射的内存时，它仍然使用旧的Cache数据。这样就发生Cache与内存的数据“不一致性”错误。
```



