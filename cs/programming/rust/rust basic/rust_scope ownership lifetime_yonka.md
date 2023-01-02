

# RAII



## 析构： Drop

Rust 中的析构函数概念是通过 [`Drop`](https://rustwiki.org/zh-CN/std/ops/trait.Drop.html) trait 提供的。当资源离开作用域，就调用析构函数。你无需为每种类型都实现 [`Drop`](https://rustwiki.org/zh-CN/std/ops/trait.Drop.html) trait，只要为那些需要自己的析构函数逻辑的类型实现就可以了。



# ownership

因为变量要负责释放它们拥有的资源，所以**资源只能拥有一个所有者**。这也防止了资源的重复释放。注意并非所有变量都拥有资源（例如[引用](http://localhost:3000/flow_control/match/destructuring/destructure_pointers.html)）。

在进行赋值（`let x = y`）或通过值来传递函数参数（`foo(x)`）的时候，资源的**所有权**（ownership）会发生转移。按照 Rust 的说法，这被称为资源的**移动**（move）。

在移动资源之后，原来的所有者不能再被使用，这可避免悬挂指针（dangling pointer）的产生。



```rust
// 此函数取得堆分配的内存的所有权
fn destroy_box(c: Box<i32>) {
    println!("Destroying a box that contains {}", c);

    // `c` 被销毁且内存得到释放
}

fn main() {
    // 栈分配的整型
    let x = 5u32;

    // 将 `x` *复制*到 `y`——不存在资源移动
    let y = x;

    // 两个值各自都可以使用
    println!("x is {}, and y is {}", x, y);

    // `a` 是一个指向堆分配的整数的指针
    let a = Box::new(5i32);

    println!("a contains: {}", a);
    // y: 这里宏做了一些特别的事情使得实际只做了ref吧，否则这里也move了

    // *移动* `a` 到 `b`
    let b = a;
    // 把 `a` 的指针地址（而非数据）复制到 `b`。现在两者都指向
    // 同一个堆分配的数据，但是现在是 `b` 拥有它。

    // 报错！`a` 不能访问数据，因为它不再拥有那部分堆上的内存。
    //println!("a contains: {}", a);
    // 试一试 ^ 去掉此行注释

    // 此函数从 `b` 中取得堆分配的内存的所有权
    destroy_box(b);

    // 此时堆内存已经被释放，这个操作会导致解引用已释放的内存，而这是编译器禁止的。
    // 报错！和前面出错的原因一样。
    //println!("b contains: {}", b);
    // 试一试 ^ 去掉此行注释
}

```





## move



### move时可能改变可变性



当所有权转移时，数据的可变性可能发生改变。

```rust
fn main() {
    let immutable_box = Box::new(5u32);

    println!("immutable_box contains {}", immutable_box);

    // 可变性错误
    //*immutable_box = 4;

    // *移动* box，改变所有权（和可变性）
    let mut mutable_box = immutable_box;

    println!("mutable_box contains {}", mutable_box);

    // 修改 box 的内容
    *mutable_box = 4;

    println!("mutable_box now contains {}", mutable_box);
}

```



### 部分move

在单个变量的[解构](http://localhost:3000/flow_control/match/destructuring.html)内，可以同时使用 `by-move` 和 `by-reference` 模式绑定。这样做将导致变量的**部分移动**（partial move），这意味着变量的某些部分将被移动，而其他部分将保留。在这种情况下，后面不能整体使用父级变量，但是仍然可以使用只引用（而不移动）的部分。

```rust
fn main() {
    #[derive(Debug)]
    struct Person {
        name: String,
        age: u8,
    }

    let person = Person {
        name: String::from("Alice"),
        age: 20,
    };

    // `name` 从 person 中移走，但 `age` 只是引用
    let Person { name, ref age } = person;

    println!("The person's age is {}", age);

    println!("The person's name is {}", name);

    // 报错！部分移动值的借用：`person` 部分借用产生
    //println!("The person struct is {:?}", person);

    // `person` 不能使用，但 `person.age` 因为没有被移动而可以继续使用
    println!("The person's age from person struct is {}", person.age);
}
```





## misc

### 赋值、传参 操作的语义： move/copy

> 这里指的是直接对值的操作，没有进行引用

* 对于copy类型

  * 相关的资源会做copy再。。。

    > 但怎么“看出”是什么层面的copy？ 浅拷贝还是深拷贝？ 等等

  * "copy" 后原来的binding的状态为 "copied"

  * 主要是基本类型

* 对于move类型

  * 相关的资源会move到新的binding，而原来的binding不能再访问
  * "move" 后原来的binding的状态为 "moved"
  * 非基本类型

以上的语义操作，是基于（类型）实现的trait来确定的。 `move`是默认语义，除非类型实现了 `core::marker::Copy` 这个trait（才会有`copy`语义）。

# borrow

多数情况下，我们更希望能访问数据，同时不取得其所有权。为实现这点，Rust 使用了**借用**（borrowing）机制。对象可以通过引用（`&T`）来传递，从而取代通过值（`T`）来传递。

编译器（通过借用检查）静态地保证了引用**总是**指向有效的对象。也就是说，当存在引用指向一个对象时，该对象不能被销毁。



```rust
// 此函数取得一个 box 的所有权并销毁它
fn eat_box_i32(boxed_i32: Box<i32>) {
    println!("Destroying box that contains {}", boxed_i32);
}

// 此函数借用了一个 i32 类型
fn borrow_i32(borrowed_i32: &i32) {
    println!("This int is: {}", borrowed_i32);
}

fn main() {
    // 创建一个装箱的 i32 类型，以及一个存在栈中的 i32 类型。
    let boxed_i32 = Box::new(5_i32);
    let stacked_i32 = 6_i32;

    // 借用了 box 的内容，但没有取得所有权，所以 box 的内容之后可以再次借用。
    // 译注：请注意函数自身就是一个作用域，因此下面两个函数运行完成以后，
    // 在函数中临时创建的引用也就不复存在了。
    borrow_i32(&boxed_i32);  // y: 竟然可以 &Box<T> 直接当 &T 用。 虽然实现上如此，但类型上竟然也。。 是因为实现了 `AsRef<T>`吗？ as_ref 方法
    borrow_i32(&stacked_i32);

    {
        // 取得一个对 box 中数据的引用
        let _ref_to_i32: &i32 = &boxed_i32;

        // 报错！
        // 当 `boxed_i32` 里面的值之后在作用域中被借用时，不能将其销毁。
        eat_box_i32(boxed_i32);
        // 改正 ^ 注释掉此行

        // 在 `_ref_to_i32` 里面的值被销毁后，尝试借用 `_ref_to_i32`
        //（译注：如果此处不借用，则在上一行的代码中，eat_box_i32(boxed_i32)可以将 `boxed_i32` 销毁。）
        borrow_i32(_ref_to_i32);
        // `_ref_to_i32` 离开作用域且不再被借用。
    }

    // `boxed_i32` 现在可以将所有权交给 `eat_i32` 并被销毁。
    //（译注：能够销毁是因为已经不存在对 `boxed_i32` 的引用）
    eat_box_i32(boxed_i32);
}

```







## borrow时可变： mut ref

可变数据可以使用 `&mut T` 进行可变借用。这叫做**可变引用**（mutable reference），它使借用者可以读/写数据。相反，`&T` 通过不可变引用（immutable reference）来借用数据，借用者可以读数据而不能更改数据：



```rust
#[allow(dead_code)]
#[derive(Clone, Copy)]
struct Book {
    // `&'static str` 是一个对分配在只读内存区的字符串的引用
    author: &'static str,
    title: &'static str,
    year: u32,
}

// 此函数接受一个对 Book 类型的引用
fn borrow_book(book: &Book) {
    println!("I immutably borrowed {} - {} edition", book.title, book.year);
}

// 此函数接受一个对可变的 Book 类型的引用，它把年份 `year` 改为 2014 年
fn new_edition(book: &mut Book) {
    book.year = 2014;
    println!("I mutably borrowed {} - {} edition", book.title, book.year);
}

fn main() {
    // 创建一个名为 `immutabook` 的不可变的 Book 实例
    let immutabook = Book {
        // 字符串字面量拥有 `&'static str` 类型
        author: "Douglas Hofstadter",
        title: "Gödel, Escher, Bach",
        year: 1979,
    };

    // 创建一个 `immutabook` 的可变拷贝，命名为 `mutabook`
    let mut mutabook = immutabook;  // y: 实现了copy所以...

    // 不可变地借用一个不可变对象
    borrow_book(&immutabook);

    // 不可变地借用一个可变对象
    borrow_book(&mutabook);

    // 可变地借用一个可变对象
    new_edition(&mut mutabook);

    // 报错！不能可变地借用一个不可变对象
    new_edition(&mut immutabook);
    // 改正 ^ 注释掉此行
}

```





## borrow时的“读写互斥”



> 原文叫 别名使用

数据可以多次不可变借用，但是在不可变借用的同时，原始数据不能使用可变借用。或者说，同一时间内只允许**一次**可变借用。仅当最后一次使用可变引用**之后**，原始数据才可以再次借用。

```rust
struct Point { x: i32, y: i32, z: i32 }

fn main() {
    let mut point = Point { x: 0, y: 0, z: 0 };

    let borrowed_point = &point;
    let another_borrow = &point;

    // 数据可以通过引用或原始类型来访问
    println!("Point has coordinates: ({}, {}, {})",
                borrowed_point.x, another_borrow.y, point.z);
    
    // 报错！`point` 不能以可变方式借用，因为当前还有不可变借用。
    // let mutable_borrow = &mut point;
    // TODO ^ 试一试去掉此行注释

    // 被借用的值在这里被重新使用
    println!("Point has coordinates: ({}, {}, {})",
                borrowed_point.x, another_borrow.y, point.z);

    // 不可变的引用不再用于其余的代码，因此可以使用可变的引用重新借用。
    let mutable_borrow = &mut point;

    // 通过可变引用来修改数据
    mutable_borrow.x = 5;
    mutable_borrow.y = 2;
    mutable_borrow.z = 1;

    // 报错！不能再以不可变方式来借用 `point`，因为它当前已经被可变借用。
    // let y = &point.y;
    // TODO ^ 试一试去掉此行注释

    // 报错！无法打印，因为 `println!` 用到了一个不可变引用。
    // println!("Point Z coordinate is {}", point.z);
    // TODO ^ 试一试去掉此行注释

    // 正常运行！可变引用能够以不可变类型传入 `println!`
    println!("Point has coordinates: ({}, {}, {})",
                mutable_borrow.x, mutable_borrow.y, mutable_borrow.z);

    // 可变引用不再用于其余的代码，因此可以重新借用
    let new_borrowed_point = &point;
    println!("Point now has coordinates: ({}, {}, {})",
             new_borrowed_point.x, new_borrowed_point.y, new_borrowed_point.z);
}

```



## ref关键字



在通过 `let` 绑定来进行模式匹配或解构时，`ref` 关键字可用来创建结构体/元组的字段的引用。下面的例子展示了几个实例，可看到 `ref` 的作用：

```rust
#[derive(Clone, Copy)]
struct Point { x: i32, y: i32 }

fn main() {
    let c = 'Q';

    // 赋值语句中左边的 `ref` 关键字等价于右边的 `&` 符号。
    let ref ref_c1 = c;
    let ref_c2 = &c;

    println!("ref_c1 equals ref_c2: {}", *ref_c1 == *ref_c2);

    let point = Point { x: 0, y: 0 };

    // 在解构一个结构体时 `ref` 同样有效。
    let _copy_of_x = {
        // `ref_to_x` 是一个指向 `point` 的 `x` 字段的引用。
        let Point { x: ref ref_to_x, y: _ } = point;

        // 返回一个 `point` 的 `x` 字段的拷贝。
        *ref_to_x
        // *&point.x
        // point.x
        // y: 以上皆可
        // TODO y: 如果 x 没实现 copy 的话，解引用会发生什么？
    };

    // `point` 的可变拷贝
    let mut mutable_point = point;

    {
        // `ref` 可以与 `mut` 结合以创建可变引用。
        let Point { x: _, y: ref mut mut_ref_to_y } = mutable_point;

        // 通过可变引用来改变 `mutable_point` 的字段 `y`。
        *mut_ref_to_y = 1;
    }

    println!("point is ({}, {})", point.x, point.y);
    println!("mutable_point is ({}, {})", mutable_point.x, mutable_point.y);

    // 包含一个指针的可变元组
    let mut mutable_tuple = (Box::new(5u32), 3u32);
    
    {
        // 解构 `mutable_tuple` 来改变 `last` 的值。
        let (_, ref mut last) = mutable_tuple;
        *last = 2u32;
    }
    
    println!("tuple is {:?}", mutable_tuple);
}

```





## misc

### 取引用的语义： borrow

* `&`
* `&mut`

理解：

* 看起来像（其他语言的）取指针

  > 从raw pointer的语法`let raw = &x as *const i32;`来看实现估计也是指针

* borrow语义： To receive something with the promise of returning it.
  两种borrow

  * shared (`&T`)
    A piece of data can be borrowed by a single or multiple users, but data should not be altered.

    > 其实命名挺奇怪，为啥不叫readonly、immutable？ 只是因为读写锁概念里读是可多重/共享的？

  * mutable (`&mut T`)
    A piece of data can be borrowed and altered by a single user, but the data should not be accessible for any other users at that time.

  规则： 可以参考读写锁

  * 可读可写不能同时（需待另一种生存期结束
  * borrow可以对move/copy类型
  * 生存期 概念

# lifetime

**生命周期**（lifetime）是这样一种概念，编译器（中的借用检查器）用它来保证所有的借用都是有效的。确切地说，一个变量的生命周期在它创建的时候开始，在它销毁的时候结束。虽然生命周期和作用域经常被一起提到，但它们并不相同。

例如考虑这种情况，我们通过 `&` 来借用一个变量。该**借用拥有一个生命周期，此生命周期由它声明的位置决定**。于是，只要该借用在出借者（lender）被销毁前结束，借用就是有效的。然而，**借用的作用域则是由使用引用的位置决定的**。

在下面的例子和本章节剩下的内容里，我们将看到生命周期和作用域的联系与区别。

> 译注：如果代码中的生命周期示意图乱掉了，请把它复制到任何编辑器中，用等宽字体查看。为避免中文的显示问题，下面一些注释没有翻译。

```
// 下面使用连线来标注各个变量的创建和销毁，从而显示出生命周期。
// `i` 的生命周期最长，因为它的作用域完全覆盖了 `borrow1` 和
// `borrow2` 的。`borrow1` 和 `borrow2` 的周期没有关联，
// 因为它们各不相交。
fn main() {
    let i = 3; // Lifetime for `i` starts. ────────────────┐
    //                                                     │
    { //                                                   │
        let borrow1 = &i; // `borrow1` lifetime starts. ──┐│
        //                                                ││
        println!("borrow1: {}", borrow1); //              ││
    } // `borrow1 ends. ──────────────────────────────────┘│
    //                                                     │
    //                                                     │
    { //                                                   │
        let borrow2 = &i; // `borrow2` lifetime starts. ──┐│
        //                                                ││
        println!("borrow2: {}", borrow2); //              ││
    } // `borrow2` ends. ─────────────────────────────────┘│
    //                                                     │
}   // Lifetime ends. ─────────────────────────────────────┘
```

注意到这里没有用到名称或类型来标注生命周期，这限制了生命周期的用法，在后面我们将会看到生命周期更强大的功能。





## 标注生命周期



```rust
// `print_refs` 接受两个 `i32` 的引用，它们有不同的生命周期 `'a` 和 `'b`。
// 这两个生命周期都必须至少要和 `print_refs` 函数一样长。
fn print_refs<'a, 'b>(x: &'a i32, y: &'b i32) {
    println!("x is {} and y is {}", x, y);
}

// 不带参数的函数，不过有一个生命周期参数 `'a`。
fn failed_borrow<'a>() {
    let _x = 12;

    // 报错：`_x` 的生命周期不够长
    //let y: &'a i32 = &_x;
    // 在函数内部使用生命周期 `'a` 作为显式类型标注将导致失败，因为 `&_x` 的
    // 生命周期比 `y` 的短。短生命周期不能强制转换成长生命周期。
}

fn main() {
    // 创建变量，稍后用于借用。
    let (four, nine) = (4, 9);

    // 两个变量的借用（`&`）都传进函数。
    print_refs(&four, &nine);
    // 任何被借用的输入量都必须比借用者生存得更长。
    // 也就是说，`four` 和 `nine` 的生命周期都必须比 `print_refs` 的长。

    failed_borrow();
    // `failed_borrow` 未包含引用，因此不要求 `'a` 长于函数的生命周期，
    // 但 `'a` 寿命确实更长。因为该生命周期从未被约束，所以默认为 `'static`。
}

```



## 各场景中的声明周期

### 函数签名中的生命周期

排除[省略](http://localhost:3000/scope/lifetime/elision.html)（elision）的情况，带上生命周期的函数签名有一些限制：

- 任何引用**都必须**拥有标注好的生命周期。
- 任何被返回的引用**都必须**有和某个输入量相同的生命周期或是静态类型（`static`）。

另外要注意，如果没有输入的函数返回引用，有时会导致返回的引用指向无效数据，这种情况下禁止它返回这样的引用。下面例子展示了一些合法的带有生命周期的函数：



```rust
// 一个拥有生命周期 `'a` 的输入引用，其中 `'a` 的存活时间
// 至少与函数的一样长。
fn print_one<'a>(x: &'a i32) {
    println!("`print_one`: x is {}", x);
}

