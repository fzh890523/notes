# 概述

此文主要是给**java编译器开发者**的一些指南/建议/提点： 为保证JSR-133中提到的语义，应该做些什么，比如怎么做。



## 点



### 重排序

JMM规定了volatile和monitor的语义，而该语义产生了以下的结果 - 对重排序的限制。

> 这个矩阵本身不是JMM规定的



| **Can Reorder**            | *2nd operation*          | *2nd operation*            | *2nd operation*            |
| -------------------------- | ------------------------ | -------------------------- | -------------------------- |
| *1st operation*            | Normal Load Normal Store | Volatile Load MonitorEnter | Volatile Store MonitorExit |
| Normal Load Normal Store   |                          |                            | No                         |
| Volatile Load MonitorEnter | No                       | No                         | No                         |
| Volatile store MonitorExit |                          | No                         | No                         |



* Normal Load

  对non-volatile fields的

  * getfield
  * getstatic
  * array load

* Normal Store

  对non-volatile fields的

  * putfield
  * putstatic
  * array store

* Volatile Load

  对volatile fields的

  * getfield
  * getstatic

* Volatile Store

  对volatile fields的

  * putfield
  * putstatic

* MonitorEnter

  * 对 lock object


  * 包括进入同步方法

* MonitorExit

  * 对 lock object


  * 包括离开同步方法







### 内存屏障



### 多处理器



## Recipes - 诀窍（不知道怎么翻译）

> 小窍门、guide之类的吧

