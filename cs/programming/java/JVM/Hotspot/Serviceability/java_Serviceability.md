# 介绍

参考： [hotspot docs Serviceability](http://openjdk.java.net/groups/hotspot/docs/Serviceability.html) or [local](resource/hotspot_docs_Serviceability.md)

> 文档里有详细介绍，比如没种技术相关的源代码等

hotspot里在serviceability方面有以下几种技术：

* SA（serviceability agent）

  sun私有，原先是给Hotspot工程师使用，然后才给enduser使用

  ​

  能做：

  * 从java进程或产生的core file里读（内存）数据
  * 从raw数据里提取出Hotspot数据结构（的数据）
  * 从Hotspot数据结构的数据里提取java对象

  ​

  **注意**： SA执行期间目标进程会halt

  ​

  实现

  * linux
    * ptrace
    * /proc
  * windows
    * dbgeng.dll 也有实现是用windows的进程debug原语（只能对活着的进程）
  * solaris
    * libproc

  ​

  一致性问题

  > 根源在于**friend class VMStructs**（vmStructs.cpp，vmStructs_*.cpp），该cpp会被编译为vmStructs.o（包含在libjvm.so），其中有SA用于读取Hotspot数据结构的所有（元）数据
  >
  > 所以，一旦该cpp中字段有改动（重命名、删除等），使用该数据的java代码也需要同步，否则会读取失败

  ​

  了解更多：[The HotSpot™ Serviceability Agent: An out-of-process high level debugger for a Java™ virtual machine](https://www.usenix.org/legacy/events/jvm01/full_papers/russell/russell_html/index.html) or [local](resource/Java™ Virtual Machine Research and Technology Symposium 2001 Paper.html)

* jvmstat性能指标（计数）

  每个版本的计数器名字可能有差别，不能太依赖，尤其是`java.`开头的

  以共享内存文件的形式暴露，适当的时机进行更新

  * solaris和linux都是通过mmap方式，把文件map到内存
  * 这个文件成为backing store file，位于/tmp下
  * 文件名类似：`/tmp/hsperfdata_user-name/vm-id`
  * windows上有些差异，不详述

* JVM TI

  [JSR 163 - JavaTM Platform Profiling Architecture](http://jcp.org/en/jsr/detail?id=163)的参考C实现

  在Hotspot代码里很多地方下了钩子 = = （grep -i jvmti）

  [介绍实现的ppt](http://openjdk.java.net/groups/hotspot/docs/jvmtiImpl.pdf) or [local](resource/jvmtiImpl.pdf)

  > [JVMTI](http://docs.oracle.com/javase/7/docs/platform/jvmti/jvmti.html)全称JVM Tool Interface，是JVM暴露出来的一些供用户扩展的接口集合。JVMTI是基于事件驱动的，JVM每执行到一定的逻辑就会调用一些事件的回调接口（如果有的话），这些接口可以供开发者扩展自己的逻辑。

* JMM接口（监控和管理）

* 动态attach

  sun私有技术...

  linux上实现：

  * SIGQUIT（SIGBREAK）信号
  * signal dispatcher线程、attach listener线程
  * unix socket文件（target process cwd、/tmp下）
  * ...

  solarics上实现：

  * Doors IPC

  windows上实现：

  * named pipe


* DTrace

  Solaris上的动态跟踪工具，Hotspot可以基于其监控各方面的操作


* pstack

  solaris上的工具，输出stack trace。 而Hotspot增强为可以显示java栈帧

* 一些命令行选项如-verbose

* SIGQUIT/ctrl-break 输出一份stack dump of executing threads



## 常见点解释

### JVMTI和动态attach的关系

一句话： 

> 动态attach后做什么呢： 一般是load agent - 这个agent就是JVMTI agent

两句话：

* 动态attach是触发条件、入口

  > attach也是一套API，它负责动态地将dynamic module attach到指定进程id的java进程内并触发回调

  ```java
  import java.io.IOException;
  import com.sun.tools.attach.VirtualMachine;

  public class VMAttacher {

      public static void main(String[] args) throws Exception {
  	 // args[0]为java进程id
           VirtualMachine virtualMachine = com.sun.tools.attach.VirtualMachine.attach(args[0]);
           // args[1]为共享库路径，args[2]为传递给agent的参数
           virtualMachine.loadAgentPath(args[1], args[2]);
           virtualMachine.detach();
      }

  }
  ```

* JVMTI是执行内容的基础： 暴露的API

  JVMTI可以通过动态attach（的API）启动，也可以通过jvm启动选项方式启动：

  ```Shell
      java -agentlib:<agent-lib-name>=<options> Sample
      # 注意，这里的共享库路径是环境变量路径，例如 java -agentlib:foo=opt1,opt2，java启动时会从linux的LD_LIBRARY_PATH或windows的PATH环境变量定义的路径处装载foo.so或foo.dll，找不到则抛异常

      java -agentpath:<path-to-agent>=<options> Sample
      # 这是以绝对路径的方式装载共享库，例如 java -agentpath:/home/admin/agentlib/foo.so=opt1,opt2
  ```

  ​

### attach API用途

* 一类是加载Java agent，这个是可扩展的（自己写Java agent）；

* 另一类是工具通过私有API发送命令给JVM，执行特定的功能（如jstack、jmap）

  > 这种要扩展就必须改JVM自身，对上层程序员来说这扩展性几乎为0。除非打算自己修改与编译JVM，不然要捞JVM层面的数据还是用SA方便。



### SA和JVM TI的关系

#### 用途

* SA主要是给JVM开发人员做JVM调试用的，更底层
  * 不碰JVM底层的话一般用不到
  * 也不绝对，比如jstack -F就会用
  * 根据strace观察到的，原来说用动态attach+JVMTI的jmap、jinfo也是用SA实现
* JVM TI主要是给应用开发者用的




#### 角度

* SA： 进程外

  受限少

  性能很低（linux上一次ptrace读一个字，然后根据jvm版本重新构造为jvm指令 - *所以jmap等工具需要和target进程的java同一个包（版本）*）

* JVM TI： 进程内

  JVM hang住时就无效了，因为依赖JVM的accept-exec-return （这时候就需要-F了）



#### 优缺点

* SA
  * 优点
    * 无需target JVM配合，可以用于hang进程
    * 可以在更宽松的条件下使用，比如root用户（而不是必须用户名一致）
  * 缺点
    * 对于量大的数据（堆），非常慢
    * 工具和target 进程必须同一个版本（JDK）
    * 不保证安全点
* JVM TI
  * 优点
    * 直接在JVM里操作，访问速度快
    * 可以使用任意版本的工具和target进程交互
  * 缺点
    * 工具和target进程需要edui/egid一致
    * 只能用于正常jvm（hang住的不想跟你）
    * target JVM可能 `-XX:+DisableAttachMechanism`（不需要多解释了）




#### others



[R大的回复](http://hllvm.group.iteye.com/group/topic/34278)

```
sa-jdi.jar是HotSpot自带的底层调试支持，Serviceability Agent的实现。它的来龙去脉请参考2001年的一篇报告：http://static.usenix.org/event/jvm01/full_papers/russell/russell_html/

SA意义上的agent跟JVMTI/Java agent不是同一个层次上的。

现有的SA实现只支持“snapshot”式的使用方式，也就是
1、通过系统级调试API将一个活的目标进程暂停下来，将SA连接上去，做分析，做完之后将SA断开连接，然后恢复目标进程的正常运行；
2、打开core dump，利用SA来分析里面的内容。也就是事后分析（postmortem）。

“snapshot”是指，上面这两种使用场景都不支持在SA保持连接的同时让目标进程运行。

我是经常会用到CLHSDB的，但通常Java应用层的开发不会直接用它吧。
SA系的工具可以很方便的抓取到JVM层面的信息，相对Java应用来说这就是“底层信息”了。当然对应用来说也可以派上用场，例如淘宝的同事叔同写的这个：TBJMap“增强版jmap”，或者我之前写的这玩儿的原型https://gist.github.com/1551013，又例如我写的用来显示NIO direct memory使用状况的工具：https://gist.github.com/1593521

SA里自带工具大部分都通过JDK的工具命令暴露出来了，例如jmap、jstack这些的某些选项的功能就是通过SA来实现的。

以前在淘宝的时候，我们的生产环境上要连接/拔下一个空的SA系工具大概要300ms。如果要额外捞点“位置固定”的数据那花不了多少时间。但如果要遍历GC堆的话花上几秒甚至几分钟都是完全正常的。生产环境上要小心使用。

关于文档：CLHSDB的文档在这里有 http://hg.openjdk.java.net/jdk7u/jdk7u6/hotspot/file/7566374c3c89/agent/doc/
而SA系的内部更细节的地方就没文档了。只有源码。上文提到的论文是除了源码之外最好的入门途径。

===================================================

可能有人留意到了JDK自带的那些工具中，有好些都是有两种实现的。以jstack为例，默认参数下它用的是attach API，也就是楼主说的tools.jar里的那边；使用了-F或-m，或者是对core dump来使用的时候，它用的就是SA系的实现。

这俩最大的区别是，attach API是“in-process”，而SA是“out-of-process”的。如果对调试器的实现方式熟悉的话这俩名词应该不会陌生。张银奎在《程序员》杂志上连载的“调试之剑”专栏的2011年1、2月两期讲CLRv4的调试架构重构的时候正好也提到了这俩概念，可以参考。

简单来说，in-process就是调试器（或类似的功能）运行在目标进程内，而out-of-process是调试器跟目标进程是独立的两个进程，通过进程间通信来实现调试。

前者的重要好处是不依赖底层（特别是操作系统提供的调试API）所以容易移植，而且运行速度较快（因为访问数据的时候是在同进程内的）。但缺点也很明显：调试功能自身容易干扰到被调试目标，形成“海森堡效应”。
例如说，本来要用jstack看一个Java程序的所有Java线程的栈，attach API的实现方式是：
1、客户端连接到目标JVM，向其发出一个“threaddump”文本命令；
2、目标JVM接收到这个命令，执行JVM内的一个thread_dump()函数，将收集到的结果以文本形式返回
3、客户端接收到返回的文本并将其显示出来。
这样，如果thread_dump()自身的实现有bug，搞不好就把这个目标Java进程给弄挂了。
又或者，如果目标进程没响应了（hung了），那它就接收不到那个threaddump命令，jstack就没办法正常执行了。

而后者，out-of-process方式则正好相反：调试功能在目标进程之外运行，通过进程间通信从目标进程获取数据。通常这种进程间通信依赖于操作系统提供的调试API，移植起来会麻烦一些，而且数据的访问速度较慢；但好处是调试功能与调试目标之间有良好的隔离，即便调试功能自身有问题也不会影响到目标进程。例如说用jstack -F去看线程栈信息，就算目标进程hung了也照样能看；或者是jstack -F自身有bug死循环了，只要把这个jstack -F的进程杀掉，目标进程就又能继续正常运行，不会受影响。 
```



```
SA最有趣的一点就是它暴露出来的API完全是Java的，有极高的灵活性和可扩展性，所以用它来写工具非常方便。有时候我要对VM内的数据做分析的时候都会借助SA先用Java来写，如果实在有必要做在VM里再把它用C++重新写一次。

而attach API有两类用途，一类是加载Java agent，这个是可扩展的（自己写Java agent）；另一类是工具通过私有API发送命令给JVM，执行特定的功能（如jstack、jmap），这种要扩展就必须改JVM自身，对上层程序员来说这扩展性几乎为0。除非打算自己修改与编译JVM，不然要捞JVM层面的数据还是用SA方便。 
```



```
SA虽然方便，但是要了解只能靠源码了解其大概能实现什么功能，这个比较痛苦。。顺便问下R大，SA是否是JVMTI其中一部分的Java实现 ，还是？？如果两者不一样的话，能否对比下。。:wink:
RednaxelaFX的博客
	
RednaxelaFX 2012-08-23
不，SA跟JVMTI的目的和用途都不一样。两者没有直接关系。

简单来说SA本来就是给JVM开发人员对VM自身做调试用的，并不是设计来给外面或者说Java应用层的开发人员用的。它的Java部分的源码跟HotSpot VM的C++源码有很强的联系，对VM开发人员来说这很直观所以也就不需要什么文档了——会对细节关心的人相应的也应该对VM自身很熟悉才对。

JVMTI是JVMDI、JVMPI等早期JDK提供的调试、剖析API的继承者。它本来就是设计来给外面的人使用的，而且是标准API，需要考虑到跨JVM的兼容性。
它现在主要用来支撑Java层的调试器（例如各主流Java IDE自带的那些）、profiler之类的工具。它的设计就是前面提到的in-process方式的。

要说相似性，Java agent跟JVMTI倒有一点点重叠的地方。JVMTI agent跟Java agent最大的区别是实现语言，前者用C/C++，后者用Java。它们重叠的地方主要是在类加载事件的监控、字节码改写之类的使用场景上。 
```



### 几个agent之间的关系

ref:

* [JVM源码分析之javaagent原理完全解读](http://www.infoq.com/cn/articles/javaagent-illustrated) or [local](resource/JVM源码分析之javaagent原理完全解读.html)



* JVMTIAgent

  JVMTI 动态库

  启动方式： `-agentlib:${agent_name}`

* Instrument Agent（JPLISAgent(Java Programming Language Instrumentation Services Agent)）

  一个JVMTI agent实现

* javaagent

  基于JPLISAgent实现的（java部分？）

  启动方式： `-javaagent:${agent_name}.jar`，会间接加载instrument agent



# JVMTI

移到单独的JVMTI文档中。



# SA

见 `java_SA.md`。











