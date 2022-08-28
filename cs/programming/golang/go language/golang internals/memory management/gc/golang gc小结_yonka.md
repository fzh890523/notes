------

Plans for Go 1.4+ garbage collector:

- hybrid stop-the-world/concurrent collector
- stop-the-world part limited by a 10ms deadline
- CPU cores dedicated to running the concurrent collector
- tri-color mark-and-sweep algorithm
- non-generational
- non-compacting
- fully precise
- incurs a small cost if the program is moving pointers around
- lower latency, but most likely also lower throughput, than Go 1.3 GC

Go 1.3 garbage collector updates on top of Go 1.1:

- concurrent sweep (results in smaller pause times)
- fully precise

Go 1.1 garbage collector:

- mark-and-sweep (parallel implementation)
- non-generational
- non-compacting
- mostly precise (except stack frames)
- stop-the-world
- bitmap-based representation
- zero-cost when the program is not allocating memory (that is: shuffling pointers around is as fast as in C, although in practice this runs somewhat slower than C because the Go compiler is not as advanced as C compilers such as GCC)
- supports finalizers on objects
- there is no support for weak references

Go 1.0 garbage collector:

- same as Go 1.1, but instead of being mostly precise the garbage collector is conservative. The conservative GC is able to ignore objects such as []byte.

Replacing the GC with a different one is controversial, for example:

- except for very large heaps, it is unclear whether a generational GC would be faster overall

- package "unsafe" makes it hard to implement fully precise GC and compacting GC

  

  来源： <<http://stackoverflow.com/questions/7823725/what-kind-of-garbage-collection-does-go-use>>

   

------







# golang GC诊断



## GC log

ref： https://my.oschina.net/u/2374678/blog/799477



```
gc 1 @2.104s 0%: 0.018+1.3+0.076 ms clock, 0.054+0.35/1.0/3.0+0.23 ms cpu, 4->4->3 MB, 5 MB goal, 4 P
gc 2 @2.241s 0%: 0.019+2.4+0.077 ms clock, 0.079+0/2.4/6.4+0.30 ms cpu, 5->6->5 MB, 6 MB goal, 4 P
gc 3 @2.510s 0%: 0.011+3.2+0.063 ms clock, 0.047+0.10/2.9/9.0+0.25 ms cpu, 11->11->10 MB, 12 MB goal, 4 P
gc 4 @3.021s 0%: 0.013+6.6+0.076 ms clock, 0.053+0.34/6.2/18+0.30 ms cpu, 21->21->20 MB, 22 MB goal, 4 P
gc 5 @3.725s 0%: 0.015+15+0.079 ms clock, 0.062+0.35/15/45+0.31 ms cpu, 40->40->39 MB, 41 MB goal, 4 P
gc 6 @4.741s 0%: 0.008+35+0.17 ms clock, 0.035+0.19/35/100+0.70 ms cpu, 76->76->75 MB, 78 MB goal, 4 P
gc 7 @6.688s 0%: 0.020+117+0.34 ms clock, 0.082+11/117/330+1.3 ms cpu, 147->148->146 MB, 151 MB goal, 4 P
gc 8 @68.645s 0%: 0.019+146+0.30 ms clock, 0.078+0.006/146/407+1.2 ms cpu, 285->285->248 MB, 292 MB goal, 4 P
scvg0: inuse: 426, idle: 0, sys: 427, released: 0, consumed: 427 (MB)
gc 9 @175.448s 0%: 0.030+60+0.12 ms clock, 0.12+0.013/60/177+0.51 ms cpu, 484->484->248 MB, 496 MB goal, 4 P
gc 10 @236.621s 0%: 0.006+59+0.11 ms clock, 0.025+0/59/173+0.47 ms cpu, 484->484->248 MB, 496 MB goal, 4 P
gc 11 @285.967s 0%: 0.027+57+0.22 ms clock, 0.11+0/57/163+0.89 ms cpu, 484->484->248 MB, 496 MB goal, 4 P
scvg1: inuse: 331, idle: 175, sys: 507, released: 0, consumed: 507 (MB)
gc 12 @333.817s 0%: 0.009+52+0.18 ms clock, 0.036+0/52/155+0.72 ms cpu, 484->484->248 MB, 496 MB goal, 4 P
```



```sh
gc 2 @2.241s 0%: 0.019+2.4+0.077 ms clock, 0.079+0/2.4/6.4+0.30 ms cpu, 5->6->5 MB, 6 MB goal, 4 P
    
gc # 前缀
2 # gc编号，次数
@2.241s # 程序启动时间
0%: # 启动后花在gc上的时间的比例 （wall time？）
0.019+2.4+0.077 ms clock,  # 不同阶段的gc时间（wall lock），三个阶段： stop-the-world (STW) sweep termination, concurrent mark and scan, and STW mark termination。 1 和 3是stw的
0.079+0/2.4/6.4+0.30 ms cpu,  # 不同阶段的gc cpu时间  # ... CMS又拆分为...
5->6->5 MB,  # gc前后的heap size和 live heap
6 MB goal, 
4 P  # 用了几个核
```



