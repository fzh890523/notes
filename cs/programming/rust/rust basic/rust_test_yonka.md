

Rust 是一门非常重视正确性的语言，这门语言本身就提供了对编写软件测试的支持。

测试有三种风格：

- [单元](http://localhost:3000/testing/unit_testing.html)测试。
- [文档](http://localhost:3000/testing/doc_testing.html)测试。
- [集成](http://localhost:3000/testing/integration_testing.html)测试。

Rust 也支持在测试中指定额外的依赖：

- [开发依赖](http://localhost:3000/testing/dev_dependencies.html)












* 可以在同文件里
  
  ```rust
  #[test]
  fn test_greet() {}
  ```



# 单测



测试（test）是这样一种 Rust 函数：它保证其他部分的代码按照所希望的行为正常 运行。测试函数的函数体通常会进行一些配置，运行我们想要测试的代码，然后 断言（assert）结果是不是我们所期望的。

大多数单元测试都会被放到一个叫 `tests` 的、带有 `#[cfg(test)]` 属性 的[模块](http://localhost:3000/mod.html)中，测试函数要加上 `#[test]` 属性。

> y: 确实拆单独文件更好点

当测试函数中有什么东西 [panic](http://localhost:3000/std/panic.html) 了，测试就失败。有一些这方面的 辅助[宏](http://localhost:3000/macros.html)：

- `assert!(expression)` - 如果表达式的值是 `false` 则 panic。
- `assert_eq!(left, right)` 和 `assert_ne!(left, right)` - 检验左右两边是否 相等/不等。

```rust
pub fn add(a: i32, b: i32) -> i32 {
    a + b
}

// 这个加法函数写得很差，本例中我们会使它失败。
#[allow(dead_code)]
fn bad_add(a: i32, b: i32) -> i32 {
    a - b
}

#[cfg(test)]
mod tests {
    // 注意这个惯用法：在 tests 模块中，从外部作用域导入所有名字。
    use super::*;

    #[test]
    fn test_add() {
        assert_eq!(add(1, 2), 3);
    }

    #[test]
    fn test_bad_add() {
        // 这个断言会导致测试失败。注意私有的函数也可以被测试！
        assert_eq!(bad_add(1, 2), 3);
    }
}
```

可以使用 `cargo test` 来运行测试。

```bash
$ cargo test

running 2 tests
test tests::test_bad_add ... FAILED
test tests::test_add ... ok

failures:

---- tests::test_bad_add stdout ----
        thread 'tests::test_bad_add' panicked at 'assertion failed: `(left == right)`
  left: `-1`,
 right: `3`', src/lib.rs:21:8
note: Run with `RUST_BACKTRACE=1` for a backtrace.


failures:
    tests::test_bad_add

test result: FAILED. 1 passed; 1 failed; 0 ignored; 0 measured; 0 filtered out
```

## 测试 panic

一些函数应当在特定条件下 panic。为测试这种行为，请使用 `#[should_panic]` 属性。这 个属性接受可选参数 `expected = `以指定 panic 时的消息。如果你的函数能以多种方式 panic，这个属性就保证了你在测试的确实是所指定的 panic。

```rust
pub fn divide_non_zero_result(a: u32, b: u32) -> u32 {
    if b == 0 {
        panic!("Divide-by-zero error");
    } else if a < b {
        panic!("Divide result is zero");
    }
    a / b
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_divide() {
        assert_eq!(divide_non_zero_result(10, 2), 5);
    }

    #[test]
    #[should_panic]
    fn test_any_panic() {
        divide_non_zero_result(1, 0);
    }

    #[test]
    #[should_panic(expected = "Divide result is zero")]
    fn test_specific_panic() {
        divide_non_zero_result(1, 10);
    }
}
```

运行这些测试会输出：

```bash
$ cargo test

running 3 tests
test tests::test_any_panic ... ok
test tests::test_divide ... ok
test tests::test_specific_panic ... ok

test result: ok. 3 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out

   Doc-tests tmp-test-should-panic

running 0 tests

test result: ok. 0 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out
```



## 运行特定的测试

要运行特定的测试，只要把测试名称传给 `cargo test` 命令就可以了。

```bash
$ cargo test test_any_panic
running 1 test
test tests::test_any_panic ... ok

test result: ok. 1 passed; 0 failed; 0 ignored; 0 measured; 2 filtered out

   Doc-tests tmp-test-should-panic

running 0 tests

test result: ok. 0 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out
```

要运行多个测试，可以仅指定测试名称中的一部分，用它来匹配所有要运行的测试。

```bash
$ cargo test panic
running 2 tests
test tests::test_any_panic ... ok
test tests::test_specific_panic ... ok

test result: ok. 2 passed; 0 failed; 0 ignored; 0 measured; 1 filtered out

   Doc-tests tmp-test-should-panic

running 0 tests

test result: ok. 0 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out
```

## 忽略测试

可以把属性 `#[ignore]` 赋予测试以排除某些测试，或者使用 `cargo test -- --ignored` 命令来运行它们。

```rust
pub fn add(a: i32, b: i32) -> i32 {
    a + b
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_add() {
        assert_eq!(add(2, 2), 4);
    }

    #[test]
    fn test_add_hundred() {
        assert_eq!(add(100, 2), 102);
        assert_eq!(add(2, 100), 102);
    }

    #[test]
    #[ignore]
    fn ignored_test() {
        assert_eq!(add(0, 0), 0);
    }
}
$ cargo test
running 1 test
test tests::ignored_test ... ignored

test result: ok. 0 passed; 0 failed; 1 ignored; 0 measured; 0 filtered out

   Doc-tests tmp-ignore

running 0 tests

test result: ok. 0 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out

$ cargo test -- --ignored
running 1 test
test tests::ignored_test ... ok

test result: ok. 1 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out

   Doc-tests tmp-ignore

running 0 tests

test result: ok. 0 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out
```



# 文档测试



为 Rust 工程编写文档的主要方式是在源代码中写注释。文档注释使用 [markdown](https://daringfireball.net/projects/markdown/) 语法 书写，支持代码块。Rust 很注重正确性，这些注释中的代码块也会被编译并且用作测试。

```rust
/// 第一行是对函数的简短描述。
///
/// 接下来数行是详细文档。代码块用三个反引号开启，Rust 会隐式地在其中添加
/// `fn main()` 和 `extern crate <cratename>`。比如测试 `doccomments` crate：
///
/// ```
/// let result = doccomments::add(2, 3);
/// assert_eq!(result, 5);
/// ```
pub fn add(a: i32, b: i32) -> i32 {
    a + b
}

/// 文档注释通常可能带有 "Examples"、"Panics" 和 "Failures" 这些部分。
///
/// 下面的函数将两数相除。
///
/// # Examples
///
/// ```
/// let result = doccomments::div(10, 2);
/// assert_eq!(result, 5);
/// ```
///
/// # Panics
///
/// 如果第二个参数是 0，函数将会 panic。
///
/// ```rust,should_panic
/// // panics on division by zero
/// doccomments::div(10, 0);
/// ```
pub fn div(a: i32, b: i32) -> i32 {
    if b == 0 {
        panic!("Divide-by-zero error");
    }

    a / b
}
```

这些测试仍然可以通过 `cargo test` 执行：

```bash
$ cargo test
running 0 tests

test result: ok. 0 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out

   Doc-tests doccomments

running 3 tests
test src/lib.rs - add (line 7) ... ok
test src/lib.rs - div (line 21) ... ok
test src/lib.rs - div (line 31) ... ok

test result: ok. 3 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out
```

## 文档测试的目的

**文档测试的主要目的是作为使用函数功能的例子**，这是最重要的[指导 原则](https://rust-lang-nursery.github.io/api-guidelines/documentation.html#examples-use--not-try-not-unwrap-c-question-mark)之一。文档测试应当可以作为完整的代码段被直接 使用（很多用户会复制文档中的代码来用，所以例子要写得完善）。但使用 `?` 符号会 导致编译失败，因为 `main` 函数会返回 `unit` 类型。幸运的是，我们可以在文档中 隐藏几行源代码：你可以写 `fn try_main() -> Result<(), ErrorType>` 这样的 函数，把它隐藏起来，然后在隐藏的 `main` 中展开它。听起来很复杂？请看例子：

```rust
/// 在文档测试中使用隐藏的 `try_main`。
///
/// ```
/// # // 被隐藏的行以 `#` 开始，但它们仍然会被编译！
/// # fn try_main() -> Result<(), String> { // 隐藏行包围了文档中显示的函数体
/// let res = try::try_div(10, 2)?;
/// # Ok(()) // 从 try_main 返回
/// # }
/// # fn main() { // 开始主函数，其中将展开 `try_main` 函数
/// #    try_main().unwrap(); // 调用并展开 try_main，这样出错时测试会 panic
/// # }
pub fn try_div(a: i32, b: i32) -> Result<i32, String> {
    if b == 0 {
        Err(String::from("Divide-by-zero"))
    } else {
        Ok(a / b)
    }
}
```

**参见**

- 关于文档风格的 [RFC505](https://github.com/rust-lang/rfcs/blob/master/text/0505-api-comment-conventions.md)
- [API 指导原则](https://rust-lang-nursery.github.io/api-guidelines/documentation.html)中关于文档的原则



# 集成测试

[单元测试](http://localhost:3000/testing/unit_testing.html)一次仅能单独测试一个模块，这种测试是小规模的，并且能测试私有 代码；集成测试是 crate 外部的测试，并且仅使用 crate 的公共接口，就像其他使用 该 crate 的程序那样。集成测试的目的是检验你的库的各部分是否能够正确地协同工作。

cargo 在与 `src` 同级别的 `tests` 目录寻找集成测试。

文件 `src/lib.rs`：

```rust
// 在一个叫做 'adder' 的 crate 中定义此函数。
pub fn add(a: i32, b: i32) -> i32 {
    a + b
}
```

包含测试的文件：`tests/integration_test.rs`：

```rust
#[test]
fn test_add() {
    assert_eq!(adder::add(3, 2), 5);
}
```

使用 `cargo test` 命令：

```bash
$ cargo test
running 0 tests

test result: ok. 0 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out

     Running target/debug/deps/integration_test-bcd60824f5fbfe19

running 1 test
test test_add ... ok

test result: ok. 1 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out

   Doc-tests adder

running 0 tests

test result: ok. 0 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out
```

`tests` 目录中的每一个 Rust 源文件都被编译成一个单独的 crate。在集成测试中要想 共享代码，一种方式是创建具有公用函数的模块，然后在测试中导入并使用它。

文件 `tests/common.rs`:

```rust
pub fn setup() {
    // 一些配置代码，比如创建文件/目录，开启服务器等等。
}
```

包含测试的文件：`tests/integration_test.rs`

```rust
// 导入共用模块。
mod common;

#[test]
fn test_add() {
    // 使用共用模块。
    common::setup();
    assert_eq!(adder::add(3, 2), 5);
}
```

带有共用代码的模块遵循和普通的[模块](http://localhost:3000/mod.html)一样的规则，所以完全可以把公共模块 写在 `tests/common/mod.rs` 文件中。



# 开发依赖(dev/test dep)

有时仅在测试中才需要一些依赖（比如基准测试相关的）。这种依赖要写在 `Cargo.toml` 的 `[dev-dependencies]` 部分。这些依赖不会传播给其他依赖于这个包的包。

比如说使用 `pretty_assertions`，这是扩展了标准的 `assert!` 宏的一个 crate。

文件 `Cargo.toml`:

```ignore
# 这里省略了标准的 crate 数据
[dev-dependencies]
pretty_assertions = "1"
```

文件 `src/lib.rs`:

```rust
pub fn add(a: i32, b: i32) -> i32 {
    a + b
}

#[cfg(test)]
mod tests {
    use super::*;
    use pretty_assertions::assert_eq; // 仅用于测试, 不能在非测试代码中使用

    #[test]
    fn test_add() {
        assert_eq!(add(2, 3), 5);
    }
}
```

## 参见

[Cargo](https://doc.crates.io/specifying-dependencies.html) 文档中关于指定依赖的部分。

