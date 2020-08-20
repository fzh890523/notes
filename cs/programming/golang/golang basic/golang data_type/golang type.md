

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





