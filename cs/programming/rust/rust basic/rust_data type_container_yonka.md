### Vectors 

* 可扩容
* 类型需要一致
* 泛型类型，元素类型可以任意
  `Vec<T>`，如 `Vec<i32>`
* 从支持的语义操作，vec更像完整功能的go slice/java list
  * A pointer to the data
  * No of elements currently have(length)
  * Capacity (Amount of space allocated for any future elements).
  

```rust
let mut a = Vec::new(); //1.With new() keyword
let mut b = vec![]; //2.Using the vec! macro
// 这种非泛型形式的vec可以塞什么？


let mut a2: Vec<i32> = Vec::new();
let mut b2: Vec<i32> = vec![];
let mut b3 = vec![1i32, 2, 3];//Sufixing 1st value with data type

let mut b4 = vec![1, 2, 3];
let mut b5: Vec<i32> = vec![1, 2, 3];
let mut b6  = vec![1i32, 2, 3];  // 元素类型的标识法： 1i32 囧
let mut b7 = vec![0; 10]; //Ten zeroes



//Accessing and changing existing data
let mut c = vec![5, 4, 3, 2, 1];
c[0] = 1;
c[1] = 2;
//c[6] = 2; Cannot assign values this way, index out of bounds
println!("{:?}", c); //[1, 2, 3, 2, 1]

//push and pop
let mut d: Vec<i32> = Vec::new();
d.push(1); //[1] : Add an element to the end
d.push(2); //[1, 2]
d.pop(); //[1] : : Remove an element from the end


// 🔎 Capacity and reallocation
let mut e: Vec<i32> = Vec::with_capacity(10);
println!("Length: {}, Capacity : {}", e.len(), e.capacity()); //Length: 0, Capacity : 10

// These are all done without reallocating...
for i in 0..10 {
    e.push(i);
}
// ...but this may make the vector reallocate
e.push(11);
```

```rust
let mut v = vec![1, 2, 3, 4, 5];

for i in &v {
    println!("A reference to {}", i);
}

for i in &mut v {
    println!("A mutable reference to {}", i);
}

for i in v {
    println!("Take ownership of the vector and its element {}", i);
}
```