

# [Go 语言的并发模型--通过通信来共享内存](http://se77en.cc/2014/04/08/share-by-communicating-the-concurrency-slogan-in-golang/)

By [Damon Zhao](https://plus.google.com//u/0/102193487255973478115?rel=author)

 4月 8 2014 更新日期:5月 25 2015

文章目录

1. [1. 概述](http://se77en.cc/2014/04/08/share-by-communicating-the-concurrency-slogan-in-golang/#概述)
2. [2. 前提](http://se77en.cc/2014/04/08/share-by-communicating-the-concurrency-slogan-in-golang/#前提)
3. [3. 问题](http://se77en.cc/2014/04/08/share-by-communicating-the-concurrency-slogan-in-golang/#问题)
4. 4. 解决办法
   1. [4.1. 共享内存的解决方案](http://se77en.cc/2014/04/08/share-by-communicating-the-concurrency-slogan-in-golang/#共享内存的解决方案)
   2. [4.2. 通过通信的解决方案](http://se77en.cc/2014/04/08/share-by-communicating-the-concurrency-slogan-in-golang/#通过通信的解决方案)
   3. [4.3. 通过通信来共享内存是如何工作的](http://se77en.cc/2014/04/08/share-by-communicating-the-concurrency-slogan-in-golang/#通过通信来共享内存是如何工作的)
   4. [4.4. 附属卡的流程](http://se77en.cc/2014/04/08/share-by-communicating-the-concurrency-slogan-in-golang/#附属卡的流程)
   5. [4.5. 控制流程](http://se77en.cc/2014/04/08/share-by-communicating-the-concurrency-slogan-in-golang/#控制流程)
5. [5. 总结](http://se77en.cc/2014/04/08/share-by-communicating-the-concurrency-slogan-in-golang/#总结)

## 概述

我一直在找一种好的方法来解释 go 语言的[并发模型](http://golang.org/doc/effective_go.html#concurrency)：

> 不要通过共享内存来通信，相反，应该通过通信来共享内存

但是没有发现一个好的解释来满足我下面的需求：

- 通过一个例子来说明最初的问题
- 提供一个共享内存的解决方案
- 提供一个通过通信的解决方案

这篇文章我就从这三个方面来做出解释。

读过这篇文章后你应该会了解**通过通信来共享内存**的模型，以及它和**通过共享内存来通信**的区别，你还将看到如何分别通过这两种模型来解决访问和修改共享资源的问题。

## 前提

设想一下我们要访问一个银行账号：

```
type Account interface {
  Withdraw(uint)
  Deposit(uint)
  Balance() int
}

type Bank struct {
  account Account
}

func NewBank(account Account) *Bank {
  return &Bank{account: account}
}

func (bank *Bank) Withdraw(amount uint, actor_name string) {
  fmt.Println("[-]", amount, actor_name)
  bank.account.Withdraw(amount)
}

func (bank *Bank) Deposit(amount uint, actor_name string) {
  fmt.Println("[+]", amount, actor_name)
  bank.account.Deposit(amount)
}

func (bank *Bank) Balance() int {
  return bank.account.Balance()
}
```

因为 `Account` 是一个接口，所以我们提供一个简单的实现：

```
type SimpleAccount struct{
  balance int
}

func NewSimpleAccount(balance int) *SimpleAccount {
  return &SimpleAccount{balance: balance}
}

func (acc *SimpleAccount) Deposit(amount uint) {
  acc.setBalance(acc.balance + int(amount))
}

func (acc *SimpleAccount) Withdraw(amount uint) {
  if acc.balance >= int(amount) {
    acc.setBalance(acc.balance - int(amount))
  } else {
    panic("杰克穷死")
  }
}

func (acc *SimpleAccount) Balance() int {
  return acc.balance
}

func (acc *SimpleAccount) setBalance(balance int) {
  acc.add_some_latency()  //增加一个延时函数，方便演示
  acc.balance = balance
}

func (acc *SimpleAccount) add_some_latency() {
  <-time.After(time.Duration(rand.Intn(100)) * time.Millisecond)
}
```

你可能注意到了 `balance` 没有被直接修改，而是被放到了 `setBalance` 方法里进行修改。这样设计是为了更好的描述问题。稍后我会做出解释。

把上面所有部分弄好以后我们就可以像下面这样使用它啦：

```
func main() {
  balance := 80
  b := NewBank(NewSimpleAccount(balance))
  
  fmt.Println("初始化余额", b.Balance())
  
  b.Withdraw(30, "马伊琍")
  
  fmt.Println("-----------------")
  fmt.Println("剩余余额", b.Balance())
}
```

运行上面的代码会输出：

```
初始化余额 80
[-] 30 马伊琍
-----------------
剩余余额 50
```

没错！

不错在现实生活中，一个银行账号可以有很多个附属卡，不同的附属卡都可以对同一个账号进行存取钱，所以我们来修改一下代码：

```
func main() {
  balance := 80
  b := NewBank(NewSimpleAccount(balance))
  
  fmt.Println("初始化余额", b.Balance())
  
  done := make(chan bool)
  
  go func() { b.Withdraw(30, "马伊琍"); done <- true }()
  go func() { b.Withdraw(10, "姚笛"); done <- true }()
  
  //等待 goroutine 执行完成
  <-done
  <-done
  
  fmt.Println("-----------------")
  fmt.Println("剩余余额", b.Balance())
}
```

这儿两个附属卡并发的从账号里取钱，来看看输出结果：

```
初始化余额 80
[-] 30 马伊琍
[-] 10 姚笛
-----------------
剩余余额 70
```

这下把文章高兴坏了:)

结果当然是错误的，剩余余额应该是40而不是70，那么让我们看看到底哪儿出问题了。

## 问题

当并发访问共享资源时，无效状态有很大可能会发生。

在我们的例子中，当两个附属卡同一时刻从同一个账号取钱后，我们最后得到银行账号(即共享资源)错误的剩余余额(即无效状态)。

我们来看一下执行时候的情况：

```
                处理情况
            --------------
            _马伊琍_|_姚笛_
1. 获取余额     80  |  80
2. 取钱       -30  | -10
3. 当前剩余     50  |  70
               ... | ...
4. 设置余额     50  ?  70  //该先设置哪个好呢？
5. 后设置的生效了
            --------------
6. 剩余余额        70
```

上面 `...` 的地方描述了我们 `add_some_latency` 实现的延时状况，现实世界经常发生延迟情况。所以最后的剩余余额就由最后设置余额的那个附属卡决定。

## 解决办法

我们通过两种方法来解决这个问题：

- 共享内存的解决方案
- 通过通信的解决方案

所有的解决方案都是简单的封装了一下 `SimpleAccount` 来实现保护机制。

### 共享内存的解决方案

又叫 “通过共享内存来通信”。

这种方案暗示了使用锁机制来预防同时访问和修改共享资源。锁告诉其它处理程序这个资源已经被一个处理程序占用了，因此别的处理程序需要排队直到当前处理程序处理完毕。

让我们来看看 `LockingAccount` 是怎么实现的：

```
type LockingAccount struct {
  lock    sync.Mutex
  account *SimpleAccount
}

//封装一下 SimpleAccount
func NewLockingAccount(balance int) *LockingAccount {
  return &LockingAccount{account: NewSimpleAccount(balance)}
}

func (acc *LockingAccount) Deposit(amount uint) {
  acc.lock.Lock()
  defer acc.lock.Unlock()
  acc.account.Deposit(amount)
}

func (acc *LockingAccount) Withdraw(amount uint) {
  acc.lock.Lock()
  defer acc.lock.Unlock()
  acc.account.Withdraw(amount)
}

func (acc *LockingAccount) Balance() int {
  acc.lock.Lock()
  defer acc.lock.Unlock()
  return acc.account.Balance()
}
```

直接明了！注意 `lock sync.Lock`，`lock.Lock()`，`lock.Unlock()`。

这样每次一个附属卡访问银行账号(即共享资源)，这个附属卡会自动获得锁直到最后操作完毕。

我们的 `LockingAccount` 像下面这样使用：

```
func main() {
  balance := 80
  b := NewBank(NewLockingAccount(balance))
  
  fmt.Println("初始化余额", b.Balance())
  
  done := make(chan bool)
  
  go func() { b.Withdraw(30, "马伊琍"); done <- true }()
  go func() { b.Withdraw(10, "姚笛"); done <- true }()
  
  //等待 goroutine 执行完成
  <-done
  <-done
  
  fmt.Println("-----------------")
  fmt.Println("剩余余额", b.Balance())
}
```

输出的结果是：

```
初始化余额 80
[-] 30 马伊琍
[-] 10 姚笛
-----------------
剩余余额 40
```

现在结果正确了！

在这个例子中第一个处理程序加锁后独享共享资源，其它处理程序只能等待它执行完成。

我们接着看一下执行时的情况，假设马伊琍先拿到了锁：

```
                    处理过程
                ________________
                _马伊琍_|__姚笛__
加锁                   ><
得到余额            80  |
取钱               -30  |
当前余额            50  |
                   ... |
设置余额            50  |
解除锁                 <>
                       |
当前余额                50
                       |
加锁                   ><
得到余额                |  50
取钱                    | -10
当前余额                |  40
                       |  ...
设置余额                |  40
解除锁                  <>
                ________________
剩余余额                40
```

现在我们的处理程序在访问共享资源时相继的产生了正确的结果。

### 通过通信的解决方案

又叫 “通过通信来共享内存”。

现在账号被命名为 `ConcurrentAccount`，像下面这样来实现：

```
type ConcurrentAccount struct {
  account     *SimpleAccount
  deposits    chan uint
  withdrawals chan uint
  balances    chan chan int
}

func NewConcurrentAccount(amount int) *ConcurrentAccount{
  acc := &ConcurrentAccount{
    account :    &SimpleAccount{balance: amount},
    deposits:    make(chan uint),
    withdrawals: make(chan uint),
    balances:    make(chan chan int),
  }
  acc.listen()
  
  return acc
}

func (acc *ConcurrentAccount) Balance() int {
  ch := make(chan int)
  acc.balances <- ch
  return <-ch
}

func (acc *ConcurrentAccount) Deposit(amount uint) {
  acc.deposits <- amount
}

func (acc *ConcurrentAccount) Withdraw(amount uint) {
  acc.withdrawals <- amount
}

func (acc *ConcurrentAccount) listen() {
  go func() {
    for {
      select {
      case amnt := <-acc.deposits:
        acc.account.Deposit(amnt)
      case amnt := <-acc.withdrawals:
        acc.account.Withdraw(amnt)
      case ch := <-acc.balances:
        ch <- acc.account.Balance()
      }
    }
  }()
}
```

`ConcurrentAccount` 同样封装了 `SimpleAccount` ，然后增加了通信通道

调用代码和加锁版本的一样，这里就不写了，唯一不一样的就是初始化银行账号的时候：

```
b := NewBank(NewConcurrentAccount(balance))
```

运行产生的结果和加锁版本一样：

```
初始化余额 80
[-] 30 马伊琍
[-] 10 姚笛
-----------------
剩余余额 40
```

让我们来深入了解一下细节。

### 通过通信来共享内存是如何工作的

一些基本注意点：

- 共享资源被封装在一个控制流程中。
  结果就是资源成为了非共享状态。没有处理程序能够直接访问或者修改资源。你可以看到访问和修改资源的方法实际上并没有执行任何改变。

```
func (acc *ConcurrentAccount) Balance() int {
  ch := make(chan int)
  acc.balances <- ch
  balance := <-ch
  return balance
}
func (acc *ConcurrentAccount) Deposit(amount uint) {
  acc.deposits <- amount
}

func (acc *ConcurrentAccount) Withdraw(amount uint) {
  acc.withdrawals <- amount
}
```

- 访问和修改是通过消息和控制流程通信。
- 在控制流程中任何访问和修改的动作都是相继发生的。
  当控制流程接收到访问或者修改的请求后会立即执行相关动作。让我们仔细看看这个流程：

```
func (acc *ConcurrentAccount) listen() {
  // 执行控制流程
  go func() {
    for {
      select {
      case amnt := <-acc.deposits:
        acc.account.Deposit(amnt)
      case amnt := <-acc.withdrawals:
        acc.account.Withdraw(amnt)
      case ch := <-acc.balances:
        ch <- acc.account.Balance()
      }
    }
  }()
}
```

[select](http://golang.org/ref/spec#Select_statements) 不断地从各个通道中取出消息，每个通道都跟它们所要执行的操作相一致。

重要的一点是：在 `select` 声明内部的一切都是相继执行的(在同一个处理程序中排队执行)。一次只有一个事件(在通道中接受或者发送)发生，这样就保证了同步访问共享资源。

领会这个有一点绕。

让我们用例子来看看 `Balance()` 的执行情况：

```
        一张附属卡的流程      |   控制流程 
     ----------------------------------------------

1.     b.Balance()         |
2.             ch -> [acc.balances]-> ch
3.             <-ch        |  balance = acc.account.Balance()
4.     return  balance <-[ch]<- balance
5                          |
```

这两个流程都干了点什么呢？

### 附属卡的流程

1. 调用 `b.Balance()`
2. 新建通道 `ch`，将 `ch` 通道塞入通道 `acc.balances` 中与控制流程通信，这样控制流程也可以通过 `ch` 来返回余额
3. 等待 `<-ch` 来取得要接受的余额
4. 接受余额
5. 继续

### 控制流程

1. 空闲或者处理
2. 通过 `acc.balances` 通道里面的 `ch` 通道来接受余额请求
3. 取得真正的余额值
4. 将余额值发送到 `ch` 通道
5. 准备处理下一个请求

控制流程每次只处理一个 `事件`。这也就是为什么除了描述出来的这些以外，第2-4步没有别的操作执行。

## 总结

这篇博客描述了问题以及问题的解决办法，但那时没有深入去探究不同解决办法的优缺点。

其实这篇文章的例子更适合用 mutex，因为这样代码更加清晰。

最后，请毫无顾忌的指出我的错误！

[golang](http://se77en.cc/tags/golang/)[concurrency](http://se77en.cc/tags/concurrency/)

 [golang](http://se77en.cc/categories/golang/)

****![img](http://s.jiathis.com/qrcode.php?url=http%3A%2F%2Fse77en.cc%2F2014%2F04%2F08%2Fshare-by-communicating-the-concurrency-slogan-in-golang%2F)

[**PREVIOUS:** 基于 Martini 的跨域资源共享(CORS)](http://se77en.cc/2014/04/09/cors-with-martini-and-golang/)[**NEXT:**使用 Koa 从零打造 TODO 应用 ](http://se77en.cc/2014/02/05/koa-zero-to-todo-list-translation/)

[最新]()[最早]()[最热]()

- [12条评论](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/cc171eb7-c3de-44f0-ab9c-fd0be841b08f/)
- [9条新浪微博](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/cc171eb7-c3de-44f0-ab9c-fd0be841b08f/)


- [![A-limon]()](http://weibo.com/heidan)

  [A-limon](http://weibo.com/heidan)

  不明觉厉

  2014年4月9日[回复](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/cc171eb7-c3de-44f0-ab9c-fd0be841b08f/)[顶](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/cc171eb7-c3de-44f0-ab9c-fd0be841b08f/)[转发](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/cc171eb7-c3de-44f0-ab9c-fd0be841b08f/)

- ![wayne]()

  wayne

  请原谅我爆粗口，写的真tmd太好了，通俗易懂。

  2014年4月10日[回复](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/cc171eb7-c3de-44f0-ab9c-fd0be841b08f/)[顶](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/cc171eb7-c3de-44f0-ab9c-fd0be841b08f/)[转发](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/cc171eb7-c3de-44f0-ab9c-fd0be841b08f/)

- [![bobozhengsir](http://tp4.sinaimg.cn/1792074827/50/5645442834/1)](http://weibo.com/bobosir)

  [bobozhengsir](http://weibo.com/bobosir)

  一点小错误：第二段代码14行的变量名写错了mount -----》amount

  2014年8月4日[回复](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/cc171eb7-c3de-44f0-ab9c-fd0be841b08f/)[顶](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/cc171eb7-c3de-44f0-ab9c-fd0be841b08f/)[转发](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/cc171eb7-c3de-44f0-ab9c-fd0be841b08f/)

- [![Icecream_Android](http://tp1.sinaimg.cn/1689748412/50/1300160392/1)](http://weibo.com/shenggxhz)

  [Icecream_Android](http://weibo.com/shenggxhz)

  balances chan chan int 为什么在函数 func (acc *ConcurrentAccount) Balance() int 里，可以直接把chan int传给chan chan int类型呢？

  2014年8月16日[回复](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/cc171eb7-c3de-44f0-ab9c-fd0be841b08f/)[顶](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/cc171eb7-c3de-44f0-ab9c-fd0be841b08f/)[转发](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/cc171eb7-c3de-44f0-ab9c-fd0be841b08f/)

- ![凯风](http://q.qlogo.cn/qqapp/100229475/57710B30EC6E091D3065F26CB90B8C1F/100)

  凯风

  对go还在熟悉阶段, 不是很了解switch的执行. 使用for, switch不会导致一直执行, 从而导致CPU占用非常高么?

  2014年9月27日[回复](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/cc171eb7-c3de-44f0-ab9c-fd0be841b08f/)[顶](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/cc171eb7-c3de-44f0-ab9c-fd0be841b08f/)[转发](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/cc171eb7-c3de-44f0-ab9c-fd0be841b08f/)

- [![AriesDevil](http://cn.gravatar.com/avatar/5ce4735b776f58a0a48a74ef03fe5e68?s=50)](http://se77en.cc/)

  [AriesDevil](http://se77en.cc/)

  [回复 Icecream_Android: ]()不是传给，是把 chan int 塞到了 chan chan int 里，你把 chan int 看做一个整体，比如叫 X，相当于把 X 塞到了 chan X 中

  2014年11月27日[回复](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/cc171eb7-c3de-44f0-ab9c-fd0be841b08f/)[顶](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/cc171eb7-c3de-44f0-ab9c-fd0be841b08f/)[转发](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/cc171eb7-c3de-44f0-ab9c-fd0be841b08f/)

- [![AriesDevil](http://cn.gravatar.com/avatar/5ce4735b776f58a0a48a74ef03fe5e68?s=50)](http://se77en.cc/)

  [AriesDevil](http://se77en.cc/)

  [回复 bobozhengsir: ]()已修复

  2014年11月27日[回复](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/cc171eb7-c3de-44f0-ab9c-fd0be841b08f/)[顶](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/cc171eb7-c3de-44f0-ab9c-fd0be841b08f/)[转发](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/cc171eb7-c3de-44f0-ab9c-fd0be841b08f/)

- [![AriesDevil](http://cn.gravatar.com/avatar/5ce4735b776f58a0a48a74ef03fe5e68?s=50)](http://se77en.cc/)

  [AriesDevil](http://se77en.cc/)

  [回复 凯风: ]()不会，只是监听器的作用

  2014年11月27日[回复](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/cc171eb7-c3de-44f0-ab9c-fd0be841b08f/)[顶](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/cc171eb7-c3de-44f0-ab9c-fd0be841b08f/)[转发](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/cc171eb7-c3de-44f0-ab9c-fd0be841b08f/)

- ![xkey](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/cc171eb7-c3de-44f0-ab9c-fd0be841b08f/index_files/de1f70f2-61a5-468c-98a4-65c056f7f636.png)

  xkey

  b := NewBank(bank.NewSimpleAccount(balance)),这里写错了吧，应该去掉bank吧

  4月17日[回复](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/cc171eb7-c3de-44f0-ab9c-fd0be841b08f/)[顶](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/cc171eb7-c3de-44f0-ab9c-fd0be841b08f/)[转发](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/cc171eb7-c3de-44f0-ab9c-fd0be841b08f/)

- [![谢晞鸣_Jeremy](http://tp1.sinaimg.cn/1780975572/50/5697365439/1)](http://weibo.com/fdx321)

  [谢晞鸣_Jeremy](http://weibo.com/fdx321)

  ![[good]](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/cc171eb7-c3de-44f0-ab9c-fd0be841b08f/index_files/b59b9920-3d3b-42ea-8637-1c5f5e3de466.gif)

  5月24日[回复](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/cc171eb7-c3de-44f0-ab9c-fd0be841b08f/)[顶](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/cc171eb7-c3de-44f0-ab9c-fd0be841b08f/)[转发](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/cc171eb7-c3de-44f0-ab9c-fd0be841b08f/)

- [![AriesDevil](http://cn.gravatar.com/avatar/5ce4735b776f58a0a48a74ef03fe5e68?s=50)](http://se77en.cc/)

  [AriesDevil](http://se77en.cc/)

  [回复 xkey: ]()已修复，谢谢指出~

  5月25日[回复](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/cc171eb7-c3de-44f0-ab9c-fd0be841b08f/)[顶](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/cc171eb7-c3de-44f0-ab9c-fd0be841b08f/)[转发](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/cc171eb7-c3de-44f0-ab9c-fd0be841b08f/)

- ![kingluo](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/cc171eb7-c3de-44f0-ab9c-fd0be841b08f/index_files/de1f70f2-61a5-468c-98a4-65c056f7f636.png)

  kingluo

  通过通信来共享内容，其实跟erlang里面的gen_server behavior是一致的原理。

  ​

  来源： <<http://se77en.cc/2014/04/08/share-by-communicating-the-concurrency-slogan-in-golang/>>

   

