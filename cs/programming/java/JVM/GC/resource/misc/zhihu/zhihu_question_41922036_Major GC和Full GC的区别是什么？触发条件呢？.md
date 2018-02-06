# question

**Major GC和Full GC的区别是什么？触发条件呢？**

如题。大神们说说你们的看法呗。（我理解是Major GC只针对老年代，Full GC会先触发一次Minor GC，不知对否）。



# answer: R大

针对HotSpot VM的实现，它里面的GC其实准确分类只有两大种：

- Partial GC：并不收集整个GC堆的模式
- - Young GC：只收集young gen的GC
  - Old GC：只收集old gen的GC。只有CMS的concurrent collection是这个模式
  - Mixed GC：收集整个young gen以及部分old gen的GC。只有G1有这个模式
- Full GC：收集整个堆，包括young gen、old gen、perm gen（如果存在的话）等所有部分的模式。

Major GC通常是跟full GC是等价的，收集整个GC堆。但因为HotSpot VM发展了这么多年，外界对各种名词的解读已经完全混乱了，当有人说“major GC”的时候一定要问清楚他想要指的是上面的full GC还是old GC。

- young GC：当young gen中的eden区分配满的时候触发。注意young GC中有部分存活对象会晋升到old gen，所以young GC后old gen的占用量通常会有所升高。
- full GC：当准备要触发一次young GC时，如果发现统计数据说之前young GC的平均晋升大小比目前old gen剩余的空间大，则不会触发young GC而是转为触发full GC（因为HotSpot VM的GC里，除了CMS的concurrent collection之外，其它能收集old gen的GC都会同时收集整个GC堆，包括young gen，所以不需要事先触发一次单独的young GC）；或者，如果有perm gen的话，要在perm gen分配空间但已经没有足够空间时，也要触发一次full GC；或者System.gc()、heap dump带GC，默认也是触发full GC。

HotSpot VM里其它非并发GC的触发条件复杂一些，不过大致的原理与上面说的其实一样。
当然也总有例外。Parallel Scavenge（-XX:+UseParallelGC）框架下，默认是在要触发full GC前先执行一次young GC，并且两次GC之间能让应用程序稍微运行一小下，以期降低full GC的暂停时间（因为young GC会尽量清理了young gen的死对象，减少了full GC的工作量）。控制这个行为的VM参数是-XX:+ScavengeBeforeFullGC。这是HotSpot VM里的奇葩嗯。可跳传送门围观：[JVM full GC的奇怪现象，求解惑？ - RednaxelaFX 的回答](https://www.zhihu.com/question/48780091/answer/113063216)



# answer: [Ted Mosby](https://www.zhihu.com/people/ted-mosby-4-95)



1. Full GC定义是相对明确的，就是针对整个新生代、老生代、元空间（metaspace，java8以上版本取代perm gen）的全局范围的GC；

2. Minor GC和Major GC是俗称，在Hotspot JVM实现的Serial GC, Parallel GC, CMS, G1 GC中大致可以对应到某个Young GC和Old GC算法组合；

3. 最重要是搞明白上述Hotspot JVM实现中几种GC算法组合到底包含了什么。

   1. Serial GC算法：Serial Young GC ＋ Serial Old GC （敲黑板！敲黑板！敲黑板！实际上它是全局范围的Full GC）；
   2. Parallel GC算法：Parallel Young GC ＋ 非并行的PS MarkSweep GC / 并行的Parallel Old GC（敲黑板！敲黑板！敲黑板！这俩实际上也是全局范围的Full GC），选PS MarkSweep GC 还是 Parallel Old GC 由参数UseParallelOldGC来控制；
   3. CMS算法：ParNew（Young）GC + CMS（Old）GC （piggyback on ParNew的结果／老生代存活下来的object只做记录，不做compaction）＋ Full GC for CMS算法（应对核心的CMS GC某些时候的不赶趟，开销很大）；
   4. G1 GC：Young GC + mixed GC（新生代，再加上部分老生代）＋ Full GC for G1 GC算法（应对G1 GC算法某些时候的不赶趟，开销很大）；

4. 搞清楚了上面这些组合，我们再来看看各类GC算法的触发条件。

   简单说，触发条件就是某GC算法对应区域满了，或是预测快满了。比如，
   1. 各种Young GC的触发原因都是eden区满了；
   2. Serial Old GC／PS MarkSweep GC／Parallel Old GC的触发则是在要执行Young GC时候预测其promote的object的总size超过老生代剩余size；
   3. CMS GC的initial marking的触发条件是老生代使用比率超过某值；
   4. G1 GC的initial marking的触发条件是Heap使用比率超过某值，跟4.3 heuristics 类似；
   5. Full GC for CMS算法和Full GC for G1 GC算法的触发原因很明显，就是4.3 和 4.4 的fancy算法不赶趟了，只能全局范围大搞一次GC了（相信我，这很慢！这很慢！这很慢！）；

5. 题主说的 “Full GC会先触发一次Minor GC” － 指的应该是

   1. （说错了，我删了）

   2. PS MarkSweep GC／Parallel Old GC（Full GC）之前会跑一次Parallel Young GC

      原因就是减轻Full GC 的负担。

哇～整个picture 是有点乱，希望我整理的还算清楚：）

















