

1、介绍了go中的词法定义

> 符号定义： Extended Backus-Naur Form (EBNF)
>
> 字符：newline, unicode_char, unicode_letter, unicode_digit 和 字母数字 letter、decimal_digit、octa_digit、hex_digit

> ```
> newline        = /* the Unicode code point U+000A */ .
> unicode_char   = /* an arbitrary Unicode code point except newline */ .
> unicode_letter = /* a Unicode code point classified as "Letter" */ .
> unicode_digit  = /* a Unicode code point classified as "Decimal Digit" */ .
> ```
>
> ```
> letter        = unicode_letter | "_" .
> decimal_digit = "0" … "9" .
> octal_digit   = "0" … "7" .
> hex_digit     = "0" … "9" | "A" … "F" | "a" … "f" .
> ```
>
> 源码表示： utf-8
>
> 词法元素： 评论、tokens、semicolons（分号）、identifiers标识符、关键字keywords、操作符operators和分隔符delimiters、整型书面量integer literals、浮点型书面量floating-point literals、复数书面量imaginary literals、文字书面量rune literals、字符串书面量

2、常量和变量

> \#常量
>
> 类型： 布尔、文字rune、整型、浮点型、复数、字符串 常量，其中 文字rune、整型、浮点型、复数常量被称为数字常量
>
> 表达： 常量、常量表达式、返回常量结果的conversion（类型转换）、返回常量结果的内建函数。 预声明的标识符iota表示整型常量。 复数常量是一种常量表达式的形式。 数字常量表示任意精度的数都不会溢出
>
> 常量类型： 显式（常量声明 或者 类型转换conversion） 或者 隐式（变量声明 或 赋值 或 表达式里的操作数operand（尤其注意这一点））

> > 如果常量可以被自动定义为可以表示的类型，如 3.0 可以为整型、浮点型 而 2147483648.0（1<<31）可以使float32、float64、unit32但不能是int32或string
> >
> > 无明确类型的常量会有一个默认类型： 在需要有类型值的上下文中会转换为该类型。 如 i := 0，此时i为int --- 无类型常量的默认类型有： bool、rune、int、float64、complex128和string，分别对应boolean、rune、integer、floating-point、complex和string这几大类
> >
> > \# 注意： 这里用integer、boolean、floating-point 来表示大类别的整型、布尔、浮点型
> >
> > go里没有对应IEEE-754里的无穷、非数的值，不过 math包里的 Inf、NaN、IsInf、IsNaN会返回这样的值... （虽然比较奇怪的用float64类型的返回值来表示正无穷或负无穷）

> 精度： 虽然数字常量允许任意精度，但编译器的实现可能是用有限精度的方式表示，所以不能过于依赖，但会规定：

> > 整型integer常量至少256bit
> >
> > 浮点型floating-point常量（包括复数常量里的浮点部分）至少256bit小数和32bit有符号指数
> >
> > 如果不能精确表示整型常量的话报错
> >
> > 如果由于溢出不能表示浮点类型或复数常量的话报错
> >
> > 如果因为精度限制不能表示浮点类型或复数常量的话round to nearest
> >
> > \# 这些限制对于 书面量常量和常量表达式的结果 都生效

> \#变量
>
> 变量： 用于存放值的存储位置，允许的值由变量类型决定
>
> 变量空间的预留/分配：

> > 下面的操作可以为具名变量name variable保留存储空间（reserve storage）：
>
> > > 1 变量声明
>
> > > 2 函数参数和返回值
>
> > > 3 函数声明的签名
>
> > > 4 函数书面量
>
> > 而new或者取复合类型书面量的地址的操作会在运行时为变量分配存储空间

> 变量的静态类型或者说类型： 变量声明时的类型 或者 new调用或复杂类型书面量提供的类型 或者 结构化的变量的元素的类型
>
> 接口类型的变量还会有一个 动态类型： 在运行时把值赋给变量时的该值的具体类型（如果该值预先声明为nil的话，则没有类型，那么这里也没办法有具体类型了）

> > 如
> >
> > type A interface {
> >
> > ​    func AMethod(){}
> >
> > }
> >
> > type B struct {
> >
> > ​    B: int,
> >
> > }
> >
> > func (b B)AMethod(){}
> >
> > 那么：
> >
> > var a A
> >
> > b := B{B: 1}
> >
> > a = b  // 此时a的静态类型是A，而动态类型是B

3、类型

