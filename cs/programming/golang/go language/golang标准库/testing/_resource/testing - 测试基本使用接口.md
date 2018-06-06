## [testing - 测试基本使用接口](http://www.cnblogs.com/yjf512/p/4237077.html)

2015-01-20 18:49 by 轩脉刃, 704 阅读, 0 评论, [收藏](http://www.cnblogs.com/yjf512/p/4237077.html#), [编辑](http://i.cnblogs.com/EditPosts.aspx?postid=4237077)

# testing - 测试基本使用接口

当你写完一个函数，结构体，main之后，你下一步需要的就是测试了。testing包提供了很简单易用的测试包。

# 写一个基本的测试用例

测试文件的文件名需要以_test.go为结尾，测试用例需要以TestXxxx的样式存在。

比如我要测试utils包的sql.go中的函数：

```
func GetOne(db *sql.DB, query string, args ...interface{}) (map[string][]byte, error) {
```

就需要创建一个sql_test.go

```
package utils

import (
    "database/sql"
    _ "fmt"
    _ "github.com/go-sql-driver/mysql""strconv""testing"
)

func Test_GetOne(t *testing.T) {
    db, err := sql.Open("mysql", "root:123.abc@tcp(192.168.33.10:3306)/test")
    defer func() {
        db.Close()
    }()
    if err != nil {
        t.Fatal(err)
    }

    // 测试empty
    car_brand, err := GetOne(db, "select * from user where id = 999999")
    if (car_brand != nil) || (err != nil) {
        t.Fatal("emtpy测试错误")
    }
}
```

# testing的测试用例形式

测试用例有四种形式：
TestXxxx(t *testing.T) // 基本测试用例BenchmarkXxxx(b *testing.B) // 压力测试的测试用例
Example_Xxx() // 测试控制台输出的例子
TestMain(m *testing.M) // 测试Main函数

给个Example的例子:（Example需要在最后用注释的方式确认控制台输出和预期是不是一致的）

```
func Example_GetScore() {
    score := getScore(100, 100, 100, 2.1)
    fmt.Println(score)
    // Output:// 31.1
}
```

# testing的变量

gotest的变量有这些：

- test.short : 一个快速测试的标记，在测试用例中可以使用testing.Short()来绕开一些测试
- test.outputdir : 输出目录
- test.coverprofile : 测试覆盖率参数，指定输出文件
- test.run : 指定正则来运行某个/某些测试用例
- test.memprofile : 内存分析参数，指定输出文件
- test.memprofilerate : 内存分析参数，内存分析的抽样率
- test.cpuprofile : cpu分析输出参数，为空则不做cpu分析
- test.blockprofile : 阻塞事件的分析参数，指定输出文件
- test.blockprofilerate : 阻塞事件的分析参数，指定抽样频率
- test.timeout : 超时时间
- test.cpu : 指定cpu数量
- test.parallel : 指定运行测试用例的并行数

# testing包内的结构

- B : 压力测试
- BenchmarkResult : 压力测试结果
- Cover : 代码覆盖率相关结构体
- CoverBlock : 代码覆盖率相关结构体
- InternalBenchmark : 内部使用的结构
- InternalExample : 内部使用的结构
- InternalTest : 内部使用的结构
- M : main测试使用的结构
- PB : Parallel benchmarks 并行测试使用结果
- T : 普通测试用例
- TB : 测试用例的接口

# testing的通用方法

T结构内部是继承自common结构，common结构提供集中方法，是我们经常会用到的：

当我们遇到一个断言错误的时候，我们就会判断这个测试用例失败，就会使用到：

```
Fail  : case失败，测试用例继续
FailedNow : case失败，测试用例中断
```

当我们遇到一个断言错误，只希望跳过这个错误，但是不希望标示测试用例失败，会使用到：

```
SkipNow : case跳过，测试用例不继续
```

当我们只希望在一个地方打印出信息，我们会用到:

```
Log : 输出信息
Logf : 输出有format的信息
```

当我们希望跳过这个用例，并且打印出信息:

```
Skip : Log + SkipNow
Skipf : Logf + SkipNow
```

当我们希望断言失败的时候，测试用例失败，打印出必要的信息，但是测试用例继续：

```
Error : Log + Fail
Errorf : Logf + Fail
```

当我们希望断言失败的时候，测试用例失败，打印出必要的信息，测试用例中断：

```
Fatal : Log + FailNow
Fatalf : Logf + FailNow
```

![Creative Commons License](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/7bc7695b-8d0d-46d4-ac2e-4197e4c597b9/index_files/298954b8-c850-4f29-9ace-5cd642d86723.png)

专注Web开发50年。请加群：[![Thinking in Web](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/7bc7695b-8d0d-46d4-ac2e-4197e4c597b9/index_files/6172cbab-d0fd-4526-96bf-f56f8ef302dc.png)](http://shang.qq.com/wpa/qunwpa?idkey=017d388202803b8f514317087f69ffabc5a1340342943a0fd8d3af1148653eb5)

本文基于[署名-非商业性使用 3.0](http://creativecommons.org/licenses/by-nc/3.0/deed.zh)许可协议发布，欢迎转载，演绎，但是必须保留本文的署名[叶剑峰](http://www.cnblogs.com/yjf512/)（包含链接http://www.cnblogs.com/yjf512/），且不得用于商业目的。如您有任何疑问或者授权方面的协商，请[与我联系](mailto:jianfengye110@gmail.com)。

