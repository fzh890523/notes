# index

- [介绍](#介绍)
- [建议](#建议)
- [happens before](#happens before)
- 同步
  - [初始化](#初始化)
  - [goroutine创建](#goroutine创建)
  - [goroutine销毁](#goroutine销毁)
  - [channel通信](#channel通信)
  - [锁](#锁)
  - [Once](#Once)
- [不正确的同步](#不正确的同步)

# 介绍

go内存模型指定了 *在什么情况下一个协程在读变量时可以保证观察到其他协程（同时）对该变量的修改*

# 建议

如果程序里需要修改同时被多个goroutine访问的数据的话，类似的访问操作需要 *串行化*。

可以通过sync和sync/atomic来达到类似的目的。

# happens before

**背景 - 重排序** 
 重排序的存在： 编译器为了优化执行效率，可能会将指令重排序，也即实际的指令顺序可能和代码里的不一样。比如：

 

```
var a = 1
var b = 1

// 重排后可能是

var b = 1
var a = 1
```



**背景 - happens before** 
 两个e1和e2的相对顺序： 如果e1在e2之前发生（happens before），那么也可以说e2在e1之后发生（happens after）。 而如果e1既没有在e2之前发生也没有在e2之后发生，那么说e1和e2是并发的。 
 上面的理解从**实际发生的顺序**关系来说的。 
 happens before还是一个理解是从**效果**上来说： 通过一些机制使得观察者能**观察到**e1 happens before e2。 --- 一般称为 **happens before rule** 
 如，java中有：

 

```
程序顺序规则：一个线程中的每个操作，happens- before 于该线程中的任意后续操作。

监视器锁规则：对一个监视器锁的解锁，happens- before 于随后对这个监视器锁的加锁。

volatile变量规则：对一个volatile域的写，happens- before 于任意后续对这个volatile域的读。

传递性：如果A happens- before B，且B happens- before C，那么A happens- before C。
```





**happens before顺序** 
 go编译器会在重排序时保证读写操作的相对有序

**单个goroutine内的写操作可见** 
 如果满足下面的条件，则读操作r是可以观察到写操作w对变量v的修改的：

1. r没有在w之前发生
2. 没有其他的写操作发生在w之后以及r之前

也即（**对于单goroutine来说，两组条件等效**）

1. w发生在r之前
2. 其他写操作发生在w之前或者r之后

而在多goroutine情况下，第二组条件实际要强于第一组，因为存在写操作与w或者r是并发的，第一组条件没有将这部分情况排除。 
 需要（通过同步）使得并发的操作变成串行化的来符合条件二。

*变量v初始化为零值的操作也等同于写操作* 
 **大于一个机器字的操作实际表现为多个机器字操作并且顺序未指定**

# 同步

## 初始化

程序的初始化是在单个goroutine里完成的，但这个goroutine可能（允许）创建其他goroutines，那样的话就并发执行了... 
 如：



```
    如果包p导入了包q，那么q的init函数会在p的init之前执行
```





## goroutine创建

**go**语句（statement）会启动一个新的goroutine，该操作会先于该goroutine启动其执行操作（也即是先启动goroutine，然后再是goroutine执行）

*启动的goroutine需要等调度，所以可能go语句后面的语句和go里面的执行内容的执行顺序无法保证* 
 如：

```go
var a string

func f() {
    print(a)
}

func hello() {
    a = "hello, world"
    go f()
}
// 可能print还没来得及执行 hello方法就执行完了
```





## goroutine销毁

同上，goroutine需要等调度，所以不能保证其执行完 与 其他事件 的先后顺序。 
 如：

```go
var a string

func hello() {
    go func() { a = "hello" }()
    print(a)
}
// 可能a还没来得及被赋值就被print了  
```



如果一个goroutine的操作结果必须被另一个goroutine观察到的话，需要使用同步机制，比如lock、channel等，来保持相对的顺序。

## channel通信

channel通信是golang中的主要同步机制，在一个channel上的send都会match到一个receive，当然，一般是不同的goroutine。

**对channel的send 发生在 对应的receive 之前（happens before）** 
 如：

 

```go
var c = make(chan int, 10)
var a string

func f() {
    a = "hello, world"
    c <- 0
}

func main() {
    go f()
    <-c
    print(a)
}
// 通过c来同步操作，确保print在赋值之后
```





**关闭channel的操作 发生在 因为channel被关闭读取到零值 之前（happens before）**

**从一个无buffer的channel receive（返回） 发生在 对应的send结束 之前**

**从一个容量为C的channel 做第k次receive（返回） 发生在 第k+C次send结束 之前** 



 

```go
var limit = make(chan int, 3)

func main() {
    for _, w := range work {
        go func(w func()) {
            limit <- 1
            w()
            <-limit
        }(w)
    }
    select{}
}
// 类似semaphore的实现
```





## 锁

sync包下的**sync.Mutex** 和 **sync.RWMutex** 
 ...



 

```go
var l sync.Mutex
var a string

func f() {
    a = "hello, world"
    l.Unlock()
}

func main() {
    l.Lock()
    go f()
    l.Lock()
    print(a)
}
```



## Once

sync包提供了一种并发（多goroutine）安全的初始化方式： 使用Once类型。 
 once.Do(f)并发执行的时候只会有一次在执行，其他的block至执行结束...（然后返回）。

 

```go
var a string
var once sync.Once

func setup() {
    a = "hello, world"
}

func doprint() {
    once.Do(setup)
    print(a)
}

func twoprint() {
    go doprint()
    go doprint()
}
```





# 不正确的同步



```go
var a, b int

func f() {
    a = 1
    b = 2
}

func g() {
    print(b)
    print(a)
}

func main() {
    go f()
    g()
}
// 可能会发生 2 0 的输出
// 如前面提到的，因为f里没有读写顺序，所以可能导致重排序为 b=2; a=1，然后...
// 此外，还有可能 写了a以后并没有被另一个g观察到... 不过不是本场景
```





上面是 **重排序** 带来的问题，下面是 **缓存未失效** 带来的问题：



 

```go
var a string
var done bool

func setup() {
    a = "hello, world"
    done = true
}

func main() {
    go setup()
    for !done {
    }
    print(a)
}
// 可能需要CPU刷新写缓存（把修改内容写回主存）才会终止循环。
```



```go
type T struct {
    msg string
}

var g *T

func setup() {
    t := new(T)
    t.msg = "hello, world"
    g = t
}

func main() {
    go setup()
    for g == nil {
    }
    print(g.msg)
}
// 同样...
```

