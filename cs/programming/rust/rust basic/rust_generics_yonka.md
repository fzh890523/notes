



## **泛型函数**

调用泛型函数有时需要显式地指明类型参量。这可能是因为调用了返回类型是泛型的 函数，或者编译器没有足够的信息来推断类型参数。

调用函数时，使用显式指定的类型参数会像是这样：`fun::<A, B, ...>()`。





```rust
fn takes_anything<T>(x: T) { // x has type T, T is a generic type
}

fn takes_two_of_the_same_things<T>(x: T, y: T) { // Both x and y has the same type
}

fn takes_two_things<T, U>(x: T, y: U) { // Multiple types
}
```



```rust
struct A;          // 具体类型 `A`。
struct S(A);       // 具体类型 `S`。
struct SGen<T>(T); // 泛型类型 `SGen`。

// 下面全部函数都得到了变量的所有权，并立即使之离开作用域，将变量释放。

// 定义一个函数 `reg_fn`，接受一个 `S` 类型的参数 `_s`。
// 因为没有 `<T>` 这样的泛型类型参数，所以这不是泛型函数。
fn reg_fn(_s: S) {}

// 定义一个函数 `gen_spec_t`，接受一个 `SGen<A>` 类型的参数 `_s`。
// `SGen<>` 显式地接受了类型参数 `A`，且在 `gen_spec_t` 中，`A` 没有被用作
// 泛型类型参数，所以函数不是泛型的。
fn gen_spec_t(_s: SGen<A>) {}

// 定义一个函数 `gen_spec_i32`，接受一个 `SGen<i32>` 类型的参数 `_s`。
// `SGen<>` 显式地接受了类型参量 `i32`，而 `i32` 是一个具体类型。
// 由于 `i32` 不是一个泛型类型，所以这个函数也不是泛型的。
fn gen_spec_i32(_s: SGen<i32>) {}

// 定义一个函数 `generic`，接受一个 `SGen<T>` 类型的参数 `_s`。
// 因为 `SGen<T>` 之前有 `<T>`，所以这个函数是关于 `T` 的泛型函数。
fn generic<T>(_s: SGen<T>) {}

fn main() {
    // 使用非泛型函数
    reg_fn(S(A));          // 具体类型。
    gen_spec_t(SGen(A));   // 隐式地指定类型参数 `A`。
    gen_spec_i32(SGen(6)); // 隐式地指定类型参数 `i32`。

    // 为 `generic()` 显式地指定类型参数 `char`。
    generic::<char>(SGen('a'));

    // 为 `generic()` 隐式地指定类型参数 `char`。
    generic(SGen('c'));
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







```rust
// 一个具体类型 `A`。
struct A;

// 在定义类型 `Single` 时，第一次使用类型 `A` 之前没有写 `<A>`。
// 因此，`Single` 是个具体类型，`A` 取上面的定义。
struct Single(A);
//            ^ 这里是 `Single` 对类型 `A` 的第一次使用。

// 此处 `<T>` 在第一次使用 `T` 前出现，所以 `SingleGen` 是一个泛型类型。
// 因为 `T` 是泛型的，所以它可以是任何类型，包括在上面定义的具体类型 `A`。
struct SingleGen<T>(T);

fn main() {
    // `Single` 是具体类型，并且显式地使用类型 `A`。
    let _s = Single(A);
    
    // 创建一个 `SingleGen<char>` 类型的变量 `_char`，并令其值为 `SingleGen('a')`
    // 这里的 `SingleGen` 的类型参数是显式指定的。
    let _char: SingleGen<char> = SingleGen('a');

    // `SingleGen` 的类型参数也可以隐式地指定。
    let _t    = SingleGen(A); // 使用在上面定义的 `A`。
    let _i32  = SingleGen(6); // 使用 `i32` 类型。
    let _char = SingleGen('a'); // 使用 `char`。
}

```



## 泛型impl



```rust
#![allow(unused)]
fn main() {
struct S; // 具体类型 `S`
struct GenericVal<T>(T,); // 泛型类型 `GenericVal`

// GenericVal 的 `impl`，此处我们显式地指定了类型参数：
impl GenericVal<f32> {} // 指定 `f32` 类型
impl GenericVal<S> {} // 指定为上面定义的 `S`

// `<T>` 必须在类型之前写出来，以使类型 `T` 代表泛型。
impl <T> GenericVal<T> {}
}

```



```rust
struct Val {
    val: f64
}

struct GenVal<T>{
    gen_val: T
}

// Val 的 `impl`
impl Val {
    fn value(&self) -> &f64 { &self.val }
}

