# **特质 trait**

`trait` 是对未知类型 `Self` 定义的方法集。该类型也可以访问同一个 trait 中定义的 其他方法。

对任何数据类型都可以实现 trait。



```rust
struct Sheep { naked: bool, name: &'static str }

trait Animal {
    // 静态方法签名；`Self` 表示实现者类型（implementor type）。
    fn new(name: &'static str) -> Self;  // y: 竟然还能约束要求实现静态方法。 这个new是用来当构造器的？

    // 实例方法签名；这些方法将返回一个字符串。
    fn name(&self) -> &'static str;
    fn noise(&self) -> &'static str;

    // trait 可以提供默认的方法定义。
    fn talk(&self) {
        println!("{} says {}", self.name(), self.noise());
    }
}

impl Sheep {
    fn is_naked(&self) -> bool {
        self.naked
    }

    fn shear(&mut self) {
        if self.is_naked() {
            // 实现者可以使用它的 trait 方法。
            println!("{} is already naked...", self.name());
        } else {
            println!("{} gets a haircut!", self.name);

            self.naked = true;
        }
    }
}

// 对 `Sheep` 实现 `Animal` trait。
impl Animal for Sheep {
    // `Self` 是实现者类型：`Sheep`。
    fn new(name: &'static str) -> Sheep {
        Sheep { name: name, naked: false }
    }

    fn name(&self) -> &'static str {
        self.name
    }

    fn noise(&self) -> &'static str {
        if self.is_naked() {
            "baaaaah?"
        } else {
            "baaaaah!"
        }
    }
    
    // 默认 trait 方法可以重载。
    fn talk(&self) {
        // 例如我们可以增加一些安静的沉思。
        println!("{} pauses briefly... {}", self.name, self.noise());
    }
}

fn main() {
    // 这种情况需要类型标注。
    let mut dolly: Sheep = Animal::new("Dolly");
    // 试一试 ^ 移除类型标注。

    dolly.talk();
    dolly.shear();
    dolly.talk();
}

```



## derive派生

