

错误处理（error handling）是处理可能发生的失败情况的过程。



在 Rust 中有多种处理错误的方式，在接下来的小节中会一一介绍。它们多少有些 区别，使用场景也不尽相同。总的来说：

- 显式的 `panic` 主要用于测试，以及处理不可恢复的错误。在原型开发中这很有用，比如 用来测试还没有实现的函数，不过这时使用 `unimplemented` 更能表达意图。另外在 测试中，`panic` 是一种显式地失败（fail）的好方法。
- `Option` 类型是为了值是可选的、或者缺少值并不是错误的情况准备的。比如说寻找 父目录时，`/` 和 `C:` 这样的目录就没有父目录，这应当并不是一个错误。当处理 `Option` 时，`unwrap` 可用于原型开发，也可以用于能够确定 `Option` 中一定有值 的情形。然而 `expect` 更有用，因为它允许你指定一条错误信息，以免万一还是出现 了错误。
- 当错误有可能发生，且应当由调用者处理时，使用 `Result`。你也可以 `unwrap` 然后 使用 `expect`，但是除了在测试或者原型开发中，请不要这样做。



## panic

最简单的错误处理机制就是 `panic`。它会打印一个错误消息，开始 回退（unwind）任务，且通常会退出程序。这里我们显式地在错误条件下调用 `panic`：

```rust
fn give_princess(gift: &str) {
    // 公主讨厌蛇，所以如果公主表示厌恶的话我们要停止！
    if gift == "snake" { panic!("AAAaaaaa!!!!"); }

    println!("I love {}s!!!!!", gift);
}

fn main() {
    give_princess("teddy bear");
    give_princess("snake");
}

```



## Option



在标准库（`std`）中有个叫做 `Option<T>`（option 中文意思是 “选项”）的枚举 类型，用于有 “不存在” 的可能性的情况。它表现为以下两个 “option”（选项）中 的一个：

- `Some(T)`：找到一个属于 `T` 类型的元素
- `None`：找不到相应元素

这些选项可以通过 `match` 显式地处理，或使用 `unwrap` 隐式地处理。隐式处理要么 返回 `Some` 内部的元素，要么就 `panic`。



```rust
// 平民（commoner）们见多识广，收到什么礼物都能应对。
// 所有礼物都显式地使用 `match` 来处理。
fn give_commoner(gift: Option<&str>) {
    // 指出每种情况下的做法。
    match gift {
        Some("snake") => println!("Yuck! I'm throwing that snake in a fire."),
        Some(inner)   => println!("{}? How nice.", inner),
        None          => println!("No gift? Oh well."),
    }
}

// 养在深闺人未识的公主见到蛇就会 `panic`（恐慌）。
// 这里所有的礼物都使用 `unwrap` 隐式地处理。
fn give_princess(gift: Option<&str>) {
    // `unwrap` 在接收到 `None` 时将返回 `panic`。
    let inside = gift.unwrap();
    if inside == "snake" { panic!("AAAaaaaa!!!!"); }

    println!("I love {}s!!!!!", inside);
}

fn main() {
    let food  = Some("chicken");
    let snake = Some("snake");
    let void  = None;

    give_commoner(food);
    give_commoner(snake);
    give_commoner(void);

    let bird = Some("robin");
    let nothing = None;

    give_princess(bird);
    give_princess(nothing);
}

```



### 用`?`解开Option



你可以使用 `match` 语句来解开 `Option`，但使用 `?` 运算符通常会更容易。如果 `x` 是 `Option`，那么若 `x` 是 `Some` ，对`x?`表达式求值将返回底层值，否则无论函数是否正在执行都将终止且返回 `None`。

> y: 与`unwrap`不同，后者失败会panic； 前者则 "abort"，等于match写法



```rust
fn next_birthday(current_age: Option<u8>) -> Option<String> {
    // 如果 `current_age` 是 `None`，这将返回 `None`。
    // 如果 `current_age` 是 `Some`，内部的 `u8` 将赋值给 `next_age`。
    let next_age: u8 = current_age?;
    Some(format!("Next year I will be {}", next_age))
}
```

你可以将多个 `?` 链接在一起，以使代码更具可读性。