// GenVal 的 `impl`，指定 `T` 是泛型类型
impl <T> GenVal<T> {
    fn value(&self) -> &T { &self.gen_val }
}

fn main() {
    let x = Val { val: 3.0 };
    let y = GenVal { gen_val: 3i32 };
    
    println!("{}, {}", x.value(), y.value());
}

```



## 泛型trait

当然 `trait` 也可以是泛型的。我们在这里定义了一个 `trait`，它把 `Drop` `trait` 作为泛型方法实现了，可以 `drop`（丢弃）调用者本身和一个输入参数。

```rust
// 不可复制的类型。
struct Empty;
struct Null;

// `T` 的泛型 trait。
trait DoubleDrop<T> {
    // 定义一个调用者的方法，接受一个额外的参数 `T`，但不对它做任何事。
    fn double_drop(self, _: T);
}

// 对泛型的调用者类型 `U` 和任何泛型类型 `T` 实现 `DoubleDrop<T>` 。
impl<T, U> DoubleDrop<T> for U {  // y: 有点厉害，还能这样...
    // 此方法获得两个传入参数的所有权，并释放它们。
    fn double_drop(self, _: T) {}
}
// TODO y: 有什么限制吗？ 影响哪些范围内的类型？ 否则很容易冲突啊

fn main() {
    let empty = Empty;
    let null  = Null;

    // 释放 `empty` 和 `null`。
    empty.double_drop(null);

    //empty;
    //null;
    // ^ 试一试：去掉这两行的注释。
}
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



## 泛型约束



在使用泛型时，类型参数常常必须使用 trait 作为**约束**（bound）来明确规定 类型应实现哪些功能。例如下面的例子用到了 `Display` trait 来打印，所以它用 `Display` 来约束 `T`，也就是说 `T` **必须**实现 `Display`。

```rust
// 定义一个函数 `printer`，接受一个类型为泛型 `T` 的参数，
// 其中 `T` 必须实现 `Display` trait。
fn printer<T: Display>(t: T) {
    println!("{}", t);
}
```

约束把泛型类型限制为符合约束的类型。请看：

```rust
struct S<T: Display>(T);

// 报错！`Vec<T>` 未实现 `Display`。此次泛型具体化失败。
let s = S(vec![1]);
```

约束的另一个作用是泛型的实例可以访问作为约束的 trait 的方法。例如：

```rust
// 这个 trait 用来实现打印标记：`{:?}`。
use std::fmt::Debug;

trait HasArea {
    fn area(&self) -> f64;
}

impl HasArea for Rectangle {
    fn area(&self) -> f64 { self.length * self.height }
}

#[derive(Debug)]
struct Rectangle { length: f64, height: f64 }
#[allow(dead_code)]
struct Triangle  { length: f64, height: f64 }

// 泛型 `T` 必须实现 `Debug` 。只要满足这点，无论什么类型
// 都可以让下面函数正常工作。
fn print_debug<T: Debug>(t: &T) {
    println!("{:?}", t);
}

// `T` 必须实现 `HasArea`。任意符合该约束的泛型的实例
// 都可访问 `HasArea` 的 `area` 函数
fn area<T: HasArea>(t: &T) -> f64 { t.area() }

fn main() {
    let rectangle = Rectangle { length: 3.0, height: 4.0 };
    let _triangle = Triangle  { length: 3.0, height: 4.0 };

    print_debug(&rectangle);
    println!("Area: {}", area(&rectangle));

    //print_debug(&_triangle);
    //println!("Area: {}", area(&_triangle));
    // ^ 试一试：取消上述语句的注释。
    // | 报错：未实现 `Debug` 或 `HasArea`。
}
```

