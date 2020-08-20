# 原															go语言的time包

​    		    		发表于2年前(2013-08-02 11:50)      		阅读（4149） | 评论（[1](http://my.oschina.net/u/943306/blog/149395#comments)）    					    	    *21*人收藏此文章,[我要收藏](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/003ab9ee-d562-4118-87f0-14e835ec0970/)

*赞**2*

[go](http://www.oschina.net/search?scope=blog&q=go)[time](http://www.oschina.net/search?scope=blog&q=time)[time包](http://www.oschina.net/search?scope=blog&q=time%E5%8C%85)

*目录[-]*

[go语言的time包](http://my.oschina.net/u/943306/blog/149395#OSC_h1_1)

[组成](http://my.oschina.net/u/943306/blog/149395#OSC_h2_2)

[函数](http://my.oschina.net/u/943306/blog/149395#OSC_h2_3)

[Sleep函数](http://my.oschina.net/u/943306/blog/149395#OSC_h3_4)

[After函数](http://my.oschina.net/u/943306/blog/149395#OSC_h3_5)

[AfterFunc函数](http://my.oschina.net/u/943306/blog/149395#OSC_h3_6)

[Tick函数](http://my.oschina.net/u/943306/blog/149395#OSC_h3_7)

[time.Time的方法（time.Time自己独有的函数）](http://my.oschina.net/u/943306/blog/149395#OSC_h2_8)

[Before & After方法](http://my.oschina.net/u/943306/blog/149395#OSC_h3_9)

[Sub方法](http://my.oschina.net/u/943306/blog/149395#OSC_h3_10)

[Add方法](http://my.oschina.net/u/943306/blog/149395#OSC_h3_11)

# go语言的time包

## 组成

- time.Duration（时长，耗时）
- time.Time（时间点）
- time.C（放时间点的管道）[ Time.C:=make(chan time.Time) ]

time包里有2个东西，一个是时间点，另一个是时长 
时间点的意思就是“某一刻”，比如 2000年1月1日1点1分1秒 那一刻（后台记录的是unix时间，从1970年开始计算） 
时长就是某一刻与另一刻的差，也就是耗时

## 函数

### Sleep函数

time.Sleep(time.Duration) 
表示**睡多少时间**，睡觉时，是阻塞状态

```
fmt.Println("start sleeping...")time.Sleep(time.Second)
fmt.Println("end sleep.")//【结果】打印start sleeping后，等了正好1秒后，打印了end sleep//会阻塞，Sleep时，什么事情都不会做
```

[![time.sleep]()](http://static.oschina.net/uploads/space/2013/0802/143748_oxET_943306.png)

### After函数

time.After(time.Duration) 
和Sleep差不多，意思是**多少时间之后**，但在取出管道内容前不阻塞

```
fmt.Println("the 1")
tc:=time.After(time.Second) //返回一个time.C这个管道，1秒(time.Second)后会在此管道中放入一个时间点(time.Now())//时间点记录的是放入管道那一刻的时间值
fmt.Println("the 2")
fmt.Println("the 3")
<-tc   //阻塞中，直到取出tc管道里的数据
fmt.Println("the 4")//【结果】立即打印123，等了1秒不到一点点的时间，打印了4，结束//打印the 1后，获得了一个空管道，这个管道1秒后会有数据进来//打印the 2，（这里可以做更多事情）//打印the 3//等待，直到可以取出管道的数据（取出数据的时间与获得tc管道的时间正好差1秒钟）//打印the 4
```

[![time.After1]()](http://static.oschina.net/uploads/space/2013/0802/143824_2a5o_943306.png)

```
fmt.Println("the 1")
tc:=time.After(time.Second) //返回一个time.C这个管道，1秒(time.Second)后会在此管道中放入//一个时间点(time.Now())，时间点记录的是放入管道那一刻的时间值
fmt.Println("the 2")
fmt.Println("the 3")time.Sleep(time.Second*0.5)//这里是假设这个Println动作执行了半秒钟
fmt.Println("the 4")time.Sleep(time.Second*0.5)//这里是假设这个Println动作执行了半秒钟
fmt.Println("the 5")
fmt.Println("the 6")
fmt.Println("the 7")
<-tc   //阻塞中，直到取出tc管道里的数据
fmt.Println("the 8")//【结果】立即打印1和2，花了半秒打印了3和4，然后又立即打印了5678，结束//这里的<-tc是立即能获得数据的//因为早在执行差不多Print 6的时候，管道内已经有数据了//当gorotine线把数据丢到管道中后，它自己阻塞了（具体请了解goroutine）
```

[![time.After2]()](http://static.oschina.net/uploads/space/2013/0802/143844_MNzs_943306.png)

### AfterFunc函数

time.AfterFunc(time.Duration,func()) 
和After差不多，意思是**多少时间之后在goroutine line执行函数**

```
f := func() {
    fmt.Println("Time out")
}
time.AfterFunc(1*time.Second, f)
time.Sleep(2 * time.Second) //要保证主线比子线“死的晚”，否则主线死了，子线也等于死了
//【结果】运行了1秒后，打印出timeout，又过了1秒，程序退出
//将一个间隔和一个函数给AfterFunc后
//间隔时间过后，执行传入的函数
```

[![time.AfterFunc]()](http://static.oschina.net/uploads/space/2013/0802/155932_3gDS_943306.png)

由于f函数不是在Main Line执行的，而是注册在goroutine Line里执行的 
所以一旦后悔的话，需要使用Stop命令来停止即将开始的执行，如果已经开始执行就来不及了

```
houhui := true
f := func() {
    fmt.Println("Time out")
}
ta := time.AfterFunc(2*time.Second, f)
time.Sleep(time.Second)
if houhui {
    ta.Stop()
}
time.Sleep(3 * time.Second)    //要保证主线比子线“死的晚”，否则主线死了，子线也等于死了
//【结果】运行了3秒多一点点后，程序退出，什么都不打印
//注册了个f函数，打算2秒后执行
//过了1秒后，后悔了，停掉（Stop）它
```

### Tick函数

time.Tick(time.Duration) 
和After差不多，意思是**每隔多少时间后**，其他与After一致

```
fmt.Println("the 1")
tc:=time.Tick(time.Second) //返回一个time.C这个管道，1秒(time.Second)后会在此管道中放入一个时间点，//1秒后再放一个，一直反复，时间点记录的是放入管道那一刻的时间for i:=1;i<=2;i++{
    <-tc
    fmt.Println("hello")
}
//每隔1秒，打印一个hello
```

[![time.Tick]()](http://static.oschina.net/uploads/space/2013/0802/150007_fpNQ_943306.png)

## time.Time的方法（time.Time自己独有的函数）

### Before & After方法

判断一个时间点是否在另一个时间点的前面（后面），返回true或false

```
t1:=time.Now()time.Sleep(time.Second)
t2:=time.Now()a:=t2.After(t1)     //t2的记录时间是否在t1记录时间的**后面**呢，是的话，a就是true
fmt.Println(a)       //trueb:=t2.Before(t1)     //t2的记录时间是否在t1记录时间的**前面**呢，是的话，b就是true
fmt.Println(b)       //false
```

### Sub方法

两个时间点相减，获得时间差（Duration）

```
t1:=time.Now()time.Sleep(time.Second)
t2:=time.Now()
d:=t2.Sub(t1)     //时间2减去时间1
fmt.Println(d)       //打印结果差不多为1.000123几秒，因为Sleep无法做到精确的睡1秒
后发生的时间  减去   先发生时间，是正数

```

### Add方法

拿一个**时间点**，add一个**时长**，获得另一个**时间点**

```
t1:=time.Now()              //现在是12点整（假设）,那t1记录的就是12点整
t2:=t1.Add(time.Hour)          //那t1的时间点 **加上(Add)** 1个小时，是几点呢？
fmt.Println(t2)       //13点（呵呵）
```

来源： <<http://my.oschina.net/u/943306/blog/149395>>