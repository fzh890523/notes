

`make(chan ${type}, ${buf})` 允许`buf`为0，等同于 `make(chan ${type})`





## nil channel



- A send to a nil channel blocks forever
- A receive from a nil channel blocks forever



## closed channel



- A send to a closed channel panics
- A receive from a closed channel returns the zero value immediately



## listen to N channels(variable)

ref: https://stackoverflow.com/questions/19992334/how-to-listen-to-n-channels-dynamic-select-statement



```go
cases := make([]reflect.SelectCase, len(chans))
for i, ch := range chans {
    cases[i] = reflect.SelectCase{Dir: reflect.SelectRecv, Chan: reflect.ValueOf(ch)}
}
chosen, value, ok := reflect.Select(cases)
# ok will be true if the channel has not been closed.
ch := chans[chosen]
msg := value.String()
```



```go
agg := make(chan string)
for _, ch := range chans {
  go func(c chan string) {
    for msg := range c {
      agg <- msg
    }
  }(ch)
}

select {
case msg <- agg:
    fmt.Println("received ", msg)
}
// 号称比reflect性能好很多
```





