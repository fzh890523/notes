



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

