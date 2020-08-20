

### Memory, variables in memory, and pointers

[Other topics in this series - Table of Contents](http://golangtutorials.blogspot.com/2011/05/table-of-contents.html)

```
Full code
```

```
package main

import "fmt"

func main() {
    i := 5var j int
    fmt.Println("i is: ", i)
    fmt.Println("j is: ", j)}
```

i is: 5
j is: 0

```
i := 5
```

```
j
```

[![img]()](http://4.bp.blogspot.com/-lqjgJiK_jVI/TefRvx5CrWI/AAAAAAAABHk/-bPZ6LiisoY/s1600/memory-representation.png)

### Default values of primitive types

```
Full code
```

```
package main

import "fmt"

func main() {var i int
    fmt.Println("default int is: ", i)var s string
    fmt.Println("default string is: ", s)var f float64
    fmt.Println("default float64 is: ", f)var arInt [3]int
    fmt.Println("default int array is: ", arInt)var c complex64
    fmt.Println("default complex64 is: ", c)}
```

default int is: 0
default string is: 
default float64 is: 0
default int array is: [0 0 0]
default complex64 is: (0+0i)

### Addresses and memory location

```
Full code
```

```
package main

import "fmt"

func main() {
    i := 5
    fmt.Println("i is: ", i)
    fmt.Println("address of i is: ", &i)}
```

i is: 5
address of i is: 0xf840000040

```
&
```

```
package main

import "fmt"

func main() {var i int
    fmt.Println("address of i is: ", &i)var s string
    fmt.Println("address of s is: ", &s)var f float64
    fmt.Println("address of f is: ", &f)var c complex64
    fmt.Println("address of c is: ", &c)}
```

address of i is: 0xf840000040
address of s is: 0xf8400013e0
address of f is: 0xf8400000f8
address of c is: 0xf8400000f0

```
*
```

```
Full code
```

```
package main

import "fmt"

func main() {var i int
    fmt.Println("value of i is: ", i)
    fmt.Println("address of i is: ", &i)
    fmt.Println("value at address ", &i, " is: ", *(&i)) //value at (address of i)
    fmt.Println()var s string
    fmt.Println("value of s is: ", s)
    fmt.Println("address of s is: ", &s)
    fmt.Println("value at address ", &s, " is: ", *&s) ////value at address of i
    fmt.Println()var f float64
    fmt.Println("value of f is: ", f)
    fmt.Println("address of f is: ", &f)
    fmt.Println("value at address ", &f, " is: ", *&f)
    fmt.Println()var c complex64
    fmt.Println("value of c is: ", c)
    ptr := &c //address of c.  
    fmt.Println("address of c is: ", ptr)
    fmt.Println("value at address ", ptr, " is: ", *ptr) //value at the address}
```

value of i is: 0
address of i is: 0xf840000040
value at address 0xf840000040 is: 0
value of s is: 
address of s is: 0xf8400013b0
value at address 0xf8400013b0 is: 
value of f is: 0
address of f is: 0xf8400000e8
value at address 0xf8400000e8 is: 0
value of c is: (0+0i)
address of c is: 0xf8400000b8
value at address 0xf8400000b8 is: (0+0i)

```
ptr := &c
```

```
ptr
```

```
c
```

```
ptr
```

```
c
```

```
ptr
```

```
c
```

```
i := 5; ptr := &i
```

```
i
```

```
*ptr
```

[![img](http://2.bp.blogspot.com/-9xPNbIKj2P8/TefRnFC87TI/AAAAAAAABHY/Xyy8OG3f0Ak/s1600/pointer-representation.png)](http://2.bp.blogspot.com/-9xPNbIKj2P8/TefRnFC87TI/AAAAAAAABHY/Xyy8OG3f0Ak/s1600/pointer-representation.png)

```
package main

func main() {const i = 5
    ptr := &i //error: cannot take the address of i

    ptr2 := &10 //error: cannot take the address of 10}
```

### The need for addresses/pointers/references

http://en.wikipedia.org/wiki/Paris

[Other topics in this series - Table of Contents](http://golangtutorials.blogspot.com/2011/05/table-of-contents.html)

来源： <<http://golangtutorials.blogspot.jp/2011/06/memory-variables-in-memory-and-pointers.html>>



