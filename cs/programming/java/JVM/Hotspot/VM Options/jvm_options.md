# overview



## 格式



```shell
java [ options ] class [ arguments ]
# 入口： class.main

java [ options ] -jar file.jar [ arguments ]
# 入口： jar中MANIFEST.MF的Main-Class项指定
```



* options

  pass to JVM itself

* arguments

  pass to JVM application



## 分类



* 标准选项
  * 系统属性 - `-D${property}=${value}`
* 非标准选项 - `-X`
  * 不稳定选项 - `-XX`



> 基本上都是-XX

* 高级运行时选项

  控制运行时的行为

* 高级JIT编译器选项

  控制JIT编译行为

* 高级serviceability选项

  收集系统信息，执行debug

* 高级GC选项

  



# list



## 标准选项



* `-client`

  client模式

* `-server`

  server模式

* `-agentlib:${libname}[=${options}]`

  启动时加载agentlib（通过特定查找机制查找libname指定的lib然后加载，如动态库机制）

  ```shell
  -agentlib:hprof=cpu=samples,interval=20,depth=3
  ```

* `-agentpath:${pathname}[=${options}]`

  启动时加载agentlib，指定路径方式

* `-classpath ${classpath}` or `-cp ${classpath}`

* `-D${property}=${value}`

  系统属性

* `-d32`

  32位环境运行

* `-d64`

  64位环境运行。 目前只有Hotspot server模式支持，`-server`隐式指定`-d64`而`-client`忽略`-d64`

* `-disableassertions[:${package name}"..." | :${class name} ]` or `-da[:${package name}"..." | :${class name} ]` - default

  禁用assertion，默认行为。

  ```shell
  java -ea:com.wombat.fruitbat... -da:com.wombat.fruitbat.Brickbat MyClass
  ```

* `-enableassertions[:${package name}"..." | :${class name} ]` or `-ea[:${package name}"..." | :${class name} ]`

  启用assertion

  ```shell
  java -ea:com.wombat.fruitbat... -da:com.wombat.fruitbat.Brickbat MyClass
  ```

* `-enablesystemassertions` or `-esa`

  启用系统类的assertion

* `-disablesystemassertions` or `-dsa`

  禁用...

* `-help` or `-?`

* `-jar`

  执行jar（以jar中类为入口）

* `-javaagent:${jarpath}[=${options}]`

  启动时加载javaagent

* `-jre-restrict-search`

  在版本搜索中包括用户专用 JRE，过时功能

* `-no-jre-restrict-search`

  在版本搜索中排除用户专用 JRE，过时功能

* `-showversion`

  show-and-continue

* `-splash:${imagepath}`

  ```shell
  -splash:images/splash.gif
  ```

* `-verbose` or `-verbose:class`

  print each classload

  输出看起来跟`TraceClassLoading`一样。

* `-verbose:gc`

  print each gc

* `-verbose:jni`

  打印JNI活动信息（JNI方法使用等）

* `-version`

  show-and-exit

  ```shell
  java -version                                                                 feature_diag-profile
  java version "1.8.0_121"
  Java(TM) SE Runtime Environment (build 1.8.0_121-b13)
  Java HotSpot(TM) 64-Bit Server VM (build 25.121-b13, mixed mode)
  ```

  

* `-version:release`

  指定应该选用的`java`命令的发行版（一定的格式）

  ```shell
  -version:"1.6.0_13 1.6* & 1.6.0_10+"
  ```

## 非标准选项

* `-X`

  打印所有`-X`选项

* `-Xint`

  运行在解释器模式（禁用JIT）

* `-Xbatch`

  禁用后台编译。 看描述会导致对方法的调用必须等编译完成（而不是先以解释模式执行）。

  等于`-XX:-BackgroundCompilation`

* `-Xbootclasspath:bootclasspath`

  set

  这几个bootclasspath参数都要求：

  > Do not deploy applications that use this option to override a class in `rt.jar`, because this violates the JRE binary code license.

* `-Xbootclasspath/a:path`

  append (after)

* `-Xbootclasspath/p:path`

  insert before

* `-Xcheck:jni`

  对JNI调用做额外的检查（尤其是参数检查），会降低性能。

  思路是： 快速失败好于不可预期的行为。

* `-Xcomp`

  `-Xint`的相反操作，禁用解释，强制在方法第一次调用时做JIT。

  > 默认：
  >
  > * `-client`会先解释执行1000次收集信息以便优先的编译
  > * `-server`…10000次
  >
  > 次数参数可以通过： `-XX:CompileThreshold`选项调整

* `-Xdebug`

  啥都不做，只为兼容老版本。 囧...

* `-Xdiag`

  打印额外的诊断信息。

* `-Xfuture`

  做更严格的class文件校验。

  > 默认会做一些容错处理。
  >
  > 而抛弃容错可以“确保”class更“面向未来”

* `-Xinternalversion`

  类似`-version`，输出更多的JVM内部信息。

  ```shell
  java -Xinternalversion                                                        feature_diag-profile
  Java HotSpot(TM) 64-Bit Server VM (25.121-b13) for bsd-amd64 JRE (1.8.0_121-b13), built on Dec 12 2016 20:39:32 by "java_re" with gcc 4.2.1 (Based on Apple Inc. build 5658) (LLVM build 2336.11.00)
  ```

* `-Xmaxjitcodesize=${size}`

  设置JIT编译代码的最大缓存大小，单位byte，可以加k/K,m/M,g/G单位。

  默认：

  * 一般： 240MB

  * 禁用分层编译（`-XX:-TieredCompilation`）： 48MB

    > 所谓的分层编译，就是一种折中方式，在系统执行初期，执行频率比较高的代码先被c1编译器编译，以便尽快进入编译执行，然后随着时间的推移，执行频率较高的代码再被c2编译器编译，以达到最高的性能。

* `-Xmixed`

  默认模式： 解释执行，然后对于热点方法编译执行

* `-Xnoclassgc`

  禁用类GC，可以节约一些GC时间，但会增加更多的“永久”内存占用，并且在一些场景可能带来OOM（比如大量/持续生成临时类的场景）

* `-Xprof`

  对程序做profile，并且把结果输出到stdout，主要用于开发而非生产。

* `-Xincgc`

* `-Xloggc:${file}`

* `-Xmn${size}` or `-XX:NewSize` + `-XX:MaxNewSize`

  同时设置初始和最大young区堆大小（也可以通过后面两个参数来分别设置（不同大小）），单位byte，可以指定其他单位。

  如：

  ```shell
  -Xmn256m
  -Xmn262144k
  -Xmn268435456
  ```

  

  见下面详解。

* `-Xms${size}`

  设置初始（整个）堆大小，单位是byte，可以指定其他单位。

  需要是1024的整数倍（也即最小粒度为K），并且`>1MB`。

  默认为： young区+old区。

  ```shell
  -Xms6291456
  -Xms6144k
  -Xms6m
  ```

* `-Xmx${size}`

  设置`内存分配池（memory allocation pool）`的最大值，单位为byte，可以指定其他单位。

  > java似乎是把java堆称为`内存分配池`/`内存池`

  需要是1024的整数倍（也即最小粒度为K），并且`>2MB`。

  默认会根据系统配置在运行时选择（确定）该值。

  对于`server`来说一般`-Xmx`和`-Xms`会设置同样的值。

  ```shell
  -Xmx83886080
  -Xmx81920k
  -Xmx80m
  ```

* `-Xrs`

  减少JVM本身对系统信号的使用（以便让应用自己来使用系统信号）。

  参见详解。

* `-Xshare:${mode}`

  设置CDS运行模式。

  可选模式有：

  * auto
  * on
  * off
  * dump

* `-XshowSettings:${category}`

  打印设置信息然后继续。 如果不带其他参数的化，无法继续则会再打印使用帮助然后退出。

  输出样例见详解。

  category可以是：

  * all
  * locale
  * properties
  * vm

* `-Xss${size}`

  设置线程栈大小，单位byte，可以指定其他单位。

  等效于 `-XX:ThreadStackSize`

  默认值：

  - Linux/ARM (32-bit): 320 KB
  - Linux/i386 (32-bit): 320 KB
  - Linux/x64 (64-bit): 1024 KB
  - OS X (64-bit): 1024 KB
  - Oracle Solaris/i386 (32-bit): 320 KB
  - Oracle Solaris/x64 (64-bit): 1024 KB

  如：

  ```shell
  -Xss1m
  -Xss1024k
  -Xss1048576
  ```

* `-Xusealtsigs`

  设置JVM使用其他信号来作为内部信号，默认为`SIGUSR1`,`SIGUSR2`

  等效于： `-XX:+UseAltSigs`

* `-Xverify:${mode}`

  不建议关闭，会影响安全性，并且使问题排查更复杂。

  可选mode有：

  * remote

    默认模式，对所有非bootstrapCL加载的bytecode进行校验。

  * all 校验所有bytecode

  * none 不校验所有bytecode



### 不稳定选项



#### 高级运行时选项

* `-XX:+CheckEndorsedAndExtDirs`

  检查当前是否在使用（生效）`endorsed-standards override`或`extension`机制，是的话则避免（怎么避免？直接exit？）。

  详见详解。

* `-XX:+DisableAttachMechanism`

  顾名思义，启用该选项会禁用attach机制（默认该选项是禁用的，也即attach机制是启用的），那么基于该机制的工具都将不可用（jcmd、jstack、jmap、jinfo等的部分/全部功能）。

* `-XX:ErrorFile=${filename}`

  指定*出现不可恢复的错误时记录错误数据的文件*的路径，默认为 `${cwd}/hs_err_pid${pid}.log` 不过提供fallback到 `sys tmp` 目录下的容错处理。

* `-XX:+FailOverToOldVerifier`

  当新的type checker校验失败时，fallback到老的校验器。

  只对老版本的bytecode有效。

  默认禁用。

* `-XX:+FlightRecorder`

  启用JRF，商用特性，需要和 `-XX:+UnlockCommercialFeatures`一起使用才有效。 

  默认不开启，可以运行时通过`jcmd`打开（显式禁用时除外）。

* `-XX:-FlightRecorder`

  显式禁用JFR，无法在运行时打开。

* `-XX:FlightRecorderOptions=${parameter}=${value}`

  JFR的参数，显然需要先启用JFR。

  可用参数：

  * `defaultrecording={true|false}`
  * `disk={true|false}`
  * `dumponexit={true|false}`
  * `dumponexitpath=${path}`
  * `globalbuffersize=${size}`
  * `loglevel={quiet|error|warning|info|debug|trace}`
  * `maxage=${time}`
  * `maxchunksize=${size}`
  * `maxsize=${size}`
  * `repository=${path}`
  * `samplethreads={true|false}`
  * `settings=${path}`
  * `stackdepth=${depth}`
  * `threadbuffersize=${size}`

  如：

  ```shell
  -XX:FlightRecorderOptions=defaultrecording=true,disk=true,maxchunksize=10M
  ```

* `-XX:LargePageSizeInBytes=${size}`

  用于**在solaris**上为java堆设置large page，单位是byte，可以指定其他单位。

  大小需要是2的幂次。

* `-XX:MaxDirectMemorySize=${size}`

  NIO direct-buffer的最大大小，单位是byte，可以指定其他单位。

  默认是0，表示由JVM自动选择一个值。

* `-XX:NativeMemoryTracking=${mode}`

  指定追踪JVM native memory使用情况的模式，可选模式如下：

  * off

    默认值。 不做追踪。

  * summary

    只对JVM子系统（如 java堆、类、代码、线程...）（对native mem）的使用做追踪。

  * detail

    除了JVM子系统外，还在单独`CallSite`、单独虚拟内存区域和提交区域 的层面进行追踪。

  参见 引申 - NativeMemoryTracking。

* `-XX:ObjectAlignmentInBytes=${alignment}`

  设置java对象的内存对齐方式，单位是byte。

  默认是8 bytes。

  需要是2的幂次，并且范围在`[8, 256]`。

* `-XX:OnError=${string}`

  指定在出现不可恢复的错误时执行的命令，可以是多个（分号分隔），如果有空格则需要用引号…。

  如：

  ```shell
  -XX:OnError="gcore %p;dbx - %p"  # %p为占位符，会用进程号替换
  ```

* `-XX:OnOutOfMemoryError=${string}`

  和`onError`类似，不过场景是： an `OutOfMemoryError` exception is first thrown

  TODO 整个生命周期中第一次抛出OOM异常还是？

  > 理解上是如果被catch了（静默处理等）然后下次再出现的话就不会调用了...

* `-XX:+PerfDataSaveToFile`

  启用特性： 在java应用退出时 将二进制格式`jstat`统计数据保存到文件 `${sys_tmp}/hsperfdata_${pid}`。

  jstat命令可以指定以该文件路径为参数来显示统计信息，如：

  ```shell
  jstat -class file:///<path>/hsperfdata_<pid>
  jstat -gc file:///<path>/hsperfdata_<pid>
  ```

  默认禁用。

  > 这里`${sys_tmp}/hsperfdata_${pid}` 是退出时的一份持久dump；
  >
  > 而`${sys_tmp}/hsperfdata_${username}/${pid}`是运行时的数据退出时应该会删除
  >
  > 数据是一样的，都是jstat binary。

  ```shell
  ${JAVA_HOME}/bin/jstat -gc file:////tmp/hsperfdata_admin/10248 500
   S0C    S1C    S0U    S1U      EC       EU        OC         OU       PC     PU    YGC     YGCT    FGC    FGCT     GCT
  512.0  512.0   48.9   0.0    4288.0   1511.6   10624.0     5678.1   21248.0 13407.2  20340   17.645   0      0.000   17.645
  512.0  512.0   48.9   0.0    4288.0   1511.6   10624.0     5678.1   21248.0 13407.2  20340   17.645   0      0.000   17.645
  ```

* `-XX:+PrintCommandLineFlags`

  启用特性： 打印一些有JVM（自行）选择的flags，便于了解实际运行参数。

  默认禁用 - 不打印。

  如：

  ```shell
  java -XX:+PrintCommandLineFlags
  -XX:InitialHeapSize=16262592 -XX:MaxHeapSize=260201472 -XX:+PrintCommandLineFlags -XX:+UseCompressedOops
  ```

* `-XX:+PrintNMTStatistics`

  启用特性： 在JVM退出时打印所收集的native mem追踪数据。

  前提： 已启用native mem追踪（`-XX:NativeMemoryTracking`）。

  默认： 禁用 - 不打印。

* `-XX:+RelaxAccessControlCheck`

  减少做class校验时对访问控制的检查。 对于*比较新*（`recent`）bytecode版本的class无效，也即只能对比较老版本的做“宽容对待”。

  默认： 禁用。

* `-XX:+ResourceManagement`

  启用运行时的资源管理。

  商业特性，需要先解锁（`-XX:+UnlockCommercialFeatures`）。

  参见详解-特性。

* `-XX:ResourceManagementSampleInterval=${value} (milliseconds)`

  设置资源管理功能的采样间隔，单位毫秒。

  前提： 开启资源管理功能（`-XX:+ResourceManagement`）

  默认为100ms。 设置0值等同于禁用，负值则会被默认值替代。

* `-XX:SharedArchiveFile=${path}`

  指定CDS存档文件的路径（应该前提是启用/使用CDS功能吧）。

* `-XX:SharedClassListFile=${file_name}`

  指定记录要打包到CDS存档的class文件的清单文件路径，内容类似：

  ```java
  java/lang/Object
  hello/Main
  ```

* `-XX:+ShowMessageBoxOnError`

  设置当JVM遇到不可恢复错误时显示对话框。

  这样会使得JVM不再继续推出，从而可以（来得及）attach上去做debug等操作。

  默认禁用。

* `-XX:StartFlightRecording=${parameter}=${value}`

  启动JFR，等同于运行时使用`JFR.start`。

  商用特性，需要先解锁（…）。

  可以指定参数：

  * `compress={true|false}`
  * `defaultrecording={true|false}`
  * `delay=${time}`
  * `dumponexit={true|false}`
  * `duration=${time}`
  * `filename=${path}`
  * `name=${identifier}`
  * `maxage=${time}`
  * `maxsize=${size}`
  * `settings=${path}`

  如：

  ```shell
  -XX:StartFlightRecording=filename=test.jfr,compress=true
  ```

* `-XX:ThreadStackSize=${size}`

  参见 `-Xss`

