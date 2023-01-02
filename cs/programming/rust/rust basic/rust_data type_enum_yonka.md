



`enum` 关键字允许创建一个从数个不同取值中选其一的枚举类型（enumeration）。任何一个在 `struct` 中合法的取值在 `enum` 中也合法。

> 保留one-of语义，只是在one-of-value（of same type）之外，支持了不同类型



```rust
// 该属性用于隐藏对未使用代码的警告。
#![allow(dead_code)]

// 创建一个 `enum`（枚举）来对 web 事件分类。注意变量名和类型共同指定了 `enum`
// 取值的种类：`PageLoad` 不等于 `PageUnload`，`KeyPress(char)` 不等于
// `Paste(String)`。各个取值不同，互相独立。
enum WebEvent {
    // 一个 `enum` 可以是单元结构体（称为 `unit-like` 或 `unit`），
    PageLoad,
    PageUnload,
    // 或者一个元组结构体，
    KeyPress(char),
    Paste(String),
    // 或者一个普通的结构体。
    Click { x: i64, y: i64 }
}

// 此函数将一个 `WebEvent` enum 作为参数，无返回值。
fn inspect(event: WebEvent) {
    match event {
        WebEvent::PageLoad => println!("page loaded"),
        WebEvent::PageUnload => println!("page unloaded"),
        // 从 `enum` 里解构出 `c`。
        WebEvent::KeyPress(c) => println!("pressed '{}'.", c),
        WebEvent::Paste(s) => println!("pasted \"{}\".", s),
        // 把 `Click` 解构给 `x` and `y`。
        WebEvent::Click { x, y } => {
            println!("clicked at x={}, y={}.", x, y);
        },
    }
}

fn main() {
    let pressed = WebEvent::KeyPress('x');
    // `to_owned()` 从一个字符串切片中创建一个具有所有权的 `String`。
    let pasted  = WebEvent::Paste("my text".to_owned());
    let click   = WebEvent::Click { x: 20, y: 80 };
    let load    = WebEvent::PageLoad;
    let unload  = WebEvent::PageUnload;

    inspect(pressed);
    inspect(pasted);
    inspect(click);
    inspect(load);
    inspect(unload);
}

```



```rust
// 该属性用于隐藏对未使用代码的警告。
#![allow(dead_code)]

enum Status {
    Rich,
    Poor,
}

enum Work {
    Civilian,
    Soldier,
}

fn main() {
    // 显式地 `use` 各个名称使他们直接可用，而不需要指定它们来自 `Status`。
    use Status::{Poor, Rich};
    // 自动地 `use` `Work` 内部的各个名称。
    use Work::*;

    // `Poor` 等价于 `Status::Poor`。
    let status = Poor;
    // `Civilian` 等价于 `Work::Civilian`。
    let work = Civilian;

    match status {
        // 注意这里没有用完整路径，因为上面显式地使用了 `use`。
        Rich => println!("The rich have lots of money!"),
        Poor => println!("The poor have no money..."),
    }

    match work {
        // 再次注意到没有用完整路径。
        Civilian => println!("Civilians work!"),
        Soldier  => println!("Soldiers fight!"),
    }
}

```







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





* c style enum

  ```rust
  // 该属性用于隐藏对未使用代码的警告。
  #![allow(dead_code)]
  
  // 拥有隐式辨别值（implicit discriminator，从 0 开始）的 enum
  enum Number {
      Zero,
      One,
      Two,
  }
  
  // 拥有显式辨别值（explicit discriminator）的 enum
  enum Color {
      Red = 0xff0000,
      Green = 0x00ff00,
      Blue = 0x0000ff,
  }
  
  fn main() {
      // `enum` 可以转成整型。
      println!("zero is {}", Number::Zero as i32);
      println!("one is {}", Number::One as i32);
  
      println!("roses are #{:06x}", Color::Red as i32);
      println!("violets are #{:06x}", Color::Blue as i32);
  }
  
  ```

  