# [标准库—命令行参数解析flag](http://blog.studygolang.com/2013/02/%e6%a0%87%e5%87%86%e5%ba%93-%e5%91%bd%e4%bb%a4%e8%a1%8c%e5%8f%82%e6%95%b0%e8%a7%a3%e6%9e%90flag/)

 [Go包(标准库）](http://blog.studygolang.com/category/package/) [添加评论](http://blog.studygolang.com/2013/02/%E6%A0%87%E5%87%86%E5%BA%93-%E5%91%BD%E4%BB%A4%E8%A1%8C%E5%8F%82%E6%95%B0%E8%A7%A3%E6%9E%90flag/#respond)

二232013

 

[0]()

在写命令行程序（工具、server）时，对命令参数进行解析是常见的需求。各种语言一般都会提供解析命令行参数的方法或库，以方便程序员使用。如果命令行参数纯粹自己写代码解析，对于比较复杂的，还是挺费劲的。在go标准库中提供了一个包：flag，方便进行命令行解析。

注：区分几个概念
1）命令行参数（或参数）：是指运行程序提供的参数
2）已定义命令行参数：是指程序中通过flag.Xxx等这种形式定义了的参数
3）非flag（non-flag）命令行参数（或保留的命令行参数）：后文解释

## 一、flag包概述

flag包实现了命令行参数的解析。

定义flags有两种方式：

### 1）flag.Xxx()，其中Xxx可以是Int、String等；返回一个相应类型的指针，如：

| `1`  | `var ip = flag.Int(``"flagname"``, 1234, ``"help message for flagname"``)` |
| ---- | ---------------------------------------- |
|      |                                          |

### 2）flag.XxxVar()，将flag绑定到一个变量上，如：

| `1`  | `var flagvar ``int` |
| ---- | ------------------- |
|      |                     |

| `2`  | `flag.IntVar(&amp;flagvar, ``"flagname"``, 1234, ``"help message for flagname"``)` |
| ---- | ---------------------------------------- |
|      |                                          |

另外，还可以创建自定义flag，只要实现flag.Value接口即可（要求receiver是指针），这时候可以通过如下方式定义该flag：

| `1`  | `flag.Var(&amp;flagVal, ``"name"``, ``"help message for flagname"``)` |
| ---- | ---------------------------------------- |
|      |                                          |

注意到，这种方式是没有提供默认值的，所以默认值就是类型的零值。

在所有的flag定义完成之后，可以通过调用flag.Parse()进行解析。

命令行flag的语法有如下三种形式：
-flag // 只支持bool类型
-flag=x
-flag x // 只支持非bool类型

其中第三种形式只能用于非bool类型的flag，原因是：如果支持，那么对于这样的命令 cmd -x *，如果有一个文件名字是：0或false等，则命令的愿意会改变（之所以这样，是因为bool类型支持-flag这种形式，如果bool类型不支持-flag这种形式，则bool类型可以和其他类型一样处理。也正因为这样，Parse()中，对bool类型进行了特殊处理）。默认的，提供了-flag，则对应的值为true，否则为flag.Bool/BoolVar中指定的默认值；如果希望显示设置为false则使用-flag=false。

int类型可以是十进制、十六进制、八进制甚至是负数；bool类型可以是1, 0, t, f, true, false, TRUE, FALSE, True, False。Duration可以接受任何time.ParseDuration能解析的类型

## 二、类型和函数

在看类型和函数之前，先看一下变量
**ErrHelp**：该错误类型用于当命令行指定了-help参数但没有定义时。
**Usage**：这是一个函数，用户输出所有定义了的命令行参数和帮助信息（usage message）。一般，当命令行参数解析出错时，该函数会被调用。我们可以指定自己的Usage函数，即：flag.Usage = func(){}

### 1、函数

go标准库中，经常这么做：

> 定义了一个类型，提供了很多方法；为了方便使用，会实例化一个该类型的实例（通用），这样便可以直接使用该实例调用方法。比如：encoding/base64中提供了StdEncoding和URLEncoding实例，使用时：base64.StdEncoding.Encode()

在flag包中，进行了进一步封装：将FlagSet的方法都重新定义了一遍，也就是提供了一序列函数，而函数中只是简单的调用已经实例化好了的FlagSet实例：commandLine 的方法，这样commandLine实例便不需要export。这样，使用者是这么调用：flag.Parse()而不是flag.commandLine.Parse()

这里不详细介绍各个函数，在类型方法中介绍。

### 2、类型（数据结构）

**1）ErrorHandling**

| `1`  | `type ErrorHandling ``int` |
| ---- | -------------------------- |
|      |                            |

该类型定义了在参数解析出错时错误处理方式定义了三个该类型的常量：

| `1`  | `const` `(` |
| ---- | ----------- |
|      |             |

| `2`  | `    ``ContinueOnError ErrorHandling = iota` |
| ---- | ---------------------------------------- |
|      |                                          |

| `3`  | `    ``ExitOnError` |
| ---- | ------------------- |
|      |                     |

| `4`  | `    ``PanicOnError` |
| ---- | -------------------- |
|      |                      |

| `5`  | `)`  |
| ---- | ---- |
|      |      |

三个常量在源码的FlagSet方法parseOne()中使用了。

**2）Flag**

| `1`  | `// A Flag represents the state of a flag.` |
| ---- | ---------------------------------------- |
|      |                                          |

| `2`  | `type Flag ``struct` `{` |
| ---- | ------------------------ |
|      |                          |

| `3`  | `    ``Name     string ``// name as it appears on command line` |
| ---- | ---------------------------------------- |
|      |                                          |

| `4`  | `    ``Usage    string ``// help message` |
| ---- | ---------------------------------------- |
|      |                                          |

| `5`  | `    ``Value    Value  ``// value as set` |
| ---- | ---------------------------------------- |
|      |                                          |

| `6`  | `    ``DefValue string ``// default value (as text); for usage message` |
| ---- | ---------------------------------------- |
|      |                                          |

| `7`  | `}`  |
| ---- | ---- |
|      |      |

Flag类型代表一个flag的状态。

比如：autogo -f abc.txt，代码flag.String(“f”, “a.txt”, “usage”)，则该Flag实例（可以通过flag.Lookup(“f”)获得）相应的值为：f, usage, abc.txt, a.txt。
****

**3）FlagSet**

| `1`  | `// A FlagSet represents a set of defined flags.` |
| ---- | ---------------------------------------- |
|      |                                          |

| `2`  | `type FlagSet ``struct` `{` |
| ---- | --------------------------- |
|      |                             |

| `3`  | `    ``// Usage is the function called when an error occurs while parsing flags.` |
| ---- | ---------------------------------------- |
|      |                                          |

| `4`  | `    ``// The field is a function (not a method) that may be changed to point to` |
| ---- | ---------------------------------------- |
|      |                                          |

| `5`  | `    ``// a custom error handler.` |
| ---- | ---------------------------------- |
|      |                                    |

| `6`  | `    ``Usage func()` |
| ---- | -------------------- |
|      |                      |

| `7`  |      |
| ---- | ---- |
|      |      |

| `8`  | `    ``name string ``// FlagSet的名字。commandLine给的是os.Args[0]` |
| ---- | ---------------------------------------- |
|      |                                          |

| `9`  | `    ``parsed ``bool` `// 是否执行过Parse()` |
| ---- | --------------------------------------- |
|      |                                         |

| `10` | `    ``actual map[string]*Flag ``// 存放实际传递了的参数（即命令行参数）` |
| ---- | ---------------------------------------- |
|      |                                          |

| `11` | `    ``formal map[string]*Flag ``// 存放所有已定义命令行参数` |
| ---- | ---------------------------------------- |
|      |                                          |

| `12` | `    ``args []string ``// arguments after flags // 存放非flag（non-flag）参数` |
| ---- | ---------------------------------------- |
|      |                                          |

| `13` | `    ``exitOnError ``bool` `// does the program exit if there's an error?` |
| ---- | ---------------------------------------- |
|      |                                          |

| `14` | `    ``errorHandling ErrorHandling ``// 当解析出错时，处理错误的方式` |
| ---- | ---------------------------------------- |
|      |                                          |

| `15` | `    ``output io.Writer ``// nil means stderr; use out() accessor` |
| ---- | ---------------------------------------- |
|      |                                          |

| `16` | `}`  |
| ---- | ---- |
|      |      |

4）Value接口

| `1`  | `// Value is the interface to the dynamic value stored in a flag.` |
| ---- | ---------------------------------------- |
|      |                                          |

| `2`  | `// (The default value is represented as a string.)` |
| ---- | ---------------------------------------- |
|      |                                          |

| `3`  | `type Value interface {` |
| ---- | ------------------------ |
|      |                          |

| `4`  | `    ``String() string` |
| ---- | ----------------------- |
|      |                         |

| `5`  | `    ``Set(string) error` |
| ---- | ------------------------- |
|      |                           |

| `6`  | `}`  |
| ---- | ---- |
|      |      |

所有参数类型需要实现Value接口，flag包中，为int、float、bool等实现了该接口。借助该接口，我们可以自定义flag

## 三、主要类型的方法（包括类型实例化）

flag包中主要是FlagSet类型。

### 1、实例化方式

NewFlagSet()用于实例化FlagSet。预定义的FlagSet实例commandLine的定义方式：

| `1`  | `// The default set of command-line flags, parsed from os.Args.` |
| ---- | ---------------------------------------- |
|      |                                          |

| `2`  | `var commandLine = NewFlagSet(os.Args[0], ExitOnError)` |
| ---- | ---------------------------------------- |
|      |                                          |

可见，默认的FlagSet实例在解析出错时会提出程序。

由于FlagSet中的字段没有export，其他方式获得FlagSet实例后，比如：FlagSet{}或new(FlagSet)，应该调用Init()方法，初始化name和errorHandling。

### 2、定义flag参数方法

这一序列的方法都有两种形式，在一开始已经说了两种方式的区别。这些方法用于定义某一类型的flag参数。

### 3、解析参数（Parse）

| `1`  | `func (f *FlagSet) Parse(arguments []string) error` |
| ---- | ---------------------------------------- |
|      |                                          |

从参数列表中解析定义的flag。参数arguments不包括命令名，即应该是os.Args[1:]。事实上，flag.Parse()函数就是这么做的：

| `1`  | `// Parse parses the command-line flags from os.Args[1:].  Must be called` |
| ---- | ---------------------------------------- |
|      |                                          |

| `2`  | `// after all flags are defined and before flags are accessed by the program.` |
| ---- | ---------------------------------------- |
|      |                                          |

| `3`  | `func Parse() {` |
| ---- | ---------------- |
|      |                  |

| `4`  | `    ``// Ignore errors; commandLine is set for ExitOnError.` |
| ---- | ---------------------------------------- |
|      |                                          |

| `5`  | `    ``commandLine.Parse(os.Args[1:])` |
| ---- | -------------------------------------- |
|      |                                        |

| `6`  | `}`  |
| ---- | ---- |
|      |      |

该方法应该在flag参数定义后而具体参数值被访问前调用。

如果提供了-help参数（命令中给了）但没有定义（代码中），该方法返回ErrHelp错误。默认的commandLine，在Parse出错时会退出（ExitOnError）

为了更深入的理解，我们看一下Parse(arguments []string)的源码：

| `1`  | `func (f *FlagSet) Parse(arguments []string) error {` |
| ---- | ---------------------------------------- |
|      |                                          |

| `2`  | `    ``f.parsed = ``true` |
| ---- | ------------------------- |
|      |                           |

| `3`  | `    ``f.args = arguments` |
| ---- | -------------------------- |
|      |                            |

| `4`  | `    ``for` `{` |
| ---- | --------------- |
|      |                 |

| `5`  | `        ``seen, err := f.parseOne()` |
| ---- | ------------------------------------- |
|      |                                       |

| `6`  | `        ``if` `seen {` |
| ---- | ----------------------- |
|      |                         |

| `7`  | `            ``continue` |
| ---- | ------------------------ |
|      |                          |

| `8`  | `        ``}` |
| ---- | ------------- |
|      |               |

| `9`  | `        ``if` `err == nil {` |
| ---- | ----------------------------- |
|      |                               |

| `10` | `            ``break` |
| ---- | --------------------- |
|      |                       |

| `11` | `        ``}` |
| ---- | ------------- |
|      |               |

| `12` | `        ``switch` `f.errorHandling {` |
| ---- | -------------------------------------- |
|      |                                        |

| `13` | `        ``case` `ContinueOnError:` |
| ---- | ----------------------------------- |
|      |                                     |

| `14` | `            ``return` `err` |
| ---- | ---------------------------- |
|      |                              |

| `15` | `        ``case` `ExitOnError:` |
| ---- | ------------------------------- |
|      |                                 |

| `16` | `            ``os.Exit(2)` |
| ---- | -------------------------- |
|      |                            |

| `17` | `        ``case` `PanicOnError:` |
| ---- | -------------------------------- |
|      |                                  |

| `18` | `            ``panic(err)` |
| ---- | -------------------------- |
|      |                            |

| `19` | `        ``}` |
| ---- | ------------- |
|      |               |

| `20` | `    ``}` |
| ---- | --------- |
|      |           |

| `21` | `    ``return` `nil` |
| ---- | -------------------- |
|      |                      |

| `22` | `}`  |
| ---- | ---- |
|      |      |

真正解析参数的方法是非导出方法parseOne。

结合parseOne方法，我们来解释non-flag以及包文档中的这句话：

> Flag parsing stops just before the first non-flag argument (“-” is a non-flag argument) or after the terminator “–”.

**我们需要了解解析什么时候停止**

根据Parse()中for循环终止的条件（不考虑解析出错），我们知道，当parseOne返回false, nil时，Parse解析终止。正常解析完成我们不考虑。看一下parseOne的源码发现，有两处会返回false, nil。

**1）第一个non-flag参数**

| `1`  | `s := f.args[0]` |
| ---- | ---------------- |
|      |                  |

| `2`  | `if` `len(s) == 0 || s[0] != ``'-'` `|| len(s) == 1 {` |
| ---- | ---------------------------------------- |
|      |                                          |

| `3`  | `    ``return` `false``, nil` |
| ---- | ----------------------------- |
|      |                               |

| `4`  | `}`  |
| ---- | ---- |
|      |      |

也就是，当遇到单独的一个”-”或不是”-”开始时，会停止解析。比如：

./autogo – -f或./autogo build -f

这两种情况，-f都不会被正确解析。**像该例子中的”-”或build（以及之后的参数），我们称之为non-flag参数**

**2）两个连续的”–”**

| `1`  | `if` `s[1] == ``'-'` `{` |
| ---- | ------------------------ |
|      |                          |

| `2`  | `    ``num_minuses++` |
| ---- | --------------------- |
|      |                       |

| `3`  | `    ``if` `len(s) == 2 { ``// "--" terminates the flags` |
| ---- | ---------------------------------------- |
|      |                                          |

| `4`  | `        ``f.args = f.args[1:]` |
| ---- | ------------------------------- |
|      |                                 |

| `5`  | `        ``return` `false``, nil` |
| ---- | --------------------------------- |
|      |                                   |

| `6`  | `    ``}` |
| ---- | --------- |
|      |           |

| `7`  | `}`  |
| ---- | ---- |
|      |      |

也就是，当遇到联系的两个”-”时，解析停止。

说明：这里说的”-”和”–”，位置和”-f”这种的一样。也就是说，下面这种情况并不是这里说的：

./autogo -f –

这里的”–”会被当成是f的值

parseOne方法中接下来是处理-flag=x，然后是-flag（bool类型）（这里对bool进行了特殊处理），接着是-flag x这种形式，最后，将解析成功的Flag实例存入FlagSet的actual map中。

另外，在parseOne中有这么一句：

| `1`  | `f.args = f.args[1:]` |
| ---- | --------------------- |
|      |                       |

也就是说，没执行成功一次parseOne，f.args会少一个。所以，FlagSet中的args最后留下来的就是所有non-flag参数。

### 4、Arg(i int)和Args()、NArg()、NFlag()

Arg(i int)和Args()这两个方法就是获取non-flag参数的；NArg()获得non-flag个数；NFlag()获得FlagSet中actual长度（即被设置了的参数个数）。

### 5、Visit/VisitAll

这两个函数分别用户访问FlatSet的actual和formal中的Flag，而具体的访问方式由调用者决定。

### 6、PrintDefaults()

打印所有已定义参数的默认值（调用VisitAll），默认输出到标准错误，除非指定了FlagSet的output（通过SetOutput()设置）

### 7、Set(name, value string)

设置某个flag的值（通过Flag的name）

## 四、总结

使用建议：虽然上面讲了那么多，一般来说，我们只简单的定义flag，然后parse，就如同开始的例子一样。

如果项目需要复杂或更高级的命令行解析方式，可以试试[goptions](https://github.com/polaris1119/goptions)包

如果想要像go工具那样的多命令（子命令）处理方式，可以试试[command](https://github.com/polaris1119/command)

 [polaris](http://blog.studygolang.com/author/polaris/) 发表于 21:30

来源： <<http://blog.studygolang.com/2013/02/%E6%A0%87%E5%87%86%E5%BA%93-%E5%91%BD%E4%BB%A4%E8%A1%8C%E5%8F%82%E6%95%B0%E8%A7%A3%E6%9E%90flag/>>

 