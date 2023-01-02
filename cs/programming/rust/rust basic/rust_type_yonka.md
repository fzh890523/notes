

## 类型转换



Rust 不提供原生类型之间的隐式类型转换（coercion），但可以使用 `as` 关键字进行显 式类型转换（casting）。

整型之间的转换大体遵循 C 语言的惯例，除了 C 会产生未定义行为的情形。在 Rust 中所 有整型转换都是定义良好的。



### as



```rust
// 不显示类型转换产生的溢出警告。
#![allow(overflowing_literals)]

fn main() {
    let decimal = 65.4321_f32;

    // 错误！不提供隐式转换
    let integer: u8 = decimal;
    // 改正 ^ 注释掉这一行

    // 可以显式转换
    let integer = decimal as u8;
    let character = integer as char;

    println!("Casting: {} -> {} -> {}", decimal, integer, character);

    // 当把任何类型转换为无符号类型 T 时，会不断加上或减去 (std::T::MAX + 1)
    // 直到值位于新类型 T 的范围内。

    // 1000 已经在 u16 的范围内
    println!("1000 as a u16 is: {}", 1000 as u16);

    // 1000 - 256 - 256 - 256 = 232
    // 事实上的处理方式是：从最低有效位（LSB，least significant bits）开始保留
    // 8 位，然后剩余位置，直到最高有效位（MSB，most significant bit）都被抛弃。
    // 译注：MSB 就是二进制的最高位，LSB 就是二进制的最低位，按日常书写习惯就是
    // 最左边一位和最右边一位。
    println!("1000 as a u8 is : {}", 1000 as u8);
    // -1 + 256 = 255
    println!("  -1 as a u8 is : {}", (-1i8) as u8);

    // 对正数，这就和取模一样。
    println!("1000 mod 256 is : {}", 1000 % 256);

    // 当转换到有符号类型时，（位操作的）结果就和 “先转换到对应的无符号类型，
    // 如果 MSB 是 1，则该值为负” 是一样的。

    // 当然如果数值已经在目标类型的范围内，就直接把它放进去。
    println!(" 128 as a i16 is: {}", 128 as i16);
    // 128 转成 u8 还是 128，但转到 i8 相当于给 128 取八位的二进制补码，其值是：
    println!(" 128 as a i8 is : {}", 128 as i8);

    // 重复之前的例子
    // 1000 as u8 -> 232
    println!("1000 as a u8 is : {}", 1000 as u8);
    // 232 的二进制补码是 -24
    println!(" 232 as a i8 is : {}", 232 as i8);
}

```



## 类型推断



```rust
fn main() {
    // 因为有类型说明，编译器知道 `elem` 的类型是 u8。
    let elem = 5u8;

    // 创建一个空向量（vector，即不定长的，可以增长的数组）。
    let mut vec = Vec::new();
    // 现在编译器还不知道 `vec` 的具体类型，只知道它是某种东西构成的向量（`Vec<_>`）
    
    // 在向量中插入 `elem`。
    vec.push(elem);
    // 啊哈！现在编译器知道 `vec` 是 u8 的向量了（`Vec<u8>`）。
    // 试一试 ^ 注释掉 `vec.push(elem)` 这一行。

    println!("{:?}", vec);
}

```



### From和Into

