# file



```rust
use std::fs;
use std::fs::{File, OpenOptions};
use std::io;
use std::io::prelude::*;
use std::os::unix;
use std::path::Path;

// `% cat path` 的简单实现
fn cat(path: &Path) -> io::Result<String> {
    let mut f = File::open(path)?;
    let mut s = String::new();
    match f.read_to_string(&mut s) {
        Ok(_) => Ok(s),
        Err(e) => Err(e),
    }
}

// `% echo s > path` 的简单实现
fn echo(s: &str, path: &Path) -> io::Result<()> {
    let mut f = File::create(path)?;

    f.write_all(s.as_bytes())
}

// `% touch path` 的简单实现（忽略已存在的文件）
fn touch(path: &Path) -> io::Result<()> {
    match OpenOptions::new().create(true).write(true).open(path) {
        Ok(_) => Ok(()),
        Err(e) => Err(e),
    }
}

fn main() {
    println!("`mkdir a`");
    // 创建一个目录，返回 `io::Result<()>`
    match fs::create_dir("a") {
        Err(why) => println!("! {:?}", why.kind()),
        Ok(_) => {},
    }

    println!("`echo hello > a/b.txt`");
    // 前面的匹配可以用 `unwrap_or_else` 方法简化
    echo("hello", &Path::new("a/b.txt")).unwrap_or_else(|why| {
        println!("! {:?}", why.kind());
    });

    println!("`mkdir -p a/c/d`");
    // 递归地创建一个目录，返回 `io::Result<()>`
    fs::create_dir_all("a/c/d").unwrap_or_else(|why| {
        println!("! {:?}", why.kind());
    });

    println!("`touch a/c/e.txt`");
    touch(&Path::new("a/c/e.txt")).unwrap_or_else(|why| {
        println!("! {:?}", why.kind());
    });

    println!("`ln -s ../b.txt a/c/b.txt`");
    // 创建一个符号链接，返回 `io::Resutl<()>`
    if cfg!(target_family = "unix") {
        unix::fs::symlink("../b.txt", "a/c/b.txt").unwrap_or_else(|why| {
        println!("! {:?}", why.kind());
        });
    }

    println!("`cat a/c/b.txt`");
    match cat(&Path::new("a/c/b.txt")) {
        Err(why) => println!("! {:?}", why.kind()),
        Ok(s) => println!("> {}", s),
    }

    println!("`ls a`");
    // 读取目录的内容，返回 `io::Result<Vec<Path>>`
    match fs::read_dir("a") {
        Err(why) => println!("! {:?}", why.kind()),
        Ok(paths) => for path in paths {
            println!("> {:?}", path.unwrap().path());
        },
    }

    println!("`rm a/c/e.txt`");
    // 删除一个文件，返回 `io::Result<()>`
    fs::remove_file("a/c/e.txt").unwrap_or_else(|why| {
        println!("! {:?}", why.kind());
    });

    println!("`rmdir a/c/d`");
    // 移除一个空目录，返回 `io::Result<()>`
    fs::remove_dir("a/c/d").unwrap_or_else(|why| {
        println!("! {:?}", why.kind());
    });
}

```





## File



`File` 结构体表示一个被打开的文件（它包裹了一个文件描述符），并赋予了对所表示的 文件的读写能力。

由于在进行文件 I/O（输入/输出）操作时可能出现各种错误，因此 `File` 的所有方法都 返回 `io::Result<T>` 类型，它是 `Result<T, io::Error>` 的别名。

这使得所有 I/O 操作的失败都变成**显式的**。借助这点，程序员可以看到所有的失败 路径，并被鼓励主动地处理这些情形。



## open



`open` 静态方法能够以只读模式（read-only mode）打开一个文件。

> TODO y: 哈？ 

`File` 拥有资源，即文件描述符（file descriptor），它会在自身被 `drop` 时关闭文件。

