

* function
* module
  * same file
  * different file, same dir
  * different dir

# module

ref: https://learning-rust.github.io/docs/d3.modules.html

* 大约是 static block/namespace 的概念

  ```rust
  // 伪码
  type Module = Vec<Item>
  
  type Item enum {
    Func
    Struct
    Trait
    ImplBlock
    Module
  }
  ```

  

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

## module/symbol 访问控制/可见性


* 使用

  ```rust
  Syntax
  Visibility :
        pub
     | pub ( crate )
     | pub ( self )
     | pub ( super )
     | pub ( in SimplePath )
  ```

* 行为


  * By default, everything is *private*, with two exceptions: 

    > rust的private应该是protected概念，同“级”都可见


    * Associated items in a `pub` Trait are public by default; 
    * Enum variants in a `pub` enum are also public by default. 

  * When an item is declared as `pub`, it can be thought of as being accessible to the outside world

  


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

* module本身也有pub概念

  y: pub/non-pub mod 具体语义？ 
  
  应该还是和其他一样，non-pub mod只有“同级”可见





```rust
// 一个名为 `my_mod` 的模块
mod my_mod {
    // 模块中的项默认具有私有的可见性
    fn private_function() {
        println!("called `my_mod::private_function()`");
    }

    // 使用 `pub` 修饰语来改变默认可见性。
    pub fn function() {
        println!("called `my_mod::function()`");
    }

    // 在同一模块中，项可以访问其它项，即使它是私有的。
    pub fn indirect_access() {
        print!("called `my_mod::indirect_access()`, that\n> ");
        private_function();
    }

    // 模块也可以嵌套
    pub mod nested {
        pub fn function() {
            println!("called `my_mod::nested::function()`");
        }

        #[allow(dead_code)]
        fn private_function() {
            println!("called `my_mod::nested::private_function()`");
        }

        // 使用 `pub(in path)` 语法定义的函数只在给定的路径中可见。
        // `path` 必须是父模块（parent module）或祖先模块（ancestor module）
        // y: 过于灵活
        pub(in crate::my_mod) fn public_function_in_my_mod() {
            print!("called `my_mod::nested::public_function_in_my_mod()`, that\n > ");
            public_function_in_nested()
        }

        // 使用 `pub(self)` 语法定义的函数则只在当前模块中可见。
        // y: 这不等于没加？
        pub(self) fn public_function_in_nested() {
            println!("called `my_mod::nested::public_function_in_nested");
        }

        // 使用 `pub(super)` 语法定义的函数只在父模块中可见。
        pub(super) fn public_function_in_super_mod() {
            println!("called my_mod::nested::public_function_in_super_mod");
        }
    }

    pub fn call_public_function_in_my_mod() {
        print!("called `my_mod::call_public_funcion_in_my_mod()`, that\n> ");
        nested::public_function_in_my_mod();
        print!("> ");
        nested::public_function_in_super_mod();
    }

    // `pub(crate)` 使得函数只在当前 crate 中可见
    pub(crate) fn public_function_in_crate() {
        println!("called `my_mod::public_function_in_crate()");
    }

    // 嵌套模块的可见性遵循相同的规则
    mod private_nested {
        #[allow(dead_code)]
        pub fn function() {
            println!("called `my_mod::private_nested::function()`");
        }
    }
}

fn function() {
    println!("called `function()`");
}

