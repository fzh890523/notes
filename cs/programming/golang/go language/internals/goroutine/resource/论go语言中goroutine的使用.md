

## [论go语言中goroutine的使用](http://www.cnblogs.com/yjf512/archive/2012/06/30/2571247.html)

2012-06-30 19:31 by 轩脉刃, 7845 阅读, 8 评论, [收藏](http://www.cnblogs.com/yjf512/archive/2012/06/30/2571247.html#), [编辑](http://i.cnblogs.com/EditPosts.aspx?postid=2571247)

go中的goroutine是go语言在语言级别支持并发的一种特性。初接触go的时候对go的goroutine的欢喜至极，实现并发简便到简直bt的地步。但是在项目过程中，越来越发现goroutine是一个很容易被大家滥用的东西。goroutine是一把双面刃。这里列举一下goroutine使用的几宗罪：

# 1 goroutine的指针传递是不安全的

非常符合逻辑的代码：

主routine开一个routine把request传递给saveRequestToRedis1，让它把请求储存到redis节点1中

同时开另一个routine把request传递给saveReuqestToRedis2，让它把请求储存到redis节点2中

然后主routine就进入循环（不结束进程）

 

问题现在来了，saveRequestToRedis1和saveReuqestToRedis2两个函数其实不是我写的，而是团队另一个人写的，我对其中的实现一无所知，也不想去仔细看内部的具体实现。但是根据函数名，我想当然地把request指针传递进入。

 

好了，实际上saveRequestToRedis1和saveRequestToRedis2 是这样实现的：

这样有什么问题？saveRequestToRedis1和saveReuqestToRedis2两个goroutine修改了同一个共享数据结构，但是由于routine的执行是无序的，因此我们无法保证request.ToUsers设置和redis.Save()是一个原子操作，这样就会出现实际存储redis的数据错误的bug。

 

好吧，你可以说这个saveRequestToRedis的函数实现的有问题，没有考虑到会是使用go routine调用。请再想一想，这个saveRequestToRedis的具体实现是没有任何问题的，它不应该考虑上层是怎么使用它的。那就是我的goroutine的使用有问题，主routine在开一个routine的时候并没有确认这个routine里面的任何一句代码有没有修改了主routine中的数据。对的，主routine确实需要考虑这个情况。但是按照这个思路，所以呢？主goroutine在启用go routine的时候需要阅读子routine中的每行代码来确定是否有修改共享数据？？这在实际项目开发过程中是多么降低开发速度的一件事情啊！

 

go语言使用goroutine是想减轻并发的开发压力，却不曾想是在另一方面增加了开发压力。

 

上面说的那么多，就是想得出一个结论：

gorotine的指针传递是不安全的！！

 

如果上一个例子还不够隐蔽，这里还有一个例子：

很少人会考虑到this指针指向的对象是否会有问题，这里的this指针传递给routine应该说是非常隐蔽的。

 

# 2 goroutine增加了函数的危险系数

这点其实也是源自于上面一点。上文说，往一个go函数中传递指针是不安全的。那么换个角度想，你怎么能保证你要调用的函数在函数实现内部不会使用go呢？如果不去看函数体内部具体实现，是没有办法确定的。

例如我们将上面的典型例子稍微改改

这下我们没有使用并发，就一定不会出现这问题了吧？追到函数里面去，傻眼了：

我勒个去啊，里面起了一个goroutine，并修改了request指针指向的对象。这里就产生了错误了。好吧，如果在调用函数的时候，不看函数内部的具体实现，这个问题就无法避免。所以说呢？所以说，从最坏的思考角度出发，每个调用函数理论上来说都是不安全的！试想一下，这个调用函数如果不是自己开发组的人编写的，而是使用网络上的第三方开源代码...确实无法想象找出这个bug要花费多少时间。

# 3 goroutine的滥用陷阱

看一下这个例子：

神奇啊，go无处不在，好像眨眨眼就在哪里冒出来了。这就是go的滥用，到处都见到go，但是却不是很明确，哪里该用go？为什么用go？goroutine确实会有效率的提升么？

c语言的并发比go语言的并发复杂和繁琐地多，因此我们在使用之前会深思，考虑使用并发获得的好处和坏处。go呢？几乎不。

 

# 处理方法

下面说几个我处理这些问题的方法：

## 1 当启动一个goroutine的时候，如果一个函数必须要传递一个指针，但是函数层级很深，在无法保证安全的情况下，传递这个指针指向对象的一个克隆，而不是直接传递指针

Clone函数需要另外写。可以在结构体定义之后简单跟上这个方法。比如：

其实从效率角度考虑这样确实会产生不必要的Clone的操作，耗费一定内存和CPU。但是在我看来，首先，为了安全性，这个尝试是值得的。其次，如果项目对效率确实有很高的要求，那么你不妨在开发阶段遵照这个原则使用clone，然后在项目优化阶段，作为一种优化手段，将不必要的Clone操作去掉。这样就能在保证安全的前提下做到最好的优化。

## 2 什么时候使用go的问题

有两种思维逻辑会想到使用goroutine：

### 1 业务逻辑需要并发

比如一个服务器，接收请求，阻塞式的方法是一个请求处理完成后，才开始第二个请求的处理。其实在设计的时候我们一定不会这么做，我们会在一开始就已经想到使用并发来处理这个场景，每个请求启动一个goroutine为它服务，这样就达到了并行的效果。这种goroutine直接按照思维的逻辑来使用goroutine

### 2 性能优化需要并发

一个场景是这样：需要给一批用户发送消息，正常逻辑会使用

但是在考虑到性能问题的时候，我们就不会这样做，如果users的个数很大，比如有1000万个用户？我们就没必要将1000万个用户放在一个routine中运行处理，考虑将1000万用户分成1000份，每份开一个goroutine，一个goroutine分发1万个用户，这样在效率上会提升很多。这种是性能优化上对goroutine的需求

 

按照项目开发的流程角度来看。在项目开发阶段，第一种思路的代码实现会直接影响到后续的开发实现，因此在项目开发阶段应该马上实现。但是第二种，项目中是由很多小角落是可以使用goroutine进行优化的，但是如果在开发阶段对每个优化策略都考虑到，那一定会直接打乱你的开发思路，会让你的开发周期延长，而且很容易埋下潜在的不安全代码。因此第二种情况在开发阶段绝不应该直接使用goroutine，而该在项目优化阶段以优化的思路对项目进行重构。

 

# 总结

总结下，文章写了这么多，并不是想让你对goroutine的使用产生畏惧，而是想强调一个观点：

goroutine的使用应该是保守型的。

在你敲下go这两个字母之前请仔细思考是否应该使用goroutine这柄利刃。

 

后续

在你看完这篇以后，也建议看看**stevewang的**这篇吧：

http://blog.sina.com.cn/s/blog_9be3b8f10101dsr6.html

![Creative Commons License](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/b444a153-2782-4393-b1a1-58bea83c7030/index_files/134670a8-b2ee-4241-8cf6-8941f0437f87.png)

专注Web开发50年。请加群：[![Thinking in Web](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/b444a153-2782-4393-b1a1-58bea83c7030/index_files/f009d14e-1ac6-42ff-a54b-353d4dcecd51.png)](http://shang.qq.com/wpa/qunwpa?idkey=017d388202803b8f514317087f69ffabc5a1340342943a0fd8d3af1148653eb5)

本文基于[署名-非商业性使用 3.0](http://creativecommons.org/licenses/by-nc/3.0/deed.zh)许可协议发布，欢迎转载，演绎，但是必须保留本文的署名[叶剑峰](http://www.cnblogs.com/yjf512/)（包含链接http://www.cnblogs.com/yjf512/），且不得用于商业目的。如您有任何疑问或者授权方面的协商，请[与我联系](mailto:jianfengye110@gmail.com)。

来源： <<http://www.cnblogs.com/yjf512/archive/2012/06/30/2571247.html>>

 

