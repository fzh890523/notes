### arrays/tuples/slice

#### arrays

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

#### tuples

和py tuple不一样，类似 **快捷struct**。

* 固定长度不可变
  `mut` 表示？
* 元素类型可以不同，但确定后不可变，也即同一index位置的类型不可变

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

#### slice

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