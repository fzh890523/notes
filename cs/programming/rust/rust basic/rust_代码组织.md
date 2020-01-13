

* function
* module
  * same file
  * different file, same dir
  * different dir

# module

ref: https://learning-rust.github.io/docs/d3.modules.html

* 大约是 static block/namespace 的概念
* dir/file本身act as module
  `mod.rs`作为该dir module的入口，也即 `xx/mod.rs` 中的symbol `hello`只需要`xx::hello`而不是`xx::mod::hello` （类比py的`__init__.py`）
  此外，file内可以再声明module
  > 参考py
* 访问方式 `${outerMod}::${innerMod}::${symbol}`
  但还没弄明白 相对路径 和 完全路径 的关系，到底是？
* 访问权限: 见下面节
* 定义mod
  ```rust
  mod tests {}
  ```
* 引用mod
  ```rust
  mod tests;  // Import mod.
  use tests::greet;  // Import symbol of mod.
  use super::hello;
  ```
* 用`super`来访问outer/parent mod
  `super::hello()`
* `self`来访问本级mod
  > 好像很奇怪的样子。。。 -> 可以用来相对引用，比如 `use self::grettings::hello`

## 访问方式

* `mod`
  好像是不能import dir下面的文件（sub modules），也即 `mod phrases::grettings` 是不允许的，可以 `mod phrases; phrases::grettings::hello()` （如果权限允许）。
  > 这样看起来好烦

  还有个做法是在`phrases/mod.rs`里把子module `greetings`的`hello` re-export到本级： `pub mod greetings; pub use self::grettings::hello`
* `use`

## module/symbol 访问控制

* dir/file module默认<del>pub</del>“protected”
  这个protected借用java的概念，同dir访问应该是可以的； --- 或者把dir/file理解为module的话，那就等于同module能访问private symbol，也是OK的
  如果要在外访问，要在parent mod的入口（entry point，也即 `mod.rs`）里声明为pub，如：
  ```rust
  // main.rs
  mod phrases;
  fn main() {
      phrases::greetings::hello();
      // 访问同包的phases可以，但不同包的greetings需要。。。
  }

  // phrases/mod.rs
  pub mod greetings;

  // phrases/greetings.rs
  pub fn hello() {}
  ```
* nest module默认<del>private</del>protected，需要显式pub；
* nest module里的symbol需要显式pub才能被outside访问
* 同module或者子module能访问private ...
  子module内能访问parent的private...

  ```rust
  pub mod greeting {
      pub fn hell() {}
  }
  ```
## module in same file

```rust
fn main() {
   greetings::hello();
}

mod greetings {
  // ⭐️ By default, everything inside a module is private
  pub fn hello() { // ⭐️ So function has to be public to access from outside
    println!("Hello, world!");
  }
}
```

嵌套的module
```rust
fn main() { 
  phrases::greetings::hello();
}

mod phrases { 
  pub mod greetings { 
    pub fn hello() { 
      println!("Hello, world!");
    }
  }
}
```

## module in different file, same dir

```rust
// ↳ main.rs
mod greetings; // Import greetings module

fn main() {
  greetings::hello();
}

// ↳ greetings.rs
// ⭐️ No need to wrap the code with a mod declaration. The file itself acts as a module.
pub fn hello() { // The function has to be public to access from outside
  println!("Hello, world!");
}
```

```rust
// ↳ main.rs
mod phrases;

fn main() {
  phrases::greetings::hello();
}

// ↳ phrases.rs
pub mod greetings { // ⭐️ The module has to be public to access from outside
  pub fn hello() {
    println!("Hello, world!");
  }
}
```

## module in different file, different dir

```rust
// ↳ main.rs
mod greetings;

fn main() {
  greetings::hello();
}

// ↳ greetings/mod.rs
pub fn hello() { // ⭐️ The function has to be public to access from outside
  println!("Hello, world!");
}
```

`mod.rs`作为dir module的入口
```rust
// ↳ main.rs
mod phrases;

fn main() {
  phrases::greetings::hello();
}

// ↳ phrases/mod.rs
pub mod greetings { // ⭐️ The module has to be public to access from outside
  pub fn hello() {
    println!("Hello, world!");
  }
}
```

其他文件作为dir module的子module
```rust
// ↳ main.rs
mod phrases;

fn main() {
  phrases::hello()
}

// ↳ phrases/mod.rs
mod greetings;

pub fn hello() {
  greetings::hello()
}

// ↳ phrases/greetings.rs
pub fn hello() {
  println!("Hello, world!");
}
```


```rust
// ↳ main.rs
mod phrases;

fn main() {
    phrases::greetings::hello();
}

// ↳ phrases/mod.rs
pub mod greetings;  // ⭐️ `pub mod` instead `mod`

// ↳ phrases/greetings.rs
pub fn hello() {
  println!("Hello, world!");
}
```

```rust
// ↳ phrases/greetings.rs
pub fn hello() {
  println!("Hello, world!");
}

// ↳ phrases/mod.rs
pub mod greetings;

pub use self::greetings::hello; // Re-export `greetings::hello` to phrases

// ↳ main.rs
mod phrases;

fn main() {
    phrases::hello(); // You can call `hello()` directly from phrases
}
```

# crates

* 类似其他语言的package
* 每个crate单独编译
  其中的文件会和crate文件merge然后编译为一个单元
* 每个crate会产生一个 以下其一
  * 可执行二进制： `src/main.rs`作为入口
    此时仍然可以有`src/lib.rs`并且可以把主逻辑丢进去然后再`src/main.rs`里调它
  * 一个lib： `src/lib.rs`作为入口

  > 单入口好像有点蠢啊？！

```rust
// # Think we run,
cargo new greetings
touch greetings/src/lib.rs

// # It generates,
greetings
 ├── Cargo.toml
 └── src
    ├── lib.rs
    └── main.rs

// # Think we modify following files,

// 01. greetings/src/lib.rs
pub fn hello() {
    println!("Hello, world!");
}

// 02. greetings/src/main.rs
extern crate greetings;

fn main() {
    greetings::hello();
}
```

# workspaces

# use

