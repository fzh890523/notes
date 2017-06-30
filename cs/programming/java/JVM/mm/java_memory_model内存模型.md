# 背景



## 可见性 & 原子性 & 重排序



这三个都是比较常见的概念，也容易弄混。



* 可见性

  * 重点在： **共享资源**

  * 为什么不可见？

    * CPU cache 和 内存（地址）的一致性 - common case

      > 如：
      >
      > ```c
      > a += 1
      > ```
      >
      > 类似：
      >
      > ```assembly
      > add xx 1
      > ```
      >
      > 这里看似是“可见”的，但由于CPU cacheline的存在，在 cache invalidate 之前可能是没实际写入内存（xx地址）的（参见 [cs_system_cpu_digest](../../../../system/cpu/cs_system_cpu_digest.md)）。

    * 线程工作内存-堆内存一致性 - JMM

      > C（等）之所以没这个问题是因为C里写栈和写堆是明确的，编译器（一般）不会在这方面做优化（trick）
      >

      ​

      > 而java：
      >
      > 1. java栈
      >
      >    - 1 - 2
      >    - 1 - 3
      >
      > 2. 局部变量表
      >
      >    thread local，没有可见性和原子性等问题
      >
      > 3. 堆
      >
      >    non thread local，有可见性和原子性等问题
      >
      > 对堆内存数据（要么是全局如 Integer.class 要是 类属性如 ClassA.sf1 要么是 实例属性 objOfClassA.a），类似： 
      >
      > ```java
      > instance.a++;
      > ```
      >
      > ​
      >
      > 这样的操作，实际上类似：
      >
      > ```assembly
      > 1
      > ```
      >
      > ​
      >
      > 如：
      >
      > ```java
      > int b = a;
      > T1 t = new T1();
      > t.a++;
      > b++;
      > a = b;
      > a++;
      > sa = b;
      > sa = a;
      > ```
      >
      > ```assembly
      >   L0
      >     LINENUMBER 15 L0
      >     ALOAD 0
      >     GETFIELD T.a : I
      >     ISTORE 1
      >    L1
      >     LINENUMBER 16 L1
      >     NEW T$T1
      >     DUP
      >     INVOKESPECIAL T$T1.<init> ()V
      >     ASTORE 2
      >    L2
      >     LINENUMBER 17 L2
      >     ALOAD 2
      >     DUP
      >     GETFIELD T$T1.a : I
      >     ICONST_1
      >     IADD
      >     PUTFIELD T$T1.a : I
      >    L3
      >     LINENUMBER 18 L3
      >     IINC 1 1
      >    L4
      >     LINENUMBER 19 L4
      >     ALOAD 0
      >     ILOAD 1
      >     PUTFIELD T.a : I
      >    L5
      >     LINENUMBER 20 L5
      >     ALOAD 0
      >     DUP
      >     GETFIELD T.a : I
      >     ICONST_1
      >     IADD
      >     PUTFIELD T.a : I
      >    L6
      >     LINENUMBER 21 L6
      >     ILOAD 1
      >     PUTSTATIC T.sa : I
      >    L7
      >     LINENUMBER 22 L7
      >     ALOAD 0
      >     GETFIELD T.a : I
      >     PUTSTATIC T.sa : I
      >    L8
      >     LINENUMBER 23 L8
      >     RETURN
      >    L9
      >     LOCALVARIABLE this LT; L0 L9 0
      >     LOCALVARIABLE b I L1 L9 1
      >     LOCALVARIABLE t LT$T1; L2 L9 2
      >     MAXSTACK = 3
      >     MAXLOCALS = 3
      > ```
      >
      > <del>所以影响到可见性</del>
      >
      > 这里影响到可见性的实际是读操作，如：
      >
      > ```assembly
      > // 伪码
      > load xx
      > ```
      >
      > ​

* 原子性

  * 重点在： **操作**

    > 还是以上面的add操作为例。
    >
    > CPU无法直接对内存地址做add操作，实际上类似：
    >
    > ```assembly
    > # 伪码
    > mov xx,eax
    > add 1,eax
    > mov eax,xx
    > ```
    >
    > 非单步操作，(在没有其他措施的情况下)原子性肯定无法保证

* 重排序

  * 重点在： **原理** 和 **影响**



已有特性和这几点的关系：

* volatile
* synchronize
* lock




# volatile

[What does volatile do?]()

