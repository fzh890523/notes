



结构体（structure，缩写成 struct）有 3 种类型，使用 `struct` 关键字来创建：

- 元组结构体（tuple struct），事实上就是具名元组而已。
- 经典的 [C 语言风格结构体](https://en.wikipedia.org/wiki/Struct_(C_programming_language))（C struct）。
- 单元结构体（unit struct），不带字段，在泛型中很有用。



```rust
#[derive(Debug)]
struct Person {
    name: String,
    age: u8,
}

// 单元结构体
struct Unit;

// 元组结构体
struct Pair(i32, f32);

// 带有两个字段的结构体
struct Point {
    x: f32,
    y: f32,
}

// 结构体可以作为另一个结构体的字段
#[allow(dead_code)]
struct Rectangle {
    // 可以在空间中给定左上角和右下角在空间中的位置来指定矩形。
    top_left: Point,
    bottom_right: Point,
}

fn main() {
    // 使用简单的写法初始化字段，并创建结构体
    let name = String::from("Peter");
    let age = 27;
    let peter = Person { name, age };

    // 以 Debug 方式打印结构体
    println!("{:?}", peter);

    // 实例化结构体 `Point`
    let point: Point = Point { x: 10.3, y: 0.4 };

    // 访问 point 的字段
    println!("point coordinates: ({}, {})", point.x, point.y);

    // 使用结构体更新语法创建新的 point，
    // 这样可以用到之前的 point 的字段
    let bottom_right = Point { x: 5.2, ..point };

    // `bottom_right.y` 与 `point.y` 一样，因为这个字段就是从 `point` 中来的
    println!("second point: ({}, {})", bottom_right.x, bottom_right.y);

    // 使用 `let` 绑定来解构 point
    let Point { x: left_edge, y: top_edge } = point;

    let _rectangle = Rectangle {
        // 结构体的实例化也是一个表达式
        top_left: Point { x: left_edge, y: top_edge },
        bottom_right: bottom_right,
    };

    // 实例化一个单元结构体
    let _unit = Unit;

    // 实例化一个元组结构体
    let pair = Pair(1, 0.1);

    // 访问元组结构体的字段
    println!("pair contains {:?} and {:?}", pair.0, pair.1);

    // 解构一个元组结构体
    let Pair(integer, decimal) = pair;

    println!("pair contains {:?} and {:?}", integer, decimal);
}

```





### 三种风格的struct

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





### struct（字段）可见性

结构体的字段也是一个可见性的层次。字段默认拥有私有的可见性，也可以加上 `pub` 修 饰语来重载该行为。只有从结构体被定义的模块之外访问其字段时，这个可见性才会 起作用，其意义是隐藏信息（即封装，encapsulation）。



* 默认private （也即“protected”，struct同级别可见）



```rust
mod my {
    // 一个公有的结构体，带有一个公有的字段（类型为泛型 `T`）
    pub struct OpenBox<T> {
        pub contents: T,
    }

    // 一个公有的结构体，带有一个私有的字段（类型为泛型 `T`）    
    #[allow(dead_code)]
    pub struct ClosedBox<T> {
        contents: T,
    }

    impl<T> ClosedBox<T> {
        // 一个公有的构造器方法
        pub fn new(contents: T) -> ClosedBox<T> {
            ClosedBox {
                contents: contents,
            }
        }
    }
}

fn main() {
    // 带有公有字段的公有结构体，可以像平常一样构造
    let open_box = my::OpenBox { contents: "public information" };

    // 并且它们的字段可以正常访问到。
    println!("The open box contains: {}", open_box.contents);

    // 带有私有字段的公有结构体不能使用字段名来构造。
    // 报错！`ClosedBox` 含有私有字段。
    //let closed_box = my::ClosedBox { contents: "classified information" };
    // 试一试 ^ 取消此行注释


    // 不过带有私有字段的结构体可以使用公有的构造器来创建。
    let _closed_box = my::ClosedBox::new("classified information");

    // 并且一个结构体中的私有字段不能访问到。
    // 报错！`content` 字段是私有的。
    //println!("The closed box contains: {}", _closed_box.contents);
    // 试一试 ^ 取消此行注释    

}

```





### 类型别名（struct）



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