fn main() {
    // 模块机制消除了相同名字的项之间的歧义。
    function();
    my_mod::function();

    // 公有项，包括嵌套模块内的，都可以在父模块外部访问。
    my_mod::indirect_access();
    my_mod::nested::function();
    my_mod::call_public_function_in_my_mod();

    // pub(crate) 项可以在同一个 crate 中的任何地方访问
    my_mod::public_function_in_crate();

    // pub(in path) 项只能在指定的模块中访问
    // 报错！函数 `public_function_in_my_mod` 是私有的
    //my_mod::nested::public_function_in_my_mod();
    // 试一试 ^ 取消该行的注释

    // 模块的私有项不能直接访问，即便它是嵌套在公有模块内部的

    // 报错！`private_function` 是私有的
    //my_mod::private_function();
    // 试一试 ^ 取消此行注释

    // 报错！`private_function` 是私有的
    //my_mod::nested::private_function();
    // 试一试 ^ 取消此行的注释

    // Error! `private_nested` is a private module
    //my_mod::private_nested::function();
    // 试一试 ^ 取消此行的注释
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



## `mod` vs `use`

> `use` is different from `mod`. `use` provides for the abbreviation references to contents of the source tree. `mod` establishes the source tree. 



意思是：

* mod才是用来描述/建立依赖关系的，从这个角度它才是那个`import`

* use则主要用作`import as`

  use还可以`import EXACT_SYMBOL as`，比如import某个函数，这是go不支持的





# struct可见性

见 struct文档





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



# 文件分层



> 从下例看来：
>
> * `mod xx;` 等于 `import relatively xx;`
>
>   而xx可以是同级的
>
>   * `xx.rs` file 
>   * `xx` dir，此时查找`xx/mod.rs`
>
> * 似乎，`mod.rs`中的`mod xx;`有特殊含义，既是`import`又是`declare`，比如`pub mod xx;` 和 `mod xx;` 就不同，前者讲`mod xx`声明为`pub`
>
>   普通文件中的则只有`import`语义
>
> TODO y: relative-import不是好东西啊，那要怎么做 absolute-import如`import <root>/xx`呢？ 或者 `import ./xx/yy`



模块可以分配到文件/目录的层次结构中。让我们将《可见性》一节中 的[例子](http://localhost:3000/mod/visibility.html)的代码拆分到多个文件中：

```bash
$ tree .
.
|-- my
|   |-- inaccessible.rs
|   |-- mod.rs
|   `-- nested.rs
`-- split.rs
```

`split.rs` 的内容：

```rust
// 此声明将会查找名为 `my.rs` 或 `my/mod.rs` 的文件，并将该文件的内容放到
// 此作用域中一个名为 `my` 的模块里面。
mod my;

fn function() {
    println!("called `function()`");
}

fn main() {
    my::function();

    function();

    my::indirect_access();

    my::nested::function();
}
```

`my/mod.rs` 的内容：

```rust
// 类似地，`mod inaccessible` 和 `mod nested` 将找到 `nested.rs` 和
// `inaccessible.rs` 文件，并在它们放到各自的模块中。
mod inaccessible;
pub mod nested;

pub fn function() {
    println!("called `my::function()`");
}

fn private_function() {
    println!("called `my::private_function()`");
}

pub fn indirect_access() {
    print!("called `my::indirect_access()`, that\n> ");

    private_function();
}
```

`my/nested.rs` 的内容：

```rust
pub fn function() {
    println!("called `my::nested::function()`");
}

#[allow(dead_code)]
fn private_function() {
    println!("called `my::nested::private_function()`");
}
```

`my/inaccessible.rs` 的内容：

```rust
#[allow(dead_code)]
pub fn public_function() {
    println!("called `my::inaccessible::public_function()`");
}
```

我们看到代码仍然正常运行，就和前面的一样：

```bash
$ rustc split.rs && ./split
called `my::function()`
called `function()`
called `my::indirect_access()`, that
> called `my::private_function()`
called `my::nested::function()`
```





# crate

crate（中文有 “包，包装箱” 之意）是 Rust 的编译单元。当调用 `rustc some_file.rs` 时，`some_file.rs` 被当作 **crate 文件**。如果 `some_file.rs` 里面含有 `mod` 声明，那么模块文件的内容将在编译之前被插入 crate 文件的相应声明处。换句话说，模 块**不会**单独被编译，只有 crate 才会被编译。

crate 可以编译成二进制可执行文件（binary）或库文件（library）。默认情况 下，`rustc` 将从 crate 产生二进制可执行文件。这种行为可以通过 `rustc` 的选项 `--crate-type` 重载。



* `rustc --crate-type=lib rary.rs` 编译lib得到 `library.rlib`

  默认`lib<crate-file>`这么命名，可以`--crate-name`来指定名字

* `rustc executable.rs --extern rary=library.rlib --edition=2018 && ./executable ` 编译执行文件并执行



# 包管理

见 包管理 文档





# use

`use` 声明可以将一个完整的路径绑定到一个新的名字，从而更容易访问。

> 等于 `import as` ？



```rust
// 将 `deeply::nested::function` 路径绑定到 `other_function`。
use deeply::nested::function as other_function;

fn function() {
    println!("called `function()`");
}

mod deeply {
    pub mod nested {
        pub fn function() {
            println!("called `deeply::nested::function()`")
        }
    }
}

fn main() {
    // 更容易访问 `deeply::nested::funcion`
    other_function();

    println!("Entering block");
    {
        // 这和 `use deeply::nested::function as function` 等价。
        // 此 `function()` 将遮蔽外部的同名函数。
        use deeply::nested::function;
        function();

        // `use` 绑定拥有局部作用域。在这个例子中，`function()`
        // 的遮蔽只存在这个代码块中。
        println!("Leaving block");
    }

    function();
}

```











# super/self

可以在路径中使用 `super` （父级）和 `self`（自身）关键字，从而在访问项时消除 歧义，以及防止不必要的路径硬编码。



```rust
fn function() {
    println!("called `function()`");
}

mod cool {
    pub fn function() {
        println!("called `cool::function()`");
    }
}

mod my {
    fn function() {
        println!("called `my::function()`");
    }
    
    mod cool {
        pub fn function() {
            println!("called `my::cool::function()`");
        }
    }
    
    pub fn indirect_call() {
        // 让我们从这个作用域中访问所有名为 `function` 的函数！
        print!("called `my::indirect_call()`, that\n> ");
        
        // `self` 关键字表示当前的模块作用域——在这个例子是 `my`。
        // 调用 `self::function()` 和直接调用 `function()` 都得到相同的结果，
        // 因为他们表示相同的函数。
        self::function();
        function();
        
        // 我们也可以使用 `self` 来访问 `my` 内部的另一个模块：
        self::cool::function();
        
        // `super` 关键字表示父作用域（在 `my` 模块外面）。
        super::function();
        
        // 这将在 *crate* 作用域内绑定 `cool::function` 。
        // 在这个例子中，crate 作用域是最外面的作用域。
        {
            use crate::cool::function as root_function;
            root_function();
        }
    }
}

fn main() {
    my::indirect_call();
}

```













