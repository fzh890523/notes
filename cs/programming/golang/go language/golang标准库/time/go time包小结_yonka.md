**time中的时长单位**

> 单位是 nano，一般用 数量 * time.单位，比如 5 * time.Second
>
> ```Go
>     const (
>         Nanosecond  Duration = 1
>         Microsecond          = 1000 * Nanosecond
>         Millisecond          = 1000 * Microsecond
>         Second               = 1000 * Millisecond
>         Minute               = 60 * Second
>         Hour                 = 60 * Minute
>     )
> ```

**Sleep、After、Tick区别**

> Sleep 是立即生效，阻塞当前goroutine
>
> After 是返回一个chan，只有当从该chan读取时才会生效（此时类似Sleep）
>
> Tick 类似After，但是循环的，也即chan可以多次读取（也即After类似一次性的Tick）