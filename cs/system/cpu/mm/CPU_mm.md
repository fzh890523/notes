

# memory ordering model



Ref:

* [wikipedia - memory ordering](https://en.wikipedia.org/wiki/Memory_ordering)



| Memory ordering in some architectures |       |       |         |       |           |           |           |      |             |       |       |                |
| ------------------------------------- | ----- | ----- | ------- | ----- | --------- | --------- | --------- | ---- | ----------- | ----- | ----- | -------------- |
| Type                                  | Alpha | ARMv7 | PA-RISC | POWER | SPARC RMO | SPARC PSO | SPARC TSO | x86  | x86 oostore | AMD64 | IA-64 | z/Architecture |
| Loads reordered after loads           | Y     | Y     | Y       | Y     | Y         |           |           |      | Y           |       | Y     |                |
| Loads reordered after stores          | Y     | Y     | Y       | Y     | Y         |           |           |      | Y           |       | Y     |                |
| Stores reordered after stores         | Y     | Y     | Y       | Y     | Y         | Y         |           |      | Y           |       | Y     |                |
| Stores reordered after loads          | Y     | Y     | Y       | Y     | Y         | Y         | Y         | Y    | Y           | Y     | Y     | Y              |
| Atomic reordered with loads           | Y     | Y     |         | Y     | Y         |           |           |      |             |       | Y     |                |
| Atomic reordered with stores          | Y     | Y     |         | Y     | Y         | Y         |           |      |             |       | Y     |                |
| Dependent loads reordered             | Y     |       |         |       |           |           |           |      |             |       |       |                |
| Incoherent instruction cache pipeline | Y     | Y     |         | Y     | Y         | Y         | Y         | Y    | Y           |       | Y     |                |





## Speculative Processor Ordering



预测顺序？

ref:

* [wikipedia - speculative execution](https://en.wikipedia.org/wiki/Speculative_execution)
* [wilipedia - out of order execution](https://en.wikipedia.org/wiki/Out-of-order_execution)



## Compile-time memory ordering



编译器可以在不违背语义的前提下自由的重排（生成的）代码，如果不期望该行为，需要用编译器支持的方式指示“编译器”…。



### 编译期的mem-bar

* 只在编译期生效 - 指示编译器
* 不影响CPU重排



如：

- The GNU inline assembler statement

  ```
  asm volatile("" ::: "memory");
  ```

  or even

  ```
  __asm__ __volatile__ ("" ::: "memory");

  ```

  forbids [GCC](https://en.wikipedia.org/wiki/GNU_Compiler_Collection) compiler to reorder read and write commands around it.[[1\]](https://en.wikipedia.org/wiki/Memory_ordering#cite_note-1)

- The C11/C++11 command

  ```
  atomic_signal_fence(memory_order_acq_rel);

  ```

  forbids the compiler to reorder read and write commands around it.[[2\]](https://en.wikipedia.org/wiki/Memory_ordering#cite_note-2)

- [Intel ECC compiler](https://en.wikipedia.org/wiki/Intel_C%2B%2B_Compiler) uses "full compiler fence"

  ```
  __memory_barrier()

  ```

  intrinsics.[[3\]](https://en.wikipedia.org/wiki/Memory_ordering#cite_note-3)[[4\]](https://en.wikipedia.org/wiki/Memory_ordering#cite_note-4)

- [Microsoft Visual C++](https://en.wikipedia.org/wiki/Microsoft_Visual_C%2B%2B) Compiler:[[5\]](https://en.wikipedia.org/wiki/Memory_ordering#cite_note-5)

  ```
  _ReadWriteBarrier()
  ```





## Runtime memory ordering



### SMP系统中的内存一致性模型

> MCM: memory-consistency model
>
> SMP: symmetric multiprocessing



- Sequential consistency (all reads and all writes are in-order)
- Relaxed consistency (some types of reordering are allowed)
  - Loads can be reordered after loads (for better working of cache coherency, better scaling) - LL
  - Loads can be reordered after stores - SL
  - Stores can be reordered after stores - SS
  - Stores can be reordered after loads - LS（X86/AMD64）
- Weak consistency (reads and writes are arbitrarily reordered, limited only by explicit [memory barriers](https://en.wikipedia.org/wiki/Memory_barrier)) （IA-64）




不同架构提供的**原生**语义/保证，如果不能满足上层需求的话，可能需要辅助以其他技术手段。 当然，如果底层语义过强的话可能代价太大/性能不好，需要适当选用。



### 硬件mem-bar

* 运行时生效
* 需要显示使用



如：

- [x86](https://en.wikipedia.org/wiki/X86), [x86-64](https://en.wikipedia.org/wiki/X86-64)

  ```
  lfence (asm), void _mm_lfence(void)
  sfence (asm), void _mm_sfence(void)[10]
  mfence (asm), void _mm_mfence(void)[11]

  ```

- [PowerPC](https://en.wikipedia.org/wiki/PowerPC)

  ```
  sync (asm)

  ```

- [MIPS](https://en.wikipedia.org/wiki/MIPS_architecture)

  ```
  sync (asm)

  ```

- [Itanium](https://en.wikipedia.org/wiki/Itanium)

  ```
  mf (asm)

  ```

- [POWER](https://en.wikipedia.org/wiki/IBM_POWER_Instruction_Set_Architecture)

  ```
  dcs (asm)

  ```

- [ARMv7](https://en.wikipedia.org/wiki/ARMv7)[[12\]](https://en.wikipedia.org/wiki/Memory_ordering#cite_note-arm_memory_barrier-12)

  ```
  dmb (asm)
  dsb (asm)
  isb (asm)
  ```




#### 编译器支持-硬件mem-bar

考虑到（一定的）跨平台性，除了内联汇编以外，主流编译器也提供了一些mem-bar语义的语句，可以”帮忙“翻译为底层平台相关指令。

如：

- [GCC](https://en.wikipedia.org/wiki/GNU_Compiler_Collection),[[13\]](https://en.wikipedia.org/wiki/Memory_ordering#cite_note-13) version 4.4.0 and later,[[14\]](https://en.wikipedia.org/wiki/Memory_ordering#cite_note-14) has `__sync_synchronize`.
- Since C11 and C++11 an `atomic_thread_fence()` command was added.
- The [Microsoft Visual C++](https://en.wikipedia.org/wiki/Microsoft_Visual_C%2B%2B) compiler[[15\]](https://en.wikipedia.org/wiki/Memory_ordering#cite_note-15) has `MemoryBarrier()`.
- [Sun Studio Compiler Suite](https://en.wikipedia.org/wiki/Sun_Studio_Compiler_Suite)[[16\]](https://en.wikipedia.org/wiki/Memory_ordering#cite_note-16) has `__machine_r_barrier`, `__machine_w_barrier` and `__machine_rw_barrier`.




## memory ordering



Ref:

* [Who ordered memory fences on an x86?](https://bartoszmilewski.com/2008/11/05/who-ordered-memory-fences-on-an-x86/)



detailed in [Intel 64 Architecture Memory Ordering White Paper](http://www.intel.com/products/processor/manuals/318147.pdf) and the AMD spec, [AMD64 Architecture Programmer’s Manual](http://www.amd.com/us-en/assets/content_type/white_papers_and_tech_docs/24593.pdf), list a lot of memory ordering guarantees, among them:

- Loads are *not* reordered with other loads.
- Stores are *not* reordered with other stores.
- Stores are *not* reordered with older loads.
- In a multiprocessor system, memory ordering *obeys causality* (memory ordering respects transitive visibility).
- In a multiprocessor system, stores to the same location have a *total order*.
- In a multiprocessor system, locked instructions have a *total order*.
- Loads and stores are *not* reordered with locked instructions.

The x86 also has (expensive–on the order of 100 cycles) memory fence instructions, mfence, lfence, and sfence; but, considering all those guarantees, why would anyone use them? The famous [double-checked locking pattern](https://bartoszmilewski.wordpress.com/2008/08/04/multicores-and-publication-safety/), on the x86, works just fine without any fences.



# caching type/memory type



Ref:

* [IA32中的5种caching type(也叫memory type)](http://www.cnblogs.com/aoaoblogs/archive/2010/12/25/1917009.html)

  大部分内容来至于IA32手册第三卷10.3 METHODS OF CACHING AVAILABLE

  英文部分是绝对正确的，因为是原文。中文部分是一些自己的总结与理解，凑活着看吧。

  IA32现在一共有5种caching type(也叫memory type)

  Table 10-2. Memory Types and Their Properties

  | Memory Type and Mnemonic | Cacheable                    | Write Cacheable | Allows Speculative Reads | Memory Ordering Model                    |
  | ------------------------ | ---------------------------- | --------------- | ------------------------ | ---------------------------------------- |
  | Strong Uncacheable(UC)   | No                           | No              | No                       | Strong Ordering                          |
  | Uncacheable (UC-)        | No                           | No              | No                       | Strong Ordering. Can only be selected through the PAT. Can be overridden by WC in MTRRs. |
  | Write Combining (WC)     | No                           | No              | Yes                      | Weak Ordering. Available by programming MTRRs or by selecting it through the PAT. |
  | Write-through (WT)       | Yes                          | No              | Yes                      | Speculative Processor Ordering           |
  | Write Back(WB)           | Yes                          | Yes             | Yes                      | Speculative Processor Ordering           |
  | Write protected(WP)      | Yes for reads, no for writes | No              | Yes                      | Speculative Processor Ordering. Available by programming MTRRs. |

  1.   **Strong Uncacheable(UC)** : 
     对于UC的内存**读写操作都不会写到cache里，不会被reordering**.这种类型的内存适用于memory-mapped I/O device,比如说集成显卡。对于被memory-mapped I/O device使用的内存，由于会被CPU和I/O device同时访问，那么CPU的cache就会导致一致性的问题(Note1)。reordering也会导致I/O device读到dirty data，比如说I/O device把这些内存作为一些控制用的寄存器使用.
     对于普通用途的内存，UC会导致性能的急剧下降。

     Note: 一种例外是，有些I/O device支持bus coherency protocol，可以和CPU保持cache一致性，这样的话是可以使用cacheable的内存的，但是这种总线协议也是有代价的。

  2. **Uncacheable (UC-)**: 

     和UC类型一样，除了UC- memory type可以通过设置MTRRs被改写为WC memory type.

  3. **Write Combining (WC)**: 

     WC内存不会被cache, **bus coherency protcoal不会保证WC内存的读写**。对于WC类型的**写操作，可能会被延迟，数据被combined in write combining buffer**, 这样可以减少总线上的访存操作。Speculative reads are allowed(Note)。
     对于video frame buffer, 适合使用WC类型的内存。因为CPU对于frame buffer一般只有写操作，没有读，并不需要cache。对frame buffer而言，的写操作是否按顺序没有关系。
     (Note:  Speculative read是指读之前并不验证内存的有效性，先冒险的读进来，如果发现不是有效数据再取消读取操作，并更新内存后再读取. 比如说数据还是被buffer在WC buffer中)

  4. **Write-through (WT) and Write-back(WB)**

     |      |                                          |                             |                                          |                                          |
     | ---- | ---------------------------------------- | --------------------------- | ---------------------------------------- | ---------------------------------------- |
     | WT   | Writes and reads to and from system memory are cached.Reads come from cache lines on cache hitsread misses cause cache fillsSpeculative reads are allowed | Write combining is allowed. | All writes are written to a cache line (when possible) and through to system memory. When writing through to memory, invalid cache lines are never filledand valid cache lines are either filled or invalidated. Write combining is allowed.(Write misses doesn't cause cache line fills)【写到内存，更新（或者不更新（标记失效？））cache】 | 适用于bus上的设备只读取内存而不需要写(Note: Windows上似乎没有使用这种类型的内存) |
     | WB   | Same as WT                               | Write combining is allowed. | Write misses cause cache line fillsand writes are performed entirely in the cache, when possible【写到cache；满了后flush/write back】 | 最普通的只会被CPU使用的内存，由于write操作是在cache中进行的，只有必要的时候才会被写会memory，可减少了bus的上的压力 |

  5. **Write protected(WP)**: 

     读操作和WT/WB没有什么区别，读会被cache. 写不一样，**写的时候会在bus上传播这个操作，并且导致其他处理器上的cache lines被更新**。
     主要用于多处理器的情况。WP的内存，在写的时候就会更新其他处理器上的cache,而WB/WT类型的内存需要**等到其他处理读的时候才会去更新无效的cache**

* [Intel 64 and IA32 内存类型 - memory type](http://blog.csdn.net/kickxxx/article/details/42705737)

  内存类型又称为cache类型，用来定义系统内存区域的cache类型，当前Intel 64和IA32支持如下内存类型：

  **Strong Unacheable(UC) **

  内存不可以cached，所有的读和写操作出现在总线上的顺序就是程序执行的顺序。不会执行预测内存访问，或者分支预测。这种内存类型的cache机制非常适合那些做

  memory-mappped的IO设备。如果在普通内存上使用这种内存类型，将极大降低处理器性能。

  **Uncacheable(UC-)**

  和Strong Uncacheable类似，除了UC-内存类型**可以通过修改MTRRs寄存器变成WC内存类型**。这种内存类型自Pentium III处理器开始出现，并指只能通过PAT来指定。

  **Write Combining(WC)**

  这种类型的系统内存不可以cached，处理器总线的一致性协议不会保证一致性。可以预测读；写操作被 delay并且被合并到write combine buffer(WC buffer)中，以便减少内存的访问。

  如果WC buffer仅仅部分被填充，那么写操作要推迟到下一次Serializing事件，比如SFENCE或者MFENCE指令，CPUID，对uncached memory的读写操作，一个中断，或者一个Lock指令。

  这种内存类型非常适合video frame buffers，在需要把内存的内容显示到graphics display之前，写顺序并不重要。在Pentium Pro好Pentium II处理器家族中通过编程MTRRs选择这种内存类型；从Pentium III处理器开始通过编程MTRRs或者通过PAT来设置。

  **Write-through (WT)**

  对系统内存的读写操作都是cached。cache hits则从cache lines中读取，cache missed则会填充cache lines。允许预测读。**写到cache lines的同时也会写通到系统内存（有可能不会写到cache line中，而是直接写通到系统内存）**。当写通到系统内存时，无效的cache line不会被填充，而有效的cache line或者被填充或者变得无效。

  write combining是允许的。

  这种内存类型非常适合framebuffers，或者那些访问系统内存时不会snooping总线内存访问的设备。

  这种内存类型保证处理器中的cache和系统内存间的一致性。

  **Write-back (WB)**

  内存读写都会cached。cache hits则从cache lines中读取，cache missed则会导致cache fills（cache填充）。写失败则导致cache line填充，如果可能整个写都在cache中执行。

  Write combining是允许的。通过减少不必要的系统内存写操作，Write-back减少了总线传输。写到cache lines中并不会立刻推送到系统内存中，相反，这些写操作仅仅保留在系统cache中。这些修改的cache lines会推迟写到系统内存，直到执行write-back操作。

  Write-back操作的触发：在cache lines需要重新分配时，比如当cache中分配的cache lines已经满了；或者在需要保证cache一致性时。

  这种内存类型提供了最好的性能，但是访问这种系统内存的设备能够snooping内存访问，以确保系统内存和cache的一致性

  **Write Protected (WP)**

  从cache lines中读取，如果cache missed则引发cache fills。写操作则传播到系统总线，使得总线上处理器的相应cache lines变得无效。

  预测读是允许的。这种内存类型从P6处理器家族开始出现，通过MTRRs寄存器设置。