```rust
use std::fs::File;
use std::io::prelude::*;
use std::path::Path;

fn main() {
    // 创建指向所需的文件的路径
    let path = Path::new("hello.txt");
    let display = path.display();

    // 以只读方式打开路径，返回 `io::Result<File>`
    let mut file = match File::open(&path) {
        // `io::Error` 的 `description` 方法返回一个描述错误的字符串。
        Err(why) => panic!("couldn't open {}: {:?}", display, why),
        Ok(file) => file,
    };

    // 读取文件内容到一个字符串，返回 `io::Result<usize>`
    let mut s = String::new();
    match file.read_to_string(&mut s) {
        Err(why) => panic!("couldn't read {}: {:?}", display, why),
        Ok(_) => print!("{} contains:\n{}", display, s),
    }

    // `file` 离开作用域，并且 `hello.txt` 文件将被关闭。
}
```

下面是所希望的成功的输出：

```bash
$ echo "Hello World!" > hello.txt
$ rustc open.rs && ./open
hello.txt contains:
Hello World!
```



## create

`create` 静态方法以只写模式（write-only mode）打开一个文件。若文件已经存在，则 旧内容将被销毁。否则，将创建一个新文件。



```rust
static LOREM_IPSUM: &'static str =
"Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod
tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam,
quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo
consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse
cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non
proident, sunt in culpa qui officia deserunt mollit anim id est laborum.
";

use std::io::prelude::*;
use std::fs::File;
use std::path::Path;

fn main() {
    let path = Path::new("out/lorem_ipsum.txt");
    let display = path.display();

    // 以只写模式打开文件，返回 `io::Result<File>`
    let mut file = match File::create(&path) {
        Err(why) => panic!("couldn't create {}: {:?}", display, why),
        Ok(file) => file,
    };

    // 将 `LOREM_IPSUM` 字符串写进 `file`，返回 `io::Result<()>`
    match file.write_all(LOREM_IPSUM.as_bytes()) {
        Err(why) => {
            panic!("couldn't write to {}: {:?}", display, why)
        },
        Ok(_) => println!("successfully wrote to {}", display),
    }
}

```







## read



方法 `lines()` 在文件的行上返回一个迭代器。

`File::open` 需要一个泛型 `AsRef<Path>`。这正是 `read_lines()` 期望的输入。

> y: 为什么要写成这样？ `&Path`不行吗？
> 因为不一定要 `&Path` 类型呀，从下面就可以看到了。 可以为各种类型实现 `AsRef<Path>`
> 换句话说，`r = &v` 在rust中的语义不那么纯粹了，具体做什么，要看（可能是推导出的）r的类型

```rust
use std::fs::File;
use std::io::{self, BufRead};
use std::path::Path;

fn main() {
    // 在生成输出之前，文件主机必须存在于当前路径中
    if let Ok(lines) = read_lines("./hosts") {
        // 使用迭代器，返回一个（可选）字符串
        for line in lines {
            if let Ok(ip) = line {
                println!("{}", ip);
            }      
        }   
    }
}

// 输出包裹在 Result 中以允许匹配错误，
// 将迭代器返回给文件行的读取器（Reader）。
fn read_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<File>>>
where P: AsRef<Path>, {
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}
```



> ```rust
>     #[stable(feature = "rust1", since = "1.0.0")]
>     pub fn open<P: AsRef<Path>>(path: P) -> io::Result<File> {
>         OpenOptions::new().read(true).open(path.as_ref())
>     }
> ```
>
> y: `&str` 实现了`AsRef<Path>` ？
>
> ```rust
> #[stable(feature = "rust1", since = "1.0.0")]
> impl AsRef<Path> for String {
>     #[inline]
>     fn as_ref(&self) -> &Path {
>         Path::new(self)
>     }
> }
> 
> #[stable(feature = "rust1", since = "1.0.0")]
> impl AsRef<Path> for str {
>     #[inline]
>     fn as_ref(&self) -> &Path {
>         Path::new(self)
>     }
> }
> ```
>
> 













