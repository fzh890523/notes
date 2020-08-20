

## [golang rpc的两种调用方法](http://www.cnblogs.com/yjf512/archive/2013/02/28/2937261.html)

2013-02-28 17:20 by 轩脉刃, 4881 阅读, 0 评论, [收藏](http://www.cnblogs.com/yjf512/archive/2013/02/28/2937261.html#), [编辑](http://i.cnblogs.com/EditPosts.aspx?postid=2937261)

golang的rpc有两种方法进行调用，一种是rpc例子中给的：

 

 

另一种是使用NewServer

这种是当rpc已经注册的时候就要使用了另外一种了。即一个server只能在DefaultRPC中注册一种类型。

当Server使用rpc.NewServer的时候，client也需要进行下改动了

![Creative Commons License](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/fe1843c3-8683-432f-966e-ab3efbe06e6d/index_files/e27f0bec-f739-4ab4-9e2d-1aa2f94f949c.png)

专注Web开发50年。请加群：[![Thinking in Web](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/fe1843c3-8683-432f-966e-ab3efbe06e6d/index_files/ee6db9b6-e8a5-4096-bd2f-d0048611fd27.png)](http://shang.qq.com/wpa/qunwpa?idkey=017d388202803b8f514317087f69ffabc5a1340342943a0fd8d3af1148653eb5)

本文基于[署名-非商业性使用 3.0](http://creativecommons.org/licenses/by-nc/3.0/deed.zh)许可协议发布，欢迎转载，演绎，但是必须保留本文的署名[叶剑峰](http://www.cnblogs.com/yjf512/)（包含链接http://www.cnblogs.com/yjf512/），且不得用于商业目的。如您有任何疑问或者授权方面的协商，请[与我联系](mailto:jianfengye110@gmail.com)。

来源： <<http://www.cnblogs.com/yjf512/archive/2013/02/28/2937261.html>>

 

