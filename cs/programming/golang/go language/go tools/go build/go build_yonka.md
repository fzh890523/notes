





# compile time constants

ref: https://stackoverflow.com/questions/15214459/how-to-properly-use-build-tags



```sh
go build -ldflags '-X main.DEBUG=YES' test.go
# 老版本是： -X main.DEBUG YES
```



```go
package main

import (
    "fmt"
)

var DEBUG = "NO"

func main() {
    fmt.Printf("DEBUG is %q\n", DEBUG)
}
```





# build tag



ref: https://stackoverflow.com/questions/10646531/golang-conditional-compilation/10649653#10649653



```go
// +build main1

package main

import (
    "fmt"
)

func main() {
    fmt.Println("This is main 1")
}

// +build main2

package main

import (
    "fmt"
)

func main() {
    fmt.Println("This is main 2")
}
```


> 在 package main 前面需要有空行，因为在此行之前的（连续）注释行被视为包注释



```sh
go build -tags 'main1'
```



## 一些应用场景



### 指定、排除平台

* `// +build linux,386 darwin,!cgo`

  等于： `(linux AND 386) OR (darwin AND (NOT cgo))`

* ```
  // +build linux darwin
  // +build 386
  ```

  等于： `(linux OR darwin) AND 386`

* `// +build !windows`

  除windows以外的平台都...



# flags



## gcflags



```sh
-gcflags "all=-N -l"
# idea上还有bug，提示里错误写为： -gcflags='all -N -l'
```



## ldflags



```sh

```







