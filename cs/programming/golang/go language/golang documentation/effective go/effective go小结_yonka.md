

effective go小结_yonka

# INDEX

- Introduction
  - [Examples](https://note.wiz.cn/web?dc=2d2636a2-ddc2-4ff4-bf4f-1165a05f015c&cmd=ca%2C%2FProgramming%2FGo%2Fgo%20language%2Fdocumentation%2Feffective%20go%2F&kb=89077880-eff4-11e0-a402-00237def97cc#Examples)
- [Formatting](https://note.wiz.cn/web?dc=2d2636a2-ddc2-4ff4-bf4f-1165a05f015c&cmd=ca%2C%2FProgramming%2FGo%2Fgo%20language%2Fdocumentation%2Feffective%20go%2F&kb=89077880-eff4-11e0-a402-00237def97cc#Formatting)
- [Commentary](https://note.wiz.cn/web?dc=2d2636a2-ddc2-4ff4-bf4f-1165a05f015c&cmd=ca%2C%2FProgramming%2FGo%2Fgo%20language%2Fdocumentation%2Feffective%20go%2F&kb=89077880-eff4-11e0-a402-00237def97cc#Commentary)
- Names
  - [Package names](#Package names)
  - [Getters](https://note.wiz.cn/web?dc=2d2636a2-ddc2-4ff4-bf4f-1165a05f015c&cmd=ca%2C%2FProgramming%2FGo%2Fgo%20language%2Fdocumentation%2Feffective%20go%2F&kb=89077880-eff4-11e0-a402-00237def97cc#Getters)
  - [Interface names](#Interface names)
  - [MixedCaps](https://note.wiz.cn/web?dc=2d2636a2-ddc2-4ff4-bf4f-1165a05f015c&cmd=ca%2C%2FProgramming%2FGo%2Fgo%20language%2Fdocumentation%2Feffective%20go%2F&kb=89077880-eff4-11e0-a402-00237def97cc#MixedCaps)
- [Semicolons](https://note.wiz.cn/web?dc=2d2636a2-ddc2-4ff4-bf4f-1165a05f015c&cmd=ca%2C%2FProgramming%2FGo%2Fgo%20language%2Fdocumentation%2Feffective%20go%2F&kb=89077880-eff4-11e0-a402-00237def97cc#Semicolons)
- [Control structures](#Control structures)
  - [If](https://note.wiz.cn/web?dc=2d2636a2-ddc2-4ff4-bf4f-1165a05f015c&cmd=ca%2C%2FProgramming%2FGo%2Fgo%20language%2Fdocumentation%2Feffective%20go%2F&kb=89077880-eff4-11e0-a402-00237def97cc#If)
  - [Redeclaration and reassignment](#Redeclaration and reassignment)
  - [For](https://note.wiz.cn/web?dc=2d2636a2-ddc2-4ff4-bf4f-1165a05f015c&cmd=ca%2C%2FProgramming%2FGo%2Fgo%20language%2Fdocumentation%2Feffective%20go%2F&kb=89077880-eff4-11e0-a402-00237def97cc#For)
  - [Switch](https://note.wiz.cn/web?dc=2d2636a2-ddc2-4ff4-bf4f-1165a05f015c&cmd=ca%2C%2FProgramming%2FGo%2Fgo%20language%2Fdocumentation%2Feffective%20go%2F&kb=89077880-eff4-11e0-a402-00237def97cc#Switch)
  - [Type switch](#Type switch)
- Functions
  - [Multiple return values](#Multiple return values)
  - [Named result parameters](#Named result parameters)
  - [Defer](https://note.wiz.cn/web?dc=2d2636a2-ddc2-4ff4-bf4f-1165a05f015c&cmd=ca%2C%2FProgramming%2FGo%2Fgo%20language%2Fdocumentation%2Feffective%20go%2F&kb=89077880-eff4-11e0-a402-00237def97cc#Defer)
- Data
  - [Allocation with new](#Allocation with new)
  - [Constructors and composite literals](#Constructors and composite literals)
  - [Allocation with make](#Allocation with make)
  - [Arrays](https://note.wiz.cn/web?dc=2d2636a2-ddc2-4ff4-bf4f-1165a05f015c&cmd=ca%2C%2FProgramming%2FGo%2Fgo%20language%2Fdocumentation%2Feffective%20go%2F&kb=89077880-eff4-11e0-a402-00237def97cc#Arrays)
  - [Slices](https://note.wiz.cn/web?dc=2d2636a2-ddc2-4ff4-bf4f-1165a05f015c&cmd=ca%2C%2FProgramming%2FGo%2Fgo%20language%2Fdocumentation%2Feffective%20go%2F&kb=89077880-eff4-11e0-a402-00237def97cc#Slices)
  - [Two-dimensional slices](#Two-dimensional slices)
  - [Maps](https://note.wiz.cn/web?dc=2d2636a2-ddc2-4ff4-bf4f-1165a05f015c&cmd=ca%2C%2FProgramming%2FGo%2Fgo%20language%2Fdocumentation%2Feffective%20go%2F&kb=89077880-eff4-11e0-a402-00237def97cc#Maps)
  - [Printing](https://note.wiz.cn/web?dc=2d2636a2-ddc2-4ff4-bf4f-1165a05f015c&cmd=ca%2C%2FProgramming%2FGo%2Fgo%20language%2Fdocumentation%2Feffective%20go%2F&kb=89077880-eff4-11e0-a402-00237def97cc#Printing)
  - [Append](https://note.wiz.cn/web?dc=2d2636a2-ddc2-4ff4-bf4f-1165a05f015c&cmd=ca%2C%2FProgramming%2FGo%2Fgo%20language%2Fdocumentation%2Feffective%20go%2F&kb=89077880-eff4-11e0-a402-00237def97cc#Append)
- Initialization
  - [Constants](https://note.wiz.cn/web?dc=2d2636a2-ddc2-4ff4-bf4f-1165a05f015c&cmd=ca%2C%2FProgramming%2FGo%2Fgo%20language%2Fdocumentation%2Feffective%20go%2F&kb=89077880-eff4-11e0-a402-00237def97cc#Constants)
  - [Variables](https://note.wiz.cn/web?dc=2d2636a2-ddc2-4ff4-bf4f-1165a05f015c&cmd=ca%2C%2FProgramming%2FGo%2Fgo%20language%2Fdocumentation%2Feffective%20go%2F&kb=89077880-eff4-11e0-a402-00237def97cc#Variables)
  - [The init function](#The init function)
- Methods
  - [Pointers vs. Values](#Pointers vs. Values)
- [Interfaces and other types](#Interfaces and other types)
  - [Interfaces](https://note.wiz.cn/web?dc=2d2636a2-ddc2-4ff4-bf4f-1165a05f015c&cmd=ca%2C%2FProgramming%2FGo%2Fgo%20language%2Fdocumentation%2Feffective%20go%2F&kb=89077880-eff4-11e0-a402-00237def97cc#Interfaces)
  - [Conversions](https://note.wiz.cn/web?dc=2d2636a2-ddc2-4ff4-bf4f-1165a05f015c&cmd=ca%2C%2FProgramming%2FGo%2Fgo%20language%2Fdocumentation%2Feffective%20go%2F&kb=89077880-eff4-11e0-a402-00237def97cc#Conversions)
  - [Interface conversions and type assertions](#Interface conversions and type assertions)
  - [Generality](https://note.wiz.cn/web?dc=2d2636a2-ddc2-4ff4-bf4f-1165a05f015c&cmd=ca%2C%2FProgramming%2FGo%2Fgo%20language%2Fdocumentation%2Feffective%20go%2F&kb=89077880-eff4-11e0-a402-00237def97cc#Generality)
  - [Interfaces and methods](#Interfaces and methods)
- [The blank identifier](#The blank identifier)
  - [The blank identifier in multiple assignment](#The blank identifier in multiple assignment)
  - [Unused imports and variables](#Unused imports and variables)
  - [Import for side effect](#Import for side effect)
  - [Interface checks](#Interface checks)
- [Embedding](https://note.wiz.cn/web?dc=2d2636a2-ddc2-4ff4-bf4f-1165a05f015c&cmd=ca%2C%2FProgramming%2FGo%2Fgo%20language%2Fdocumentation%2Feffective%20go%2F&kb=89077880-eff4-11e0-a402-00237def97cc#Embedding)
- Concurrency
  - [Share by communicating](#Share by communicating)
  - [Goroutines](https://note.wiz.cn/web?dc=2d2636a2-ddc2-4ff4-bf4f-1165a05f015c&cmd=ca%2C%2FProgramming%2FGo%2Fgo%20language%2Fdocumentation%2Feffective%20go%2F&kb=89077880-eff4-11e0-a402-00237def97cc#Goroutines)
  - [Channels](https://note.wiz.cn/web?dc=2d2636a2-ddc2-4ff4-bf4f-1165a05f015c&cmd=ca%2C%2FProgramming%2FGo%2Fgo%20language%2Fdocumentation%2Feffective%20go%2F&kb=89077880-eff4-11e0-a402-00237def97cc#Channels)
  - [Channels of channels](#Channels of channels)
  - [Parallelization](https://note.wiz.cn/web?dc=2d2636a2-ddc2-4ff4-bf4f-1165a05f015c&cmd=ca%2C%2FProgramming%2FGo%2Fgo%20language%2Fdocumentation%2Feffective%20go%2F&kb=89077880-eff4-11e0-a402-00237def97cc#Parallelization)
  - [A leaky buffer](#A leaky buffer)
- Errors
  - [Panic](https://note.wiz.cn/web?dc=2d2636a2-ddc2-4ff4-bf4f-1165a05f015c&cmd=ca%2C%2FProgramming%2FGo%2Fgo%20language%2Fdocumentation%2Feffective%20go%2F&kb=89077880-eff4-11e0-a402-00237def97cc#Panic)
  - [Recover](https://note.wiz.cn/web?dc=2d2636a2-ddc2-4ff4-bf4f-1165a05f015c&cmd=ca%2C%2FProgramming%2FGo%2Fgo%20language%2Fdocumentation%2Feffective%20go%2F&kb=89077880-eff4-11e0-a402-00237def97cc#Recover)
- [A web server](#A web server)

# Introduction介绍

go参考了很多已有语言，但也有自己的特点，不应该直接用java、c++的方式套用到go来判断好坏。 而且尝试从“go”的角度来思考问题才会得到独特的程序。 
对于编写golang，理解golang的特点和习语很重要。

## Examples

参见标准库，是很好的样例。 大多数都包含使用样例。

# Formatting格式

鼓励同一种风格。 
gofmt 可以/应该用于强制统一风格。

# Commentary注释

支持c风格的/**/和c++风格的//，后者为主。前者一般只用在包注释，不过在表达式内的注释和 disable large swaths of code 的场景也有使用。 
godoc程序（同时也可以作为web服务）可以从代码内容中提取文档。 顶级声明之前的无换行分隔的注释会（和声明一起，作为声明的解释）被提取。 所以注释要好好写 = = 
包注释必须（非语法强制）要有，可以在同包任何一个文件中，使用块注释风格。 如果非常短的话可也可以使用行注释风格。

```
/*Package regexp implements a simple library for regular expressions.The syntax of the regular expressions accepted is:    regexp:        concatenation { '|' concatenation }    concatenation:        { closure }    closure:        term [ '*' | '+' | '?' ]    term:        '^'        '$'        '.'        character        '[' [ '^' ] character-ranges ']'        '(' regexp ')'*/package regexp
```

因为godoc不会对注释内容做处理，所以注意保持好的格式。 
可以对group的声明做统一的注释，如下：

```
// Error codes returned by failures to parse an expression.var (    ErrInternal      = errors.New("regexp: internal error")    ErrUnmatchedLpar = errors.New("regexp: unmatched '('")    ErrUnmatchedRpar = errors.New("regexp: unmatched ')'")    ...)
```

# Names

go中的命名很重要，甚至有语义，如首字母大小写决定了是否export。

## Package names

包名是包中内容的“namespace”而不是目录名，import "a/b"而b下面的包名可以是c，那么此时应该用 c.DoSth()而不是b.DoSth()。

------

多数情况下路径名会包含一定的信息，也即 a/b下面的方法DoSth就没必要命名为 ABDoSth或者 DoASth了。

------

可以在import是为包定义别名，如 import v1 "a/b"

------

go鼓励短命名，即使可能并不能完整表达函数（或其他）的含义，go更鼓励用注释去解释具体功能。 如： once.Do vs once.DoOrWaitUntilDone = =

## Getters

go不从语法层面支持，但可以自定义。 
但go鼓励用Xxx而不是GetXxx的命名方式来作为xxx的getter。 
对于setters，go建议用SetXxx。 = =

## Interface names

对于单方法的接口，go建议用+er后缀的方式，比如Read方法的接口命名为Reader。 
很多内置的方法都有特定的含义，如 Read、Write、Close、Flush、String等，为了避免混淆，只有当确定符合（函数签名）和含义的时候，才把函数命名为…，避免**“误”实现接口**。 当然，如果明确是要实现该接口的话，那就go ahead…

## MixedCaps

驼峰法而不是下划线

# Semicolons

和c一样go的正式语法页需要分号来分隔语句，不过可以被词法分析器来自动填充，规则如下： 
*如果一行的最后是标示符（包括int、float64等），或者基本的书面量，或者 break continue fallthrough return ++ -- ) }这几个token之一，那么词法分析器会自动在该token之后插入分号* 
也即 **如果行末是一个可以结束语句的token，则插入分号**

------

A semicolon can also be omitted immediately before a closing brace, so a statement such as

```
go func() { for { dst <- <-src } }()

```

needs no semicolons. Idiomatic Go programs have semicolons only in places such as for loop clauses, to separate the initializer, condition, and continuation elements. They are also necessary to separate multiple statements on a line, should you write code that way.

------

分号插入规则带来一个限制： 不能把左大括号放在新行里，否则的话上一行的末尾会被插入一个分号，带来一些非期望的结果

```
if i < f() {    g()}// not like thisif i < f()  // wrong!{           // wrong!    g()}
```

# Control structures

类似C但在一些重要的方面有所不同

1. 没有do和while
2. for更通用（取代了while）
3. switch支持的场景更丰富
4. if和switch都接受初始化语句，类似for
5. break和continue支持label来指定break/continue到哪
6. 新引入select结构

## If

```
if x > 0 {    return y}// 在允许的场景下鼓励下一种风格// 注意，此例中如果outer scope里有err定义的话，这里也不会报错，而是shadow。 尤其是 a, err := 的场景时，注意和外面的 a, err := 会有差别。if err := file.Chmod(0664); err != nil {    log.Print(err)    return err}
```

go里估计尽早返回以减少层级，如：

```
// 省了elsef, err := os.Open(name)if err != nil {    return err}d, err := f.Stat()if err != nil {    f.Close()    return err}codeUsing(f, d)
```

## Redeclaration and reassignment

go在下面的场景下允许在已经声明一个变量的情况下再声明

- inner scope里，会shadow outer scope里的同名变量

```
a := 1if a := sth(); a > 1 {    fmt.Println("ok")}
```

- 同时多个变量赋值，其他变量中至少有一个是新声明的

```
a, err := sth()b, err := other()// 尤其是返回值场景func Sth() (err error) {    a, err := sth()  // 不会报错，虽然err已经声明了}// 但注意，下面的inner scope里的err不会赋值到outer scope的err变量a, err := sth()if a != nil {    return}if b, err := other(); err != nil {    fmt.Println("other met error")}// 此时 err仍然为nilfunc Sth() (err error) {    if a, err := hehe(); err != nil {        log.Error("hehe met error (%v)", err)    }    return  // 此时err还是nil}
```

## For

兼具c里for和while的功能，但没有do-while。 支持更丰富的形式。

- c for风格

```
// Like a C forfor init; condition; post { }
```

- c while风格

```
// Like a C whilefor condition { }
```

- c for(;;)

```
// Like a C for(;;)for { }
```

### 迭代数组/slice

**go风格**

- 索引和元素都要

```
for _, value := range s {}
```

- 只要索引

```
for i := range s {}
```

- 只要值

```
for _, v := range s {}
```

**类C风格**

```
for i := 0; i<len(s); i++ {    v := s[i]    // do sth}
```

### 迭代字符串

每次迭代返回 rune 和 该rune的第一个byte在该字符串的bytes中的index。 
go会做更多的事： 每次迭代都会尝试解析下一个utf8字符，解析成功则返回该字符作为v； 如果解析失败则返回下一个byte作为v。 i部分则是该字符或者byte的第一个byte的index。 
*无论是否为合法的utf-8字符，都是合法的code point*

**code point**

```
go里用一个新的类型rune来表示，实际是alias of int32
之所以是4 byte，是因为4-6 byte utf-8在主流的...中认为是不合法的。
也即golang要支持utf-md4的话，需要以bytes方式。

```

如：

```
for pos, char := range "日本\x80語" { // \x80 is an illegal UTF-8 encoding    fmt.Printf("character %#U starts at byte position %d\n", char, pos)}
```

------

go里没有逗号操作符，而且 ++和--是语句而不是表达式，所以如果要在for循环（的initializer里）做多变量赋值的话要用类似：

```
for i, j := 0, len(a)-1; i < j; i, j = i+1, j-1 {}

```

这种格式，而不是：

```
// invalid
for i := 0, j := len(a)-1; i < j; i++, j-- {}

```

## Switch

比c中的更通用，表达式不需要是常量，甚至也不需要是整数。 
从上到下直到有一个匹配。

```
func unhex(c byte) byte {    switch {    case '0' <= c && c <= '9':        return c - '0'    case 'a' <= c && c <= 'f':        return c - 'a' + 10    case 'A' <= c && c <= 'F':        return c - 'A' + 10    }    return 0}
```

注意： go里默认不会fallthrough，也即如果一个匹配上了就会跳出，除非显式的fallthrough。

此外，go的switch还支持多值：

```
func shouldEscape(c byte) bool {    switch c {    case ' ', '?', '&', '=', '#', '+', '%':        return true    }    return false}
```

还支持（返回true的）表达式形式：

```
Loop:    for n := 0; n < len(src); n += size {        switch {        case src[n] < sizeOne:            if validateOnly {                break            }            size = 1            update(src[n])        case src[n] < sizeTwo:            if n+1 >= len(src) {                err = errShortInput                break Loop            }            if validateOnly {                break            }            size = 2            update(src[n] + src[n+1]<<shift)        }    }
```

```
// Compare returns an integer comparing the two byte slices,// lexicographically.// The result will be 0 if a == b, -1 if a < b, and +1 if a > bfunc Compare(a, b []byte) int {    for i := 0; i < len(a) && i < len(b); i++ {        switch {        case a[i] > b[i]:            return 1        case a[i] < b[i]:            return -1        }    }    switch {    case len(a) > len(b):        return 1    case len(a) < len(b):        return -1    }    return 0}
```

## Type switch

go的switch支持各种类型，包括 “类型” 类型。

```
var t interface{}t = functionOfSomeType()switch t := t.(type) {default:    fmt.Printf("unexpected type %T\n", t)     // %T prints whatever type t hascase bool:    fmt.Printf("boolean %t\n", t)             // t has type boolcase int:    fmt.Printf("integer %d\n", t)             // t has type intcase *bool:    fmt.Printf("pointer to boolean %t\n", *t) // t has type *boolcase *int:    fmt.Printf("pointer to integer %d\n", *t) // t has type *int}
```

常用于接口的多个实际类型做处理…

# Functions

## Multiple return values

c里常通过： *传一个 result （int/bool）的指针作为函数参数，而在函数体里根据处理情况对其进行赋值* 的方式来指示执行情况。 
go里则不用，因为支持多返回值，可以直接把执行情况作为返回值之一进行返回。

```
func (file *File) Write(b []byte) (n int, err error)

```

## Named result parameters

命名参数也是go的特点，使得函数签名更加容易理解…。 
对于命名参数，会在函数开始时 声明并初始化为(其类型的)零值，函数执行中可以对其进行赋值。 如果return时没有带参数的话，会使用该值作为返回值。

```
func ReadFull(r Reader, buf []byte) (n int, err error) {    for len(buf) > 0 && err == nil {        var nr int        nr, err = r.Read(buf)        n += nr        buf = buf[nr:]    }    return}
```

## Defer

go支持defer一个语句/函数调用，将其执行安排在函数调return后而真正返回前。

```
// Contents returns the file's contents as a string.func Contents(filename string) (string, error) {    f, err := os.Open(filename)    if err != nil {        return "", err    }    defer f.Close()  // f.Close will run when we're finished.    var result []byte    buf := make([]byte, 100)    for {        n, err := f.Read(buf[0:])        result = append(result, buf[0:n]...) // append is discussed later.        if err != nil {            if err == io.EOF {                break            }            return "", err  // f will be closed if we return here.        }    }    return string(result), nil // f will be closed if we return here.}
```

defer的顺序是LIFO的，也即栈式

defer的优点在于：

- 符合逻辑使用，使用的时候“安排”释放/归还，使程序结构清晰简单

defer的不足：

- 不能“分段式”，也即不能在资源使用完后立即释放。 当使用某资源后将会有较长时间才从函数return的话，依赖defer释放的资源会被hold住较长时间

defer常用来“预置”对可能的panic的处理（比如从中recover等）

# Data

## Allocation with new

**背景 - 引用** 
【个人理解】 引用就像 **单“人”持有的指针**，可以通过它来访问值（的内容），但一旦发生赋值操作（函数里返回、传参也算）则把值copy一份然后把新指针给…作为新的引用（继续保持“单人持有”的特性）

go支持两种分配内存的方式： new 和 make

### new

内置方法，分配内存，并不初始化内存，只置零（要明白这个意思），也即 new 分配了类型T的“实例”所需内存空间，置零后返回其（这块空间/这个实例的）地址，也即 *T。 
因为new得到的指针指向的值是“零值”状态，所以依赖零值（也即零值就可以）的场景下是可以用的，如：

```
type SyncedBuffer struct {    lock    sync.Mutex    buffer  bytes.Buffer}p := new(SyncedBuffer)  // type *SyncedBuffervar v SyncedBuffer  // type Syncedbuffer// p和v都是可用的，因为SyncedBuffer这个类型是零值可用的
```

注意，**也有些类型不是零值可用的**，如slice、map等，这些类型需要额外的零值以外的初始化操作：

```
var s1 []int32  // type []int32sp1 := new([]int32)  // type *[]int32// s1[0] = 1  // invalid// (*sp1)[0] = 1  // invalid
```

## Constructors and composite literals

### 构造器

基于上一节可以知道，有些类型要使用需要做额外的初始化，可以使用构造器，go里一般类型Xxx的构造器命名为 NewXxx。 
下面的例子有些类似，但有些差别：

```
func NewFile(fd int, name string) *File {    if fd < 0 {        return nil    }    f := new(File)    f.fd = fd    f.name = name    f.dirinfo = nil    f.nepipe = 0    return f}// There's a lot of boiler plate in there. We can// simplify it using a composite literal, which is an// expression that creates a new instance each time it// is evaluated.func NewFile(fd int, name string) *File {    if fd < 0 {        return nil    }    f := File{fd, name, nil, 0}    return &f}
```

go里返回局部变量的指针是安全的，函数返回后该指针指向的内存区域会被保留。

### 组合书面量

组合书面量的方式，需要所有字段都有并且按序。 不过，可以用具名（labeling）的方式，这样的话可以只有其中部分字段（更不用要求顺序了），其他字段则为零值。

```
return &File{fd, name, nil, 0}// vsreturn &File{fd: fd, name: name}
```

作为特殊的情况，如果一个字段值都不给出的话，则所有字段都为零值，和new操作效果相同

```
return &File{}// vsreturn new(File)
```

## Allocation with make

只用于 **slice、map、channel** 的创建和初始化。 
它比new做的更多一些，创建零值的slice/map/channel之后，会继续初始化操作，所以*需要提供初始化需要的参数（如果需要的话，比如map可以没有，slice则必须有…）*

```
func TestZeroValue(){    var s1 []int32    var sp1 = new([]int32)    var s2 = make([]int32, 0, 10)    fmt.Println(s1 == nil, *sp1 == nil, s2 == nil)}// output: true true false// make等于是把array指向的数组（指针）做了赋值，而不是0值指针了// 不建议的方式，不必要的增加复杂度var p *[]int = new([]int)*p = make([]int, 100, 100)
```

## Arrays

数组用于按计划/预期的分配连续内存（如 分配特定长度的数组作为缓存，避免每次都分配），但主要还是作为slice的组成部分。

go的数组和c中的主要区别如下：

1. go中的数组是值，也即发生赋值操作时会完整copy一份 = =
2. 基于1，如果作为实参传递的话，函数收到的是一份拷贝
3. 数组的长度是它的类型（属性）的一部分，也即[10]int和[20]int不是一个类型（自然不能互相赋值）

值类型的特点很有用但代价也高（对于大数组），所以在需要的时候为了保证效率，可以用数组的指针。

```
func Sum(a *[3]float64) (sum float64) {    for _, v := range *a {        sum += v    }    return}array := [...]float64{7.0, 8.5, 9.1}x := Sum(&array)  // Note the explicit address-of operator
```

数组书面量时（因为长度是已知的），可以把长度用…代替，如：

```
array := [...]int{1,2,3}

```

## Slices

wrap了数组，使得数组更通用，除了少数场景（比如矩阵等）go里的数组相关操作大多是通过slice来完成的。 
slice持有对底层数组的引用，做slice赋值操作时实际指向同一个底层数组。 此时如果做按索引赋值操作，影响到的是这两个slice。

*当一个slice\**对cap的需求超过当前cap**（比如在len == cap时做append操作）时，会创建新的数组，把老数组的值copy过去然后将新的数组作为底层数组 --- 意味着从这一刻开始，该slice和之前共用底层的slices没有关系了*

```
func Append(slice, data []byte) []byte {    l := len(slice)    if l + len(data) > cap(slice) {  // reallocate        // Allocate double what's needed, for future growth.        newSlice := make([]byte, (l+len(data))*2)        // The copy function is predeclared and works for any slice type.        copy(newSlice, slice)        slice = newSlice    }    slice = slice[0:l+len(data)]    for i, c := range data {        slice[l+i] = c    }    return slice}
```

## Two-dimensional slices

go里的array和slice都是一维的，也即二维数组[3][3]int实际类似：

```
111222333

```

而二维slice则类似：

```
struct{    cap(3)    len(2)    array --->                cap1                len1                array1 ---> ...                cao2                len2                array2 ---> ...                0                0                0}
```

从“二维”array的内存布局可以看出，[3][2]int的元素只能是 [2]int。 
同样，[][]int的元素可以是任意（不同）长度的slice，也即[]int。

## Maps

key:value结构。 
key可以是 == 操作符支持的任何类型，如 整数、浮点数、复数、字符串、指针、接口（只要其动态类型支持 == ）、结构体、数组。 
*== 不支持slice，所以slice也不能用于map的key*

和slice一样，map也持有一个底层数据结构的引用，所以作为参数传递时，得到的新值的实际数据仍然是同一份。

map支持用组合书面量的方式来初始化。

```
var timeZone = map[string]int{    "UTC":  0*60*60,    "EST": -5*60*60,    "CST": -6*60*60,    "MST": -7*60*60,    "PST": -8*60*60,}
```

### 读操作

- v := m[key] 
  如果key不存在，会返回value类型的零值，而不是报错
- v, ok := m[key] 
  ok为bool型，如果key存在则ok为true，key不存在则ok的false
- _, ok := m[key] 
  第二种方式的延伸，可用于判断是否key存在

### 删除操作

- delete(m, key)

### 写操作

- m[key] = value

## Printing

go使用c的风格来做格式化打印，不过功能更丰富也更通用。 
fmt包中的方法： Sprintf、Printf、FPrintf… 支持提供格式化模板；  
Print、Println则支持多个打印内容并且按特定格式输出（Println会在参数之间用空格，并且在最后加换行； 
Print则只在相邻两个参数都不是字符串时才加空格）。

```
fmt.Printf("Hello %d\n", 23)fmt.Fprint(os.Stdout, "Hello ", 23, "\n")fmt.Println("Hello", 23)fmt.Println(fmt.Sprint("Hello ", 23))
```

C中类似的格式化打印

```
var x uint64 = 1<<64 - 1fmt.Printf("%d %x; %d %x\n", x, x, int64(x), int64(x))// prints18446744073709551615 ffffffffffffffff; -1 -1
```

go特有的格式化

- %v 打印任何变量的值，对任意类型都可以
- %T 打印任何变量的类型

```
fmt.Printf("%v\n", timeZone)  // or just fmt.Println(timeZone)// which gives outputmap[CST:-21600 PST:-28800 EST:-18000 UTC:0 MST:-25200]type T struct {    a int    b float64    c string}t := &T{ 7, -2.35, "abc\tdef" }fmt.Printf("%v\n", t)fmt.Printf("%+v\n", t)fmt.Printf("%#v\n", t)fmt.Printf("%#v\n", timeZone)// prints&{7 -2.35 abc   def}&{a:7 b:-2.35 c:abc     def}&main.T{a:7, b:-2.35, c:"abc\tdef"}map[string] int{"CST":-21600, "PST":-28800, "EST":-18000, "UTC":0, "MST":-25200}fmt.Printf("%T\n", timeZone)// printsmap[string] int
```

## Append

内置函数，签名如下：

```
func append(slice []T, elements ...T) []T

```

其中，T是类型的占位符，可以表示任意类型（但开发者自己不能这么写，因为append是编译器支持的）

append做的事： 把任意数量的值加到指定的slice末尾然后返回（新的）slice 
**注意，这里是把得到的slice返回，而不是原地操作给出的slice。 当len < 
cap时，append会把新的元素加到底层数组的下一个index； 但当 len == cap时，则会新开辟底层数组…。 
所以不能依赖原有slice，同时也不代表原有slice就绝对不发生变化**

# Initialization

虽然和C/C++的没有特别大的差别，但go的初始化会更强大。 可以在初始化时build复杂的结构…

## 初始化顺序

1. constans - 编译期间
2. 当前包import的包完成（2-4）初始化
3. 当前包的variable initializer完成初始化
4. 当前包内（各个代码文件里）的init方法

## Constants

go里的常量只是常量，在运行时创建，即便定义在函数的局部变量里。 
*只能是： \**数字、字符（rune）、字符串、布尔**。* 
因为编译期限制，定义常量的表达式只能是常数表达式，由编译器来evaluate。 如：

```
1 << 3  // 合法的常量表达式

```

而：

```
math.Sin(math.Po/4)  // 不是常量表达式，因为对math.Sin的函数调用需要在运行时

```

### go里的枚举

常见的方式是通过iota枚举器来创建。 iota可以是表达式的一部分，而该表达式可以隐式重复。 如下：

```
type ByteSize float64const (    _           = iota // ignore first value by assigning to blank identifier    KB ByteSize = 1 << (10 * iota)    MB    GB    TB    PB    EB    ZB    YB)func (b ByteSize) String() string {    switch {    case b >= YB:        return fmt.Sprintf("%.2fYB", b/YB)    case b >= ZB:        return fmt.Sprintf("%.2fZB", b/ZB)    case b >= EB:        return fmt.Sprintf("%.2fEB", b/EB)    case b >= PB:        return fmt.Sprintf("%.2fPB", b/PB)    case b >= TB:        return fmt.Sprintf("%.2fTB", b/TB)    case b >= GB:        return fmt.Sprintf("%.2fGB", b/GB)    case b >= MB:        return fmt.Sprintf("%.2fMB", b/MB)    case b >= KB:        return fmt.Sprintf("%.2fKB", b/KB)    }    return fmt.Sprintf("%.2fB", b)}
```

## Variables

可以和常量一样初始化，不过initializer可以是通用的运行时执行的表达式。

```
var (    home   = os.Getenv("HOME")    user   = os.Getenv("USER")    gopath = os.Getenv("GOPATH"))
```

## The init function

每个源文件都可以定义自己的init函数（每个文件都可以有多个）。 init会在包内所有的变量声明完成（并完成初始化）以后，同时也需要在所有import的包都完成初始化以后。 
除了可以用来完成那些无法用声明表示的初始化以外，init函数也经常用于校验程序状态的合理性。

```
func init() {    if user == "" {        log.Fatal("$USER not set")    }    if home == "" {        home = "/home/" + user    }    if gopath == "" {        gopath = home + "/go"    }    // gopath may be overridden by --gopath flag on command line.    flag.StringVar(&gopath, "gopath", gopath, "override default GOPATH")}
```

# Methods

## Pointers vs. Values

前面提到了，方法可以定义在任何具名类型上（除了指针和接口。 **？？指针不是可以么？**）。 receiver不必是结构体。

```
type ByteSlice []bytefunc (slice ByteSlice) Append(data []byte) []byte {    // Body exactly the same as above}
```

如果不喜欢 s := s.Append(…) 的风格的话，则可以：

```
func (p *ByteSlice) Append(data []byte) {    slice := *p    // Body as above, without the return.    *p = slice}
```

也可以：

```
func (p *ByteSlice) Write(data []byte) (n int, err error) {    slice := *p    // Again as above.    *p = slice    return len(data), nil}// 使用var b ByteSlicefmt.Fprintf(&b, "This hour has %d days\n", 7)
```

这样实现Writer接口…

# Interfaces and other types

## Interfaces

go里的接口提供了一种方式来定义对象的行为： … （省略） duck typing。 
一个类型可以实现多个接口… 比如 集合类型可能实现 sort.Interface接口， 包括 Len(), Less(i, j int) bool 和 Swap(i, j int)

```
type Sequence []int// Methods required by sort.Interface.func (s Sequence) Len() int {    return len(s)}func (s Sequence) Less(i, j int) bool {    return s[i] < s[j]}func (s Sequence) Swap(i, j int) {    s[i], s[j] = s[j], s[i]}// Method for printing - sorts the elements before printing.func (s Sequence) String() string {    sort.Sort(s)    str := "["    for i, elem := range s {        if i > 0 {            str += " "        }        str += fmt.Sprint(elem)    }    return str + "]"}
```

## Conversions

```
// 另一种Sequence的String实现func (s Sequence) String() string {    sort.Sort(s)    return fmt.Sprint([]int(s))  // ...}
```

因为Sequence和[]int是同一种类型（实质上），所以可以转换，这种转换并没有创建新的值，而是临时性的把已有值当做新的类型使用。 
而其他一些合法的转换则有可能创建了新的值，比如 integer ---> floating point。

*转换expression的类型来访问不同的方法集* 是 go中常见的做法。 比如：

```
type Sequence []int// Method for printing - sorts the elements before printingfunc (s Sequence) String() string {    sort.IntSlice(s).Sort()    return fmt.Sprint([]int(s))}
```

## Interface conversions and type assertions

type switch是conversion的一种

```
type Stringer interface {    String() string}var value interface{} // Value provided by caller.switch str := value.(type) {case string:  // 具体类型    return strcase Stringer:  // 接口    return str.String()}
```

类型断言

```
value.(typeName)

```

如

```
tr := value.(string)
// 如果并不能...的话，会panic出runtime error

```

可以使用“尝试”的形式：

```
str, ok := value.(string)if ok {    fmt.Printf("string value is: %q\n", str)} else {    fmt.Printf("value is not a string\n")}
```

## Generality

如果一个类型存在的目的只是为了实现某接口而没有导出其他的方法的话，那么不需要导出该类型，只导出该接口可以使得更清晰： 只关心行为，而不是实现。 
这种情况下可以通过构造器直接返回接口类型的方式…

```
type Block interface {    BlockSize() int    Encrypt(src, dst []byte)    Decrypt(src, dst []byte)}type Stream interface {    XORKeyStream(dst, src []byte)}// NewCTR returns a Stream that encrypts/decrypts using the given Block in// counter mode. The length of iv must be the same as the Block's block size.func NewCTR(block Block, iv []byte) Stream
```

## Interfaces and methods

```
type Handler interface {    ServeHTTP(ResponseWriter, *Request)}// Simple counter server.type Counter struct {    n int}func (ctr *Counter) ServeHTTP(w http.ResponseWriter, req *http.Request) {    ctr.n++    fmt.Fprintf(w, "counter = %d\n", ctr.n)}// exampleimport "net/http"// ...ctr := new(Counter)http.Handle("/counter", ctr)// 对于简单的Counter，实际可以：// Simpler counter server.type Counter intfunc (ctr *Counter) ServeHTTP(w http.ResponseWriter, req *http.Request) {    *ctr++    fmt.Fprintf(w, "counter = %d\n", *ctr)}
```

# The blank identifier

空白标示符： _ 
类似/dev/null的概念，作为变量占位符，只写属性。

## The blank identifier in multiple assignment

多变量赋值的场景： 如果只关心一部分返回值的话，可以把其他返回值赋给 _

```
if _, err := os.Stat(path); os.IsNotExist(err) {    fmt.Printf("%s does not exist\n", path)}
```

## Unused imports and variables

不用的导入和变量，开发中需要的导入和变量但又没有马上用到，可以赋给 _，需要的时候再改为合适的变量名（配合TODO）。

```
package mainimport (    "fmt"    "io"    "log"    "os")func main() {    fd, err := os.Open("test.go")    if err != nil {        log.Fatal(err)    }    // TODO: use fd.}
```

```
package mainimport (    "fmt"    "io"    "log"    "os")var _ = fmt.Printf // For debugging; delete when done.var _ io.Reader    // For debugging; delete when done.func main() {    fd, err := os.Open("test.go")    if err != nil {        log.Fatal(err)    }    // TODO: use fd.    _ = fd}
```

## Import for side effect

import包不是为了使用，而仅仅是为了触发其初始化。

```
import _ "net/http/pprof"

```

## 接口（实现）检查

（在global级别）把某类型T的值赋给某接口I类型的变量，可以让编译器进行类型检测以（在编译期）确认该类型是否实现该接口。 而此变量本身是无用的，所以可以：

```
var i I = &T{}

```

```
if _, ok := val.(json.Marshaler); ok {    fmt.Printf("value %v of type %T implements json.Marshaler\n", val, val)}
```

```
var _ json.Marshaler = (*RawMessage)(nil)

```

# Embedding

go不支持典型的基于类型的子类，不过提供了一种通过**在结构体或接口内“嵌入”另一个的方式**来*“借”其他结构体/接口（的组成片段）*。

## 接口嵌入

如：

```
type Reader interface {    Read(p []byte) (n int, err error)}type Writer interface {    Write(p []byte) (n int, err error)}
```

那么，可以通过下面的方式来获得一个新的集合两个接口方法定义的新接口：

```
// ReadWriter is the interface that combines the Reader and Writer interfaces.type ReadWriter interface {    Reader    Writer}
```

**只有接口才能嵌入接口**

## 结构体嵌入

```
// bufio包中的Reader、Writer这两个struct type，分别实现了Reader和Writer接口// ReadWriter stores pointers to a Reader and a Writer.// It implements io.ReadWriter.// 没给field name则默认为其类型同名...type ReadWriter struct {    *Reader  // *bufio.Reader    *Writer  // *bufio.Writer}// 组合方式：type ReadWriter struct {    reader *Reader    writer *Writer}// 使用：func (rw *ReadWriter) Read(p []byte) (n int, err error) {    return rw.reader.Read(p)}还可以：func (rw *ReadWriter) Read(p []byte) (n int, err error) {    return rw.Read(p)  // 此时调的是 (r *Reader) Read(p []byte) (n int, err error)，也即receiver还是Reader而不是ReadWriter}
```

实际上，当嵌入类型时，inner type的方法会成为outer type的方法（加入到其方法集中 --- 更可能是当outer type的方法集中没有的时候会去其），但当调用的时候receiver还是inner type而不是outertype。

```
type Job struct {    Command string    *log.Logger}//使用job.Log("starting now...")func (job *Job) Logf(format string, args ...interface{}) {    job.Logger.Logf("%q: %s", job.Command, fmt.Sprintf(format, args...))}
```

### 嵌入struct的初始化（书面量）

```
func NewJob(command string, logger *log.Logger) *Job {    return &Job{command, logger}}// orjob := &Job{command, log.New(os.Stderr, "Job: ", log.Ldate)}
```

### 嵌入类型的字段名

1. 可以指定字段名
2. 如果没指定则默认为类型名，比如rw.Reader
3. 如果在同一层次有其他变量与嵌入struct的默认类型名重复，比如Job还有一个字段叫Reader的话，则是错误的情况，但如果没有被外部使用（也即rw.Reader这样调用）的话，该错误是可以容忍的

# Concurrency

## Share by communicating

并发编程是很大的话题，下面只提及与go相关的。 
很多情况下并发编程的难度都来自需要正确的（并发）访问共享变量。 而go鼓励用另一种方式： 共享的内容/值在channels里传递以达到不会被多个线程共享的目的，在同一时刻只有一个goroutine访问该值。 --- 从设计上避免数据争用。 
**不要通过共享内存的方式来通信，而是通过通信的方式来共享内存**

## Goroutines

之所以叫goroutine，是因为已有的名词 线程、协程、进程等等 都不能准确的表达“goroutine”的含义。 
goroutine的简单的模型： *并发的执行在同一个地址空间的其他goroutine中的函数*。 
goroutine是轻量级的，开销只比占空间分配多一点点。 而且栈初始空间很小，所以很廉价，，根据需要来分配/释放堆空间。

goroutines被复用到多个OS线程，所以当有一个（goroutine）阻塞时（比如等待IO），其他的（goroutine）会继续运行。 goroutine的实现把线程的创建和管理这些复杂的内容隐藏了。

在一个函数/方法/语句咋执行前加go关键字 - 使其在新的goroutine中执行。 该语句执行完时goroutine就（创建了）存在了（但不保证马上开始执行，需要等调度）。

```
go list.Sort()  // run list.Sort concurrently; don't wait for it.

```

```
func Announce(message string, delay time.Duration) {    go func() {        time.Sleep(delay)        fmt.Println(message)    }()  // Note the parentheses - must call the function.}
```

## Channels

channel用make分配，返回“引用”。 可以指定buffer size，默认无buffer（size为0）

```
ci := make(chan int)            // unbuffered channel of integerscj := make(chan int, 0)         // unbuffered channel of integerscs := make(chan *os.File, 100)  // buffered channel of pointers to Files
```

无buffer的channel把 值的交换和两个操作（读、写）的同步关联在一起。 保证下面的顺序：

1. 写返回
2. 读操作取到值

```
c := make(chan int)  // Allocate a channel.// Start the sort in a goroutine; when it completes, signal on the channel.go func() {    list.Sort()    c <- 1  // Send a signal; value does not matter.}()doSomethingForAWhile()<-c   // Wait for sort to finish; discard sent value.
```

对于无buffer channel，sender会阻塞到receiver获取到值； 对于有buffer channel，sender会阻塞（如果buffer满）到（buffer不满）值被copy到buffer

有buffer的channel可以用作信号量，比如限制吞吐等。

```
var sem = make(chan int, MaxOutstanding)func handle(r *Request) {    sem <- 1    // Wait for active queue to drain.    process(r)  // May take a long time.    <-sem       // Done; enable next request to run.}func Serve(queue chan *Request) {    for {        req := <-queue        go handle(req)  // Don't wait for handle to finish.    }    // 这样可以不阻塞request chan，但代价是可能会有大量的blocking goroutine存在}// 另一个版本func Serve(queue chan *Request) {    for req := range queue {        sem <- 1        go func() {            process(req) // Buggy; see explanation below.            <-sem        }()    }}// bug： go里loop initializer里声明的变量会在每一次迭代中重用，导致每个...实际引用到的是同一个req// 下面是正确的版本func Serve(queue chan *Request) {    for req := range queue {        sem <- 1        go func(req *Request) {            process(req)            <-sem        }(req)    }}// 或者func Serve(queue chan *Request) {    for req := range queue {        req := req // Create new instance of req for the goroutine.        sem <- 1        go func() {            process(req)            <-sem        }()    }}// go里允许 req := req 这样的做法，因为 循环外是 outer scope，循环内是inner scope，而循环的initializer类似middle scope，相对于inner scope都是outer scope，而inner scope可以定义同名变量来shadow outer scope中的...
```

## Channels of channels

channel是一级类，所以可以像其他一级类一样创建和传递，这是go里最重要的属性之一。 普遍用于实现安全、并行的解复用demultiplexing。

```
type Request struct {    args        []int    f           func([]int) int    resultChan  chan int}
```

```
func sum(a []int) (s int) {    for _, v := range a {        s += v    }    return}request := &Request{[]int{3, 4, 5}, sum, make(chan int)}// Send requestclientRequests <- request// Wait for response.fmt.Printf("answer: %d\n", <-request.resultChan)
```

```
func handle(queue chan *Request) {    for req := range queue {        req.resultChan <- req.f(req.args)    }}
```

## Parallelization

一个场景： 并行计算。 
下面是 map-reduce 的一个思路（分治法）

```
type Vector []float64// Apply the operation to v[i], v[i+1] ... up to v[n-1].func (v Vector) DoSome(i, n int, u Vector, c chan int) {    for ; i < n; i++ {        v[i] += u.Op(v[i])    }    c <- 1    // signal that this piece is done}const numCPU = 4 // number of CPU coresfunc (v Vector) DoAll(u Vector) {    c := make(chan int, numCPU)  // Buffering optional but sensible.    for i := 0; i < numCPU; i++ {        go v.DoSome(i*len(v)/numCPU, (i+1)*len(v)/numCPU, u, c)    }    // Drain the channel.    for i := 0; i < numCPU; i++ {        <-c    // wait for one task to complete    }    // All done.}
```

上面的常量并行数可以用其他更合适的方式代替：

```
var numCPU = runtime.NumCPU()
// 直接取

```

或者

```
var numCPU = runtime.GOMAXPROCS(0)
// 传0表示查询
// 更合适，因为即使NumCPU个goroutine，在并行计算的场景下还是受到GOMAXPROCS的制约（GOMAXPROCS可以通过环境变量来设置）

```

## A leaky buffer

漏桶free list

```
var freeList = make(chan *Buffer, 100)var serverChan = make(chan *Buffer)func client() {    for {        var b *Buffer        // Grab a buffer if available; allocate if not.        select {        case b = <-freeList:            // Got one; nothing more to do.        default:            // None free, so allocate a new one.            b = new(Buffer)        }        load(b)              // Read next message from the net.        serverChan <- b      // Send to server.    }}func server() {    for {        b := <-serverChan    // Wait for work.        process(b)        // Reuse buffer if there's room.        select {        case freeList <- b:            // Buffer on free list; nothing more to do.        default:            // Free list full, just carry on.        }    }}
```

# Errors

go鼓励用多返回值里的error来表示错误以及针对性的处理，真正的“错误”机制 - panic 只用于极端情况。

## error

error接口定义如下

```
type error interface    Error() string}
```

需要注意的是 接口 == nil 的判定， nil 指针 不等于 nil

```
type MyError struct {    Msg string}func (me *MyError) Error() {    return msg}func doSth() error {    var me *MyError  // nil pointer, but not nil    if caseA() {        me = &MyError{Msg: "Case A"}    }    return}
```

## Panic

极端情况、不可理喻的情况、一般已经不需要调用者针对处理的情况…

```
// A toy implementation of cube root using Newton's method.func CubeRoot(x float64) float64 {    z := x/3   // Arbitrary initial value    for i := 0; i < 1e6; i++ {        prevz := z        z -= (z*z*z-x) / (3*z*z)        if veryClose(z, prevz) {            return z        }    }    // A million iterations has not converged; something is wrong.    panic(fmt.Sprintf("CubeRoot(%g) did not converge", x))}
```

```
var user = os.Getenv("USER")func init() {    if user == "" {        panic("no value for $USER")    }}
```

## Recover

一旦发生（slice访问越界、类型断言失败等），会立即停止运行，并且开始unwind展开该goroutine的栈，一路检查是否有defer函数需要执行。 当unwind操作到底栈顶时，程序失败…。 
不过可以用recover来重新获得goroutine的控制权并且恢复正常的执行。 （根据上文可以知道，recover需要在defer中）

执行recover时，会停止unwind操作并且返回传递给recover的参数。

```
func server(workChan <-chan *Work) {    for work := range workChan {        go safelyDo(work)    }}func safelyDo(work *Work) {    defer func() {        if err := recover(); err != nil {            log.Println("work failed:", err)        }    }()    do(work)}
```

# A web server

```
package mainimport (    "flag"    "html/template"    "log"    "net/http")var addr = flag.String("addr", ":1718", "http service address") // Q=17, R=18var templ = template.Must(template.New("qr").Parse(templateStr))func main() {    flag.Parse()    http.Handle("/", http.HandlerFunc(QR))    err := http.ListenAndServe(*addr, nil)    if err != nil {        log.Fatal("ListenAndServe:", err)    }}func QR(w http.ResponseWriter, req *http.Request) {    templ.Execute(w, req.FormValue("s"))}const templateStr = `<html><head><title>QR Link Generator</title></head><body>{{if .}}<img src="http://chart.apis.google.com/chart?chs=300x300&cht=qr&choe=UTF-8&chl={{.}}" /><br>{{.}}<br><br>{{end}}<form action="/" name=f method="GET"><input maxLength=1024 size=70name=s value="" title="Text to QR Encode"><input type=submitvalue="Show QR" name=qr></form></body></html>
```