// 可变引用同样也可能拥有生命周期。
fn add_one<'a>(x: &'a mut i32) {
    *x += 1;
}

// 拥有不同生命周期的多个元素。对下面这种情形，两者即使拥有
// 相同的生命周期 `'a` 也没问题，但对一些更复杂的情形，可能
// 就需要不同的生命周期了。
fn print_multi<'a, 'b>(x: &'a i32, y: &'b i32) {
    println!("`print_multi`: x is {}, y is {}", x, y);
}

// 返回传递进来的引用也是可行的。
// 但必须返回正确的生命周期。
fn pass_x<'a, 'b>(x: &'a i32, _: &'b i32) -> &'a i32 { x }

//fn invalid_output<'a>() -> &'a String { &String::from("foo") }
// 上面代码是无效的：`'a` 存活的时间必须比函数的长。
// 这里的 `&String::from("foo")` 将会创建一个 `String` 类型，然后对它取引用。
// 数据在离开作用域时删掉，返回一个指向无效数据的引用。

fn main() {
    let x = 7;
    let y = 9;

    print_one(&x);
    print_multi(&x, &y);

    let z = pass_x(&x, &y);
    print_one(z);

    let mut t = 3;
    add_one(&mut t);
    print_one(&t);
}

```



### 方法中的生命周期

方法的标注和函数类似：

```
struct Owner(i32);

