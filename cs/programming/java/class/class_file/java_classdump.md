ref：

* [从Java进程里dump出类的字节码文件](http://www.cnblogs.com/wade-luffy/p/7080541.html)

* [如何dump出一个Java进程里的类对应的Class文件？](http://rednaxelafx.iteye.com/blog/727938) or [local](resource/如何dump出一个Java进程里的类对应的Class文件？ - Script Ahead, Code Behind - ITeye博客.html)



# dump出字节码​



## dynamic attach + instrument API

用agent attatch 到进程，然后利用Instrumentation和ClassFileTransformer就可以获取到类的字节码了。





## SA-jdi

SA系的都要求 source和target 的jdk版本一致，这点要注意。



### ClassDump



用sd-jdi.jar里的工具。`sd-jdi.jar`里自带的的`sun.jvm.hotspot.tools.jcore.ClassDump`就可以把类的class内容dump到文件里。



ClassDump里可以设置两个System properties：

1. `sun.jvm.hotspot.tools.jcore.filter`           Filter的类名
2. `sun.jvm.hotspot.tools.jcore.outputDir`    输出的目录

sd-jdi.jar 里有一个`sun.jvm.hotspot.tools.jcore.PackageNameFilter`，可以指定Dump哪些包里的类。`PackageNameFilter`里有一个`System property`可以指定过滤哪些包：`sun.jvm.hotspot.tools.jcore.PackageNameFilter.pkgList`。

可以通过这样子的命令来使用：

```shell
sudo java -classpath "$JAVA_HOME/lib/sa-jdi.jar" -Dsun.jvm.hotspot.tools.jcore.filter=sun.jvm.hotspot.tools.jcore.PackageNameFilter -Dsun.jvm.hotspot.tools.jcore.PackageNameFilter.pkgList=cn.sf  sun.jvm.hotspot.tools.jcore.ClassDump
```



### HSDB



ClassDump使用起来比较麻烦。在sa-jdi.jar里，还有一个图形化的工具HSDB，也可以用来查看运行的的字节码。sudo java -classpath "$JAVA_HOME/lib/sa-jdi.jar" sun.jvm.hotspot.HSDB




