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

