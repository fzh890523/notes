Ref: 《Intel 64 Architeture Memory Ordering White Paper_r1.0》



# WB 内存的memory ordering



## WB内存

* WB memory is what is typically used for memory made available by C malloc library calls and by allocation primitives such as new in higher-level languages.



## items

* intel的语法是 类似  `mov target from`


* older/younger

  按照程序顺序的指令先后关系， 先 -> older，后 -> younger

* processor -> logical processor

* total order（全序？） -> Operations that write to memory have a total order if all processors agree on their order of execution.

  > TODO 额没懂，即使搜了下total order的定义，还是没懂在这里的含义
  >
  > > 从下文来看应该是： 对于各核都一致的顺序

  ```C
  偏序关系、全序关系都是公理集合论中的一种二元关系。
  偏序集合：配备了偏序关系的集合。
  全序集合：配备了全序关系的集合。

  偏序：集合内只有部分元素之间在这个关系下是可以比较的。
  比如：比如复数集中并不是所有的数都可以比较大小，那么“大小”就是复数集的一个偏序关系。

  全序：集合内任何一对元素在在这个关系下都是相互可比较的。
  比如：有限长度的序列按字典序是全序的。最常见的是单词在字典中是全序的。

  // from http://blog.csdn.net/liuchuo/article/details/51986226
  ```

* xchg -> 以lock的角色出现

  ```
  IA-32 Intel® Architecture Software Developer’s Manual Volume 3A: System Programming Guide, Part 1

  7.1.2.1 says:

  The operations on which the processor automatically follows the LOCK semantics are as follows:

  When executing an XCHG instruction that references memory.
  Similarly,

  Intel® 64 and IA-32 Architectures Software Developer’s Manual Volume 2B: Instruction Set Reference, N-Z

  XCHG:

  If a memory operand is referenced, the processor’s locking protocol is automatically implemented for the duration of the exchange operation, regardless of the presence or absence of the LOCK prefix or of the value of the IOPL.
  Note that this doesn't actually meant that the LOCK# signal is asserted whether or not the LOCK prefix is used, 7.1.4 describes how on later processors locking semantics are preserved without a LOCK# if the memory location is cached. Clever, and definitely over my head.
  ```

  ​

## overview



1. Loads are not reordered with other loads.

   也就是默认保证 LL 不乱序，等于自带LL MB

2. Stores are not reordered with other stores.

   也就是自带 SS MB

3. Stores are not reordered with older loads.

   也即自带 LS MB — LS !-> SL

   > 而 `with younder loads` 就是 SL 场景了，4中指明： 不保证...

4. Loads may be reordered with older stores to different locations but not with older stores to the same location.

   也即默认 **不** 保证 SL 不乱序 — SL ?-> LS

5. In a multiprocessor system, memory ordering obeys causality (memory ordering respects transitive visibility).

   因果律？ 传递可见性？

6. In a multiprocessor system, stores to the same location have a total order.

   > 没懂，跟2不是重复了么？
   >
   > > 强调的不是（同一个核的多个store）顺序是否（相对于program order）乱序（这点由2保证）； 而是强调（不同核对于同一location的store）顺序对于各核是一致的

7. In a multiprocessor system, locked instructions have a total order.

   > 不同核的锁指令的顺序对于各核是一致的

8. Loads and stores are not reordered with locked instructions.



### 理解！

* （同核）不乱序

  * LL
  * SS
  * LS
  * **仅限同一位置的** SL
  * [LS]🔐
  * 🔐[LS]

  引申： 同一位置的各种指令（之间）都不会乱序

* （多核）total order

  * 同一位置写
  * 锁指令



## 示例

### Loads are not reordered with other loads and stores are not reordered with other stores



```assembly
// Initially x == y == 0

// processor 0
mov [ _x], 1 // M1 
mov [_y],1 //M2

// processor 1
mov r1,[_y] // M3 
mov r2, [_x] // M4
```



> r1 == 1 and r2 == 0 is not allowed
>
> > r1 == 1 -> 执行了M3 且 在此之前执行了M2 -> M1更在此之前执行 -> 执行M4时x为1 -> r2 == 1



### Stores are not reordered with older loads



```assembly
// Initially x == y == 0

// processor 0
mov r1,[ _x] // M1 
mov [_y],1 //M2

// processor 1
mov r2,[_y] // M3 
mov [_x],1 // M4
```



