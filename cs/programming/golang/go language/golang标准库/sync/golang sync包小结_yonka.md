## Index ▾


#### Examples


#### Package files

[cond.go](https://golang.org/src/sync/cond.go) [mutex.go](https://golang.org/src/sync/mutex.go) [once.go](https://golang.org/src/sync/once.go) [pool.go](https://golang.org/src/sync/pool.go) [race0.go](https://golang.org/src/sync/race0.go) [runtime.go](https://golang.org/src/sync/runtime.go) [rwmutex.go](https://golang.org/src/sync/rwmutex.go) [waitgroup.go](https://golang.org/src/sync/waitgroup.go)

来源： <<https://golang.org/pkg/sync/>>

 

------

**WaitGroup**

> 多用于在主goroutine里阻塞等待子goroutine完成
>
> 如：
>
> ```Go
>     package main
>     import (
>     	"fmt"
>     	"sync"
>     )
>     var waitgroup sync.WaitGroup
>     func Afunction(shownum int) {
>     	fmt.Println(shownum)
>     	waitgroup.Done() //任务完成，将任务队列中的任务数量-1，其实.Done就是.Add(-1)
>     }
>     func main() {
>     	for i := 0; i < 10; i++ {
>     		waitgroup.Add(1) //每创建一个goroutine，就把任务队列中任务的数量+1
>     		go Afunction(i)
>     	}
>     	waitgroup.Wait() //.Wait()这里会发生阻塞，直到队列中所有的任务结束就会解除阻塞
>     }
> ```
>
> 好处： 简单便捷
>
> 缺点： 不够灵活，比如超时等待等
>
> 可以考虑用chan自行实现类似功能

------

**Cond**