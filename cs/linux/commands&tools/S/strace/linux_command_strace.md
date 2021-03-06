
# 常见场景

## 多线程跟踪

`-f`参数
如：`strace -fp 11001 -e open,mmap,stat`



## 多进程跟踪



`-f`





# 常见问题



### ptrace: Operation not permitted

主要redhat在fedora22之后的版本中，引入了一种叫做ptrace scope的安全机制。这种机制为了防止用户访问当前正在运行的进程的内存和状态，所以在调试程序的过程中导致gdb不能正常工作。这种安全机制可以防止恶意软件附加到其他进程中（如SSH或者GPG），读取程序内存，产生安全问题。比如著名的openssl的"心脏出血"漏洞。

其解决方法有两种，都需要root权限进行操作
（1）临时方法

将/proc/sys/kernel/yama/ptrace_scope虚拟文件的内容设为0。

echo 0 > /proc/sys/kernel/yama/ptrace_scope

> 或者 sysctl -w kernel.yama.ptrace_scope=0 && sysctl -p
>
> > yonka: 这里多执行个-p似乎无意义

重启之后失效。

（2）永久解决

编辑/etc/sysctl.d/10-ptrace.conf这个文件，若没有，创建之。设置（默认是1）

kernel.yama.ptrace_scope = 0

> 重启后才生效

