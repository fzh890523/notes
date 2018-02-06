# question



我实际想看看Integer缓存对程序的提升效果。却发现这种情况。测试代码如下：

```java
public static void main(String[] args) {
    long sum = 0L ;
    Integer.valueOf(10000);
    int range = 8000;
  
    for(int i = 0 ;i < 10;i++){
        long beginTime = new Date().getTime();
        for(int j  = 0 ;j < 10000; j++){
             for(int k = 0 ;k < 8000; k ++){
                Integer integer  = k ;
             }
         }
        long endTime = new Date().getTime();
        sum = sum + (endTime - beginTime);
    }
    System.out.println(sum);
}
```




代码执行时间为2714毫秒，当我把第三层的for循环中8000改成range.其他不变，代码如下：

```java
public static void main(String[] args) {
        long sum = 0L ;
        Integer.valueOf(10000);
        int range = 8000;
  
        for(int i = 0 ;i < 10;i++){
            long beginTime = new Date().getTime();
            for(int j  = 0 ;j < 10000; j++){
                for(int k = 0 ;k < range; k ++){
                    Integer integer  = k ;
                }
            }
            long endTime = new Date().getTime();
            sum = sum + (endTime - beginTime);
        }
        System.out.println(sum);
    }
```




两次程序**JVM参数为：-XX:AutoBoxCacheMax=8000,设置Integer的缓存上限为8000。**此时代码的执行时间为15ms. 两段代码几乎相同的代码为什么执行的时间差距如此的大 ？？另外，下面两段代码也会出现上面的情况：

```java
// 第三层for循环只相差1，一个8000，一个7999

public static void main(String[] args) {
	/*程序执行时间为2754ms*/
    long sum = 0L ;
    int range = 8000;
    Integer.valueOf(10000);

    for(int i = 0 ;i < 10;i++){
        long beginTime = new Date().getTime();
        for(int j  = 0 ;j < 10000; j++){
            for(int k = 0 ;k < 8000; k ++){
                Integer integer  = k ;
            }
        }
        long endTime = new Date().getTime();
        sum = sum + (endTime - beginTime);
    }
    System.out.println(sum);
}

public static void main(String[] args) {
	/*程序执行时间为15ms*/
    long sum = 0L ;
    int range = 8000;
    Integer.valueOf(10000);
  
    for(int i = 0 ;i < 10;i++){
        long beginTime = new Date().getTime();
        for(int j  = 0 ;j < 10000; j++){
            for(int k = 0 ;k < 7999; k ++){
                Integer integer  = k ;
            }
        }

        long endTime = new Date().getTime();
        sum = sum + (endTime - beginTime);
    }

    System.out.println(sum);
}

```




`



# answer：[陈亮](https://www.zhihu.com/people/chen-liang-82-14)



非常感谢 

[@杨博]()

 new Integer是否被内联可能是影响性能的关键因素 

```
-XX:AutoBoxCacheMax=8000 
-XX:+UnlockDiagnosticVMOptions    //后面2个参数的需要
-XX:+PrintCompilation   //打印JIT编译详情
-XX:+PrintInlining    //打印内联详情

```

当程序的循环长度小于AutoBoxCacheMax的的参数值时，即时编译结果如下：

```
com.emc.finaly.FinallyTest::main @ 36 (93 bytes)
@ 46   java.lang.Integer::valueOf (32 bytes)   inline (hot)
@ 68   java.util.Date::<init> (8 bytes)   call site not reached
@ 71   java.util.Date::getTime (5 bytes)   inline (hot)
@ 1   java.util.Date::getTimeImpl (27 bytes)   executed
 < MinInliningThreshold times
@ 14   java.util.Date::<init> (8 bytes)   call site not reached
@ 17   java.util.Date::getTime (5 bytes)   inline (hot)
@ 1   java.util.Date::getTimeImpl (27 bytes)   executed 
< MinInliningThreshold times

```

当程序的循环长度大于AutoBoxCacheMax的参数值时，即时编译结果如下：

```
@ 46   java.lang.Integer::valueOf (32 bytes)   inline (hot)
@ 28   java.lang.Integer::<init> (10 bytes)   inline (hot)
@ 1   java.lang.Number::<init> (5 bytes)   inline (hot)
@ 1   java.lang.Object::<init> (1 bytes)   inline (hot)
@ 68   java.util.Date::<init> (8 bytes)   inline (hot)
@ 1   java.lang.System::currentTimeMillis (0 bytes)   (intrinsic)
@ 4   java.util.Date::<init> (10 bytes)   inline (hot)
@ 1   java.lang.Object::<init> (1 bytes)   inline (hot)
@ 71   java.util.Date::getTime (5 bytes)   inline (hot)
@ 1   java.util.Date::getTimeImpl (27 bytes)  
 executed < MinInliningThreshold times
