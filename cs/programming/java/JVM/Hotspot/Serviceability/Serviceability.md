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



## JVMTI agent

JVMTIAgent其实就是一个动态库，利用JVMTI暴露出来的一些接口来干一些我们想做、但是正常情况下又做不到的事情，不过为了和普通的动态库进行区分，它一般会实现如下的一个或者多个函数：

```c++
JNIEXPORT jint JNICALL
Agent_OnLoad(JavaVM *vm, char *options, void *reserved);

JNIEXPORT jint JNICALL
Agent_OnAttach(JavaVM* vm, char* options, void* reserved);

JNIEXPORT void JNICALL
Agent_OnUnload(JavaVM *vm); 
```

- Agent_OnLoad函数，如果agent是在启动时加载的，也就是在vm参数里通过-agentlib来指定的，那在启动过程中就会去执行这个agent里的Agent_OnLoad函数。
- Agent_OnAttach函数，如果agent不是在启动时加载的，而是我们先attach到目标进程上，然后给对应的目标进程发送load命令来加载，则在加载过程中会调用Agent_OnAttach函数。
- Agent_OnUnload函数，在agent卸载时调用，不过貌似基本上很少实现它。



### 示例



#### IDE调试java代码： JVMTI agent - jdwp

比如我们经常使用Eclipse等工具调试Java代码，其实就是利用JRE自带的jdwp agent实现的，只是Eclipse等工具在没让你察觉的情况下将相关参数(类似`-agentlib:jdwp=transport=dt_socket,suspend=y,address=localhost:61349`)自动加到程序启动参数列表里了，其中agentlib参数就用来跟要加载的agent的名字，比如这里的jdwp(不过这不是动态库的名字，JVM会做一些名称上的扩展，比如在Linux下会去找libjdwp.so的动态库进行加载，也就是在名字的基础上加前缀lib，再加后缀.so)，接下来会跟一堆相关的参数，将这些参数传给Agent_OnLoad或者Agent_OnAttach函数里对应的options。

* agent名字：jdwp

  `-agentlib:${agent_name}`

* 动态库： libjdwp.so

  `lib${agent_name}.so`

  > 命名风格应该是per platform的



#### javaagent： JVMTI agent 之 JPLISAgent

