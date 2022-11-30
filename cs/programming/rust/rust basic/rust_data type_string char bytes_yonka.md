

## string...



### str

目测跟go的string比较类似。

* utf-8 （byte） sequence
* 长度未知（是指字符数吧，字节数应该是知道的）

差别：
* rust的str似乎都是static，也即运行时生成的字符串是String而不是str
  不可改变。
  String在堆中分配，可扩容/增长。

> In general, you should use String when you need ownership, and &str when you just need to borrow a string.

```rust
let a = "Hello, world."; //a: &'static str
let b: &str = "こんにちは, 世界!";
```



### string format







