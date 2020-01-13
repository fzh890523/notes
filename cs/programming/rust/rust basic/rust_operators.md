

## 算术运算符 arithmetic

* `+`
  除了数字以外，还可以用于 array, string 的拼接。
* `-`
* `*`
* `/`
* `%`

```rust
let a = 5;
let b = a + 1; //6
let c = a - 1; //4
let d = a * 2; //10
let e = a / 2; // ⭐️ 2 not 2.5
let f = a % 2; //1

let g = 5.0 / 2.0; //2.5
```

## 比较运算符 comparison

* `==`
* `!=`
* `<`
* `>`
* `<=`
* `>=`

```rust
let a = 1;
let b = 2;

let c = a == b; //false
let d = a != b; //true
let e = a < b; //true
let f = a > b; //false
let g = a <= a; //true
let h = a >= a; //true

// 🔎
let i = true > false; //true
let j = 'a' > 'A'; //true
```

## 逻辑运算符 logical

* `!`
  可以对int操作： 翻转每个bit
  ```rust
  let a = !-2; //1
  let b = !-1; //0
  let c = !0; //-1
  let d = !1; //-2
  ```
* `&&`
* `||`

```rust
let a = true;
let b = false;

let c = !a; //false
let d = a && b; //false
let e = a || b; //true
```

## 位运算符 bitwise

* `&`
* `|`
* `^`
* `<<`
* `>>`

```rust
let a = 1;
let b = 2;

let c = a & b;  //0  (01 && 10 -> 00)
let d = a | b;  //3  (01 || 10 -> 11)
let e = a ^ b;  //3  (01 != 10 -> 11)
let f = a << b; //4  (Add b number of 0s to the end of a -> '01'+'00' -> 100)
let g = a >> b; //0  (Remove b number of bits from the end of a -> o̶1̶ -> 0)
```

## 赋值运算符 assignment and compound assignment



复合赋值运算符： 以下运算符 加上 赋值运算符 组成。
* `+`
* `-`
* `*`
* `/`
* `%`
* `&`
* `|`
* `^`
* `<<` 
* `>>`

> 全部的算术运算符 和 全部的位运算符

```rust
let mut a = 2;

a += 5; //2 + 5 = 7
a -= 2; //7 - 2 = 5
a *= 5; //5 * 5 = 25
a /= 2; //25 / 2 = 12 not 12.5
a %= 5; //12 % 5 = 2

a &= 2; //10 && 10 -> 10 -> 2
a |= 5; //010 || 101 -> 111 -> 7
a ^= 2; //111 != 010 -> 101 -> 5
a <<= 1; //'101'+'0' -> 1010 -> 10
a >>= 2; //101̶0̶ -> 10 -> 2
```

## 类型转换运算符 type casting

`as`

```rust
let a = 15;
let b = (a as f64) / 2.0; //7.5
```

## 借（所有权）、解引用 运算符

borrowing、dereference

* `&`
* `&mut`
* `*`
  解引用

> 所以 `借` 等于取引用？
