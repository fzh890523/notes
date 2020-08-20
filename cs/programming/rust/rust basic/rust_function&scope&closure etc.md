
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