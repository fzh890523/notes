
### 赋值、传参 操作的语义： move/copy

> 这里指的是直接对值的操作，没有进行引用

* 对于copy类型
  * 相关的资源会做copy再。。。
    > 但怎么“看出”是什么层面的copy？ 浅拷贝还是深拷贝？ 等等
  * "copy" 后原来的binding的状态为 "copied"
  * 主要是基本类型
* 对于move类型
  * 相关的资源会move到新的binding，而原来的binding不能再访问
  * "move" 后原来的binding的状态为 "moved"
  * 非基本类型

以上的语义操作，是基于（类型）实现的trait来确定的。 `move`是默认语义，除非类型实现了 `core::marker::Copy` 这个trait（才会有`copy`语义）。

### 取引用的语义： borrow

* `&`
* `&mut`

理解：
* 看起来像（其他语言的）取指针
  > 从raw pointer的语法`let raw = &x as *const i32;`来看实现估计也是指针
* borrow语义： To receive something with the promise of returning it.
  两种borrow
  * shared (`&T`)
    A piece of data can be borrowed by a single or multiple users, but data should not be altered.
    > 其实命名挺奇怪，为啥不叫readonly、immutable？ 只是因为读写锁概念里读是可多重/共享的？
  * mutable (`&mut T`)
    A piece of data can be borrowed and altered by a single user, but the data should not be accessible for any other users at that time.

  规则： 可以参考读写锁
  * 可读可写不能同时（需待另一种生存期结束
  * borrow可以对move/copy类型
  * 生存期 概念

### 生存期 lifetime

**内存管理**：
* 手动内存管理
* 自动内存管理
  * 垃圾回收（运行时）
  * ...

* 一个资源，同一个时刻只能有一个owner
  重用（所以不是move也不是copy）这个资源，只能 引用/borrow
* 处理引用时，可以手动添加生存期注解，以便于编译器判断
  函数定义中的生存期注解会（隐式）自动添加
  会在编译器检查生存期注解： 第一次和最后一次使用（某资源/数据）
* 对内存的管理操作
  * 不显式的 drop（free？）数据
  * 没有类似gc语言的对象析构时的finalize调用
  * 会在生存期结束（go out of scope）时执行deallocation call
    这个

**生存期结束**：
* 同一层级SCOPE最后一次使用；
* 出了当前层级scope（如果是scope内的binding）；


**生存期约束**
如 `x.a`这样访问，要确保x生存期内a没有被释放。

#### 生存期注解

* 主要用来标识、约束生存期

* 格式 `'`开头，一般约定： 后面用一个小写字母，如 `'a`。 使用多个时，按照字母顺序一次使用

* 使用场景
  示例见下文
  * 函数声明
    
  * 结构体、枚举声明
  * impl、trait
  * 泛型

* 省略注解

  * `fn`
    条件是参数列表满足其一：

    * 只有一个入参是引用
      则所有出参中省略的生存期注解都为此
      `..(x: i32, y: &str) -> &str` → `..<'a>(x: i32, y: &'a str) -> &'a str`
    
    * 有`&self`或`&mut self`
      则...都为此
      `impl Impl{ fn function(&self, x: &str) -> &str {} }` →
`impl<'a> Impl<'a>{ fn function(&'a self, x: &'b str) -> &'a str {} }`
    > 但好像还有一种情况就是所有引用传参的入参的生存周期注解都不同，那么也可以省略。 但此时出参的生存周期约束是？
    > `..(x: &str, y: &str)` → `..<'a, 'b>(x: &'a str, y: &'b str)`

  * `impl` - 未来支持

* `'static`
  * 保留的特殊注解
  * 表示生存期在整个程序生存期间都有效
    数据保存在数据段，访问数据不会gg
    ```rust
    static N: i32 = 5; // A constant with 'static lifetime
    let a = "Hello, world."; // a: &'static str
    fn index() -> &'static str { // No need to mention <'static> ; fn index ̶<̶'̶s̶t̶a̶t̶i̶c̶>̶ 
        "Hello, world!"
    }
    ```    

**函数声明中的生存期注解**
> 不太清除具体的意义

```rust
// No inputs, return a reference
fn function<'a>() -> &'a str {}

// Single input
fn function<'a>(x: &'a str) {}

// Single input and output, both have the same lifetime
// The output should live at least as long as input exists
fn function<'a>(x: &'a str) -> &'a str {}

// Multiple inputs, only one input and the output share same lifetime
// The output should live at least as long as y exists
fn function<'a>(x: i32, y: &'a str) -> &'a str {}

// Multiple inputs, both inputs and the output share same lifetime
// The output should live at least as long as x and y exist
fn function<'a>(x: &'a str, y: &'a str) -> &'a str {}

// Multiple inputs, inputs can have different lifetimes 🔎
// The output should live at least as long as x exists
fn function<'a, 'b>(x: &'a str, y: &'b str) -> &'a str {}
```

**结构体、枚举中的生存期注解**
```rust
// Single element
// Data of x should live at least as long as Struct exists
struct Struct<'a> {
    x: &'a str
}

// Multiple elements
// Data of x and y should live at least as long as Struct exists
struct Struct<'a> {
    x: &'a str,
    y: &'a str
}


// Variant with a single element
// Data of the variant should live at least as long as Enum exists
enum Enum<'a> {
    Variant(&'a Type)
}
```

**impl中的生存期注解**
```rust
struct Struct<'a> {
    x: &'a str
}
    impl<'a> Struct<'a> {
        fn function<'a>(&self) -> &'a str {
            self.x
        }
    }


struct Struct<'a> {
    x: &'a str,
    y: &'a str
}
    impl<'a> Struct<'a> {
        fn new(x: &'a str, y: &'a str) -> Struct<'a> { // No need to specify <'a> after new; impl already has it
          Struct {
              x : x,
              y : y
          }
        }
    }


// 🔎
impl<'a> Trait<'a> for Type
impl<'a> Trait for Type<'a>
```

```rust
// 🔎
fn function<F>(f: F) where for<'a> F: FnOnce(&'a Type)
struct Struct<F> where for<'a> F: FnOnce(&'a Type) { x: F }
enum Enum<F> where for<'a> F: FnOnce(&'a Type) { Variant(F) }
impl<F> Struct<F> where for<'a> F: FnOnce(&'a Type) { fn x(&self) -> &F { &self.x } }
```