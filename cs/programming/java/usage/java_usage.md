# 运行环境



## mac/OSX



### 多版本



#### 软链接方式

`ln -f /Library/Java/JavaVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/bin/java ~/bin/java8`



```
java8 -v
Error: could not find libjava.dylib
Error: Could not find Java SE Runtime Environment.

/Library/Java/JavaVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/bin/java -version
java version "1.8.0_121"
Java(TM) SE Runtime Environment (build 1.8.0_121-b13)
Java HotSpot(TM) 64-Bit Server VM (build 25.121-b13, mixed mode)
```



似乎是把`soft link`所在目录视为...了，于是找不到相对路径下的动态链接库。 = =



#### alias方式

```shell
alias java8='/Library/Java/JavaVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/bin/java'
alias javac8='/Library/Java/JavaVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/bin/javac'
```



OK



















