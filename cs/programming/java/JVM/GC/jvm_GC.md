ref：

* [Java GC工作原理以及Minor GC、Major GC、Full GC简单总结](http://blog.csdn.net/u012500848/article/details/51355404)



# 概念



## 分代内存管理





### 不同代支持的GC方式



For Young Generation, you can use any one of the following:

- `-XX:+UseSerialGC`
- `-XX:+UseParallelGC`
- `-XX:+UseParNewGC`

For Old Generation, the available choices are:

- `-XX:+UseParallelOldGC`
- `-XX:+UseConcMarkSweepGC`





## minor GC、major GC、full GC VS CMS and etc...



ref：

* [Java Major and Minor Garbage Collections](https://stackoverflow.com/questions/16549066/java-major-and-minor-garbage-collections)

* [Minor GC、Major GC和Full GC之间的区别](http://www.importnew.com/15820.html)

  吐槽一下，翻译的不好。




> 这些术语无论是在 JVM 规范还是在垃圾收集研究论文中都没有正式的定义

确实，JLS、JVMS、Hotspot MM文档里都没有对这些词的明确定义，只有提及。



字面意思：

* minor

  清理年轻代内存

* major

  清理老年代内存

* full

  清理整个堆空间—包括年轻代和老年代



实际：

* 很多major GC是有minor触发
* major == full
* major是否先minor根据配置而定
* old代的GC动作不都属于fullGC，只有STW阶段才算（对于CMS来说为mark和remark，所以一次会fullGC计数+2）



* **Minor Collection** is triggered when then JVM is unable to allocate space for a new Object (Remember: new objects are always allocated in Young Generation's Eden Area).
* JVM considers **Major Collection** [in the Older (or Perm) Generation] as **Full GC**. 




## GC（触发）时机



### minor GC时机





### full GC时机





## misc



### GC时间



#### full GC time

[聊聊JVM（四）深入理解Major GC, Full GC, CMS](http://blog.csdn.net/iter_zc/article/details/41825395)的结论：

1. Full GC == Major GC指的是对老年代/永久代的stop the world的GC
2. Full GC的次数 = 老年代GC时 stop the world的次数
3. Full GC的时间 = 老年代GC时 stop the world的总时间
4. CMS 不等于Full GC，我们可以看到**CMS分为多个阶段，只有stop the world的阶段被计算到了Full GC的次数和时间**，而和业务线程并发的GC的次数和时间则不被认为是Full GC
5. Full GC本身不会先进行Minor GC，我们可以配置，让Full GC之前先进行一次Minor GC，因为老年代很多对象都会引用到新生代的对象，先进行一次Minor GC可以提高老年代GC的速度。比如老年代使用CMS时，设置CMSScavengeBeforeRemark优化，让CMS remark之前先进行一次Minor GC。























