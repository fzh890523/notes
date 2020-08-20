

# profiling类型



## 按照输出



### flat profiler

只统计平均调用次数（call times ？），而不根据调用者和上下文来区分。

如： methodA: 5000

### 调用图call-graph profiler

会统计调用次数和函数频率（the call times, and frequencies of the functions），以及 根据调用者区分的调用链。

如： methodD:methodC:methodB:methodA: 1000





### 输入敏感型Input-sensitive profiler

可能是可以根据入参来做进一步区分吧

如： methodA(1: "test"): 3000 （第一个参数值为test）



## 按照数据来源



### 基于事件

* java

  * JVMTI

* python

  * sys.setprofile

  ```
  c_{call,return,exception}, python_{call,return,exception}
  ```

  ​





### 基于统计

主要手段是采样，思路是**打断-爬栈**，分为几种：

* 中断打断

  kernel profile吧？

* 信号打断

  java SA

* 应用程序主动打断（基本要依赖vm API）

  java ThreadMXBean



### 基于指令（增强）



### 基于解释器指令

java BCI



### 基于模拟器/仿真器

跑在模拟器上，自然各种call都可以统计出。









