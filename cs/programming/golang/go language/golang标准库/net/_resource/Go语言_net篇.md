

## [Go语言_net篇](http://www.cnblogs.com/yjf512/archive/2012/06/16/2552296.html)

2012-06-16 23:45 by 轩脉刃, 1560 阅读, 2 评论, [收藏](http://www.cnblogs.com/yjf512/archive/2012/06/16/2552296.html#), [编辑](http://i.cnblogs.com/EditPosts.aspx?postid=2552296)

这篇想阅读的是go的net包，net包提供了各种网络IO类型，比如有TCP/IP, UDP, 域名解析， Unix domain socket

使用这个包能很容易发送一个tcp请求，创建服务器也再也不需要像C语言那样createSocket等一系列繁琐的操作了，用起来那感觉不是一般地爽啊！

 

下面先创建一个简单的Server端：

package main

 

import (

​    "net"

​    "fmt"

)

 

func main() {

​    if ln, err := net.Listen("tcp", ":8080"); err == nil {

​        defer ln.Close()

​        for{

​            ln.Accept()

​            fmt.Println("Receive a Message")

​        }

​    }   

}

 

和脚本一样简单的写法。

先net.Listen(), 第一个参数是协议，tcp还是udp，第二个参数是ip地址，这里可以不填写IP只填写端口就是

使用defer 来Close，将close写在listen之后是个好习惯

然后再循环中使用Accept()接受消息

 

 

 

下面是一个可以进行压力测试的客户端程序

package main

 

import (

​    "net"

​    "fmt"

)

 

func main() {

​    currency := 20 //并发数,记住，一个连接数是打开一个端口号，window和linux的端口号都是有限制的

​    count := 10 //每条连接发送多少次连接

​    for i:=0;i<currency;i++ {

​        go func(){

​            for j:=0;j<count;j++ {

​                sendMessage()

​            }

​        }()

​    }

​    select{}

}

 

func sendMessage() {

​    conn, err := net.Dial("tcp", "127.0.0.1:8080")

​    if(err != nil) {

​        panic("error")

​    }

​    header := "GET / HTTP/1.0\r\n\r\n"

​    fmt.Fprintf(conn, header)

}

![Creative Commons License](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/6091f319-8033-4431-8ed9-c67044bbee50/index_files/dfedbb31-eeaf-4fb3-9cfa-56d7b4fd7ddf.png)

专注Web开发50年。请加群：[![Thinking in Web](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/6091f319-8033-4431-8ed9-c67044bbee50/index_files/a6840730-b911-44a4-9ccc-4196e060f5ad.png)](http://shang.qq.com/wpa/qunwpa?idkey=017d388202803b8f514317087f69ffabc5a1340342943a0fd8d3af1148653eb5)

本文基于[署名-非商业性使用 3.0](http://creativecommons.org/licenses/by-nc/3.0/deed.zh)许可协议发布，欢迎转载，演绎，但是必须保留本文的署名[叶剑峰](http://www.cnblogs.com/yjf512/)（包含链接http://www.cnblogs.com/yjf512/），且不得用于商业目的。如您有任何疑问或者授权方面的协商，请[与我联系](mailto:jianfengye110@gmail.com)。

来源： <<http://www.cnblogs.com/yjf512/archive/2012/06/16/2552296.html>>

 