Volatile fields are special fields which are **used for communicating state between threads**. **Each read of a volatile will see the last write to that volatile by any thread; in effect**, they are designated by the programmer **as fields for which it is never acceptable to see a "stale" value as a result of caching or reordering**. The **compiler and runtime are prohibited from allocating them in registers**. They must also **ensure that after they are written, they are flushed out of the cache to main memory**, so they can immediately become visible to other threads. Similarly, **before a volatile field is read, the cache must be invalidated so that the value in main memory**, not the local processor cache, is the one seen. There are also **additional restrictions on reordering accesses to volatile variables**.

Under the old memory model, accesses to volatile variables could not be reordered with each other, but they could be reordered with nonvolatile variable accesses. This undermined the usefulness of volatile fields as a means of signaling conditions from one thread to another.

Under the new memory model, it is still true that volatile variables cannot be reordered with each other. The difference is that it is now no longer so easy to reorder normal field accesses around them. **Writing to a volatile field has the same memory effect as a monitor release, and reading from a volatile field has the same memory effect as a monitor acquire**. In effect, because the new memory model places stricter constraints on reordering of volatile field accesses with other field accesses, volatile or not, anything that was visible to thread A when it writes to volatile field `f` becomes visible to thread B when it reads `f`.

Here is a simple example of how volatile fields can be used:

```java
class VolatileExample {
  int x = 0;
  volatile boolean v = false;
  
  public void writer() {
    x = 42;
    v = true;
  }
  
  public void reader() {
    if (v == true) {
      //uses x - guaranteed to see 42.
    }
  }
}
```



Assume that one thread is calling `writer`, and another is calling `reader`. The write to `v` in `writer` releases the write to `x` to memory, and the read of `v` acquires that value from memory. Thus, if the reader sees the value `true` for v, it is also guaranteed to see the write to 42 that happened before it. This would not have been true under the old memory model.  If `v` were not volatile, then the compiler could reorder the writes in `writer`, and `reader`'s read of `x` might see 0.

**Effectively, the semantics of volatile have been strengthened substantially, almost to the level of synchronization. Each read or write of a volatile field acts like "half" a synchronization, for purposes of visibility**.

**Important Note:** Note that it is important for both threads to access the same volatile variable in order to properly set up the happens-before relationship. It is not the case that everything visible to thread A when it writes volatile field `f` becomes visible to thread B after it reads volatile field `g`. The release and acquire have to "match" (i.e., be performed on the same volatile field) to have the right semantics.



## 定位/目的

> 在线程间通信/传递状态



## 效果

线程需要“立即看见”其他线程对volatile field的写操作。

这里的**立即看见**有**可见性**和**禁止重排**的双重效果。

语义很强，几乎是**half-synchronize**



## 细节

可见性

* 写后： flush
* 读前： invalidate



禁止重排

* 老MM： volatile fields之间禁止重排

  示例：

  ```java
  class Test {
    volatile int a;
    volatile int b;
    int c;
    
    public void test() {
      c = 1;  // 1
      a = 2;  // 2
      b = 3;  // 3

      // old MM下只保证 2和3的相对顺序，也即最后可能是： 1,2,3; 2,1,3; 2,3,1
    }
    
    public void test1() {
      if(b > 0) {
        assert(a > 0);  // ok
        assert(c > 0);  // nok
      }
    }
  }
  ```

  ​

* 新MM： volatile field和其他field之间都禁止重排

  示例：

  ```java
  class Test {
    volatile int a;
    volatile int b;
    int c;
    int d;
    
    public void test() {
      c = 1;  // 1
      d = 2;  // 2    
      a = 3;  // 3
      b = 4;  // 4

      // new MM下保证a,b 与其他field(c)的顺序，所以不考虑d时只能是 2,3,4 ； 考虑d时只能是 1,2,3,4 或 2,1,3,4
    }
    
    public void test1() {
      if(b > 0) {
        assert(a > 0);  // ok
        assert(c > 0);  // ok
      }
    }
  }

  ```

  ​



# 其他



## Double-Checked Locking 