```rust
struct Person {
    job: Option<Job>,
}

#[derive(Clone, Copy)]
struct Job {
    phone_number: Option<PhoneNumber>,
}

#[derive(Clone, Copy)]
struct PhoneNumber {
    area_code: Option<u8>,
    number: u32,
}

impl Person {

    // 获取此人的工作电话号码的区号（如果存在的话）。
    fn work_phone_area_code(&self) -> Option<u8> {
        // 没有`？`运算符的话，这将需要很多的嵌套的 `match` 语句。
        // 这将需要更多代码——尝试自己编写一下，看看哪个更容易。
        self.job?.phone_number?.area_code
    }
}

fn main() {
    let p = Person {
        job: Some(Job {
            phone_number: Some(PhoneNumber {
                area_code: Some(61),
                number: 439222222,
            }),
        }),
    };

    assert_eq!(p.work_phone_area_code(), Some(61));
}
```



### 组合算子： map



可以使用[组合算子](https://rustwiki.org/zh-CN/reference/glossary.html#组合算子)（combinator），以 模块化的风格来管理控制流。

`Option` 有一个内置方法 `map()`，这个组合算子可用于 `Some -> Some` 和 `None -> None` 这样的简单映射。多个不同的 `map()` 调用可以串起来，这样更加灵活。



```rust
#![allow(dead_code)]

#[derive(Debug)] enum Food { Apple, Carrot, Potato }

#[derive(Debug)] struct Peeled(Food);
#[derive(Debug)] struct Chopped(Food);
#[derive(Debug)] struct Cooked(Food);

// 削皮。如果没有食物，就返回 `None`。否则返回削好皮的食物。
fn peel(food: Option<Food>) -> Option<Peeled> {
    match food {
        Some(food) => Some(Peeled(food)),
        None       => None,
    }
}

// 切食物。如果没有食物，就返回 `None`。否则返回切好的食物。
fn chop(peeled: Option<Peeled>) -> Option<Chopped> {
    match peeled {
        Some(Peeled(food)) => Some(Chopped(food)),
        None               => None,
    }
}

// 烹饪食物。这里，我们使用 `map()` 来替代 `match` 以处理各种情况。
fn cook(chopped: Option<Chopped>) -> Option<Cooked> {
    chopped.map(|Chopped(food)| Cooked(food))
    // y: 不太理解它怎么“脑补”出None分支
    // 看了下实现（见下面），是我naive了...
}

// 这个函数会完成削皮切块烹饪一条龙。我们把 `map()` 串起来，以简化代码。
fn process(food: Option<Food>) -> Option<Cooked> {
    food.map(|f| Peeled(f))
        .map(|Peeled(f)| Chopped(f))
        .map(|Chopped(f)| Cooked(f))
}

// 在尝试吃食物之前确认食物是否存在是非常重要的！
fn eat(food: Option<Cooked>) {
    match food {
        Some(food) => println!("Mmm. I love {:?}", food),
        None       => println!("Oh no! It wasn't edible."),
    }
}

fn main() {
    let apple = Some(Food::Apple);
    let carrot = Some(Food::Carrot);
    let potato = None;

    let cooked_apple = cook(chop(peel(apple)));
    let cooked_carrot = cook(chop(peel(carrot)));

    // 现在让我们试试看起来更简单的 `process()`。
    let cooked_potato = process(potato);

    eat(cooked_apple);
    eat(cooked_carrot);
    eat(cooked_potato);
}

```



```rust
    #[inline]
    #[stable(feature = "rust1", since = "1.0.0")]
    #[rustc_const_unstable(feature = "const_option_ext", issue = "91930")]
    pub const fn map<U, F>(self, f: F) -> Option<U>
    where
        F: ~const FnOnce(T) -> U,
        F: ~const Destruct,
    {
        match self {
            Some(x) => Some(f(x)),
            None => None,
        }
    }
```



#### vs and_then

`map`是认为 A一定能->B（当然了`Option<A>`自己为`None`的情况自己处理）；

`and_then`则是A不一定能->B，那么...





### 组合算法： and_then

如果以返回类型是 `Option<T>` 的函数作为 `map()` 的参数，会导致出现嵌套形式 `Option<Option<T>>`。这样多层串联 调用就会变得混乱。所以有必要引入 `and_then()`，在某些语言中它叫做 flatmap。

`and_then()` 使用被 `Option` 包裹的值来调用其输入函数并返回结果。 如果 `Option` 是 `None`，那么它返回 `None`。

```rust
    #[inline]
    #[stable(feature = "rust1", since = "1.0.0")]
    #[rustc_const_unstable(feature = "const_option_ext", issue = "91930")]
    pub const fn and_then<U, F>(self, f: F) -> Option<U>
    where
        F: ~const FnOnce(T) -> Option<U>,
        F: ~const Destruct,
    {
        match self {
            Some(x) => f(x),
            None => None,
        }
    }
```





```rust
#![allow(dead_code)]

#[derive(Debug)] enum Food { CordonBleu, Steak, Sushi }
#[derive(Debug)] enum Day { Monday, Tuesday, Wednesday }

// 我们没有制作寿司所需的原材料（ingredient）（有其他的原材料）。
fn have_ingredients(food: Food) -> Option<Food> {
    match food {
        Food::Sushi => None,
        _           => Some(food),
    }
}

// 我们拥有全部食物的食谱，除了法国蓝带猪排（Cordon Bleu）的。
fn have_recipe(food: Food) -> Option<Food> {
    match food {
        Food::CordonBleu => None,
        _                => Some(food),
    }
}


// 要做一份好菜，我们需要原材料和食谱。
// 我们可以借助一系列 `match` 来表达这个逻辑：
fn cookable_v1(food: Food) -> Option<Food> {
    match have_ingredients(food) {
        None       => None,
        Some(food) => match have_recipe(food) {
            None       => None,
            Some(food) => Some(food),
        },
    }
}

// 也可以使用 `and_then()` 把上面的逻辑改写得更紧凑：
fn cookable_v2(food: Food) -> Option<Food> {
    have_ingredients(food).and_then(have_recipe)
}

fn eat(food: Food, day: Day) {
    match cookable_v2(food) {
        Some(food) => println!("Yay! On {:?} we get to eat {:?}.", day, food),
        None       => println!("Oh no. We don't get to eat on {:?}?", day),
    }
}

fn main() {
    let (cordon_bleu, steak, sushi) = (Food::CordonBleu, Food::Steak, Food::Sushi);

    eat(cordon_bleu, Day::Monday);
    eat(steak, Day::Tuesday);
    eat(sushi, Day::Wednesday);
}

```



## Result



[`Result`](https://rustwiki.org/zh-CN/std/result/enum.Result.html) 是 [`Option`](https://rustwiki.org/zh-CN/std/option/enum.Option.html) 类型的更丰富的版本，描述的是可能 的**错误**而不是可能的**不存在**。

也就是说，`Result<T，E>` 可以有两个结果的其中一个：

- `Ok<T>`：找到 `T` 元素
- `Err<E>`：找到 `E` 元素，`E` 即表示错误的类型。

按照约定，预期结果是 “Ok”，而意外结果是 “Err”。

`Result` 有很多类似 `Option` 的方法。例如 `unwrap()`，它要么举出元素 `T`，要么就 `panic`。 对于事件的处理，`Result` 和 `Option` 有很多相同的组合算子。



> y: Option的正常的 有、无 两种情况； Result则是预期有但实际可能无（异常）的两种情况，Err比None也能承载更多的信息（怎么异常了？）



> 就是go的 `func xx() (ResultType, error)` 的优雅版



```rust
fn multiply(first_number_str: &str, second_number_str: &str) -> i32 {
    // 我们试着用 `unwrap()` 把数字放出来。它会咬我们一口吗？
    let first_number = first_number_str.parse::<i32>().unwrap();
    let second_number = second_number_str.parse::<i32>().unwrap();
    first_number * second_number
}

fn main() {
    let twenty = multiply("10", "2");
    println!("double is {}", twenty);

    let tt = multiply("t", "2");
    println!("double is {}", tt);
}

```



### Result map/and_then



类似的：

```rust
    #[inline]
    #[stable(feature = "rust1", since = "1.0.0")]
    pub fn map<U, F: FnOnce(T) -> U>(self, op: F) -> Result<U, E> {
        match self {
            Ok(t) => Ok(op(t)),
            Err(e) => Err(e),
        }
    }

    #[inline]
    #[stable(feature = "rust1", since = "1.0.0")]
    pub fn and_then<U, F: FnOnce(T) -> Result<U, E>>(self, op: F) -> Result<U, E> {
        match self {
            Ok(t) => op(t),
            Err(e) => Err(e),
        }
    }
```



```rust
use std::num::ParseIntError;

// 就像 `Option` 那样，我们可以使用 `map()` 之类的组合算子。
// 除去写法外，这个函数与上面那个完全一致，它的作用是：
// 如果值是合法的，计算其乘积，否则返回错误。
fn multiply(first_number_str: &str, second_number_str: &str) -> Result<i32, ParseIntError> {
    first_number_str.parse::<i32>().and_then(|first_number| {
        second_number_str.parse::<i32>().map(|second_number| first_number * second_number)
    })
}

fn print(result: Result<i32, ParseIntError>) {
    match result {
        Ok(n)  => println!("n is {}", n),
        Err(e) => println!("Error: {}", e),
    }
}

fn main() {
    // 这种情况下仍然会给出正确的答案。
    let twenty = multiply("10", "2");
    print(twenty);

    // 这种情况下就会提供一条更有用的错误信息。
    let tt = multiply("t", "2");
    print(tt);
}

```



### Result别名



在模块的层面上创建别名特别有帮助。同一模块中的错误常常会有相同的 `Err` 类 型，所以单个别名就能简便地定义**所有**相关的 `Result`。这太有用了，以至于标准库 也提供了一个别名： [`io::Result`](https://rustwiki.org/zh-CN/std/io/type.Result.html)！

> partial-type? specialized-type?



```rust
use std::num::ParseIntError;

// 为带有错误类型 `ParseIntError` 的 `Result` 定义一个泛型别名。
type AliasedResult<T> = Result<T, ParseIntError>;

// 使用上面定义过的别名来表示上一节中的 `Result<i32,ParseIntError>` 类型。
fn multiply(first_number_str: &str, second_number_str: &str) -> AliasedResult<i32> {
    first_number_str.parse::<i32>().and_then(|first_number| {
        second_number_str.parse::<i32>().map(|second_number| first_number * second_number)
    })
}

// 在这里使用别名又让我们节省了一些代码量。
fn print(result: AliasedResult<i32>) {
    match result {
        Ok(n)  => println!("n is {}", n),
        Err(e) => println!("Error: {}", e),
    }
}

fn main() {
    print(multiply("10", "2"));
    print(multiply("t", "2"));
}

```



### 提前返回： match-return, `?`/`try!`



```rust
use std::num::ParseIntError;

fn multiply(first_number_str: &str, second_number_str: &str) -> Result<i32, ParseIntError> {
    let first_number = match first_number_str.parse::<i32>() {
        Ok(first_number)  => first_number,
        Err(e) => return Err(e),
    };

    let second_number = match second_number_str.parse::<i32>() {
        Ok(second_number)  => second_number,
        Err(e) => return Err(e),
    };

    Ok(first_number * second_number)
}

fn print(result: Result<i32, ParseIntError>) {
    match result {
        Ok(n)  => println!("n is {}", n),
        Err(e) => println!("Error: {}", e),
    }
}

fn main() {
    print(multiply("10", "2"));
    print(multiply("t", "2"));
}

```

有点啰嗦





```rust
use std::num::ParseIntError;

fn multiply(first_number_str: &str, second_number_str: &str) -> Result<i32, ParseIntError> {
    let first_number = first_number_str.parse::<i32>()?;
    let second_number = second_number_str.parse::<i32>()?;

    Ok(first_number * second_number)
}

fn print(result: Result<i32, ParseIntError>) {
    match result {
        Ok(n)  => println!("n is {}", n),
        Err(e) => println!("Error: {}", e),
    }
}

fn main() {
    print(multiply("10", "2"));
    print(multiply("t", "2"));
}

```

好多了，有个小问题是： 这些err需要类型一致，如本例里的`ParseIntError`，那如果不是呢？ 见下文



`?`之前的老版本： `try!`宏

```rust
use std::num::ParseIntError;

fn multiply(first_number_str: &str, second_number_str: &str) -> Result<i32, ParseIntError> {
    let first_number = try!(first_number_str.parse::<i32>());
    let second_number = try!(second_number_str.parse::<i32>());

    Ok(first_number * second_number)
}

fn print(result: Result<i32, ParseIntError>) {
    match result {
        Ok(n)  => println!("n is {}", n),
        Err(e) => println!("Error: {}", e),
    }
}

fn main() {
    print(multiply("10", "2"));
    print(multiply("t", "2"));
}

```



### 灵活的Result处理经验



### `Iter::map` 的result

因为可能失败，所以有多种处理方式：

1. `filter_map`来过滤掉

   `.into_iter()..filter_map(|s| s.parse::<i32>().ok()).collect()`

2. 基于 `Result` 实现了 `FromIter`，那么 结果的向量（`Vec<Result<T, E>>`）可以被转换成 结果包裹着向量（`Result<Vec<T>, E>`）。一旦找到一个 `Result::Err` ，遍历就被终止

   `let numbers: Result<Vec<_>, _> = strings.into_iters().map(|s| s.parse::<i32>()).collect()`

3. 用`partition()`来分别收集合法的值和错误

   `let (numbers, errors): (Vec<_>, Vec<_>) = strings.into_iters().map(|s| s.parse::<i32>()).partition(Result::is_ok)`



## `std::error::Error` trait



```rust
type Result<T> = std::result::Result<T, DoubleError>;

#[derive(Debug, Clone)]
// 定义我们的错误类型，这种类型可以根据错误处理的实际情况定制。
// 我们可以完全自定义错误类型，也可以在类型中完全采用底层的错误实现，
// 也可以介于二者之间。
struct DoubleError;

// 错误的生成与它如何显示是完全没关系的。没有必要担心复杂的逻辑会导致混乱的显示。
//
// 注意我们没有储存关于错误的任何额外信息，也就是说，如果不修改我们的错误类型定义的话，
// 就无法指明是哪个字符串解析失败了。
impl fmt::Display for DoubleError {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "invalid first item to double")
    }
}

// 为 `DoubleError` 实现 `Error` trait，这样其他错误可以包裹这个错误类型。
impl error::Error for DoubleError {
    fn source(&self) -> Option<&(dyn error::Error + 'static)> {
        // 泛型错误，没有记录其内部原因。
        None
    }
}
```





## 处理多种错误类型

大约有几种思路：

### 1.复杂返回值类型包含各种情况

最基本的手段就是让它们互相包含，就是让返回值是一个复杂的类型，足以包含各种返回值情况

如： 

```rust
fn double_first(vec: Vec<&str>) -> Option<Result<i32, ParseIntError>> {
    vec.first().map(|first| {
        first.parse::<i32>().map(|n| 2 * n)
    })
    // 如有需要还能继续： .map_or(Ok(None), |r| r.map(Some)) 来处理成其他的结果内容
}
```

结果既可以是 None、也可以是 Err

> 还可以用别名来简写



### 2.收敛为一种错误

思路与1相反： 降多种错误收敛为一种，就可以得到统一的返回结果了

可以返回大接口err； 也可以map为特定类型err

```rust
fn double_first(vec: Vec<&str>) -> Result<i32> {
    vec.first()
       // 把错误换成我们的新类型。
       .ok_or(DoubleError)
       .and_then(|s| {
            s.parse::<i32>()
                // 这里也换成新类型。
                .map_err(|_| DoubleError)
                .map(|i| 2 * i)
        })
}
```

这个例子就是把 无值、解析错误 都转换成自定义的 err类型

```rust
fn b(i: i32) -> Box<dyn error::Error> {  // dyn error::Error 不行； impl error::Error 也不行
    if i % 2 == 0 {
        Box::new(DoubleError)
    } else {
        Box::new(fmt::Error)
    }
}
```

> y: 太丑了吧 = =

改进： 对任何实现了 `Error` trait 的类型，标准库的 `Box` 通过 [`From`](https://rustwiki.org/zh-CN/std/convert/trait.From.html) 为它们提供了 到 `Box<Error>` 的转换。 那么：

```rust
// 为 `Box<error::Error>` 取别名。
type Result<T> = std::result::Result<T, Box<dyn error::Error>>;

fn double_first(vec: Vec<&str>) -> Result<i32> {
    vec.first()
       .ok_or_else(|| EmptyVec.into())  // 装箱
       .and_then(|s| {
            s.parse::<i32>()
                .map_err(|e| e.into())  // 装箱
                .map(|i| 2 * i)
        })
}
```



### 3.收敛错误包裹原错误内容

对2的演进： 把原来的错误内容“包裹”进“统一”的/返回的错误类型中 （最终可以得到一个错误链）

```rust
#[derive(Debug)]
enum DoubleError {
    EmptyVec,
    // 在这个错误类型中，我们采用 `parse` 的错误类型中 `Err` 部分的实现。
    // 若想提供更多信息，则该类型中还需要加入更多数据。
    Parse(ParseIntError),
}

impl error::Error for DoubleError {
    fn source(&self) -> Option<&(dyn error::Error + 'static)> {
        match *self {
            DoubleError::EmptyVec => None,
            // 原因采取内部对错误类型的实现。它隐式地转换成了 trait 对象 `&error:Error`。
            // 这可以工作，因为内部的类型已经实现了 `Error` trait。
            DoubleError::Parse(ref e) => Some(e),
        }
    }
}

// 实现从 `ParseIntError` 到 `DoubleError` 的转换。
// 在使用 `?` 时，或者一个 `ParseIntError` 需要转换成 `DoubleError` 时，它会被自动调用。
impl From<ParseIntError> for DoubleError {
    fn from(err: ParseIntError) -> DoubleError {
        DoubleError::Parse(err)
    }
}

fn double_first(vec: Vec<&str>) -> Result<i32> {
    let first = vec.first().ok_or(DoubleError::EmptyVec)?;
    let parsed = first.parse::<i32>()?;

    Ok(2 * parsed)
}
```

> 这个例子过于复杂就是了，相比这里本意



### 4.`?`来处理错误

`?` 之前被解释为要么 `unwrap`，要么 `return Err(err)`，这只是在大多数情况下是正确的。`?` 实际上是指 `unwrap` 或 `return Err(From::from(err))`。由于 `From::from` 是不同类型之间的转换工具，也就是说，如果在错误可转换成返回类型地方使用 `?`，它将自动转换成返回类型。



```rust
use std::error;
use std::fmt;

// 为 `Box<error::Error>` 取别名。
type Result<T> = std::result::Result<T, Box<dyn error::Error>>;

#[derive(Debug)]
struct EmptyVec;

impl fmt::Display for EmptyVec {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "invalid first item to double")
    }
}

impl error::Error for EmptyVec {}

// 这里的结构和之前一样，但是这次没有把所有的 `Result` 和 `Option` 串起来，
// 而是使用 `?` 立即得到内部值。
fn double_first(vec: Vec<&str>) -> Result<i32> {
    let first = vec.first().ok_or(EmptyVec)?;  // ok_or 得到 std::result::Result<&str, EmptyVec>
    // ? 如果不能unwrap时... 则 这里 return 其中的  Err(Box<dyn error::Error>::from(EmptyVec))
    // y: 实在过于灵活
    let parsed = first.parse::<i32>()?;  // 同样的，parse的err也被 return Err(Box<dyn error::Error>::from(err))
    Ok(2 * parsed)
}

fn print(result: Result<i32>) {
    match result {
        Ok(n)  => println!("The first doubled is {}", n),
        Err(e) => println!("Error: {}", e),
    }
}

fn main() {
    let numbers = vec!["42", "93", "18"];
    let empty = vec![];
    let strings = vec!["tofu", "93", "18"];

    print(double_first(numbers));
    print(double_first(empty));
    print(double_first(strings));
}

```

>  y: 这里漏了内容吧 - 不是



