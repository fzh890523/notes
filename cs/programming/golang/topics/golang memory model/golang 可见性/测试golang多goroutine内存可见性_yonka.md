测试golang多goroutine内存可见性_yonka





go代码



```go
msgChan <- fmt.Sprintf("goroutine %d: m is (%v)\n", i, m)
   }
}

func consumeMsgChan(msgChan chan string) {
   for {
      msg, ok := <- msgChan
      if !ok {
         return
      }
      fmt.Println(msg)
   }
}

func TestLocalVisible() {
   oldStdout := os.Stdout
   msgChan := make(chan string, 10240)
   var err error
   os.Stdout, err = os.OpenFile("c:/users/bili/desktop/tmp/a.txt", os.O_WRONLY, 0777)
   if err != nil {
      oldStdout.WriteString(fmt.Sprintf("set stdout err, err is (%v)", err))
   }
   oldStdout.WriteString("begin\n")
   go consumeMsgChan(msgChan)
   runtime.GOMAXPROCS(8)
   for i, _ := range "123456789" {
      go printM(i + 1, msgChan)
   }
   var i int
   for {
      i++
//    time.Sleep(5 * time.Millisecond)
      m = map[int]int{i:i}
      msgChan <- fmt.Sprintf("goroutine main: m is (%v)", m)
   }
}
```





python代码

















 

```python
g_int, m_int = 0, 0

f = open("a.txt", "r")
for l in f:
   try:
      v = int(l[l.index("[")+1: l.index("]")].split(":")[0])
      if l.startswith("goroutine main"):
         now_m_int = int(l[-4])
         if now_m_int > m_int:
            m_int = now_m_int
      else:
         g = int(l[10])
         now_g_int = int(l[-4])
         if now_g_int > g_int:
            g_int = now_g_int
         if g_int < m_int:
            print "find mismatch, g is %d, g_int is %d, m_int is %d" % (g, g_int, m_int)
   except Exception as e:
      print l, e
```











改进版本代码：

```go
var s []int = []int{0}

func printM(i int, msgChan chan Msg) {
   for {
      //    time.Sleep(5 * time.Millisecond)
      if useLock {
         rw.RLock()
         msgChan <- Msg{Seq: i, CurValue: s[0]}
         rw.RUnlock()
      } else {
         msgChan <- Msg{Seq: i, CurValue: s[0]}
      }
   }
}

func consumeMsgChan(msgChan chan Msg) {
   var writerValue int
   var readerValues []int = make([]int, 8)
   for {
      msg, ok := <-msgChan
      if !ok {
         return
      }
      if msg.Seq < 0 {
         writerValue = msg.CurValue
      } else {
         if msg.CurValue < writerValue {
            fmt.Printf("find mismatch, seq is %d, reader value is %d, writer value is %d\n", msg.Seq, msg.CurValue, writerValue)
         }
         readerValues[msg.Seq] = msg.CurValue
      }
   }
}

type Msg struct {
   Seq      int
   CurValue int
}

var rw = sync.RWMutex{}
var useLock bool = true

func TestLocalVisible() {
   oldStdout := os.Stdout
   msgChan := make(chan Msg, 10240)
   // os.Stdout, err = os.OpenFile("c:/users/bili/desktop/tmp/a.txt", os.O_WRONLY, 0777)
   oldStdout.WriteString("begin\n")
   go consumeMsgChan(msgChan)
   runtime.GOMAXPROCS(8)
   for i, _ := range "12345678" {
      go printM(i, msgChan)
   }
   var i int
   for {
      i++
      //    time.Sleep(5 * time.Millisecond)
      if useLock {
         rw.Lock()
         s = []int{i}
         rw.Unlock()
      } else {
         s = []int{i}
      }
      msgChan <- Msg{Seq: -1, CurValue: i}
   }
}
```



useLock为true时，一直OK

为false时：

...

find mismatch, seq is 3, reader value is 285762, writer value is 285763

find mismatch, seq is 4, reader value is 285648, writer value is 285763

find mismatch, seq is 6, reader value is 285764, writer value is 286436

find mismatch, seq is 2, reader value is 285764, writer value is 286436

find mismatch, seq is 0, reader value is 285762, writer value is 286436

find mismatch, seq is 3, reader value is 285764, writer value is 286436

find mismatch, seq is 7, reader value is 285764, writer value is 286436

find mismatch, seq is 6, reader value is 286437, writer value is 286462

find mismatch, seq is 2, reader value is 286437, writer value is 286462

find mismatch, seq is 4, reader value is 285764, writer value is 286462

find mismatch, seq is 5, reader value is 285764, writer value is 286462

find mismatch, seq is 7, reader value is 286437, writer value is 286462

find mismatch, seq is 0, reader value is 286437, writer value is 286462

find mismatch, seq is 1, reader value is 285764, writer value is 286462

find mismatch, seq is 6, reader value is 286463, writer value is 288954

find mismatch, seq is 2, reader value is 286463, writer value is 288955

find mismatch, seq is 7, reader value is 286463, writer value is 291448

find mismatch, seq is 2, reader value is 288956, writer value is 291448

find mismatch, seq is 5, reader value is 286463, writer value is 291448

find mismatch, seq is 3, reader value is 286437, writer value is 291449

find mismatch, seq is 3, reader value is 291450, writer value is 291531

find mismatch, seq is 5, reader value is 291449, writer value is 291531

find mismatch, seq is 3, reader value is 291534, writer value is 291540

find mismatch, seq is 3, reader value is 291541, writer value is 291544

find mismatch, seq is 2, reader value is 291450, writer value is 291544

find mismatch, seq is 1, reader value is 286463, writer value is 291544

find mismatch, seq is 0, reader value is 286463, writer value is 291544

find mismatch, seq is 4, reader value is 286463, writer value is 291544

find mismatch, seq is 7, reader value is 291449, writer value is 291544

...

一直有这个报错...













