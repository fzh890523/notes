



# args



## var args



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



* Nil/empty slice都可以作为var args...