@ 14   java.util.Date::<init> (8 bytes)   inline (hot)
@ 1   java.lang.System::currentTimeMillis (0 bytes)   (intrinsic)
@ 4   java.util.Date::<init> (10 bytes)   inline (hot)
@ 1   java.lang.Object::<init> (1 bytes)   inline (hot)
@ 17   java.util.Date::getTime (5 bytes)   inline (hot)
@ 1   java.util.Date::getTimeImpl (27 bytes)   
executed < MinInliningThreshold times

```

两次编译的结果最大的差别在于对Integer以及父类构造函数的内联。对于是否使用range与否，其实本质上没有多大关系，根本在于JIT。问题到了这里似乎已经可以完结了，现在我就一个问题，内联为什么花了1秒钟这么长时间。明天做个测试，回头再更新。

============================================================

怕有些童鞋不明白我的问题是什么，前排解释一下概念和所做的工作吧。因为这些内容写在题目里面太长了。。所以就单独提出来写成答案了，下班的时候完善一下我之前做的测试和测试现象。

**拆箱与装箱：**jvm把一个包装类型自动转成基本数据类型叫做拆箱，反之叫做装箱。例如：

```
Integer integer = 2 ;//装箱 实际运行代码为Integer integer = Integer.valueOf(2);
int i = integer ;//装箱 实际运行代码为int i = integer.intValue();

```

**Integer缓存：**程序在执行装箱操作时，实际执行的代码为Integer.valueOf(number);我们查看这个方法的源码，发现number在一定范围内不创建新的对象，而是直接拿缓存。

```
/**
     * Returns an {@code Integer} instance representing the specified
     * {@code int} value.  If a new {@code Integer} instance is not
     * required, this method should generally be used in preference to
     * the constructor {@link #Integer(int)}, as this method is likely
     * to yield significantly better space and time performance by
     * caching frequently requested values.
     *
     * This method will always cache values in the range -128 to 127,
     * inclusive, and may cache other values outside of this range.
     *
     * @param  i an {@code int} value.
     * @return an {@code Integer} instance representing {@code i}.
     * @since  1.5
     */
    public static Integer valueOf(int i) {
        if (i >= IntegerCache.low && i <= IntegerCache.high)
            return IntegerCache.cache[i + (-IntegerCache.low)];
        return new Integer(i);
    }

```

low为固定值-128，high可以配置，默认值为127。最高位可以用 **AutoBoxCacheMax=XXX**来配置。

在加载IntegerCache类时，完成对IntegerCache.high的初始化。

```
  static {
            // high value may be configured by property
            int h = 127;
            String integerCacheHighPropValue =
                sun.misc.VM.getSavedProperty("java.lang.Integer.IntegerCache.high");
            if (integerCacheHighPropValue != null) {
                try {
                    int i = parseInt(integerCacheHighPropValue);
                    i = Math.max(i, 127);
                    // Maximum array size is Integer.MAX_VALUE
                    h = Math.min(i, Integer.MAX_VALUE - (-low) -1);
                } catch( NumberFormatException nfe) {
                    // If the property cannot be parsed into an int, ignore it.
                }
            }
            high = h;

            cache = new Integer[(high - low) + 1];
            int j = low;
            for(int k = 0; k < cache.length; k++)
                cache[k] = new Integer(j++);

            // range [-128, 127] must be interned (JLS7 5.1.7)
            assert IntegerCache.high >= 127;
        }