impl Owner {
    // 标注生命周期，就像独立的函数一样。
    fn add_one<'a>(&'a mut self) { self.0 += 1; }
    fn print<'a>(&'a self) {
        println!("`print`: {}", self.0);
    }
}

fn main() {
    let mut owner  = Owner(18);

    owner.add_one();
    owner.print();
}
```

> 译注：方法一般是不需要标明生命周期的，因为 `self` 的生命周期会赋给所有的输出生命周期参数，详见 [TRPL](https://rustwiki.org/zh-CN/book/ch10-03-lifetime-syntax.html#生命周期省略lifetime-elision)。



### 结构体中的生命周期

在结构体中标注生命周期也和函数的类似：

```rust
// 一个 `Borrowed` 类型，含有一个指向 `i32` 类型的引用。
// 该引用必须比 `Borrowed` 寿命更长。
#[derive(Debug)]
struct Borrowed<'a>(&'a i32);

// 和前面类似，这里的两个引用都必须比这个结构体长寿。
#[derive(Debug)]
struct NamedBorrowed<'a> {
    x: &'a i32,
    y: &'a i32,
}

// 一个枚举类型，其取值不是 `i32` 类型就是一个指向 `i32` 的引用。
#[derive(Debug)]
enum Either<'a> {
    Num(i32),
    Ref(&'a i32),
}