instrument agent实现了Agent_OnLoad和Agent_OnAttach两方法，也就是说在使用时，agent既可以在启动时加载，也可以在运行时动态加载。其中启动时加载还可以通过类似-javaagent:myagent.jar的方式来间接加载instrument agent，运行时动态加载依赖的是JVM的attach机制（[JVM Attach机制实现](http://lovestblog.cn/blog/2014/06/18/jvm-attach/)），通过发送load命令来加载agent。

##### instrument agent的核心数据结构

```
struct _JPLISAgent {
    JavaVM *                mJVM;                   /* handle to the JVM */
    JPLISEnvironment        mNormalEnvironment;     /* for every thing but retransform stuff */
    JPLISEnvironment        mRetransformEnvironment;/* for retransform stuff only */
    jobject                 mInstrumentationImpl;   /* handle to the Instrumentation instance */
    jmethodID               mPremainCaller;         /* method on the InstrumentationImpl that does the premain stuff (cached to save lots of lookups) */
    jmethodID               mAgentmainCaller;       /* method on the InstrumentationImpl for agents loaded via attach mechanism */
    jmethodID               mTransform;             /* method on the InstrumentationImpl that does the class file transform */
    jboolean                mRedefineAvailable;     /* cached answer to "does this agent support redefine" */
    jboolean                mRedefineAdded;         /* indicates if can_redefine_classes capability has been added */
    jboolean                mNativeMethodPrefixAvailable; /* cached answer to "does this agent support prefixing" */
    jboolean                mNativeMethodPrefixAdded;     /* indicates if can_set_native_method_prefix capability has been added */
    char const *            mAgentClassName;        /* agent class name */
    char const *            mOptionsString;         /* -javaagent options string */
};

struct _JPLISEnvironment {
    jvmtiEnv *              mJVMTIEnv;              /* the JVM TI environment */
    JPLISAgent *            mAgent;                 /* corresponding agent */
    jboolean                mIsRetransformer;       /* indicates if special environment */
};
```

这里解释一下几个重要项：

- mNormalEnvironment：主要提供正常的类transform及redefine功能。
- mRetransformEnvironment：主要提供类retransform功能。
- mInstrumentationImpl：这个对象非常重要，也是我们Java agent和JVM进行交互的入口，或许写过javaagent的人在写`premain`以及`agentmain`方法的时候注意到了有个Instrumentation参数，该参数其实就是这里的对象。
- mPremainCaller：指向`sun.instrument.InstrumentationImpl.loadClassAndCallPremain`方法，如果agent是在启动时加载的，则该方法会被调用。
- mAgentmainCaller：指向`sun.instrument.InstrumentationImpl.loadClassAndCallAgentmain`方法，该方法在通过attach的方式动态加载agent的时候调用。
- mTransform：指向`sun.instrument.InstrumentationImpl.transform`方法。
- mAgentClassName：在我们javaagent的MANIFEST.MF里指定的`Agent-Class`。
- mOptionsString：传给agent的一些参数。
- mRedefineAvailable：是否开启了redefine功能，在javaagent的MANIFEST.MF里设置`Can-Redefine-Classes:true`。
- mNativeMethodPrefixAvailable：是否支持native方法前缀设置，同样在javaagent的MANIFEST.MF里设置`Can-Set-Native-Method-Prefix:true`。
- mIsRetransformer：如果在javaagent的MANIFEST.MF文件里定义了`Can-Retransform-Classes:true`，将会设置mRetransformEnvironment的mIsRetransformer为true。

##### 在启动时加载instrument agent

正如前面“概述”里提到的方式，就是启动时加载instrument agent，具体过程都在`InvocationAdapter.c`的`Agent_OnLoad`方法里，这里简单描述下过程：

- 创建并初始化JPLISAgent
- 监听VMInit事件，在vm初始化完成之后做下面的事情：			创建InstrumentationImpl对象		监听ClassFileLoadHook事件		调用InstrumentationImpl的`loadClassAndCallPremain`方法，在这个方法里会调用javaagent里MANIFEST.MF里指定的`Premain-Class`类的premain方法		
- 解析javaagent里MANIFEST.MF里的参数，并根据这些参数来设置JPLISAgent里的一些内容

##### 在运行时加载instrument agent

在运行时加载的方式，大致按照下面的方式来操作：

```
VirtualMachine vm = VirtualMachine.attach(pid); 
vm.loadAgent(agentPath, agentArgs); 
```

上面会通过JVM的attach机制来请求目标JVM加载对应的agent，过程大致如下：

- 创建并初始化JPLISAgent
- 解析javaagent里MANIFEST.MF里的参数
- 创建InstrumentationImpl对象
- 监听ClassFileLoadHook事件
- 调用InstrumentationImpl的loadClassAndCallAgentmain方法，在这个方法里会调用javaagent里MANIFEST.MF里指定的Agent-Class类的agentmain方法



##### **instrument agent的ClassFileLoadHook回调实现**

不管是启动时还是运行时加载的instrument agent，都关注着同一个jvmti事件——ClassFileLoadHook，这个事件是在读取字节码文件之后回调时用的，这样可以对原来的字节码做修改，那这里面究竟是怎样实现的呢？

```
void JNICALL

eventHandlerClassFileLoadHook(  jvmtiEnv *              jvmtienv,
                                JNIEnv *                jnienv,
                                jclass                  class_being_redefined,
                                jobject                 loader,
                                const char*             name,
                                jobject                 protectionDomain,
                                jint                    class_data_len,
                                const unsigned char*    class_data,
                                jint*                   new_class_data_len,
                                unsigned char**         new_class_data) {

    JPLISEnvironment * environment  = NULL;

    environment = getJPLISEnvironment(jvmtienv);

    /* if something is internally inconsistent (no agent), just silently return without touching the buffer */

    if ( environment != NULL ) {

        jthrowable outstandingException = preserveThrowable(jnienv);
        transformClassFile( environment->mAgent,
                            jnienv,
                            loader,
                            name,
                            class_being_redefined,
                            protectionDomain,
                            class_data_len,
                            class_data,
                            new_class_data_len,
                            new_class_data,
                            environment->mIsRetransformer);

        restoreThrowable(jnienv, outstandingException);
    }

}
```

先根据jvmtiEnv取得对应的JPLISEnvironment，因为上面我已经说到其实有两个JPLISEnvironment（并且有两个jvmtiEnv），其中一个是专门做retransform的，而另外一个用来做其他事情，根据不同的用途，在注册具体的ClassFileTransformer时也是分开的，对于作为retransform用的ClassFileTransformer，我们会注册到一个单独的TransformerManager里。

接着调用transformClassFile方法，由于函数实现比较长，这里就不贴代码了，大致意思就是调用InstrumentationImpl对象的transform方法，根据最后那个参数来决定选哪个TransformerManager里的ClassFileTransformer对象们做transform操作。

```
private byte[]
    transform(  ClassLoader         loader,
                String              classname,
                Class               classBeingRedefined,
                ProtectionDomain    protectionDomain,
                byte[]              classfileBuffer,
                boolean             isRetransformer) {

        TransformerManager mgr = isRetransformer?

                                        mRetransfomableTransformerManager :
                                        mTransformerManager;

        if (mgr == null) {

            return null; // no manager, no transform

        } else {

            return mgr.transform(   loader,
                                    classname,
                                    classBeingRedefined,
                                    protectionDomain,
                                    classfileBuffer);

        }

    }


  public byte[]

    transform(  ClassLoader         loader,
                String              classname,
                Class               classBeingRedefined,
                ProtectionDomain    protectionDomain,
                byte[]              classfileBuffer) {

        boolean someoneTouchedTheBytecode = false;
        TransformerInfo[]  transformerList = getSnapshotTransformerList();
        byte[]  bufferToUse = classfileBuffer;

        // order matters, gotta run 'em in the order they were added

        for ( int x = 0; x < transformerList.length; x++ ) {

            TransformerInfo         transformerInfo = transformerList[x];
            ClassFileTransformer    transformer = transformerInfo.transformer();
            byte[]                  transformedBytes = null;

            try {

                transformedBytes = transformer.transform(   loader,
                                                            classname,
                                                            classBeingRedefined,
                                                            protectionDomain,
                                                            bufferToUse);

            }

            catch (Throwable t) {

                // don't let any one transformer mess it up for the others.
                // This is where we need to put some logging. What should go here? FIXME

            }


            if ( transformedBytes != null ) {
                someoneTouchedTheBytecode = true;
                bufferToUse = transformedBytes;
            }

        }


        // if someone modified it, return the modified buffer.
        // otherwise return null to mean "no transforms occurred"

        byte [] result;

        if ( someoneTouchedTheBytecode ) {
            result = bufferToUse;
        }
        else {
            result = null;
        }

        return result;

    }   

```

以上是最终调到的java代码，可以看到已经调用到我们自己编写的javaagent代码里了，我们一般是实现一个ClassFileTransformer类，然后创建一个对象注册到对应的TransformerManager里。

##### **Class Transform的实现**

这里说的class transform其实是狭义的，主要是针对第一次类文件加载时就要求被transform的场景，在加载类文件的时候发出ClassFileLoad事件，然后交给instrumenat agent来调用javaagent里注册的ClassFileTransformer实现字节码的修改。

##### **Class Redefine的实现**

类重新定义，这是Instrumentation提供的基础功能之一，主要用在已经被加载过的类上，想对其进行修改，要做这件事，我们必须要知道两个东西，一个是要修改哪个类，另外一个是想将那个类修改成怎样的结构，有了这两个信息之后就可以通过InstrumentationImpl下面的redefineClasses方法操作了：

```
public void redefineClasses(ClassDefinition[]   definitions) throws  ClassNotFoundException {

        if (!isRedefineClassesSupported()) {

            throw new UnsupportedOperationException("redefineClasses is not supported in this environment");

        }

        if (definitions == null) {

            throw new NullPointerException("null passed as 'definitions' in redefineClasses");

        }

        for (int i = 0; i < definitions.length; ++i) {

            if (definitions[i] == null) {

                throw new NullPointerException("element of 'definitions' is null in redefineClasses");

            }

        }

        if (definitions.length == 0) {

            return; // short-circuit if there are no changes requested

        }


        redefineClasses0(mNativeAgent, definitions);

    }

```

在JVM里对应的实现是创建一个VM_RedefineClasses的VM_Operation，注意执行它的时候会stop-the-world：

```
jvmtiError

JvmtiEnv::RedefineClasses(jint class_count, const jvmtiClassDefinition* class_definitions) {

//TODO: add locking

  VM_RedefineClasses op(class_count, class_definitions, jvmti_class_load_kind_redefine);

  VMThread::execute(&op);

  return (op.check_error());

} /* end RedefineClasses */
```

这个过程我尽量用语言来描述清楚，不详细贴代码了，因为代码量实在有点大：

- 挨个遍历要批量重定义的jvmtiClassDefinition
- 然后读取新的字节码，如果有关注ClassFileLoadHook事件的，还会走对应的transform来对新的字节码再做修改
- 字节码解析好，创建一个klassOop对象
- 对比新老类，并要求如下：			父类是同一个		实现的接口数也要相同，并且是相同的接口		类访问符必须一致		字段数和字段名要一致		新增的方法必须是private static/final的		可以删除修改方法		
- 对新类做字节码校验
- 合并新老类的常量池
- 如果老类上有断点，那都清除掉
- 对老类做JIT去优化
- 对新老方法匹配的方法的jmethodId做更新，将老的jmethodId更新到新的method上
- 新类的常量池的holer指向老的类
- 将新类和老类的一些属性做交换，比如常量池，methods，内部类
- 初始化新的vtable和itable
- 交换annotation的method、field、paramenter
- 遍历所有当前类的子类，修改他们的vtable及itable

上面是基本的过程，总的来说就是只更新了类里的内容，相当于只更新了指针指向的内容，并没有更新指针，避免了遍历大量已有类对象对它们进行更新所带来的开销。

##### **Class Retransform的实现**

retransform class可以简单理解为回滚操作，具体回滚到哪个版本，这个需要看情况而定，下面不管那种情况都有一个前提，那就是javaagent已经要求要有retransform的能力了：

- 如果类是在第一次加载的的时候就做了transform，那么做retransform的时候会将代码回滚到transform之后的代码
- 如果类是在第一次加载的的时候没有任何变化，那么做retransform的时候会将代码回滚到最原始的类文件里的字节码
- 如果类已经加载了，期间类可能做过多次redefine(比如被另外一个agent做过)，但是接下来加载一个新的agent要求有retransform的能力了，然后对类做redefine的动作，那么retransform的时候会将代码回滚到上一个agent最后一次做redefine后的字节码

我们从InstrumentationImpl的retransformClasses方法参数看猜到应该是做回滚操作，因为我们只指定了class：

```
    public void retransformClasses(Class<?>[] classes) {

        if (!isRetransformClassesSupported()) {

            throw new UnsupportedOperationException( "retransformClasses is not supported in this environment");

        }

        retransformClasses0(mNativeAgent, classes);

    }
```

不过retransform的实现其实也是通过redefine的功能来实现，在类加载的时候有比较小的差别，主要体现在究竟会走哪些transform上，如果当前是做retransform的话，那将忽略那些注册到正常的TransformerManager里的ClassFileTransformer，而只会走专门为retransform而准备的TransformerManager的ClassFileTransformer，不然想象一下字节码又被无声无息改成某个中间态了。

```
private:

  void post_all_envs() {

    if (_load_kind != jvmti_class_load_kind_retransform) {

      // for class load and redefine,

      // call the non-retransformable agents

      JvmtiEnvIterator it;

      for (JvmtiEnv* env = it.first(); env != NULL; env = it.next(env)) {

        if (!env->is_retransformable() && env->is_enabled(JVMTI_EVENT_CLASS_FILE_LOAD_HOOK)) {

          // non-retransformable agents cannot retransform back,

          // so no need to cache the original class file bytes

          post_to_env(env, false);

        }

      }

    }

    JvmtiEnvIterator it;

    for (JvmtiEnv* env = it.first(); env != NULL; env = it.next(env)) {

      // retransformable agents get all events

      if (env->is_retransformable() && env->is_enabled(JVMTI_EVENT_CLASS_FILE_LOAD_HOOK)) {

        // retransformable agents need to cache the original class file

        // bytes if changes are made via the ClassFileLoadHook

        post_to_env(env, true);

      }

    }

  }

```

##### **javaagent的其他小众功能**

javaagent除了做字节码上面的修改之外，其实还有一些小功能，有时候还是挺有用的

- 获取所有已经被加载的类：Class[] getAllLoadedClasses(); 
- 获取所有已经初始化了的类： Class[] getInitiatedClasses(ClassLoader loader); 
- 获取某个对象的大小： long getObjectSize(Object objectToSize); 
- 将某个jar加入到bootstrap classpath里优先其他jar被加载： void appendToBootstrapClassLoaderSearch(JarFile jarfile); 
- 将某个jar加入到classpath里供appclassloard去加载：void appendToSystemClassLoaderSearch(JarFile jarfile); 
- 设置某些native方法的前缀，主要在找native方法的时候做规则匹配： void setNativeMethodPrefix(ClassFileTransformer transformer, String prefix)。

# SA









