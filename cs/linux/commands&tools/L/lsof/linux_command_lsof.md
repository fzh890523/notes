



# manual





## basic



* `lsof ${filename}`

  基本用法，列出打开该指定文件的进程

  ```sh
  #lsof xxx.log
  COMMAND    PID  USER   FD   TYPE DEVICE SIZE/OFF     NODE NAME
  cmd1    142917 admin   33w   REG    8,5  1466629 70408201 xxx.log
  cmd1    142939 admin   33w   REG    8,5  1466629 70408201 xxx.log
  cmd1    142940 admin   33w   REG    8,5  1466629 70408201 xxx.log
  cmd2  158590  root    6w   REG    8,5  1466629 70408201 xxx.log
  ```

  







## Mac -h help info



```
lsof 4.89
 latest revision: ftp://lsof.itap.purdue.edu/pub/tools/unix/lsof/
 latest FAQ: ftp://lsof.itap.purdue.edu/pub/tools/unix/lsof/FAQ
 latest man page: ftp://lsof.itap.purdue.edu/pub/tools/unix/lsof/lsof_man
 usage: [-?abhlnNoOPRtUvV] [+|-c c] [+|-d s] [+D D] [+|-f[cgG]]
 [-F [f]] [-g [s]] [-i [i]] [+|-L [l]] [+|-M] [-o [o]] [-p s]
 [+|-r [t]] [-s [p:s]] [-S [t]] [-T [t]] [-u s] [+|-w] [-x [fl]] [--] [names]
Defaults in parentheses; comma-separated set (s) items; dash-separated ranges.
  -?|-h list help          -a AND selections (OR)     -b avoid kernel blocks
  -c c  cmd c ^c /c/[bix]  +c w  COMMAND width (9)    +d s  dir s files
  -d s  select by FD set   +D D  dir D tree *SLOW?*   -i select IPv[46] files
  -l list UID numbers      -n no host names           -N select NFS files
  -o list file offset      -O no overhead *RISKY*     -P no port names
  -R list paRent PID       -s list file size          -t terse listing
  -T disable TCP/TPI info  -U select Unix socket      -v list version info
  -V verbose search        +|-w  Warnings (+)         -- end option scan
  +f|-f  +filesystem or -file names     +|-f[cgG] Ct flaGs
  -F [f] select fields; -F? for help
  +|-L [l] list (+) suppress (-) link counts < l (0 = all; default = 0)
  +|-M   portMap registration (-)       -o o   o 0t offset digits (8)
  -p s   exclude(^)|select PIDs         -S [t] t second stat timeout (15)
  -T fqs TCP/TPI Fl,Q,St (s) info
  -g [s] exclude(^)|select and print process group IDs
  -i i   select by IPv[46] address: [46][proto][@host|addr][:svc_list|port_list]
  +|-r [t[m<fmt>]] repeat every t seconds (15);  + until no files, - forever.
       An optional suffix to t is m<fmt>; m must separate t from <fmt> and
      <fmt> is an strftime(3) format for the marker line.
  -s p:s  exclude(^)|select protocol (p = TCP|UDP) states by name(s).
  -u s   exclude(^)|select login|UID set s
  -x [fl] cross over +d|+D File systems or symbolic Links
  names  select named files or files on named file systems
Anyone can list all files; /dev warnings disabled; kernel ID check disabled.
```



## output



lsof输出各列信息的意义如下：

* COMMAND：进程的名称 PID：进程标识符

* USER：进程所有者

* FD：文件描述符，应用程序通过文件描述符识别该文件。如cwd、txt等 
  * FD 列中的文件描述符cwd 值表示应用程序的当前工作目录，这是该应用程序启动的目录，除非它本身对这个目录进行更改,txt 类型的文件是程序代码，如应用程序二进制文件本身或共享库，如上列表中显示的 /sbin/init 程序。
  * 其次数值表示应用程序的文件描述符，这是打开该文件时返回的一个整数。如上的最后一行文件/dev/initctl，其文件描述符为 10。u  表示该文件被打开并处于读取/写入模式，而不是只读 ® 或只写 (w) 模式。同时还有大写 的W  表示该应用程序具有对整个文件的写锁。该文件描述符用于确保每次只能打开一个应用程序实例。初始打开每个应用程序时，都具有三个文件描述符，从 0 到  2，分别表示标准输入、输出和错误流。所以大多数应用程序所打开的文件的 FD 都是从 3 开始。

* TYPE：文件类型，如DIR、REG等

  与 FD 列相比，Type 列则比较直观。文件和目录分别称为 REG 和 DIR。而CHR 和 BLK，分别表示字符和块设备；或者 UNIX、FIFO 和 IPv4，分别表示 UNIX 域套接字、先进先出 (FIFO) 队列和网际协议 (IP) 套接字。

* DEVICE：指定磁盘的名称

* SIZE：文件的大小

* NODE：索引节点（文件在磁盘上的标识）

* NAME：打开文件的确切名称





与 FD 列相比，Type 列则比较直观。文件和目录分别称为 REG 和 DIR。而CHR 和 BLK，分别表示字符和块设备；或者 UNIX、FIFO 和 IPv4，分别表示 UNIX 域套接字、先进先出 (FIFO) 队列和网际协议 (IP) 套接字。





## options











# usage



## 通过lsof找到/恢复已删除文件



1. 找到

   ```shell
   lsof -nP | grep '(deleted)'

   # 在linux上还可以
   find /proc/*/fd -ls | grep  '(deleted)'
   ```

2. 恢复

   ```shell
   cp /proc/$pid/fd/$fd ${target_path}
   ```

   > Copying from `/proc/*PID*/fd/` only works if the process currently has the file open. It can't do anything about a file that the process had open at some point in the past but no longer does. A program has no need to keep its configuration file open: once it's read its configuration, it closes the file. You aren't going to recover anything that way. The file you're attempting to recover is an unrelated file; it's a network socket that the process is listening on, and you can't read data from a socket like this, not that this would do you any good.
   >
   > `gcore 30495` [as suggested by Mark Plotnick](https://unix.stackexchange.com/questions/268247/recover-files-if-still-being-used-by-a-process#comment464904_268247) would dump a memory image of the process in the file `core.30495`. You could try to sift through that memory image, but don't get your hopes up: there's a good chance that the process has parsed the configuration file and reused whatever memory it stored the file in for other purposes. It's even likely that the whole configuration file was never entirely in memory, only piece by piece and each piece overwrote the previous one.

   










## 查看是否监听端口



```sh
lsof -nP -i:80 -sTCP:LISTEN
lsof -nP -iTCP:80 -sTCP:LISTEN

lsof -nP -iTCP -sTCP:LISTEN | grep 80
```





## 查看用户打开的文件
















