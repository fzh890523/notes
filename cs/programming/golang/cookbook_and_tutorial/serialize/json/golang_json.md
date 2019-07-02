





# manual



## basic



### annotation



#### omitempty

```go
Fds    []int       `json:"fds,omitempty"`
```



#### ignore



```go
Files  []*os.File	`json:"-"`
```





## 自定义解析

实现如下接口：
```golang
type Marshaler interface {
    MarshalJSON() ([]byte, error)
}
type Unmarshaler interface {
    UnmarshalJSON([]byte) error
}
```

如 [ref](https://blog.csdn.net/lanyang123456/article/details/83115803)：
```golang
type Bird struct {
    A map[string]string `json:"a"` 
}

func (bd *Bird) MarshalJSON() ([]byte, error) {
    l := []string{}
    for _,v := range bd.A { 
        l = append(l,v) 
    } 
    return json.Marshal(l) 
} 

func (bd *Bird) UnmarshalJSON(b []byte) error {
    l := []string{} 
    err := json.Unmarshal(b, &l) 
    if err != nil { 
        return err
    } 
    for i,v := range l { 
        k := fmt.Sprintf("%d", i) 
        bd.A[k] = v 
    } 
    return nil 
}

baoXiNiao := &Bird{A:map[string]string{}}
err = json.Unmarshal([]byte(b), baoXiNiao)
```



## Pretty print



```go
json.MarshalIndent(data, "", "\t")
// or
json.MarshalIndent(data, "", "🐱")
```









