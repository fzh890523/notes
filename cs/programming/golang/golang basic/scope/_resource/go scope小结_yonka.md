

一个作用域的坑

> ```
> func TestLoopScope() {
>        type A struct {
>               I int
>        }
>        var a *A
>        for {
>               fmt.Printf("a is nil: %v\n", a == nil)
>               a, err := &A{1}, errors.New("test err")
>               fmt.Printf("a is %v, err is %v\n", a, err)
>        }
> }
> // output:
> /*
> a is nil: true
> a is &{1}, err is test err
> a is nil: true
> a is &{1}, err is test err
> ...
> */
> ```

> 如果是：

> ```golang
> var a *A
> var err error
> fmt.Printf(...)
> a, err := ...
> ```

> 的话就很清楚了。
>
> 实际应该是在新的作用域的新 a，在每次循环时被清空...。



