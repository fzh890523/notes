ref：

* [Java ServiceabilityAgent(HSDB)使用和分析](https://liuzhengyang.github.io/2017/05/27/serviceabilityagent/)





# 概述



Serviceability Agent 简称SA, 原本是Sun公司用来debug Hotspot的工具，后来开放给Hotspot使用者，能够查看Java的内部数据结构等信息，它可以直接观察一个进程或分析core文件。`The HotSpot Serviceability Agent (SA) is a set of APIs and tools for debugging HotSpot Virtual Machine`



## SA的特点



ref： R大在hllvm group的回复



* 学习资料基本靠源码

  除了相关论文： [](https://www.usenix.org/conference/jvm-01/hotspot%E2%84%A2-serviceability-agent-out-process-high-level-debugger-java%E2%84%A2-virtual)

* `暴露出来的API完全是Java的，有极高的灵活性和可扩展性，所以用它来写工具非常方便。有时候我要对VM内的数据做分析的时候都会借助SA先用Java来写，如果实在有必要做在VM里再把它用C++重新写一次`  - R大

* out-of-process

  见下文

* 只支持“snapshot”式的使用方式

  也就是

  1、通过系统级调试API将一个活的目标进程暂停下来，将SA连接上去，做分析，做完之后将SA断开连接，然后恢复目标进程的正常运行；

  2、打开core dump，利用SA来分析里面的内容。也就是事后分析（postmortem）。

  > Snapshot： 上面这两种使用场景都不支持在SA保持连接的同时让目标进程运行。

* SA里自带工具大部分都通过JDK的工具命令暴露出来了，例如jmap、jstack这些的某些选项的功能就是通过SA来实现的。

  JDK自带的那些工具中，有好些都是有两种实现的。以jstack为例，默认参数下它用的是attach API，也就是楼主说的tools.jar里的那边；使用了-F或-m，或者是对core dump来使用的时候，它用的就是SA系的实现。

  ​



## in-process VS out-of-process

ref: [hhlvm-讨论](http://hllvm.group.iteye.com/group/forum?tag_id=690) [sa-jdi.jar 和tools.jar 使用的一些问题](http://hllvm.group.iteye.com/group/topic/34278)



attach 
API是“in-process”，而SA是“out-of-process”的。如果对调试器的实现方式熟悉的话这俩名词应该不会陌生。张银奎在《程序员》杂志上连载的“调试之剑”专栏的2011年1、2月两期讲CLRv4的调试架构重构的时候正好也提到了这俩概念，可以参考。



简单来说，in-process就是调试器（或类似的功能）运行在目标进程内，而out-of-process是调试器跟目标进程是独立的两个进程，通过进程间通信来实现调试。



前者的重要好处是不依赖底层（特别是操作系统提供的调试API）所以容易移植，而且运行速度较快（因为访问数据的时候是在同进程内的）。但缺点也很明显：调试功能自身容易干扰到被调试目标，形成“海森堡效应”。

例如说，本来要用jstack看一个Java程序的所有Java线程的栈，attach API的实现方式是：

1、客户端连接到目标JVM，向其发出一个“threaddump”文本命令；

2、目标JVM接收到这个命令，执行JVM内的一个[thread_dump()函数](http://hg.openjdk.java.net/jdk7u/jdk7u6/hotspot/file/7566374c3c89/src/share/vm/services/attachListener.cpp)，将收集到的结果以文本形式返回

3、客户端接收到返回的文本并将其显示出来。

这样，如果thread_dump()自身的实现有bug，搞不好就把这个目标Java进程给弄挂了。

又或者，如果目标进程没响应了（hung了），那它就接收不到那个threaddump命令，jstack就没办法正常执行了。



而后者，out-of-process方式则正好相反：调试功能在目标进程之外运行，通过进程间通信从目标进程获取数据。通常这种进程间通信**依赖于操作系统提供的调试API，移植起来会麻烦一些**，而且**数据的访问速度较慢**；但好处是**调试功能与调试目标之间有良好的隔离**，即便调试功能自身有问题也不会影响到目标进程。例如说用jstack
 -F去看线程栈信息，就算目标进程hung了也照样能看；或者是jstack -F自身有bug死循环了，只要把这个jstack 
-F的进程杀掉，目标进程就又能继续正常运行，不会受影响。



> SA最有趣的一点就是它暴露出来的API完全是Java的，有极高的灵活性和可扩展性，所以用它来写工具非常方便。有时候我要对VM内的数据做分析的时候都会借助SA先用Java来写，如果实在有必要做在VM里再把它用C++重新写一次。

> attach API有两类用途，一类是加载Java agent，这个是可扩展的（自己写Java 
> agent）；另一类是工具通过私有API发送命令给JVM，执行特定的功能（如jstack、jmap），这种要扩展就必须改JVM自身，对上层程序员来说这扩展性几乎为0。除非打算自己修改与编译JVM，不然要捞JVM层面的数据还是用SA方便。