```

事实上，不仅仅Integer有缓存机制，其他包装类型也有缓存操作也有缓存，只不过只有Integer可以配置上限。

最初我的问题是想看看因为缓存带来性能的提升，所以设置了 **AutoBoxCacheMax = 8000**选项，使用多层for循环装箱操作。下面看看我做的几个这方面的测试吧。

1.设置 AutoBoxCacheMax = 8000，当装箱数据小于8000，例如7999

无论是用range还是写死，程序都能在30毫秒内结束，

2.AutoBoxCacheMax不做设置，使用默认缓存大小，程序消耗时间在1700毫秒。

以上两组说明了缓存对程序效率的提升是巨大的。

3.设置 AutoBoxCacheMax = 8000，装箱数范围为【0，8000】在程序中写死8000。

程序执行时间为问题中的1700.

What The Fuck ??????仅仅增加了1，程序执行时间增加了几百倍。7999之前都能在16毫秒内完成，看样8000是质变。会不会在装箱8000的时候非常耗时呢？？于是我把第三层循环去掉

```
for(int k = 0 ;k<10;k++){
            long time = new Date().getTime();
            for(int j=0;j<10000;j++){
                //for(int i = 0;i<8000 - 1  ;i++){
                    Integer integer = 8000 ;
               // }
            }
            sum = sum + new Date().getTime() - time;
        }

```

无论8000换成8001、8100还是其他数，程序多次执行时间为16毫秒左右。对8000的装箱看样没有特殊性。我还是不死心，把8000分成两段执行，一段【0，7998】一段【7998，8001】

程序如下：

```
for(int k = 0 ;k<10;k++){
            long time = new Date().getTime();
            for(int j=0;j<10000;j++){
                for(int i = 0;i<8000 - 2  ;i++){
                    Integer integer = i ;
                }
                for(int i = 7998;i<8000 + 1  ;i++){
                    Integer integer = i ;
                }
            }
            sum = sum + new Date().getTime() - time;
        }

```

相同的执行长度，程序执行时间为38毫秒。

**初段结论：**

for循环、AutoBoxCacheMax存在某种联系。但是能力有限，不知道联系是什么。





# answer： R大



哈，发现9个月前就写了草稿。没发出来肯定是因为当时我正要继续写的时候Chrome又崩溃了。

（不怪Chrome，macOS上自带的中文输入法真是见啥杀啥）

简单说，原因是OSR编译时C2没能做充分的常量传播。仅此而已。什么cache miss啊啥的说法都完全不靠谱。

给题主的简要建议是：

1. 在HotSpot VM上跑microbenchmark切记不要在main()里跑循环计时就完事。这是典型错误。重要的事情重复三遍：**请用JMH，请用JMH，请用JMH**。除非非常了解HotSpot的实现细节，在main里这样跑循环计时得到的结果其实对一般程序员来说根本没有任何意义，因为无法解释。
2. 就算计时，用System.nanoTime()也比new Date().getTime()好太多
3. 要给常量赋个别名的话，为了保证靠谱请习惯性加上final。例如说这里题主要是写 final int range = 8000; 就会发现第三层循环的条件改用range也一样快。
4. 跑microbenchmark多留意一下 -XX:+PrintCompilation 的输出。

就酱。

首先请了解一下OSR编译。跳这个传送门：[RednaxelaFX：OSR（On-Stack Replacement）是怎样的机制？](https://www.zhihu.com/question/45910849/answer/100636125)

<- 请先跳了这个传送门再往下看，不然下面的说明会难以理解。

题主说在内层循环里用字面量8000比用同样值的局部变量range要快很多。这是因为题主在main里写循环，触发的是OSR编译——只JIT编译了方法的一部分而没有编译整个方法。题主例子里的局部变量range只被赋值了一次，但在JIT编译器看来这个赋值并不在本次OSR编译覆盖的范围内，所以JIT编译器无法知道其实这个变量实质上是个常量，只知道“喔有这么个变量，它的值会在进入OSR编译的代码时从解释器传过来”，这样编译出来的代码质量就会差很多。

我以前在Azul工作的时候遇到过有客户也问了非常类似的问题，同样是使用了错误的方法写microbenchmark同样是做了错误的解读。当时的状况是同样触发了OSR编译，在循环外定义的“实质常量”没有被常量传播到OSR编译的循环内。而循环内测的是除法的性能，那个常量值比较特别可以换成若干位移和加减来做，如果有常量传播就可以对除法做特化，没有的话则是只能做普通除法就很慢。

当时为了让客户高兴其实我有写一个patch来让HotSpot C1和C2都能在OSR编译的情况下从循环外传播更多常量进来。代码有点复杂而且解决的问题常常只是“又有人乱写microbenchmark”了，所以就没有合并到Azul Zing JVM里，也没有提交给OpenJDK。知道问题所在之后实现思路其实很简单的。

最开头的时候提到了加final修饰局部变量的事。这是Java语言规范层面的规定：一个被final修饰的局部变量，如果其初始化表达式是常量表达式，那么这个局部变量也是常量表达式，它的常量值会被Java语言编译器（例如javac或者ECJ，而不是JVM里的JIT编译器）传播并内嵌到后面的使用点上。所以如果range声明的地方加了final修饰，后面用range的地方就跟直接手写8000效果是一模一样的，从JVM的角度看输入的字节码会完全一样，性能特性当然也完全一样。

至于题主的后一个例子问循环就差1为何性能差那么多，这一部分也是跟JIT编译的触发机制有关的；另外这是在最内层循环差了1，外面还有很大的放大倍数。如果题主只想测Integer cache性能的话请用JMH测，免得要关心那么多JVM的JIT编译系统的实现细节。

如果题主坚持要用原本的代码继续做实验，试试看在差1的那个例子加上 -XX:-TieredCompilation 看性能差多少。



# experiment - yonka





```shell
 yonka  ~  Downloads  tmp  java A
