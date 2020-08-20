

# [golang]内存不断增长bytes.makeSlice

 2015-04-03 20:00   zhangqingping

 阅读 1323 次   0 人喜欢  [ 0 条评论](http://studygolang.com/articles/2763#commentForm)  [ 收藏](http://studygolang.com/articles/2763#)



golang写的一个图片服务器，在批量下载压缩时候发现内存不断增长。。。。

幸好golang自带内存占用日志结合分析工具可以方便看到内存分布。

详细可参考：

http://blog.golang.org/profiling-go-programs

可以实时统计CPU\内存信息。

这里主要说一下内存怎么搞。CPU分析的参考之前的一篇[文章](http://www.cnblogs.com/zhangqingping/p/4345071.html)。

```
//需要包含这个pprof包
import  "runtime/pprof"


//这里接收内存统计信息保存文件
var memprofile = flag.String("memprofile", "", "write memory profile to this file")


//这里是判断是否需要记录内存的逻辑
if *memprofile != "" {
        var err error
        memFile, err = os.Create(*memprofile)
        if err != nil {
            log.Println(err)
        } else {
            log.Println("start write heap profile....")
            pprof.WriteHeapProfile(memFile)
            defer memFile.Close()
        }
    }

//这里还有一个比较灵活的办法，把开启记录和关闭记录作为http请求，需要的时候开启\不需要的时候关闭。记得加上token
```

全部代码如下：

```
// GODEBUG=schedtrace=1000 ./trace_example
// GOMAXPROCS=2 GODEBUG=schedtrace=1000 ./trace_example
// GOMAXPROCS=2 GODEBUG=schedtrace=1000,scheddetail=1 ./trace_examplepackage main

import (
    "flag"
    "log"
    "os"
    "runtime/pprof"
    // "net/http"
    // _ "net/http/pprof"
    "sync"
    "time"
)

//http://www.graphviz.org/Download_macos.php// var cpuprofile = flag.String("cpuprofile", "", "write cpu profile to file")
var memprofile = flag.String("memprofile", "", "write memory profile to this file")

var memFile *os.File

func main() {
    flag.Parse()
    // if *cpuprofile != "" {
    //     f, err := os.Create(*cpuprofile)
    //     if err != nil {
    //         log.Fatal(err)
    //     }
    //     pprof.StartCPUProfile(f)
    //     defer pprof.StopCPUProfile()
    // }if *memprofile != "" {
        var err error
        memFile, err = os.Create(*memprofile)
        if err != nil {
            log.Println(err)
        } else {
            log.Println("start write heap profile....")
            pprof.WriteHeapProfile(memFile)
            defer memFile.Close()
        }
    }

    // go func() {
    //     log.Println(http.ListenAndServe("localhost:6060", nil))
    // }()
    var wg sync.WaitGroup
    wg.Add(10)
    for i := 0; i < 10; i++ {
        go work(&wg)
    }

    wg.Wait()
    // Wait to see the global run queue deplete.
    time.Sleep(300 * time.Second)
}

func work(wg *sync.WaitGroup) {
    time.Sleep(time.Second)

    var counter intfor i := 0; i < 1e10; i++ {
        time.Sleep(time.Millisecond * 100)
        pprof.WriteHeapProfile(memFile)
        counter++
    }
    wg.Done()
}
```

OK，加上这个内存分析数据之后，继续跑服务, 跑了一段时候之后，停止程序,采用以下命令进行分析。

```
 go tool pprof image_service  memory.log 
(pprof) top20
2622.12MB of 4938.25MB total (53.10%)
Dropped 180 nodes (cum <= 24.69MB)
Showing top 20 nodes out of 30 (cum >= 419.23MB)
      flat  flat%   sum%        cum   cum%
 1759.43MB 35.63% 35.63%  1759.43MB 35.63%  bytes.makeSlice
  203.06MB  4.11% 39.74%   320.58MB  6.49%  net/url.parseQuery
  166.11MB  3.36% 43.10%   166.11MB  3.36%  net/textproto.(*Reader).ReadLine
  132.03MB  2.67% 45.78%   132.03MB  2.67%  net/textproto.(*Reader).ReadMIMEHeader
  117.52MB  2.38% 48.16%   117.52MB  2.38%  net/url.unescape
   71.02MB  1.44% 49.60%    71.02MB  1.44%  mcommoninit
   60.50MB  1.23% 50.82%    60.50MB  1.23%  fmt.Sprintf
   37.51MB  0.76% 51.58%    98.01MB  1.98%  _/home/qingpingzhang/project/createdji_servers/image_service/image.(*Handler).HandleRedo
   35.51MB  0.72% 52.30%   333.65MB  6.76%  net/http.ReadRequest
   21.37MB  0.43% 52.73%    21.37MB  0.43%  github.com/gographics/imagick/imagick._Cfunc_GoBytes
   17.57MB  0.36% 53.09%    17.57MB  0.36%  bufio.NewReaderSize
    0.50MB  0.01% 53.10%    21.58MB  0.44%  net/http.(*Transport).dialConn
         0     0% 53.10%    21.87MB  0.44%  _/home/qingpingzhang/project/createdji_servers/image_service/image.(*Handler).CompressWithSizeList
         0     0% 53.10%  1781.66MB 36.08%  _/home/qingpingzhang/project/createdji_servers/image_service/image.(*Handler).DoRecompress
         0     0% 53.10%  1759.29MB 35.63%  _/home/qingpingzhang/project/createdji_servers/image_service/image.(*Handler).httpGetToMagickWand
         0     0% 53.10%    17.57MB  0.36%  bufio.NewReader
         0     0% 53.10%  1759.43MB 35.63%  bytes.(*Buffer).ReadFrom
         0     0% 53.10%    21.37MB  0.43%  github.com/gographics/imagick/imagick.(*MagickWand).GetImageBlob
         0     0% 53.10%   419.23MB  8.49%  main.(*ImageService).ServeHTTP
         0     0% 53.10%   419.23MB  8.49%  main.Action
(pprof) quit
```

初步可以定位到时下载压缩时，分配了太多byteSlice导致。

观察代码，没有发现具体原因，直到在网上发现了这篇文章：

http://openmymind.net/Go-Slices-And-The-Case-Of-The-Missing-Memory/

```
buffer := bytes.NewBuffer(make([]byte, 0, resp.ContentLength)
buffer.ReadFrom(res.Body)
body := buffer.Bytes()
```

### A Memory Leak

Look, what's a memory leak within the context of a runtime that provides garbage collection? Typically it's either a rooted object, or a reference [from](http://cpro.baidu.com/cpro/ui/uijs.php?adclass=0&app_id=0&c=news&cf=1001&ch=0&di=128&fv=19&is_app=0&jk=bb6dcebfc02e30dc&k=from&k0=from&kdi0=0&luki=6&mcpm=0&n=10&p=baidu&q=74042097_cpr&rb=0&rs=1&seller_id=1&sid=dc302ec0bfce6dbb&ssp2=1&stid=0&t=tpclicked3_hc&td=1989498&tu=u1989498&u=http%3A%2F%2Fstudygolang%2Ecom%2Farticles%2F2763&urlid=0) a rooted object, which you haven't considered. This is obviously different as it's really extra memory you might not be aware of. Rooting the object might very well be intentional, but you don't realize just how much[memory](http://cpro.baidu.com/cpro/ui/uijs.php?adclass=0&app_id=0&c=news&cf=1001&ch=0&di=128&fv=19&is_app=0&jk=bb6dcebfc02e30dc&k=memory&k0=memory&kdi0=0&luki=8&mcpm=0&n=10&p=baidu&q=74042097_cpr&rb=0&rs=1&seller_id=1&sid=dc302ec0bfce6dbb&ssp2=1&stid=0&t=tpclicked3_hc&td=1989498&tu=u1989498&u=http%3A%2F%2Fstudygolang%2Ecom%2Farticles%2F2763&urlid=0) it is you've rooted. Sure, my ignorance is at least 75% to blame. Yet I can't help but shake the feeling that there's something too subtle about all of this. Any code can return something that looks and quacks like an array of 2 integers yet takes gigs of [memory](http://cpro.baidu.com/cpro/ui/uijs.php?adclass=0&app_id=0&c=news&cf=1001&ch=0&di=128&fv=19&is_app=0&jk=bb6dcebfc02e30dc&k=memory&k0=memory&kdi0=0&luki=8&mcpm=0&n=10&p=baidu&q=74042097_cpr&rb=0&rs=1&seller_id=1&sid=dc302ec0bfce6dbb&ssp2=1&stid=0&t=tpclicked3_hc&td=1989498&tu=u1989498&u=http%3A%2F%2Fstudygolang%2Ecom%2Farticles%2F2763&urlid=0). Furthermore, `bytes.MinRead` as a [global](http://cpro.baidu.com/cpro/ui/uijs.php?adclass=0&app_id=0&c=news&cf=1001&ch=0&di=128&fv=19&is_app=0&jk=bb6dcebfc02e30dc&k=global&k0=global&kdi0=0&luki=4&mcpm=0&n=10&p=baidu&q=74042097_cpr&rb=0&rs=1&seller_id=1&sid=dc302ec0bfce6dbb&ssp2=1&stid=0&t=tpclicked3_hc&td=1989498&tu=u1989498&u=http%3A%2F%2Fstudygolang%2Ecom%2Farticles%2F2763&urlid=0) variable is just bad design. I can't imagine how many people think they've allocated X when they've really allocated X*2+512.

大致的意思是说，这个buffer采用最小单位读，若不够，则继续申请2倍大的空间。

可以查看[源码](http://cpro.baidu.com/cpro/ui/uijs.php?adclass=0&app_id=0&c=news&cf=1001&ch=0&di=128&fv=19&is_app=0&jk=bb6dcebfc02e30dc&k=%D4%B4%C2%EB&k0=%D4%B4%C2%EB&kdi0=0&luki=1&mcpm=0&n=10&p=baidu&q=74042097_cpr&rb=0&rs=1&seller_id=1&sid=dc302ec0bfce6dbb&ssp2=1&stid=0&t=tpclicked3_hc&td=1989498&tu=u1989498&u=http%3A%2F%2Fstudygolang%2Ecom%2Farticles%2F2763&urlid=0)：

```
   146    // ReadFrom reads data from r until EOF and appends it to the buffer, growing
   147    // the buffer as needed. The return value n is the number of bytes read. Any
   148    // error except io.EOF encountered during the read is also returned. If the
   149    // buffer becomes too large, ReadFrom will panic with ErrTooLarge.
   150    func (b *Buffer) ReadFrom(r io.Reader) (n int64, err error) {
   151        b.lastRead = opInvalid
   152        // If buffer is empty, reset to recover space.
   153        if b.off >= len(b.buf) {
   154            b.Truncate(0)
   155        }
   156        for {
   157            if free := cap(b.buf) - len(b.buf); free < MinRead {
   158                // not enough space at end
   159                newBuf := b.buf
   160                if b.off+free < MinRead {
   161                    // not enough space using beginning of buffer;
   162                    // double buffer capacity
   163                    newBuf = makeSlice(2*cap(b.buf) + MinRead)
   164                }
   165                copy(newBuf, b.buf[b.off:])
   166                b.buf = newBuf[:len(b.buf)-b.off]
   167                b.off = 0
   168            }
   169            m, e := r.Read(b.buf[len(b.buf):cap(b.buf)])
   170            b.buf = b.buf[0 : len(b.buf)+m]
   171            n += int64(m)
   172            if e == io.EOF {
   173                break
   174            }
   175            if e != nil {
   176                return n, e
   177            }
   178        }
   179        return n, nil // err is EOF, so return nil explicitly
   180    }
```

 

 

解决方案：

```
//ioutil.ReadAll starts at a very small 512
//it really should let you specify an initial size
buffer := bytes.NewBuffer(make([]byte, 0, 65536))
io.Copy(buffer, r.Body)
temp := buffer.Bytes()
length := len(temp)
var body []byte//are we wasting more than 10% space?if cap(temp) > (length + length / 10) {
  body = make([]byte, length)
  copy(body, temp)
} else {
  body = temp
}
```

稍微测试了以下，内存被[垃圾回收](http://cpro.baidu.com/cpro/ui/uijs.php?adclass=0&app_id=0&c=news&cf=1001&ch=0&di=128&fv=19&is_app=0&jk=bb6dcebfc02e30dc&k=%C0%AC%BB%F8%BB%D8%CA%D5&k0=%C0%AC%BB%F8%BB%D8%CA%D5&kdi0=0&luki=10&mcpm=0&n=10&p=baidu&q=74042097_cpr&rb=0&rs=1&seller_id=1&sid=dc302ec0bfce6dbb&ssp2=1&stid=0&t=tpclicked3_hc&td=1989498&tu=u1989498&u=http%3A%2F%2Fstudygolang%2Ecom%2Farticles%2F2763&urlid=0)了。为啥会出现这样的情况呢？

```
Entering interactive mode (type "help" for commands)
(pprof) top20
834.66MB of 1599.63MB total (52.18%)
Dropped 175 nodes (cum <= 8MB)
Showing top 20 nodes out of 25 (cum >= 72.01MB)
      flat  flat%   sum%        cum   cum%
  427.45MB 26.72% 26.72%   427.45MB 26.72%  bytes.makeSlice
  185.80MB 11.62% 38.34%   614.25MB 38.40%  _/home/qingpingzhang/project/createdji_servers/image_service/image.(*Handler).httpGetToMagickWand
   69.01MB  4.31% 42.65%    69.01MB  4.31%  net/textproto.(*Reader).ReadMIMEHeader
      48MB  3.00% 45.65%       48MB  3.00%  net/url.unescape
   24.51MB  1.53% 47.18%    24.51MB  1.53%  mcommoninit
   24.01MB  1.50% 48.68%    72.01MB  4.50%  net/url.parseQuery
      24MB  1.50% 50.19%   117.02MB  7.32%  net/http.ReadRequest
      24MB  1.50% 51.69%       24MB  1.50%  net/url.parse
    7.87MB  0.49% 52.18%     7.87MB  0.49%  github.com/gographics/imagick/imagick._Cfunc_GoBytes
         0     0% 52.18%     7.87MB  0.49%  _/home/qingpingzhang/project/createdji_servers/image_service/image.(*Handler).CompressWithSizeList
         0     0% 52.18%   622.62MB 38.92%  _/home/qingpingzhang/project/createdji_servers/image_service/image.(*Handler).DoRecompress
         0     0% 52.18%   427.95MB 26.75%  bytes.(*Buffer).ReadFrom
         0     0% 52.18%     7.87MB  0.49%  github.com/gographics/imagick/imagick.(*MagickWand).GetImageBlob
         0     0% 52.18%    72.01MB  4.50%  main.(*ImageService).ServeHTTP
         0     0% 52.18%    72.01MB  4.50%  main.Action
         0     0% 52.18%    72.01MB  4.50%  net/http.(*Request).ParseForm
         0     0% 52.18%   117.02MB  7.32%  net/http.(*conn).readRequest
         0     0% 52.18%   117.02MB  7.32%  net/http.(*conn).serve
         0     0% 52.18%    72.01MB  4.50%  net/http.func·014
         0     0% 52.18%    72.01MB  4.50%  net/url.ParseQuery
```

 

在golang语言自带的bytes包里面申请的内存，为啥就不会很快被回收？

不解，IO操作这块儿还需要找时间重新学习一下。

 

本文来自：[博客园](http://studygolang.com/wr?u=http://www.cnblogs.com)

感谢作者：zhangqingping

查看原文：[[golang\]内存不断增长bytes.makeSlice](http://studygolang.com/wr?u=http%3a%2f%2fwww.cnblogs.com%2fzhangqingping%2fp%2f4390913.html)

来源： <<http://studygolang.com/articles/2763>>

 