fn main() {
    let x = 18;
    let y = 15;

    let single = Borrowed(&x);
    let double = NamedBorrowed { x: &x, y: &y };
    let reference = Either::Ref(&x);
    let number    = Either::Num(y);

    println!("x is borrowed in {:?}", single);
    println!("x and y are borrowed in {:?}", double);
    println!("x is borrowed in {:?}", reference);
    println!("y is *not* borrowed in {:?}", number);
}
```

> TODO y: 有意义的使用场景样例？



### trait中的生命周期

trait 方法中生命期的标注基本上与函数类似。注意，`impl` 也可能有生命周期的标注。

```rust
// 带有生命周期标注的结构体。
#[derive(Debug)]
 struct Borrowed<'a> {
     x: &'a i32,
 }

// 给 impl 标注生命周期。
impl<'a> Default for Borrowed<'a> {
    fn default() -> Self {
        Self {
            x: &10,
        }
    }
}

fn main() {
    let b: Borrowed = Default::default();
    println!("b is {:?}", b);
}
```

> TODO y: 有意义的使用场景样例？





## 生命周期的约束

就如泛型类型能够被约束一样，生命周期（它们本身就是泛型）也可以使用约束。**`:` 字符的意义在这里稍微有些不同**，不过 `+` 是相同的。注意下面的说明：

1. `T: 'a`：在 `T` 中的**所有**引用都必须比生命周期 `'a` 活得更长。

2. `T: Trait + 'a`：`T` 类型必须实现 `Trait` trait，并且在 `T` 中的**所有**引用都必须比 `'a` 活得更长。

   > y: 泛型 + 生命周期

下面例子展示了上述语法的实际应用：

```rust
use std::fmt::Debug; // 用于约束的 trait。

