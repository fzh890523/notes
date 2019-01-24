## [mymysql与go-mysql-driver性能比较](http://www.cnblogs.com/yjf512/archive/2013/01/21/2869198.html)

2013-01-21 09:35 by 轩脉刃, 5162 阅读, 6 评论, [收藏](http://www.cnblogs.com/yjf512/archive/2013/01/21/2869198.html#), [编辑](http://i.cnblogs.com/EditPosts.aspx?postid=2869198)

mymysql和go-mysql-driver是两个现在都很流行的go的mysql驱动，这篇文章目的是要将这两个驱动进行一下比较

两个mysql驱动的下载地址：

<https://github.com/ziutek/mymysql>

<http://code.google.com/p/go-mysql-driver/>

# 首先是性能测试

## 准备工作：

在mysql建表和初始化数据（db是test）

 

## 两边的库代码和测试代码

[![Image]()](http://images.cnitblog.com/blog/136188/201301/21093451-7050cd425a3746a8ac12b098f2374c4a.png)

已经将gomysqldriver和mymysql的代码放到github上了，有兴趣的去里面看看。

<https://github.com/jianfengye/MyWorks/tree/master/gomysqltest>

代码里面注意的几点就是我们测试了get，insert，update三个操作，并且insert的时候不指定主键，让其自增，innodb的表，这样让mysql处理插入操作尽可能快。

 

## 下面运行go test -bench=".*" -v -benchmem

mymysql的表现：

[![Image(1)]()](http://images.cnitblog.com/blog/136188/201301/21093452-b802238fe8104b7999a49aabfd731641.png)

go-mysql-driver的表现：

[![Image(2)]()](http://images.cnitblog.com/blog/136188/201301/21093453-359852c6ea1640db88b5ebce7f4c23f0.png)

## 输出的数据分析：

Benchmark的测试用例名   benchtime内调用了多少次  每次调用耗时（纳秒）  每次调用耗内存  每次调用分配内存次数

比如：

mymysql 的Benchmark_getAdmin在1s内一共调用了2000次，每次调用使用了974622纳秒，使用内存大小为13444Byte，分配内存的alloc调用了220次

可以看出，go-mysql-driver的每个命令运行的时间是比mymysql多，但是内存是使用的情况却比mymysql少。

猜测原因由于go-mysql-driver是使用默认的database/sql和database/sql/driver接口，由于接口是官方提供的，估计耗时多在方法匹配上，调用内存方面由于是官方的database/sql来进行连接等分配，写的会比mymysql写的好一些。

# 下面比较两边的profile

## 在两个项目下都调用

go test -bench=".*" -c

go test -bench=".*" -cpuprofile="cpu.prof" -memprofile="mem.prof" -blockprofile="block.prof" -memprofilerate=1 -blockprofilerate=1

## 依次调用

go tool pprof mymysql.test cpu.prof

go tool pprof mymysql.test cpu.prof

go tool pprof mymysql.test cpu.prof

## 生成svg文件

具体可以参考我之前的一篇文章 go的pprof使用（<http://www.cnblogs.com/yjf512/archive/2012/12/27/2835331.html>）

我这里已经将它们都生成好了并命名为诸如mymysql_cpu.svg放在github上，你也可以直接去下载看

<https://github.com/jianfengye/MyWorks/tree/master/gomysqltest>

## 怎么看svg

关于pprof这里有一篇权威文章：cpuprofile（<http://google-perftools.googlecode.com/svn/trunk/doc/cpuprofile.html>）

 

先要明白几个名词

### sample

sample就是“取样”。pprof是基于取样调查的，比如我每纳秒取样一次，收集这个时候程序的运行函数栈，知道现在是运行在那个函数中，然后把这些信息放在pprof文件中提供分析。

### node

node就是函数调用信息，哪个函数中被调用了，调用了多少次

[![Image(3)]()](http://images.cnitblog.com/blog/136188/201301/21093453-f71e6fe48b434dcb8562c001fdada9cc.png)

### 方法名

本方法占用sample次数（占所有sample的总数）

本方法的下行方法调用次数（占所有sample的比例）

“本方法占用sample的次数”就是除了调用下行的方法之外的其他代码占用的方法数，当然是越小越好，越小说明了除了下行的方法之外的代码几乎不占用cpu时间。node的大小和这个值是正相关的

“下行方法调用次数”就是下行方法的调用中占用了多少个sample。

如果上面两个值相等，那么“下行方法调用次数”就会被被忽略。这个一般只出现在edges中。

比如sweepspan就是下行方法占用37个sample，本身只占用了1个sample。

### edges

edges就是终结点

[![Image(4)]()](http://images.cnitblog.com/blog/136188/201301/21093454-69716e9eb49d4b859d6ced33f503b8a6.png)

runtime.mcmp就是自身是终结点，没有下行方法，所以下行和本方法占用的sample相等。

## 基本信息解读

比如：

[![Image(5)]()](http://images.cnitblog.com/blog/136188/201301/21093455-fed0091037d44ee18e17c98e385f21d9.png)

**mymysql.test**是可执行文件名

**Total samples**：总的统计sample（打点数）

**Focusing on**：关注的sample。为什么有关注sample这么一说呢，并不是说所有的node和edges都是有用的信息，有的不重要的node和edges是会被忽略的。Focusing on samples就是除了这些不重要的node和edges之外的sample。

**Dropped nodes**：参考Focusing on。被忽略的node。

**Dropped edges**: 参考Focusing on。被忽略的edges。

ps: 这里默认的total sample是等于focusing sample的。你在pprof的时候可以使用--ignore参数来忽略掉那些不重要的node或者edges

明白了这些就知道了，看图应该从最大的node往小的方向看，分析下占用资源多的函数在那里，是否可以优化这个函数或者方法。

 

比如可以看一下

gomysqldriver_cpu.svg这个例子

它有个比较占用sample的分支是

[![Image(6)]()](http://images.cnitblog.com/blog/136188/201301/21093456-89696ce4eafd4e9aa37f14309f7c24df.png)

它的源头在parseDSN

看到代码里面去，会发现是解析dsn这步的时候使用了正则，导致运行Open的时候运行速度下降了。

所以说如果parseDSN这个函数的参数不是dsn string，而是使用map直接指定username，password等，这里的速度就会上去了。当然这其实也是不可以的，因为database/sql/driver的Open方法定义的参数就是一个string。

## 总结：

pprof图将代码流程完完全全地展现在我们面前。所以说呢我们可以做这么几件事情：

1 根据pprof优化代码

2 根据pprof学习一个完全陌生的开源软件

3 根据pprof学习go的一个程序是怎么运行的

4 项目上线前的性能测试和压力测试（在ab之外的有一个好的选择了）

# mymysql和go-mysql-driver的测试总结

根据以上的比较，我还是倾向于使用go-mysql-driver。原因有几个：

1 go-mysql-driver是实现了golang标准库database/sql的产物。底层实现比较有保证

2 go-mysql-driver虽然每个命令的运行时间比mymysql长，但是内存使用少得非常明显，这点两方算打平。

3 go-mysql-driver实现了database/sql，如果数据库换成其他的话，不需要更改应用逻辑的代码。

4 go-mysql-driver实现了database/sql，这个接口的设计也是非常好的，基本和php中的pdo一样，上手和学习成本低。

![Creative Commons License](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/2e24f178-c9ec-411c-a2f8-50e164e9e52e/index_files/222f634a-e6e2-44ea-9121-a3fb26695aba.png)

专注Web开发50年。请加群：[![Thinking in Web](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/2e24f178-c9ec-411c-a2f8-50e164e9e52e/index_files/44dbb834-1722-4d0c-bdc4-31e11b4dc48d.png)](http://shang.qq.com/wpa/qunwpa?idkey=017d388202803b8f514317087f69ffabc5a1340342943a0fd8d3af1148653eb5)

本文基于[署名-非商业性使用 3.0](http://creativecommons.org/licenses/by-nc/3.0/deed.zh)许可协议发布，欢迎转载，演绎，但是必须保留本文的署名[叶剑峰](http://www.cnblogs.com/yjf512/)（包含链接http://www.cnblogs.com/yjf512/），且不得用于商业目的。如您有任何疑问或者授权方面的协商，请[与我联系](mailto:jianfengye110@gmail.com)。

来源： <<http://www.cnblogs.com/yjf512/archive/2013/01/21/2869198.html>>

 