* `-XX:+TraceClassLoading`

  启用对类加载的追踪。

  默认禁用。

  以下加载追踪基于：

  * java

    ```java
    public class TestPrintClassLoading {
        public static void main(String[] args) {}
    }
    ```

  * bytecode

    ```Java
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

    

  会输出到stdout，内容类似如下：
  ```shell
  [Opened /Library/Java/JavaVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/jre/lib/rt.jar]
  [Loaded java.lang.Object from /Library/Java/JavaVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/jre/lib/rt.jar]
  [Loaded java.io.Serializable from /Library/Java/JavaVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/jre/lib/rt.jar]
  ```

* `-XX:+TraceClassLoadingPreorder`

  启用特性： 以引用顺序来追踪类加载过程。

  默认禁用。

  ```shell
  [Loading java.lang.Object from /Library/Java/JavaVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/jre/lib/rt.jar]
  [Loading java.lang.String from /Library/Java/JavaVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/jre/lib/rt.jar]
  [Loading java.io.Serializable from /Library/Java/JavaVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/jre/lib/rt.jar]
  [Loading java.lang.Comparable from /Library/Java/JavaVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/jre/lib/rt.jar]
  [Loading java.lang.CharSequence from /Library/Java/JavaVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/jre/lib/rt.jar]
  [Loading java.lang.Class from /Library/Java/JavaVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/jre/lib/rt.jar]
  ```

  和`-XX:+TraceClassLoading`一起使用

  ```shell
  [Opened /Library/Java/JavaVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/jre/lib/rt.jar]
  [Loading java.lang.Object from /Library/Java/JavaVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/jre/lib/rt.jar]
  [Loaded java.lang.Object from /Library/Java/JavaVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/jre/lib/rt.jar]
  [Loading java.lang.String from /Library/Java/JavaVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/jre/lib/rt.jar]
  [Loading java.io.Serializable from /Library/Java/JavaVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/jre/lib/rt.jar]
  [Loaded java.io.Serializable from /Library/Java/JavaVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/jre/lib/rt.jar]
  [Loading java.lang.Comparable from /Library/Java/JavaVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/jre/lib/rt.jar]
  [Loaded java.lang.Comparable from /Library/Java/JavaVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/jre/lib/rt.jar]
  [Loading java.lang.CharSequence from /Library/Java/JavaVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/jre/lib/rt.jar]
  [Loaded java.lang.CharSequence from /Library/Java/JavaVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/jre/lib/rt.jar]
  [Loaded java.lang.String from /Library/Java/JavaVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/jre/lib/rt.jar]
  [Loading java.lang.Class from /Library/Java/JavaVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/jre/lib/rt.jar]
  ...
  ```

* `-XX:+TraceClassResolution`

  启用对常量池resolutions的追踪。

  默认禁用。

  常量池resolution是个啥？

  ```shell
  RESOLVE java.io.Serializable java.lang.Object (super)
  RESOLVE java.lang.Comparable java.lang.Object (super)
  RESOLVE java.lang.CharSequence java.lang.Object (super)
  RESOLVE java.lang.String java.lang.Object (super)
  RESOLVE java.lang.String java.io.Serializable (interface)
  RESOLVE java.lang.String java.lang.Comparable (interface)
  RESOLVE java.lang.String java.lang.CharSequence (interface)
  ```

* `-XX:+TraceClassUnloading`

  启用对类卸载的追踪。

  默认禁用。

* `-XX:+TraceLoaderConstraints`

  启用对` loader constraints recording`的追踪。

  默认禁用。

  > 打印class的装载策略变化信息到stdout。
  >
  > 装载策略变化是实现classloader隔离/名称空间一致性的关键技术。
  >
  > TODO 

* `-XX:+UnlockCommercialFeatures`

  顾名思义…，当心法律风险。

* `-XX:+UseAltSigs`

  参见 `-Xusealtsigs`

* `-XX:+UseAppCDS`

  启用 `application class data sharing (AppCDS)`，需要在CDS dump期间以及应用运行时都同时指定 `-XX:SharedClassListFile` and `-XX:SharedArchiveFile`。

  商业特性，需要解锁（…）。

  实验特性，…。

* `-XX:-UseBiasedLocking`

  禁用偏向锁。

  偏向锁对于较多无竞争同步的场景会性能提升，而对于竞争较激烈场景则有额外性能开销。

  默认开启偏向锁。

* `-XX:-UseCompressedOops`

  禁用压缩指针。

  默认开启，在堆大小小于32GB时使用压缩指针，此时对象引用会以32-bit表示（而不是64），这样可以（在堆大小小于32GB场景）显著提高性能。

  只对64位JVM有效。

* `-XX:+UseHugeTLBFS`

  当预留内存时提前预分配所有大页，影响是 JVM无法动态增减大页内存区域。

  在linux上等效于`-XX:+UseLargePages`。

  默认禁用。

* `-XX:+UseLargePages`

  启用大页内存的使用。

  默认禁用 - 不使用大页内存。

* `-XX:+UseMembar`

  启用特性： 在线程状态变化时使用内存屏障。

  默认对于除ARM以外所有平台都禁用。（建议在ARM上不要禁用）

* `-XX:+UsePerfData`

  启用`perfdata`特性，会输出jstat统计数据到文件`${sys_tmp}/hsperfdata_${username}/${pid}`。

  jstat命令就是通过（指定文件或者根据pid查找文件）该文件来获取、输出数据。

  默认开启，可以通过 `-XX:-UsePerfData` 来禁用

* `-XX:+UseTransparentHugePages`

  在linux上会启用 *能自动增减的大页* 特性。

  当OS移动其他页来创建大页时可能带来性能影响（抖动）。

  实验特性。

* `-XX:+AllowUserSignalHandlers`

  允许应用安装信号处理器。

  默认禁用 - 不允许。


#### 高级JIT编译选项

* `-XX:+AggressiveOpts`

  启用激进的性能优化特性，这些特性可能在将来的版本里默认使用。

  默认禁用该选项 - 不使用实验的性能（优化）特性。

  > 比较着看了下，开启该选项后变化如下：
  > ```
  > -     bool AggressiveOpts                            = false                               {product}
  > +     bool AggressiveOpts                           := true                                {product}
  >
  > -     intx AutoBoxCacheMax                           = 128                                 {C2 product}
  > +     intx AutoBoxCacheMax                           = 20000                               {C2 product}
  >
  > -     intx BiasedLockingStartupDelay                 = 4000                                {product}
  > +     intx BiasedLockingStartupDelay                 = 500                                 {product}
  > ```
  > 也许还有一些其他没有体现在选项上的行为差异。


* `-XX:AllocateInstancePrefetchLines=${lines}`

  设置超出`instance allocation pointer`的预取行数。

  TODO 行数是指？ 怎么个预取法？

  TODO 了解一下prefetch到底是个啥 = =

  默认为1。

  只有hotspot server VM支持。

* `-XX:AllocatePrefetchDistance=${size}`

  `Sets the size (in bytes) of the prefetch distance for object allocation. Memory about to be written with the value of new objects is prefetched up to this distance starting from the address of the last allocated object. Each Java thread has its own allocation point.`

  TODO

  负数值表示根据平台来选择该值。 正数值表示预取的byte数（可以指定其他单位）。

  默认为-1。

  只有Hotspot server VM支持。

* `-XX:AllocatePrefetchInstr=${instruction}`

  设置`prefetch instruction to prefetch ahead of the allocation pointer.`。

  TODO

  允许值为`0-3`。 `The actual instructions behind the values depend on the platform`。

  默认为0。

  只有Hotspot server VM支持。

* `-XX:AllocatePrefetchLines=${lines}`

  `Sets the number of cache lines to load after the last object allocation by using the prefetch instructions generated in compiled code. `

  设置对象分配后使用prefetch指令加载的cache行数。 = = 

  TODO 不懂

  默认： 如果刚分配的对象时实例则为1； 是数组则为3。

  只有Hotspot server VM支持。

* `-XX:AllocatePrefetchStepSize=${size}`

  设置连续prefetch指令的步进大小，单位为byte，可以指定其他单位。

  默认为16bytes。

  只有Hotspot server VM支持。

* `-XX:AllocatePrefetchStyle=${style}`

  设置生成代码的prefetch指令的行为/风格。值为`0-3`，解释如下：

  * 0

    不生成prefetch指令。

  * 1

    *默认值*

    在每次分配内存（allocation）后都执行prefetch指令。

  * 2

    使用TLAB水印（watermark）指针来确定什么时候执行prefetch。

  * 3

    在SPARC平台使用BIS执行来做prefetch。

  只有Hotspot server VM支持。

* `-XX:+BackgroundCompilation`

  启用后台编译。

  默认启用，可以通过`-`禁用（等同于`-Xbatch`）。

* `-XX:CICompilerCount=${threads}`

  设置用于编译的线程数。

  默认： server JVM为2； client为1。 当启用分层编译的话会根据核数调整。

* `-XX:CodeCacheMinimumFreeSpace=${size}`

  设置编译需要的最小空间，当剩余空间小于该值时会停止编译。

  > 这部分空间是为那些非编译（得到）的code预留的，比如native adapter等。

  单位为byte，可以设置其他单位。

  默认为500KB。

* `-XX:CompileCommand=${command},${method}[,${option}]`

  以参数的形式指定JIT命令，告知其对于指定方法的编译处理。

  > 除了该选项指定的JIT命令外，还会尝试读取CompileCommandFile中指定的文件里的JIT命令

  其他见详解。

  如：

  ```shell
  -XX:CompileCommand=exclude,java/lang/String.indexOf
  # or 
  -XX:CompileCommand=exclude,java.lang.String::indexOf

  -XX:CompileCommand="exclude,java/lang/String.indexOf,(Ljava/lang/String;)I"

  -XX:CompileCommand=exclude,*.indexOf

  -XX:CompileCommand="exclude java/lang/String indexOf"
  ```

* `-XX:CompileCommandFile=${filename}`

  指定JIT编译器命令的配置/脚本文件路径。

  该文件内容的每行会表示一个JIT命令，如：

  ```shell
  print java/lang/String toString
  ```

  默认会读取 `.hotspot_compiler` 文件。

* `-XX:CompileOnly=${methods}`

  设置**只对**该选项中指定的方法进行编译。 通过完全限定名来标识。 （也支持`-XX:+PrintCompilation` and `-XX:+LogCompilation`输出的格式）

  不支持统配，不过支持部分匹配（包、包+类、方法名）。

  如：

  ```shell
  # 标准
  -XX:CompileOnly=java/lang/String.length,java/util/List.size

  # PrintCompilation/LogCompilation 输出的格式
  -XX:CompileOnly=java.lang.String::length,java.util.List::size

  # 到类
  -XX:CompileOnly=java/lang/String
  # 到包
  -XX:CompileOnly=java/lang
  # 只有方法名
  -XX:CompileOnly=.length
  ```

* `-XX:CompileThreshold=${invocations}`

  设置对方法执行编译之前先解释执行的次数门限值。

  默认： server JVM为10000； client JVM为15000。

  当启用分层编译时该选项会被忽略。

  可以通过`-Xcomp`禁用编译前的解释执行过程。

* `-XX:+DoEscapeAnalysis`

  启用逃逸分析。

  默认启用，可以通过`-`来禁用。

  只有Hotspot server VM支持。

* `-XX:InitialCodeCacheSize=${size}`

  设置JIT编译的code cache的初始大小。 单位为byte，可以指定其他单位。

  默认500KB，不能小于系统最小内存页大小。

* `-XX:+Inline`

  启用方法内联。

  默认启用。 可以通过`-`来禁用。

* `-XX:InlineSmallCode=${size}`

  设置编译方法被内联的大小门限值（小于...则...）。

  > 这里是指编译后的大小吧。

  单位是byte，可以指定其他单位。

  默认为35byte。

* `-XX:+LogCompilation`

  启用特性： 记录编译活动到日志文件`${cwd}/hotspot.log`，可以通过`-XX:LogFile`来指定其他路径。

  默认禁用 - 不记录…。 需要和`-XX:+UnlockDiagnosticVMOptions`一起使用才能生效。

* `-XX:MaxInlineSize=${size}`

  设置方法被内联的bytecode大小门限值（小于...则...）。

  单位是byte，可以指定其他单位。

  默认为35byte。

  > 跟MaxTrivialSize的区别是啥？ - 参见详解-特性-inline。
  >
  > 被认定为trivial的method永远被inline； 此外的，满足该条件的，在一定条件（调用次数统计等）下会inline

* `-XX:MaxNodeLimit=${nodes}`

  设置单方法编译时使用的最大node数量。

  默认为65000。

  TODO 了解下编译时node的概念功用。

  > 调低该值可以`迫使jit遇到大方法时放弃编译`。
  >
  > 适用场景： `jit的compiler thread申请不到内存。一般这种情况发生在要编译的方法过大，编译器又无法消除的情况`，此时可以`检查正在被编译的方法`

* `-XX:MaxTrivialSize=${size}`

  设置小方法体的方法被内联的bytecode大小门限值（小于...则...）。

  单位是byte，可以指定其他单位。

  默认为6byte。

* `-XX:+OptimizeStringConcat`

  启用String拼接操作的优化。

  默认开启，可以通过`-`来禁用。

  只有Hotspot server VM支持。

* `-XX:+PrintAssembly`

  打印汇编代码（编译bytecode 或者是使用的native代码）。 通过调用`disassembler.so`来实现。

  主要用于诊断。

  默认禁用，不打印。 前提是启用/解锁 `-XX:UnlockDiagnosticVMOptions` 。

* `-XX:+PrintCompilation`

  在每次编译一个方法时输出详细的诊断信息。

  默认禁用。 

  只是记录编译活动的话，可以用 `-XX:+LogCompilation`。

* `-XX:+PrintInlining`

  打印内联操作（决定），可以观察到哪些方法被内联了。

  默认禁用。 前提是启用 `-XX:+UnlockDiagnosticVMOptions`

* `-XX:ReservedCodeCacheSize=${size}`

  设置JIT编译代码的最大大小，单位是byte，可以指定其他单位。

  默认为240MB。 如果（通过`-XX:-TieredCompilation`）禁用分层编译的话则默认为48MB。

  最大值上限为2GB（超过会报错）。

  不能小于初始值（`-XX:InitialCodeCacheSize`）。

  等效于 `-Xmaxjitcodesize`


* `-XX:RTMAbortRatio=${abort_ratio}`

  决定做去优化的门限值（比例，单位为%）。

  当统计到RTM abort比例超过该值时，会对编译代码做去优化。

  默认50，需要先启用`-XX:+UseRTMDeopt`。


* `-XX:RTMRetryCount=${number_of_retries}`

  设置RTM🔐的重试次数（然后才fallback到普通的锁机制）。

  默认是5。

  需要启用`-XX:UseRTMLocking`才有意义。


* `-XX:-TieredCompilation`

  禁用分层编译。 

  默认启用分层编译。 （只有Hotspot server VM支持）


* `-XX:+UseAES`

  启用硬件层面的AES原生支持。

  只对于intel、AMD、SPARC硬件可用。

  > Intel Westmere (2010 and newer), AMD Bulldozer (2011 and newer), and SPARC (T4 and newer) 

  只有hotspot server VM支持。

  和`UseAESIntrinsics`一起使用。 都是默认启用。 可以分别通过`-XX:-UseAES -XX:-UseAESIntrinsics`来禁用。


* `-XX:+UseAESIntrinsics`


* `-XX:+UseCodeCacheFlushing`

  启用特性： 关闭编译器前flush code cache。

  默认启用，可以通过`-XX:-UseCodeCacheFlushing`来禁用。


* `-XX:+UseCondCardMark`

  启用特性： 在更新卡表前检查卡是否已经标记了

  默认禁用，只应 用于`machines with multiple sockets`（多核的意思吧？ 但这么理解的话不是默认启用更好吗？），会提高重度依赖并发操作的java应用的性能。

  只有Hotspot server VM支持。

  > ref：[聊聊JVM（一）相对全面的GC总结](http://blog.csdn.net/iter_zc/article/details/41746265)
  >
  > 使用这个参数的原因是在高并发的情况下，Card标记为脏的操作本身就存在着竞争，使用这个参数可以避免卡片被重复标记为脏，从而提高性能


* `-XX:+UseRTMDeopt`

  根据abort比例来自动调优RTM🔐，该比例由 `-XX:RTMAbortRatio`指定。

  如果abort掉的事务的比例超过该设置比例，会对对应（使用了RTM的）方法做去优化（重新编译）revert回普通的锁。

  默认禁用，前提是开启/设置`-XX:+UseRTMLocking`


* `-XX:+UseRTMLocking`

  设置： 对所有膨胀锁使用RTM（受限事务内存），失败时回退到普通的锁机制。

  默认禁用。 

  只在Hotspot server VM + 支持TSX（事务同步扩展）的X86 CPU上可用。

  见详解。


* `-XX:+UseSHA`

  启用SPARC架构下底层硬件对SHA加密的内嵌支持。 

  和`UseSHA1Intrinsics`, `UseSHA256Intrinsics`, and `UseSHA512Intrinsics`组合使用。 这些选项和该选项默认启用，只支持 SPARC（T4或者更新）架构上的Hotspot server VM。

  禁用：

  * 所有SHA底层支持： `-XX:-UseSHA`
  * 某个特定的SHA底层支持： 该...对应的`-`


* `-XX:+UseSHA1Intrinsics`

  启用对`SHA-1` 的底层支持。

  参见引申，下同。


* `-XX:+UseSHA256Intrinsics`

  启用对`SHA-224` and `SHA-256`的底层支持。


* `-XX:+UseSHA512Intrinsics`

  启用对`SHA-384` and `SHA-512`的底层支持。


* `-XX:+UseSuperWord`

  将标量操作（scalar op）转换为超字操作（superword op）。

  默认启用。可以通过`-`来禁用。

  只有Hotspot server VM支持。

  参见详解。


#### 高级Serviceability选项

* `-XX:+EntendedDTraceProbes`

  启用扩展的`dtrace`工具探针（会影响性能）。

  默认禁用该特性，也即只使用标准探针。

  TODO 确认下linux java的dtrace功能，linux目前所知不支持dtrace只是支持了类似dtrace的功能。

  > 应该是不支持 = =

* `-XX:+HeapDumpOnOutOfMemory`

  顾名思义，在OOM（`OutOfMemoryError`）时使用`HPROF`来dump java堆到*当前目录*（也可以使用`-XX:HeapDumpPath`来指定路径）。

  默认禁用。

* `-XX:HeapDumpPath=${path}`

  需要`-XX:+HeapDumpOnOutOfMemoryError`开启，辅助指定dump路径。

  默认路径：`${cwd}/java_pid${pid}.hprof`

  如：

  ```shell
  -XX:HeapDumpPath=./java_pid%p.hprof  # %p 为占位符会使用进程id代替

  -XX:HeapDumpPath=/var/log/java/java_heapdump.hprof
  ```

* `-XX:LogFile=${path}`

  指定log输出的文件路径，默认为 `${cwd}/${hotspot.log}`。

  > 从上下文只看到`LogCompiltion`的内容会被记录到该log

* `-XX:+PrintClassHistogram`

  启用特性： 在`SIGTERM`时打印类实例的柱状图。 效果类似： `jmap -histo` 或者 `jcmd ${pid} GC.class_histogram`。

  默认禁用。

* `-XX:+PrintConcurrentLocks`

  开启特性：在`SIGTERM`时打印`java.util.concurrent`锁。

  默认禁用。

* `-XX:+UnlockDiagnosticVMOptions`

  解锁那些用于诊断JVM的选项。

  默认禁用，也即这些选项不可用。

  见详解。

#### 高级GC选项

* `-XX:+AggressiveHeap`

  启用java堆优化，会根据内存和CPU设置一些对 *长时间运行的会密集分配内存的任务* 进行优化的参数。

  默认禁用 - 不做优化。

  见详解。

* `-XX:+AlwaysPreTouch`

  在JVM初始化（在进入main之前）时touch每个java堆的页，可以用于模拟长时间运行的任务 - 所有内存都在物理内存中 的场景。

  默认禁用。

* `-XX:+CMSClassUnloadingEnabled`

  当使用CMS时启用类卸载。

  默认启用，可以通过 `-XX:-CMSClassUnloadingEnabled`来禁用。

* `-XX:CMSExpAvgFactor=${percent}`

  Sets the percentage of time (0 to 100) used to weight the current sample when computing exponential averages for the concurrent collection statistics. 
  当进行并发回收统计，计算指数平均值时，对当前采样所用的权值（0-100）

  TODO 如何计算？ openjdk代码里没搜到使用场景 = =

  默认25%。

* `-XX:CMSInitiatingOccupancyFraction=${percent}`

  设置发起CMS GC的触发条件之 *老年代占用比例*。 负数表示使用`-XX:CMSTriggerRatio`来确定该值。

  默认为`-1`。

  > ```cpp
  > void ConcurrentMarkSweepGeneration::init_initiating_occupancy(intx io, uintx tr) {
  >   assert(io <= 100 && tr <= 100, "Check the arguments");
  >   if (io >= 0) {
  >     _initiating_occupancy = (double)io / 100.0;
  >   } else {
  >     _initiating_occupancy = ((100 - MinHeapFreeRatio) +
  >                              (double)(tr * MinHeapFreeRatio) / 100.0)
  >                             / 100.0;
  >   }
  > }
  > ```
  >
  > 

  > ```cpp
  > ConcurrentMarkSweepGeneration::used
  > // 看起来是CMS代也即老年代的使用比例
  > ```

* `-XX:+CMSScavengeBeforeRemark`

  启用CMS重标记（remark）步骤前的搜寻（scavenging）尝试。

  > ref：[关于 -XX:+CMSScavengeBeforeRemark，是否违背cms的设计初衷？- 知乎问题 igeng的回答](https://www.zhihu.com/question/61090975)
  > 在重新标记之前对年轻代做一次minor GC，这样yong gen中剩余待标记的对象数量相比gc之前势必下降很多(只剩下存活的obj，大量死亡的obj被GC干掉了)，剩余被视作“GC ROOTS”的对象数量骤减，如此Remark的工作量就少很多，重新标记的时间开销也会减少；当然**这里Remark减少的时间和YGC的时间开销要做一个权衡，根据实践结果选择是否要开启CMSScavengeBeforeRemark**

  默认禁用。

* `-XX:CMSTriggerRatio=${percent}`

  设置 当分配的内存到达`-XX:MinHeapFreeRatio`设置的值的该比例时触发CMS GC。
  > 可能理解有误，原文： `Sets the percentage (0 to 100) of the value specified by -XX:MinHeapFreeRatio that is allocated before a CMS collection cycle commences. `
  > 实际的公式： `((100 - MinHeapFreeRatio) +(double)(CMSTriggerRatio * MinHeapFreeRatio) / 100.0)/ 100.0`

  默认为80%。

  详见 引申-点-触发CMS GC

* `-XX:ConcGCThreads=${threads}`

  设置并发GC（方式）的并发度（GC线程数），默认是根据CPU数计算得到。

* `-XX:+DisableExplicitGC`

  禁止显式GC（即`System.gc()`）。

  默认禁用该选项，也即允许显式GC。

  > 即使禁止了，JVM自己还是可以在需要时触发。

* `-XX:+ExplicitGCInvokesConcurrent`

  启用通过显式调用（`System.gc()`）来触发并发GC。

  > 改变`System.gc()`的行为

  默认禁用，启用时需要和 `-XX:+UseConcMarkSweepGC` 一起使用。

  参见详解。

* `-XX:+ExplicitGCInvokesConcurrentAndUnloadsClasses`

  启用通过显式调用（`System.gc()`）来触发并发GC，并且在其中能卸载类。

  默认禁用，启用时需要和 `-XX:+UseConcMarkSweepGC` 一起使用。

* `-XX:G1HeapRegionSize=${size}`

  设置G1收集器使用的分区大小，会将java堆按该值分成多个分区。

  值范围可以是`1 MB ~ 32 MB`，默认会根据堆大小来确定。

* `-XX:+G1PrintHeapRegions`

  打印出G1分区分配会回收的信息。

  默认禁用。

* `-XX:G1ReservePercent=${percent}`

  设置G1中预留来用于减少晋升失败的堆内存比例。 取值范围为 `0 ~ 50`。

  默认为10%。


* `-XX:InitialHeapSize=${size}`

  设置初始堆大小（看起来跟`-Xms`一样啊，但文里没说，参见`-Xms`）。

* `-XX:InitialSurvivorRatio=${ratio}`

  设置`throughput GC`使用的 初始survivor空间比例。

  ```cpp
    product(uintx, InitialSurvivorRatio, 8,                                   \
            "Initial ratio of young generation/survivor space size")    
  ```

  参见详解。

* `-XX:InitiatingHeapOccupancyPercent=${percent}`

  设置并发GC触发条件之 *堆占用比例*（整个堆，而不是某个代的）。
  > 不是每个收集器都有这种行为，比如G1会(其他的不清楚 = =，CMS的触发条件之一是old gen的使用率)
  > 根据搜到的资料，暂时没看到其他收集器使用该参数

  默认为45%，0表示非停顿GC。

  > 0为什么表示非停顿GC？
  > ```cpp
  > size_t marking_initiating_used_threshold =
  >   (_g1->capacity() / 100) * InitiatingHeapOccupancyPercent;
  > // ...
  > if ((cur_used_bytes + alloc_byte_size) > marking_initiating_used_threshold) {
  > ```
  > 可见如果为0的话，每次都会为true，这里非停顿是指不用等待的意思吧

* `-XX:MaxGCPauseMillis=${time}`

  设置最大GC停顿时间，是一个软目标/承诺，JVM（只）会尽量达成。

  默认无该值（无目标）。

* `-XX:MaxHeapSize=${size}`

  参见 `-Xmx`。

* `-XX:MaxHeapFreeRatio=${percent}`

  设置最大堆空闲比例，超过该比例则会缩减堆大小。 

  默认为70%。

* `-XX:MaxMetaspaceSize=${size}`

  设置metaspace的最大大小。

  默认没有限制。

* `-XX:MaxNewSize=${size}`

  设置young gen（nursery）的最大代销。

  默认` is set ergonomically` （= = 怎么翻译？）。

  VS `-Xmn` 设置初始大小。

* `-XX:MaxTenuringThreshold=${threshold}`

  设置自适应GC中使用的最大`tenuring threshold`。

  最大为15。

  默认：对于并行（throughput）收集器为15；对于CMS为6

  > ` tenuring threshold`是啥？ 效果是什么？ 
  > 见详解。

* `-XX:MetaspaceSize=${size}`

  设置一个已分配的metaspace大小的门限值，第一次超过该值后会触发GC。（看文里，后面该值会根据使用的metadata的量来增减）。

  默认根据平台而定。

* `-XX:MinHeapFreeRatio=${percent}`

  设置GC后允许的堆最小空余比例，也即GC后如果堆空余比例小于该值的话，会扩大堆。

  默认为40%。

* `-XX:NewRatio=${ratio}`

  设置young代和old代的大小比例（`young/old`）。

  默认为2。

* `-XX:NewSize=${size}`

  参见`-Xmn`

* `-XX:ParallelGCThreads=${threads}`

  设置young和old的并行GC线程数。

  默认根据CPU核数而定。

* `-XX:+ParallelRefProcEnabled`

  启用并行引用处理信息。

  默认禁用。

  > remark阶段为单线程，

* `-XX:+PrintAdaptiveSizePolicy`

  打印自适应的分代大小调整信息。

  默认禁用。

  TODO 内容是？

* `-XX:+PrintGC`

  打印每次GC的信息。

* `-XX:+PrintGCApplicationConcurrentTime`

  打印上次停顿（如GC停顿）后过了多久。

  TODO 没懂，难道是并发GC结束时打印？

  默认停顿。

* `-XX:+PrintGCApplicationStoppedTime`

  打印每次停顿（如GC停顿）的持续时间。

  默认禁用。

* `-XX:+PrintGCDateStamps`

  打印每次GC的日期信息。

  默认禁用。

* `-XX:+PrintGCDetails`

  打印每次GC的详细信息。

  默认禁用。

* `-XX:+PrintGCTaskTimeStamps`

  打印每个单独GC工作线程任务的时间戳信息。

  默认禁用。

* `-XX:+PrintGCTimeStamps`

  打印每次GC的时间戳信息。

  默认禁用。

* `-XX:+PrintStringDeduplicationStatistics`

  打印详细的字符串去重统计数据。

  参考：`-XX:+UseStringDeduplication` 

  默认禁用。

* `-XX:+PrintTenuringDistribution`

  打印任期（**tenuring**）分布信息。

  如：

  ```shell
  Desired survivor size 48286924 bytes, new threshold 10 (max 10)
  - age 1: 28992024 bytes, 28992024 total  # 经历了最近一次扫描
  - age 2: 1366864 bytes, 30358888 total
  - age 3: 1425912 bytes, 31784800 total
  ...
  ```

  > 左右两个字段分别指：
  >
  > 1. 处于age n的对象的总大小
  > 2. 处于age <=n 的对象的总大小（也即当前和之前的累计值）

  默认禁用。

* `-XX:+ScavengeBeforeFullGC`

  启用在每次fullGC前先执行young gen的GC。

  默认启用。 可以通过`-XX:-ScavengeBeforeFullGC`来禁用

  > 建议不要禁用，因为该做法可以减少fullGC的负担（can reduce the number of objects reachable from the old generation space into the young generation space.）
  >
  > 上面这句没看懂，减少 old区引用的young区对象？
  >
  > 另一段说法：
  > ```
  > Setting  ScavengeBeforeFullGC  to  false  means that when a full GC occurs, the JVM will not perform a young GC before a full GC. That is usually a bad thing, since it means that garbage objects in the young generation (which are eligible for collection) can pre- vent objects in the old generation from being collected. Clearly there is (or was) a point in time when that setting made sense (at least for certain benchmarks), but the general re- commendation is not to change that flag.
  > ```
  > 从这里可以理解到： 指的是被young gen dead obj引用的old gen obj。 开启的好处时减少这部分obj； 而坏处时多一次停顿（young gc）。 需要综合考虑得失。

* `-XX:SoftRefLRUPolicyMSPerMB=${time}`

  设置软可达对象在最后一次被引用之后在堆里保持活跃的时间，单位ms。

  注意这个`perMB`，最后的时间的计算是`该值 * free-mem-in-heap-inMB`。

  默认为1s（1000ms）。

* `-XX:StringDeduplicationAgeThreshold=${threshold}`

  设置会被视为去重目标的字符串对象的*寿命* - 按照*逃过*GC的次数来计算。

  > 在到达此寿命之前晋升到old gen的字符串对象也会被视为去重目标

  默认为3.

* `-XX:SurvivorRatio=${ratio}`

  设置`eden区`和`survivor区`的比例。

  默认为8。

* `-XX:TargetSurvivorRatio=${percent}`

  设置 the desired percentage of survivor space (0 to 100) used after young garbage collection.

  默认为50（50%）。

  > 感觉这句话描述的乱七八糟啊
  >
  > 见详解。

* `-XX:TLABSize=${size}`

  设置初始TLAB大小，单位b，可以指定其他单位。 0表示由JVM自动确定。

* `-XX:+UseAdaptiveSizePolicy`

  使用自适应的各代大小划分。

  默认启用，可以通过 `-XX:-UseAdaptiveSizePolicy` 来禁用，然后显式指定。 参考`-XX:SurvivorRatio` 。

* `-XX:+UseCMSInitiatingOccupancyOnly`

  配置（全堆？old区？）堆占用比例作为触发CMS GC的唯一条件。

  默认禁用 - 也会使用其他。

* `-XX:+UseConcMarkSweepGC`

  配置在old区使用CMS。

  > throughput GC的latency不能满足要求时考虑CMS（或G1）

  启用时，自动启用 `-XX:+UseParNewGC` 。

  > JDK8中不建议组合使用 `-XX:+UseConcMarkSweepGC -XX:-UseParNewGC`
  >
  > > 可能是因为原先能和CMS组合使用的其他young gen GC已经不推荐组合使用了

  默认禁用 - 根据...自动选择...

* `-XX:+UseG1GC`

  配置使用G1 GC。

  > G1的target：
  >
  > * 大内存 - 6GB+
  > * 低GC停顿时间 - 0.5s-
  > * 吞吐量

  默认禁用 - 根据...自动选择...

* `-XX:+UseGCOverheadLimit`

  启用策略来限制OOM前JVM花在GC上的时间。

   当超过98%的时间用于GC而恢复的堆内存少于2%时 并行GC会抛出OOM异常。

  当堆小时，该特性可以用于避免程序长时间（没什么进展的）GC（而实际业务几乎停滞）的情况。

  默认启用。 可以通过 `-XX:-UseGCOverheadLimit` 来禁用。

* `-XX:+UseNUMA`

  启用NUMA下的针对性优化： 增加应用对于低延迟内存的使用。

  只有使用`-XX:+UseParallelGC`才可用。

  默认禁用 - 不做NUMA相关优化。

  > NUMA 参考： [NUMA架构的CPU -- 你真的用好了么？](http://cenalulu.github.io/linux/numa/)

* `-XX:+UseParallelGC`

  启用并行扫描GC（throughput收集器）来提高性能。

  启用该选项会自动启用`-XX:+UseParallelOldGC`（除非显式禁用）。

  默认禁用  - 根据机器配置和JVM类型来自动选择GC。

* `-XX:+UseParallelOldGC`

  设置对于full GC使用并行GC。

  启用该选项会自动启用 `-XX:+UseParallelGC`。

  默认禁用。

* `-XX:+UseParNewGC`

  设置对于young gen使用并行GC。

  默认禁用。 但当设置了`-XX:+UseConcMarkSweepGC`时会自动启用。

  在JDK8里不建议不带`-XX:+UseConcMarkSweepGC`而单独开启该选项。

* `-XX:+UseSerialGC`

  使用串行GC。

  > 串行GC适用于小而简单的应用。

  默认禁用 - 根据机器配置和JVM类型来自动选择GC。


* `-XX:+UseSHM`

  在linux上，使用共享内存来建立大页（large pages）。

  大页见详解-特性-large pages。


* `-XX:+UseStringDeduplication`

  开启字符串去重特性，（也许是满足一定条件的）相同字符串只存在一份。

  默认禁用，必须开启G1 GC才能启用。（`-XX:+UseG1GC`）。


* `-XX:+UseTLAB`

  在young区启用 `thread-local allocation blocks`，顾名思义，该方式（基本）无锁分配内存，可以大大提高创建新对象的速度。

  **默认启用**，禁用则使用 `-XX:-UseTLAB`



## Deprecated and Removed Options

* `-Xincgc`

  增量GC已经不推荐使用（甚至删掉了），所以这个选项也deprecated了。

  下面`*Incremental*`的也一样。

* `-Xrun${libname}`

  加载指定的debug/profile库。

  改用 `-agentlib`

* `-XX:CMDIncrementalDutyCycle=${percent}`

* `-XX:CMSIncrementalDutyCycleMin=${percent}`

* `-XX:+CMSIncrementalMode`

* `-XX:CMSIncrementalOffset=${percent}`

* `-XX:+CMSIncrementalPacing`

* `-XX:CMSIncrementalSafetyFactor=${percent}`

* `-XX:CMSInitiatingPermOccupancyFraction=${percent}`

  触发CMS GC的条件之 *永久代占用比例*。 参见下面具体解释。

  JDK8中deprecated。

* `-XX:MaxPermSize=${size}`

  因为pemGen -> metaSpace，改用`-XX:MaxMetaspaceSize`

* `-XX:PermSize=${size}`

  因为...，所以改用`-XX:MetaspaceSize`

* `-XX:+UseSplitVerifier`

  开启新特性：把class verify分成两个阶段： 类型引用（编译器完成）和类型检查（runtime完成）。

  在JDK8默认使用该特性并且无法关闭，所以删掉此选项。

* `-XX:+UseStringCache`

  TODO

  Enables caching of commonly allocated strings.

  JDK8中删掉了。




## “隐秘”选项 - 官方文档没介绍



* `-XX:+PrintInterpreter`

  需要和UnlockDiag一起使用:

  ```shell
  -XX:+UnlockDiagnosticVMOptions -XX:+PrintInterpreter
  ```

  输出类似：

  ```
  ----------------------------------------------------------------------
  Interpreter

  code size        =    137K bytes
  total space      =    255K bytes
  wasted space     =    118K bytes

  # of codelets    =    266
  avg codelet size =    530 bytes
  ```

----------------------------------------------------------------------
  slow signature handler  [0x0000000115e96640, 0x0000000115e96800]  448 bytes

  ...

----------------------------------------------------------------------
  iload_2  28 iload_2  [0x0000000115eaab60, 0x0000000115eaabc0]  96 bytes

    0x0000000115eaab60: push   %rax
    0x0000000115eaab61: jmpq   0x0000000115eaab90
    0x0000000115eaab66: sub    $0x8,%rsp
    0x0000000115eaab6a: vmovss %xmm0,(%rsp)
    0x0000000115eaab6f: jmpq   0x0000000115eaab90
    0x0000000115eaab74: sub    $0x10,%rsp
    0x0000000115eaab78: vmovsd %xmm0,(%rsp)
    0x0000000115eaab7d: jmpq   0x0000000115eaab90
    0x0000000115eaab82: sub    $0x10,%rsp
    0x0000000115eaab86: mov    %rax,(%rsp)
    0x0000000115eaab8a: jmpq   0x0000000115eaab90
    0x0000000115eaab8f: push   %rax
    0x0000000115eaab90: mov    -0x10(%r14),%eax
    0x0000000115eaab94: movzbl 0x1(%r13),%ebx
    0x0000000115eaab99: inc    %r13
    0x0000000115eaab9c: movabs $0x10e0ff040,%r10
    0x0000000115eaaba6: jmpq   *(%r10,%rbx,8)
    0x0000000115eaabaa: nopw   0x0(%rax,%rax,1)
    0x0000000115eaabb0: add    %al,(%rax)
    0x0000000115eaabb2: add    %al,(%rax)
    0x0000000115eaabb4: add    %al,(%rax)
    0x0000000115eaabb6: add    %al,(%rax)
    0x0000000115eaabb8: add    %al,(%rax)
    0x0000000115eaabba: add    %al,(%rax)
    0x0000000115eaabbc: add    %al,(%rax)
    0x0000000115eaabbe: add    %al,(%rax)
    
    ...
  ```

* `-XX:+PrintAssembly`

  需要和UnlockDiag一起使用:

  ```shell
  -XX:+UnlockDiagnosticVMOptions -XX:+PrintAssembly
  ```

  输出如下:

  ```shell
  ...
  Decoding compiled method 0x0000000104ede250:
  Code:
  [Entry Point]
  [Constants]
    # {method} {0x000000011cb36c60} 'length' '()I' in 'java/lang/String'
    #           [sp+0x40]  (sp of caller)
    0x0000000104ede3c0: mov    0x8(%rsi),%r10d
    0x0000000104ede3c4: shl    $0x3,%r10
    0x0000000104ede3c8: cmp    %rax,%r10
    0x0000000104ede3cb: jne    0x0000000104e22e20  ;   {runtime_call}
    0x0000000104ede3d1: data32 data32 nopw 0x0(%rax,%rax,1)
    0x0000000104ede3dc: data32 data32 xchg %ax,%ax
  [Verified Entry Point]
    0x0000000104ede3e0: mov    %eax,-0x14000(%rsp)
    0x0000000104ede3e7: push   %rbp
    0x0000000104ede3e8: sub    $0x30,%rsp
    0x0000000104ede3ec: movabs $0x11cd20528,%rax  ;   {metadata(method data for {method} {0x000000011cb36c60} 'length' '()I' in 'java/lang/String')}
    0x0000000104ede3f6: mov    0xdc(%rax),%edi
    0x0000000104ede3fc: add    $0x8,%edi
    0x0000000104ede3ff: mov    %edi,0xdc(%rax)
    0x0000000104ede405: movabs $0x11cb36c60,%rax  ;   {metadata({method} {0x000000011cb36c60} 'length' '()I' in 'java/lang/String')}
    0x0000000104ede40f: and    $0x1ff8,%edi
    0x0000000104ede415: cmp    $0x0,%edi
    0x0000000104ede418: je     0x0000000104ede434  ;*aload_0
                                                  ; - java.lang.String::length@0 (line 623)

    0x0000000104ede41e: mov    0xc(%rsi),%eax
    0x0000000104ede421: shl    $0x3,%rax          ;*getfield value
                                                  ; - java.lang.String::length@1 (line 623)

    0x0000000104ede425: mov    0xc(%rax),%eax     ;*arraylength
                                                  ; - java.lang.String::length@4 (line 623)
                                                  ; implicit exception: dispatches to 0x0000000104ede448
    0x0000000104ede428: add    $0x30,%rsp
    0x0000000104ede42c: pop    %rbp
    0x0000000104ede42d: test   %eax,-0x2efe333(%rip)        # 0x0000000101fe0100
                                                  ;   {poll_return}
    0x0000000104ede433: retq   
    0x0000000104ede434: mov    %rax,0x8(%rsp)
    0x0000000104ede439: movq   $0xffffffffffffffff,(%rsp)
    0x0000000104ede441: callq  0x0000000104edaf60  ; OopMap{rsi=Oop off=134}
                                                  ;*synchronization entry
                                                  ; - java.lang.String::length@-1 (line 623)
                                                  ;   {runtime_call}
    0x0000000104ede446: jmp    0x0000000104ede41e
    0x0000000104ede448: callq  0x0000000104ed6a40  ; OopMap{off=141}
                                                  ;*arraylength
                                                  ; - java.lang.String::length@4 (line 623)
                                                  ;   {runtime_call}
    0x0000000104ede44d: nop
    0x0000000104ede44e: nop
    0x0000000104ede44f: mov    0x2a8(%r15),%rax
    0x0000000104ede456: movabs $0x0,%r10
    0x0000000104ede460: mov    %r10,0x2a8(%r15)
    0x0000000104ede467: movabs $0x0,%r10
    0x0000000104ede471: mov    %r10,0x2b0(%r15)
    0x0000000104ede478: add    $0x30,%rsp
    0x0000000104ede47c: pop    %rbp
    0x0000000104ede47d: jmpq   0x0000000104e494a0  ;   {runtime_call}
    ...
  ```

  ​



### develop/notproduct选项 - 只用于debug版本JVM

* `-XX:+CountBytecodes` - develop

  ```shell
  > java -XX:+CountBytecodes HelloWorld
  Hello World
  474030 bytecodes executed in 0.9s (0.501MHz)
  [BytecodeCounter::counter_value = 474030]
  ```

* `-XX:+PrintBytecodeHistogram` - develop

  ```shell
  > java -XX:+PrintBytecodeHistogram HelloWorld
  Hello World
  Histogram of 473923 executed bytecodes:

    absolute  relative  code    name
  -----------------------------------------
       59653    12.59%    15    iload
       40571     8.56%    2a    aload_0
       29706     6.27%    84    iinc
       24089     5.08%    b4    getfield
  ...
  ```

* `-XX:+TraceBytecodes` - develop

  ```shell
  > java -XX:+TraceBytecodes HelloWorld

  [31744] static void java.lang.Object.()
  [31744]        1     0  invokestatic 17 <java/lang/Object.registerNatives()V> 
  [31744]        2     3  return

  [31744] static void java.lang.String.()
  [31744]        3     0  iconst_0
  [31744]        4     1  anewarray java/io/ObjectStreamField
  [31764]        5     4  putstatic 399 <java/lang/String.serialPersistentFields/[Ljava/io/ObjectStreamField;>
  ...
  ```

* `-XX:+PrintOptoAssembly` - develop






# 详解



## 非标准选项



### 不稳定选项



#### `-XX:CMSInitiatingPermOccupancyFraction=${percent}` - 触发CMS GC的条件之 *永久代占用比例*。 





参见： [PermGen大了也不行 - R大](http://rednaxelafx.iteye.com/blog/1108439)



```cpp
    // The field "_initiating_occupancy" represents the occupancy percentage  
    // at which we trigger a new collection cycle.  Unless explicitly specified  
    // via CMSInitiating[Perm]OccupancyFraction (argument "io" below), it  
    // is calculated by:  
    //  
    //   Let "f" be MinHeapFreeRatio in  
    //  
    //    _intiating_occupancy = 100-f +  
    //                           f * (CMSTrigger[Perm]Ratio/100)  
    //   where CMSTrigger[Perm]Ratio is the argument "tr" below.  
    //  
    // That is, if we assume the heap is at its desired maximum occupancy at the  
    // end of a collection, we let CMSTrigger[Perm]Ratio of the (purported) free  
    // space be allocated before initiating a new collection cycle.  
    //  
    void ConcurrentMarkSweepGeneration::init_initiating_occupancy(intx io, intx tr) {  
      assert(io <= 100 && tr >= 0 && tr <= 100, "Check the arguments");  
      if (io >= 0) {  
        _initiating_occupancy = (double)io / 100.0;  
      } else {  
        _initiating_occupancy = ((100 - MinHeapFreeRatio) +  
                                 (double)(tr * MinHeapFreeRatio) / 100.0)  
                                / 100.0;  
      }  
    }  

    _cmsGen ->init_initiating_occupancy(CMSInitiatingOccupancyFraction, CMSTriggerRatio);  
    _permGen->init_initiating_occupancy(CMSInitiatingPermOccupancyFraction, CMSTriggerPermRatio);  
```



默认值：

```shell
    $ jinfo -flag MinHeapFreeRatio `pgrep -u admin java`  
    -XX:MinHeapFreeRatio=40  
    $ jinfo -flag CMSTriggerPermRatio `pgrep -u admin java`  
    -XX:CMSTriggerPermRatio=80  
    $ jinfo -flag CMSInitiatingPermOccupancyFraction `pgrep -u admin java`  
    -XX:CMSInitiatingPermOccupancyFraction=-1  
```



#### `-Xmn${size}`



同时设置初始和最大young区堆大小（也可以通过后面两个参数来分别设置（不同大小）），单位byte，可以指定其他单位。



young区用来创建新对象，GC比较频繁（比其他区频繁的多）。

如果young区过小，会有大量的minor GC；而如果过大，则只会进行full GC持续时间较长。

> TODO 为什么过大则只会进行full GC？
> 有个说法是： `a larger young generation implies a smaller tenured generation, which will increase the frequency of major collections`
> 自己还一个理解是： large young gen可能(大大)减少因young gen分配失败而触发的young gc，相对而言full GC比例增加。



建议保持在整个堆的`1/2 ~ 1/4`。



#### `-Xrs`

减少JVM本身对系统信号的使用（以便让应用自己来使用系统信号）。



JVM对信号的使用：

* SIGQUIT

  根据条件：

  * dump thread
  * init attach procedure

* SIGHUP, SIGINT, SIGTERM

  发起对JVM shutdown hooks的调用。

  > 也即把这几种信号封装到JVM shutdown机制中作为触发条件。
  >
  > 标准化shutdown流程 - 通过hooks来完成cleanup。



使用该参数后，以上信号处理会失效：

* SIGQUIT JVM默认处理失效，对应的threadDump和attach功能失效

* 需要自行调用shutdown hooks（如果还希望hooks生效的话）

  应该是自行install signal handler然后...



#### `-XshowSettings:${category}`



打印设置信息然后继续。 如果不带其他参数的化，无法继续则会再打印使用帮助然后退出。



category可以是：

- all
- locale
- properties
- vm



输出样例：

```shell
java -XshowSettings:all                                                       feature_diag-profile
VM settings:
    Max. Heap Size (Estimated): 3.56G
    Ergonomics Machine Class: server
    Using VM: Java HotSpot(TM) 64-Bit Server VM

Property settings:
    awt.toolkit = sun.lwawt.macosx.LWCToolkit
    file.encoding = UTF-8
    file.encoding.pkg = sun.io
    file.separator = /
    gopherProxySet = false
    java.awt.graphicsenv = sun.awt.CGraphicsEnvironment
    java.awt.printerjob = sun.lwawt.macosx.CPrinterJob
    java.class.path = .
    java.class.version = 52.0
    java.endorsed.dirs = /Library/Java/JavaVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/jre/lib/endorsed
    java.ext.dirs = /Users/${USER}/Library/Java/Extensions
        /Library/Java/JavaVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/jre/lib/ext
        /Library/Java/Extensions
        /Network/Library/Java/Extensions
        /System/Library/Java/Extensions
        /usr/lib/java
    java.home = /Library/Java/JavaVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/jre
    java.io.tmpdir = /var/folders/cn/gg8cyzv15m9dxlgt8rv_c_sr0000gn/T/
    java.library.path = /Users/${USER}/Library/Java/Extensions
        /Library/Java/Extensions
        /Network/Library/Java/Extensions
        /System/Library/Java/Extensions
        /usr/lib/java
        .
    java.runtime.name = Java(TM) SE Runtime Environment
    java.runtime.version = 1.8.0_121-b13
    java.specification.name = Java Platform API Specification
    java.specification.vendor = Oracle Corporation
    java.specification.version = 1.8
    java.vendor = Oracle Corporation
    java.vendor.url = http://java.oracle.com/
    java.vendor.url.bug = http://bugreport.sun.com/bugreport/
    java.version = 1.8.0_121
    java.vm.info = mixed mode
    java.vm.name = Java HotSpot(TM) 64-Bit Server VM
    java.vm.specification.name = Java Virtual Machine Specification
    java.vm.specification.vendor = Oracle Corporation
    java.vm.specification.version = 1.8
    java.vm.vendor = Oracle Corporation
    java.vm.version = 25.121-b13
    line.separator = \n
    os.arch = x86_64
    os.name = Mac OS X
    os.version = 10.12.3
    path.separator = :
    sun.arch.data.model = 64
    sun.boot.class.path = /Library/Java/JavaVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/jre/lib/resources.jar
        /Library/Java/JavaVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/jre/lib/rt.jar
        /Library/Java/JavaVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/jre/lib/sunrsasign.jar
        /Library/Java/JavaVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/jre/lib/jsse.jar
        /Library/Java/JavaVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/jre/lib/jce.jar
        /Library/Java/JavaVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/jre/lib/charsets.jar
        /Library/Java/JavaVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/jre/lib/jfr.jar
        /Library/Java/JavaVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/jre/classes
    sun.boot.library.path = /Library/Java/JavaVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/jre/lib
    sun.cpu.endian = little
    sun.cpu.isalist =
    sun.io.unicode.encoding = UnicodeBig
    sun.java.launcher = SUN_STANDARD
    sun.jnu.encoding = UTF-8
    sun.management.compiler = HotSpot 64-Bit Tiered Compilers
    sun.os.patch.level = unknown
    user.country = CN
    user.dir = /Users/${USER}/git_root/ali_work/middleware/edas
    user.home = /Users/${USER}
    user.language = zh
    user.name = ${USER}
    user.timezone =

Locale settings:
    default locale = 中文
    default display locale = 中文 (中国)
    default format locale = 中文 (中国)
    available locales = , ar, ar_AE, ar_BH, ar_DZ, ar_EG, ar_IQ, ar_JO,
        ar_KW, ar_LB, ar_LY, ar_MA, ar_OM, ar_QA, ar_SA, ar_SD,
        ar_SY, ar_TN, ar_YE, be, be_BY, bg, bg_BG, ca,
        ca_ES, cs, cs_CZ, da, da_DK, de, de_AT, de_CH,
        de_DE, de_GR, de_LU, el, el_CY, el_GR, en, en_AU,
        en_CA, en_GB, en_IE, en_IN, en_MT, en_NZ, en_PH, en_SG,
        en_US, en_ZA, es, es_AR, es_BO, es_CL, es_CO, es_CR,
        es_CU, es_DO, es_EC, es_ES, es_GT, es_HN, es_MX, es_NI,
        es_PA, es_PE, es_PR, es_PY, es_SV, es_US, es_UY, es_VE,
        et, et_EE, fi, fi_FI, fr, fr_BE, fr_CA, fr_CH,
        fr_FR, fr_LU, ga, ga_IE, hi, hi_IN, hr, hr_HR,
        hu, hu_HU, in, in_ID, is, is_IS, it, it_CH,
        it_IT, iw, iw_IL, ja, ja_JP, ja_JP_JP_#u-ca-japanese, ko, ko_KR,
        lt, lt_LT, lv, lv_LV, mk, mk_MK, ms, ms_MY,
        mt, mt_MT, nl, nl_BE, nl_NL, no, no_NO, no_NO_NY,
        pl, pl_PL, pt, pt_BR, pt_PT, ro, ro_RO, ru,
        ru_RU, sk, sk_SK, sl, sl_SI, sq, sq_AL, sr,
        sr_BA, sr_BA_#Latn, sr_CS, sr_ME, sr_ME_#Latn, sr_RS, sr_RS_#Latn, sr__#Latn,
        sv, sv_SE, th, th_TH, th_TH_TH_#u-nu-thai, tr, tr_TR, uk,
        uk_UA, vi, vi_VN, zh, zh_CN, zh_HK, zh_SG, zh_TW

用法: java [-options] class [args...]
# ...
有关详细信息, 请参阅 http://www.oracle.com/technetwork/java/javase/documentation/index.html。
```



#### `-XX:+CheckEndorsedAndExtDirs`

检查当前是否在使用（生效）`endorsed-standards override`或`extension`机制，是的话则避免（怎么避免？直接exit？）。

会检查如下内容：

- 是否配置系统属性`java.ext.dirs` or `java.endorsed.dirs` 

  endorse没配置的话则会使用默认目录`lib/endorsed`

- `lib/endorsed`目录是否存在 && 不为空

- `lib/ext`是否有JDK自带以外的jar包

- 系统范围内特定平台支持的扩展目录中是否有JAR包



##### endorse机制

ref： [Java Endorsed Standards Override Mechanism](http://docs.oracle.com/javase/8/docs/technotes/guides/standards/index.html)

从JDK8起**This feature is deprecated and will be removed in a future release.**

* 目的

  让一些跟随JDK发版的库可以“更新”

* 做法

  允许override

* 约束

  需要实现`Endorsed Standards or Standalone Technologies`

  该标准定义了：

  * 版本： 只能高版本覆盖低版本
  * 白名单： 只有ref里列出的包路径才能覆盖



##### extension机制

ref：

* [Trail: The Extension Mechanism](http://docs.oracle.com/javase/tutorial/ext/)
* [Java标准教程：Extension架构](http://blog.163.com/among_1985/blog/static/27500523201001193040226/)

一句话说就是： 把非JDK jar包丢到ext目录下使得默认的类加载机制可以加载到（也不用用`-classpath`指定）。

* 默认的ext路径`${JAVA_HOME}/lib/ext`
* `java.ext.dirs`指定



#### `-XX:ObjectAlignmentInBytes=${alignment}`

设置java对象的内存对齐方式，单位是byte。

默认是8 bytes。

需要是2的幂次，并且范围在`[8, 256]`。

该选项使得可以use compressed pointers with large Java heap sizes.

堆大小限制：`4GB * ObjectAlignmentInBytes`

> 不过该值越大，对象间的未使用空间也会变大，从而可能会抵消using compressed pointers with large Java heap sizes的好处



#### `-XX:InitialSurvivorRatio=${ratio}`

设置`throughput GC`使用的 初始survivor空间比例。

> 该收集器通过`-XX:+UseParallelGC` and/or `-XX:+UseParallelOldGC`启用

默认会在该初始值的基础上，使用自适应的值（= = ），根据应用行为来调整大小。 而如果使用 `-XX:-UseAdaptiveSizePolicy` 禁用自适应值的话，则需要使用 `-XX:SurvivorRatio` 来指定整个生存期使用的值。



用于计算自适应值的公式：

> S = Y / (R + 2)
>
> * S = survivor space
> * Y = size of young gen
> * R = initial survivor space ratio
> * 2 = 2个survivor space
>
> 可以看到R越大实际的初始值越小



代码里只看到PS用到了该选项，其他collector应该是不能设置类似的参数，原因可能是其他collector不支持动态的survivor大小吧，所以”初始值“就没意义



#### `-XX:SoftRefLRUPolicyMSPerMB=${time}`

设置软可达对象在最后一次被引用之后在堆里保持活跃的时间，单位ms。

注意这个`perMB`，最后的时间的计算是`该值 * free-mem-in-heap-inMB`。

默认为1s（1000ms）。



注意不同模式下行为差别：

* client模式

  JVM倾向于flush软引用（而不是增长堆）


* server模式

  JVM倾向于增长堆（而不是flush软引用）

所以： server模式下`-Xmx`会显著的影响软引用多久被GC





#### `-XX:+UseRTMLocking`

设置： 对所有膨胀锁使用RTM（受限事务内存），失败时回退到普通的锁机制。

默认禁用。 

只在Hotspot server VM + 支持TSX（事务同步扩展）的X86 CPU上可用。



**简而言之就是： 底层硬件提供的 粗粒度的 乐观锁**



TSX

> X86指令扩展，为多线程程序提供便利。

RTM

> intel TSX的一部分。
>
> 提供了新的指令：
>
> - XBEGIN
>
>   XBEING + XEND成对使用，（对于之间的一段代码）开启事务。 如果没有冲突的话，内存和寄存器修改会（在XEND时）一起提交。
>
> - XABORT
>
>   用于显式中止事务
>
> - XEND
>
>   提交事务
>
> - XTEST
>
>   原文： `the XEND instruction to check if a set of instructions are being run in a transaction.`
>
>   应该是笔误吧，应该指的是XTEST。



- Available in all x86 modes
- **Some instructions and events may cause aborts**
  - Uncommon instructions, **interrupts**, faults, etc.
  - Always functionally safe to use any instruction
- Software must provide a non-transactional path
  - HLE: Same software code path executed without elision
  - RTM: Software fallback handler must provide alternate path



#### `-XX:CompileCommand=${command},${method}[,${option}]`

以参数的形式指定JIT命令，告知其对于指定方法的编译处理。

对方法的标识需要使用完全限定名（标准风格或者`-XX:+PrintCompilation` and `-XX:+LogCompilation`输出的风格）。 可以精确到方法签名（参数），如果没指定签名的话会匹配到所有同名方法。

可以使用`*`进行统配。

多个命令可以选择重复该选项或者在一个选项中`\n`分隔多个。

> 除了该选项指定的JIT命令外，还会尝试读取CompileCommandFile中指定的文件里的JIT命令

如：

```shell
-XX:CompileCommand=exclude,java/lang/String.indexOf
# or 
-XX:CompileCommand=exclude,java.lang.String::indexOf

-XX:CompileCommand="exclude,java/lang/String.indexOf,(Ljava/lang/String;)I"

-XX:CompileCommand=exclude,*.indexOf

-XX:CompileCommand="exclude java/lang/String indexOf"
```



##### 支持的命令

- break

  设置断点，当debug JVM时会停顿在对指定方法的编译操作开始处。

- compileonly

  白名单，只编译指定的方法。

  等效于`-XX:CompileOnly`，不过后者支持多个方法。

- dontinline

  黑名单，不内联指定方法。

- exclude

  黑名单，不编译指定方法。

- help

  打印该选项的帮助信息。

- inline

  尝试内联指定方法。

- log

  只对指定方法的编译进行日志（参考`-XX:+LogCompilation`，默认当该选项开启时会记录所有方法的编译活动）。

- option

  用于传递一个JIT编译选项给指定的方法。

  如：

  ```shell
  -XX:CompileCommand=option,java/lang/StringBuffer.append,BlockLayoutByFrequency
  ```

  会启用该方法的`BlockLayoutByFrequency`选项。

  方法的选项？
  > 网上能搜到的信息较少，翻看代码得知： 
  > 编译选项有global level和method level，综合两者得到最终值，参见 `class Compile : public Phase` 的字段
  > `Compile -> bool          method_has_option(const char * option)` 方法会提取method level的编译选项，target是`ciMethod*             _method;                // The method being compiled.`
  >
  > `BlockLayoutByFrequency`: True if we intend to do frequency based block layout。 编原的东西不是很懂，参见 [JDK-6743900 - frequency based block layout](https://bugs.openjdk.java.net/browse/JDK-6743900)

  可以逗号或者空格来分隔多个选项。

- print

  打印指定方法编译后得到的汇编代码。

- quiet

  不打印编译命令。

  默认会打印该选项指定的所有命令，类似：

  ```
  CompilerOracle: exclude java/lang/String.indexOf
  ```

  


相关的代码见 [compilerOracle.cpp#l317](http://hg.openjdk.java.net/jdk8u/jdk8u/hotspot/file/b4bdf3484720/src/share/vm/compiler/compilerOracle.cpp#l317)



#### `-XX:+UseSuperWord`

将标量操作（scalar op）转换为超字操作（superword op）。

默认启用。可以通过`-`来禁用。

只有Hotspot server VM支持。



##### 标量操作 和 超字操作的概念

>**by 雅神**
>
>标量操作就是字面意思，超字操作比如循环展开+手工依赖破除这样的矢量化优化
>
>换个例子
>
>```c
>for (int idx = 0; idx < 64; idx++) diff += abs(lhs[idx] - rhs[idx]);
>```
>
>这是个标量累加对吧
>实际上这个循环里全都是数据依赖（被diff带进来了）
>所以往下会做几个优化，让原先压在一个字（diff）上的操作，变成从数个字上的中间结果合并来的操作，提高整个循环的指令集并行度
>这里做循环展开+中间结果拆分；我们假定四字一阵发（实际未必）
>
>```c
>for (int idx = 0; idx < 64; idx += 4) {
>    int diff0 = abs(lhs[idx] - rhs[idx]);
>    int diff1 = abs(lhs[idx + 1] - rhs[idx + 1]);
>    int diff2 = abs(lhs[idx + 2] - rhs[idx + 2]);
>    int diff3 = abs(lhs[idx + 3] - rhs[idx + 3]);
>    diff += (diff1 + diff2) + (diff3 + diff4);
>}
>```
>
>循环体内前四行的数据依赖被破除，在阵发的基础上也许能更并行地执行
>第五行试图强迫编译器对结果加和做一个拆分（分别算两组+，再+起来），同样试图提高并行度
>所谓超字，在我的理解里，就是这类处理“把一个机器字上的操作，拆分成超过一个机器字的中间结果，以提高指令并行度”的优化
>至于所谓矢量，就是数学上的矢量，说白了到实现上多数都是数组



#### `-XX:+AggressiveHeap`

  启用java堆优化，会根据内存和CPU设置一些对 *长时间运行的会密集分配内存的任务* 进行优化的参数。

  默认禁用 - 不做优化。

 

> The -XX:+AggressiveHeap option inspects the machine resources (size of memory and number of processors) and attempts to set various parameters to be optimal for long-running, memory allocation-intensive jobs. 
>
> It was originally intended for machines with large amounts of memory and a large number of CPUs, but in the J2SE platform, version 1.4.1 and later it has shown itself to be useful even on four processor machines. 
>
> With this option the throughput collector (-XX:+UseParallelGC) is used along with adaptive sizing (-XX:+UseAdaptiveSizePolicy). 
>
> The physical memory on the machines must be at least 256MB before AggressiveHeap can be used. 
>
> The size of the initial heap is calculated based on the size of the physical memory and attempts to make maximal use of the physical memory for the heap (i.e., the algorithms attempt to use heaps nearly as large as the total physical memory).



> Use of the Java command line option -XX:+AggressiveHeap can result in strange and unwanted effects. +AggressiveHeap implicitly sets a number of options which often conflict with other command-line settings.



#### `-XX:MaxTenuringThreshold=${threshold}`

设置自适应GC中使用的最大`tenuring threshold`。

最大为15。

默认：对于并行（throughput）收集器为15；对于CMS为6

> ` tenuring threshold`是啥？ 效果是什么？ 
>
> Ref: [MaxTenuringThreshold - how exactly it works?](https://stackoverflow.com/questions/13543468/maxtenuringthreshold-how-exactly-it-works)
>
> Each object in Java heap has a header which is used by Garbage Collection (GC) algorithm. The young space collector (which is responsible for object promotion) uses a few bit(s) from this header to track the number of collections object that have survived (32-bit JVM use 4 bits for this, 64-bit probably some more).
>
> During young space collection, every single object is copied. The Object may be copied to one of survival spaces (one which is empty before young GC) or to the old space. <u>For each object being copied, GC algorithm increases it's age (number of collection survived) and if the age is above the current **tenuring threshold** it would be copied (promoted) to old space.</u> The Object could also be copied to the old space directly if the survival space gets full (overflow).
>
> The journey of Object has the following pattern:
>
> - allocated in eden
> - copied from eden to survival space due to young GC
> - copied from survival to (other) survival space due to young GC (this could happen few times)
> - promoted from survival (or possible eden) to old space due to young GC (or full GC)
>
> the actual **tenuring threshold** is dynamically adjusted by JVM, but MaxTenuringThreshold sets an upper limit on it.
>
> If you set MaxTenuringThreshold=0, all objects will be promoted immediately.
>
> I have [few articles](http://blog.ragozin.info/p/garbage-collection.html) about java garbage collection, there you can find more details.
>
> > **任期**门限值；JVM动态调整确定；但该参数可以设置最大值；
> >
> > 对象任期满 或者 s区满则晋升



#### `-XX:+UnlockDiagnosticVMOptions`

解锁那些用于诊断JVM的选项。

默认禁用，也即这些选项不可用。



具体如下：

```
+     bool BindCMSThreadToCPU                        = false                               {diagnostic}
+     bool BlockOffsetArrayUseUnallocatedBlock       = false                               {diagnostic}
+     bool C1PatchInvokeDynamic                      = true                                {C1 diagnostic}
+    uintx CPUForCMSThread                           = 0                                   {diagnostic}
+     bool DebugInlinedCalls                         = true                                {C2 diagnostic}
+     bool DebugNonSafepoints                        = false                               {diagnostic}
+     bool DeferInitialCardMark                      = false                               {diagnostic}
+ccstrlist DisableIntrinsic                          =                                     {C2 diagnostic}
+     bool DisplayVMOutput                           = true                                {diagnostic}
+     intx DominatorSearchLimit                      = 1000                                {C2 diagnostic}
+     bool EnableInvokeDynamic                       = true                                {diagnostic}
+     bool FLSVerifyAllHeapReferences                = false                               {diagnostic}
+     bool FLSVerifyIndexTable                       = false                               {diagnostic}
+     bool FLSVerifyLists                            = false                               {diagnostic}
+     bool FoldStableValues                          = true                                {diagnostic}
+     bool ForceDynamicNumberOfGCThreads             = false                               {diagnostic}
+     bool ForceUnreachable                          = false                               {diagnostic}
+     bool G1PrintHeapRegions                        = false                               {diagnostic}
+     bool G1PrintRegionLivenessInfo                 = false                               {diagnostic}
+     bool G1SummarizeConcMark                       = false                               {diagnostic}
+     bool G1SummarizeRSetStats                      = false                               {diagnostic}
+     intx G1SummarizeRSetStatsPeriod                = 0                                   {diagnostic}
+     bool G1TraceConcRefinement                     = false                               {diagnostic}
+     bool G1VerifyHeapRegionCodeRoots               = false                               {diagnostic}
+     bool G1VerifyRSetsDuringFullGC                 = false                               {diagnostic}
+    uintx GCLockerRetryAllocationCount              = 2                                   {diagnostic}
+     bool GCParallelVerificationEnabled             = true                                {diagnostic}
+     intx GuaranteedSafepointInterval               = 1000                                {diagnostic}
+     bool IgnoreUnverifiableClassesDuringDump       = false                               {diagnostic}
+     bool LogCompilation                            = false                               {diagnostic}
+     bool LogEvents                                 = true                                {diagnostic}
+    uintx LogEventsBufferEntries                    = 10                                  {diagnostic}
+    ccstr LogFile                                   =                                     {diagnostic}
+     bool LogVMOutput                               = false                               {diagnostic}
+     bool LoopLimitCheck                            = true                                {C2 diagnostic}
+    uintx MallocMaxTestWords                        = 0                                   {diagnostic}
+     intx MallocVerifyInterval                      = 0                                   {diagnostic}
+     intx MallocVerifyStart                         = 0                                   {diagnostic}
+     bool OptimizeExpensiveOps                      = true                                {C2 diagnostic}
+     intx ParGCCardsPerStrideChunk                  = 256                                 {diagnostic}
+    uintx ParGCStridesPerThread                     = 2                                   {diagnostic}
+     bool ParallelGCRetainPLAB                      = false                               {diagnostic}
+     bool PauseAtExit                               = false                               {diagnostic}
+     bool PauseAtStartup                            = false                               {diagnostic}
+    ccstr PauseAtStartupFile                        =                                     {diagnostic}
+     bool PrintAdapterHandlers                      = false                               {diagnostic}
+     bool PrintAssembly                             = false                               {diagnostic}
+    ccstr PrintAssemblyOptions                      =                                     {diagnostic}
+     bool PrintBiasedLockingStatistics              = false                               {diagnostic}
+     bool PrintCompilation2                         = false                               {diagnostic}
+     bool PrintCompressedOopsMode                   = false                               {diagnostic}
+     bool PrintDTraceDOF                            = false                               {diagnostic}
+     bool PrintInlining                             = false                               {diagnostic}
+     bool PrintInterpreter                          = false                               {diagnostic}
+     bool PrintIntrinsics                           = false                               {C2 diagnostic}
+     bool PrintMethodFlushingStatistics             = false                               {diagnostic}
+     bool PrintMethodHandleStubs                    = false                               {diagnostic}
+     bool PrintNMTStatistics                        = false                               {diagnostic}
+     bool PrintNMethods                             = false                               {diagnostic}
+     bool PrintNativeNMethods                       = false                               {diagnostic}
+     bool PrintPreciseBiasedLockingStatistics       = false                               {C2 diagnostic}
+     bool PrintPreciseRTMLockingStatistics          = false                               {C2 diagnostic}
+     bool PrintSignatureHandlers                    = false                               {diagnostic}
+     bool PrintStubCode                             = false                               {diagnostic}
+     bool ProfileDynamicTypes                       = true                                {C2 diagnostic}
+     bool RangeLimitCheck                           = true                                {C2 diagnostic}
+     intx ScavengeRootsInCode                       = 2                                   {diagnostic}
+     bool SerializeVMOutput                         = true                                {diagnostic}
+    ccstr SharedArchiveFile                         =                                     {diagnostic}
+     bool ShowHiddenFrames                          = false                               {diagnostic}
+     bool StringDeduplicationRehashALot             = false                               {diagnostic}
+     bool StringDeduplicationResizeALot             = false                               {diagnostic}
+     bool TraceGCTaskThread                         = false                               {diagnostic}
+     bool TraceJVMTIObjectTagging                   = false                               {diagnostic}
+     bool TraceNMethodInstalls                      = false                               {diagnostic}
+     bool TraceTypeProfile                          = false                               {C2 diagnostic}
+     bool UnlockDiagnosticVMOptions                := true                                {diagnostic}
+     bool UnrollLimitCheck                          = true                                {C2 diagnostic}
+     bool UnsyncloadClass                           = false                               {diagnostic}
+     bool UseImplicitStableValues                   = true                                {C2 diagnostic}
+     bool UseIncDec                                 = true                                {ARCH diagnostic}
+     bool UseInlineDepthForSpeculativeTypes         = true                                {C2 diagnostic}
+     bool UseNewCode                                = false                               {diagnostic}
+     bool UseNewCode2                               = false                               {diagnostic}
+     bool UseNewCode3                               = false                               {diagnostic}
+     bool VerboseVerification                       = false                               {diagnostic}
+     bool VerifyAdapterCalls                        = false                               {diagnostic}
+     bool VerifyAfterGC                             = false                               {diagnostic}
+     bool VerifyBeforeExit                          = false                               {diagnostic}
+     bool VerifyBeforeGC                            = false                               {diagnostic}
+     bool VerifyBeforeIteration                     = false                               {diagnostic}
+     bool VerifyDuringGC                            = false                               {diagnostic}
+     bool VerifyDuringStartup                       = false                               {diagnostic}
+     intx VerifyGCLevel                             = 0                                   {diagnostic}
+    uintx VerifyGCStartAt                           = 0                                   {diagnostic}
+     bool VerifyMethodHandles                       = false                               {diagnostic}
+     bool VerifyObjectStartArray                    = true                                {diagnostic}
+     bool VerifyRememberedSets                      = false                               {diagnostic}
+     bool VerifySilently                            = false                               {diagnostic}
+     bool VerifyStringTableAtExit                   = false                               {diagnostic}
+ccstrlist VerifySubSet                              =                                     {diagnostic}
+     bool WhiteBoxAPI                               = false                               {diagnostic}
```

> 获取方式： 比较 `java -XX:+PrintFlagsFinal` 和 `java -XX:+UnlockDiagnosticVMOptions -XX:+PrintFlagsFinal` 的输出





#### `-XX:+UnlockExperimentalVMOptions`



```
+     bool AggressiveUnboxing                        = false                               {C2 experimental}
+    uintx ArrayAllocatorMallocLimit                 = 18446744073709551615                    {experimental}
+     bool G1EagerReclaimHumongousObjects            = true                                {experimental}
+     bool G1EagerReclaimHumongousObjectsWithStaleRefs  = true                                {experimental}
+     intx G1ExpandByPercentOfAvailable              = 20                                  {experimental}
+    ccstr G1LogLevel                                =                                     {experimental}
+    uintx G1MaxNewSizePercent                       = 60                                  {experimental}
+    uintx G1MixedGCLiveThresholdPercent             = 85                                  {experimental}
+    uintx G1NewSizePercent                          = 5                                   {experimental}
+    uintx G1OldCSetRegionThresholdPercent           = 10                                  {experimental}
+     bool G1TraceEagerReclaimHumongousObjects       = false                               {experimental}
+     bool G1TraceStringSymbolTableScrubbing         = false                               {experimental}
+     bool G1UseConcMarkReferenceProcessing          = true                                {experimental}
+     intx NativeMonitorFlags                        = 0                                   {experimental}
+     intx NativeMonitorSpinLimit                    = 20                                  {experimental}
+     intx NativeMonitorTimeout                      = -1                                  {experimental}
+   double ObjectCountCutOffPercent                  = 0.500000                            {experimental}
+     intx PerMethodSpecTrapLimit                    = 5000                                {experimental}
+     intx PredictedLoadedClassCount                 = 0                                   {experimental}
+     intx RTMAbortRatio                             = 50                                  {ARCH experimental}
+     intx RTMAbortThreshold                         = 1000                                {ARCH experimental}
+     intx RTMLockingCalculationDelay                = 0                                   {ARCH experimental}
+     intx RTMLockingThreshold                       = 10000                               {ARCH experimental}
+     intx RTMSpinLoopCount                          = 100                                 {ARCH experimental}
+     intx RTMTotalCountIncrRate                     = 64                                  {ARCH experimental}
+     intx SpecTrapLimitExtraEntries                 = 3                                   {experimental}
+     intx SurvivorAlignmentInBytes                  = 8                                   {experimental}
+    uintx SymbolTableSize                           = 20011                               {experimental}
+     bool TrustFinalNonStaticFields                 = false                               {experimental}
+     bool UnlockExperimentalVMOptions              := true                                {experimental}
+     bool UseCriticalCMSThreadPriority              = false                               {experimental}
+     bool UseCriticalCompilerThreadPriority         = false                               {experimental}
+     bool UseCriticalJavaThreadPriority             = false                               {experimental}
+     bool UseFastUnorderedTimeStamps               := true                                {experimental}
+     bool UseMemSetInBOT                            = true                                {experimental}
+     bool UseRTMForStackLocks                       = false                               {ARCH experimental}
+     bool UseRTMXendForLockBusy                     = true                                {ARCH experimental}
+    uintx WorkStealingHardSpins                     = 4096                                {experimental}
+    uintx WorkStealingSleepMillis                   = 1                                   {experimental}
+    uintx WorkStealingSpinToYieldRatio              = 10                                  {experimental}
+    uintx WorkStealingYieldsBeforeSleep             = 5000                                {experimental}
```



#### `-XX:+ExplicitGCInvokesConcurrent`

启用通过显式调用（`System.gc()`）来触发并发GC。

> 改变`System.gc()`的行为

默认禁用，启用时需要和 `-XX:+UseConcMarkSweepGC` 一起使用。



使用场景： 

> ref： [关于 JVM 参数中 ExplicitGCInvokesConcurrent的用途](http://www.liuinsect.com/2014/05/12/whats-explicitgcinvokesconcurrent-used-for/)

* NIO框架大量使用堆外内存，为了及时回收（不等自然触发的fullGC）在框架代码里手动调`System.gc()`来触发fullGC

* 而频繁fullGC又带来副作用： 影响应用性能

* 于是，`-XX:+DisableExplicitGC`派上用场，禁止显式GC

* but，禁掉后框架使用的对外内存不能及时回收

* 那么，能不能”兼得“呢？

  思路： 修改`System.gc()`的行为，换成其他可接受的又能达到特定目的的GC。

  比如使用CMS时，可以通过本选项来改为`VM_GenCollectFullConcurrent`，实际会”酌情“进行GC，一般开销小于fullGC。

  见：

  ```cpp
    product(bool, ExplicitGCInvokesConcurrent, false,                         \
            "A System.gc() request invokes a concurrent collection; "         \
            "(effective only when UseConcMarkSweepGC)")                       \
                                                                              \
    product(bool, ExplicitGCInvokesConcurrentAndUnloadsClasses, false,        \
            "A System.gc() request invokes a concurrent collection and "      \
            "also unloads classes during such a concurrent gc cycle "         \
            "(effective only when UseConcMarkSweepGC)")                       
  // ExplicitGCInvokesConcurrentAndUnloadsClasses 为true会设置ExplicitGCInvokesConcurrent 为true
  ```

  


#### `-XX:+TraceClassResolution`

启用对常量池resolutions的追踪。

默认禁用。



```Java
RESOLVE java.io.Serializable java.lang.Object (super)  // 父类
RESOLVE java.lang.Comparable java.lang.Object (super)
RESOLVE java.lang.CharSequence java.lang.Object (super)
RESOLVE java.lang.String java.lang.Object (super)
RESOLVE java.lang.String java.io.Serializable (interface)  // 实现的接口
RESOLVE java.lang.String java.lang.Comparable (interface)
RESOLVE java.lang.String java.lang.CharSequence (interface)

RESOLVE yonka.snippets.java.clazz.load.A java.lang.String A.java:17 (reflection)  // method = ClassLoader.class.getDeclaredMethod("findLoadedClass", String.class);
RESOLVE yonka.snippets.java.clazz.load.A java.lang.Class A.java:17 (reflection)
RESOLVE java.lang.ClassLoader java.lang.ClassNotFoundException Class.java:-1

RESOLVE java.lang.ThreadLocal java.util.concurrent.atomic.AtomicInteger ThreadLocal.java:103
RESOLVE java.util.concurrent.atomic.AtomicInteger sun.misc.Unsafe AtomicInteger.java:135
RESOLVE java.lang.System java.nio.charset.Charset System.java:-1 (java.lang.System.initProperties(Ljava/util/Properties;)Ljava/util/Properties;)

RESOLVE yonka.snippets.java.clazz.load.A java.lang.ClassLoader A.java:17 (explicit)  // method = ClassLoader.class.getDeclaredMethod("findLoadedClass", String.class);
```



##### 输出内容理解

```cpp
// classFileParser.cpp
// instanceKlassHandle ClassFileParser::parseClassFile(Symbol* name, ClassLoaderData* loader_data, Handle protection_domain, KlassHandle host_klass, GrowableArray<Handle>* cp_patches, TempNewSymbol& parsed_name,  bool verify, TRAPS) 

    if (TraceClassResolution) {
      ResourceMark rm;
      // print out the superclass.
      const char * from = this_klass()->external_name();
      if (this_klass->java_super() != NULL) {
        tty->print("RESOLVE %s %s (super)\n", from, InstanceKlass::cast(this_klass->java_super())->external_name());
      }
      // print out each of the interface classes referred to by this class.
      Array<Klass*>* local_interfaces = this_klass->local_interfaces();
      if (local_interfaces != NULL) {
        int length = local_interfaces->length();
        for (int i = 0; i < length; i++) {
          Klass* k = local_interfaces->at(i);
          InstanceKlass* to_class = InstanceKlass::cast(k);
          const char * to = to_class->external_name();
          tty->print("RESOLVE %s %s (interface)\n", from, to);
        }
      }
    }
```

从代码里可以看到，打印了 `父类` 和 `实现接口` 的



```cpp
// reflection.cpp
// static void trace_class_resolution(Klass* to_class)
  if (caller != NULL) {
    const char * from = caller->external_name();
    const char * to = to_class->external_name();
    // print in a single call to reduce interleaving between threads
    if (source_file != NULL) {
      tty->print("RESOLVE %s %s %s:%d (reflection)\n", from, to, source_file, line_number);
    } else {
      tty->print("RESOLVE %s %s (reflection)\n", from, to);
    }
  }
```










# 引申



## 概念



### intrinsics

* 内联函数
* 内嵌原语
* 编译器内部函数
* 编译器固有支持

大致含义就是： 

> 某底层实现对某个特定功能特性有原生支持，无需上层（用底层通用原语/指令）实现，可以提高性能减小开销等。



#### `-XX:TargetSurvivorRatio=${percent}`

设置 the desired percentage of survivor space (0 to 100) used after young garbage collection.

默认为50（50%）。

> 感觉这句话描述的乱七八糟啊
>
> Ref: [useful-jvm-flags-part-5-young-generation-garbage-collection](https://blog.codecentric.de/en/2012/08/useful-jvm-flags-part-5-young-generation-garbage-collection/)
>
> Additionally, we can use `-XX:TargetSurvivorRatio` to specify the target utilization (in percent) of “To” at the end of a young generation GC. For example, the combination `-XX:MaxTenuringThreshold=10 -XX:TargetSurvivorRatio=90` sets an upper bound of 10 for the tenuring threshold and a target utilization of 90 percent for the “To” survivor space.
>
> > 理解（不确定对）： JVM GC机制可能会根据young GC后 to-survivor 区的占用情况（vs该值）来做相应调整，比如young区大小等

## 点



### 触发CMS GC

ref：

* [探秘Java虚拟机——内存管理与垃圾回收](http://www.blogjava.net/chhbjh/archive/2012/01/28/368936.html)

1. 当老生代空间的使用到达一定比率时触发；

   Hotspot V 1.6中默认为65%，可通过`PrintCMSInitiationStatistics`（此参数在V 1.5中不能用）来查看这个值到底是多少；可通过`CMSInitiatingOccupancyFraction`来强制指定，默认值并不是赋值在了这个值上，是根据如下公式计算出来的： `((100 - MinHeapFreeRatio) +(double)(CMSTriggerRatio * MinHeapFreeRatio) / 100.0)/ 100.0`; 其中,`MinHeapFreeRatio`默认值： 40   `CMSTriggerRatio`默认值： 80。

   此时使用的比例值为： (100-40) + 80 * 40 / 100 = 92(%)。

   > 从公式理解，CMSTriggerRatio表示MinHeapFreeRatio中能超过（使用）的比例。


2. 当perm gen采用CMS收集且空间使用到一定比率时触发；

   perm gen采用CMS收集需设置：`-XX:+CMSClassUnloadingEnabled`   Hotspot V 1.6中默认为65%；可通过`CMSInitiatingPermOccupancyFraction`来强制指定，同样，它是根据如下公式计算出来的：`((100 - MinHeapFreeRatio) +(double)(CMSTriggerPermRatio* MinHeapFreeRatio) / 100.0)/ 100.0`; 其中，`MinHeapFreeRatio`默认值： 40    `CMSTriggerPermRatio`默认值： 80。


3. Hotspot根据成本计算决定是否需要执行CMS GC；可通过`-XX:+UseCMSInitiatingOccupancyOnly`来去掉这个动态执行的策略。
4. 外部调用了`System.gc`，且设置了`ExplicitGCInvokesConcurrent`；需要注意，在hotspot 6中，在这种情况下如应用同时使用了NIO，可能会出现bug。



## 特性



### ResourceManagement

> 首先，要区分这个RM和 auto resource management。 后者类似py的context management 或者 C++ 的RAII，参见 [Better Resource Management with Java SE 7: Beyond Syntactic Sugar](http://www.oracle.com/technetwork/articles/java/trywithresources-401775.html)。
>
> ```java
> class AutoClose implements AutoCloseable {
>   // implement methods
>   @Override
>   public void close() throws Exception {
>     // ...
>   }
> }
>
> AutoCloseable ac = new AutoClose();
> try {
>   ac.work();
> } finally {
>   ac.close();
> }
>
> // or 
>
> try (AutoCloseable ac = new AutoClose()) {
>   ac.work();
> }
>
> // public abstract class InputStream implements Closeable
> // public interface Closeable extends AutoCloseable
> try (InputStream is = getInput()) {
>   // do sth
> }
> ```
>
> 但这个不是，这是Hotspot JVM的商用特性 = =



ref：

* [Package jdk.management.resource](http://weinert-automation.de/java/docs/jre/api/management/rm/index.html?jdk/management/resource/package-summary.html) or [local](resource/jdk.management.resource (Resource Management ).html)

  


#### 介绍

包括 资源追踪上下文、度量 和 factories（工厂？）。 提供了基本的框架和实现来追踪资源的使用。

> 工厂应该是指工厂类。



资源管理的架构包括三个主要组件：

* 资源追踪API

  * [`ResourceContextFactory`](http://weinert-automation.de/java/docs/jre/api/management/rm/jdk/management/resource/ResourceContextFactory.html) provides access to [ResourceContext](http://weinert-automation.de/java/docs/jre/api/management/rm/jdk/management/resource/ResourceContext.html)s and provides the main entry point to the API.
  * [`ResourceContext`](http://weinert-automation.de/java/docs/jre/api/management/rm/jdk/management/resource/ResourceContext.html) contains a set of [ResourceMeter](http://weinert-automation.de/java/docs/jre/api/management/rm/jdk/management/resource/ResourceMeter.html)s that track usage of resources by threads bound to the [ResourceContext](http://weinert-automation.de/java/docs/jre/api/management/rm/jdk/management/resource/ResourceContext.html).
  * [`ResourceType`](http://weinert-automation.de/java/docs/jre/api/management/rm/jdk/management/resource/ResourceType.html)s are used to identify a type of resource. For example [`FILE_OPEN opening a file`](http://weinert-automation.de/java/docs/jre/api/management/rm/jdk/management/resource/ResourceType.html#FILE_OPEN) or [`SOCKET_WRITE writing to a socket`](http://weinert-automation.de/java/docs/jre/api/management/rm/jdk/management/resource/ResourceType.html#SOCKET_WRITE).
  * [`ResourceMeter`](http://weinert-automation.de/java/docs/jre/api/management/rm/jdk/management/resource/ResourceMeter.html)s track the usage of a ResourceType. The [ResourceMeter](http://weinert-automation.de/java/docs/jre/api/management/rm/jdk/management/resource/ResourceMeter.html)s [`SimpleMeter`](http://weinert-automation.de/java/docs/jre/api/management/rm/jdk/management/resource/SimpleMeter.html), [`NotifyingMeter`](http://weinert-automation.de/java/docs/jre/api/management/rm/jdk/management/resource/NotifyingMeter.html),[`BoundedMeter`](http://weinert-automation.de/java/docs/jre/api/management/rm/jdk/management/resource/BoundedMeter.html), and [`ThrottledMeter`](http://weinert-automation.de/java/docs/jre/api/management/rm/jdk/management/resource/ThrottledMeter.html) count the resource use and approve, throttle, or deny resource use.
  * [`ResourceId`](http://weinert-automation.de/java/docs/jre/api/management/rm/jdk/management/resource/ResourceId.html)s identify specific resources and the accuracy of the measurements.
  * [`ResourceApprover`](http://weinert-automation.de/java/docs/jre/api/management/rm/jdk/management/resource/ResourceApprover.html) is an interface implemented by the resource manager and is notified of resource use. The response from the resource manager determines whether the resource is approved, limited or denied.

* 资源测量

  实现了对特定子系统的hook以 收集信息、请求资源批准、允许/限制/禁止资源访问。

  处理方式是： 动态的定位到当前调用线程关联的ResourceContext然后把ResourceRequest转发给匹配的ResourceType。

  覆盖了：

  * 文件描述符 - 打开的文件描述符计数
    * 关联到确切文件的文件描述符
    * 关联到socket和socket channel的文件描述符
  * 文件 - 打开的文件计数、发送/接收bytes
    * FileInputStream, FileOutputstream, RandomAccessFile
    * NIO的同步/异步FileChannel
    * 标准流 `System.err`, `System.in`, `System.out`
  * sockets和datagrams - 打开的socket的计数、发送/接收bytes； 数据包发送/接收
    * Socket, ServerSocket, DatagramSocket
    * NIO SocketChannel and DatagramChannel
    * NIO AsynchronousSocketChannel
  * 堆 - 分配和保留的内存大小（bytes），总分配数量
  * 线程 - 活动线程数量；每个资源上下文的CPU时间

* 资源管理



#### 使用

Example using a SimpleMeter to count the bytes written with FileOutputStream

```java
    void test1() {
        ResourceContextFactory rfactory = ResourceContextFactory.getInstance();
        ResourceContext rc1 = rfactory.create("context1");
        ResourceMeter writeMeter = SimpleMeter.create(ResourceType.FILE_WRITE);
        rc1.addResourceMeter(writeMeter);
        rc1.bindThreadContext();

        try {
            long bytesWritten = writeFile("example1.tmp");
            assert bytesWritten == writeMeter.get() : "Expected: " + bytesWritten + ", actual: " + writeMeter.get();
        } finally {
            ResourceContext.unbindThreadContext();
        }
    }
```

Example using a NotifyingMeter with callback to count bytes

```java
    public void test1() {
        ResourceContextFactory rfactory = ResourceContextFactory.getInstance();
        ResourceContext rcontext = rfactory.create("test");

        SimpleMeter fileOpenMeter = SimpleMeter.create(ResourceType.FILE_OPEN);
        rcontext.addResourceMeter(fileOpenMeter);

        SimpleMeter fileWriteMeter = SimpleMeter.create(ResourceType.FILE_WRITE);
        rcontext.addResourceMeter(fileWriteMeter);

        SimpleMeter threadCPUMeter = SimpleMeter.create(ResourceType.THREAD_CPU);
        rcontext.addResourceMeter(threadCPUMeter);

        SimpleMeter heapAllocMeter = SimpleMeter.create(ResourceType.HEAP_ALLOCATED);
        rcontext.addResourceMeter(heapAllocMeter);

        AtomicLong progress = new AtomicLong();
        NotifyingMeter fileReadMeter = NotifyingMeter.create(ResourceType.FILE_READ,
                (ResourceMeter c, long prev, long amt, ResourceId id) -> {
                    // total up the lengths of the positive requests
                    progress.getAndAdd(Math.max(0, amt));
                    return amt;
                });
        rcontext.addResourceMeter(fileReadMeter);

        rcontext.bindThreadContext();
        try {
            FileConsumer fc = FileConsumer.create();
            fc.write();
            fc.read();
        } catch (IOException ioe) {
            System.out.printf("ioe: %s%n", ioe);
        } finally {
            ResourceContext.unbindThreadContext();
        }
        System.out.printf(" cpu:        %9d ns%n", threadCPUMeter.getValue());
        System.out.printf(" file open:  %9d bytes%n", fileOpenMeter.getValue());
        System.out.printf(" file read:  %9d bytes%n", fileReadMeter.getValue());
        System.out.printf(" file write: %9d bytes%n", fileWriteMeter.getValue());
        System.out.printf(" heap total: %9d bytes%n", heapAllocMeter.getValue());
        System.out.printf(" progress:   %9d bytes%n", progress.get());
    }
```

Produces the output

```
 cpu:         76960825 ns
 file open:          8 bytes
 file read:      82639 bytes
 file write:     82639 bytes
 heap total:    801624 bytes
 progress:       99188 bytes
```






### NativeMemoryTracking

ref：

* [Java Platform, Standard Edition Troubleshooting Guide - 2.7 Native Memory Tracking](https://docs.oracle.com/javase/8/docs/technotes/guides/troubleshoot/tooldescr007.html)
* [technotes-guides-Native Memory Tracking](https://docs.oracle.com/javase/8/docs/technotes/guides/vm/nmt-8.html)



#### 介绍



Table 2-1 Native Memory Tracking Memory Categories

| Category                 | Description                              |
| ------------------------ | ---------------------------------------- |
| Java Heap                | The heap where your objects live         |
| Class                    | Class meta data                          |
| Code                     | Generated code                           |
| GC                       | data use by the GC, such as card table   |
| Compiler                 | Memory used by the compiler when generating code |
| Symbol                   | Symbols                                  |
| Memory Tracking          | Memory used by NMT itself                |
| Pooled Free Chunks       | Memory used by chunks in the arena chunk pool |
| Shared space for classes | Memory mapped to class data sharing archive |
| Thread                   | Memory used by threads, including thread data structure, resource area and handle area and so on. |
| Thread stack             | Thread stack. It is marked as committed memory, but it might not be completely committed by the OS |
| Internal                 | Memory that does not fit the previous categories, such as the memory used by the command line parser, JVMTI, properties and so on. |
| Unknown                  | When memory category can not be determined.</br> Arena: When arena is used as a stack or value object</br> Virtual Memory: When type information has not yet arrived |



**arena**

> Arena is a chunk of memory allocated using malloc. 
>
> Memory is freed from these chunks in bulk, when exiting a scope or leaving an area of code. These chunks may be reused in other subsystems to hold temporary memory, for example, pre-thread allocations. 
>
> Arena malloc policy ensures no memory leakage. So Arena is tracked as a whole and not individual objects. Some amount of initial memory can not by tracked.



**性能损失**

Enabling NMT will result in a 5-10 percent JVM performance drop and memory usage for NMT adds 2 machine words to all malloc memory as malloc header. NMT memory usage is also tracked by NMT.





#### 使用

1. 启动参数方式（`-XX:NativeMemoryTracking`）启动

   好像不能用jcmd启动

2. jcmd方式输出 or 启动参数方式指定vm exit时输出

   * `jcmd <pid> VM.native_memory [summary | detail | baseline | summary.diff | detail.diff | shutdown][scale= KB | MB | GB]`

     summary/detail是查看当前数据；而先做baseline然后执行diff的话可以查看diff值（分两次执行）

   * `-XX:+UnlockDiagnosticVMOptions -XX:+PrintNMTStatistics`



#### 样例



**summary**

```shell
jcmd 67571 VM.native_memory summary                                                     master
67571:

Native Memory Tracking:

Total: reserved=5878329KB, committed=1799645KB		<--- total memory tracked by Native Memory Tracking
-                 Java Heap (reserved=4194304KB, committed=1372672KB)		<--- Java Heap
                            (mmap: reserved=4194304KB, committed=1372672KB)

-                     Class (reserved=1141417KB, committed=104105KB)		<--- class metadata
                            (classes #15767)		<--- number of loaded classes
                            (malloc=10921KB #19857)		<--- malloc'd memory, #number of malloc
                            (mmap: reserved=1130496KB, committed=93184KB)

-                    Thread (reserved=83456KB, committed=83456KB)
                            (thread #75)		<--- number of threads
                            (stack: reserved=82944KB, committed=82944KB)		<--- memory used by thread stacks
                            (malloc=233KB #380)
                            (arena=279KB #149)		<--- resource and handle areas

-                      Code (reserved=258227KB, committed=48927KB)
                            (malloc=8627KB #9115)
                            (mmap: reserved=249600KB, committed=40300KB)

-                        GC (reserved=163634KB, committed=153194KB)
                            (malloc=10390KB #320)
                            (mmap: reserved=153244KB, committed=142804KB)

-                  Compiler (reserved=227KB, committed=227KB)
                            (malloc=96KB #309)
                            (arena=131KB #3)

-                  Internal (reserved=14689KB, committed=14689KB)
                            (malloc=14657KB #65790)
                            (mmap: reserved=32KB, committed=32KB)

-                    Symbol (reserved=17790KB, committed=17790KB)
                            (malloc=15033KB #170500)
                            (arena=2757KB #1)

-    Native Memory Tracking (reserved=4386KB, committed=4386KB)
                            (malloc=178KB #2791)
                            (tracking overhead=4208KB)

-               Arena Chunk (reserved=199KB, committed=199KB)
                            (malloc=199KB)
```

> 



**detail**

```shell
67571:

Native Memory Tracking:

Total: reserved=5880757KB, committed=1800477KB
-                 Java Heap (reserved=4194304KB, committed=1372672KB)
                            (mmap: reserved=4194304KB, committed=1372672KB)

-                     Class (reserved=1143476KB, committed=104372KB)
                            (classes #15784)
                            (malloc=10932KB #20237)
                            (mmap: reserved=1132544KB, committed=93440KB)

-                    Thread (reserved=83456KB, committed=83456KB)
                            (thread #75)
                            (stack: reserved=82944KB, committed=82944KB)
                            (malloc=233KB #380)
                            (arena=279KB #149)

-                      Code (reserved=258545KB, committed=49441KB)
                            (malloc=8945KB #9482)
                            (mmap: reserved=249600KB, committed=40496KB)

-                        GC (reserved=163634KB, committed=153194KB)
                            (malloc=10390KB #328)
                            (mmap: reserved=153244KB, committed=142804KB)
                            

-                  Compiler (reserved=241KB, committed=241KB)
                            (malloc=111KB #330)
                            (arena=131KB #3)

-                  Internal (reserved=14696KB, committed=14696KB)
                            (malloc=14664KB #65945)
                            (mmap: reserved=32KB, committed=32KB)

-                    Symbol (reserved=17799KB, committed=17799KB)
                            (malloc=15042KB #170536)
                            (arena=2757KB #1)

-    Native Memory Tracking (reserved=4405KB, committed=4405KB)
                            (malloc=181KB #2843)
                            (tracking overhead=4224KB)

-               Arena Chunk (reserved=199KB, committed=199KB)
                            (malloc=199KB)

Virtual memory map:

[0x0000000108a4c000 - 0x0000000108a54000] reserved and committed 32KB for Internal from
    [0x000000010a8b19bc] _ZN10PerfMemory20create_memory_regionEm+0x728
    [0x000000010a8b10c3] _ZN10PerfMemory10initializeEv+0x39
    [0x000000010a96cc6b] _ZN7Threads9create_vmEP14JavaVMInitArgsPb+0x13b
    
    [0x000000010a723eb7] JNI_CreateJavaVM+0x76

[0x0000000108a5d000 - 0x0000000108e1d000] reserved 3840KB for Code from
    [0x000000010a9adec8] _ZN13ReservedSpace10initializeEmmbPcmb+0x174
    [0x000000010a9ae26d] _ZN13ReservedSpaceC2Emm+0x81
    [0x000000010a69fc6b] _ZN8CodeHeap7reserveEmmm+0x1df
    [0x000000010a59c667] _ZN9CodeCache10initializeEv+0x7d

        [0x0000000108af7000 - 0x0000000108af9000] committed 8KB from
            [0x000000010a9ad91b] _ZN12VirtualSpace9expand_byEmb+0x117
            [0x000000010a69fa67] _ZN8CodeHeap9expand_byEm+0xc9
            [0x000000010a59c91d] _ZN9CodeCache8allocateEib+0x6f
            [0x000000010a86d487] _ZN7nmethod11new_nmethodE12methodHandleiiP11CodeOffsetsiP24DebugInformationRecorderP12DependenciesP10CodeBufferiP9OopMapSetP21ExceptionHandlerTableP22ImplicitExceptionTableP16AbstractCompileri+0x16d

        [0x0000000108af6000 - 0x0000000108af7000] committed 4KB from
            [0x000000010a9ad91b] _ZN12VirtualSpace9expand_byEmb+0x117
            [0x000000010a69fa67] _ZN8CodeHeap9expand_byEm+0xc9
            [0x000000010a59c91d] _ZN9CodeCache8allocateEib+0x6f
            [0x000000010a599856] _ZN10BufferBlob6createEPKci+0x62
            
...

[0x000000010a4cb782] _ZL28attach_listener_thread_entryP10JavaThreadP6Thread+0x29
[0x000000010a96ab0f] _ZN10JavaThread17thread_main_innerEv+0x9b
[0x000000010a96c1fc] _ZN10JavaThread3runEv+0x1c2
[0x000000010a88a5b2] _ZL10java_startP6Thread+0xf6
                             (reserved=1024KB, committed=1024KB)

[0x000000010a9b6844] _ZN8VMThread3runEv+0x20
[0x000000010a88a5b2] _ZL10java_startP6Thread+0xf6
[0x00007fff9922baab] _pthread_body+0xb4
[0x00007fff9922b9f7] _pthread_body+0x0
                             (reserved=1024KB, committed=1024KB)

[0x000000010a9adec8] _ZN13ReservedSpace10initializeEmmbPcmb+0x174
[0x000000010a9ae095] _ZN13ReservedSpaceC1EmmbPcm+0x17
[0x000000010a8c5bce] _ZN19ParallelCompactData13create_vspaceEmm+0x78
[0x000000010a8c5d62] _ZN19ParallelCompactData22initialize_region_dataEm+0x28
                             (reserved=320KB, committed=320KB)

[0x000000010a8b19bc] _ZN10PerfMemory20create_memory_regionEm+0x728
[0x000000010a8b10c3] _ZN10PerfMemory10initializeEv+0x39
[0x000000010a96cc6b] _ZN7Threads9create_vmEP14JavaVMInitArgsPb+0x13b
[0x000000010a723eb7] JNI_CreateJavaVM+0x76
                             (reserved=32KB, committed=32KB)
```

> 看起来像是native mem的分配情况（分配时的栈帧？）



官方的detail示例

```shell
Virtual memory map:
 
[0x8f1c1000 - 0x8f467000] reserved 2712KB for Thread Stack
                from [Thread::record_stack_base_and_size()+0xca]
        [0x8f1c1000 - 0x8f467000] committed 2712KB from [Thread::record_stack_base_and_size()+0xca]
 
[0x8f585000 - 0x8f729000] reserved 1680KB for Thread Stack
                from [Thread::record_stack_base_and_size()+0xca]
        [0x8f585000 - 0x8f729000] committed 1680KB from [Thread::record_stack_base_and_size()+0xca]
 
[0x8f930000 - 0x90100000] reserved 8000KB for GC
                from [ReservedSpace::initialize(unsigned int, unsigned int, bool, char*, unsigned int, bool)+0x555]
        [0x8f930000 - 0x90100000] committed 8000KB from [PSVirtualSpace::expand_by(unsigned int)+0x95]
 
[0x902dd000 - 0x9127d000] reserved 16000KB for GC
                from [ReservedSpace::initialize(unsigned int, unsigned int, bool, char*, unsigned int, bool)+0x555]
        [0x902dd000 - 0x9127d000] committed 16000KB from [os::pd_commit_memory(char*, unsigned int, unsigned int, bool)+0x36]
 
[0x9127d000 - 0x91400000] reserved 1548KB for Thread Stack
                from [Thread::record_stack_base_and_size()+0xca]
        [0x9127d000 - 0x91400000] committed 1548KB from [Thread::record_stack_base_and_size()+0xca]
 
[0x91400000 - 0xb0c00000] reserved 516096KB for Java Heap                                                                            <--- reserved memory range
                from [ReservedSpace::initialize(unsigned int, unsigned int, bool, char*, unsigned int, bool)+0x190]                  <--- callsite that reserves the memory
        [0x91400000 - 0x93400000] committed 32768KB from [VirtualSpace::initialize(ReservedSpace, unsigned int)+0x3e8]               <--- committed memory range and its callsite
        [0xa6400000 - 0xb0c00000] committed 172032KB from [PSVirtualSpace::expand_by(unsigned int)+0x95]                             <--- committed memory range and its callsite
 
[0xb0c61000 - 0xb0ce2000] reserved 516KB for Thread Stack
                from [Thread::record_stack_base_and_size()+0xca]
        [0xb0c61000 - 0xb0ce2000] committed 516KB from [Thread::record_stack_base_and_size()+0xca]
 
[0xb0ce2000 - 0xb0e83000] reserved 1668KB for GC
                from [ReservedSpace::initialize(unsigned int, unsigned int, bool, char*, unsigned int, bool)+0x555]
        [0xb0ce2000 - 0xb0cf0000] committed 56KB from [PSVirtualSpace::expand_by(unsigned int)+0x95]
        [0xb0d88000 - 0xb0d96000] committed 56KB from [CardTableModRefBS::resize_covered_region(MemRegion)+0xebf]
        [0xb0e2e000 - 0xb0e83000] committed 340KB from [CardTableModRefBS::resize_covered_region(MemRegion)+0xebf]
 
[0xb0e83000 - 0xb7003000] reserved 99840KB for Code
                from [ReservedSpace::initialize(unsigned int, unsigned int, bool, char*, unsigned int, bool)+0x555]
        [0xb0e83000 - 0xb0e92000] committed 60KB from [VirtualSpace::initialize(ReservedSpace, unsigned int)+0x3e8]
        [0xb1003000 - 0xb139b000] committed 3680KB from [VirtualSpace::initialize(ReservedSpace, unsigned int)+0x37a]
 
[0xb7003000 - 0xb7603000] reserved 6144KB for Class
                from [ReservedSpace::initialize(unsigned int, unsigned int, bool, char*, unsigned int, bool)+0x555]
        [0xb7003000 - 0xb73a4000] committed 3716KB from [VirtualSpace::initialize(ReservedSpace, unsigned int)+0x37a]
 
[0xb7603000 - 0xb760b000] reserved 32KB for Internal
                from [PerfMemory::create_memory_region(unsigned int)+0x8ba]
 
[0xb770b000 - 0xb775c000] reserved 324KB for Thread Stack
                from [Thread::record_stack_base_and_size()+0xca]
        [0xb770b000 - 0xb775c000] committed 324KB from [Thread::record_stack_base_and_size()+0xca]
```





**baseline and diff**

```shell
jcmd 67571 VM.native_memory baseline                                                    master
67571:
Baseline succeeded

jcmd 67571 VM.native_memory summary.diff                                                master
67571:

Native Memory Tracking:

Total: reserved=5880968KB -448KB, committed=1800948KB -448KB		<--- total memory changes vs. earlier baseline. '+'=increase '-'=decrease
 

-                 Java Heap (reserved=4194304KB, committed=1372672KB)
                            (mmap: reserved=4194304KB, committed=1372672KB)

-                     Class (reserved=1143481KB, committed=104377KB)
                            (classes #15791)		<--- no more classes loaded
                            (malloc=10937KB #20411 -8)		<--- malloc'd memory no changes, but number of malloc count decreased by 8
                            (mmap: reserved=1132544KB, committed=93440KB)

-                    Thread (reserved=83456KB, committed=83456KB)
                            (thread #75)		<--- no more thread
                            (stack: reserved=82944KB, committed=82944KB)
                            (malloc=233KB #380)
                            (arena=279KB #149)		<--- no more arenas

-                      Code (reserved=258671KB +1KB, committed=49827KB +1KB)
                            (malloc=9071KB +1KB #9636 -1)
                            (mmap: reserved=249600KB, committed=40756KB)

-                        GC (reserved=163634KB, committed=153194KB)
                            (malloc=10390KB #335)
                            (mmap: reserved=153244KB, committed=142804KB)

-                  Compiler (reserved=241KB, committed=241KB)
                            (malloc=111KB #325)
                            (arena=131KB #3)

-                  Internal (reserved=14702KB, committed=14702KB)
                            (malloc=14670KB #66129 +4)
                            (mmap: reserved=32KB, committed=32KB)

-                    Symbol (reserved=17800KB, committed=17800KB)
                            (malloc=15043KB #170544)
                            (arena=2757KB #1)

-    Native Memory Tracking (reserved=4479KB +62KB, committed=4479KB +62KB)
                            (malloc=234KB +51KB #3625 +748)
                            (tracking overhead=4244KB +11KB)

-               Arena Chunk (reserved=199KB -512KB, committed=199KB -512KB)
                            (malloc=199KB -512KB)
                            
                            
                            
jcmd 67571 VM.native_memory detail.diff                                                 master
67571:

Native Memory Tracking:

Total: reserved=5880990KB -426KB, committed=1801358KB -38KB

-                 Java Heap (reserved=4194304KB, committed=1372672KB)
                            (mmap: reserved=4194304KB, committed=1372672KB)

-                     Class (reserved=1143482KB, committed=104634KB +256KB)
                            (classes #15791)
                            (malloc=10938KB #20437 +18)
                            (mmap: reserved=1132544KB, committed=93696KB +256KB)

-                    Thread (reserved=83456KB, committed=83456KB)
                            (thread #75)
                            (stack: reserved=82944KB, committed=82944KB)
                            (malloc=233KB #380)
                            (arena=279KB #149)

-                      Code (reserved=258687KB +18KB, committed=49975KB +150KB)
                            (malloc=9087KB +18KB #9664 +27)
                            (mmap: reserved=249600KB, committed=40888KB +132KB)

-                        GC (reserved=163634KB, committed=153194KB)
                            (malloc=10390KB #335)
                            (mmap: reserved=153244KB, committed=142804KB)

-                  Compiler (reserved=241KB, committed=241KB)
                            (malloc=111KB #323 -2)
                            (arena=131KB #3)

-                  Internal (reserved=14704KB +2KB, committed=14704KB +2KB)
                            (malloc=14672KB +2KB #66195 +70)
                            (mmap: reserved=32KB, committed=32KB)

-                    Symbol (reserved=17800KB, committed=17800KB)
                            (malloc=15043KB #170544)
                            (arena=2757KB #1)

-    Native Memory Tracking (reserved=4481KB +65KB, committed=4481KB +65KB)
                            (malloc=235KB +51KB #3637 +760)
                            (tracking overhead=4246KB +13KB)

-               Arena Chunk (reserved=199KB -512KB, committed=199KB -512KB)
                            (malloc=199KB -512KB)

[0x000000010a836401] _ZNK14LinkedListImplI10MallocSiteLN11ResourceObj15allocation_typeE2EL10MemoryType10ELN17AllocFailStrategy13AllocFailEnumE1EE8new_nodeERKS0_+0x27
[0x000000010a834834] _ZN14LinkedListImplI10MallocSiteLN11ResourceObj15allocation_typeE2EL10MemoryType10ELN17AllocFailStrategy13AllocFailEnumE1EE3addERKS0_+0x12
[0x000000010a834e49] _ZN26MallocAllocationSiteWalker14do_malloc_siteEPK10MallocSite+0x23
[0x000000010a824e04] _ZN15MallocSiteTable4walkEP16MallocSiteWalker+0x2e
                             (malloc=23KB +15KB #371 +235)

[0x000000010a825058] _ZN15MallocSiteTable9new_entryERK15NativeCallStack+0x0
[0x000000010a8250c2] _ZN15MallocSiteTable13lookup_or_addERK15NativeCallStackPmS3_+0x0
[0x000000010a8253d6] _ZN15MallocSiteTable13allocation_atERK15NativeCallStackmPmS3_+0x0
                             (malloc=150KB +1KB #2392 +11)

[0x000000010a836583] _ZNK14LinkedListImplI20ReservedMemoryRegionLN11ResourceObj15allocation_typeE2EL10MemoryType10ELN17AllocFailStrategy13AllocFailEnumE1EE8new_nodeERKS0_+0x25
[0x000000010a834eae] _ZN14LinkedListImplI20ReservedMemoryRegionLN11ResourceObj15allocation_typeE2EL10MemoryType10ELN17AllocFailStrategy13AllocFailEnumE1EE3addERKS0_+0x12
[0x000000010a835389] _ZN29VirtualMemoryAllocationWalker18do_allocation_siteEPK20ReservedMemoryRegion+0x23
[0x000000010a9ac347] _ZN20VirtualMemoryTracker19walk_virtual_memoryEP19VirtualMemoryWalker+0x57
                             (malloc=11KB +11KB #122 +122)

...

[0x000000010a9adec8] _ZN13ReservedSpace10initializeEmmbPcmb+0x174
[0x000000010a9ae095] _ZN13ReservedSpaceC1EmmbPcm+0x17
[0x000000010a847c83] _ZN16VirtualSpaceNodeC2Em+0x18b
[0x000000010a847d9d] _ZN16VirtualSpaceList24create_new_virtual_spaceEm+0x3f
                             (mmap: reserved=83968KB, committed=82432KB +256KB)

[0x000000010a9adec8] _ZN13ReservedSpace10initializeEmmbPcmb+0x174
[0x000000010a9adfd7] _ZN17ReservedCodeSpaceC2Emmb+0x23
[0x000000010a69fb4c] _ZN8CodeHeap7reserveEmmm+0xc0
[0x000000010a59c667] _ZN9CodeCache10initializeEv+0x7d
                             (mmap: reserved=245760KB, committed=40256KB +128KB)
```



官方detail diff样例：

```shell
Details:
 
[0x01195652] ChunkPool::allocate(unsigned int)+0xe2
                            (malloc=482KB -481KB, #8 -8)
 
[0x01195652] ChunkPool::allocate(unsigned int)+0xe2
                            (malloc=2786KB -19742KB, #134 -618)
 
[0x013bd432] CodeBlob::set_oop_maps(OopMapSet*)+0xa2
                            (malloc=591KB +6KB, #681 +37)
 
[0x013c12b1] CodeBuffer::block_comment(int, char const*)+0x21                <--- [callsite address] method name + offset
                            (malloc=562KB +33KB, #35940 +2125)               <--- malloc'd amount, increased by 33KB #malloc count, increased by 2125
 
[0x0145f172] ConstantPool::ConstantPool(Array<unsigned char>*)+0x62
                            (malloc=69KB +2KB, #610 +15)
 
...
 
[0x01aa3ee2] Thread::allocate(unsigned int, bool, unsigned short)+0x122
                            (malloc=21KB +2KB, #13 +1)
 
[0x01aa73ca] Thread::record_stack_base_and_size()+0xca
                            (mmap: reserved=7104KB +324KB, committed=7104KB +324KB)
```



### codecache





#### 限制codecache大小的意义

有一些场景可能会触发codecache中code增加： 

> 应用状态变化带来一批新的热点方法（触发编译），这样会增加codecache占用，而如果不限制的话...
>
> 典型的：
>
> * 启动 -> 正常运行
> * 整点秒杀、cache miss/cache crash 等异常流量



具体行为：

> 做flush操作，丢弃部分code腾出空间给新的...



#### codecache大小

显然，经常会有（热点方法）状态变化的应用， 适合更大的codecache。



### inline

ref：

* [When does the JIT automatically inline methods?](https://stackoverflow.com/questions/36585250/when-does-the-jit-automatically-inline-methods)
* [Performance of using default methods to compile Scala trait methods](http://lampscalaw3dev.epfl.ch/blog/2016/07/08/trait-method-performance.html)



HotSpot JIT inlining policy is rather complicated. It involves many heuristics like 

* caller method size
* callee method size
* IR node count
* inlining depth
* invocation count
* call site count
* throw count
* method signatures
* etc.

Some limits are skipped for accessor methods (getters/setters) and for trivial methods (bytecode count less than 6).

The related source code is mostly in [bytecodeInfo.cpp](http://hg.openjdk.java.net/jdk8u/jdk8u/hotspot/file/448a5dcf414f/src/share/vm/opto/bytecodeInfo.cpp#l311).
See `InlineTree::try_to_inline`, `should_inline`, `should_not_inline` functions.

The main JVM flags to control inlining are

* `-XX:MaxInlineLevel` (maximum number of nested calls that are inlined)
* `-XX:MaxInlineSize` (maximum bytecode size of a method to be inlined)
* `-XX:FreqInlineSize` (maximum bytecode size of a frequent method to be inlined)
* `-XX:MaxTrivialSize` (maximum bytecode size of a trivial method to be inlined)
* `-XX:MinInliningThreshold` (min. invocation count a method needs to have to be inlined)
* `-XX:LiveNodeCountInliningCutoff` (max number of live nodes in a method)






Both C1 and C2 perform inlining. The policy whether to inline a method is non-trivial and uses several heuristics (implemented in [bytecodeInfo.cpp](http://hg.openjdk.java.net/jdk8u/jdk8u/hotspot/file/f22b5be95347/src/share/vm/opto/bytecodeInfo.cpp), methods `should_inline`, `should_not_inline` and `try_to_inline`). A simplified summary:

- Trivial methods (6 bytes by default, `MaxTrivialSize`) are always inlined.
- Methods up to 35 bytes (`MaxInlineSize`) invoked more than 250 (`MinInliningThreshold`) times are inlined.
- Methods up to 325 bytes (`FreqInlineSize`) are inlined if the callsite is “hot” (or “frequent”), which means it is invoked more than 20 times (no command-line flag in release versions) per one invocation of the caller method.
- The inlining depth is limited (9 by default, `MaxInlineLevel`).
- No inlining is performed if the callsite method is already very large.

The procedure is the same for C1 and C2, it uses the invocation counter that is also used for compilation decisions (previous section).

Dmitry points out that a method being inlined might already be compiled, in which case the compiled assembly will be inlined. The size limits for inlining are controlled by a different parameter in this case, see [this thread](https://groups.google.com/forum/#!msg/mechanical-sympathy/8ARGnMds7tU/p4rxkhi-vgcJ) and [this ticket](https://bugs.openjdk.java.net/browse/JDK-6316156) for reference.



### JIT control

ref：

* [Why there is no infrastrucutre for hinting JIT compiler in JVM?](https://stackoverflow.com/questions/41458617/why-there-is-no-infrastrucutre-for-hinting-jit-compiler-in-jvm)




In fact, **there is** an infrastructure to control HotSpot JVM compiler.

#### 1. Compiler command file

You may specify a file containing compiler commands with `-XX:CompileCommandFile=` JVM option. There are commands to force inlining, to exclude a method from compilation, to set a per-method option (e.g. `MaxNodeLimit`) and so on. The full list of available commands can be found [here](http://hg.openjdk.java.net/jdk8u/jdk8u/hotspot/file/b4bdf3484720/src/share/vm/compiler/compilerOracle.cpp#l317).

An example Compiler command file may look like

```
inline java.util.ArrayList::add
exclude *::<clinit>
print com.example.MyClass::*
```

#### 2. Annotations

JDK-specific annotations are another way to control JVM optimizations. There are certain annotations that HotSpot JVM knows about, e.g.

- [`@java.lang.invoke.ForceInline`](http://hg.openjdk.java.net/jdk8u/jdk8u/jdk/file/0e4fc29a5ce4/src/share/classes/java/lang/invoke/ForceInline.java#l30)
- [`@java.lang.invoke.DontInline`](http://hg.openjdk.java.net/jdk8u/jdk8u/jdk/file/0e4fc29a5ce4/src/share/classes/java/lang/invoke/DontInline.java#l30)
- [`@java.lang.invoke.Stable`](http://hg.openjdk.java.net/jdk8u/jdk8u/jdk/file/0e4fc29a5ce4/src/share/classes/java/lang/invoke/Stable.java#l30)
- [`@sun.misc.Contended`](http://hg.openjdk.java.net/jdk8u/jdk8u/jdk/file/0e4fc29a5ce4/src/share/classes/sun/misc/Contended.java#l33)

**Note:** All these mechanisms are non-standard. They apply only to OpenJDK and Oracle JDK. There is no standard way to hint JVM compiler, because there are many JVM implementations with completely different compilation strategies. Particularly, there are JVMs with no JIT compilation at all.



the *are* hints to the JVM’s optimizer that this method is a good candidate for inlining:

- It’s `static` or `private`, i.e. non-overridable
- It’s extremely short
- It’s called several times within a loop




### large pages

#### 介绍

定义： 明显大于标准页大小的内存页，可以优化CPU页翻译的开销（buf使用等）。

> * 大页可以使得一个TLB条目表示更大的内存区域，于是TLB所需条目减少，压力减小，有利于内存敏感性应用

> 不过大页也会带来负面影响：
>
> * 可能导致内存短缺
>
>   为什么呢？ 看到有说法，large page mem不会被swap out并且立即分配而不是缺页时分配。
>
>   不知道还有其他原因没。
>
> * 使得其他应用的paging过重（why？）
>
> * 基于以上，拖慢整个系统
>
>
>
> * 还有就是一个运行很久的系统可能内存碎片过多而无法保留/分配足够大的内存来match huge page，此时OS/JVM会切换到正常页

#### TLB - translation-lookaside buffer

* 虚拟地址到物理地址的映射信息的缓存
* “珍稀“资源
* 当处理器访问多级页表时需要多次内存访问，会消耗（使用、占用）该缓存

#### 使用

##### 查看是否支持

* linux

  ```shell
  # cat /proc/meminfo | grep Huge
  HugePages_Total: 0
  HugePages_Free: 0
  Hugepagesize: 2048 kB
  # 类似的输出表示支持
  ```



##### 



## others

### jvm Global flags



```
[Global flags]
    uintx AdaptivePermSizeWeight                    = 20              {product}           
    uintx AdaptiveSizeDecrementScaleFactor          = 4               {product}           
    uintx AdaptiveSizeMajorGCDecayTimeScale         = 10              {product}           
    uintx AdaptiveSizePausePolicy                   = 0               {product}           
    uintx AdaptiveSizePolicyCollectionCostMargin    = 50              {product}           
    uintx AdaptiveSizePolicyInitializingSteps       = 20              {product}           
    uintx AdaptiveSizePolicyOutputInterval          = 0               {product}           
    uintx AdaptiveSizePolicyWeight                  = 10              {product}           
    uintx AdaptiveSizeThroughPutPolicy              = 0               {product}           
    uintx AdaptiveTimeWeight                        = 25              {product}           
     bool AdjustConcurrency                         = false           {product}           
     bool AggressiveOpts                            = false           {product}           
     intx AliasLevel                                = 3               {product}           
     intx AllocateInstancePrefetchLines             = 1               {product}           
     intx AllocatePrefetchDistance                  = 256             {product}           
     intx AllocatePrefetchInstr                     = 0               {product}           
     intx AllocatePrefetchLines                     = 3               {product}           
     intx AllocatePrefetchStepSize                  = 64              {product}           
     intx AllocatePrefetchStyle                     = 1               {product}           
     bool AllowJNIEnvProxy                          = false           {product}           
     bool AllowParallelDefineClass                  = false           {product}           
     bool AllowUserSignalHandlers                   = false           {product}           
     bool AlwaysActAsServerClassMachine             = false           {product}           
     bool AlwaysCompileLoopMethods                  = false           {product}           
     intx AlwaysInflate                             = 0               {product}           
     bool AlwaysLockClassLoader                     = false           {product}           
     bool AlwaysPreTouch                            = false           {product}           
     bool AlwaysRestoreFPU                          = false           {product}           
     bool AlwaysTenure                              = false           {product}           
     bool AnonymousClasses                          = false           {product}           
    uintx ArraycopyDstPrefetchDistance              = 0               {product}           
    uintx ArraycopySrcPrefetchDistance              = 0               {product}           
     bool AssertOnSuspendWaitFailure                = false           {product}           
     intx Atomics                                   = 0               {product}           
     intx AutoBoxCacheMax                           = 128             {C2 product}        
    uintx AutoGCSelectPauseMillis                   = 5000            {product}           
     intx BCEATraceLevel                            = 0               {product}           
     intx BackEdgeThreshold                         = 100000          {pd product}        
     bool BackgroundCompilation                     = true            {pd product}        
    uintx BaseFootPrintEstimate                     = 268435456       {product}           
     intx BiasedLockingBulkRebiasThreshold          = 20              {product}           
     intx BiasedLockingBulkRevokeThreshold          = 40              {product}           
     intx BiasedLockingDecayTime                    = 25000           {product}           
     intx BiasedLockingStartupDelay                 = 4000            {product}           
     bool BindCMSThreadToCPU                        = false           {diagnostic}        
     bool BindGCTaskThreadsToCPUs                   = false           {product}           
     intx BlockCopyLowLimit                         = 2048            {product}           
     bool BlockLayoutByFrequency                    = true            {C2 product}        
     intx BlockLayoutMinDiamondPercentage           = 20              {C2 product}        
     bool BlockLayoutRotateLoops                    = true            {C2 product}        
     bool BlockOffsetArrayUseUnallocatedBlock       = false           {diagnostic}        
     intx BlockZeroingLowLimit                      = 2048            {product}           
     bool BranchOnRegister                          = false           {C2 product}        
     bool BytecodeVerificationLocal                 = false           {product}           
     bool BytecodeVerificationRemote                = true            {product}           
     bool C1OptimizeVirtualCallProfiling            = true            {C1 product}        
     bool C1ProfileBranches                         = true            {C1 product}        
     bool C1ProfileCalls                            = true            {C1 product}        
     bool C1ProfileCheckcasts                       = true            {C1 product}        
     bool C1ProfileInlinedCalls                     = true            {C1 product}        
     bool C1ProfileVirtualCalls                     = true            {C1 product}        
     bool C1UpdateMethodData                        = true            {C1 product}        
     intx CICompilerCount                           = 2               {product}           
     bool CICompilerCountPerCPU                     = false           {product}           
     bool CITime                                    = false           {product}           
     bool CMSAbortSemantics                         = false           {product}           
    uintx CMSAbortablePrecleanMinWorkPerIteration   = 100             {product}           
     intx CMSAbortablePrecleanWaitMillis            = 100             {manageable}        
    uintx CMSBitMapYieldQuantum                     = 10485760        {product}           
    uintx CMSBootstrapOccupancy                     = 50              {product}           
     bool CMSClassUnloadingEnabled                  = false           {product}           
    uintx CMSClassUnloadingMaxInterval              = 0               {product}           
     bool CMSCleanOnEnter                           = true            {product}           
     bool CMSCompactWhenClearAllSoftRefs            = true            {product}           
    uintx CMSConcMarkMultiple                       = 32              {product}           
     bool CMSConcurrentMTEnabled                    = true            {product}           
    uintx CMSCoordinatorYieldSleepCount             = 10              {product}           
     bool CMSDumpAtPromotionFailure                 = false           {product}           
    uintx CMSExpAvgFactor                           = 50              {product}           
     bool CMSExtrapolateSweep                       = false           {product}           
    uintx CMSFullGCsBeforeCompaction                = 0               {product}           
    uintx CMSIncrementalDutyCycle                   = 10              {product}           
    uintx CMSIncrementalDutyCycleMin                = 0               {product}           
     bool CMSIncrementalMode                        = false           {product}           
    uintx CMSIncrementalOffset                      = 0               {product}           
     bool CMSIncrementalPacing                      = true            {product}           
    uintx CMSIncrementalSafetyFactor                = 10              {product}           
    uintx CMSIndexedFreeListReplenish               = 4               {product}           
     intx CMSInitiatingOccupancyFraction            = -1              {product}           
     intx CMSInitiatingPermOccupancyFraction        = -1              {product}           
     intx CMSIsTooFullPercentage                    = 98              {product}           
   double CMSLargeCoalSurplusPercent                = 0.950000        {product}           
   double CMSLargeSplitSurplusPercent               = 1.000000        {product}           
     bool CMSLoopWarn                               = false           {product}           
    uintx CMSMaxAbortablePrecleanLoops              = 0               {product}           
     intx CMSMaxAbortablePrecleanTime               = 5000            {product}           
    uintx CMSOldPLABMax                             = 1024            {product}           
    uintx CMSOldPLABMin                             = 16              {product}           
    uintx CMSOldPLABNumRefills                      = 4               {product}           
    uintx CMSOldPLABReactivityCeiling               = 10              {product}           
    uintx CMSOldPLABReactivityFactor                = 2               {product}           
     bool CMSOldPLABResizeQuicker                   = false           {product}           
    uintx CMSOldPLABToleranceFactor                 = 4               {product}           
     bool CMSPLABRecordAlways                       = true            {product}           
    uintx CMSParPromoteBlocksToClaim                = 16              {product}           
     bool CMSParallelRemarkEnabled                  = true            {product}           
     bool CMSParallelSurvivorRemarkEnabled          = true            {product}           
     bool CMSPermGenPrecleaningEnabled              = true            {product}           
    uintx CMSPrecleanDenominator                    = 3               {product}           
    uintx CMSPrecleanIter                           = 3               {product}           
    uintx CMSPrecleanNumerator                      = 2               {product}           
     bool CMSPrecleanRefLists1                      = true            {product}           
     bool CMSPrecleanRefLists2                      = false           {product}           
     bool CMSPrecleanSurvivors1                     = false           {product}           
     bool CMSPrecleanSurvivors2                     = true            {product}           
    uintx CMSPrecleanThreshold                      = 1000            {product}           
     bool CMSPrecleaningEnabled                     = true            {product}           
     bool CMSPrintChunksInDump                      = false           {product}           
     bool CMSPrintObjectsInDump                     = false           {product}           
    uintx CMSRemarkVerifyVariant                    = 1               {product}           
     bool CMSReplenishIntermediate                  = true            {product}           
    uintx CMSRescanMultiple                         = 32              {product}           
    uintx CMSRevisitStackSize                       = 1048576         {product}           
    uintx CMSSamplingGrain                          = 16384           {product}           
     bool CMSScavengeBeforeRemark                   = false           {product}           
    uintx CMSScheduleRemarkEdenPenetration          = 50              {product}           
    uintx CMSScheduleRemarkEdenSizeThreshold        = 2097152         {product}           
    uintx CMSScheduleRemarkSamplingRatio            = 5               {product}           
   double CMSSmallCoalSurplusPercent                = 1.050000        {product}           
   double CMSSmallSplitSurplusPercent               = 1.100000        {product}           
     bool CMSSplitIndexedFreeListBlocks             = true            {product}           
     intx CMSTriggerPermRatio                       = 80              {product}           
     intx CMSTriggerRatio                           = 80              {product}           
     intx CMSWaitDuration                           = 2000            {manageable}        
    uintx CMSWorkQueueDrainThreshold                = 10              {product}           
     bool CMSYield                                  = true            {product}           
    uintx CMSYieldSleepCount                        = 0               {product}           
     intx CMSYoungGenPerWorker                      = 67108864        {pd product}        
    uintx CMS_FLSPadding                            = 1               {product}           
    uintx CMS_FLSWeight                             = 75              {product}           
    uintx CMS_SweepPadding                          = 1               {product}           
    uintx CMS_SweepTimerThresholdMillis             = 10              {product}           
    uintx CMS_SweepWeight                           = 75              {product}           
    uintx CPUForCMSThread                           = 0               {diagnostic}        
     bool CheckJNICalls                             = false           {product}           
     bool ClassUnloading                            = true            {product}           
     intx ClearFPUAtPark                            = 0               {product}           
     bool ClipInlining                              = true            {product}           
    uintx CodeCacheExpansionSize                    = 65536           {pd product}        
    uintx CodeCacheFlushingMinimumFreeSpace         = 1536000         {product}           
    uintx CodeCacheMinimumFreeSpace                 = 512000          {product}           
     bool CollectGen0First                          = false           {product}           
     bool CompactFields                             = true            {product}           
     intx CompilationPolicyChoice                   = 0               {product}           
     intx CompilationRepeat                         = 0               {C1 product}        
ccstrlist CompileCommand                            =                 {product}           
    ccstr CompileCommandFile                        =                 {product}           
ccstrlist CompileOnly                               =                 {product}           
     intx CompileThreshold                          = 10000           {pd product}        
     bool CompilerThreadHintNoPreempt               = true            {product}           
     intx CompilerThreadPriority                    = -1              {product}           
     intx CompilerThreadStackSize                   = 0               {pd product}        
    uintx ConcGCThreads                             = 0               {product}           
     intx ConditionalMoveLimit                      = 3               {C2 pd product}     
     bool ConvertSleepToYield                       = true            {pd product}        
     bool ConvertYieldToSleep                       = false           {product}           
     bool CreateMinidumpOnCrash                     = false           {product}           
     bool CriticalJNINatives                        = true            {product}           
     bool DTraceAllocProbes                         = false           {product}           
     bool DTraceMethodProbes                        = false           {product}           
     bool DTraceMonitorProbes                       = false           {product}           
     bool DebugInlinedCalls                         = true            {diagnostic}        
     bool DebugNonSafepoints                        = false           {diagnostic}        
     bool Debugging                                 = false           {product}           
    uintx DefaultMaxRAMFraction                     = 4               {product}           
     intx DefaultThreadPriority                     = -1              {product}           
     bool DeferInitialCardMark                      = false           {diagnostic}        
     intx DeferPollingPageLoopCount                 = -1              {product}           
     intx DeferThrSuspendLoopCount                  = 4000            {product}           
     bool DeoptimizeRandom                          = false           {product}           
     bool DisableAttachMechanism                    = false           {product}           
     bool DisableExplicitGC                         = false           {product}           
ccstrlist DisableIntrinsic                          =                 {diagnostic}        
     bool DisplayVMOutput                           = true            {diagnostic}        
     bool DisplayVMOutputToStderr                   = false           {product}           
     bool DisplayVMOutputToStdout                   = false           {product}           
     bool DoEscapeAnalysis                          = true            {C2 product}        
     intx DominatorSearchLimit                      = 1000            {C2 diagnostic}     
     bool DontCompileHugeMethods                    = true            {product}           
     bool DontYieldALot                             = false           {pd product}        
     bool DumpSharedSpaces                          = false           {product}           
     bool EagerXrunInit                             = false           {product}           
     intx EliminateAllocationArraySizeLimit         = 64              {C2 product}        
     bool EliminateAllocations                      = true            {C2 product}        
     bool EliminateAutoBox                          = false           {C2 diagnostic}     
     bool EliminateLocks                            = true            {C2 product}        
     bool EliminateNestedLocks                      = true            {C2 product}        
     intx EmitSync                                  = 0               {product}           
     bool EnableInvokeDynamic                       = true            {diagnostic}        
    uintx ErgoHeapSizeLimit                         = 0               {product}           
    ccstr ErrorFile                                 =                 {product}           
    ccstr ErrorReportServer                         =                 {product}           
     bool EstimateArgEscape                         = true            {product}           
     intx EventLogLength                            = 2000            {product}           
     bool ExplicitGCInvokesConcurrent               = false           {product}           
     bool ExplicitGCInvokesConcurrentAndUnloadsClasses  = false           {product}           
     bool ExtendedDTraceProbes                      = false           {product}           
     bool FLSAlwaysCoalesceLarge                    = false           {product}           
    uintx FLSCoalescePolicy                         = 2               {product}           
   double FLSLargestBlockCoalesceProximity          = 0.990000        {product}           
     bool FLSVerifyAllHeapReferences                = false           {diagnostic}        
     bool FLSVerifyIndexTable                       = false           {diagnostic}        
     bool FLSVerifyLists                            = false           {diagnostic}        
     bool FailOverToOldVerifier                     = true            {product}           
     bool FastTLABRefill                            = true            {product}           
     intx FenceInstruction                          = 0               {product}           
     intx FieldsAllocationStyle                     = 1               {product}           
     bool FilterSpuriousWakeups                     = true            {product}           
     bool ForceDynamicNumberOfGCThreads             = false           {diagnostic}        
     bool ForceNUMA                                 = false           {product}           
     bool ForceTimeHighResolution                   = false           {product}           
     bool ForceUnreachable                          = false           {diagnostic}        
     intx FreqInlineSize                            = 325             {pd product}        
     bool FullProfileOnReInterpret                  = true            {diagnostic}        
   double G1ConcMarkStepDurationMillis              = 10.000000       {product}           
     intx G1ConcRefinementGreenZone                 = 0               {product}           
     intx G1ConcRefinementRedZone                   = 0               {product}           
     intx G1ConcRefinementServiceIntervalMillis     = 300             {product}           
    uintx G1ConcRefinementThreads                   = 0               {product}           
     intx G1ConcRefinementThresholdStep             = 0               {product}           
     intx G1ConcRefinementYellowZone                = 0               {product}           
     intx G1ConfidencePercent                       = 50              {product}           
    uintx G1HeapRegionSize                          = 0               {product}           
     intx G1MarkRegionStackSize                     = 1048576         {product}           
     bool G1PrintHeapRegions                        = false           {diagnostic}        
     bool G1PrintRegionLivenessInfo                 = false           {diagnostic}        
     intx G1RSetRegionEntries                       = 0               {product}           
    uintx G1RSetScanBlockSize                       = 64              {product}           
     intx G1RSetSparseRegionEntries                 = 0               {product}           
     intx G1RSetUpdatingPauseTimePercent            = 10              {product}           
     intx G1RefProcDrainInterval                    = 10              {product}           
    uintx G1ReservePercent                          = 10              {product}           
    uintx G1SATBBufferEnqueueingThresholdPercent    = 60              {product}           
     intx G1SATBBufferSize                          = 1024            {product}           
     bool G1SummarizeConcMark                       = false           {diagnostic}        
     bool G1SummarizeRSetStats                      = false           {diagnostic}        
     intx G1SummarizeRSetStatsPeriod                = 0               {diagnostic}        
     bool G1TraceConcRefinement                     = false           {diagnostic}        
     intx G1UpdateBufferSize                        = 256             {product}           
     bool G1UseAdaptiveConcRefinement               = true            {product}           
    uintx GCDrainStackTargetSize                    = 64              {product}           
    uintx GCHeapFreeLimit                           = 2               {product}           
    uintx GCLockerEdenExpansionPercent              = 5               {product}           
     bool GCLockerInvokesConcurrent                 = false           {product}           
    uintx GCLogFileSize                             = 0               {product}           
     bool GCOverheadReporting                       = false           {product}           
     intx GCOverheadReportingPeriodMS               = 100             {product}           
     bool GCParallelVerificationEnabled             = true            {diagnostic}        
    uintx GCPauseIntervalMillis                     = 0               {product}           
    uintx GCTaskTimeStampEntries                    = 200             {product}           
    uintx GCTimeLimit                               = 98              {product}           
    uintx GCTimeRatio                               = 99              {product}           
     intx GuaranteedSafepointInterval               = 1000            {diagnostic}        
    ccstr HPILibPath                                =                 {product}           
    uintx HeapBaseMinAddress                        = 2147483648      {pd product}        
     bool HeapDumpAfterFullGC                       = false           {manageable}        
     bool HeapDumpBeforeFullGC                      = false           {manageable}        
     bool HeapDumpOnOutOfMemoryError                = false           {manageable}        
    ccstr HeapDumpPath                              =                 {manageable}        
    uintx HeapFirstMaximumCompactionCount           = 3               {product}           
    uintx HeapMaximumCompactionInterval             = 20              {product}           
    uintx HeapSizePerGCThread                       = 87241520        {product}           
     bool IgnoreUnrecognizedVMOptions               = false           {product}           
    uintx InitialCodeCacheSize                      = 2555904         {pd product}        
     bool InitialCompileFast                        = false           {diagnostic}        
     bool InitialCompileReallyFast                  = false           {diagnostic}        
    uintx InitialHeapSize                          := 78632640        {product}           
    uintx InitialRAMFraction                        = 64              {product}           
    uintx InitialSurvivorRatio                      = 8               {product}           
     intx InitialTenuringThreshold                  = 7               {product}           
    uintx InitiatingHeapOccupancyPercent            = 45              {product}           
     bool Inline                                    = true            {product}           
     intx InlineSmallCode                           = 1000            {pd product}        
     bool InsertMemBarAfterArraycopy                = true            {C2 product}        
     intx InteriorEntryAlignment                    = 16              {C2 pd product}     
     intx InterpreterProfilePercentage              = 33              {product}           
     bool JNIDetachReleasesMonitors                 = true            {product}           
     bool JavaMonitorsInStackTrace                  = true            {product}           
     intx JavaPriority10_To_OSPriority              = -1              {product}           
     intx JavaPriority1_To_OSPriority               = -1              {product}           
     intx JavaPriority2_To_OSPriority               = -1              {product}           
     intx JavaPriority3_To_OSPriority               = -1              {product}           
     intx JavaPriority4_To_OSPriority               = -1              {product}           
     intx JavaPriority5_To_OSPriority               = -1              {product}           
     intx JavaPriority6_To_OSPriority               = -1              {product}           
     intx JavaPriority7_To_OSPriority               = -1              {product}           
     intx JavaPriority8_To_OSPriority               = -1              {product}           
     intx JavaPriority9_To_OSPriority               = -1              {product}           
     bool LIRFillDelaySlots                         = false           {C1 pd product}     
    uintx LargePageHeapSizeThreshold                = 134217728       {product}           
    uintx LargePageSizeInBytes                      = 0               {product}           
     bool LazyBootClassLoader                       = true            {product}           
     bool LinkWellKnownClasses                      = false           {diagnostic}        
     bool LogCompilation                            = false           {diagnostic}        
     bool LogEvents                                 = true            {diagnostic}        
     intx LogEventsBufferEntries                    = 10              {diagnostic}        
    ccstr LogFile                                   =                 {diagnostic}        
     bool LogVMOutput                               = false           {diagnostic}        
     bool LoopLimitCheck                            = true            {C2 diagnostic}     
     intx LoopOptsCount                             = 43              {C2 product}        
     intx LoopUnrollLimit                           = 60              {C2 pd product}     
     intx LoopUnrollMin                             = 4               {C2 product}        
     bool LoopUnswitching                           = true            {C2 product}        
     intx MallocVerifyInterval                      = 0               {diagnostic}        
     intx MallocVerifyStart                         = 0               {diagnostic}        
     bool ManagementServer                          = false           {product}           
    uintx MarkStackSize                             = 4194304         {product}           
    uintx MarkStackSizeMax                          = 536870912       {product}           
     intx MarkSweepAlwaysCompactCount               = 4               {product}           
    uintx MarkSweepDeadRatio                        = 1               {product}           
     intx MaxBCEAEstimateLevel                      = 5               {product}           
     intx MaxBCEAEstimateSize                       = 150             {product}           
     intx MaxDirectMemorySize                       = -1              {product}           
     bool MaxFDLimit                                = true            {product}           
    uintx MaxGCMinorPauseMillis                     = 18446744073709551615{product}           
    uintx MaxGCPauseMillis                          = 18446744073709551615{product}           
    uintx MaxHeapFreeRatio                          = 70              {product}           
    uintx MaxHeapSize                              := 1258291200      {product}           
     intx MaxInlineLevel                            = 9               {product}           
     intx MaxInlineSize                             = 35              {product}           
     intx MaxJavaStackTraceDepth                    = 1024            {product}           
     intx MaxJumpTableSize                          = 65000           {C2 product}        
     intx MaxJumpTableSparseness                    = 5               {C2 product}        
     intx MaxLabelRootDepth                         = 1100            {C2 product}        
     intx MaxLoopPad                                = 11              {C2 product}        
    uintx MaxNewSize                                = 18446744073709486080{product}           
     intx MaxNodeLimit                              = 65000           {C2 product}        
    uintx MaxPermHeapExpansion                      = 5439488         {product}           
    uintx MaxPermSize                               = 85983232        {pd product}        
 uint64_t MaxRAM                                    = 137438953472    {pd product}        
    uintx MaxRAMFraction                            = 4               {product}           
     intx MaxRecursiveInlineLevel                   = 1               {product}           
     intx MaxTenuringThreshold                      = 15              {product}           
     intx MaxTrivialSize                            = 6               {product}           
     bool MethodFlushing                            = true            {product}           
     intx MethodHandlePushLimit                     = 3               {diagnostic}        
     intx MinCodeCacheFlushingInterval              = 30              {product}           
    uintx MinHeapDeltaBytes                         = 196608          {product}           
    uintx MinHeapFreeRatio                          = 40              {product}           
     intx MinInliningThreshold                      = 250             {product}           
     intx MinJumpTableSize                          = 18              {C2 product}        
    uintx MinPermHeapExpansion                      = 327680          {product}           
    uintx MinRAMFraction                            = 2               {product}           
    uintx MinSurvivorRatio                          = 3               {product}           
    uintx MinTLABSize                               = 2048            {product}           
     intx MonitorBound                              = 0               {product}           
     bool MonitorInUseLists                         = false           {product}           
     intx MultiArrayExpandLimit                     = 6               {C2 product}        
     bool MustCallLoadClassInternal                 = false           {product}           
     intx NUMAChunkResizeWeight                     = 20              {product}           
    uintx NUMAInterleaveGranularity                 = 2097152         {product}           
     intx NUMAPageScanRate                          = 256             {product}           
     intx NUMASpaceResizeRate                       = 1073741824      {product}           
     bool NUMAStats                                 = false           {product}           
     intx NativeMonitorFlags                        = 0               {product}           
     intx NativeMonitorSpinLimit                    = 20              {product}           
     intx NativeMonitorTimeout                      = -1              {product}           
     bool NeedsDeoptSuspend                         = false           {pd product}        
     bool NeverActAsServerClassMachine              = false           {pd product}        
     bool NeverTenure                               = false           {product}           
     intx NewRatio                                  = 2               {product}           
    uintx NewSize                                   = 1310720         {product}           
    uintx NewSizeThreadIncrease                     = 5320            {pd product}        
     intx NmethodSweepCheckInterval                 = 5               {product}           
     intx NmethodSweepFraction                      = 16              {product}           
     intx NodeLimitFudgeFactor                      = 1000            {C2 product}        
    uintx NumberOfGCLogFiles                        = 0               {product}           
     intx NumberOfLoopInstrToAlign                  = 4               {C2 product}        
     intx ObjectAlignmentInBytes                    = 8               {lp64_product}      
    uintx OldPLABSize                               = 1024            {product}           
    uintx OldPLABWeight                             = 50              {product}           
    uintx OldSize                                   = 5439488         {product}           
     bool OmitStackTraceInFastThrow                 = true            {product}           
ccstrlist OnError                                   =                 {product}           
ccstrlist OnOutOfMemoryError                        =                 {product}           
     intx OnStackReplacePercentage                  = 140             {pd product}        
     bool OptimizeFill                              = true            {C2 product}        
     bool OptimizeMethodHandles                     = true            {diagnostic}        
     bool OptimizePtrCompare                        = true            {C2 product}        
     bool OptimizeStringConcat                      = true            {C2 product}        
     bool OptoBundling                              = false           {C2 pd product}     
     intx OptoLoopAlignment                         = 16              {pd product}        
     bool OptoScheduling                            = false           {C2 pd product}     
    uintx PLABWeight                                = 75              {product}           
     bool PSChunkLargeArrays                        = true            {product}           
     intx ParGCArrayScanChunk                       = 50              {product}           
     intx ParGCCardsPerStrideChunk                  = 256             {diagnostic}        
    uintx ParGCDesiredObjsFromOverflowList          = 20              {product}           
    uintx ParGCStridesPerThread                     = 2               {diagnostic}        
     bool ParGCTrimOverflow                         = true            {product}           
     bool ParGCUseLocalOverflow                     = false           {product}           
     intx ParallelGCBufferWastePct                  = 10              {product}           
     bool ParallelGCRetainPLAB                      = false           {diagnostic}        
    uintx ParallelGCThreads                         = 4               {product}           
     bool ParallelGCVerbose                         = false           {product}           
    uintx ParallelOldDeadWoodLimiterMean            = 50              {product}           
    uintx ParallelOldDeadWoodLimiterStdDev          = 80              {product}           
     bool ParallelRefProcBalancingEnabled           = true            {product}           
     bool ParallelRefProcEnabled                    = false           {product}           
     bool PartialPeelAtUnsignedTests                = true            {C2 product}        
     bool PartialPeelLoop                           = true            {C2 product}        
     intx PartialPeelNewPhiDelta                    = 0               {C2 product}        
     bool PauseAtExit                               = false           {diagnostic}        
     bool PauseAtStartup                            = false           {diagnostic}        
    ccstr PauseAtStartupFile                        =                 {diagnostic}        
    uintx PausePadding                              = 1               {product}           
     intx PerBytecodeRecompilationCutoff            = 200             {product}           
     intx PerBytecodeTrapLimit                      = 4               {product}           
     intx PerMethodRecompilationCutoff              = 400             {product}           
     intx PerMethodTrapLimit                        = 100             {product}           
     bool PerfAllowAtExitRegistration               = false           {product}           
     bool PerfBypassFileSystemCheck                 = false           {product}           
     intx PerfDataMemorySize                        = 32768           {product}           
     intx PerfDataSamplingInterval                  = 50              {product}           
    ccstr PerfDataSaveFile                          =                 {product}           
     bool PerfDataSaveToFile                        = false           {product}           
     bool PerfDisableSharedMem                      = false           {product}           
     intx PerfMaxStringConstLength                  = 1024            {product}           
    uintx PermGenPadding                            = 3               {product}           
    uintx PermMarkSweepDeadRatio                    = 5               {product}           
    uintx PermSize                                  = 21757952        {pd product}        
     bool PostSpinYield                             = true            {product}           
     intx PreBlockSpin                              = 10              {product}           
     intx PreInflateSpin                            = 10              {pd product}        
     bool PreSpinYield                              = false           {product}           
     bool PreferInterpreterNativeStubs              = false           {pd product}        
     intx PrefetchCopyIntervalInBytes               = 576             {product}           
     intx PrefetchFieldsAhead                       = 1               {product}           
     intx PrefetchScanIntervalInBytes               = 576             {product}           
     bool PreserveAllAnnotations                    = false           {product}           
    uintx PreserveMarkStackSize                     = 1024            {product}           
    uintx PretenureSizeThreshold                    = 0               {product}           
     bool PrintAdapterHandlers                      = false           {diagnostic}        
     bool PrintAdaptiveSizePolicy                   = false           {product}           
     bool PrintAssembly                             = false           {diagnostic}        
    ccstr PrintAssemblyOptions                      =                 {diagnostic}        
     bool PrintBiasedLockingStatistics              = false           {diagnostic}        
     bool PrintCMSInitiationStatistics              = false           {product}           
     intx PrintCMSStatistics                        = 0               {product}           
     bool PrintClassHistogram                       = false           {manageable}        
     bool PrintClassHistogramAfterFullGC            = false           {manageable}        
     bool PrintClassHistogramBeforeFullGC           = false           {manageable}        
     bool PrintCommandLineFlags                     = false           {product}           
     bool PrintCompilation                          = false           {product}           
     bool PrintCompilation2                         = false           {diagnostic}        
     bool PrintCompressedOopsMode                   = false           {diagnostic}        
     bool PrintConcurrentLocks                      = false           {manageable}        
     bool PrintDTraceDOF                            = false           {diagnostic}        
     intx PrintFLSCensus                            = 0               {product}           
     intx PrintFLSStatistics                        = 0               {product}           
     bool PrintFlagsFinal                          := true            {product}           
     bool PrintFlagsInitial                         = false           {product}           
     bool PrintGC                                   = false           {manageable}        
     bool PrintGCApplicationConcurrentTime          = false           {product}           
     bool PrintGCApplicationStoppedTime             = false           {product}           
     bool PrintGCDateStamps                         = false           {manageable}        
     bool PrintGCDetails                            = false           {manageable}        
     bool PrintGCTaskTimeStamps                     = false           {product}           
     bool PrintGCTimeStamps                         = false           {manageable}        
     bool PrintHeapAtGC                             = false           {product rw}        
     bool PrintHeapAtGCExtended                     = false           {product rw}        
     bool PrintHeapAtSIGBREAK                       = true            {product}           
     bool PrintInlining                             = false           {diagnostic}        
     bool PrintInterpreter                          = false           {diagnostic}        
     bool PrintIntrinsics                           = false           {diagnostic}        
     bool PrintJNIGCStalls                          = false           {product}           
     bool PrintJNIResolving                         = false           {product}           
     bool PrintMethodHandleStubs                    = false           {diagnostic}        
     bool PrintNMethods                             = false           {diagnostic}        
     bool PrintNativeNMethods                       = false           {diagnostic}        
     bool PrintOldPLAB                              = false           {product}           
     bool PrintOopAddress                           = false           {product}           
     bool PrintPLAB                                 = false           {product}           
     bool PrintParallelOldGCPhaseTimes              = false           {product}           
     bool PrintPreciseBiasedLockingStatistics       = false           {C2 diagnostic}     
     bool PrintPromotionFailure                     = false           {product}           
     bool PrintReferenceGC                          = false           {product}           
     bool PrintRevisitStats                         = false           {product}           
     bool PrintSafepointStatistics                  = false           {product}           
     intx PrintSafepointStatisticsCount             = 300             {product}           
     intx PrintSafepointStatisticsTimeout           = -1              {product}           
     bool PrintSharedSpaces                         = false           {product}           
     bool PrintSignatureHandlers                    = false           {diagnostic}        
     bool PrintStringTableStatistics                = false           {product}           
     bool PrintStubCode                             = false           {diagnostic}        
     bool PrintTLAB                                 = false           {product}           
     bool PrintTenuringDistribution                 = false           {product}           
     bool PrintTieredEvents                         = false           {product}           
     bool PrintVMOptions                            = false           {product}           
     bool PrintVMQWaitTime                          = false           {product}           
     bool PrintWarnings                             = true            {product}           
    uintx ProcessDistributionStride                 = 4               {product}           
     bool ProfileDynamicTypes                       = true            {diagnostic}        
     bool ProfileInterpreter                        = true            {pd product}        
     bool ProfileIntervals                          = false           {product}           
     intx ProfileIntervalsTicks                     = 100             {product}           
     intx ProfileMaturityPercentage                 = 20              {product}           
     bool ProfileVM                                 = false           {product}           
     bool ProfilerPrintByteCodeStatistics           = false           {product}           
     bool ProfilerRecordPC                          = false           {product}           
    uintx PromotedPadding                           = 3               {product}           
     intx QueuedAllocationWarningCount              = 0               {product}           
     bool RangeCheckElimination                     = true            {product}           
     bool RangeLimitCheck                           = true            {C2 diagnostic}     
     intx ReadPrefetchInstr                         = 0               {product}           
     intx ReadSpinIterations                        = 100             {product}           
     bool ReassociateInvariants                     = true            {C2 product}        
     bool ReduceBulkZeroing                         = true            {C2 product}        
     bool ReduceFieldZeroing                        = true            {C2 product}        
     bool ReduceInitialCardMarks                    = true            {C2 product}        
     bool ReduceSignalUsage                         = false           {product}           
     intx RefDiscoveryPolicy                        = 0               {product}           
     bool ReflectionWrapResolutionErrors            = true            {product}           
     bool RegisterFinalizersAtInit                  = true            {product}           
     bool RelaxAccessControlCheck                   = false           {product}           
     bool RequireSharedSpaces                       = false           {product}           
    uintx ReservedCodeCacheSize                     = 50331648        {pd product}        
     bool ResizeOldPLAB                             = true            {product}           
     bool ResizePLAB                                = true            {product}           
     bool ResizeTLAB                                = true            {pd product}        
     bool RestoreMXCSROnJNICalls                    = false           {product}           
     bool RewriteBytecodes                          = true            {pd product}        
     bool RewriteFrequentPairs                      = true            {pd product}        
     intx SafepointPollOffset                       = 256             {C1 pd product}     
     intx SafepointSpinBeforeYield                  = 2000            {product}           
     bool SafepointTimeout                          = false           {product}           
     intx SafepointTimeoutDelay                     = 10000           {product}           
     bool ScavengeBeforeFullGC                      = true            {product}           
     intx ScavengeRootsInCode                       = 2               {diagnostic}        
     intx SelfDestructTimer                         = 0               {product}           
     bool SerializeVMOutput                         = true            {diagnostic}        
    uintx SharedDummyBlockSize                      = 536870912       {product}           
    uintx SharedMiscCodeSize                        = 4194304         {product}           
    uintx SharedMiscDataSize                        = 6291456         {product}           
     bool SharedOptimizeColdStart                   = true            {diagnostic}        
    uintx SharedReadOnlySize                        = 10485760        {product}           
    uintx SharedReadWriteSize                       = 14680064        {product}           
     bool SharedSkipVerify                          = false           {diagnostic}        
     bool ShowMessageBoxOnError                     = false           {product}           
     intx SoftRefLRUPolicyMSPerMB                   = 1000            {product}           
     bool SplitIfBlocks                             = true            {product}           
     intx StackRedPages                             = 1               {pd product}        
     intx StackShadowPages                          = 20              {pd product}        
     bool StackTraceInThrowable                     = true            {product}           
     intx StackYellowPages                          = 2               {pd product}        
     bool StartAttachListener                       = false           {product}           
     intx StarvationMonitorInterval                 = 200             {product}           
     bool StressLdcRewrite                          = false           {product}           
     bool StressTieredRuntime                       = false           {product}           
    uintx StringTableSize                           = 1009            {product}           
     bool SuppressFatalErrorMessage                 = false           {product}           
    uintx SurvivorPadding                           = 3               {product}           
     intx SurvivorRatio                             = 8               {product}           
     intx SuspendRetryCount                         = 50              {product}           
     intx SuspendRetryDelay                         = 5               {product}           
     intx SyncFlags                                 = 0               {product}           
    ccstr SyncKnobs                                 =                 {product}           
     intx SyncVerbose                               = 0               {product}           
    uintx TLABAllocationWeight                      = 35              {product}           
    uintx TLABRefillWasteFraction                   = 64              {product}           
    uintx TLABSize                                  = 0               {product}           
     bool TLABStats                                 = true            {product}           
    uintx TLABWasteIncrement                        = 4               {product}           
    uintx TLABWasteTargetPercent                    = 1               {product}           
     intx TargetPLABWastePct                        = 10              {product}           
     intx TargetSurvivorRatio                       = 50              {product}           
    uintx TenuredGenerationSizeIncrement            = 20              {product}           
    uintx TenuredGenerationSizeSupplement           = 80              {product}           
    uintx TenuredGenerationSizeSupplementDecay      = 2               {product}           
     intx ThreadPriorityPolicy                      = 0               {product}           
     bool ThreadPriorityVerbose                     = false           {product}           
    uintx ThreadSafetyMargin                        = 52428800        {product}           
     intx ThreadStackSize                           = 1024            {pd product}        
    uintx ThresholdTolerance                        = 10              {product}           
     intx Tier0BackedgeNotifyFreqLog                = 10              {product}           
     intx Tier0InvokeNotifyFreqLog                  = 7               {product}           
     intx Tier0ProfilingStartPercentage             = 200             {product}           
     intx Tier1FreqInlineSize                       = 35              {C2 product}        
     intx Tier1Inline                               = 0               {C2 product}        
     intx Tier1LoopOptsCount                        = 0               {C2 product}        
     intx Tier1MaxInlineSize                        = 8               {C2 product}        
     intx Tier23InlineeNotifyFreqLog                = 20              {product}           
     intx Tier2BackEdgeThreshold                    = 0               {product}           
     intx Tier2BackedgeNotifyFreqLog                = 14              {product}           
     intx Tier2CompileThreshold                     = 0               {product}           
     intx Tier2InvokeNotifyFreqLog                  = 11              {product}           
     intx Tier3BackEdgeThreshold                    = 60000           {product}           
     intx Tier3BackedgeNotifyFreqLog                = 13              {product}           
     intx Tier3CompileThreshold                     = 2000            {product}           
     intx Tier3DelayOff                             = 2               {product}           
     intx Tier3DelayOn                              = 5               {product}           
     intx Tier3InvocationThreshold                  = 200             {product}           
     intx Tier3InvokeNotifyFreqLog                  = 10              {product}           
     intx Tier3LoadFeedback                         = 5               {product}           
     intx Tier3MinInvocationThreshold               = 100             {product}           
     intx Tier4BackEdgeThreshold                    = 40000           {product}           
     intx Tier4CompileThreshold                     = 15000           {product}           
     intx Tier4InvocationThreshold                  = 5000            {product}           
     intx Tier4LoadFeedback                         = 3               {product}           
     intx Tier4MinInvocationThreshold               = 600             {product}           
     bool TieredCompilation                         = false           {pd product}        
     intx TieredCompileTaskTimeout                  = 50              {product}           
     intx TieredRateUpdateMaxTime                   = 25              {product}           
     intx TieredRateUpdateMinTime                   = 1               {product}           
     intx TieredStopAtLevel                         = 4               {product}           
     bool TimeLinearScan                            = false           {C1 product}        
     bool TraceBiasedLocking                        = false           {product}           
     bool TraceClassLoading                         = false           {product rw}        
     bool TraceClassLoadingPreorder                 = false           {product}           
     bool TraceClassResolution                      = false           {product}           
     bool TraceClassUnloading                       = false           {product rw}        
     bool TraceCompileTriggered                     = false           {diagnostic}        
     bool TraceDynamicGCThreads                     = false           {product}           
     bool TraceGCTaskThread                         = false           {diagnostic}        
     bool TraceGen0Time                             = false           {product}           
     bool TraceGen1Time                             = false           {product}           
    ccstr TraceJVMTI                                =                 {product}           
     bool TraceJVMTIObjectTagging                   = false           {diagnostic}        
     bool TraceLoaderConstraints                    = false           {product rw}        
     bool TraceMonitorInflation                     = false           {product}           
     bool TraceNMethodInstalls                      = false           {diagnostic}        
     bool TraceOSRBreakpoint                        = false           {diagnostic}        
     bool TraceParallelOldGCTasks                   = false           {product}           
     intx TraceRedefineClasses                      = 0               {product}           
     bool TraceRedundantCompiles                    = false           {diagnostic}        
     bool TraceSafepointCleanupTime                 = false           {product}           
     bool TraceSuperWord                            = false           {C2 product}        
     bool TraceSuspendWaitFailures                  = false           {product}           
     bool TraceTriggers                             = false           {diagnostic}        
     intx TrackedInitializationLimit                = 50              {C2 product}        
     bool TransmitErrorReport                       = false           {product}           
     intx TypeProfileMajorReceiverPercent           = 90              {product}           
     intx TypeProfileWidth                          = 2               {product}           
     intx UnguardOnExecutionViolation               = 0               {product}           
     bool UnlinkSymbolsALot                         = false           {product}           
     bool UnlockDiagnosticVMOptions                := true            {diagnostic}        
     bool UnrollLimitCheck                          = true            {C2 diagnostic}     
     bool UnsyncloadClass                           = false           {diagnostic}        
     bool Use486InstrsOnly                          = false           {product}           
     intx UseAVX                                    = 0               {product}           
     bool UseAdaptiveGCBoundary                     = false           {product}           
     bool UseAdaptiveGenerationSizePolicyAtMajorCollection  = true            {product}           
     bool UseAdaptiveGenerationSizePolicyAtMinorCollection  = true            {product}           
     bool UseAdaptiveNUMAChunkSizing                = true            {product}           
     bool UseAdaptiveSizeDecayMajorGCCost           = true            {product}           
     bool UseAdaptiveSizePolicy                     = true            {product}           
     bool UseAdaptiveSizePolicyFootprintGoal        = true            {product}           
     bool UseAdaptiveSizePolicyWithSystemGC         = false           {product}           
     bool UseAddressNop                             = true            {product}           
     bool UseAltSigs                                = false           {product}           
     bool UseAutoGCSelectPolicy                     = false           {product}           
     bool UseBiasedLocking                          = true            {product}           
     bool UseBimorphicInlining                      = true            {C2 product}        
     bool UseBlockCopy                              = false           {product}           
     bool UseBlockZeroing                           = false           {product}           
     bool UseBoundThreads                           = true            {product}           
     bool UseBsdPosixThreadCPUClocks                = true            {product}           
     bool UseCBCond                                 = false           {product}           
     bool UseCMSBestFit                             = true            {product}           
     bool UseCMSCollectionPassing                   = true            {product}           
     bool UseCMSCompactAtFullCollection             = true            {product}           
     bool UseCMSInitiatingOccupancyOnly             = false           {product}           
     bool UseCodeCacheFlushing                      = true            {product}           
     bool UseCompiler                               = true            {product}           
     bool UseCompilerSafepoints                     = true            {product}           
     bool UseCompressedOops                        := true            {lp64_product}      
     bool UseConcMarkSweepGC                        = false           {product}           
     bool UseCondCardMark                           = false           {product}           
     bool UseCountLeadingZerosInstruction           = false           {product}           
     bool UseCounterDecay                           = true            {product}           
     bool UseDivMod                                 = true            {C2 product}        
     bool UseDynamicNumberOfGCThreads               = false           {product}           
     bool UseFPUForSpilling                         = false           {C2 product}        
     bool UseFastAccessorMethods                    = false           {product}           
     bool UseFastEmptyMethods                       = false           {product}           
     bool UseFastJNIAccessors                       = true            {product}           
     bool UseG1GC                                   = false           {product}           
     bool UseGCLogFileRotation                      = false           {product}           
     bool UseGCOverheadLimit                        = true            {product}           
     bool UseGCTaskAffinity                         = false           {product}           
     bool UseHeavyMonitors                          = false           {product}           
     bool UseHugeTLBFS                              = false           {product}           
     bool UseIncDec                                 = true            {diagnostic}        
     bool UseInlineCaches                           = true            {product}           
     bool UseInterpreter                            = true            {product}           
     bool UseJumpTables                             = true            {C2 product}        
     bool UseLWPSynchronization                     = true            {product}           
     bool UseLargePages                             = false           {pd product}        
     bool UseLargePagesIndividualAllocation         = false           {pd product}        
     bool UseLoopCounter                            = true            {product}           
     bool UseLoopPredicate                          = true            {C2 product}        
     bool UseMaximumCompactionOnSystemGC            = true            {product}           
     bool UseMembar                                 = true            {pd product}        
     bool UseNUMA                                   = false           {product}           
     bool UseNUMAInterleaving                       = false           {product}           
     bool UseNewCode                                = false           {diagnostic}        
     bool UseNewCode2                               = false           {diagnostic}        
     bool UseNewCode3                               = false           {diagnostic}        
     bool UseNewLongLShift                          = false           {product}           
     bool UseNiagaraInstrs                          = false           {product}           
     bool UseOSErrorReporting                       = false           {pd product}        
     bool UseOldInlining                            = true            {C2 product}        
     bool UseOnStackReplacement                     = true            {pd product}        
     bool UseOnlyInlinedBimorphic                   = true            {C2 product}        
     bool UseOprofile                               = false           {product}           
     bool UseOptoBiasInlining                       = true            {C2 product}        
     bool UsePPCLWSYNC                              = true            {product}           
     bool UsePSAdaptiveSurvivorSizePolicy           = true            {product}           
     bool UseParNewGC                               = false           {product}           
     bool UseParallelGC                            := true            {product}           
     bool UseParallelOldGC                          = true            {product}           
     bool UsePerfData                               = true            {product}           
     bool UsePopCountInstruction                    = true            {product}           
     bool UseRDPCForConstantTableBase               = false           {C2 product}        
     bool UseSHM                                    = false           {product}           
     intx UseSSE                                    = 4               {product}           
     bool UseSSE42Intrinsics                        = true            {product}           
     bool UseSerialGC                               = false           {product}           
     bool UseSharedSpaces                           = false           {product}           
     bool UseSignalChaining                         = true            {product}           
     bool UseSpinning                               = false           {product}           
     bool UseSplitVerifier                          = true            {product}           
     bool UseStoreImmI16                            = false           {product}           
     bool UseStringCache                            = false           {product}           
     bool UseSuperWord                              = true            {C2 product}        
     bool UseTLAB                                   = true            {pd product}        
     bool UseThreadPriorities                       = true            {pd product}        
     bool UseTypeProfile                            = true            {product}           
     bool UseUnalignedLoadStores                    = false           {product}           
     intx UseVIS                                    = 99              {product}           
     bool UseVMInterruptibleIO                      = false           {product}           
     bool UseVectoredExceptions                     = false           {pd product}        
     bool UseXMMForArrayCopy                        = true            {product}           
     bool UseXmmI2D                                 = false           {product}           
     bool UseXmmI2F                                 = false           {product}           
     bool UseXmmLoadAndClearUpper                   = true            {product}           
     bool UseXmmRegToRegMoveAll                     = true            {product}           
     bool VMThreadHintNoPreempt                     = false           {product}           
     intx VMThreadPriority                          = -1              {product}           
     intx VMThreadStackSize                         = 1024            {pd product}        
     intx ValueMapInitialSize                       = 11              {C1 product}        
     intx ValueMapMaxLoopSize                       = 8               {C1 product}        
     intx ValueSearchLimit                          = 1000            {C2 product}        
     bool VerifyAfterGC                             = false           {diagnostic}        
     bool VerifyBeforeExit                          = false           {diagnostic}        
     bool VerifyBeforeGC                            = false           {diagnostic}        
     bool VerifyBeforeIteration                     = false           {diagnostic}        
     bool VerifyDuringGC                            = false           {diagnostic}        
     intx VerifyGCLevel                             = 0               {diagnostic}        
    uintx VerifyGCStartAt                           = 0               {diagnostic}        
     bool VerifyMergedCPBytecodes                   = true            {product}           
     bool VerifyMethodHandles                       = false           {diagnostic}        
     bool VerifyObjectStartArray                    = true            {diagnostic}        
     bool VerifyRememberedSets                      = false           {diagnostic}        
     intx WorkAroundNPTLTimedWaitHang               = 1               {product}           
    uintx YoungGenerationSizeIncrement              = 20              {product}           
    uintx YoungGenerationSizeSupplement             = 80              {product}           
    uintx YoungGenerationSizeSupplementDecay        = 8               {product}           
    uintx YoungPLABSize                             = 4096            {product}           
     bool ZeroTLAB                                  = false           {product}           
     intx hashCode                                  = 0               {product}           
```
