> 具名类型和非具名类型： 前者通过类型名指定； 后者通过类型书面量指定（从已有类型组成新的类型）
>
> ```
> Type      = TypeName | TypeLit | "(" Type ")" .
> TypeName  = identifier | QualifiedIdent .
> TypeLit   = ArrayType | StructType | PointerType | FunctionType | InterfaceType |
> 	    SliceType | MapType | ChannelType .
> ```
>
> boolean、numeric、string的这几种类型的具名实例是预先声明的；
>
> 复杂类型 数组、结构体、指针、函数、接口、切片、map和channel 类型可能是使用类型书面量构建的
>
> \# 每种类型都有一个底层类型： 如果是预声明类型或类型书面量的话，那么就是它自己； 否则的话就是它引用的底层类型，如：

> > typ T1 string  // string
> >
> > type T2 T1  // string
> >
> > // string的底层类型也是string
> >
> > type T3 []T1  // []T1
> >
> > type T4 T3  // []T1
> >
> > // []T1 的底层类型也是[]T1

> 方法集method set

> > 类型会有一个关联的方法集： 接口类型的方法集是它的接口，其他类型T的方法集是接收类型（receiver type）为T的所有方法的集合，而对应的*T类型的方法集是接收类型为T或*T的所有方法的集合，此外还有结构体的匿名字段...
>
> > 类型的方法集决定了它实现的类型（接口），可以用该类型的receiver来调这些方法

> boolean布尔类型

> > 指（represent） 由预声明的常量true和false表示（denote）的布尔真值
> >
> > 预声明的布尔类型是 bool

> numeric数字类型

> > 指（represent） 整数和浮点数的集合
>
> > 预声明的架构无关的数字类型有：
>
> > ```
> > nt8       the set of all unsigned  8-bit integers (0 to 255)
> > uint16      the set of all unsigned 16-bit integers (0 to 65535)
> > uint32      the set of all unsigned 32-bit integers (0 to 4294967295)
> > uint64      the set of all unsigned 64-bit integers (0 to 18446744073709551615)
> >
> > int8        the set of all signed  8-bit integers (-128 to 127)
> > int16       the set of all signed 16-bit integers (-32768 to 32767)
> > int32       the set of all signed 32-bit integers (-2147483648 to 2147483647)
> > int64       the set of all signed 64-bit integers (-9223372036854775808 to 9223372036854775807)
> >
> > float32     the set of all IEEE-754 32-bit floating-point numbers
> > float64     the set of all IEEE-754 64-bit floating-point numbers
> >
> > complex64   the set of all complex numbers with float32 real and imaginary parts
> > complex128  the set of all complex numbers with float64 real and imaginary parts
> >
> > byte        alias for uint8
> > rune        alias for int32
> > ```
> >
> > 平台相关的数字类型有：
>
> > ```
> > uint     either 32 or 64 bits
> > int      same size as uint
> > uintptr  an unsigned integer large enough to store the uninterpreted bits of a pointer value
> > ```
>
> > 为了避免移植问题，所有的数字类型都是不同的，除了 byte是uint8的alias；rune是int32的alias外。 不同类型混用需要做转换

> string字符串类型

> > 指 字符串值的集（set），字符串值是（可能为空的）byte序列。 
>
> > 字符串不可变
>
> > 预声明的字符串类型是 string
>
> > len(s)得到的是byte数。 常量字符串的len是编译期间确定的（常量）。
>
> > 对字符串做切片操作得到的也是byte（子）序列。
>
> > 不能对字符串的元素（byte）做取地址操作，比如 &s[5]

> array数组类型

> > 指 某类型元素的确定长度的序列，元素数即为数组长度，不为负
> >
> > ```
> > ArrayType   = "[" ArrayLength "]" ElementType .
> > ArrayLength = Expression .
> > ElementType = Type .
> > ```
> >
> > 数组长度是数组类型的一部分，一定是一个可以用int表示的非负常数（也即限制在2^31 - 1以内），可以通过len函数获取
> >
> > 可以通过索引访问或切片
> >
> > 数组类型是一维的，但可以组成多维...
> >
> > ```
> > [32]byte
> > [2*N] struct { x, y int32 }
> > [1000]*float64
> > [3][5]int
> > [2][2][2]float64  // same as [2]([2]([2]float64))
> > ```

> slice切片类型
>
> struct结构体类型
>
> pointer指针类型
>
> function函数类型
>
> interface接口类型
>
> map类型
>
> channel通道类型

4、类型和值的属性

5、块

6、声明和作用域

7、表达式expression

8、语句statement

9、内建函数

10、包

11、程序初始化和执行

12、错误处理

13、运行时panic

14、系统考虑system consideration

