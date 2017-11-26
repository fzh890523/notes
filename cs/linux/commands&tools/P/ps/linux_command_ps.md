

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







