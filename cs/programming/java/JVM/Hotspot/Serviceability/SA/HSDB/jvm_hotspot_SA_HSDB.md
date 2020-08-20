# 概述

SA提供的入口在`$JAVA_HOME/lib/sa-jdi.jar`中，最常用的工具就是`HSDB(Hotspot Debugger)`了，HSDB可以查看Java对象的oops、查看类信息、线程栈信息、堆信息、方法字节码和JIT编译后的汇编代码等，可以说非常底层全面。





# 功能



tools

* Class Browser
* Code Viewer
* Compute Reverse Ptrs
* Deadlock Detection
* Find Object by Query
* Find Pointer
* Find Value in Heap
* Find Value in Code Cache
* Heap Parameters
* Inspector
* Memory Viewer
* Monitor Cache Dump
* Object Histogram
* Show System Process
* Show VM Version
* Show -XX flags



# 使用



```Shell
java -cp ${JAVA_HOME}/lib/sa-jdi.jar sun.jvm.hotspot.HSDB
```

> mac上需要sudo = =





# CLHSDB



在服务器上可能没有窗口界面，CLHSDB(command line Hotspot Debugger)可以提供与HSDB相同的功能。

> 注意： 以linux为例，traced进程无法再被trace，所以CLHSDB和HSDB不能同时进行。



```shell
java -cp /usr/local/jdk1.8.0_45/lib/sa-jdi.jar sun.jvm.hotspot.CLHSDB
```

输入help可以查看可以使用的命令

然后通过attach pid就可以attach到一个进程上, 例如通过threads查看进程列表



```shell
java -cp ${JAVA_HOME}/lib/sa-jdi.jar sun.jvm.hotspot.CLHSDB                                 feature_diag-profile
hsdb> help
Available commands:
  assert true | false
  attach pid | exec core
  buildreplayjars [ all | app | boot ]  | [ prefix ]
  detach
  dis address [length]
  disassemble address
  dumpcfg { -a | id }
  dumpcodecache
  dumpideal { -a | id }
  dumpilt { -a | id }
  dumpreplaydata { <address > | -a | <thread_id> }
  echo [ true | false ]
  examine [ address/count ] | [ address,address]
  field [ type [ name fieldtype isStatic offset address ] ]
  findpc address
  flags [ flag | -nd ]
  help [ command ]
  history
  inspect expression
  intConstant [ name [ value ] ]
  jdis address
  jhisto
  jstack [-v]
  livenmethods
  longConstant [ name [ value ] ]
  pmap
  print expression
  printall
  printas type expression
  printmdo [ -a | expression ]
  printstatics [ type ]
  pstack [-v]
  quit
  reattach
  revptrs address
  scanoops start end [ type ]
  search [ heap | perm | rawheap | codecache | threads ] value
  source filename
  symbol address
  symboldump
  symboltable name
  thread { -a | id }
  threads
  tokenize ...
  type [ type [ name super isOop isInteger isUnsigned size ] ]
  universe
  verbose true | false
  versioncheck [ true | false ]
  vmstructsdump
  where { -a | id }
```





