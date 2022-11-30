### 

## **泛型函数**

```rust
fn takes_anything<T>(x: T) { // x has type T, T is a generic type
}

fn takes_two_of_the_same_things<T>(x: T, y: T) { // Both x and y has the same type
}

fn takes_two_things<T, U>(x: T, y: U) { // Multiple types
}
```

## **泛型struct**

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

## **泛型枚举**

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