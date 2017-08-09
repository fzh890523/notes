# 远程debug



类似：

```
-agentlib:jdwp=transport=dt_socket,server=y,suspend=n,address=54242
```

* socket
* attach
* localhost
* 54242

加到要（被）debug的JVM进程的启动参数（JAVA_OPTS）。



## 远程debug 和 debug的冲突

远程debug用的是jdwp机制，而IDE debug本身也是用 jdwp agent 的机制，所以如果debug模式启动的程序，再远程debug时会报错： `ERROR: Cannot load this JVM TI agent twice, check your java command line for duplicate jdwp options.`













