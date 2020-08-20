

## [Golang Gob编码](http://www.cnblogs.com/yjf512/archive/2012/08/24/2653697.html)

2012-08-24 09:47 by 轩脉刃, 7748 阅读, 1 评论, [收藏](http://www.cnblogs.com/yjf512/archive/2012/08/24/2653697.html#), [编辑](http://i.cnblogs.com/EditPosts.aspx?postid=2653697)

gob是Golang包自带的一个数据结构序列化的编码/解码工具。编码使用Encoder，解码使用Decoder。一种典型的应用场景就是RPC(remote procedure calls)。

gob和json的pack之类的方法一样，由发送端使用Encoder对数据结构进行编码。在接收端收到消息之后，接收端使用Decoder将序列化的数据变化成本地变量。

 

有一点需要注意，

### 发送方的结构和接受方的结构并不需要完全一致

结构体中缺省的字段将不会被发送。而且在接收方，并不需要所有的字段都要有对应的结构属性对应。godoc中的这个例子很形象：

[![clip_image001](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/13822f27-dd34-4c24-b3f9-213dbe434605/index_files/0e93a13f-7b5c-4d47-8fe3-f60e3bb61ea2.png)](http://images.cnblogs.com/cnblogs_com/yjf512/201208/201208240947024897.png)

当发送方传递的是struct{A, B int}结构的值的时候，接收方可以允许前9种结构，但是后面4种结构确实不允许的。

 

个人觉得这种设定是很符合逻辑的：接收端只接受和发送数据“**相似**”的数据结构。允许模拟相似，但是不允许矛盾。

 

# 各个类型的编解码规则

整型：分为sign int和usign int， 其中从上面例子也看到，int和uint是不能互相编解码的。float和int也是不能互相编解码的。

Struct，array，slice是可以被编码的。但是function和channel是不能被编码的。

bool类型是被当作uint来编码的，0是false，1是true。

浮点类型的值都是被当作float64类型的值来编码的

String和[]byte传递是uint(byte个数) + byte[]的形式编码的

Slice和array是按照uint(array个数) + 每个array编码 这样的形式进行编码的

Maps是按照 uint(Map个数) + 键值对 这样的形式进行编码的

 

Struct是按照一对对（属性名 + 属性值）来进行编码的。其中属性值是其自己对应的gob编码。前面说过，如果有一个属性值为0或空，则这个属性直接被忽略。每个属性的序号是由编码时候顺序决定的，从0开始顺序递增。Struct在序列化前会以-1代表序列化的开始，以0代表序列化结束。即Struct的序列化是按照 “-1 （0 属性1名字 属性1值） （1 属性2名字 属性2值） 0 ”来进行编码的。

 

非常重要的一点：

### Struct中的属性应该是public的，即应该是大写字母开头。

这样才能被包外的函数访问！！（谢谢TreapDB提醒）

# Gob提供的函数

[![clip_image002]()](http://images.cnblogs.com/cnblogs_com/yjf512/201208/201208240947037274.png)

 

# Encode和Decode

对于Encoder和Decoder可以看这个例子：

所有Encoder和Decoder的构造函数都有一个io结构，需要制定你将使用哪个io进行编码解码的传输。

## 这个代码要注意几个地方：

### 1 P和Q是两个结构体，应该说是“相似”的两个结构体

### 2 Encode是将结构体传递过来，但是Decode的函数参数却是一个pointer！

这点在godoc中有说：

f e is nil, the value will be discarded. Otherwise, the value underlying e must be a pointer to the correct type for the next data item received.

Decode的参数如果不是nil，那就一定是一个指针了。

### 3 如果你将Encode传入一个pointer，即

这个function也是没有问题的。

# Register和RegisterName

这两个方法是当编解码中有一个字段是interface{}的时候需要对interface{}的可能产生的类型进行注册。具体就看一下下面这个例子：

这里使用了gob.Register(Inner{})告诉系统：所有的Interface是有可能为Inner结构的。

在这个例子中，如果你注释了gob.Register, 系统会报错。

RegisterName是和Register一样的效果，只是在Register的同时也为这个类型附上一个别名。

 

# GebEncoder和GobDecoder

这是两个接口，如果你的数据结构实现了这两个接口，当调用encoder.Encode和decoder.Decode的时候就会调用这两个结构的对应函数

看一下下面这个例子：

这里我的P实现了GobEncoder接口，因此在enc.Encode的时候会调用func (this *P)GobEncode() ([]byte, error)

当然我这个函数直接返回的是空byte，因此在解码的时候会报错：decode error:gob: type mismatch in decoder: want struct type main.Q; got non-struct

这两个接口暴露出来就代表你为自己定义的结构进行编解码规则制定。当然，**如果使用自己的编解码规则，在编码和解码的过程就需要是一对的**。

# 后记

gob包是golang提供的“私有”的编解码方式，文档中也说了它的效率会比json，xml等更高（虽然我也没有验证）。因此在两个Go 服务之间的相互通信建议不要再使用json传递了，完全可以直接使用gob来进行数据传递。

# 参考资料

<http://blog.golang.org/2011/03/gobs-of-data.html>

<http://www.mikespook.com/2011/03/%E3%80%90%E7%BF%BB%E8%AF%91%E3%80%91gob-%E7%9A%84%E6%95%B0%E6%8D%AE/>

![Creative Commons License](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/13822f27-dd34-4c24-b3f9-213dbe434605/index_files/8c30012f-9f3a-43f8-b8d2-d88b15b4359a.png)

专注Web开发50年。请加群：[![Thinking in Web](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/13822f27-dd34-4c24-b3f9-213dbe434605/index_files/972a44fc-2e30-4059-853b-36bd82326a28.png)](http://shang.qq.com/wpa/qunwpa?idkey=017d388202803b8f514317087f69ffabc5a1340342943a0fd8d3af1148653eb5)

本文基于[署名-非商业性使用 3.0](http://creativecommons.org/licenses/by-nc/3.0/deed.zh)许可协议发布，欢迎转载，演绎，但是必须保留本文的署名[叶剑峰](http://www.cnblogs.com/yjf512/)（包含链接http://www.cnblogs.com/yjf512/），且不得用于商业目的。如您有任何疑问或者授权方面的协商，请[与我联系](mailto:jianfengye110@gmail.com)。

来源： <<http://www.cnblogs.com/yjf512/archive/2012/08/24/2653697.html>>

 