多说一句，某些情况下也可使用 [`where`](http://localhost:3000/generics/where.html) 分句来形成约束，这拥有更好的表现力。



### 空约束

约束的工作机制会产生这样的效果：即使一个 `trait` 不包含任何功能，你仍然可以用它 作为约束。标准库中的 `Eq` 和 `Ord` 就是这样的 `trait`。

> 不是duck-typing，要显式“impl”

```rust
struct Cardinal;
struct BlueJay;
struct Turkey;

trait Red {}
trait Blue {}

impl Red for Cardinal {}
impl Blue for BlueJay {}

// 这些函数只对实现了相应的 trait 的类型有效。
// 事实上这些 trait 内部是空的，但这没有关系。
fn red<T: Red>(_: &T)   -> &'static str { "red" }
fn blue<T: Blue>(_: &T) -> &'static str { "blue" }

fn main() {
    let cardinal = Cardinal;
    let blue_jay = BlueJay;
    let _turkey   = Turkey;

    // 由于约束，`red()` 不能作用于 blue_jay （蓝松鸟），反过来也一样。
    println!("A cardinal is {}", red(&cardinal));
    println!("A blue jay is {}", blue(&blue_jay));
    //println!("A turkey is {}", red(&_turkey));
    // ^ 试一试：去掉此行注释。
}
```







### 多重约束

多重约束（multiple bounds）可以用 `+` 连接。和平常一样，类型之间使用 `,` 隔开。

```rust
use std::fmt::{Debug, Display};

fn compare_prints<T: Debug + Display>(t: &T) {
    println!("Debug: `{:?}`", t);
    println!("Display: `{}`", t);
}

fn compare_types<T: Debug, U: Debug>(t: &T, u: &U) {
    println!("t: `{:?}", t);
    println!("u: `{:?}", u);
}

fn main() {
    let string = "words";
    let array = [1, 2, 3];
    let vec = vec![1, 2, 3];

    compare_prints(&string);
    //compare_prints(&array);
    // 试一试 ^ 将此行注释去掉。

    compare_types(&array, &vec);
}
```



### where子句

> vs ...



约束也可以使用 `where` 分句来表达，它放在 `{` 的前面，而不需写在类型第一次出现 之前。另外 `where` 从句可以用于任意类型的限定，而不局限于类型参数本身。

`where` 在下面一些情况下很有用：

- 当分别指定泛型的类型和约束会**更清晰**时：

```rust
impl <A: TraitB + TraitC, D: TraitE + TraitF> MyTrait<A, D> for YourType {}

// 使用 `where` 从句来表达约束
impl <A, D> MyTrait<A, D> for YourType where
    A: TraitB + TraitC,
    D: TraitE + TraitF {}
```

- 当使用 `where` 从句比正常语法**更有表现力**时。本例中的 `impl` 如果不用 `where` 从句，就无法直接表达。

```rust
use std::fmt::Debug;

trait PrintInOption {
    fn print_in_option(self);
}

// 这里需要一个 `where` 从句，否则就要表达成 `T: Debug`（这样意思就变了），
// 或者改用另一种间接的方法。
impl<T> PrintInOption for T where
    Option<T>: Debug {
    // 我们要将 `Option<T>: Debug` 作为约束，因为那是要打印的内容。
    // 否则我们会给出错误的约束。
    fn print_in_option(self) {
        println!("{:?}", Some(self));
    }
}

fn main() {
    let vec = vec![1, 2, 3];

    vec.print_in_option();
}
```



## newtype

`newtype` 惯用法（译注：即为不同种类的数据分别定义新的类型）能保证在编译时，提供 给程序的都是正确的类型。

```rust
struct Years(i64);

struct Days(i64);

```

> y: 不知道把这个加入泛型下面是什么意思



## 关联项/associated item

“关联项”（associated item）指与多种类型的[项](https://rustwiki.org/zh-CN/reference/items.html)有关的一组规则。它是 `trait` 泛型的扩展，允许在 `trait` 内部定义新的项。

一个这样的项就叫做一个关联类型。当 `trait` 对于实现了它的容器类型是泛型的，关联 项就提供了简单的使用方法。

> 译注：“关联项”这个说法实际上只在 RFC 里出现了，官方的《The Rust Programming Language》第一版和第二版都只有“关联类型”的说法。如果觉得这里的说法很别扭的话 不要理会就是了。TRPL 对关联类型的定义是：“一种将类型占位符与 trait 联系起来的 做法，这样 trait 中的方法签名中就可以使用这些占位符类型。trait 的实现会指定在 该实现中那些占位符对应什么具体类型。”等看完这一节再回头看这个定义就很明白了。



> y: 倒是给个样例啊。 噢，后面有



https://doc.rust-lang.org/beta/reference/items/associated-items.html



* associated functions： method

  包含static method 和 instance method

  > rust官方似乎只把后者叫做method，前者就只叫 associated func？

* associated types：  are [type aliases](https://doc.rust-lang.org/beta/reference/items/type-aliases.html) associated with another type. 

  一些trait需要“关联”一些类型信息

  > 从下面的例子理解，有点像partial-func，或者可以叫 partial-type

  ```rust
  struct ArrayLender<'a, T>(&'a mut [T; 16]);
  
  trait Lend {
      // Generic associated type declaration
      type Lender<'a> where Self: 'a;
      fn lend<'a>(&'a mut self) -> Self::Lender<'a>;
  }
  
  impl<T> Lend for [T; 16] {
      // Generic associated type definition
      type Lender<'a> = ArrayLender<'a, T> where Self: 'a;
  
      fn lend<'a>(&'a mut self) -> Self::Lender<'a> {
          ArrayLender(self)
      }
  }
  
  fn borrow<'a, T: Lend>(array: &'a mut T) -> <T as Lend>::Lender<'a> {
      array.lend()
  }
  
  
  fn main() {
      let mut array = [0usize; 16];
      let lender = borrow(&mut array);
  }
  
  ```

  ```rust
  trait Container {
      type E;
      fn empty() -> Self;
      fn insert(&mut self, elem: Self::E);
  }
  
  impl<T> Container for Vec<T> {
      type E = T;
      fn empty() -> Vec<T> { Vec::new() }
      fn insert(&mut self, x: T) { self.push(x); }
  }
  ```







### vs 泛型方式

`trait` 如果对实现了它的容器类型是泛型的，则须遵守类型规范要求——`trait` 的 使用者**必须**指出 `trait` 的全部泛型类型。

在下面例子中，`Contains` `trait` 允许使用泛型类型 `A` 和 `B`。然后我们为 `Container` 类型实现了这个 trait，将 `A` 和 `B` 指定为 `i32`，这样就可以对 它们使用 `difference()` 函数。

因为 `Contains` 是泛型的，我们必须在 `fn difference()` 中显式地指出**所有的**泛型 类型。但实际上，我们想要表达，`A` 和 `B` 究竟是什么类型是由输入 `C` 决定的。在 下一节会看到，关联类型恰好提供了这样的功能。





泛型写法

```rust
struct Container(i32, i32);

