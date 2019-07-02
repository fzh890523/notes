# intro



* index 不能为负数





# tutorial



## len

```go
func f(s ...int) {
	fmt.Printf("len(s): %d, s: (%#v) or (%v)\n", len(s), s, s)
	fmt.Printf("s == nil?: %v\n", s == nil)
}

func test3() {
	var s []int
	f(s...)
	s = []int{}
	f(s...)

	//len(s): 0, s: ([]int(nil)) or ([])
	//s == nil?: true
	//len(s): 0, s: ([]int{}) or ([])
	//s == nil?: false
}
```



* len(nil-slice) 是ok的
* nil != empty
* nil-slice可以作为var-args





## re-slicing



```go

func test1() {
	s := make([]int, 5)

	s0 := s[4:]
	fmt.Printf("s0: (%#v)\n", s0)  // s0: ([]int{0})
	s1 := s[5:]
	fmt.Printf("s1: (%#v)\n", s1)  // s1: ([]int{})
	s2 := s[6:] // panic: runtime error: slice bounds out of range
	fmt.Printf("s2: (%#v)\n", s2)
}
func test2() {
	s := make([]int, 5)

	s0 := s[2:3:4]
	fmt.Printf("s0: (%#v), len: %d, cap: %d\n", s0, len(s0), cap(s0))  // s0: ([]int{0}), len: 1, cap: 2
	s1 := s[2:3:5]
	fmt.Printf("s1: (%#v), len: %d, cap: %d\n", s1, len(s1), cap(s1))  // s1: ([]int{0}), len: 1, cap: 3
	s2 := s[2:3:6]  // panic: runtime error: slice bounds out of range
	fmt.Printf("s2: (%#v), len: %d, cap: %d\n", s2, len(s2), cap(s2))
}
```





* 下界可以是 len 表示一个都不要（empty）
* 下界 > len 会panic 而不是类似py的empty
* 3-index-reslicing在go里含义跟一般的有所差别： [lower_bound:upper_bound:cap_bound]





## delete item

https://stackoverflow.com/questions/37334119/how-to-delete-an-element-from-array-in-golang/37335777

**Order matters**

If you want to keep your array ordered, you have to shift all of the elements at the right of the deleting index by one to the left. Hopefully, this can be done easily in Golang:

```Go
func remove(slice []int, s int) []int {
    return append(slice[:s], slice[s+1:]...)
}
```

However, this is inefficient because you may end up with moving all of the elements, which is costy.

**Order is not important**

If you do not care about ordering, you have the much faster possibility to swap the element to delete with the one at the end of the slice and then return the n-1 first elements:

```Go
func remove(s []int, i int) []int {
    s[len(s)-1], s[i] = s[i], s[len(s)-1]
    return s[:len(s)-1]
}
```

With the reslicing method, emptying an array of 1 000 000 elements take 224s, with this one it takes only 0.06ns. I suspect that internally, go only changes the length of the slice, without modifying it.





```go
b := a[:0]
for _, x := range a {
	if f(x) {
		b = append(b, x)
	}
}	

// ref: https://github.com/golang/go/wiki/SliceTricks#filtering-without-allocating
```



而类似： 

```go
for i, x := range a {
    if !f(x) {
        a = append(a[:i], a[i:]...)
    }
}

// 1. 没搜到不允许的说法，但也没搜到可信的可以这么写的依据，所以保守起见，不建议使用
// 2. 性能也没好，大约在 m * log(n)，相对于官方推荐做法的 log(n-m)
```







## append



```go
var s []int

s = append(nil, 1)  // 报错如下

// first argument to append must be typed slice; have untyped nil

// 其实这里应该是可以做类型推断的吧 = =
```



## compare/equal



* shallow compare or `==`/`!=`

  ```
  Slice, map, and function values are not comparable. However, as a special case, a slice, map, or function value may be compared to the predeclared identifier nil. Comparison of pointer, channel, and interface values to nil is also allowed and follows from the general rules above.
  ```

  from: https://golang.org/ref/spec#Comparison_operators

  

## expand/grow




















