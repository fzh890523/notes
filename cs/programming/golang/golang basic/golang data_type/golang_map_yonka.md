

### 特殊赋值操作 `+=`

```go
m := map[string]int{}
m["a"] += 1  // 看起来是完整的等效于 m["a"] = m["a"] + 1， 不存在的key取出了zero value再...
```





### delete while iterating



This is safe! You can also find a similar sample in [Effective Go](http://golang.org/doc/effective_go.html#for):

```golang
for key := range m {
    if key.expired() {
        delete(m, key)
    }
}
```

And [the language specification](https://golang.org/ref/spec#For_statements):

> The iteration order over maps is not specified and is not guaranteed to be the same from one iteration to the next. If map entries that have not yet been reached are **removed during iteration**, the corresponding iteration values will not be produced. If map entries are **created during iteration**, that entry may be produced during the iteration or may be skipped. The choice may vary for each entry created and from one iteration to the next. If the map is nil, the number of iterations is 0.



## 特殊值行为： nil值



* 读： return zero value
* 写： panic



```go
var m map[string]string
v := m["a"]  // get ""
m["a"] = v  // panic
```









