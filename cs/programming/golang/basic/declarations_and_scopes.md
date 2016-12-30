

# 变量声明

## 循环中的变量声明

`var i type` 等同于 `var i type = type-zero-value`
所以for循环中的变量声明等于每次循环都重新init了，不用担心值复用问题。

```golang
package main

import "fmt"

func main() {
    var i int
    for i=0;i<20;i++{
        var k int
        if i % 5 == 0 {
            k = 10
        }
        fmt.Printf("k is %d while i is %d\n", k, i)
    }
}
```

```
k is 10 while i is 0
k is 0 while i is 1
k is 0 while i is 2
k is 0 while i is 3
k is 0 while i is 4
k is 10 while i is 5
k is 0 while i is 6
k is 0 while i is 7
k is 0 while i is 8
k is 0 while i is 9
k is 10 while i is 10
k is 0 while i is 11
k is 0 while i is 12
k is 0 while i is 13
k is 0 while i is 14
k is 10 while i is 15
k is 0 while i is 16
k is 0 while i is 17
k is 0 while i is 18
k is 0 while i is 19
```

参考c：
```
yonka@yonkaubuntu:/tmp$ ./a.out 
i is 1, j is 1
i is 2, j is 1
i is 3, j is 1
i is 4, j is 1
i is 5, j is 1
i is 6, j is 1
i is 7, j is 1
i is 8, j is 1
i is 9, j is 1
yonka@yonkaubuntu:/tmp$ cat t.c
```
```c
#include <stdio.h>

void main() {
        for(int i=1; i<10; i++) {
                int j;
                if(j % 2 == 0) {
                        j++;
                }
                printf("i is %d, j is %d\n", i, j);
        }
}
```
也是同样的行为，也即`int j`等同于`int j = 0`。