[`From`](https://rustwiki.org/zh-CN/std/convert/trait.From.html) trait 允许一种类型定义 “怎么根据另一种类型生成自己”，因此它提供了一种类型转换的简单机制。在标准库中有无数 `From` 的实现，规定原生类型及其他常见类型的转换功能。



```rust
use std::convert::From;

#[derive(Debug)]
struct Number {
    value: i32,
}

impl From<i32> for Number {
    fn from(item: i32) -> Self {
        Number { value: item }
    }
}

fn main() {
    let num = Number::from(30);
    println!("My number is {:?}", num);
}

```



[`Into`](https://rustwiki.org/zh-CN/std/convert/trait.Into.html) trait 就是把 `From` trait 倒过来而已。也就是说，如果你为你的类型实现了 `From`，那么同时你也就免费获得了 `Into`。

使用 `Into` trait 通常要求指明要转换到的类型，因为编译器大多数时候不能推断它。不过考虑到我们免费获得了 `Into`，这点代价不值一提。



```rust
use std::convert::From;

#[derive(Debug)]
struct Number {
    value: i32,
}

impl From<i32> for Number {
    fn from(item: i32) -> Self {
        Number { value: item }
    }
}

fn main() {
    let int = 5;
    // 试试删除类型说明
    let num: Number = int.into();  // funny，也是个语法糖吧
    println!("My number is {:?}", num);
}

```





#### TryFrom/TryInto

类似于 [`From` 和 `Into`](http://localhost:3000/conversion/from_into.html)，[`TryFrom`](https://rustwiki.org/zh-CN/std/convert/trait.TryFrom.html) 和 [`TryInto`](https://rustwiki.org/zh-CN/std/convert/trait.TryInto.html) 是 类型转换的通用 trait。不同于 `From`/`Into` 的是，`TryFrom` 和 `TryInto` trait 用于易出错的转换，也正因如此，其返回值是 [`Result`](https://rustwiki.org/zh-CN/std/result/enum.Result.html) 型。

> 如 str -> bool/num ？ 可能格式不合法



```rust
use std::convert::TryFrom;
use std::convert::TryInto;

#[derive(Debug, PartialEq)]
struct EvenNumber(i32);

impl TryFrom<i32> for EvenNumber {
    type Error = ();

    fn try_from(value: i32) -> Result<Self, Self::Error> {
        if value % 2 == 0 {
            Ok(EvenNumber(value))
        } else {
            Err(())
        }
    }
}

fn main() {
    // TryFrom

    assert_eq!(EvenNumber::try_from(8), Ok(EvenNumber(8)));
    assert_eq!(EvenNumber::try_from(5), Err(()));

    // TryInto

    let result: Result<EvenNumber, ()> = 8i32.try_into();
    assert_eq!(result, Ok(EvenNumber(8)));
    let result: Result<EvenNumber, ()> = 5i32.try_into();
    assert_eq!(result, Err(()));
}

```







#### ToString/FromStr

要把任何类型转换成 `String`，只需要实现那个类型的 [`ToString`](https://rustwiki.org/zh-CN/std/string/trait.ToString.html) trait。然而不要直接这么做，您应该实现[`fmt::Display`](https://rustwiki.org/zh-CN/std/fmt/trait.Display.html) trait，它会自动提供 [`ToString`](https://rustwiki.org/zh-CN/std/string/trait.ToString.html)，并且还可以用来打印类型，就像 [`print!`](http://localhost:3000/hello/print.html) 一节中讨论的那样。



```rust
use std::fmt;

struct Circle {
    radius: i32
}

impl fmt::Display for Circle {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "Circle of radius {}", self.radius)
    }
}

fn main() {
    let circle = Circle { radius: 6 };
    println!("{}", circle.to_string());  // 自动获得了...
}

```





## 类型别名



可以用 `type` 语句给已有的类型取个新的名字。类型的名字必须遵循驼峰命名法（像是 `CamelCase` 这样），否则编译器将给出警告。原生类型是例外，比如： `usize`、`f32`，等等。



> TODO y: 这个跟struct的区别？ `struct List(Vec<i32>);`



```rust
// `NanoSecond` 是 `u64` 的新名字。
type NanoSecond = u64;
type Inch = u64;

// 通过这个属性屏蔽警告。
#[allow(non_camel_case_types)]
type u64_t = u64;
// 试一试 ^ 移除上面那个属性

fn main() {
    // `NanoSecond` = `Inch` = `u64_t` = `u64`.
    let nanoseconds: NanoSecond = 5 as u64_t;
    let inches: Inch = 2 as u64_t;

    // 注意类型别名*并不能*提供额外的类型安全，因为别名*并不是*新的类型。
    println!("{} nanoseconds + {} inches = {} unit?",
             nanoseconds,
             inches,
             nanoseconds + inches);
}

```