通过 `#[derive]` [属性](http://localhost:3000/attribute.html)，编译器能够提供某些 trait 的基本实现。如果 需要更复杂的行为，这些 trait 也可以手动实现。

下面是可以自动派生的 trait：

- 比较 trait: [`Eq`](https://rustwiki.org/zh-CN/std/cmp/trait.Eq.html), [`PartialEq`](https://rustwiki.org/zh-CN/std/cmp/trait.PartialEq.html), [`Ord`](https://rustwiki.org/zh-CN/std/cmp/trait.Ord.html), [`PartialOrd`](https://rustwiki.org/zh-CN/std/cmp/trait.PartialOrd.html)

  > Eq and PartialEq are traits that allow you to define total and partial equality between values, respectively. Implementing them overloads the == and != operators.

  > 这两个 Traits 的名称实际上来自于抽象代数中的等价关系和局部等价关系，实际上两者的区别仅有一点，即是否在相等比较中是否满足反身性（Reflexivity）。
  >
  > 两者均需要满足的条件有：
  >
  > - 对称性（Symmetry）：`a == b` 可推出 `b == a`
  > - 传递性（Transitivity）：`a == b` 且 `b == c` 可推出 `a == c`
  >
  > Eq 相比 PartialEq 需要额外满足反身性，即 `a == a`，对于浮点类型，[Rust](https://so.csdn.net/so/search?q=Rust&spm=1001.2101.3001.7020) 只实现了 PartialEq 而不是 Eq，原因就是 `NaN != NaN`。
  >
  > PartialEq 可使用 `#[derive]` 来交由编译器实现，这样一个 struct 在进行相等比较时，会对其中每一个字段进行比较，如果遇到枚举，还会对枚举所拥有的数据进行比较。

  > Ord and PartialOrd are traits that allow you to define total and partial orderings between values, respectively. Implementing them overloads the <, <=, >, and >= operators.

  > 类似于 Eq，Ord 指的是 Total Order，需要满足以下三个性质：
  >
  > - 反对称性（Antisymmetry）：`a <= b` 且 `a >= b` 可推出 `a == b`
  > - 传递性（Transitivity）：`a <= b` 且 `b <= c` 可推出 `a <= c`
  > - 连通性（Connexity）：`a <= b` 或 `a >= b`
  >
  > 而 PartialOrd 无需满足连通性，只满足反对称性和传递性即可。
  >
  > - 反对称性：`a < b` 则有 `!(a > b)`，反之亦然
  > - 传递性：`a < b` 且 `b < c` 可推出 `a < c`，`==` 和 `>` 同理
  >
  > Ord & PartialOrd 均可通过 `#[derive]` 交由编译器自动实现，当使用 `#[derive]` 实现后，将会基于 struct 的字段声明以字典序进行比较，遇到枚举中的数据也会以此类推。可以注意到 Ord & PartialOrd 的性质要求会进行等于的比较，所以有以下对 Eq & PartialEq 的依赖要求：
  >
  > - PartialOrd 要求你的类型实现 PartialEq
  > - Ord 要求你的类型实现 PartialOrd 和 Eq（因此 PartialEq 也需要被实现）
  >
  > 实现 PartialEq，PartialOrd 以及 Ord 时要特别注意彼此之间不能有冲突。

- [`Clone`](https://rustwiki.org/zh-CN/std/clone/trait.Clone.html), 用来从 `&T` 创建副本 `T`。

- [`Copy`](https://rustwiki.org/zh-CN/core/marker/trait.Copy.html)，使类型具有 “复制语义”（copy semantics）而非 “移动语义”（move semantics）。

- [`Hash`](https://rustwiki.org/zh-CN/std/hash/trait.Hash.html)，从 `&T` 计算哈希值（hash）。

- [`Default`](https://rustwiki.org/zh-CN/std/default/trait.Default.html), 创建数据类型的一个空实例。

- [`Debug`](https://rustwiki.org/zh-CN/std/fmt/trait.Debug.html)，使用 `{:?}` formatter 来格式化一个值。



```rust
// `Centimeters`，可以比较的元组结构体
#[derive(PartialEq, PartialOrd)]
struct Centimeters(f64);

// `Inches`，可以打印的元组结构体
#[derive(Debug)]
struct Inches(i32);

impl Inches {
    fn to_centimeters(&self) -> Centimeters {
        let &Inches(inches) = self;

        Centimeters(inches as f64 * 2.54)
    }
}

// `Seconds`，不带附加属性的元组结构体
struct Seconds(i32);

fn main() {
    let _one_second = Seconds(1);

    // 报错：`Seconds` 不能打印；它没有实现 `Debug` trait
    //println!("One second looks like: {:?}", _one_second);
    // 试一试 ^ 取消此行注释

    // 报错：`Seconds`不能比较；它没有实现 `PartialEq` trait
    //let _this_is_true = (_one_second == _one_second);
    // 试一试 ^ 取消此行注释
    // y: 这种不言自明的可比较类型竟然还要显式 derive eq

    let foot = Inches(12);

    println!("One foot equals {:?}", foot);

    let meter = Centimeters(100.0);

    let cmp =
        if foot.to_centimeters() < meter {
            "smaller"
        } else {
            "bigger"
        };

    println!("One foot is {} than one meter.", cmp);
}

```



## dyn

Rust 编译器需要知道每个函数的返回类型需要多少空间。这意味着所有函数都必须返回一个具体类型。与其他语言不同，如果你有个像 `Animal` 那样的的 trait，则不能编写返回 `Animal` 的函数，因为其不同的实现将需要不同的内存量。

但是，有一个简单的解决方法。相比于直接返回一个 trait 对象，我们的函数返回一个包含一些 `Animal` 的 `Box`。`box` 只是对堆中某些内存的引用。因为引用的大小是静态已知的，并且编译器可以保证引用指向已分配的堆 `Animal`，所以我们可以从函数中返回 trait！

每当在堆上分配内存时，Rust 都会尝试尽可能明确。因此，如果你的函数以这种方式返回指向堆的 trait 指针，则需要使用 `dyn` 关键字编写返回类型，例如 `Box<dyn Animal>`。



```rust
struct Sheep {}
struct Cow {}

trait Animal {
    // 实例方法签名
    fn noise(&self) -> &'static str;
}

// 实现 `Sheep` 的 `Animal` trait。
impl Animal for Sheep {
    fn noise(&self) -> &'static str {
        "baaaaah!"
    }
}

// 实现 `Cow` 的 `Animal` trait。
impl Animal for Cow {
    fn noise(&self) -> &'static str {
        "moooooo!"
    }
}

fn gen<T: Animal>(a: T) {
    println!("{}", a.noise());
}
// y: 但泛型实现不了random_animal

// 返回一些实现 Animal 的结构体，但是在编译时我们不知道哪个结构体。
fn random_animal(random_number: f64) -> Box<dyn Animal> {
    // y: rust里怎么做type assert？
    if random_number < 0.5 {
        Box::new(Sheep {})
    } else {
        Box::new(Cow {})
    }
}

fn main() {
    let random_number = 0.234;
    let animal = random_animal(random_number);
    println!("You've randomly chosen an animal, and it says {}", animal.noise());
}


```





## 运算符重载



在 Rust 中，很多运算符可以通过 trait 来重载。也就是说，这些运算符可以根据它们的 输入参数来完成不同的任务。这之所以可行，是因为**运算符就是方法调用的语法糖**。例 如，`a + b` 中的 `+` 运算符会调用 `add` 方法（也就是 `a.add(b)`）。这个 `add` 方 法是 `Add` trait 的一部分。因此，`+` 运算符可以被任何 `Add` trait 的实现者使用。



```rust
use std::ops;

struct Foo;
struct Bar;

#[derive(Debug)]
struct FooBar;

#[derive(Debug)]
struct BarFoo;

// `std::ops::Add` trait 用来指明 `+` 的功能，这里我们实现 `Add<Bar>`，它是用于
// 把对象和 `Bar` 类型的右操作数（RHS）加起来的 `trait`。
// 下面的代码块实现了 `Foo + Bar = FooBar` 这样的运算。
impl ops::Add<Bar> for Foo {
    type Output = FooBar;

    fn add(self, _rhs: Bar) -> FooBar {
        println!("> Foo.add(Bar) was called");

        FooBar
    }
}

// 通过颠倒类型，我们实现了不服从交换律的加法。
// 这里我们实现 `Add<Foo>`，它是用于把对象和 `Foo` 类型的右操作数加起来的 trait。
// 下面的代码块实现了 `Bar + Foo = BarFoo` 这样的运算。
impl ops::Add<Foo> for Bar {
    type Output = BarFoo;

    fn add(self, _rhs: Foo) -> BarFoo {
        println!("> Bar.add(Foo) was called");

        BarFoo
    }
}

fn main() {
    println!("Foo + Bar = {:?}", Foo + Bar);
    println!("Bar + Foo = {:?}", Bar + Foo);
}

```



## Drop



[`Drop`](https://rustwiki.org/zh-CN/std/ops/trait.Drop.html) trait 只有一个方法：`drop`，当对象离开作用域时会自动调用该 方法。`Drop` trait 的主要作用是释放实现者的实例拥有的资源。

`Box`，`Vec`，`String`，`File`，以及 `Process` 是一些实现了 `Drop` trait 来释放 资源的类型。`Drop` trait 也可以为任何自定义数据类型手动实现。



```rust
struct Droppable {
    name: &'static str,
}

// 这个简单的 `drop` 实现添加了打印到控制台的功能。
impl Drop for Droppable {
    fn drop(&mut self) {
        println!("> Dropping {}", self.name);
    }
}

fn main() {
    let _a = Droppable { name: "a" };

    // 代码块 A
    {
        let _b = Droppable { name: "b" };

        // 代码块 B
        {
            let _c = Droppable { name: "c" };
            let _d = Droppable { name: "d" };

            println!("Exiting block B");
        }
        println!("Just exited block B");

        println!("Exiting block A");
    }
    println!("Just exited block A");

    // 变量可以手动使用 `drop` 函数来销毁。
    drop(_a);
    // 试一试 ^ 将此行注释掉。

    println!("end of the main function");

    // `_a` *不会*在这里再次销毁，因为它已经被（手动）销毁。
}

```



## iterator

`Iterator` trait 用来对集合（collection）类型（比如数组）实现迭代器。

这个 trait 只需定义一个返回 `next`（下一个）元素的方法，这可手动在 `impl` 代码块 中定义，或者自动定义（比如在数组或区间中）。

为方便起见，`for` 结构会使用 [`.into_iter()`](https://rustwiki.org/zh-CN/std/iter/trait.IntoIterator.html) 方法将一些集合类型 转换为迭代器。

[more](https://rustwiki.org/zh-CN/core/iter/trait.Iterator.html)



```rust
struct Fibonacci {
    curr: u32,
    next: u32,
}

// 为 `Fibonacci`（斐波那契）实现 `Iterator`。
// `Iterator` trait 只需定义一个能返回 `next`（下一个）元素的方法。
impl Iterator for Fibonacci {
    type Item = u32;
    
    // 我们在这里使用 `.curr` 和 `.next` 来定义数列（sequence）。
    // 返回类型为 `Option<T>`：
    //     * 当 `Iterator` 结束时，返回 `None`。
    //     * 其他情况，返回被 `Some` 包裹（wrap）的下一个值。
    fn next(&mut self) -> Option<u32> {
        let new_next = self.curr + self.next;

        self.curr = self.next;
        self.next = new_next;

        // 既然斐波那契数列不存在终点，那么 `Iterator` 将不可能
        // 返回 `None`，而总是返回 `Some`。
        Some(self.curr)
    }
}

// 返回一个斐波那契数列生成器
fn fibonacci() -> Fibonacci {
    Fibonacci { curr: 1, next: 1 }
}

fn main() {
    // `0..3` 是一个 `Iterator`，会产生：0、1 和 2。
    let mut sequence = 0..3;

    println!("Four consecutive `next` calls on 0..3");
    println!("> {:?}", sequence.next());
    println!("> {:?}", sequence.next());
    println!("> {:?}", sequence.next());
    println!("> {:?}", sequence.next());

    // `for` 遍历 `Iterator` 直到返回 `None`，
    // 并且每个 `Some` 值都被解包（unwrap），然后绑定给一个变量（这里是 `i`）。       
    println!("Iterate through 0..3 using `for`");
    for i in 0..3 {
        println!("> {}", i);
    }

    // `take(n)` 方法提取 `Iterator` 的前 `n` 项。
    println!("The first four terms of the Fibonacci sequence are: ");
    for i in fibonacci().take(4) {
        println!("> {}", i);
    }

    // `skip(n)` 方法移除前 `n` 项，从而缩短了 `Iterator` 。
    println!("The next four terms of the Fibonacci sequence are: ");
    for i in fibonacci().skip(4).take(4) {
        println!("> {}", i);
    }

    let array = [1u32, 3, 3, 7];

    // `iter` 方法对数组/slice 产生一个 `Iterator`。
    println!("Iterate the following array {:?}", &array);
    for i in array.iter() {
        println!("> {}", i);
    }
}

```



## impl Trait



如果函数返回实现了 `MyTrait` 的类型，可以将其返回类型编写为 `-> impl MyTrait`。这可以大大简化你的类型签名！

```rust
use std::iter;
use std::vec::IntoIter;

// 该函数组合了两个 `Vec <i32>` 并在其上返回一个迭代器。
// 看看它的返回类型多么复杂！
fn combine_vecs_explicit_return_type(
    v: Vec<i32>,
    u: Vec<i32>,
) -> iter::Cycle<iter::Chain<IntoIter<i32>, IntoIter<i32>>> {
    v.into_iter().chain(u.into_iter()).cycle()
}

// 这是完全相同的函数，但其返回类型使用 `impl Trait`。
// 看看它多么简单！
fn combine_vecs(
    v: Vec<i32>,
    u: Vec<i32>,
) -> impl Iterator<Item=i32> {  // TODO y: 这会儿不提返回值类型要编译期确定size？
    // y: 还是说这只是个语法糖实际返回值类型还是...？ 从编译器提示来看并不是... 已经无法访问原来类型的信息（如其他方法）
    v.into_iter().chain(u.into_iter()).cycle()
}

fn main() {
    let v1 = vec![1, 2, 3];
    let v2 = vec![4, 5];
    let mut v3 = combine_vecs(v1, v2);
    assert_eq!(Some(1), v3.next());
    assert_eq!(Some(2), v3.next());
    assert_eq!(Some(3), v3.next());
    assert_eq!(Some(4), v3.next());
    assert_eq!(Some(5), v3.next());
    println!("all done");
}
```

更重要的是，某些 Rust 类型无法写出。例如，每个闭包都有自己未命名的具体类型。在使用 `impl Trait` 语法之前，必须在堆上进行分配才能返回闭包。但是现在你可以像下面这样静态地完成所有操作：

```rust
// 返回一个将输入和 `y` 相加的函数
fn make_adder_function(y: i32) -> impl Fn(i32) -> i32 {
    let closure = move |x: i32| { x + y };
    closure
}

fn main() {
    let plus_one = make_adder_function(1);
    assert_eq!(plus_one(2), 3);
}
```

您还可以使用 `impl Trait` 返回使用 `map` 或 `filter` 闭包的迭代器！这使得使用 `map` 和 `filter` 更容易。因为闭包类型没有名称，所以如果函数返回带闭包的迭代器，则无法写出显式的返回类型。但是有了 `impl Trait`，你就可以轻松地做到这一点：

```rust
fn double_positives<'a>(numbers: &'a Vec<i32>) -> impl Iterator<Item = i32> + 'a {
    numbers
        .iter()
        .filter(|x| x > &&0)
        .map(|x| x * 2)
}

```



## clone

当处理资源时，默认的行为是在赋值或函数调用的同时将它们转移。但是我们有时候也需要 把资源复制一份。

[`Clone`](https://rustwiki.org/zh-CN/std/clone/trait.Clone.html) trait 正好帮助我们完成这任务。通常，我们可以使用由 `Clone` trait 定义的 `.clone()` 方法。



```rust
// 不含资源的单元结构体
#[derive(Debug, Clone, Copy)]
struct Nil;

// 一个包含资源的结构体，它实现了 `Clone` trait
#[derive(Clone, Debug)]
struct Pair(Box<i32>, Box<i32>);

fn main() {
    // 实例化 `Nil`
    let nil = Nil;
    // 复制 `Nil`，没有资源用于移动（move）
    let copied_nil = nil;

    // 两个 `Nil` 都可以独立使用
    println!("original: {:?}", nil);
    println!("copy: {:?}", copied_nil);

    // 实例化 `Pair`
    let pair = Pair(Box::new(1), Box::new(2));
    println!("original: {:?}", pair);

    // 将 `pair` 绑定到 `moved_pair`，移动（move）了资源
    let moved_pair = pair;
    println!("copy: {:?}", moved_pair);

    // 报错！`pair` 已失去了它的资源。
    //println!("original: {:?}", pair);
    // 试一试 ^ 取消此行注释。

    // 将 `moved_pair`（包括其资源）克隆到 `cloned_pair`。
    let cloned_pair = moved_pair.clone();
    // 使用 std::mem::drop 来销毁原始的 pair。
    drop(moved_pair);

    // 报错！`moved_pair` 已被销毁。
    //println!("copy: {:?}", moved_pair);
    // 试一试 ^ 将此行注释掉。

    // 由 .clone() 得来的结果仍然可用！
    println!("clone: {:?}", cloned_pair);
}

```



## 父trait

**Rust 没有“继承”**，但是您可以将一个 trait 定义为另一个 trait 的超集（即父 trait）。例如：

> y: 差不多就是 “只有接口的继承” 了

```rust
trait Person {
    fn name(&self) -> String;
}

// Person 是 Student 的父 trait。
// 实现 Student 需要你也 impl 了 Person。
trait Student: Person {
    fn university(&self) -> String;
}

trait Programmer {
    fn fav_language(&self) -> String;
}

// CompSciStudent (computer science student，计算机科学的学生) 是 Programmer 和 Student 两者的子类。
// 实现 CompSciStudent 需要你同时 impl 了两个父 trait。
trait CompSciStudent: Programmer + Student {
    fn git_username(&self) -> String;
}
// y: “接口的多继承”

fn comp_sci_student_greeting(student: &dyn CompSciStudent) -> String {
    format!(
        "My name is {} and I attend {}. My favorite language is {}. My Git username is {}",
        student.name(),
        student.university(),
        student.fav_language(),
        student.git_username()
    )
}

fn main() {}
```



## 消除重叠 trait



一个类型可以实现许多不同的 trait。如果两个 trait 都需要相同的名称怎么办？例如，许多 trait 可能拥有名为 `get()` 的方法。他们甚至可能有不同的返回类型！

有个好消息：**由于每个 trait 实现都有自己的 `impl` 块，因此很清楚您要实现哪个 trait 的 `get` 方法**。

> y: sweet!

何时需要**调用**这些方法呢？为了消除它们之间的歧义，我们必须使用完全限定语法（Fully Qualified Syntax）。

> `<Form as UsernameWidget>::get(&form);` 说真的，有点丑...
>
> `<type as trait>::method(inst)`

```rust
trait UsernameWidget {
    // 从这个 widget 中获取选定的用户名
    fn get(&self) -> String;
}

trait AgeWidget {
    // 从这个 widget 中获取选定的年龄
    fn get(&self) -> u8;
}

// 同时具有 UsernameWidget 和 AgeWidget 的表单
struct Form {
    username: String,
    age: u8,
}

impl UsernameWidget for Form {
    fn get(&self) -> String {
        self.username.clone()
    }
}

impl AgeWidget for Form {
    fn get(&self) -> u8 {
        self.age
    }
}

fn main() {
    let form = Form{
        username: "rustacean".to_owned(),
        age: 28,
    };

    // 如果取消注释此行，则会收到一条错误消息，提示 “multiple `get` found”（找到了多个`get`）。
    // 因为毕竟有多个名为 `get` 的方法。
    // println!("{}", form.get());

    let username = <Form as UsernameWidget>::get(&form);
    assert_eq!("rustacean".to_owned(), username);
    let age = <Form as AgeWidget>::get(&form);
    assert_eq!(28, age);
}

```





# items




* 数据（struct） 和 方法（impl） 分离
  
  * struct -类比-> class （但没方法）
  * impl -类比-> method；
    ```rust
    impl s${structName} {
        fn ${methodName}
  }
  
    // or 
    impl ${traitName} for ${structName} {
        // ...
    }
    ```
  * method `self`
    类似py等，需要显式的第一个参数作为self（这点不如go清晰）。
    可以是：
    * `self` 转移ownership
    * `&self` 引用
    * `&mut self` 可变引用
  * 关联函数（associated function） - 类似 静态方法
  > 好像第一个参数不是`self`即是
  
    访问起来，不是通过`${instance}.${method}`而是`${struct}::${function}`
  
* trait -类比-> interface
  可以有默认方法实现，可以被override

  * 泛型trait
    ```rust
    trait From<T> {
        fn from(T) -> Self;
    }
        impl From<u8> for u16 {
            //...
        }
        impl From<u8> for u32{
            //...
        }
    
    // Should specify after the trait name like generic functions    
    ```
  * trait继承
    多继承的语法比较有意思： `+`

    ```rust
    trait Person {
        fn full_name(&self) -> String;
    }

    trait Employee : Person { // Employee inherits from person trait
      fn job_title(&self) -> String;
    }

    trait ExpatEmployee : Employee + Expat { // ExpatEmployee inherits from Employee and Expat traits
      fn additional_tax(&self) -> f64;
    }    
    ```

**不带trait的impl**

也即简单的给stuct添加方法，组成完整的“class”

```rust
struct Player {
    first_name: String,
    last_name: String,
}

impl Player {
    fn full_name(&self) -> String {
        format!("{} {}", self.first_name, self.last_name)
    }
}

fn main() {
    let player_1 = Player {
        first_name: "Rafael".to_string(),
        last_name: "Nadal".to_string(),
    };

    println!("Player 01: {}", player_1.full_name());
}

// ⭐️ Implementation must appear in the same crate as the self type

// 💡 And also in Rust, new traits can be implemented for existing types even for types like i8, f64 and etc.
// Same way existing traits can be implemented for new types you are creating.
// But we can not implement existing traits into existing types.
```

**带trait的impl**

```rust
struct Player {
    first_name: String,
    last_name: String,
}

trait FullName {
    fn full_name(&self) -> String;
}

impl FullName for Player {
    fn full_name(&self) -> String {
        format!("{} {}", self.first_name, self.last_name)
    }
}

fn main() {
    let player_2 = Player {
        first_name: "Roger".to_string(),
        last_name: "Federer".to_string(),
    };

    println!("Player 02: {}", player_2.full_name());
}

// 🔎 Other than functions, traits can contain constants and types.
```

**default method示例**

```rust
trait Foo {
    fn bar(&self);
    fn baz(&self) { println!("We called baz."); }
}
```