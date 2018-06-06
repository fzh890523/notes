## [Go语言_反射篇](http://www.cnblogs.com/yjf512/archive/2012/06/10/2544391.html)

2012-06-10 22:51 by 轩脉刃, 9364 阅读, 5 评论, [收藏](http://www.cnblogs.com/yjf512/archive/2012/06/10/2544391.html#), [编辑](http://i.cnblogs.com/EditPosts.aspx?postid=2544391)

Go语言的基本语法的使用已经在前几篇陆陆续续学完了，下面可能想写一些Go的标准库的使用了。

先是reflect库。

reflect库的godoc在<http://golang.org/pkg/reflect/>

# Type和Value

首先，reflect包有两个数据类型我们必须知道，一个是Type，一个是Value。

Type就是定义的类型的一个数据类型，Value是值的类型

具体的Type和Value里面包含的方法就要看文档了：

<http://golang.org/pkg/reflect/>

 

这里我写了个程序来理解Type和Value：

输出结果：

[![clip_image001]()](http://images.cnblogs.com/cnblogs_com/yjf512/201206/20120610225027327.png)

这个程序看到几点：

### 1 TypeOf和ValueOf是获取Type和Value的方法

### 2 ValueOf返回的<float64 Value>是为了说明这里的value是float64

### 3 第三个b的定义实现了php中的string->method的方法，为什么返回的是reflect.Value[]数组呢？当然是因为Go的函数可以返回多个值的原因了。

# Value的方法和属性

好了，我们看到Value的Type定义了这么多Set方法：

[![clip_image002]()](http://images.cnblogs.com/cnblogs_com/yjf512/201206/201206102250278965.png)

下面看这么个例子：

返回：

[![clip_image003]()](http://images.cnblogs.com/cnblogs_com/yjf512/201206/201206102250307135.png)

 

这段代码能有一些事情值得琢磨：

### 1 为什么a和b的ValueOf返回的是不一样的？

a是一个结构，b是一个指针。好吧，在Go中，指针的定义和C中是一样的。

### 2 reflect.ValueOf(a).FieldByName("name")

这是一个绕路的写法，其实和a.name是一样的意思，主要是要说明一下Value.FieldByName的用法

### 3 val := reflect.ValueOf(b).FieldByName("name") 是有error的，为什么？

b是一个指针，指针的ValueOf返回的是指针的Type，它是没有Field的，所以也就不能使用FieldByName

### 4 fmt.Println(reflect.ValueOf(a).FieldByName("name").CanSet())为什么是false?

看文档中的解释：

[![clip_image004](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/0f5f3907-5531-45f9-a492-b8e1719124a3/index_files/d54b2093-8259-4f44-9e52-b41d49db0f58.png)](http://images.cnblogs.com/cnblogs_com/yjf512/201206/201206102250331511.png)

好吧，什么是addressable，and was not obtained by the use of unexported struct fields?

CanSet当Value是可寻址的时候，返回true，否则返回false

看到第二个c和p的例子，我们可以这么理解：

### 当前面的CanSet是一个指针的时候（p）它是不可寻址的，但是当是p.Elem()(实际上就是*p)，它就是可以寻址的

这个确实有点绕。

 

总而言之，reflect包是开发过程中几乎必备的包之一。能合理和熟练使用它对开发有很大的帮助。

![Creative Commons License](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/0f5f3907-5531-45f9-a492-b8e1719124a3/index_files/756c34bb-2058-420c-af8a-745f1bf03387.png)

专注Web开发50年。请加群：[![Thinking in Web](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/0f5f3907-5531-45f9-a492-b8e1719124a3/index_files/5904515e-6596-440a-a4e2-5d31e534e373.png)](http://shang.qq.com/wpa/qunwpa?idkey=017d388202803b8f514317087f69ffabc5a1340342943a0fd8d3af1148653eb5)

本文基于[署名-非商业性使用 3.0](http://creativecommons.org/licenses/by-nc/3.0/deed.zh)许可协议发布，欢迎转载，演绎，但是必须保留本文的署名[叶剑峰](http://www.cnblogs.com/yjf512/)（包含链接http://www.cnblogs.com/yjf512/），且不得用于商业目的。如您有任何疑问或者授权方面的协商，请[与我联系](mailto:jianfengye110@gmail.com)。

来源： <<http://www.cnblogs.com/yjf512/archive/2012/06/10/2544391.html>>