#[derive(Debug)]
struct Ref<'a, T: 'a>(&'a T);
// `Ref` 包含一个指向泛型类型 `T` 的引用，其中 `T` 拥有一个未知的生命周期
// `'a`。`T` 拥有生命周期限制， `T` 中的任何*引用*都必须比 `'a` 活得更长。另外
// `Ref` 的生命周期也不能超出 `'a`。

// 一个泛型函数，使用 `Debug` trait 来打印内容。
fn print<T>(t: T) where
    T: Debug {
    println!("`print`: t is {:?}", t);
}

// 这里接受一个指向 `T` 的引用，其中 `T` 实现了 `Debug` trait，并且在 `T` 中的
// 所有*引用*都必须比 `'a'` 存活时间更长。另外，`'a` 也要比函数活得更长。
fn print_ref<'a, T>(t: &'a T) where
    T: Debug + 'a {
    println!("`print_ref`: t is {:?}", t);
}

fn main() {
    let x = 7;
    let ref_x = Ref(&x);

    print_ref(&ref_x);
    print(ref_x);
}
```



## 生命周期的转换

一个较长的生命周期可以强制转成一个较短的生命周期，使它在一个通常情况下不能工作的作用域内也能正常工作。强制转换可由编译器隐式地推导并执行，也可以通过声明不同的生命周期的形式实现。

```rust
// 在这里，Rust 推导了一个尽可能短的生命周期。
// 然后这两个引用都被强制转成这个生命周期。
fn multiply<'a>(first: &'a i32, second: &'a i32) -> i32 {
    first * second
}