// 这个 trait 检查给定的 2 个项是否储存于容器中
// 并且能够获得容器的第一个或最后一个值。
trait Contains<A, B> {
    fn contains(&self, _: &A, _: &B) -> bool; // 显式地要求 `A` 和 `B`
    fn first(&self) -> i32; // 未显式地要求 `A` 或 `B`
    fn last(&self) -> i32;  // 未显式地要求 `A` 或 `B`
}

impl Contains<i32, i32> for Container {
    // 如果存储的数字和给定的相等则为真。
    fn contains(&self, number_1: &i32, number_2: &i32) -> bool {
        (&self.0 == number_1) && (&self.1 == number_2)
    }

    // 得到第一个数字。
    fn first(&self) -> i32 { self.0 }

    // 得到最后一个数字。
    fn last(&self) -> i32 { self.1 }
}

// 容器 `C` 就包含了 `A` 和 `B` 类型。鉴于此，必须指出 `A` 和 `B` 显得很麻烦。
fn difference<A, B, C>(container: &C) -> i32 where
    C: Contains<A, B> {
    container.last() - container.first()
}

fn main() {
    let number_1 = 3;
    let number_2 = 10;

    let container = Container(number_1, number_2);

    println!("Does container contain {} and {}: {}",
        &number_1, &number_2,
        container.contains(&number_1, &number_2));
    println!("First number: {}", container.first());
    println!("Last number: {}", container.last());

    println!("The difference is: {}", difference(&container));
}

```



关联类型写法

```rust
struct Container(i32, i32);

// 这个 trait 检查给定的 2 个项是否储存于容器中
// 并且能够获得容器的第一个或最后一个值。
trait Contains {
    // 在这里定义可以被方法使用的泛型类型。
    type A;
    type B;

    fn contains(&self, _: &Self::A, _: &Self::B) -> bool;
    fn first(&self) -> i32;
    fn last(&self) -> i32;
}

impl Contains for Container {
    // 指出 `A` 和 `B` 是什么类型。如果 `input`（输入）类型
    // 为 `Container(i32, i32)`，那么 `output`（输出）类型
    // 会被确定为 `i32` 和 `i32`。
    type A = i32;
    type B = i32;

    // `&Self::A` 和 `&Self::B` 在这里也是合法的类型。
    fn contains(&self, number_1: &i32, number_2: &i32) -> bool {
        (&self.0 == number_1) && (&self.1 == number_2)
    }

    // 得到第一个数字。
    fn first(&self) -> i32 { self.0 }

    // 得到最后一个数字。
    fn last(&self) -> i32 { self.1 }
}

fn difference<C: Contains>(container: &C) -> i32 {
    container.last() - container.first()
}

