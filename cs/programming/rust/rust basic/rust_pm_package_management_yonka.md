

`cargo` 是官方的 Rust 包管理工具。 它有很多非常有用的功能来提高代码质量和开发人员的开发效率！ 这些功能包括：

- 依赖管理和与 [crates.io](https://crates.io/)（官方 Rust 包注册服务）集成
- 方便的单元测试
- 方便的基准测试



## 依赖

创建一个新的 Rust 项目：

```sh
# 二进制可执行文件
cargo new foo

# 或者库
cargo new --lib foo
```



完成上述命令后，将看到如下内容：

```txt
foo
├── Cargo.toml
└── src
    └── main.rs  # or lib.rs 
```

`main.rs` 是新项目的入口源文件——这里没什么新东西。 `Cargo.toml` 是本项目（`foo`）的 `cargo` 的配置文件。 浏览 `Cargo.toml` 文件，将看到类似以下的的内容：

```toml
[package]
name = "foo"
version = "0.1.0"
authors = ["mark"]

[dependencies]
```

`package` 下面的 `name` 字段表明项目的名称。 如果您发布 crate（后面将做更多介绍），那么 `crates.io` 将使用此字段标明的名称。 这也是编译时输出的二进制可执行文件的名称。

`version` 字段是使用[语义版本控制](https://semver.org/)（Semantic Versioning）的 crate 版本号。

`authors` 字段表明发布 crate 时的作者列表。

`dependencies` 这部分可以让你为项目添加依赖。



`cargo` 还支持[其他类型的依赖](https://doc.rust-lang.org/cargo/reference/specifying-dependencies.html)。 下面是一个简单的示例：

```toml
[package]
name = "foo"
version = "0.1.0"
authors = ["mark"]

[dependencies]
clap = "2.27.1" # 来自 crates.io
rand = { git = "https://github.com/rust-lang-nursery/rand" } # 来自网上的仓库
bar = { path = "../bar" } # 来自本地文件系统的路径
```

`cargo` 不仅仅是一个包依赖管理器。`Cargo.toml` 的所有可用配置选项都列在 [格式规范](https://doc.rust-lang.org/cargo/reference/manifest.html)中。

要构建我们的项目，我们可以在项目目录中的任何位置（包括子目录！）执行 `cargo build`。我们也可以执行 `cargo run` 来构建和运行。请注意，这些命令将处理所有依赖，在需要时下载 crate，并构建所有内容，包括 crate。（请注意，它只重新构建尚未构建的内容，这和 `make` 类似）。



## 执行入口



约定

* 默认 `<crate>/src/main.rs`
* 可以额外 `<crate>/src/bin/my_other_bin.rs`来添加其他入口



编译： `--bin my_other_bin`这样来指定入口rs



## test



约定：

* 单侧： 放在要测试的模块（也即源代码处）中

* 集成测试： 放在`tests/`目录下

  ```sh
  foo
  ├── Cargo.toml
  ├── src
  │   └── main.rs
  └── tests
      ├── my_test.rs
      └── my_other_test.rs
  ```

  每个文件一个集成测试



执行： `cargo test`

可以指定匹配模式，如`cargo test test_foo`会prefix match`test_foo*`



## build







有时使用 `cargo` 正常构建还是不够的。也许你的 crate 在 cargo 成功编译之前需要一些先决条件，比如代码生成或者需要编译的一些本地代码。为了解决这个问题，我们构建了 cargo 可以运行的脚本。

要向包中添加构建脚本，可以在 `Cargo.toml` 中指定它，如下所示：

```toml
[package]
...
build = "build.rs"
```

跟默认情况不同，这里 cargo 将在项目目录中优先查找 `build.rs` 文件。（本句采用意译，英文原文为：Otherwise Cargo will look for a `build.rs` file in the project directory by default.）



构建脚本只是另一个 Rust 文件，此文件将在编译包中的任何其他内容之前，优先进行编译和调用。 因此，此文件可实现满足 crate 的先决条件。

cargo 通过[此处指定](https://doc.rust-lang.org/cargo/reference/environment-variables.html#environment-variables-cargo-sets-for-build-scripts)的可以使用的环境变量为脚本提供输入。（英文原文：Cargo provides the script with inputs via environment variables [specified here](https://doc.rust-lang.org/cargo/reference/environment-variables.html#environment-variables-cargo-sets-for-build-scripts) that can be used.）

此脚本通过 stdout （标准输出）提供输出。打印的所有行都写入到 `target/debug/build/<pkg>/output`。另外，以 `cargo:` 为前缀的行将由 cargo 直接解析，因此可用于定义包编译的参数。







