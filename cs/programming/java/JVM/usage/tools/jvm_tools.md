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





## data look into/analysis



### 获得coredump的线程堆栈信息

1. `jstack "$JAVA_HOME/bin/java" core.xxx > core.log`



## Stats data

