// `<'a: 'b, 'b>` 读作生命周期 `'a` 至少和 `'b` 一样长。
// 在这里我们我们接受了一个 `&'a i32` 类型并返回一个 `&'b i32` 类型，这是
// 强制转换得到的结果。
fn choose_first<'a: 'b, 'b>(first: &'a i32, _: &'b i32) -> &'b i32 {
    first
}

fn main() {
    let first = 2; // 较长的生命周期

    {
        let second = 3; // 较短的生命周期

        println!("The product is {}", multiply(&first, &second));
        println!("{} is the first", choose_first(&first, &second));
    };
}

```



> TODO y: 长转短的意义是？



## static生命周期

`'static` 生命周期是可能的生命周期中最长的，它会在整个程序运行的时期中存在。`'static` 生命周期也可被强制转换成一个更短的生命周期。有两种方式使变量拥有 `'static` 生命周期，它们都把数据保存在可执行文件的只读内存区：

- 使用 `static` 声明来产生常量（constant）。
- 产生一个拥有 `&'static str` 类型的 `string` 字面量。

看下面的例子，了解列举到的各个方法：

```
// 产生一个拥有 `'static` 生命周期的常量。
static NUM: i32 = 18;

// 返回一个指向 `NUM` 的引用，该引用不取 `NUM` 的 `'static` 生命周期，
// 而是被强制转换成和输入参数的一样。
fn coerce_static<'a>(_: &'a i32) -> &'a i32 {
    &NUM
}

