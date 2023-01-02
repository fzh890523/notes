

* `let`

* `mut`

* `const` 
  define constants.
  It lives for the entire lifetime of a program but has no fixed address in the memory. 
  
  > Always use const, instead of static. It’s pretty rare that you actually want a memory location associated with your constant, and using a const allows for optimizations like constant propagation not only in your crate but also in downstream crates.
* `static`
  define a ‘global variable’ type facility.
  There is only one instance for each value, and it’s at a fixed location in memory.
  
  > Usually, statics are placed at the top of the code file, outside the functions.



变量绑定有一个作用域（scope），它被限定只在一个**代码块**（block）中生存（live）。 代码块是一个被 `{}` 包围的语句集合。另外也允许[变量遮蔽](https://en.wikipedia.org/wiki/Variable_shadowing)（variable shadowing）。



* 默认immu，除非显式`mut`

* 有作用域scope

  取决于声明而不是赋值

* 允许shadow

* 允许声明、初始化（赋值）分离，也即允许先声明后赋值
* 但禁止使用未初始化变量，所以要求随后的赋值语句是一定能执行到的



```rust
fn main() {
    // 此绑定生存于 main 函数中
    let long_lived_binding = 1;

    // 这是一个代码块，比 main 函数拥有更小的作用域
    {
        // 此绑定只存在于本代码块
        let short_lived_binding = 2;

        println!("inner short: {}", short_lived_binding);

        // 此绑定*遮蔽*了外面的绑定
        let long_lived_binding = 5_f32;

        println!("inner long: {}", long_lived_binding);
    }
    // 代码块结束

    // 报错！`short_lived_binding` 在此作用域上不存在
    println!("outer short: {}", short_lived_binding);
    // 改正 ^ 注释掉这行

    println!("outer long: {}", long_lived_binding);

    // 此绑定同样*遮蔽*了前面的绑定
    let long_lived_binding = 'a';

    println!("outer long: {}", long_lived_binding);
}

```



```rust
fn main() {
    // 声明一个变量绑定
    let a_binding;

    {
        let x = 2;

        // 初始化一个绑定
        a_binding = x * x;
    }

    println!("a binding: {}", a_binding);

    let another_binding;

    // 报错！使用了未初始化的绑定
    println!("another binding: {}", another_binding);
    // 改正 ^ 注释掉此行

    another_binding = 1;

    println!("another binding: {}", another_binding);
}
```

