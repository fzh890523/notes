

```sh
go_memstats_alloc_bytes 2.7990608e+07  # 已分配的对象的总byte数。 == 下面的heap_alloc_bytes
go_memstats_alloc_bytes_total 4.27100856e+08  # 累计分配对象的总byte数，只增不减
go_memstats_buck_hash_sys_bytes 1.539931e+06  # 用于对bucket hash table做profile的内存 - 晕太特定了吧
go_memstats_frees_total 7.782501e+06  # 累计的释放的堆对象数。 mallocs - frees = live objects
go_memstats_gc_cpu_fraction 5.5364616826500596e-05  # gc活动使用的cpu的比例。 cpu的分母是程序启动后的总cpu时间（也即核数乘以总时间）
go_memstats_gc_sys_bytes 2.85696e+06  # gc元数据占用的内存 - 这个元数据是指？gc活动中自己分配的？
go_memstats_heap_alloc_bytes 2.7990608e+07  # 已分配对象的总内存，包括live/reachable objects和unreachable但还没被gc的。 简单的说就是分配时增加，回收时减少。 这里专门解释了，sweep阶段才会减少，而sweep “incrementally ”发生在gc周期之间，所以这个值的减少也是平滑的。 相对的，如果是stw期间执行的话就会变化剧烈了。
go_memstats_heap_idle_bytes 3.0646272e+07  # 没有使用的spans的总大小，甚至可能是已经还给os的。 idle - released 就是真正的unused，这部分可以用来分配对象，也可作栈内存（听这个意思，stack span也是这个span的一部分？）
go_memstats_heap_inuse_bytes 3.4889728e+07  # 使用中的spans的总大小，inuse表示其中至少有一个对象，同样的，一个span只能用于（接近）同样大小的对象。 inuse - alloc 大约就是已使用但没用完的spans部分吧，但可能因为要分配的对象尺寸不合适而无法继续充分使用
go_memstats_heap_objects 423898  # 已分配的对象数量
go_memstats_heap_released_bytes 2.7230208e+07  # 已释放的spans的大小。 idle spans的一部分。
go_memstats_heap_sys_bytes 6.5536e+07  # 这个原文表述有点混乱，估计意思是（（go）堆到达过的）最大虚拟地址空间 - 这样的话这个metric意义不大啊。 go堆的生长方向？
go_memstats_last_gc_time_seconds 1.6099263967523825e+09  # 最近一次gc的时间戳（unix epoch，转为seconds了吧）
go_memstats_lookups_total 0  # 统计runtime执行了多少次指针查找。 这里是0感觉是需要开启某选项才会做这个统计
go_memstats_mallocs_total 8.206399e+06  # 可能是对应`Mallocs`这个field。 那么表示累计分配的堆对象数量，只增。 Mallocs - Frees 就是live objects数量。 
go_memstats_mcache_inuse_bytes 13888  使用中的（原文叫allocated）mcache内存部分
go_memstats_mcache_sys_bytes 16384  # 用来构建mcache的内存（或者说以mcache形式存在的（go）堆外内存）
go_memstats_mspan_inuse_bytes 667080  # 使用中的（原文叫allocated）mspan内存部分
go_memstats_mspan_sys_bytes 802816  # 用来构建mspan的内存（或者说以mspan形式存在的（go）堆外内存）
go_memstats_next_gc_bytes 5.5278416e+07  # 触发下一次gc的堆大小。 每次gc后会根据GOGC值和当前堆大小来计算改值并设置
go_memstats_other_sys_bytes 1.927581e+06  # （go）堆外内存的杂项部分
go_memstats_stack_inuse_bytes 1.572864e+06  # 目测是goroutine栈的内存，实际在os堆上。 统计方式是统计in-use的stack span的总内存； 只要其中有栈就视为in-use； 一个stack span里的stack需要是同等大小（听起来像是一个span分割为多个同样大小的stack slot？ 不够用了就把栈换到更大的span的stack slot？）
go_memstats_stack_sys_bytes 1.572864e+06  # = StackInuse + os级别线程栈内存
go_memstats_sys_bytes 7.4252536e+07  # 从os获取的所有内存。 估计意思是（到达过的）最大虚拟地址空间，包括heap, stacks, and other internal data structures。 区别于 heap_sys 只计算heap部分。 但这几部分的内存布局并不清楚啊。。。
process_resident_memory_bytes 7.9691776e+07  # rss
process_virtual_memory_bytes 1.60333824e+08  # vsz
process_virtual_memory_max_bytes -1
```









