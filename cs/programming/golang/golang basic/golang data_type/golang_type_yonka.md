

# type assertion



* nil 不能assert

  如：

  ```go
  func a(i Interface1) {
      v, ok := i.(int)
  }
  
  a(nil)  // panic rather than ok == false
  ```
  
  但可以 `.(type)` (不过仅限interface类型，基础和指针都不行)，如：
  ```go
  var err error
  switch err.(type) {
  case xxx
  }
  ```





# enum



```go
const (
	OPDel OP = iota - 1  // start from -1
	OPUpdate
	OPAdd
)

const (
	read   = 1 << iota // 00000001 = 1
	write              // 00000010 = 2
	remove             // 00000100 = 4

	// admin will have all of the permissions
	admin = read | write | remove
)

const (
	_   int = iota // Skip the first value of 0
	Foo            // Foo = 1
	Bar            // Bar = 2
	_
	_
	Bin // Bin = 5
	// Using a comment or a blank line will not increment the iota value

	Baz // Baz = 6
)


const (
	_  = 1 << (iota * 10) // ignore the first value
	KB                    // decimal:       1024 -> binary 00000000000000000000010000000000
	MB                    // decimal:    1048576 -> binary 00000000000100000000000000000000
	GB                    // decimal: 1073741824 -> binary 01000000000000000000000000000000
)

const (
	tomato, apple int = iota + 1, iota + 2
	orange, chevy
	ford, _
)


```





