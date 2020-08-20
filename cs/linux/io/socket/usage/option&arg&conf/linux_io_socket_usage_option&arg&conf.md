

# sysctl参数



参考： 

* [man-tcp.7](http://man7.org/linux/man-pages/man7/tcp.7.html) or [local](resource/tcp(7) - Linux manual page.html)





## tcp_mem (since Linux 2.4)



### 解释



tcp_mem (since Linux 2.4)

> This is a vector of 3 integers: `[low, pressure, high]`.  
>
> These bounds, measured in units of the system page size, are used by TCP to track its memory usage.  
>
> The defaults are calculated at boot time from the amount of available memory.  (TCP can only use low memory for this, which is limited to around 900 megabytes on 32-bit systems.  64-bit systems do not suffer this limitation.)



> * low
>
>   TCP doesn't regulate its memory allocation when the number of pages it has allocated globally is below this number.    
>
> * pressure 
>
>   When the amount of memory allocated by TCP exceeds this number of pages, TCP moderates its memory consumption.  
>
>   This memory pressure state is exited once the number of pages allocated falls below the low mark.
>
> * high      
>
>   The maximum number of pages, globally, that TCP willallocate. 
>
>   This value overrides any other limitsimposed by the kernel.



Ref: [TCP performance tuning - how to tune linux](https://www.acc.umu.se/~maswan/linux-netperf.txt) or local

```
If you go up above the middle value of net/ipv4/tcp_mem, you enter 
tcp_memory_pressure, which means that new tcp windows won't grow until 
you have gotten back under the pressure value. Allowing bigger windows means
that it takes fewer connections for someone evil to make the rest of the
tcp streams to go slow.
```



middle一定时，win越大，那么越可能被少数“坏”链接把允许的mem都吃掉从而影响其他链接（win不能增长了）。



### 相关排查

- `cat /proc/sys/net/ipv4/tcp_mem`

  输出类似：

  ```shell
  3093984 4125312 6187968
  ```

- `sysctl -a | grep tcp_mem`

- `cat /proc/net/sockstat`

  输出类似：

  ```shell
  sockets: used 14565
  TCP: inuse 35938 orphan 21564 tw 70529 alloc 35942 mem 1894
  UDP: inuse 11 mem 3
  UDPLITE: inuse 0
  RAW: inuse 0
  FRAG: inuse 0 memory 0
  ```

  注意那个 **mem**

> 配置和当前值的单位都是页。





## tcp_max_orphans (integer; default: see below; since Linux 2.4)



```
The maximum number of orphaned (not attached to any user file handle) TCP sockets allowed in the system.  

When this number is exceeded, the orphaned connection is reset and a warning is printed.  

This limit exists only to prevent simple denial-of-service attacks.  

Lowering this limit is not recommended.

Network conditions might require you to increase the number of orphans allowed, but note that each orphan can eat up to ~64K of unswappable memory.  

The default initial value is set equal to the kernel parameter NR_FILE.  

This initial default is adjusted depending on the memory in the system.
```













