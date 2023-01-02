



# statement



Rust 有多种语句。最普遍的语句类型有两种：一种是声明绑定变量，另一种是表达式带上英文分号(;)：



```rust
fn main() {
    // 变量绑定
    let x = 5;

    // 表达式;
    x;
    x + 1;
    15;
}

```







# expression



代码块也是表达式，所以它们可以用作赋值中的值。代码块中的最后一个表达式将赋给适当的表达式，例如局部变量。但是，如果代码块的最后一个表达式结尾处有分号，则返回值为 `()`（译注：代码块中的最后一个语句是代码块中**实际执行**的最后一个语句，而不一定是代码块中最后一行的语句）。



```rust
fn main() {
    let x = 5u32;

    let y = {
        let x_squared = x * x;
        let x_cube = x_squared * x;

        // 将此表达式赋给 `y`
        x_cube + x_squared + x
    };

    let z = {
        // 分号结束了这个表达式，于是将 `()` 赋给 `z`
        2 * x;
    };

    println!("x is {:?}", x);
    println!("y is {:?}", y);
    println!("z is {:?}", z);
}

```







