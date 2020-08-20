



## 概念厘清



### const 与 指针

ref： [知乎-`const*`与`*const`](https://zhuanlan.zhihu.com/p/33208035)



```c
double pi = 3.14;
const double *ptr = &pi;
double const *ptr1 = &pi;  // 等效于line 2
double *const ptr2 = &pi;
```



* 类型（这里的`double`）在哪不重要

* `const`（往右看）更靠近哪个则是apply to 哪个

  * `* const <var>` const修饰`<var>`，也即该 指针变量 不可变 - **const pointer**/**常量指针**

    也即： **不能修改该指针本身（为指向另一个内容/内存）**

    也即不能做如下操作：

    ```c
    double * const p = &pi;
    p = &pi1;  // 错误
    ```

  * `const * <var>` const修饰`* <var>`，也即该 指针指向内容（内存） 不可变 - **pointer to const**/**指针常量**

    也即： **不能通过该指针来修改指向的内容/内存**

    也即不能做如下操作：

    ```c
    double const * p = &pi;
    *p = pi1;  // 错误
    ```

    ​