> r1 == 1 and r2 == 1 is not allowed
>
> > r1 == 1 -> 执行了M1 且是 M4-M1 -> M3-M4-M1-M2 -> M3执行时y为0 -> r2 == 0



### Loads may be reordered with older stores to different locations



#### Loads may be reordered with older stores

```assembly
// Initially x == y == 0

// processor 0
mov [ _x], 1 // M1 
mov r1, [ _y] // M2

// processor 1
mov [ _y], 1 // M3 
mov r2, [_x] // M4
```



> r1 == 0 and r2 == 0 is allowed
>
> > 此时乱序为：
> >
> > ```assembly
> > // processor 0
> > mov r1, [ _y] // M2
> > mov [ _x], 1 // M1 
> >
> > // processor 1
> > mov r2, [_x] // M4
> > mov [ _y], 1 // M3 
> > ```



#### Loads are not reordered with older stores to the same location

```assembly
// Initially x == y == 0

// processor 0
mov [_x],1 //M1 
mov r1, [ _x] // M2

// processor 1
mov [ _y], 1 // M3 
mov r2, [ _y] // M4
```



> Must have r1 == 1 and r2 == 1





### Intra-processor forwarding is allowed

```assembly
// Initially x == y == 0

// processor 0
mov [_x],1 // M1 
mov r1, [ _x] // M2 
mov r2, [ _y] // M3

// processor 1
mov [ _y], 1  // M4
mov r3, [ _y] // M5
mov r4, [ _x] // M6
```



> r2 == 0 and r4 == 0 is allowed
>
> 其他核的store可能对本核（暂时）不可见



### Stores are transitively visible

传递性可见





```assembly
// Initially x == y == 0

// processor 0
mov [_x],1 //M1

// processor 1
mov r1, [ _x] // M2 
mov [_y],1 //M3

// processor 2
mov r2, [ _y] // M4 
mov r3, [ _x] // M5
```



> r1 == 1, r2 == 1, r3 == 0 is not allowed
>
> * r2 == 1, r3 == 0 -> M2-M3-M4-M5-M1
> * r1 == 1 -> M1-M2
>
> 这两者相悖： 







### Total order on stores to the same location



```assembly
// Initially x == 0

// processor 0
mov [_x],1 //M1

// processor 1
mov [_x],2 //M2

// processor 2
mov r1, [ _x] // M3 
mov r2, [ _x] // M4

// processor 3
mov r3, [ _x] // M5 
mov r4, [ _x] // M6
```



> r1 == 1, r2 == 2, r3 == 2, r4 == 1 is not allowed
>
> > * r1 == 1, r2 == 2 -> M1-M3-M2-M4
> > * r3 == 2, r4 == 1 -> M2-M5-M1-M6
> >
> > 这两者相悖： M1-M2对各核都应该是同一个确定的顺序





### Locked instructions have a total order 



```assembly
// Initially x == y == 0, r1 == r2 == 1

// process 0
xchg [ _x], r1 // M1

// process 1
xchg [ _y], r2 // M2

// process 3
mov r3, [ _x] //M3 
mov r4, [ _y] //M4

// process 4
mov r5, [ _y] //M5 
mov r6, [ _x] //M6
```



> r3 == 1, r4 == 0, r5 == 1, r6 == 0 is not allowed
>
> > - r3 == 1, r4 == 0 -> M1-M3-M4-M2
> > - r5 == 1, r6 == 0 -> M2-M5-M6-M1
> >
> > 这两者相悖： M1-M2对各核都应该是同一个确定的顺序



### Loads and stores are not reordered with locks 



#### Loads are not reordered with locks

```assembly
// Initially x == y == 0, r1 == r3 == 1

// processor 0
xchg [ _x],r1 // M1 
mov r2,[_y] //M2

// processor 1
xchg [_y],r3 // M3 
mov r4,[_x] // M4
```



> r2 == 0 and r4 == 0 is not allowed
>
> r2 == 0 -> 执行M2时y为0，所以是 M1-M2-M3-M4 -> M4执行时x为1 -> r4 == 1



#### Stores are not reordered with locks

```assembly
// Initially x == y == 0, r1 == 1

// processor 0
xchg [ _x],r1 // M1 
mov [_y],1 //M2

// processor 1
mov r2,[_y] // M3 
mov r3,[_x] // M4
```



> r2 == 1 and r3 == 0 is not allowed
>
> r2 == 1 -> 执行M3时y为1，所以是 M1-M2-M3-M4 -> M4执行时x为1 -> r3 == 1









