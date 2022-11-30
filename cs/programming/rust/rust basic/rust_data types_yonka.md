
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



## 复杂类型 complex data types




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



#### 类型别名（struct）



```rust
// 定义一个包含单个 `Vec` 的结构体 `List`。
struct List(Vec<i32>);

impl fmt::Display for List {
  fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
    // ...
  }
}
```



这里类似 go的`type MyInt1 int`、`type List Vec<i32>` （只是模仿）。