> The GC cleans some amount of garbage each pass. It does not necessarily release it to the OS (if it thinks it would just have to request it again shortly); and if it does, the OS does not necessarily reclaim it (until there is memory pressure from another process, the OS may leave that memory allocated to your process in case it needs it again).
>
> Live heap size is how much of the heap is actively in use, less any dead objects and free heap space ready for future allocations. Goal heap size is how much memory GC thinks it needs to get from the OS to handle your process's allocations on an ongoing basis without having to constantly request more memory from the OS (i.e. how much stays alive + how much is allocated & discarded between GC runs).
>
> The goal of GC is to clean up dead objects in the heap, **and** to maintain enough free heap space to handle most allocations without having to request more memory from the OS (which is slow), while also not keeping excessive free memory (so that the OS can still allocate to other processes).



```
gctrace: setting gctrace=1 causes the garbage collector to emit a single line to standard
error at each collection, summarizing the amount of memory collected and the
length of the pause. Setting gctrace=2 emits the same summary but also
repeats each collection. The format of this line is subject to change.
Currently, it is:
	gc # @#s #%: #+#+# ms clock, #+#/#/#+# ms cpu, #->#-># MB, # MB goal, # P
where the fields are as follows:
	gc #        the GC number, incremented at each GC
	@#s         time in seconds since program start
	#%          percentage of time spent in GC since program start
	#+...+#     wall-clock/CPU times for the phases of the GC
	#->#-># MB  heap size at GC start, at GC end, and live heap
	# MB goal   goal heap size
	# P         number of processors used
The phases are stop-the-world (STW) sweep termination, concurrent
mark and scan, and STW mark termination. The CPU times
for mark/scan are broken down in to assist time (GC performed in
line with allocation), background GC time, and idle GC time.
If the line ends with "(forced)", this GC was forced by a
runtime.GC() call and all phases are STW.

Setting gctrace to any value > 0 also causes the garbage collector
to emit a summary when memory is released back to the system.
This process of returning memory to the system is called scavenging.
The format of this summary is subject to change.
Currently it is:
	scvg#: # MB released  printed only if non-zero
	scvg#: inuse: # idle: # sys: # released: # consumed: # (MB)
where the fields are as follows:
	scvg#        the scavenge cycle number, incremented at each scavenge
	inuse: #     MB used or partially used spans
	idle: #      MB spans pending scavenging
	sys: #       MB mapped from the system
	released: #  MB released to the system
	consumed: #  MB allocated from the system
```





### 开启gc log

* 环境变量 `GODEBUG`

  `GODEBUG gctrace=1`



## 触发GC

* 自动触发

  * `GOGC`

  * `SetMaxHeap`  - ongoing

    ref:
    
    * [proposal: runtime: add a mechanism for specifying a minimum target heap size #23044](https://github.com/golang/go/issues/23044)
    * [golang-codereviews › [go] runtime/debug: add SetMaxHeap API](https://groups.google.com/forum/#!topic/golang-codereviews/brkajcJ0mhI)

* 主动触发

  见下文



## GC设置



* 环境变量`GOGC`

  设置触发GC的比例，默认为100。特殊值`off`表示禁用。

* [`debug.SetGCPercent()`](https://golang.org/pkg/runtime/debug/#SetGCPercent)

  同样设置该GC比例。-1表示禁用。
  
  ```
  SetGCPercent sets the garbage collection target percentage: a collection is triggered when the ratio of freshly allocated data to live data remaining after the previous collection reaches this percentage. SetGCPercent returns the previous setting. The initial setting is the value of the GOGC environment variable at startup, or 100 if the variable is not set. A negative percentage disables garbage collection. 
  ```
  
  亲测-1有效。
  
  但不能避免`GC forced`，可能内部还有些其他机制绕过这个



## 主动触发GC



[`runtime.GC()`](https://golang.org/pkg/runtime/#GC)



## 主动归还内存

`runtime.FreeOSMemory()`



This is because Go currently does not give memory of GC-ed objects back to the operating system, to be precise, only for objects smaller then predefined limit (32KB). Instead memory is cached to speed up future allocations [Go:malloc](https://code.google.com/p/go/codesearch#go/src/pkg/runtime/malloc.h&l=48-62). Also, it seems that this is going to be fixed in the future [TODO](https://groups.google.com/d/msg/golang-nuts/WBlJjT-zu7E/hJJRSBGfKsIJ).

Edit: New GC behavior: If the memory is not used for a while (about 5 min), runtime will advise the kernel to remove the physical mappings from the unused virtual ranges. This process can be forced by calling `runtime.FreeOSMemory()`

Ref: https://stackoverflow.com/questions/16516189/why-total-mb-in-golang-heap-profile-is-less-than-res-in-top













