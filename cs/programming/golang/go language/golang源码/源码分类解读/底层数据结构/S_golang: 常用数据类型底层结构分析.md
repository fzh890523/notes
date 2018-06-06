

# 原															golang: 常用数据类型底层结构分析

​    		    		发表于2年前(2014-01-29 20:09)      		阅读（3266） | 评论（[7](http://my.oschina.net/goal/blog/196891#comments)）    					    	    *21*人收藏此文章,[我要收藏](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/c0eb8e85-d92e-452d-b4e1-ece68d6df2fa/)

*赞**4*

[尖er货来了！亚马逊AWS AWSome Day 免费云计算培训开始报名](http://www.oschina.net/action/visit/ad?id=1074)  ![img](http://my.oschina.net/img/hot3.png?t=1445088771000)

摘要

[golang](http://www.oschina.net/search?scope=blog&q=golang)[类型](http://www.oschina.net/search?scope=blog&q=%E7%B1%BB%E5%9E%8B)[数据类型](http://www.oschina.net/search?scope=blog&q=%E6%95%B0%E6%8D%AE%E7%B1%BB%E5%9E%8B)[基础类型](http://www.oschina.net/search?scope=blog&q=%E5%9F%BA%E7%A1%80%E7%B1%BB%E5%9E%8B)[底层结构](http://www.oschina.net/search?scope=blog&q=%E5%BA%95%E5%B1%82%E7%BB%93%E6%9E%84)

*目录[-]*

[基础类型](http://my.oschina.net/goal/blog/196891#OSC_h2_1)

[rune类型](http://my.oschina.net/goal/blog/196891#OSC_h2_2)

[string类型](http://my.oschina.net/goal/blog/196891#OSC_h2_3)

[slice类型](http://my.oschina.net/goal/blog/196891#OSC_h2_4)

[接口类型](http://my.oschina.net/goal/blog/196891#OSC_h2_5)

[map类型](http://my.oschina.net/goal/blog/196891#OSC_h2_6)

虽然golang是用C实现的，并且被称为下一代的C语言，但是golang跟C的差别还是很大的。它定义了一套很丰富的数据类型及数据结构，这些类型和结构或者是直接映射为C的数据类型，或者是用C struct来实现。了解golang的数据类型和数据结构的底层实现，将有助于我们更好的理解golang并写出质量更好的代码。

## 基础类型

源码在：$GOROOT/src/pkg/runtime/runtime.h 。我们先来看下基础类型：

[?](http://my.oschina.net/goal/blog/196891#)

int8、uint8、int16、uint16、int32、uint32、int64、uint64、float32、float64分别对应于C的类型，这个只要有C基础就很容易看得出来。uintptr和intptr是无符号和有符号的指针类型，并且确保在64位平台上是8个字节，在32位平台上是4个字节，uintptr主要用于golang中的指针运算。而intgo和uintgo之所以不命名为int和uint，是因为int在C中是类型名，想必uintgo是为了跟intgo的命名对应吧。intgo和uintgo对应golang中的int和uint。从定义可以看出int和uint是可变大小类型的，在64位平台上占8个字节，在32位平台上占4个字节。所以如果有明确的要求，应该选择int32、int64或uint32、uint64。byte类型的底层类型是uint8。可以看下测试：

[?](http://my.oschina.net/goal/blog/196891#)

[?](http://my.oschina.net/goal/blog/196891#)

数据类型分为静态类型和底层类型，相对于以上代码中的变量b来说，byte是它的静态类型，uint8是它的底层类型。这点很重要，以后经常会用到这个概念。

## rune类型

rune是int32的别名，用于表示unicode字符。通常在处理中文的时候需要用到它，当然也可以用range关键字。

## string类型

string类型的底层是一个C struct。

[?](http://my.oschina.net/goal/blog/196891#)

成员str为字符数组，len为字符数组长度。golang的字符串是不可变类型，对string类型的变量初始化意味着会对底层结构的初始化。至于为什么str用byte类型而不用rune类型，这是因为golang的for循环对字符串的遍历是基于字节的，如果有必要，可以转成rune切片或使用range来迭代。我们来看个例子：

$GOPATH/src

----basictype_test

--------main.go

[?](http://my.oschina.net/goal/blog/196891#)

[?](http://my.oschina.net/goal/blog/196891#)

内建函数len对string类型的操作是直接从底层结构中取出len值，而不需要额外的操作，当然在初始化时必需同时初始化len的值。

## slice类型

slice类型的底层同样是一个C struct。

[?](http://my.oschina.net/goal/blog/196891#)

包括三个成员。array为底层数组，len为实际存放的个数，cap为总容量。使用内建函数make对slice进行初始化，也可以类似于数组的方式进行初始化。当使用make函数来对slice进行初始化时，第一个参数为切片类型，第二个参数为len，第三个参数可选，如果不传入，则cap等于len。通常传入cap参数来预先分配大小的slice，避免频繁重新分配内存。

[?](http://my.oschina.net/goal/blog/196891#)

[?](http://my.oschina.net/goal/blog/196891#)

由于切片指向一个底层数组，并且可以通过切片语法直接从数组生成切片，所以需要了解切片和数组的关系，否则可能就会不知不觉的写出有bug的代码。比如有如下代码：

[?](http://my.oschina.net/goal/blog/196891#)

[?](http://my.oschina.net/goal/blog/196891#)

您可以清楚的看到，在改变slice后，array也被改变了。这是因为slice通过数组创建的切片指向这个数组，也就是说这个slice的底层数组就是这个array。因此很显然，slice的改变其实就是改变它的底层数组。当然如果删除或添加元素，那么len也会变化，cap可能会变化。

那这个slice是如何指向array呢？slice的底层数组指针指向array中索引为2的元素(因为切片是通过array[2:4]来生成的)，len记录元素个数，而cap则等于len。

之所以说cap可能会变，是因为cap表示总容量，添加或删除操作不一定会使总容量发生变化。我们接着再来看另一个例子：

[?](http://my.oschina.net/goal/blog/196891#)

[?](http://my.oschina.net/goal/blog/196891#)

经过append操作之后，对slice的修改并未影响到array。原因在于append的操作令slice重新分配底层数组，所以此时slice的底层数组不再指向前面定义的array。

但是很显然，这种规则对从切片生成的切片也是同样的，请看代码：

[?](http://my.oschina.net/goal/blog/196891#)

[?](http://my.oschina.net/goal/blog/196891#)

slice1和slice2共用一个底层数组，修改slice2的元素导致slice1也发生变化。

[?](http://my.oschina.net/goal/blog/196891#)

[?](http://my.oschina.net/goal/blog/196891#)

而append操作可令slice1或slice2重新分配底层数组，因此对slice1或slice2执行append操作都不会相互影响。

## 接口类型

接口在golang中的实现比较复杂，在$GOROOT/src/pkg/runtime/type.h中定义了：

[?](http://my.oschina.net/goal/blog/196891#)

在$GOROOT/src/pkg/runtime/runtime.h中定义了：

[?](http://my.oschina.net/goal/blog/196891#)

interface实际上是一个结构体，包括两个成员，一个是指向数据的指针，一个包含了成员的类型信息。Eface是interface{}底层使用的数据结构。因为interface中保存了类型信息，所以可以实现反射。反射其实就是查找底层数据结构的元数据。完整的实现在：$GOROOT/src/pkg/runtime/iface.c 。

[?](http://my.oschina.net/goal/blog/196891#)

[?](http://my.oschina.net/goal/blog/196891#)

## map类型

golang的map实现是hashtable，源码在：$GOROOT/src/pkg/runtime/hashmap.c 。

[?](http://my.oschina.net/goal/blog/196891#)

测试代码如下：

[?](http://my.oschina.net/goal/blog/196891#)

[?](http://my.oschina.net/goal/blog/196891#)

golang的坑还是比较多的，需要深入研究底层，否则很容易掉坑里。

来源： <<http://my.oschina.net/goal/blog/196891>>

