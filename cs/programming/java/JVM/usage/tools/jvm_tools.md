Ref:

* [聊聊JVM（一）相对全面的GC总结](http://blog.csdn.net/iter_zc/article/details/41746265)





# 场景



## 参数配置



### 查看JVM启动参数



1. `jps -v`
2. `jinfo -flags pid`
3. `jinfo pid` -- 列出JVM启动参数和system.properties
4. `ps -ef | grep Java`



### 查看当前堆的配置

1. `jstat -gc pid 1000 3`  -- 列出堆的各个区域的大小
2. `jstat -gcutil pid 1000 3` -- 列出堆的各个区域使用的比例
3. `jmap -heap pid`  -- 列出当前使用的GC算法，堆的各个区域大小



## Runtime data



### 查看线程的堆栈信息

1. `jstack -l pid`



### dump堆内的对象

1. `jmap -dump:live,format=b,file=xxx pid`
2. `-XX:+HeapDumpOnOutOfMemoryError -XX:HeapDumpPath=xxx`  -- 设置JVM参数，当JVM OOM时输出堆的dump



注意：

**ulimit  -c unlimited  -- 设置Linux ulimit参数，可以产生coredump且不受大小限制。之前在线上遇到过一个极其诡异的问题，JVM整个进程突然挂了，这时候依靠JVM本身生成dump文件已经不行了，只有依赖linux，让系统来生成进程挂掉的core dump文件**



### 查看java生成的汇编代码

* hsdis
* jitwatch



ref：

* [聊聊JVM（十）Mac下hsdis和jitwatch下载和使用](http://blog.csdn.net/iter_zc/article/details/41897137)
* [利用hsdis和JITWatch查看分析HotSpot JIT compiler生成的汇编代码](http://blog.csdn.net/hengyunabc/article/details/26898657)




一点小改动：

jitwatch.tar.gz解压后的jitwatch.sh改为：

```shell
# Absolute path to this script, e.g. /home/user/bin/foo.sh
SCRIPT=$(greadlink -f "$0")
# Absolute path this script is in, thus /home/user/bin
SCRIPTPATH=$(dirname "$SCRIPT")

JITWATCH_HOME="${SCRIPTPATH}/jitwatch-master/lib";
JITWATCH_JAR="${SCRIPTPATH}/jitwatch-1.0.0-SNAPSHOT.jar"
# echo $JITWATCH_HOME
${JAVA_HOME}/bin/java -cp $JAVA_HOME/lib/tools.jar:$JAVA_HOME/jre/lib/jfxrt.jar:$JITWATCH_JAR:$JITWATCH_HOME/hamcrest-core-1.3.jar:$JITWATCH_HOME/logback-classic-1.1.2.jar:$JITWATCH_HOME/logback-core-1.1.2.jar:$JITWATCH_HOME/slf4j-api-1.7.7.jar org.adoptopenjdk.jitwatch.launch.LaunchUI
```

> mac上依赖`brew install coreutils`
>
> linux上用readlink就行



使用中的小坑：

JIT sandbox里代码编辑器是根据class后面的内容作为`${className}.java`存储的，所以要写成以下风格。

```java
public class SandboxTest 
{  
```



## bytecode



### 查看java反编译出的java指令

* javap、
* IDE插件
  * IDEA的`Bytecode Viewer`



效果类似如下：

```java
// class version 52.0 (52)
// access flags 0x21
public class TestPrintClassLoading {
  // compiled from: TestPrintClassLoading.java

  // access flags 0x1
  public <init>()V
   L0
    LINENUMBER 6 L0
    ALOAD 0
    INVOKESPECIAL java/lang/Object.<init> ()V
    RETURN
   L1
    LOCALVARIABLE this LTestPrintClassLoading; L0 L1 0
    MAXSTACK = 1
    MAXLOCALS = 1

  // access flags 0x9
  public static main([Ljava/lang/String;)V
   L0
    LINENUMBER 9 L0
    RETURN
   L1
    LOCALVARIABLE args [Ljava/lang/String; L0 L1 0
    MAXSTACK = 0
    MAXLOCALS = 1
}
```



## class file



### 查看class file bytes结构

* JavaClassViewer

  ![](https://static.oschina.net/uploads/code/201012/08221854_Urqk.png)

  基本够用，更多参见 《java_class file.md》

* JBE（java bytecode editor）

  "全称，java bytecode editor。可以用于查看class文件结构、反汇编指令。反汇编后，指令以助记符的形式呈现，字节码就能够人肉阅读了。"

  [主页](http://www.cs.ioc.ee/~ando/jbe/)

  [下载-Java Bytecode Editor 0.1.1](http://www.cs.ioc.ee/~ando/dl.php?file=jbe/jbe-0.1.1.zip)

  * jbe.sh要改下，改成 `cd bin && java -cp ./ ...`
  * 不怎么好用，远没有JavaClassViewer详细，也没多出什么信息。 就是可以少许编辑（删除条目等）

* jasmin

  "Jasmin是一个字节码汇编器。开发者在文本文件中通过助记符编写指令，Jasmin能够将其汇编成class文件。"

  [主页](http://jasmin.sourceforge.net/)

  [sourceforge-Download](http://sourceforge.net/project/showfiles.php?group_id=100746)

  * 好像是命令行的，作用不是用来阅读而是生成

* javap

  * `javap -v ${classname}`

    打印的信息还很完整，比IDEA的`show bytecode`完整多了



### 



#### javap -v的输出

```java
javap -v TestPrintClassLoading3                                                                    master
Classfile /Users/zhihengfang/git_root/mine/java/test/src/main/java/TestPrintClassLoading3.class
  Last modified 2017-11-9; size 910 bytes
  MD5 checksum 91cb3edbad676b3b1f0b3098f791fbb7
  Compiled from "TestPrintClassLoading3.java"
public class TestPrintClassLoading3
  minor version: 0
  major version: 52
  flags: ACC_PUBLIC, ACC_SUPER
Constant pool:
   #1 = Methodref          #10.#39        // java/lang/Object."<init>":()V
   #2 = Fieldref           #4.#40         // TestPrintClassLoading3.o:Ljava/lang/Object;
   #3 = Class              #41            // java/lang/String
   #4 = Class              #42            // TestPrintClassLoading3
   #5 = Methodref          #4.#39         // TestPrintClassLoading3."<init>":()V
   #6 = Fieldref           #4.#43         // TestPrintClassLoading3.i:I
   #7 = Class              #44            // java/lang/IllegalStateException
   #8 = Class              #45            // java/lang/IllegalArgumentException
   #9 = Class              #46            // java/lang/Exception
  #10 = Class              #47            // java/lang/Object
  #11 = Utf8               i
  #12 = Utf8               I
  #13 = Utf8               s
  #14 = Utf8               Ljava/lang/String;
  #15 = Utf8               l
  #16 = Utf8               Ljava/lang/Long;
  #17 = Utf8               list
  #18 = Utf8               Ljava/util/List;
  #19 = Utf8               map
  #20 = Utf8               Ljava/util/Map;
  #21 = Utf8               Signature
  #22 = Utf8               Ljava/util/Map<Ljava/lang/String;Ljava/lang/Integer;>;
  #23 = Utf8               o
  #24 = Utf8               Ljava/lang/Object;
  #25 = Utf8               <init>
  #26 = Utf8               ()V
  #27 = Utf8               Code
  #28 = Utf8               LineNumberTable
  #29 = Utf8               main
  #30 = Utf8               ([Ljava/lang/String;)V
  #31 = Utf8               StackMapTable
  #32 = Class              #47            // java/lang/Object
  #33 = Class              #48            // "[Ljava/lang/String;"
  #34 = Class              #42            // TestPrintClassLoading3
  #35 = Class              #49            // java/lang/RuntimeException
  #36 = Class              #46            // java/lang/Exception
  #37 = Utf8               SourceFile
  #38 = Utf8               TestPrintClassLoading3.java
  #39 = NameAndType        #25:#26        // "<init>":()V
  #40 = NameAndType        #23:#24        // o:Ljava/lang/Object;
  #41 = Utf8               java/lang/String
  #42 = Utf8               TestPrintClassLoading3
  #43 = NameAndType        #11:#12        // i:I
  #44 = Utf8               java/lang/IllegalStateException
  #45 = Utf8               java/lang/IllegalArgumentException
  #46 = Utf8               java/lang/Exception
  #47 = Utf8               java/lang/Object
  #48 = Utf8               [Ljava/lang/String;
  #49 = Utf8               java/lang/RuntimeException
{
  public TestPrintClassLoading3();
    descriptor: ()V
    flags: ACC_PUBLIC
    Code:
      stack=1, locals=1, args_size=1
         0: aload_0
         1: invokespecial #1                  // Method java/lang/Object."<init>":()V
         4: return
      LineNumberTable:
        line 9: 0

  public static void main(java.lang.String[]);
    descriptor: ([Ljava/lang/String;)V
    flags: ACC_PUBLIC, ACC_STATIC
    Code:
      stack=3, locals=4, args_size=1
         0: aload_0
         1: iconst_0
         2: aaload
         3: astore_1
         4: getstatic     #2                  // Field o:Ljava/lang/Object;
         7: instanceof    #3                  // class java/lang/String
        10: ifeq          13
        13: new           #4                  // class TestPrintClassLoading3
        16: dup
        17: invokespecial #5                  // Method "<init>":()V
        20: astore_2
        21: aload_2
        22: dup
        23: getfield      #6                  // Field i:I
        26: iconst_1
        27: iadd
        28: putfield      #6                  // Field i:I
        31: goto          35
        34: astore_3
        35: goto          39
        38: astore_3
        39: return
      Exception table:
         from    to  target type
            21    31    34   Class java/lang/IllegalStateException
            21    31    34   Class java/lang/IllegalArgumentException
            21    35    38   Class java/lang/Exception
      LineNumberTable:
        line 18: 0
        line 19: 4
        line 23: 13
        line 26: 21
        line 28: 31
        line 27: 34
        line 30: 35
        line 29: 38
        line 31: 39
      StackMapTable: number_of_entries = 5
        frame_type = 252 /* append */
          offset_delta = 13
          locals = [ class java/lang/Object ]
        frame_type = 255 /* full_frame */
          offset_delta = 20
          locals = [ class "[Ljava/lang/String;", class java/lang/Object, class TestPrintClassLoading3 ]
          stack = [ class java/lang/RuntimeException ]
        frame_type = 0 /* same */
        frame_type = 66 /* same_locals_1_stack_item */
          stack = [ class java/lang/Exception ]
        frame_type = 0 /* same */
}
SourceFile: "TestPrintClassLoading3.java"
```





## data look into/analysis



### 获得coredump的线程堆栈信息

1. `jstack "$JAVA_HOME/bin/java" core.xxx > core.log`



## Stats data

























