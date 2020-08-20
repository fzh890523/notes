## [go的sql包](http://www.cnblogs.com/yjf512/archive/2013/01/23/2872577.html)

2013-01-23 09:55 by 轩脉刃, 2621 阅读, 8 评论, [收藏](http://www.cnblogs.com/yjf512/archive/2013/01/23/2872577.html#), [编辑](http://i.cnblogs.com/EditPosts.aspx?postid=2872577)

go的sql包是在pkg/database中，里面的两个包sql和sql/driver可以一起看。建议看这个两个包之前可以先看看sql文件夹下的doc.txt。这个文档说了几点比较重要的：

1 这两个包是真正Go风格的包。

2 这使用这两个包就不需要关于并发处理了，也不需要维护自己的数据库连接池了，一旦建立了一个连接，这个连接是可以在各个goroutine之间共用的。

3 sql/driver提供的是数据库的接口，具体的实现还需要自己实现。

 

# 先看database/driver包

## 第一个用到的方法是Register

这个方法将实现了driver.Driver的驱动注册到变量drivers中，当写完一个驱动之后，就需要将驱动注册到sql中才能使用sql包中的这些接口。这个实现了driver.Driver的必须实现了Open方法.

driver.Open返回的是driver.Conn，它的三个方法

Prepare ： 参数绑定

Close ： 关闭连接

Begin ： 支持事务

## 先看Prepare，这个和php mysql pdo一样的用法

Conn.Prepare("select * from test where a=?")

返回的Stmt结构：

Close ： 关闭这个statement

NumInput ： 返回有多少个可以绑定的参数

Exec ： Insert或者update等无返回的使用

Query ：select等查询操作使用

Exec是绑定变量，然后返回Result结构

Query是绑定变量，然后返回Rows结果集

## 看Result里面的方法：

LastInsertId()  : Insert操作之后获取到的主键id

RowsAffect() : 影响到的行数

Rows：

Columns() : 返回的数据有哪些列，其实就是返回的表列名

Close() : 关闭Rows，调用之后不能再进行任何操作

Next() : 将下一行的数据取到des[] Value中。这里的Value接口可以是int64，float64，bool，[]byte，string，time.Time

## 下面回到Begin，返回了Tx

开始事务之后除了查询之外，就两种行为：Commit和Rollback，这两种行为都是Tx接口的方法

drvier中的结构全是接口性质的，它需要你来实现并注册到Register中。

# 驱动的具体使用是在database/sql中

首先将几个sql的结构看一遍

首先sql中的结构都是对driver中的结构进行了一层封装，比如像Rows，有个内部属性是rowsi driver.Rows。

对sql的实际操作都是使用driver.Rows里面的接口进行操作的，实际也就是使用你自己实现的driver进行操作。

driver和sql就像插头和一个充满插头的汽车一样，你实现了driver，即实现了这些插头的配置，你就可使用sql这个汽车了。

Result ： 和driver中的Result一致，即你如果实现了driver.Result，那你自然就实现了sql.Result。它是个接口，其实没有什么特殊的用处，如果sql包中的所有Result都换成driver.Result也是行的，估计是作者希望返回值不要引用到其他包去，就使用这种方法。

Rows :  基于了driver.Rows, 还在上面扩展了几个其他的方法。拥有方法：

Close

Cloumns

Err

Next

Scan

Stmt ：基于driver.Stmt。拥有方法

Close

Exec

Query

QueryRow

Tx：基于driver.Tx。拥有方法：

Commit

Exec

Prepare

Query

QueryRow

Rollback

Stmt

从sql.Open开始

返回了sql.DB结构，这个结构是实现了driver.Conn结构，除了Conn已有的Prepare，Begin，Close之外，还直接多了几个查询方法：

Driver() : 返回当前驱动

Exec() : 直接进行操作

Query() : 进行查询，返回Rows

QueryRow() : 预期返回单行，返回Row

不管Rows还是Row都有个很好用的方法Scan来将数据放到制定的变量中去。

比如下面就是典型的Rows用法

Prepare返回Stmt结构

Exec返回Result结构

至于这几个结构也有各自的方法。

![Creative Commons License](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/14f76bcd-645b-4d68-b3e3-2d26b9bc3b3b/index_files/b9092559-fe68-4c91-a749-81758e2f4eb4.png)

专注Web开发50年。请加群：[![Thinking in Web](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/14f76bcd-645b-4d68-b3e3-2d26b9bc3b3b/index_files/dda98066-3976-4e98-b90b-33c79de0ef2b.png)](http://shang.qq.com/wpa/qunwpa?idkey=017d388202803b8f514317087f69ffabc5a1340342943a0fd8d3af1148653eb5)

本文基于[署名-非商业性使用 3.0](http://creativecommons.org/licenses/by-nc/3.0/deed.zh)许可协议发布，欢迎转载，演绎，但是必须保留本文的署名[叶剑峰](http://www.cnblogs.com/yjf512/)（包含链接http://www.cnblogs.com/yjf512/），且不得用于商业目的。如您有任何疑问或者授权方面的协商，请[与我联系](mailto:jianfengye110@gmail.com)。

来源： <<http://www.cnblogs.com/yjf512/archive/2013/01/23/2872577.html>>