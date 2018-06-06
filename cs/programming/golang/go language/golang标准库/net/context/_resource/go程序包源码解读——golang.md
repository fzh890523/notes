

# go程序包源码解读——golang.org/x/net/contex

** 2015-10-13 16:30  ** xiaohu50

** 阅读 1094 次  ** 0 人喜欢  [** 0 条评论](http://studygolang.com/articles/5131#commentForm)  [** 收藏](http://studygolang.com/articles/5131#)

> 强烈建议读者先完成<http://blog.golang.org/pipelines>的阅读，此文对于go的channel的用法会让初学者有一种豁然开朗的感觉。

# 简介

**context**是一个在go中时常用到的程序包，google官方开发。特别常见的一个应用场景是由一个请求衍生出的各个goroutine之间需要满足一定的约束关系，以实现一些诸如有效期，中止routine树，传递请求全局变量之类的功能。使用context实现上下文功能约定需要在你的方法的传入参数的第一个传入一个**context.Context**类型的变量。我们将通过源代码的阅读和一些示例代码来说明context的用法。

文档在此：<https://godoc.org/golang.org/x/net/context>，另有一篇官方博客<http://blog.golang.org/context>

# 核心接口Context

```
type Context interface {
    Deadline() (deadline time.Time, ok bool)
    Done() <-chan struct{}
    Err() error
    Value(key interface{}) interface{}
}
```

默认错误：

```
var Canceled = errors.New("context canceled")
var DeadlineExceeded = errors.New("context deadline exceeded")
```

空Context：

```
type emptyCtx intfunc (*emptyCtx) Deadline() (deadline time.Time, ok bool) {
    return
}

func (*emptyCtx) Done() <-chan struct{} {
    return nil
}

func (*emptyCtx) Err() error {
    return nil
}

func (*emptyCtx) Value(key interface{}) interface{} {
    return nil
}

func (e *emptyCtx) String() string {
    switch e {
    case background:
        return "context.Background"case todo:
        return "context.TODO"
    }
    return "unknown empty Context"
}
```

包中提供的两个空Context：

```
var (
    background = new(emptyCtx)
    todo       = new(emptyCtx)
)


func Background() Context {
    return background
}

func TODO() Context {
    return todo
}

```

# Cancel

cancelCtx结构体继承了Context，实现了canceler方法：

```
//*cancelCtx 和 *timerCtx 都实现了canceler接口，实现该接口的类型都可以被直接canceledtype canceler interface {
    cancel(removeFromParent bool, err error)
    Done() <-chan struct{}
}


type cancelCtx struct {
    Context
    done chan struct{} // closed by the first cancel call.
    mu       sync.Mutex
    children map[canceler]bool // set to nil by the first cancel call
    err      error             // 当其被cancel时将会把err设置为非nil
}

func (c *cancelCtx) Done() <-chan struct{} {
    return c.done
}

func (c *cancelCtx) Err() error {
    c.mu.Lock()
    defer c.mu.Unlock()
    return c.err
}

func (c *cancelCtx) String() string {
    return fmt.Sprintf("%v.WithCancel", c.Context)
}

//核心是关闭c.done//同时会设置c.err = err, c.children = nil//依次遍历c.children，每个child分别cancel//如果设置了removeFromParent，则将c从其parent的children中删除func (c *cancelCtx) cancel(removeFromParent bool, err error) {
    if err == nil {
        panic("context: internal error: missing cancel error")
    }
    c.mu.Lock()
    if c.err != nil {
        c.mu.Unlock()
        return // already canceled
    }
    c.err = err
    close(c.done)
    for child := range c.children {
        // NOTE: acquiring the child's lock while holding parent's lock.
        child.cancel(false, err)
    }
    c.children = nil
    c.mu.Unlock()

    if removeFromParent {
        removeChild(c.Context, c) // 从此处可以看到 cancelCtx的Context项是一个类似于parent的概念
    }
}
```

再来看一些Cancel相关的方法：

```
type CancelFunc func()

// WithCancel方法返回一个继承自parent的Context对象，同时返回的cancel方法可以用来关闭返回的Context当中的Done channel// 其将新建立的节点挂载在最近的可以被cancel的父节点下（向下方向）// 如果传入的parent是不可被cancel的节点，则直接只保留向上关系func WithCancel(parent Context) (ctx Context, cancel CancelFunc) {
    c := newCancelCtx(parent)
    propagateCancel(parent, &c)
    return &c, func() { c.cancel(true, Canceled) }
}

func newCancelCtx(parent Context) cancelCtx {
    return cancelCtx{
        Context: parent,
        done:    make(chan struct{}),
    }
}

// 传递cancel// 从当前传入的parent开始（包括该parent），向上查找最近的一个可以被cancel的parent// 如果找到的parent已经被cancel，则将方才传入的child树给cancel掉// 否则，将child节点直接连接为找到的parent的children中（Context字段不变，即向上的父亲指针不变，但是向下的孩子指针变直接了）// // 如果没有找到最近的可以被cancel的parent，即其上都不可被cancel，则启动一个goroutine等待传入的parent终止，则cancel传入的child树，或者等待传入的child终结。func propagateCancel(parent Context, child canceler) {
    if parent.Done() == nil {
        return // parent is never canceled
    }
    if p, ok := parentCancelCtx(parent); ok {
        p.mu.Lock()
        if p.err != nil {
            // parent has already been canceled
            child.cancel(false, p.err)
        } else {
            if p.children == nil {
                p.children = make(map[canceler]bool)
            }
            p.children[child] = true
        }
        p.mu.Unlock()
    } else {
        go func() {
            select {
            case <-parent.Done():
                child.cancel(false, parent.Err())
            case <-child.Done():
            }
        }()
    }
}

// 从传入的parent对象开始，依次往上找到一个最近的可以被cancel的对象，即cancelCtx或者timerCtxfunc parentCancelCtx(parent Context) (*cancelCtx, bool) {
    for {
        switch c := parent.(type) {
        case *cancelCtx:
            return c, truecase *timerCtx:
            return &c.cancelCtx, truecase *valueCtx:
            parent = c.Context
        default:
            return nil, false
        }
    }
}

//从parent开始往上找到最近的一个可以cancel的父对象// 从父对象的children map中删除这个childfunc removeChild(parent Context, child canceler) {
    p, ok := parentCancelCtx(parent)
    if !ok {
        return
    }
    p.mu.Lock()
    if p.children != nil {
        delete(p.children, child)
    }
    p.mu.Unlock()
}
```

# Deadline & Timeout

首先来看一个继承自cancelCtx的结构体：

```
type timerCtx struct {
    cancelCtx //此处的封装为了继承来自于cancelCtx的方法，cancelCtx.Context才是父亲节点的指针
    timer *time.Timer // Under cancelCtx.mu. 是一个计时器
    deadline time.Time
}

func (c *timerCtx) Deadline() (deadline time.Time, ok bool) {
    return c.deadline, true
}

func (c *timerCtx) String() string {
    return fmt.Sprintf("%v.WithDeadline(%s [%s])", c.cancelCtx.Context, c.deadline, c.deadline.Sub(time.Now()))
}

// 与cencelCtx有所不同，其除了处理cancelCtx.cancel，还回对c.timer进行Stop()，并将c.timer=nilfunc (c *timerCtx) cancel(removeFromParent bool, err error) {
    c.cancelCtx.cancel(false, err)
    if removeFromParent {
        // Remove this timerCtx from its parent cancelCtx's children.
        removeChild(c.cancelCtx.Context, c)
    }
    c.mu.Lock()
    if c.timer != nil {
        c.timer.Stop()
        c.timer = nil
    }
    c.mu.Unlock()
}

```

由此结构体衍生出的两个方法：

```
func WithDeadline(parent Context, deadline time.Time) (Context, CancelFunc) {
    // 如果parent的deadline比新传入的deadline已经要早，则直接WithCancel，因为新传入的deadline没有效，父亲的deadline会先到期。if cur, ok := parent.Deadline(); ok && cur.Before(deadline) {
        // The current deadline is already sooner than the new one.return WithCancel(parent)
    }
    c := &timerCtx{
        cancelCtx: newCancelCtx(parent),
        deadline:  deadline,
    }
    // 接入树
    propagateCancel(parent, c)

    // 检查如果已经过期，则cancel新的子树
    d := deadline.Sub(time.Now())
    if d <= 0 {
        c.cancel(true, DeadlineExceeded) // deadline has already passedreturn c, func() { c.cancel(true, Canceled) }
    }

    c.mu.Lock()
    defer c.mu.Unlock()
    if c.err == nil {
        // 还没有被cancel的话，就设置deadline之后cancel的计时器
        c.timer = time.AfterFunc(d, func() {
            c.cancel(true, DeadlineExceeded)
        })
    }
    return c, func() { c.cancel(true, Canceled) }
}


// timeout和deadline本质一样
func WithTimeout(parent Context, timeout time.Duration) (Context, CancelFunc) {
    return WithDeadline(parent, time.Now().Add(timeout))
}
```

# Value

valueCtx主要用来传递一些元数据，通过WithValue()来传入继承，通过Value()来读取，简单，不赘述

```
func WithValue(parent Context, key interface{}, val interface{}) Context {
    return &valueCtx{parent, key, val}
}

type valueCtx struct {
    Context
    key, val interface{}
}

func (c *valueCtx) String() string {
    return fmt.Sprintf("%v.WithValue(%#v, %#v)", c.Context, c.key, c.val)
}

func (c *valueCtx) Value(key interface{}) interface{} {
    if c.key == key {
        return c.val
    }
    return c.Context.Value(key)
}

```

# 使用原则：

- Programs
   that use Contexts should follow these rules to keep interfaces 
  consistent across packages and enable static analysis tools to check 
  context propagation: 
  使用Context的程序包需要遵循如下的原则来满足接口的一致性以及便于静态分析
- Do
   not store Contexts inside a struct type; instead, pass a Context 
  explicitly to each function that needs it. The Context should be the 
  first parameter, typically named ctx: 
  不要把Context存在一个结构体当中，显式地传入函数。Context变量需要作为第一个参数使用，一般命名为ctx

```
func DoSomething(ctx context.Context, arg Arg) error {
 ... use ctx ...
 }
```

- Do not pass a nil Context, even if a function permits it. Pass context.TODO if you are unsure about which Context to use. 
  即使方法允许，也不要传入一个nil的Context，如果你不确定你要用什么Context的时候传一个context.TODO
- Use
   context Values only for request-scoped data that transits processes and
   APIs, not for passing optional parameters to functions. 
  使用context的Value相关方法只应该用于在程序和接口中传递的和请求相关的元数据，不要用它来传递一些可选的参数
- The
   same Context may be passed to functions running in different 
  goroutines; Contexts are safe for simultaneous use by multiple 
  goroutines. 
  同样的Context可以用来传递到不同的goroutine中，Context在多个goroutine中是安全的

# 使用要点

其实本身非常简单，在导入这个包之后，初始化Context对象，在每个资源访问方法中都调用它，然后在使用时检查Context对象是否已经被Cancel，如果是就释放绑定的资源。如下所示（下面的代码截取自<http://blog.golang.org/context>中的示例程序）

初始化并设置最终cancel：

```
func handleSearch(w http.ResponseWriter, req *http.Request) {
    // ctx is the Context for this handler. Calling cancel closes the // ctx.Done channel, which is the cancellation signal for requests // started by this handler.
    var (
        ctx    context.Context
        cancel context.CancelFunc
    )
    timeout, err := time.ParseDuration(req.FormValue("timeout"))
    if err == nil {
        // The request has a timeout, so create a context that is // canceled automatically when the timeout expires.
        ctx, cancel = context.WithTimeout(context.Background(), timeout)
    } else {
        ctx, cancel = context.WithCancel(context.Background())
    }
    defer cancel() // Cancel ctx as soon as handleSearch returns.
```

在中间过程传递，并在资源相关操作时判断是否ctx.Done()传出了值，关于Done()的使用应该参考<http://blog.golang.org/pipelines> 
意外终结的程序返回值应该为对应的ctx.Err()

```
func httpDo(ctx context.Context, req *http.Request, f func(*http.Response, error) error) error {
    // Run the HTTP request in a goroutine and pass the response to f.
    tr := &http.Transport{}
    client := &http.Client{Transport: tr}
    c := make(chan error, 1)
    go func() { c <- f(client.Do(req)) }()
    select {
    case <-ctx.Done():
        tr.CancelRequest(req)
        <-c // Wait for f to return.return ctx.Err()
    case err := <-c:
        return err
    }
}
```

版权声明：本文为博主原创文章，未经博主允许不得转载。

本文来自：[CSDN博客](http://studygolang.com/wr?u=http://blog.csdn.net)

感谢作者：xiaohu50

查看原文：[go程序包源码解读——golang.org/x/net/contex](http://studygolang.com/wr?u=http%3a%2f%2fblog.csdn.net%2fxiaohu50%2farticle%2fdetails%2f49100433)

来源： <http://studygolang.com/articles/5131>

