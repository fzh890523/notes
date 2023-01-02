
## primitive data types

### bool

### char(unicode, 4 bytes)

不知道是utf-16还是。。

### int

**i8, i16, i32, i64, i128**



| DATA TYPE | MIN                                      | MAX                                     |
| :-------- | :--------------------------------------- | :-------------------------------------- |
| i8        | -128                                     | 127                                     |
| i16       | -32768                                   | 32767                                   |
| i32       | -2147483648                              | 2147483647                              |
| i64       | -9223372036854775808                     | 9223372036854775807                     |
| i128      | -170141183460469231731687303715884105728 | 170141183460469231731687303715884105727 |



**u8, u16, u32, u64, u128**

| DATA TYPE | MIN  | MAX                                     |
| :-------- | :--- | :-------------------------------------- |
| u8        | 0    | 255                                     |
| u16       | 0    | 65535                                   |
| u32       | 0    | 4294967295                              |
| u64       | 0    | 18446744073709551615                    |
| u128      | 0    | 340282366920938463463374607431768211455 |



**isize, usize**

大约是go里的int, uint（似乎没有）。 一个字长为长度的signed int/unsigned int。



### float: f32, f64



## 字面量



对数值字面量，只要把类型作为后缀加上去，就完成了类型说明。比如指定字面量 `42` 的 类型是 `i32`，只需要写 `42i32`。

无后缀的数值字面量，其类型取决于怎样使用它们。如果没有限制，编译器会对整数使用 `i32`，对浮点数使用 `f64`。



```rust
fn main() {
    // 带后缀的字面量，其类型在初始化时已经知道了。
    let x = 1u8;
    let y = 2u32;
    let z = 3f32;

    // 无后缀的字面量，其类型取决于如何使用它们。
    let i = 1;
    let f = 1.0;

    // `size_of_val` 返回一个变量所占的字节数
    println!("size of `x` in bytes: {}", std::mem::size_of_val(&x));
    println!("size of `y` in bytes: {}", std::mem::size_of_val(&y));
    println!("size of `z` in bytes: {}", std::mem::size_of_val(&z));
    println!("size of `i` in bytes: {}", std::mem::size_of_val(&i));
    println!("size of `f` in bytes: {}", std::mem::size_of_val(&f));
}

```









## 复杂类型 complex data types


