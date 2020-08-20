# 问题



**JVM full GC的奇怪现象，求解惑？**

今天本来是想用JMX写个监控程序，但在尝试监控GC情况时，发现一个奇怪的现象
能看出来新生代用PS Scavenge，老年代用PS MarkSweep
我用JMX分别取了两个回收器的上次回收情况，发现一个奇怪的问题：

PS MarkSweep上一次垃圾回收前后的内存使用情况是：
[PS Survivor Space] - before:327680 after:0
[PS Eden Space] - before:0 after:0
[PS Old Gen] - before:203895944 after:203923920
[PS Perm Gen] - before:211102776 after:211102776

这次垃圾回收前后Old Gen的内存使用情况几乎没有变化，都是194M左右，回收后甚至比回收前还高了一点点，而Old Gen的最大内存是910M。Perm Gen的内存使用率也没有变化（Perm区分了560M）。只有Survivor Space从327680降到了0。
这次Full GC看起来不仅基本啥事都没干，而且似乎根本不应该被触发？触发前Eden是空的，Old Gen和Perm Gen的可用空间都还很大。
求解惑。

另外附加一些信息：
本次PS MarkSweep的耗时是1743ms，GC开始时间和在上一次PS Scavenge的GC完成时间完全一样，也就是说，上一次YoungGC完成后立刻就开始了这次FullGC，并且基本啥都没回收掉。
JVM版本是64-bit Hotspot VM, 1.6.0_27



# R大回复



不奇怪，一切现象都是有原因的。

先来了解些背景信息。在题主所使用的JDK 6 update 27的HotSpot VM里，-XX:+UseParallelGC会启用题主所说的配置（这也是HotSpot VM在Server Class Machine上的默认配置）。

其中，负责执行minor GC（只收集young gen）的是PS Scavenge，全称是ParallelScavenge，是个并行的copying算法的收集器；
而负责执行full GC（收集整个GC堆，包括young gen、old gen、perm gen）的是PS MarkSweep——但整个收集器并不是并行的，而在骨子里是跟Serial Old是同一份代码，是串行收集的。其算法是经典的LISP2算法，是一种mark-compact GC（不要被MarkSweep的名字骗了）。
（注意这个跟使用-XX:+UseParallelOldGC所指定的并不是同一个收集器，那个是PS Compact，是个并行的全堆收集器）

ParallelScavenge这个GC套装的full GC有个很特别的实现细节，那就是：当触发full GC的时候，实际上会先使用PS Scavenge执行一次young GC收集young gen，然后紧接着去用PS MarkSweep执行一次真正的full GC收集全堆。
所以说题主看到的现象就是很正常的一次ParallelScavenge的full GC而已。

```
  product(bool, ScavengeBeforeFullGC, true,                                 \
          "Scavenge youngest generation before each full GC, "              \
          "used with UseParallelGC")  

```

指定 -XX:-ScavengeBeforeFullGC 就可以不在执行full GC的时候先执行一次PS Scavenge。

jdk6/jdk6/hotspot: 7561dfbeeee5 src/share/vm/gc_implementation/parallelScavenge/psMarkSweep.cpp**

```
void PSMarkSweep::invoke(bool maximum_heap_compaction) {
  // ...

  if (ScavengeBeforeFullGC) {
    PSScavenge::invoke_no_policy();
  }

  // ...
}

```

================================

> 这次FullGC为什么会被触发？回收前Old Gen的使用率是194M/910M，Survivor只占用了300多K，Eden则是0，此时Old Gen空间很富余，从YoungGen晋升的对象也只有300多K，PermGen也很富余……按照我的理解，似乎此时不应该触发FullGC啊？

正因为上面说的，ParallelScavenge这套GC在触发full GC时实际上会先做一个young GC（PS Scavenge），再执行真正的full GC（PS MarkSweep），所以题主在看数据的时候就被弄晕了：
题主实际看到的是在“真正的full GC”的数据，而这是在刚刚做完那个young GC后的，所以自然，此时edgen是空的，而survivor space里的对象都是活的。

要看这次full GC为何触发，必须去看这个因为full GC而触发的young GC之前的状态才行。

================================













