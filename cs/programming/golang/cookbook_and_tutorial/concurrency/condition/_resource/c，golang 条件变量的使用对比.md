

# c，golang 条件变量的使用对比

** 2015-01-15 13:24  ** yujian0231

** 阅读 701 次  ** 0 人喜欢  [** 0 条评论](http://studygolang.com/articles/2190#commentForm)  [** 收藏](http://studygolang.com/articles/2190#)

看到golang 标准库 sync  package 提供了传统的mutex, once, cond, rwmutex 等基于共享内存的同步原语，就想写个代码实验一下。

```
type Cond struct {    
      // L is held while observing or changing the condition
    L Locker
      // contains filtered or unexported fields
}
```

Cond 结构包含一个可导出的Locker 对象

```
func NewCond(l Locker) *Cond
```

NewCond 函数 接受一个实现Locker 接口的对象， 返回一个指向Cond 的指针;   pthread_cond_t  对应于此

```
func (c *Cond) Broadcast()
```

Brocast 唤醒所有在这个cond 对象上等待的 goroutine； pthread_cond_brocast() 对应于此

```
func (c *Cond) Signal()
```

Signal 唤醒一个再此cond 对象上等待的goroutine；  pthread_cond_signal() 对应于此

```
type Mutex struct {    // contains filtered or unexported fields  }
```

```
func (m *Mutex) Lock()
```

```
func (m *Mutex) Unlock()
```

Mutex 拥有Lock，Unlock 方法， 所以实现了 

```
type Locker interface {
    Lock()
    Unlock()
}
```

```
#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int condition = 0;
int count = 0;

int consume( void )
{
   while( 1 )
   {
      pthread_mutex_lock( &mutex );
      while( condition == 0 )
         pthread_cond_wait( &cond, &mutex );
      printf( "Consumed %d\n", count );
      condition = 0;
      pthread_cond_signal( &cond );      
      pthread_mutex_unlock( &mutex );
   }

   return( 0 );
}

void*  produce( void * arg )
{
   while( 1 )
   {
      pthread_mutex_lock( &mutex );
      while( condition == 1 )
         pthread_cond_wait( &cond, &mutex );
      printf( "Produced %d\n", count++ );
      condition = 1;
      pthread_cond_signal( &cond );      
      pthread_mutex_unlock( &mutex );
   }
   return( 0 );
}

int main( void )
{
   pthread_t thr;
   pthread_create( &thr, NULL, &produce, NULL );
   return consume();
}
```

接着等价的golang 实现：

```
package main 

import (
    "fmt"
    "sync"
)

var count = 0
var condition = 0

func main(){
    lock := new(sync.Mutex)
    cond := sync.NewCond(lock)

    go func (){
        for {
           lock.Lock() 
           for condition == 0 {
               cond.Wait()
           }
           fmt.Printf("Consumed %d\n", count )
           condition = 0
           cond.Signal()
           lock.Unlock()
        }
    }()
    for {
           lock.Lock() 
           for condition == 1 {
               cond.Wait()
           }
           fmt.Printf("Produced %d\n", count )
           count++
           condition = 1
           cond.Signal()
           lock.Unlock()
    }
}
```

来源： <<http://studygolang.com/articles/2190>>

