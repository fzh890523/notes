ref：

* [博文 - 豆列：从表到里学习JVM实现](http://rednaxelafx.iteye.com/blog/1886170)
* [豆列：从表到里学习JVM实现](https://www.douban.com/doulist/2545443/)



# [豆列：从表到里学习JVM实现](http://rednaxelafx.iteye.com/blog/1886170) 


[JVM](http://www.iteye.com/blogs/tag/JVM)

​    刚写了个学习JVM用的豆列跟大家分享。
豆列地址：[http://book.douban.com/doulist/2545443/](http://book.douban.com/doulist/2545443/)
下面把豆列的介绍贴过来。具体书单请到上面的豆列地址那边去看。
在社会化分工、软件行业细分专业化的趋势下，会真的参与到底层系统实现的人肯定是越来越少（比例上说）。真的会参与到JVM实现的人肯定是少数。
但如果您对JVM是如何实现的有兴趣、充满好奇，却苦于没有足够系统的知识去深入，那么这个豆列就是为您打好基础而准备的。
如果只想用用Java用用JVM，对深入到实现细节无甚兴趣的话，这个豆列就请不必参考了，免得浪费钱浪费时间精力，呵呵 :-)
本豆列的脉络是：
\1. JVM与Java语言规范
要了解JVM是如何实现的，首先必须要知道JVM到底是什么、不是什么，表面上应该提供怎样的功能。为此，JVM规范必读，而且应该时常放在手边参考。
而JVM的主要服务对象是Java编程语言。虽然JVM也可以支持众多其它语言，但JVM里的“J”仍然最重要，Java的语言特性影响了JVM的原始设计，所以Java语言规范也应该阅读。特别是，JVM关于线程和同步相关的规定都是交由Java语言规范的相关章节定义的。
\2. 虚拟机概论
这里选取《Virtual Machines: Versatile Platforms for Systems and Processes》，帮助您了解“虚拟机”一词到底指代什么，有什么不同类型，大概有哪些实现方法，等等。读完这本书有助获得一个清晰的大局观。
\3. 为Java程序员从用户的角度介绍JVM的使用经验的几本书
虽然这几本并没有深入到JVM实现的非常细节的角落，但对已经习惯用Java语言编程的程序员来说，有这么几本书带领自己从熟悉的领域进入不熟悉的领域总是件好事。
这几本书中，最深入JVM内部的是《Oracle JRockit: The Definitive Guide》；有丰富调优建议的是《Java Performance》；结合实现大概介绍JVM的抽象概念的是周志明的《深入理解Java虚拟机》。
\4. 虚拟机的入门级实现
先通过《Language Implementation Patterns》了解编程语言的一些入门级实现方式，把高级语言编译器与虚拟机两个概念联系起来。
然后通过《プログラミング言語を作る》了解非常简易的、用树遍历式以及字节码式解释器实现虚拟机大概是个怎么回事。虽然这本书没有实现JVM，但它介绍的Diksam与早期JVM的实现颇有相似之处，可参考。
接下来《深入嵌入式Java虚拟机》介绍了一种实际的JVM——KVM的实现细节。KVM是CLDC的参考实现（RI）里的JVM，结构简单，资源消耗小，适合入门阅读。
这部分最后是《The School of Niklaus Wirth》，里面有一章介绍了HotSpot Client Compiler 
(C1)的原始设计思路。这是个非常简单、但相对来说性能还不错的JIT编译器，可用于对JIT编译器的基本了解。这本书本身就很赞，不为学习虚拟机也可以一读。
需要注意的是从“简易的JVM实现”到“高性能、复杂的JVM实现”跨度非常大；前者的许多部分的实现方式与后者相当不同。先从简易的实现开始入手主要是为了对JVM里大概都有些什么组件有所了解。但如果目标是了解高性能JVM的实现，那就必须在GC、编译原理方面打下更好的基础，重新洗一次脑。
\5. C++基础书
下面要开始逐渐深入JVM的内部实现，如果没有良好的C或（与？）C++基础会比较吃力。虽然也有几乎完全用Java语言实现的高性能JVM，例如Maxine VM与Jikes RVM，但它们都是研究性质的；商用JVM实现仍然是C与C++的天下。
这里我先推荐C++之父自己写的那本书来入门。虽然BS巨巨后来还出过本新书，而近来也渐渐开始有介绍C++11的入门书，但实际上现在多数JVM实现用的还是C99或非常古老的C++（连C++03都不一定用到了），所以用这本老书应该就够了。
然后通过《深度探索C++对象模型》来学习C++对象模型的常见实现方式。这对后面理解Java对象模型的实现很有帮助。
\6. GC与编译原理的入门书
GC书总共就那么几本，倒也没啥可挑的。《The Garbage Collection Handbook》是绝对必读。
编译原理的书就稍微尴尬些。现有的编译原理书大都针对静态编译器、针对像C或C++那样的偏静态、偏native的语言。我还没读到过什么编译原理书是专门介绍JIT编译器或者说动态编译器的。静态与动态编译器会有些取舍上、实现策略上的差异，不过还好其核心的原理都是一样的，所以还是可以推荐几本书。龙书用来最初入门，鲸书用来补充一些优化相关的知识，EAC第二版用来学习编译器一种比较良好的逻辑组织方式，最后学一下针对现代机器的优化。
\7. 介绍计算机体系结构的书
实际JVM实现里，如果有JIT编译器或者动态编译器那它们的编译目标多半是底层机器的机器码。这就涉及到计算机体系结构了。
如果您只对Java语言和抽象的JVM有一定了解，那可以用《计算机组成及汇编语言原理》来入门。这本书比较奇葩，用JVM的字节码指令集来当作真实机器介绍体系结构的概念。我并不太喜欢这本书，但感觉它对有Java背景的初学者来说应该有点用。要注意的是千万别只读这本书来入门，请结合下面要介绍的一本书来重新洗一次脑。
如果对C或C++已经有所了解，那《深入理解计算机系统》（CSAPP）是计算机体系结构入门的最适合的书了。
\8. 进一步阅读
到此为止各种抽象概念应该都了解得差不多了。那么要在真实的机器上实现高性能JVM，就必须要对真实机器的指令集细节有所了解。x86/x86-64、SPARC、ARM、MIPS，要在哪个平台上做高性能实现就要学习哪个平台的指令集及指令级别优化技巧。这里就不具体推荐书了。
操作系统层面的知识同样重要。像是说JVM要实现线程、内存分配啥的，都可能要跟系统调用或CRT对系统调用的包装打交道。这部分也需要另外找书来读。我回头再考虑下要不要加几本道这个豆列里来。
另外，从80年代开始高级语言虚拟机的实现技术有了突飞猛进的发展，但却没有专门的书对这个领域做综述和导读。多数有用的资料其实还是在论文里。光靠读书是远远不够用的，论文这块也请关注。
顺便广告一下：我的博客里关于虚拟机的文章也推荐给大家参考：[http://rednaxelafx.iteye.com/blog/362738](http://rednaxelafx.iteye.com/blog/362738)
=====================================
这个豆列没有漏掉 [《Inside the Java Virtual Machine, Second Edition》](http://book.douban.com/subject/1788390/) ，中文版[《深入Java虚拟机(原书第2版)》](http://book.douban.com/subject/1138768/)，只是我现在已经不再推荐它。这本书刚出版的时候确实引起了一番学习Java虚拟机的热潮，但其部分内容从现在的角度看已经过时，特别是涉及JVM实现的部分。像火车算法什么的现在已经没有JVM实现使用。不过话说回来，了解了解这些过时的信息也没什么不好，前提是能自己分辨清楚哪些信息是适用于现在的JVM的，而哪些已经成为了历史。
另外有一本清华大学出版社出的[《解析Java虚拟机开发——权衡优化.高效和安全的最优方案》](http://book.douban.com/subject/24757135/)，这本纯粹是对周志明的《深入理解Java虚拟机》一书的抄袭。然而抄袭也抄得很不给力，印刷、排版都不如原版。建议不要购买。

**12** 
顶

**7** 
踩

分享到：      [![img](http://rednaxelafx.iteye.com/images/sina.jpg)](javascript:;)      [![img](http://rednaxelafx.iteye.com/images/tec.jpg)](javascript:;)    

​      [对C语义的for循环的基本代码生成模式](http://rednaxelafx.iteye.com/blog/1961217)      |      [借HSDB来探索HotSpot VM的运行时数据](http://rednaxelafx.iteye.com/blog/1847971)    

- 2013-06-13 14:13
- 浏览 38877
- [评论(16)](http://rednaxelafx.iteye.com/blog/1886170#comments)
- 分类:[编程语言](http://www.iteye.com/blogs/category/language)
- [相关推荐](http://www.iteye.com/wiki/blog/1886170)

                              

评论

​    16 楼    [xiyueshenyan](http://xiyueshenyan.iteye.com)    2017-05-17            

Virtual Machines: Versatile Platforms for Systems and Processes 读了这本才知道，没一定基础还是不要读的好！有些空中楼阁的感觉

​    15 楼    [xyfwjk](http://xyfwjk.iteye.com)    2014-11-18            

逛论坛时,偶然发现了这样一篇高质量的JVM讲解.[借HSDB来探索HotSpot VM的运行时数据]
之前看过周志明的《深入理解Java虚拟机》,看了你的博客,发现你是真大神.这个学习列表对我很有用.

​    14 楼    [fh63045](http://fenghong.iteye.com)    2014-02-27            

收藏 ![img](http://rednaxelafx.iteye.com/images/smiles/icon_idea.gif) 

​    13 楼    [sun128837](http://conntsing.iteye.com)    2013-09-16            

liuzejian4 写道

RednaxelaFX 写道

realvalkyrie 写道

プログラミング言語を作る
这个读不懂，什么时候翻译一把。。

​    12 楼    [liuzejian4](http://liuzejian4.iteye.com)    2013-09-07            

RednaxelaFX 写道

realvalkyrie 写道

プログラミング言語を作る
这个读不懂，什么时候翻译一把。。

​    11 楼    [Checkmate](http://taojintianxia.iteye.com)    2013-06-19            

你的文章看过一些,真没想到你竟然涉猎这么广泛又深入.

​    10 楼    [RednaxelaFX](http://rednaxelafx.iteye.com)    2013-06-14            

realvalkyrie 写道

プログラミング言語を作る
这个读不懂，什么时候翻译一把。。

​    9 楼    [RednaxelaFX](http://rednaxelafx.iteye.com)    2013-06-14            

kelly_zsl 写道

能不能推荐几本实用点。。别跟商业挂钩啊。。

​    8 楼    [zhb8015](http://peter8015.iteye.com)    2013-06-14            

《深入java虚拟机》这本不错，用心研究一下肯定受益。

​    7 楼    [kelly_zsl](http://kelly-zsl.iteye.com)    2013-06-14            

能不能推荐几本实用点。。别跟商业挂钩啊。。

​    6 楼    [realvalkyrie](http://realvalkyrie.iteye.com)    2013-06-14            

プログラミング言語を作る
这个读不懂，什么时候翻译一把。。

​    5 楼    [zidafone](http://zidafone.iteye.com)    2013-06-14            

需要很多时间和精力啊，佩服能坚持下来的人![img](http://rednaxelafx.iteye.com/images/smiles/icon_idea.gif) 

​    4 楼    [RednaxelaFX](http://rednaxelafx.iteye.com)    2013-06-14            

ywbrj042 写道

如果能够深入到这种层次，绝对是技术专家了。

​    3 楼    [star4evar](http://star4evar.iteye.com)    2013-06-14            

谢谢 先搜藏 等有空系统的学习一下jvm的原理

​    2 楼    [ywbrj042](http://ywbrj042.iteye.com)    2013-06-14            

如果能够深入到这种层次，绝对是技术专家了。

​    1 楼    [yunzhu](http://yunzhu.iteye.com)    2013-06-13            

好文章，具有指导意义，以后沿着这条路子走走看 ![img](http://rednaxelafx.iteye.com/images/smiles/icon_idea.gif) 



# 从表到里学习JVM实现

​      [        ![RednaxelaFX](https://img3.doubanio.com/icon/u19617360-4.jpg)      ](https://www.douban.com/people/RednaxelaFX/)              来自: [          RednaxelaFX            (San Francisco Bay Area)        ](https://www.douban.com/people/RednaxelaFX/)                  2013-06-13创建              2014-11-30更新                  

​        在社会化分工、软件行业细分专业化的趋势下，会真的参与到底层系统实现的人肯定是越来越少（比例上说）。真的会参与到JVM实现的人肯定是少数。
但如果您对JVM是如何实现的有兴趣、充满好奇，却苦于没有足够系统的知识去深入，那么这个豆列就是为您打好基础而准备的。
如果只想用用Java用用JVM，对深入到实现细节无甚兴趣的话，这个豆列就请不必参考了，免得浪费钱浪费时间精力，呵呵 :-)
本豆列的脉络是：
\1. JVM与Java语言规范
要了解JVM是如何实现的，首先必须要知道JVM到底是什么、不是什么，表面上应该提供怎样的功能。为此，JVM规范必读，而且应该时常放在手边参考。
而JVM的主要服务对象是Java编程语言。虽然JVM也可以支持众多其它语言，但JVM里的“J”仍然最重要，Java的语言特性影响了JVM的原始设计，所以Java语言规范也应该阅读。特别是，JVM关于线程和同步相关的规定都是交由Java语言规范的相关章节定义的。
\2. 虚拟机概论
这里选取《Virtual Machines: Versatile Platforms for Systems and Processes》，帮助您了解“虚拟机”一词到底指代什么，有什么不同类型，大概有哪些实现方法，等等。读完这本书有助获得一个清晰的大局观。
\3. 为Java程序员从用户的角度介绍JVM的使用经验的几本书
虽然这几本并没有深入到JVM实现的非常细节的角落，但对已经习惯用Java语言编程的程序员来说，有这么几本书带领自己从熟悉的领域进入不熟悉的领域总是件好事。
这几本书中，最深入JVM内部的是《Oracle JRockit: The Definitive Guide》；有丰富调优建议的是《Java Performance》；结合实现大概介绍JVM的抽象概念的是周志明的《深入理解Java虚拟机》。
\4. 虚拟机的入门级实现
先通过《Language Implementation Patterns》了解编程语言的一些入门级实现方式，把高级语言编译器与虚拟机两个概念联系起来。
然后通过《プログラミング言語を作る》了解非常简易的、用树遍历式以及字节码式解释器实现虚拟机大概是个怎么回事。虽然这本书没有实现JVM，但它介绍的Diksam与早期JVM的实现颇有相似之处，可参考。
接下来《深入嵌入式Java虚拟机》介绍了一种实际的JVM——KVM的实现细节。KVM是CLDC的参考实现（RI）里的JVM，结构简单，资源消耗小，适合入门阅读。
陈涛的《HotSpot实战》主要介绍了HotSpot 
VM的核心VM的实现。第一版的内容还不算深，介绍runtime的部分比较多，而介绍GC与动态编译器的具体实现的部分比较少。想对HotSpot 
VM的实现有初步了解，但担心GC与编译原理的知识太难的同学可以从这本书入手。
（不过请注意自己对着代码确实把知识消化掉，因为这个第一版的内容有不少小错误嗯…）
这部分最后是《The School of Niklaus Wirth》，里面有一章介绍了HotSpot Client Compiler 
(C1)的原始设计思路。这是个非常简单、但相对来说性能还不错的JIT编译器，可用于对JIT编译器的基本了解。这本书本身就很赞，不为学习虚拟机也可以一读。
需要注意的是从“简易的JVM实现”到“高性能、复杂的JVM实现”跨度非常大；前者的许多部分的实现方式与后者相当不同。先从简易的实现开始入手主要是为了对JVM里大概都有些什么组件有所了解。但如果目标是了解高性能JVM的实现，那就必须在GC、编译原理方面打下更好的基础，重新洗一次脑。
\5. C++基础书
下面要开始逐渐深入JVM的内部实现，如果没有良好的C或（与？）C++基础会比较吃力。虽然也有几乎完全用Java语言实现的高性能JVM，例如Maxine VM与Jikes RVM，但它们都是研究性质的；商用JVM实现仍然是C与C++的天下。
这里我先推荐C++之父自己写的那本书来入门。虽然BS巨巨后来还出过本新书，而近来也渐渐开始有介绍C++11的入门书，但实际上现在多数JVM实现用的还是C99或非常古老的C++（连C++03都不一定用到了），所以用这本老书应该就够了。
然后通过《深度探索C++对象模型》来学习C++对象模型的常见实现方式。这对后面理解Java对象模型的实现很有帮助。
\6. GC与编译原理的入门书
GC书总共就那么几本，倒也没啥可挑的。《The Garbage Collection Handbook》是绝对必读。
编译原理的书就稍微尴尬些。现有的编译原理书大都针对静态编译器、针对像C或C++那样的偏静态、偏native的语言。我还没读到过什么编译原理书是专门介绍JIT编译器或者说动态编译器的。《The Compiler Design Handbook》（[http://book.douban.com/subject/2333166/](https://www.douban.com/link2/?url=http%3A%2F%2Fbook.douban.com%2Fsubject%2F2333166%2F)）算是有一章专门讲动态编译，不过也只是很入门的介绍。
静态与动态编译器会有些取舍上、实现策略上的差异，不过还好其核心的原理都是一样的，所以还是可以推荐几本书。龙书用来最初入门，鲸书用来补充一些优化相关的知识，EAC第二版用来学习编译器一种比较良好的逻辑组织方式，最后学一下针对现代机器的优化。《Optimizing
 Compilers for Modern Architectures》（[http://book.douban.com/subject/2126867/](https://www.douban.com/link2/?url=http%3A%2F%2Fbook.douban.com%2Fsubject%2F2126867%2F)）是我比较喜欢的一本。
\7. 介绍计算机体系结构的书
实际JVM实现里，如果有JIT编译器或者动态编译器那它们的编译目标多半是底层机器的机器码。这就涉及到计算机体系结构了。
如果您只对Java语言和抽象的JVM有一定了解，那可以用《计算机组成及汇编语言原理》来入门。这本书比较奇葩，用JVM的字节码指令集来当作真实机器介绍体系结构的概念。我并不太喜欢这本书，但感觉它对有Java背景的初学者来说应该有点用。要注意的是千万别只读这本书来入门，请结合CSAPP来重新洗一次脑。
如果对C或C++已经有所了解，那《深入理解计算机系统》（CSAPP）是计算机体系结构入门的最适合的书了。
\8. 进一步阅读
到此为止各种抽象概念应该都了解得差不多了。那么要在真实的机器上实现高性能JVM，就必须要对真实机器的指令集细节有所了解。x86/x86-64、SPARC、ARM、MIPS，要在哪个平台上做高性能实现就要学习哪个平台的指令集及指令级别优化技巧。这里就不具体推荐书了。
操作系统层面的知识同样重要。像是说JVM要实现线程、内存分配啥的，都可能要跟系统调用或CRT对系统调用的包装打交道。这部分也需要另外找书来读。我回头再考虑下要不要加几本道这个豆列里来。
另外，从80年代开始高级语言虚拟机的实现技术有了突飞猛进的发展，但却没有专门的书对这个领域做综述和导读。多数有用的资料其实还是在论文里。光靠读书是远远不够用的，论文这块也请关注。
顺便广告一下：我的博客里关于虚拟机的文章也推荐给大家阅读：[http://rednaxelafx.iteye.com/blog/362738](https://www.douban.com/link2/?url=http%3A%2F%2Frednaxelafx.iteye.com%2Fblog%2F362738&link2key=6d26cff436)
=====================================
这个豆列没有漏掉《Inside the Java Virtual Machine, Second Edition》（ [http://book.douban.com/subject/1788390/](https://www.douban.com/link2/?url=http%3A%2F%2Fbook.douban.com%2Fsubject%2F1788390%2F) ，中文版《深入Java虚拟机(原书第2版)》，[http://book.douban.com/subject/1138768/](https://www.douban.com/link2/?url=http%3A%2F%2Fbook.douban.com%2Fsubject%2F1138768%2F)）
，只是我现在已经不再推荐它了。这本书刚出版的时候确实引起了一番学习Java虚拟机的热潮，但其部分内容从现在的角度看已经过时，特别是涉及JVM实现的部分。像火车算法什么的现在已经没有JVM实现使用。不过话说回来，了解了解这些过时的信息也没什么不好，前提是能自己分辨清楚哪些信息是适用于现在的JVM的，而哪些已经成为了历史。
另外有一本清华大学出版社出的《解析Java虚拟机开发——权衡优化.高效和安全的最优方案》（[http://book.douban.com/subject/24757135/](https://www.douban.com/link2/?url=http%3A%2F%2Fbook.douban.com%2Fsubject%2F24757135%2F)），这本纯粹是对周志明的《深入理解Java虚拟机》一书的抄袭；周志明与出版社对清华大学出版社的侵权行为提起的诉讼已经胜诉。然而抄袭也抄得很不给力，印刷、排版都不如原版。建议不要购买。