fn main() {
    {
        // 产生一个 `string` 字面量并打印它：
        let static_string = "I'm in read-only memory";
        println!("static_string: {}", static_string);

        // 当 `static_string` 离开作用域时，该引用不能再使用，不过
        // 数据仍然存在于二进制文件里面。
    }

    {
        // 产生一个整型给 `coerce_static` 使用：
        let lifetime_num = 9;

        println!("static NUM: {}", &NUM);
        // 将对 `NUM` 的引用强制转换成 `lifetime_num` 的生命周期：
        let coerced_static = coerce_static(&lifetime_num);

        println!("coerced_static: {}", coerced_static);
        println!("static NUM: {}", &NUM);
    }

    println!("NUM: {} stays accessible!", NUM);
}
```

> TODO y: 感觉莫名其妙啊，干了个啥？ 意义？



## 省略生命周期elision

> y: 一般就这样了



有些生命周期的模式太常用了，所以借用检查器将会隐式地添加它们以减少程序输入量和增强可读性。这种隐式添加生命周期的过程称为省略（elision）。在 Rust 使用省略仅仅是因为这些模式太普遍了。

下面代码展示了一些省略的例子。对于省略的详细描述，可以参考官方文档的[生命周期省略](https://rustwiki.org/zh-CN/book/ch10-03-lifetime-syntax.html#生命周期省略lifetime-elision)。

```rust
// `elided_input` 和 `annotated_input` 事实上拥有相同的签名，
// `elided_input` 的生命周期会被编译器自动添加：
fn elided_input(x: &i32) {
    println!("`elided_input`: {}", x)
}

fn annotated_input<'a>(x: &'a i32) {
    println!("`annotated_input`: {}", x)
}

// 类似地，`elided_pass` 和 `annotated_pass` 也拥有相同的签名，
// 生命周期会被隐式地添加进 `elided_pass`：
fn elided_pass(x: &i32) -> &i32 { x }

fn annotated_pass<'a>(x: &'a i32) -> &'a i32 { x }

fn main() {
    let x = 3;

    elided_input(&x);
    annotated_input(&x);

    println!("`elided_pass`: {}", elided_pass(&x));
    println!("`annotated_pass`: {}", annotated_pass(&x));
}
```



## misc

**内存管理**：
* 手动内存管理
* 自动内存管理
  * 垃圾回收（运行时）
  * ...

* 一个资源，同一个时刻只能有一个owner
  重用（所以不是move也不是copy）这个资源，只能 引用/borrow
* 处理引用时，可以手动添加生存期注解，以便于编译器判断
  函数定义中的生存期注解会（隐式）自动添加
  会在编译器检查生存期注解： 第一次和最后一次使用（某资源/数据）
* 对内存的管理操作
  * 不显式的 drop（free？）数据
  * 没有类似gc语言的对象析构时的finalize调用
  * 会在生存期结束（go out of scope）时执行deallocation call
    这个

**生存期结束**：
* 同一层级SCOPE最后一次使用；
* 出了当前层级scope（如果是scope内的binding）；


**生存期约束**
如 `x.a`这样访问，要确保x生存期内a没有被释放。

#### 生存期注解

* 主要用来标识、约束生存期

* 格式 `'`开头，一般约定： 后面用一个小写字母，如 `'a`。 使用多个时，按照字母顺序一次使用

* 使用场景
  示例见下文
  * 函数声明
    
  * 结构体、枚举声明
  * impl、trait
  * 泛型

* 省略注解

  * `fn`
    条件是参数列表满足其一：

    * 只有一个入参是引用
      则所有出参中省略的生存期注解都为此
      `..(x: i32, y: &str) -> &str` → `..<'a>(x: i32, y: &'a str) -> &'a str`
    
    * 有`&self`或`&mut self`
      则...都为此
      `impl Impl{ fn function(&self, x: &str) -> &str {} }` →
