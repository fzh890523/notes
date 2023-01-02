

# 线程

Rust 通过 `spawn` 函数提供了创建本地操作系统（native OS）线程的机制，该函数的参数是一个通过值捕获变量的闭包（moving closure）。

```rust
use std::thread;

static NTHREADS: i32 = 10;

// 这是主（`main`）线程
fn main() {
    // 提供一个 vector 来存放所创建的子线程（children）。
    let mut children = vec![];

    for i in 0..NTHREADS {
        // 启动（spin up）另一个线程
        children.push(thread::spawn(move || {
            println!("this is thread number {}", i)
        }));
    }

    for child in children {
        // 等待线程结束。返回一个结果。
        let _ = child.join();
    }
}
```

这些线程由操作系统调度（schedule）。



## （多线程）数据竞争



标准库提供了开箱即用的线程类型，把它和 Rust 的所有权概念与别名规则结合 起来，可以自动地避免数据竞争（data race）。

当某状态对某线程是可见的，别名规则（即一个可变引用 XOR 一些只读引用。译注：XOR 是异或的意思，即「二者仅居其一」）就自动地避免了别的线程对它的操作。（当需要同步 处理时，请使用 `Mutex` 或 `Channel` 这样的同步类型。）

> 注意到，虽然我们在线程之间传递了引用，但 Rust 理解我们是在传递只读的引用，因此 不会发生数据竞争等不安全的事情。另外，因为我们把数据块 `move` 到了线程中，Rust 会保证数据存活至线程退出，因此不会产生悬挂指针。



```rust
    // 对分段的数据进行迭代。
    // .enumerate() 会把当前的迭代计数与被迭代的元素以元组 (index, element)
    // 的形式返回。接着立即使用 “解构赋值” 将该元组解构成两个变量，
    // `i` 和 `data_segment`。
    for (i, data_segment) in chunked_data.enumerate() {
        println!("data segment {} is \"{}\"", i, data_segment);

        // 用单独的线程处理每一段数据
        //
        // spawn() 返回新线程的句柄（handle），我们必须拥有句柄，
        // 才能获取线程的返回值。
        //
        // 'move || -> u32' 语法表示该闭包：
        // * 没有参数（'||'）
        // * 会获取所捕获变量的所有权（'move'）
        // * 返回无符号 32 位整数（'-> u32'）
        //
        // Rust 可以根据闭包的内容推断出 '-> u32'，所以我们可以不写它。
        //
        // 试一试：删除 'move'，看看会发生什么
        // y: 会borrow...，而i无法borrorw，data_segment因为本来就是ref，所以可以...
        children.push(thread::spawn(move || -> u32 {
            // 计算该段的每一位的和：
            let result = data_segment
                        // 对该段中的字符进行迭代..
                        .chars()
                        // ..把字符转成数字..
                        .map(|c| c.to_digit(10).expect("should be a digit"))
                        // ..对返回的数字类型的迭代器求和
                        .sum();

            // println! 会锁住标准输出，这样各线程打印的内容不会交错在一起
            println!("processed segment {}, result={}", i, result);

            // 不需要 “return”，因为 Rust 是一种 “表达式语言”，每个代码块中
            // 最后求值的表达式就是代码块的值。
            result

        }));
    }
```



## （线程间）通信



### channel

Rust 为线程之间的通信提供了异步的通道（`channel`）。通道允许两个端点之间信息的 单向流动：`Sender`（发送端） 和 `Receiver`（接收端）。



```rust
use std::sync::mpsc::{Sender, Receiver};
use std::sync::mpsc;
use std::thread;

static NTHREADS: i32 = 3;

fn main() {
    // 通道有两个端点：`Sender<T>` 和 `Receiver<T>`，其中 `T` 是要发送
    // 的消息的类型（类型标注是可选的）
    let (tx, rx): (Sender<i32>, Receiver<i32>) = mpsc::channel();

    for id in 0..NTHREADS {
        // sender 端可被复制
        let thread_tx = tx.clone();

        // 每个线程都将通过通道来发送它的 id
        thread::spawn(move || {
            // 被创建的线程取得 `thread_tx` 的所有权
            // 每个线程都把消息放在通道的消息队列中
            thread_tx.send(id).unwrap();

            // 发送是一个非阻塞（non-blocking）操作，线程将在发送完消息后
            // 会立即继续进行
            println!("thread {} finished", id);
        });
    }

    // 所有消息都在此处被收集
    let mut ids = Vec::with_capacity(NTHREADS as usize);
    for _ in 0..NTHREADS {
        // `recv` 方法从通道中拿到一个消息
        // 若无可用消息的话，`recv` 将阻止当前线程
        ids.push(rx.recv());
    }

    // 显示消息被发送的次序
    println!("{:?}", ids);
}

```