ref： [The "Double-Checked Locking is Broken" Declaration](http://www.cs.umd.edu/~pugh/java/memoryModel/DoubleCheckedLocking.html)



### Case1: 无任何同步使用单例 - nok

```java
// Single threaded version
class Foo { 
  private Helper helper = null;
  public Helper getHelper() {
    if (helper == null) 
        helper = new Helper();
    return helper;
    }
  // other functions and members...
  }
```

显然... 会多创建一些...



### Case2: 单例获取方法级别同步 - ok but low-performance

```java
// Correct multithreaded version
class Foo { 
  private Helper helper = null;
  public synchronized Helper getHelper() {
    if (helper == null) 
        helper = new Helper();
    return helper;
    }
  // other functions and members...
  }
```

显然...



### Case3: double-check + synchronized 下的单例 - nok

```java
// Broken multithreaded version
// "Double-Checked Locking" idiom
class Foo { 
  private Helper helper = null;
  public Helper getHelper() {
    if (helper == null) 
      synchronized(this) {
        if (helper == null) 
          helper = new Helper();
      }    
    return helper;
    }
  // other functions and members...
  }
```



nok原因：

1. （其他线程）可能拿到未初始化完成的helper

   1. 编译器内联构造器，这时初始化helper对象和将对象引用赋值给helper遍历可能是乱序的

   2. SMP下可能被CPU或内存系统乱序

      ？？ = =

2. ​



### Case4: double-check + synchronized + 局部变量完成初始化后赋值 - nok

```java
// (Still) Broken multithreaded version
// "Double-Checked Locking" idiom
class Foo { 
  private Helper helper = null;
  public Helper getHelper() {
    if (helper == null) {
      Helper h;
      synchronized(this) {
        h = helper;
        if (h == null) 
            synchronized (this) {
              h = new Helper();
            } // release inner synchronization lock
        helper = h;
        } 
      }    
    return helper;
    }
  // other functions and members...
  }
```

这是3的升级版本，但还是有问题。

原因是**对monitorexit语义的误解**：

> monitorexit只保证之前的语句在exit之前完成
>
> **而不保证之后的语句在exit之后执行**
>
> 所以这里是**单向内存屏障**。 而如果是双向/全内存屏障的话，会有性能损失
>
> > 文里还提到，即使是双向内存屏障也不一定能保证…，因为有些CPU，即使某核使用内存屏障还强制写回后，其他核也需要执行缓存一致指令才能… （也即内存屏障不是全局的 = =）

所以`helper = h;` 可能被重排到inner synchronized block里，然后问题又回到了…（还可以继续重排吗？）





推断（不一定对）：

> monitorenter只保证之后的代码在enter之后执行，**而不保证之前的代码在enter之前执行**

继续推断：

> exit应该有flush cache语义； 
>
> enter应该有invalidate cache语义





### Case5: 静态field赋值方式实现单例 - ok

```java
class HelperSingleton {
  static Helper singleton = new Helper();
  }
```



### Case6: double-check + synchronized + 单例为32-bit基本类型 - ok

```java
// Correct Double-Checked Locking for 32-bit primitives
class Foo { 
  private int cachedHashCode = 0;
  public int hashCode() {
    int h = cachedHashCode;
    if (h == 0) 
    synchronized(this) {
      if (cachedHashCode != 0) return cachedHashCode;
      h = computeHashCode();
      cachedHashCode = h;
      }
    return h;
    }
  // other functions and members...
  }
```



基本类型不需要初始化...

但64bit不行，因为64bit（普通）操作不保证原子性，所以可能读到**半内容**



### Case7: ThreadLocal + double-check + synchronized - ok （？）

```java
  class Foo {
	 /** If perThreadInstance.get() returns a non-null value, this thread
		has done synchronization needed to see initialization
		of helper */
         private final ThreadLocal perThreadInstance = new ThreadLocal();
         private Helper helper = null;
         public Helper getHelper() {
             if (perThreadInstance.get() == null) createHelper();
             return helper;
         }
         private final void createHelper() {
             synchronized(this) {
                 if (helper == null)
                     helper = new Helper();
             }
	     // Any non-null value would do as the argument here
             perThreadInstance.set(perThreadInstance);
         }
	}
```

这里为什么没有`拿到一个没初始化完毕的对象`的风险？



### Case8: 新MM下volatile+doube-check+synchronized

```java
// Works with acquire/release semantics for volatile
// Broken under current semantics for volatile
  class Foo {
        private volatile Helper helper = null;
        public Helper getHelper() {
            if (helper == null) {
                synchronized(this) {
                    if (helper == null)
                        helper = new Helper();
                }
            }
            return helper;
        }
    }
```

volatile新语义： 禁止重排，保证volatile field访问语句和其他field访问语句的顺序

这里为什么没有`拿到一个没初始化完毕的对象`的风险？

















