

# 使用技巧

## 等待其他goroutine结束

1、sleep

> 粗糙

> ```Go
>     func main() {
>         messages := make(chan int)
>         go func() {
>             time.Sleep(time.Second * 3)
>             messages <- 1
>         }()
>         go func() {
>             time.Sleep(time.Second * 2)
>             messages <- 2
>         }() 
>         go func() {
>             time.Sleep(time.Second * 1)
>             messages <- 3
>         }()
>         go func() {
>             for i := range messages {
>                 fmt.Println(i)
>             }
>         }()
>         time.Sleep(time.Second * 5)
>     }
> ```

2、用chan

> 可靠，但代码略累赘

> ```Go
>     func main() {
>         messages := make(chan int)
>         // Use this channel to follow the execution status
>         // of our goroutines :D
>         done := make(chan bool)
>         go func() {
>             time.Sleep(time.Second * 3)
>             messages <- 1
>             done <- true
>         }()
>         go func() {
>             time.Sleep(time.Second * 2)
>             messages <- 2
>             done <- true
>         }() 
>         go func() {
>             time.Sleep(time.Second * 1)
>             messages <- 3
>             done <- true
>         }()
>         go func() {
>             for i := range messages {
>                 fmt.Println(i)
>             }
>         }()
>         for i := 0; i < 3; i++ {
>             <-done
>         }
>     }
> ```

3、标准方法： sync.WaitGroup

> ```Go
>     import (
>         "fmt"
>         "sync"
>         "time"
>     )
>     func main() {
>         messages := make(chan int)
>         var wg sync.WaitGroup
>         // you can also add these one at 
>         // a time if you need to 
>         wg.Add(3)
>         go func() {
>             defer wg.Done()
>             time.Sleep(time.Second * 3)
>             messages <- 1
>         }()
>         go func() {
>             defer wg.Done()
>             time.Sleep(time.Second * 2)
>             messages <- 2
>         }() 
>         go func() {
>             defer wg.Done()
>             time.Sleep(time.Second * 1)
>             messages <- 3
>         }()
>         go func() {
>             for i := range messages {
>                 fmt.Println(i)
>             }
>         }()
>         wg.Wait()
>     }
> ```