`impl<'a> Impl<'a>{ fn function(&'a self, x: &'b str) -> &'a str {} }`
    > 但好像还有一种情况就是所有引用传参的入参的生存周期注解都不同，那么也可以省略。 但此时出参的生存周期约束是？
    > `..(x: &str, y: &str)` → `..<'a, 'b>(x: &'a str, y: &'b str)`

  * `impl` - 未来支持

* `'static`
  * 保留的特殊注解
  * 表示生存期在整个程序生存期间都有效
    数据保存在数据段，访问数据不会gg
    ```rust
    static N: i32 = 5; // A constant with 'static lifetime
    let a = "Hello, world."; // a: &'static str
    fn index() -> &'static str { // No need to mention <'static> ; fn index ̶<̶'̶s̶t̶a̶t̶i̶c̶>̶ 
        "Hello, world!"
    }
    ```

**函数声明中的生存期注解**
> 不太清除具体的意义

```rust
// No inputs, return a reference
fn function<'a>() -> &'a str {}

// Single input
fn function<'a>(x: &'a str) {}

// Single input and output, both have the same lifetime
// The output should live at least as long as input exists
fn function<'a>(x: &'a str) -> &'a str {}

// Multiple inputs, only one input and the output share same lifetime
// The output should live at least as long as y exists
fn function<'a>(x: i32, y: &'a str) -> &'a str {}

// Multiple inputs, both inputs and the output share same lifetime
// The output should live at least as long as x and y exist
fn function<'a>(x: &'a str, y: &'a str) -> &'a str {}

// Multiple inputs, inputs can have different lifetimes 🔎
// The output should live at least as long as x exists
fn function<'a, 'b>(x: &'a str, y: &'b str) -> &'a str {}
```

**结构体、枚举中的生存期注解**
```rust
// Single element
// Data of x should live at least as long as Struct exists
struct Struct<'a> {
    x: &'a str
}

// Multiple elements
// Data of x and y should live at least as long as Struct exists
struct Struct<'a> {
    x: &'a str,
    y: &'a str
}


// Variant with a single element
// Data of the variant should live at least as long as Enum exists
enum Enum<'a> {
    Variant(&'a Type)
}
```

**impl中的生存期注解**
```rust
struct Struct<'a> {
    x: &'a str
}
    impl<'a> Struct<'a> {
        fn function<'a>(&self) -> &'a str {
            self.x
        }
    }


struct Struct<'a> {
    x: &'a str,
    y: &'a str
}
    impl<'a> Struct<'a> {
        fn new(x: &'a str, y: &'a str) -> Struct<'a> { // No need to specify <'a> after new; impl already has it
          Struct {
              x : x,
              y : y
          }
        }
    }


// 🔎
impl<'a> Trait<'a> for Type
impl<'a> Trait for Type<'a>
```

```rust
// 🔎
fn function<F>(f: F) where for<'a> F: FnOnce(&'a Type)
struct Struct<F> where for<'a> F: FnOnce(&'a Type) { x: F }
enum Enum<F> where for<'a> F: FnOnce(&'a Type) { Variant(F) }
impl<F> Struct<F> where for<'a> F: FnOnce(&'a Type) { fn x(&self) -> &F { &self.x } }
```