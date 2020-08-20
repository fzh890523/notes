
* 数据（struct） 和 方法（impl） 分离
  * struct -类比-> class （但没方法）
  * impl -类比-> method；
    ```rust
    impl ${structName} {
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