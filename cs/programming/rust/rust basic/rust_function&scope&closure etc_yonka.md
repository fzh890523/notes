






* 函数指针（一级对象）
  
  ```rust
  let b: fn(i32) -> i32 = plus_one;
  ```
  
* 闭包（？！）
  Also known as anonymous functions or lambda functions.
  > ??!!

  ```rust
    let square = |x: i32| -> i32 { // Input parameters are passed inside | | and expression body is wrapped within { }
        x * x 
    };

    let square = |x| x * x; // { } are optional for single-lined closures
  ```



### functions

```rust
fn plus_one(a: i32) -> i32 {
    a + 1
}

let b: fn(i32) -> i32 = plus_one;
let c = b(5); //6
```




## return value



### return &str/&i32...

> 这个内容也许放到lifetime部分更合适

1. references are borrowed, i.e. they point to some data but do not own it, it is owned by someone else. 

2. In this particular case the string, a slice to which you want to return, is owned by the function because it is stored in a local variable.

   When the function exits, all its local variables are destroyed; this involves calling destructors, and the destructor of `String` frees the memory used by the string. 

3. Rust was created, among everything else, to prevent such problems. Therefore, in Rust it is impossible to return a reference pointing into local variables of the function, which is possible in languages like C.
