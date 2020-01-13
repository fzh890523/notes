
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

### str

目测跟go的string比较类似。

* utf-8 （byte） sequence
* 长度未知（是指字符数吧，字节数应该是知道的）

差别：
* rust的str似乎都是static，也即运行时生成的字符串是String而不是str
  不可改变。
  String在堆中分配，可扩容/增长。

> In general, you should use String when you need ownership, and &str when you just need to borrow a string.

```rust
let a = "Hello, world."; //a: &'static str
let b: &str = "こんにちは, 世界!";
```

### functions

```rust
fn plus_one(a: i32) -> i32 {
    a + 1
}

let b: fn(i32) -> i32 = plus_one;
let c = b(5); //6
```

## 复杂类型 complex data types

### Vectors 

* 可扩容
* 类型需要一致
* 泛型类型，元素类型可以任意
  `Vec<T>`，如 `Vec<i32>`
* 从支持的语义操作，vec更像完整功能的go slice/java list
  * A pointer to the data
  * No of elements currently have(length)
  * Capacity (Amount of space allocated for any future elements).
  

```rust
let mut a = Vec::new(); //1.With new() keyword
let mut b = vec![]; //2.Using the vec! macro
// 这种非泛型形式的vec可以塞什么？


let mut a2: Vec<i32> = Vec::new();
let mut b2: Vec<i32> = vec![];
let mut b3 = vec![1i32, 2, 3];//Sufixing 1st value with data type

let mut b4 = vec![1, 2, 3];
let mut b5: Vec<i32> = vec![1, 2, 3];
let mut b6  = vec![1i32, 2, 3];  // 元素类型的标识法： 1i32 囧
let mut b7 = vec![0; 10]; //Ten zeroes



//Accessing and changing existing data
let mut c = vec![5, 4, 3, 2, 1];
c[0] = 1;
c[1] = 2;
//c[6] = 2; Cannot assign values this way, index out of bounds
println!("{:?}", c); //[1, 2, 3, 2, 1]

//push and pop
let mut d: Vec<i32> = Vec::new();
d.push(1); //[1] : Add an element to the end
d.push(2); //[1, 2]
d.pop(); //[1] : : Remove an element from the end


// 🔎 Capacity and reallocation
let mut e: Vec<i32> = Vec::with_capacity(10);
println!("Length: {}, Capacity : {}", e.len(), e.capacity()); //Length: 0, Capacity : 10

// These are all done without reallocating...
for i in 0..10 {
    e.push(i);
}
// ...but this may make the vector reallocate
e.push(11);
```

```rust
let mut v = vec![1, 2, 3, 4, 5];

for i in &v {
    println!("A reference to {}", i);
}

for i in &mut v {
    println!("A mutable reference to {}", i);
}

for i in v {
    println!("Take ownership of the vector and its element {}", i);
}
```


### Structs

三种风格的struct：
* c风格
  ```rust
  // Struct Declaration
  struct Color {
      red: u8,
      green: u8,
      blue: u8
  }
  ```

  特色的地方：
  * 可以从其他struct实例（部分）copy字段值
    ```rust
    let blue = Color{blue: 255, .. link_color}  // copy rest fileds from link_color instance
    ```
* tuple风格
  ```rust
  struct Color(u8, u8, u8);
  struct Kilometers(i32);  // 只有一个元素的情况叫做 newtype 模式，通过这种方式可以定义一个新的类似。 类似go的 type Kilometers int32
  ```
* unit struct
  没有成员的struct。 等于定义了一个数据无关的新类型。
  比较少单独使用，常组合其他特性一起使用，比如泛型、实现trait等。
  ```rust
  struct Electron;
  
  fn main() {
    let x = Electron;
  }
  ```

示例：

**c风格struct**

