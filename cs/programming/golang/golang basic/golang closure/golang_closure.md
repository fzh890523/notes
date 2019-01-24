



```go
func main() {
	var con = 100
	wg := &sync.WaitGroup{}
	wg.Add(con)
	for i := 0; i < con; i++ {
		v := i
		go func() {
			time.Sleep(10 * time.Millisecond) // 让前面先执行完
			fmt.Printf("%d-before: %d\n", i, v)
			v = v * 10
			wg.Done()
		}()
		fmt.Printf("out: %d\n", v)
		go func() {
			time.Sleep(11 * time.Millisecond) // 让前面先执行完
			fmt.Printf("%d-after: %d\n", i, v)
			v = v * 10
		}()
	}
	wg.Wait()
	time.Sleep(5 * time.Second)
}


// output: 序号i最后print都是100而v则是“独有”的值
```



```
out: 0
out: 1
out: 2
...
out: 99
100-before: 1
100-before: 37
100-before: 29
100-before: 2
...
100-before: 94
100-before: 96
100-before: 99
100-after: 0
100-after: 10
100-after: 80
100-after: 30
...
100-after: 940
100-after: 980
100-after: 990
```







之前的怀疑： 闭包里对于整个循环的scope是一个struct，这样的话，对于局部变量的访问有并发问题（并且更严重的是读到的是最后一个）。



确认的情况： 

* 对于循环内的局部变量，实际是每次循环一个scope，这样访问没有并发问题
* 对于循环“上”的局部变量，比如 `for i:=0; i<max; i++`的`i`则和其他语言一样实际是同一个scope，访问到的也是同一个。。。











