# Golang 1.3 sync.Atomic源码解析

Posted on [2014年10月19日](http://dev.cmcm.com/archives/199)  by  [毛, 剑](http://dev.cmcm.com/)		 | [暂无评论](http://dev.cmcm.com/archives/199#respond)		

[上一篇文章](http://dev.cmcm.com/archives/22)我们说到sync.Mutex的源码实现，核心就是使用到了CPU指令CAS，从并发性能上来说atomic的效率是要高于mutex的，毕竟mutex做了不少的其他步骤，而atomic的核心其实就是和处理器密切关系的，通过一两个指令就能完成的原子操作，我们接下来来看看atomic在golang中的一些细节。 通过目录：

> ```
> 64bit_arm.go              asm_amd64p32.s            asm_linux_arm.s           atomic_test.go            race.go
> asm_386.s                 asm_arm.s                 asm_netbsd_arm.s          doc.go
> asm_amd64.s               asm_freebsd_arm.s         atomic_linux_arm_test.go  export_linux_arm_test.go
> ```

发现golang主要还是依赖汇编来来实现的原子操作，不同的CPU架构是有对应不同的.s汇编文件的。 我们重点看下asm_amd64.s X86-64cpu架构下的实现：

```
TEXT ·CompareAndSwapUint64(SB),NOSPLIT,$0-25// http://godoc.org/sync/atomic#CompareAndSwapUint64// 函数原型是：func CompareAndSwapUint64(addr *uint64, old, new uint64) (swapped bool)// 把addr参数写入BP寄存器，为什么是+0(FP）,我猜测应该是当前调用堆栈栈底 + 偏移来实现的变量保存，而golang中的指针应该是8个字节            
    MOVQ    addr+0(FP), BP         
    // 把old参数写入到AX寄存器，当前偏移＝8，因为uint64占用8个字节                                      
    MOVQ    old+8(FP), AX  
    // 把new参数写入到CX寄存器，当前偏移＝16                                                     
    MOVQ    new+16(FP), CX   
    // 使用LOCK指令，表示多核下需要总线锁或者使用MESI协议来保证原子指令的原子性，后面我会重点介绍                                           
    LOCK   
    // CMPXCHG r/m,r 将累加器AL/AX/EAX/RAX中的值与首操作数（目的操作数）比较，如果相等，// 第2操作数（源操作数）的值装载到首操作数，zf置1。如果不等， 首操作数的值装载到AL/AX/EAX/RAX并将zf清0    // 我们看到了CMPXCHG的定义，那么很明显这里使用了AX和CX来做对比                                                              
    CMPXCHGQ    CX, 0(BP)// 最后我们用SETE指令来判断ZF位，是否等于1来判断操作时候成功，而且把值写入到返回值swapped中，当前偏移=24                                                      
    SETEQ   swapped+24(FP)                                                         
    RET 
```

整个汇编就结束了，其他atomic中的函数应该都是类似的，这里不一个个分析了。

### Bus Locking & MESI

下面我们重点来看看MESI协议以及总线锁到底是怎么回事。 我这里主要使用“Intel® 64 and IA-32 Architectures Software Developer’s Manual”文档作为参考，第8章的多核管理和第11章的内存缓存管理。

> 8.1.2 Bus Locking Intel 64 and IA-32 processors provide a LOCK# signal that is asserted automatically during certain critical memory operations to lock the system bus or equivalent link. While this output signal is asserted, requests from other processors or bus agents for control of the bus are blocked. Software can specify other occasions when the LOCK semantics are to be followed by prepending the LOCK prefix to an instruction.

8.1.2中提到总线锁在Intel 64和IA-32处理器中提供了LOCK# 信号来锁定总线，来自其他处理器的请求或者是总线代理的总线控制请求全部会阻塞，我当时就非常纳闷如果这样来搞，是不是整个系统都hang住（虽然是一瞬间，单条指令足够快），始终觉得不够优雅，然后我惊奇的发现Intel其实提供了更优的方式。

> For the P6 and more recent processor families, if the memory area being accessed is cached internally in the processor, the LOCK# signal is generally not asserted; instead, locking is only applied to the processor’s caches (see Section 8.1.4, “Effects of a LOCK Operation on Internal Processor Caches”).

P6以上的cpu家族，如果访问的内存区域已经被L1/2/3之类的CPU cache缓存过，那么LOCK# 信号不会触发断言（即不锁总线），替代的使用了cpu cache协议一致性来保证指令的原子性。 我们先看11.2章提到一些缓存相关的术语：

cache line fill（cache line填充）：当cpu认为从内存中读取的字节是可以被缓存的时候，处理器读取内存填充cache line写入到从L1/2/3；

cache hit（cache命中）：当内存地址中的数据仍然被缓存的时候，处理器可以直接从L1/2/3中获取数据而不是从内存中读取；

write hit（写命中）：当处理器尝试写内存数据的缓存时，首先先检测cache line是否存在该内存地址，如果不存在，处理器（根据已经生效的策略）会直接写缓存L1/2/3而不是内存；

修改态（Modified）：此cache line已被修改过（脏行），内容已不同于主存并且为此cache line专有；

独占态（Exclusive）：此cache line内容同于主存，但不出现于其它cache中；

共享态（Shared）：此cache line内容同于主存，但也出现于其它cache中；

无效态（Invalid）：此cache行内容无效（空行）；

因为L1/2/3cpu高速缓存的存在，我们试想时候可以利用缓存的一些特性和状态来保证原子操作的完成呢？答案是肯定的：

在多核系统中，IA-32cpu（i486开始）和Intel 64处理器有**嗅探**其他处理器访问系统内存到自己的处理器高速缓存的能力了，利用这个特性就能实现缓存一致性了，比如在奔腾和P6家族的处理器中，通过嗅探其他处理器发现有一个处理器尝试写一个内存地址（它处于共享状态）那么嗅探者会立即无效它自身的cache line而且在下次访问该内存地址的时候强制一个cache line fill操作。

当一个处理器通过嗅探发现另外一个处理器尝试访问处于modifyed状态的内存地址高速缓存（还未写回write-back到系统内存的地址），那么嗅探的处理器会发送一个HITM# 信号给其他处理器告诉该cache line处于已经修改的状态，而且即将会触发一个隐式的写回操作。

隐式的写回操作的意思是：直接传送到初始请求处理器和嗅探的内存控制器，以确保系统存储器已被更新。这里，携带有效数据的处理器会直接传送给其他处理器而不是写回系统内存，因为他把写会系统内存的责任托管给了内存控制器。

> 11.4 The following section describes the cache control protocol currently defined for the Intel 64 and IA-32 architectures. In the L1 data cache and in the L2/L3 unified caches, the MESI (modified, exclusive, shared, invalid) cache protocol maintains consistency with caches of other processors. The L1 data cache and the L2/L3 unified caches have two MESI status flags per cache line. Each line can be marked as being in one of the states defined in Table 11-4. In general, the operation of the MESI protocol is transparent to programs.

11.4节中提到了缓存管理协议MESI（修改，独占，共享，无效）来实现不同cpu之间的缓存一致性。每个cpu cache line都有对应的两个MESI状态标志位，我们看看下表：

| Cache Line 状态    | M(Modifed) | E(Exclusive) | S(Shared)        | I(Invalid) |
| ---------------- | ---------- | ------------ | ---------------- | ---------- |
| cache line 是否失效? | 是          | 是            | 是                | 否          |
| 内存副本（L1/2/3)     | 过期         | 有效           | 有效               | -          |
| 副本是否存在于其他核       | 否          | 否            | 可能               | 可能         |
| 对cache line的写操作  | 不经过系统总线    | 不经过系统总线      | 引起该核独占cache line | 直接使用系统总线   |

整个和sync.atomic相关的cpu知识和源码介绍到这里：）

Posted in [Golang](http://dev.cmcm.com/archives/category/golang).| Tagged [atomic](http://dev.cmcm.com/archives/tag/atomic), [cpu](http://dev.cmcm.com/archives/tag/cpu), [Golang](http://dev.cmcm.com/archives/tag/golang).		| 324 views

来源： <<http://dev.cmcm.com/archives/199>>