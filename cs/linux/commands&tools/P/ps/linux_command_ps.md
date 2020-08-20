

# tutorial



## 参数



* 指定输出的列/格式 `-o`

  如： `ps -o pcpu,pid,spid`

* 显示线程 `-T`

* 指定显示某个进程 `-p`

  如： `ps -p 1234`

* 显示所有进程 `-e`/`-A`

* 指定排序字段 `--sort`

  如： `--sort=+pid,-pcpu` 表示 按pid排序，同pid的按pcpu倒序排序



## 字段



```

```





### ruid, ruser: real



real uid...



### euid, euser: effective



### uid, user == effective



Alias for euid, euser



### pcpu / %cpu

cpu占比，但注意是根据 `cputime / uptime` 计算而来，所以得出的是某进程启动以来的cpu消耗情况，而不能反应该进程当前的cpu消耗情况。

ref： [stackexchange: top-and-ps-not-showing-the-same-cpu-result](https://unix.stackexchange.com/questions/58539/top-and-ps-not-showing-the-same-cpu-result)



```
   CPU usage is currently expressed as the percentage of time spent running
   during the entire lifetime of a process.  This is not ideal, and it does not
   conform to the standards that ps otherwise conforms to.  CPU usage is
   unlikely to add up to exactly 100%.
```



#### 计算方式



`ps` calculates CPU usage in the following manner:

```
uptime  = total time system has been running.
ps_time = process start time measured in seconds from boot.
pu_time = total time process has been using the CPU.

;; Seconds process has been running:
seconds   = uptime - ps_time
;; Usage:
cpu_usage = pu_time * 1000 / seconds

print: cpu_usage / 10 "." cpu_usage % 10

Example:

uptime  = 344,545
ps_time = 322,462
pu_time =   3,383

seconds   = 344,545 - 322,462 = 22,083
cpu_usage = 3,383 * 1,000 / 22,083 = 153

print: 153 / 10 "." 153 % 10 => 15.3
```



#### 数据来源



`ps`, as well as `top`, uses data from files stored under `/proc/` - or the [process information pseudo-file system](http://www.kernel.org/doc/man-pages/online/pages/man5/proc.5.html).

You have some files in root of `/proc/` that have various information about the overall state of the system. In addition each process has its own sub folder `/proc/<PID>/` where process specific data is stored. So, for example the process from your question had a folder at `/proc/3038/`.

When `ps` calculates CPU usage it uses two files:

```
/proc/uptime      The uptime of the system (seconds), and the amount of time spent in idle process (seconds).
/proc/[PID]/stat  Status information about the process.

```

- From `uptime` it uses the first value (*uptime*). 
- From `[PID]/stat` it uses the following:

```
 #  Name      Description
14  utime     CPU time spent in user code, measured in jiffies
15  stime     CPU time spent in kernel code, measured in jiffies
16  cutime    CPU time spent in user code, including time from children
17  cstime    CPU time spent in kernel code, including time from children 
22  starttime Time when the process started, measured in jiffies

```

A *jiffie* is clock tick. So in addition it uses various methods, ie., `sysconf(_SC_CLK_TCK)`, to get system's Hertz (number of ticks per second) - ultimately using 100 as a fall-back after exhausting other options.

So if `utime` is 1234 and Hertz is 100 then:

```
seconds = utime / Hertz = 1234 / 100 = 12.34

```

The actual calculation is done by:

```
total_time = utime + stime

IF include_dead_children
    total_time = total_time + cutime + cstime
ENDIF

seconds = uptime - starttime / Hertz

pcpu = (total_time * 1000 / Hertz) / seconds

print: "%CPU" pcpu / 10 "." pcpu % 10

```

Example (Output from a custom Bash script):

```
$ ./psw2 30894
System information
           uptime : 353,512 seconds
             idle : 0
Process information
              PID : 30894
         filename : plugin-containe
            utime : 421,951 jiffies 4,219 seconds
            stime : 63,334 jiffies 633 seconds
           cutime : 0 jiffies 0 seconds
           cstime : 1 jiffies 0 seconds
        starttime : 32,246,240 jiffies 322,462 seconds

Process run time  : 31,050
Process CPU time  : 485,286 jiffies 4,852 seconds
CPU usage since birth: 15.6%
```



#### 如何用ps来获得“当前使用cpu负荷”



ps -p  -o cputime,etimes

```
CODE    HEADER   DESCRIPTION
cputime TIME     cumulative CPU time, "[DD-]hh:mm:ss" format.  (alias time).
etime   ELAPSED  elapsed time since the process was started, [DD-]hh:]mm:ss.
etimes  ELAPSED  elapsed time since the process was started, in seconds.

```

I use `etime` over `etimes` in this sample, on calculations, only to be a bit more clear. Also I add %cpu for "fun". In i.e. a bash script one would obviously use `etimes` - or better read from `/proc/<PID>/` etc.

```
Start:
$ ps -p 30894 -o %cpu,cputime,etime,etimes
%CPU     TIME     ELAPSED ELAPSED
 5.9 00:13:55    03:53:56   14036

End:
%CPU     TIME     ELAPSED ELAPSED
 6.2 00:14:45    03:56:07   14167

Calculate times:
            13 * 60 + 55 =    835   (cputime this far)
3 * 3,600 + 53 * 60 + 56 = 14,036   (time running this far)

            14 * 60 + 45 =    885   (cputime at end)
3 * 3,600 + 56 * 60 +  7 = 14,167   (time running at end)

Calculate percent load:
((885 - 835) / (14,167 - 14,036)) * 100 = 38

```

Process was using the CPU 38% of the time during this period.



问题：

1. 计算的数值最小单位是秒，而如果想获取“最近1s”的cpu消耗占比就略尴尬了
2. 这里的TIME、ELAPSED考虑到多核了吗？ 是不是 `delta(TIME) / cores / delta(ELAPSED)` ？



### 启动时间



* `lstart` 精确的启动时间点，格式为`Fri Mar  4 16:04:27 2016`
* `etime` 精确的启动（后持续）时间，格式为`[[DD-]hh:]mm:ss` 如 `41-21:14:04`







## 场景



### 查看线程 `-T -p ${pid}`



## 典型用法



Ref: 

https://www.cnblogs.com/weifeng1463/p/8807849.html



### `ps aux`(bsd style)



参数含义

```s
a      Lift the BSD-style "only yourself" restriction, which is imposed upon the set of all processes 
when some BSD-style (without "-") options are used or when the ps personality setting is BSD-like.  
The set of processes selected in this manner is in addition to the set of processes selected by other means.
An alternate description is that this option causes ps to list all processes with a terminal (tty), 
or to list all processes when used together with the x option.

x      Lift the BSD-style "must have a tty" restriction, which is imposed upon the set of all processes 
when some BSD-style (without "-") options are used or when the ps personality setting is BSD-like.  
The set of processes selected in this manner is in addition to the set of processes selected by other means.  
An alternate description is that this option causes ps to list all processes owned by you (same EUID as ps), 
or to list all processes when used together with the a option.

u      Display user-oriented format.
```



输出字段

```s
USER       PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND
```



我们可以看到START和TIME列，通过 man 其说明如下：

```
bsdstart    START     time the command started.  
If the process was started less than 24 hours ago, the output format is " HH:MM", 
else it is " Mmm:SS" (where Mmm is the three letters of the month). 
See also lstart, start, start_time, and stime.

bsdtime     TIME      accumulated cpu time, user + system.  
The display format is usually "MMM:SS", 
but can be shifted to the right if the process used more than 999 minutes of cpu time.
```

START 是命令启动的时间，如果在 24 小时之内启动的，则输出格式为”HH:MM”（小时：分钟），
否则就是”Mmm:SS”（月份英语单词前 3 个字母：一月的第几号？[SS 这里面怎么理解？为什么有冒号呢？输出并没冒号]） 可以知道，这里并不能直接看出 24 小时之前启动的命令的精确启动时间。

TIME 是累积的 CPU 时间（user+system），显示格式通常是”MMM:SS”。（分钟：秒） 可以看出，这里并不是指从命令启动开始到现在所花的时间。



### `ps -ef`(unix style)



参数含义

```s
-e     Select all processes.  Identical to -A.

-f     Do full-format listing. 
This option can be combined with many other UNIX-style options to add additional columns.  
It also causes the command arguments to be printed.  
When used with -L, the NLWP (number of threads) and LWP (thread ID) columns will be added.  
See the c option, the format keyword args, and the format keyword comm.

-o format     User-defined format.  
format is a single argument in the form of a blank-separated or comma-separated list, 
which offers a way to specify individual output columns.  
The recognized keywords are described in the STANDARD FORMAT SPECIFIERS section below.  

Headers may be renamed (ps -o pid,ruser=RealUser -o comm=Command) as desired.  
If all column headers are empty (ps -o pid= -o comm=) then the header line will not be output.  
Column width will increase as needed for wide headers; 
this may be used to widen up columns such as WCHAN (ps -o pid,wchan=WIDE-WCHAN-COLUMN -o comm).  
Explicit width control (ps opid,wchan:42,cmd) is offered too.  
The behavior of ps -o pid=X,comm=Y varies with personality; 
output may be one column named "X,comm=Y" or two columnsnamed "X" and "Y".  
Use multiple -o options when in doubt.  
Use the PS_FORMAT environment variable to specify a default as desired; 
DefSysV and DefBSD are macros that may be used to choose the default UNIX or BSD columns.       
```



输出字段

```s
UID        PID  PPID  C STIME TTY          TIME CMD
```



我们可以看到 STIM E和 TIME 列，通过 man 其说明如下 (我这台服务器上 ps 版本为 procps-ng version 3.3.9，man 中找不到 STIME 的解释，通过观察输出，
我们可以推断这个 STIME 其实和前面 START 是一样的，指的是命令启动的时间，[这里](http://www.linfo.org/ps.html)有这个说明）：

TIME 列也和前面说的 TIME 列一样指的命令使用的累积 CPU 时间。