fn main() {
    let number_1 = 3;
    let number_2 = 10;

    let container = Container(number_1, number_2);

    println!("Does container contain {} and {}: {}",
        &number_1, &number_2,
        container.contains(&number_1, &number_2));
    println!("First number: {}", container.first());
    println!("Last number: {}", container.last());
    
    println!("The difference is: {}", difference(&container));
}

```



## 虚类型参数（phantom type）

虚类型（phantom type）参数是一种在运行时不出现，而在（且仅在）编译时进行静态检查 的类型参数。

可以用额外的泛型类型参数指定数据类型，该类型可以充当标记，也可以供编译时类型检查 使用。这些额外的参数没有存储值，也没有运行时行为。



```rust
use std::marker::PhantomData;

// 这个虚元组结构体对 `A` 是泛型的，并且带有隐藏参数 `B`。
#[derive(PartialEq)] // 允许这种类型进行相等测试（equality test）。
struct PhantomTuple<A, B>(A,PhantomData<B>);

// 这个虚类型结构体对 `A` 是泛型的，并且带有隐藏参数 `B`。
#[derive(PartialEq)] // 允许这种类型进行相等测试。
struct PhantomStruct<A, B> { first: A, phantom: PhantomData<B> }

// 注意：对于泛型 `A` 会分配存储空间，但 `B` 不会。
//       因此，`B` 不能参与运算。

fn main() {
    // 这里的 `f32` 和 `f64` 是隐藏参数。
    // 被指定为 `<char, f32>` 的 `PhantomTuple` 类型。
    let _tuple1: PhantomTuple<char, f32> = PhantomTuple('Q', PhantomData);
    // 被指定为 `<char, f64>` `PhantomTuple` 类型。
    let _tuple2: PhantomTuple<char, f64> = PhantomTuple('Q', PhantomData);

    // 被指定为 `<char, f32>` 的类型。
    let _struct1: PhantomStruct<char, f32> = PhantomStruct {
        first: 'Q',
        phantom: PhantomData,
    };
    // 被指定为 `<char, f64>` 的类型。
    let _struct2: PhantomStruct<char, f64> = PhantomStruct {
        first: 'Q',
        phantom: PhantomData,
    };
    
    // 编译期错误！类型不匹配，所以这些值不能够比较：
    //println!("_tuple1 == _tuple2 yields: {}",
    //          _tuple1 == _tuple2);
    
    // 编译期错误！类型不匹配，所以这些值不能够比较：
    //println!("_struct1 == _struct2 yields: {}",
    //          _struct1 == _struct2);
}

```

> y: 没看明白在干嘛。。 有什么用？



```rust
use std::ops::Add;
use std::marker::PhantomData;

/// 创建空枚举类型来表示单位。
#[derive(Debug, Clone, Copy)]
enum Inch {}
#[derive(Debug, Clone, Copy)]
enum Mm {}

/// `Length` 是一个带有虚类型参数 `Unit` 的类型，
/// 而且对于表示长度的类型（即 `f64`）而言，`Length` 不是泛型的。
///
/// `f64` 已经实现了 `Clone` 和 `Copy` trait.
#[derive(Debug, Clone, Copy)]
struct Length<Unit>(f64, PhantomData<Unit>);

/// `Add` trait 定义了 `+` 运算符的行为。
impl<Unit> Add for Length<Unit> {
     type Output = Length<Unit>;

    // add() 返回一个含有和的新的 `Length` 结构体。
    fn add(self, rhs: Length<Unit>) -> Length<Unit> {
        // `+` 调用了针对 `f64` 类型的 `Add` 实现。
        Length(self.0 + rhs.0, PhantomData)
    }
}

fn main() {
    // 指定 `one_foot` 拥有虚类型参数 `Inch`。
    let one_foot:  Length<Inch> = Length(12.0, PhantomData);
    // `one_meter` 拥有虚类型参数 `Mm`。
    let one_meter: Length<Mm>   = Length(1000.0, PhantomData);

    // `+` 调用了我们对 `Length<Unit>` 实现的 `add()` 方法。
    //
    // 由于 `Length` 了实现了 `Copy`，`add()` 不会消耗 `one_foot`
    // 和 `one_meter`，而是复制它们作为 `self` 和 `rhs`。
    let two_feet = one_foot + one_foot;
    let two_meters = one_meter + one_meter;

    // 加法正常执行。
    println!("one foot + one_foot = {:?} in", two_feet.0);
    println!("one meter + one_meter = {:?} mm", two_meters.0);

    // 无意义的运算当然会失败：
    // 编译期错误：类型不匹配。
    //let one_feter = one_foot + one_meter;
}


```

> y: 噢这个例子就清楚一些了。  高玩。。。。