1532
 yonka  ~  Downloads  tmp  java B
445
 yonka  ~  Downloads  tmp  java -XX:AutoBoxCacheMax=8000 A
592
 yonka  ~  Downloads  tmp  java -XX:AutoBoxCacheMax=8000 B
8
 yonka  ~  Downloads  tmp  java -XX:AutoBoxCacheMax=8000 -XX:-TieredCompilation B
8
 yonka  ~  Downloads  tmp  java -XX:AutoBoxCacheMax=8000 -XX:-TieredCompilation A
581
 yonka  ~  Downloads  tmp  vim A.java
 yonka  ~  Downloads  tmp  diff A.java B.java
3c3
< public class A {
---
> public class B {
11c11
<                 for(int k = 0 ;k < 8000; k ++){
---
>                 for(int k = 0 ;k < range; k ++){

 yonka  ~  Downloads  tmp  java -XX:AutoBoxCacheMax=8000 -XX:-TieredCompilation -XX:+UnlockDiagnosticVMOptions -XX:+PrintInlining A
                            @ 59   java.lang.Integer::valueOf (32 bytes)   inline (hot)
                            @ 80   java.util.Date::<init> (8 bytes)   call site not reached
                            @ 83   java.util.Date::getTime (5 bytes)   inline (hot)
                              @ 1   java.util.Date::getTimeImpl (27 bytes)   executed < MinInliningThreshold times
                            @ 27   java.util.Date::<init> (8 bytes)   call site not reached
                            @ 30   java.util.Date::getTime (5 bytes)   inline (hot)
                              @ 1   java.util.Date::getTimeImpl (27 bytes)   executed < MinInliningThreshold times
576
 yonka  ~  Downloads  tmp  java -XX:AutoBoxCacheMax=8000 -XX:-TieredCompilation -XX:+UnlockDiagnosticVMOptions -XX:+PrintInlining B
                            @ 57   java.lang.Integer::valueOf (32 bytes)   inline (hot)
                            @ 78   java.util.Date::<init> (8 bytes)   call site not reached
                            @ 81   java.util.Date::getTime (5 bytes)   inline (hot)
                              @ 1   java.util.Date::getTimeImpl (27 bytes)   executed < MinInliningThreshold times
                            @ 27   java.util.Date::<init> (8 bytes)   call site not reached
                            @ 30   java.util.Date::getTime (5 bytes)   inline (hot)
                              @ 1   java.util.Date::getTimeImpl (27 bytes)   executed < MinInliningThreshold times
6
 yonka  ~  Downloads  tmp  java -XX:AutoBoxCacheMax=8000 -XX:-TieredCompilation -XX:+UnlockDiagnosticVMOptions -XX:+PrintCompilation A
    134    1             java.lang.String::hashCode (55 bytes)
    136    2             java.lang.Integer::valueOf (32 bytes)
    136    3 %           A::main @ 49 (110 bytes)
    708    3 %           A::main @ -2 (110 bytes)   made not entrant
573
 yonka  ~  Downloads  tmp  java -XX:AutoBoxCacheMax=8000 -XX:-TieredCompilation -XX:+UnlockDiagnosticVMOptions -XX:+PrintCompilation B
    134    1             java.lang.String::hashCode (55 bytes)
    136    2             java.lang.Integer::valueOf (32 bytes)
    137    3 %           B::main @ 49 (108 bytes)
    143    3 %           B::main @ -2 (108 bytes)   made not entrant
8

# bytecode差别： 基本上只有
      51: sipush        8000
      51: iload_3      
```















