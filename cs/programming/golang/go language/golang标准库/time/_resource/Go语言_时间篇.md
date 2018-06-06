

## [Go语言_时间篇](http://www.cnblogs.com/yjf512/archive/2012/06/12/2546243.html)

2012-06-12 13:50 by 轩脉刃, 9310 阅读, 9 评论, [收藏](http://www.cnblogs.com/yjf512/archive/2012/06/12/2546243.html#), [编辑](http://i.cnblogs.com/EditPosts.aspx?postid=2546243)

哎，先发下牢骚，Go文档还是太弱了，特别是标准库中的文档说明，很多函数几乎就一句话过了。这点确实该向Microsoft的MSDN好好学学。

 

# Go的time包是标准库中的包之一

不用说，几乎是开发必须用到的包之一。time包的说明文档在：

<http://golang.org/pkg/time/>

 

先注意下Overview中的一句话：

The calendrical calculations always assume a Gregorian calendar.

这个包的时间计算式是基于格里历，就是我们使用的公历。关于格里历的历史，可以看百度百科：

<http://baike.baidu.com/view/391054.htm>

 

看看godoc文档，最大的数据类型就是Time了，这个Time类型最微小可以表示到nanosecond（微毫秒，十亿份之一秒）。

[![clip_image001]()](http://images.cnblogs.com/cnblogs_com/yjf512/201206/201206121349473169.png)

 

Time的比较是使用Before,After和Equal方法。看一眼After：

func (t Time) After(u Time) bool

很好，返回的是bool类型，是我们所需要的。

 

Sub方法返回的是两个时间点之间的时间距离，看上图看到它返回的是Duration结构，这个结构的具体类型和操作也在godoc中

 

Add方法和Sub方法是相反的，获取t0和t1的时间距离d是使用Sub，将t0加d获取t1就是使用Add方法

 

IsZero方法：Time的zero时间点是January 1, year 1, 00:00:00 UTC，这个函数判断一个时间是否是zero时间点

 

Local，UTC，Ln是用来显示和计算地区时间的。

 

# 下面从几个需求直接看time的使用

## 1 请打出当前时间的时间戳，然后将时间戳格式为年月日时分秒的形式

显示：

[![clip_image002]()](http://images.cnblogs.com/cnblogs_com/yjf512/201206/201206121349478775.png)

特别是Format这个函数，可以好好使用

## 2 输出当前星期几？

[![clip_image003]()](http://images.cnblogs.com/cnblogs_com/yjf512/201206/201206121349481316.png)

 

文档中对这个Weekday类型就没有说明!!没法，直接看代码可以看到：

[![clip_image004]()](http://images.cnblogs.com/cnblogs_com/yjf512/201206/20120612134948270.png)

 

Weekday有一个String()方法

好了，看到这里外带我们有一个推测：

 

## 当一个结构中有定义String()函数的时候，fmt.Println()是会调用String的

例子如下：

[![clip_image005](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/e4c71e06-7f6d-4934-a7cd-60101bde810f/index_files/2fabfc7d-ff60-43f8-a64e-8283c50daf2a.png)](http://images.cnblogs.com/cnblogs_com/yjf512/201206/201206121349484763.png)

 

Go的Time之旅结束！！

![Creative Commons License](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/e4c71e06-7f6d-4934-a7cd-60101bde810f/index_files/e70b5faf-4e5b-450f-93d4-4ba8d88d3496.png)

专注Web开发50年。请加群：[![Thinking in Web](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/e4c71e06-7f6d-4934-a7cd-60101bde810f/index_files/9081bcfb-0bf5-4d9c-92a5-873f0d38fa49.png)](http://shang.qq.com/wpa/qunwpa?idkey=017d388202803b8f514317087f69ffabc5a1340342943a0fd8d3af1148653eb5)

本文基于[署名-非商业性使用 3.0](http://creativecommons.org/licenses/by-nc/3.0/deed.zh)许可协议发布，欢迎转载，演绎，但是必须保留本文的署名[叶剑峰](http://www.cnblogs.com/yjf512/)（包含链接http://www.cnblogs.com/yjf512/），且不得用于商业目的。如您有任何疑问或者授权方面的协商，请[与我联系](mailto:jianfengye110@gmail.com)。

来源： <<http://www.cnblogs.com/yjf512/archive/2012/06/12/2546243.html>>

