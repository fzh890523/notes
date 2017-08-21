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

  ​



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

  ​

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

  ​

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

  TODO 内存分配池是什么？

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

  TODO 怎么个追踪法？

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

  TODO 资源管理都干了啥？

* `-XX:ResourceManagementSampleInterval=${value} (milliseconds)`

  设置资源管理功能的采样间隔，单位毫秒。

  前提： 开启资源管理功能（`-XX:+ResourceManagement`）

  TODO 默认值？

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

  TODO 怎么个trace法？ 然后呢？打印吗？

* `-XX:+TraceClassLoadingPreorder`

  启用特性： 以引用顺序来追踪类加载过程。

  默认禁用。

* `-XX:+TraceClassResolution`

  启用对常量池resolutions的追踪。

  默认禁用。

  TODO 常量池resolution是个啥？

* `-XX:+TraceClassUnloading`

  启用对类卸载的追踪。

  默认禁用。

* `-XX:+TraceLoaderConstraints`

  启用对` loader constraints recording`的追踪。

  默认禁用。

  TODO ` loader constraints recording`是个什么鬼？ 类加载器约束记录？

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

  > 应该是code cache剩余空间吧？

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

  默认禁用 - 不记录…。 需要和`-XX:UnlockDiagnosticVMOptions`一起使用才能生效。

* `-XX:MaxInlineSize=${size}`

  设置方法被内联的bytecode大小门限值（小于...则...）。

  单位是byte，可以指定其他单位。

  默认为35byte。

  TODO 跟MaxTrivialSize的区别是啥？

* `-XX:MaxNodeLimit=${nodes}`

  设置单方法编译时使用的最大node数量。

  默认为65000。

  TODO 了解下编译时node的概念功用。

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

  TODO 啥意思？

  默认启用。可以通过`-`来禁用。

  只有Hotspot server VM支持。

#### 高级Serviceability选项

* `-XX:+EntendedDTraceProbes`

  启用扩展的`dtrace`工具探针（会影响性能）。

  默认禁用该特性，也即只使用标准探针。

  TODO 确认下linux java的dtrace功能，linux目前所知不支持dtrace只是支持了类似dtrace的功能。

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

  TODO 这是指啥log？

* `-XX:+PrintClassHistogram`

  启用特性： 在`SIGTERM`时打印类实例的柱状图。 效果类似： `jmap -histo` 或者 `jcmd ${pid} GC.class_histogram`。

  默认禁用。

* `-XX:+PrintConcurrentLocks`

  开启特性：在`SIGTERM`时打印`java.util.concurrent`锁。

  默认禁用。

* `-XX:+UnlockDiagnosticVMOptions`

  解锁那些用于诊断JVM的选项。

  默认禁用，也即这些选项不可用。

#### 高级GC选项

* `-XX:+AggressiveHeap`

  启用java堆优化，会根据内存和CPU设置一些对 *长时间运行的会密集分配内存的任务* 进行优化的参数。

  默认禁用 - 不做优化。

  TODO 哪些参数？ 怎么优化？

* `-XX:+AlwaysPreTouch`

  在JVM初始化（在进入main之前）时touch每个java堆的页，可以用于模拟长时间运行的任务 - 所有内存都在物理内存中 的场景。

  默认禁用。

* `-XX:+CMSClassUnloadingEnabled`

  当使用CMS时启用类卸载。

  默认启用，可以通过 `-XX:-CMSClassUnloadingEnabled`来禁用。

* `-XX:CMSExpAvgFactor=${percent}`

  Sets the percentage of time (0 to 100) used to weight the current sample when computing exponential averages for the concurrent collection statistics. 

  TODO 啥意思？

  默认25%。

* `-XX:CMSInitiatingOccupancyFraction=${percent}`

  设置发起CMS GC的触发条件之 *老年代占用比例*。 负数表示使用`-XX:CMSTriggerRatio`来确定该值。

  默认为`-1`。

* `-XX:+CMSScavengeBeforeRemark`

  启用CMS重标记（remark）步骤前的搜寻（scavenging）尝试。

  TODO 结合CMS流程熟悉一下。

  默认禁用。

* `-XX:CMSTriggerRatio=${percent}`

  设置 当分配的内存到达`-XX:MinHeapFreeRatio`设置的值的该比例时触发CMS GC。

  默认为80%。

  TODO 跟MinHeapFreeRatio怎么结合起来的？ 没看明白。

* `-XX:ConcGCThreads=${threads}`

  设置并发GC（方式）的并发度（GC线程数），默认是根据CPU数计算得到。

* `-XX:+DisableExplicitGC`

  禁止显式GC（即`System.gc()`）。

  默认禁用该选项，也即允许显式GC。

  > 即使禁止了，JVM自己还是可以在需要时触发。

* `-XX:+ExplicitGCInvokesConcurrent`

  启用通过显式调用（`System.gc()`）来触发并发GC。

  默认禁用，启用时需要和 `-XX:+UseConcMarkSweepGC` 一起使用。

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

  参见详解。

* `-XX:InitiatingHeapOccupancyPercent=${percent}`

  设置并发GC触发条件之 *堆占用比例*（整个堆，而不是某个代的）。

  默认为45%，0表示非停顿GC。

  TODO 哪些GC收集器会使用这个？ 0为什么表示非停顿GC？

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

  TODO ` tenuring threshold`是啥？ 效果是什么？

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

  打印并行引用处理信息。

  默认禁用。

  TODO 啥？

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
  > TODO 上面这句没看懂，减少 old区引用的young区对象？

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
  > TODO

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

  TODO 了解下NUMA。

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

  TODO 了解大页


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

​设置： 对所有膨胀锁使用RTM（受限事务内存），失败时回退到普通的锁机制。

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

  TODO 方法的选项？

  可以逗号或者空格来分隔多个选项。

- print

  打印指定方法编译后得到的汇编代码。

- quiet

  不打印编译命令。

  默认会打印该选项指定的所有命令，类似：

  ```
  CompilerOracle: exclude java/lang/String.indexOf
  ```

  ​



# 引申



## 概念



### intrinsics

* 内联函数
* 内嵌原语
* 编译器内部函数
* 编译器固有支持

大致含义就是： 

> 某底层实现对某个特定功能特性有原生支持，无需上层（用底层通用原语/指令）实现，可以提高性能减小开销等。