```rust
// Struct Declaration
struct Color {
    red: u8,
    green: u8,
    blue: u8
}

fn main() {
  // Creating an instance
  let black = Color {red: 0, green: 0, blue: 0};

  // Accessing its fields using dot notation
  println!("Black = rgb({}, {}, {})", black.red, black.green, black.blue); //Black = rgb(0, 0, 0)

  // Structs are immutable by default, use `mut` to make it mutable but doesn't support field level mutability
  let mut link_color = Color {red: 0,green: 0,blue: 255};
  link_color.blue = 238;
  println!("Link Color = rgb({}, {}, {})", link_color.red, link_color.green, link_color.blue); //Link Color = rgb(0, 0, 238)

  // Copy elements from another instance
  let blue = Color {blue: 255, .. link_color};
  println!("Blue = rgb({}, {}, {})", blue.red, blue.green, blue.blue); //Blue = rgb(0, 0, 255)

  // Destructure the instance using a `let` binding, this will not destruct blue instance
  let Color {red: r, green: g, blue: b} = blue;
  println!("Blue = rgb({}, {}, {})", r, g, b); //Blue = rgb(0, 0, 255)

  // Creating an instance via functions & accessing its fields
  let midnightblue = get_midnightblue_color();
  println!("Midnight Blue = rgb({}, {}, {})", midnightblue.red, midnightblue.green, midnightblue.blue); //Midnight Blue = rgb(25, 25, 112)

  // Destructure the instance using a `let` binding
  let Color {red: r, green: g, blue: b} = get_midnightblue_color();
  println!("Midnight Blue = rgb({}, {}, {})", r, g, b); //Midnight Blue = rgb(25, 25, 112)
}

fn get_midnightblue_color() -> Color {
    Color {red: 25, green: 25, blue: 112}
}
```

**tuple风格struct**

```rust
struct Color(u8, u8, u8);
struct Kilometers(i32);

fn main() {
  // Creating an instance
  let black = Color(0, 0, 0);

  // Destructure the instance using a `let` binding, this will not destruct black instance
  let Color(r, g, b) = black;
  println!("Black = rgb({}, {}, {})", r, g, b); //black = rgb(0, 0, 0);

  // Newtype pattern
  let distance = Kilometers(20);
  // Destructure the instance using a `let` binding
  let Kilometers(distance_in_km) = distance;
  println!("The distance: {} km", distance_in_km); //The distance: 20 km
}
```

### Enums

* 允许各实例类型不整齐 - 特色
  * 无类型的： unit
  * struct类型
  * tuple类型
* 带数据的枚举实例，跟其他的语言的，概念上可能有些差别
  不只是判断 == 某个实例了，而是判断是否是某个实例（的类型），然后可以对带的数据做处理

```rust
enum Day {
    Sunday,
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday
}

// The `Day` is the enum
// Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday are the variants


enum FlashMessage {
  Success, // A unit variant
  Warning{ category: i32, message: String }, // A struct variant
  Error(String) // A tuple variant
}

fn main() {
  let mut form_status = FlashMessage::Success;
  print_flash_message(form_status);

  form_status = FlashMessage::Warning {category: 2, message: String::from("Field X is required")};
  print_flash_message(form_status);

  form_status = FlashMessage::Error(String::from("Connection Error"));
  print_flash_message(form_status);
}

fn print_flash_message(m : FlashMessage) {
  // Pattern matching with enum
  match m {
    FlashMessage::Success =>
      println!("Form Submitted correctly"),
    FlashMessage::Warning {category, message} => // Destructure, should use same field names
      println!("Warning : {} - {}", category, message),
    FlashMessage::Error(msg) =>
      println!("Error : {}", msg)
  }
}
```

### Generics


**泛型函数**

```rust
fn takes_anything<T>(x: T) { // x has type T, T is a generic type
}

fn takes_two_of_the_same_things<T>(x: T, y: T) { // Both x and y has the same type
}

fn takes_two_things<T, U>(x: T, y: U) { // Multiple types
}
```

**泛型struct**

* 语法上跟其他语言的类似
* 没有 类型约束/边界 的支持？
  有的：
  ```rust
  fn make_sound<T: GetSound>(t: &T) {
      println!("{}!", t.get_sound())
  }  
  ```

```rust
struct Point<T> {
  x: T,
  y: T,
}

fn main() {
  let point_a = Point { x: 0, y: 0 }; // T is a int type
  let point_b = Point { x: 0.0, y: 0.0 }; // T is a float type
}

// 🔎 When adding an implementation for a generic struct, the type parameters should be declared after the impl as well
//   impl<T> Point<T> {
```

**泛型枚举**

```rust
enum Option<T> {
    Some(T),
    None,
}

enum Result<T, E> {
    Ok(T),
    Err(E),
}
```