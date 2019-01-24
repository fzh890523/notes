

# type assertion



* nil 不能assert

  如：

  ```go
  func a(i Interface1) {
      v, ok := i.(int)
  }
  
  a(nil)  // panic rather than ok == false
  ```






