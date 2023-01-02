

## arrays

> 数组（array）是一组拥有相同类型 `T` 的对象的集合，在内存中是连续存储的。数组使用中括号 `[]` 来创建，且它们的大小在编译时会被确定。数组的类型标记为 `[T; length]`（译注：`T` 为元素类型，`length` 表示数组大小）。



一般意义上的数组：
* 长度不可变
  `mut`表示？
* 同一类型

如果要自动扩容的话： `vectors`

```rust
let a = [1, 2, 3]; // a[0] = 1, a[1] = 2, a[2] = 3
let mut b = [1, 2, 3];

let c: [i32; 0] = []; //[Type; NO of elements] -> [] /empty array
let d: [i32; 3] = [1, 2, 3];

let e = ["my value"; 3]; //["my value", "my value", "my value"];

println!("{:?}", a); //[1, 2, 3]
println!("{:#?}", a);
//  [
//      1,
//      2,
//      3
//  ]
```



* `&`就成了slice是不是有点武断？ 😂

  还有没有 **数组的引用** 这个东西/类型？

  > 试了下，原来是个语法糖，也即可以把 数组引用 作为 slice 传递，估计做了个装箱
  >
  > 但 **还是有数组的引用这个类型，如 `&[i32; 5]`**

* `mut`后即可修改
* index访问要用`[0]`这种，而不能用tuple的`.0`这种

* `let vv = ys[1 .. 4];` 这样似乎得到的是一个不定长数组，而因为编译器长度不可知，所以会报错

  > 那为什么不干脆不支持该语法呢？



```rust
use std::mem;

// 此函数借用一个 slice
fn analyze_slice(slice: &[i32]) {
    println!("first element of the slice: {}", slice[0]);
    println!("the slice has {} elements", slice.len());
}

// 此函数借用一个 array
fn analyze_array(slice: &[i32; 5]) {
    println!("first element of the slice: {}", slice[0]);
    println!("the slice has {} elements", slice.len());
}

fn main() {
    // 定长数组（类型标记是多余的）
    let xs: [i32; 5] = [1, 2, 3, 4, 5];

    analyze_array(&xs);  // array ref
  
    // 所有元素可以初始化成相同的值
    let ys: [i32; 500] = [0; 500];

    // 下标从 0 开始
    println!("first element of the array: {}", xs[0]);
    println!("second element of the array: {}", xs[1]);

    // `len` 返回数组的大小
    println!("array size: {}", xs.len());

    // 数组是在栈中分配的
    println!("array occupies {} bytes", mem::size_of_val(&xs));

    // 数组可以自动被借用成为 slice
    println!("borrow the whole array as a slice");
    analyze_slice(&xs);  // array ref -> slice

    // slice 可以指向数组的一部分
    println!("borrow a section of the array as a slice");
    analyze_slice(&ys[1 .. 4]);

    // 越界的下标会引发致命错误（panic）
    println!("{}", xs[5]);
}

```







## slice

> 切片（slice）类型和数组类似，但其大小在编译时是不确定的。相反，切片是一个双字对象（two-word object），第一个字是一个指向数据的指针，第二个字是切片的长度。这个 “字” 的宽度和 usize 相同，由处理器架构决定，比如在 x86-64 平台上就是 64 位。slice 可以用来借用数组的一部分。slice 的类型标记为 `&[T]`。



* 与go slice类似
  * 本身不限定长度
  * 指向其他数据结构（主要是数组？）的引用
  * 支持切片操作
* 也有差别
  * 应该不能自动扩容

```rust
let a: [i32; 4] = [1, 2, 3, 4];//Parent Array

let b: &[i32] = &a; //Slicing whole array
let c = &a[0..4]; // From 0th position to 4th(excluding)
let d = &a[..]; //Slicing whole array

let e = &a[1..3]; //[2, 3]
let f = &a[1..]; //[2, 3, 4]
let g = &a[..3]; //[1, 2, 3]
```





## tuples



> 元组是一个可以包含各种类型值的组合。元组使用括号 `()` 来构造（construct），而每个元组自身又是一个类型标记为 `(T1, T2, ...)` 的值，其中 `T1`、`T2` 是每个元素的类型。函数可以使用元组来返回多个值，因为元组可以拥有任意多个值。

> 和py tuple很不一样，类似 **快捷struct**。



* 固定长度不可变
  
* `mut` 后可修改内容

* 元素类型可以不同，但确定后不可变，也即同一index位置的类型不可变

  > 等于其实也有确定类型（known-size at compile time）

```rust
let a = (1, 1.5, true, 'a', "Hello, world!");
// a.0 = 1, a.1 = 1.5, a.2 = true, a.3 = 'a', a.4 = "Hello, world!"

let b: (i32, f64) = (1, 1.5);

let (c, d) = b; // c = 1, d = 1.5
let (e, _, _, _, f) = a; //e = 1, f = "Hello, world!", _ indicates not interested of that item

let g = (0,); //single-element tuple

let h = (b, (2, 4), 5); //((1, 1.5), (2, 4), 5)

println!("{:?}", a); //(1, 1.5, true, 'a', "Hello, world!")
```





```rust
// 元组可以充当函数的参数和返回值
fn reverse(pair: (i32, bool)) -> (bool, i32) {
    // 可以使用 `let` 把一个元组的成员绑定到一些变量
    let (integer, boolean) = pair;

    (boolean, integer)
}

// 在 “动手试一试” 的练习中要用到下面这个结构体。
#[derive(Debug)]
struct Matrix(f32, f32, f32, f32);

fn main() {
    // 包含各种不同类型的元组
    let long_tuple = (1u8, 2u16, 3u32, 4u64,
                      -1i8, -2i16, -3i32, -4i64,
                      0.1f32, 0.2f64,
                      'a', true);

    // 通过元组的下标来访问具体的值
    println!("long tuple first value: {}", long_tuple.0);
    println!("long tuple second value: {}", long_tuple.1);

    // 元组也可以充当元组的元素
    let tuple_of_tuples = ((1u8, 2u16, 2u32), (4u64, -1i8), -2i16);

    // 元组可以打印
    println!("tuple of tuples: {:?}", tuple_of_tuples);

    // 但很长的元组无法打印
    // let too_long_tuple = (1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13);
    // println!("too long tuple: {:?}", too_long_tuple);
    // 试一试 ^ 取消上面两行的注释，阅读编译器给出的错误信息。

    let pair = (1, true);
    println!("pair is {:?}", pair);

    println!("the reversed pair is {:?}", reverse(pair));

    // 创建单元素元组需要一个额外的逗号，这是为了和被括号包含的字面量作区分。
    println!("one element tuple: {:?}", (5u32,));
    println!("just an integer: {:?}", (5u32));

    // 元组可以被解构（deconstruct），从而将值绑定给变量
    let tuple = (1, "hello", 4.5, true);

    let (a, b, c, d) = tuple;
    println!("{:?}, {:?}, {:?}, {:?}", a, b, c, d);

    let matrix = Matrix(1.1, 1.2, 2.1, 2.2);
    println!("{:?}", matrix)

}

```







