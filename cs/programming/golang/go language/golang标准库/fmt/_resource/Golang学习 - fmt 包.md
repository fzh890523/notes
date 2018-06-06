

# [Golang学习 - fmt 包](http://www.cnblogs.com/golove/p/3286303.html)

**fmt 包中的函数和方法**

 

本文参考了《Go语言标准库》章节：
<https://github.com/polaris1119/The-Golang-Standard-Library-by-Example/blob/master/chapter01/01.3.md>

 

```go
// format.go

------------------------------------------------------------

// Fprintf 将参数列表 a 填写到格式字符串 format 的占位符中// 并将填写后的结果写入 w 中，返回写入的字节数func Fprintf(w io.Writer, format string, a ...interface{}) (n int, err error)

// Printf 将参数列表 a 填写到格式字符串 format 的占位符中// 并将填写后的结果写入 os.Stdout 中，返回写入的字节数func Printf(format string, a ...interface{}) (n int, err error)

// Sprintf 将参数列表 a 填写到格式字符串 format 的占位符中// 返回填写后的结果func Sprintf(format string, a ...interface{}) string// Errorf 将参数列表 a 填写到格式字符串 format 的占位符中// 并将填写后的结果转换为 error 类型返回func Errorf(format string, a ...interface{}) error

------------------------------------------------------------

// Fprint 将参数列表 a 中的各个参数转换为字符串格式并写入到 w 中// 各个参数之间以空格分隔，返回写入的字节数func Fprint(w io.Writer, a ...interface{}) (n int, err error)

// Print 将参数列表 a 中的各个参数转换为字符串格式并写入到 os.Stdout 中// 各个参数之间以空格分隔，返回写入的字节数func Print(a ...interface{}) (n int, err error)

// Sprint 将参数列表 a 中的各个参数转换为字符串格式并返回// 各个参数之间以空格分隔func Sprint(a ...interface{}) string

------------------------------------------------------------

// Fprintln 在 Fprint 的基础上，再向 w 中写入一个换行符func Fprintln(w io.Writer, a ...interface{}) (n int, err error)

// Println 在 Print 的基础上，再向 os.Stdout 中写入一个换行符func Println(a ...interface{}) (n int, err error)

// Sprintln 在 Sprint 的基础上，在返回值的最后添加一个换行符func Sprintln(a ...interface{}) string

------------------------------------------------------------

// 示例func main() {
	fmt.Fprintf(os.Stdout, "%08b\n", 32) // 00100000
	fmt.Printf("%08b\n", 32)             // 00100000
	fmt.Print(fmt.Sprintf("%08b\n", 32)) // 00100000
	fmt.Print(fmt.Errorf("%08b\n", 32))  // 00100000

	fmt.Fprint(os.Stdout, "A")
	fmt.Print("B")
	fmt.Print(fmt.Sprint("C"))
	// ABC

	fmt.Print("\n")

	fmt.Fprintln(os.Stdout, "A") // A
	fmt.Println("B")             // B
	fmt.Print(fmt.Sprintln("C")) // C
}

------------------------------------------------------------

// Formatter 用于实现对象的自定义格式输出type Formatter interface {
	// Format 用来处理当对象遇到 c 标记时的输出方式（c 相当于 %s 中的 s）// f 用来获取占位符的宽度、精度、扩展标记等信息，同时实现最终的输出// c 是要处理的标记Format(f State, c rune)
}

// State 用来获取占位符的状态，包括宽度、精度、扩展标记等信息// 同时实现格式化后的字符串的输出type State interface {
	// Write 用于将格式化后的字符串输出到指定的对象// 根据 Print 函数（Fprintf，Printf，Sprintf）的不同，输出到不同的地方Write(b []byte) (ret int, err error)
	// Width 返回占位符的宽度值（wid）和宽度是否被设置（ok）Width() (wid int, ok bool)
	// Precision 返回占位符的精度值（prec）和精度是否被设置（ok）Precision() (prec int, ok bool)
	// Flag 返回扩展标记 c（一个字符，比如 %#s 中的 #）是否已被设置Flag(c int) bool
}

type Stringer interface {
	// String 获取对象的文本形式String() string
}

type GoStringer interface {
	// GoString 获取对象的 Go 语法文本形式（以 %#v 格式输出的文本）GoString() string
}

------------------------------------------------------------

// 示例type Ustr stringfunc (us Ustr) String() string {
	return string(us) + " 自定义格式"
}

func (us Ustr) GoString() string {
	return string(us) + " Go 格式"
}

func (us Ustr) Format(f fmt.State, c rune) {
	switch c {
	case 'm', 'M':
		f.Write([]byte(us + "\n扩展标记：["))
		if f.Flag('-') {
			f.Write([]byte(" -"))
		}
		if f.Flag('+') {
			f.Write([]byte(" +"))
		}
		if f.Flag('#') {
			f.Write([]byte(" #"))
		}
		if f.Flag(' ') {
			f.Write([]byte(" space"))
		}
		if f.Flag('0') {
			f.Write([]byte(" 0"))
		}
		f.Write([]byte(" ]\n"))
		if w, wok := f.Width(); wok {
			f.Write([]byte("宽度值：" + fmt.Sprint(w) + "\n"))
		}
		if p, pok := f.Precision(); pok {
			f.Write([]byte("精度值：" + fmt.Sprint(p)))
		}
	case 'v': // 如果使用 Format 函数，则必须自己处理所有格式，包括 %#vif f.Flag('#') {
			f.Write([]byte(us.GoString()))
		} else {
			f.Write([]byte(us.String()))
		}
	default: // 如果使用 Format 函数，则必须自己处理默认输出
		f.Write([]byte(us.String()))
	}
}

func main() {
	us := Ustr("Hello World!")
	fmt.Printf("% 0-+#8.5m\n", us)
	// Hello World!// 扩展标记：[ - + # space 0 ]// 宽度值：8// 精度值：5
	fmt.Println(us)
	// Hello World! 自定义格式
	fmt.Printf("%#v\n", us)
	// Hello World! Go 格式
}



============================================================



// scan.go

------------------------------------------------------------

// Fscanf 用于扫描 r 中的数据，并根据 format 指定的格式// 将扫描出的数据填写到参数列表 a 中// 当 r 中的数据被全部扫描完毕或扫描长度超出 format 指定的长度时// 则停止扫描（换行符会被当作空格处理）func Fscanf(r io.Reader, format string, a ...interface{}) (n int, err error)

func main() {
	s := strings.NewReader("我的名字叫 Golang ，今年 4 岁")
	var name stringvar age int// 注意：这里必须传递指针 &name, &age// 要获取的数据前后必须有空格
	fmt.Fscanf(s, "我的名字叫 %s ，今年 %d 岁", &name, &age)
	fmt.Printf("%s  %d", name, age)
	// Golang  4
}

// Scanf 用于扫描 os.Stdin 中的数据，并根据 format 指定的格式// 将扫描出的数据填写到参数列表 a 中// 当 r 中的数据被全部扫描完毕或者扫描长度超出 format 指定的长度时// 则停止扫描（换行符会被当作空格处理）func Scanf(format string, a ...interface{}) (n int, err error)

func main() {
	var name stringvar age int// 注意：这里必须传递指针 &name, &age// 要获取的数据前后必须有空格
	fmt.Scanf("%s %d", &name, &age)
	// 在控制台输入：Golang 4
	fmt.Printf("我的名字叫 %s ，今年 %d 岁", name, age)
	// 我的名字叫 Golang ，今年 4 岁
}

// Sscanf 用于扫描 str 中的数据，并根据 format 指定的格式// 将扫描出的数据填写到参数列表 a 中// 当 r 中的数据被全部扫描完毕或者扫描长度超出 format 指定的长度时// 则停止扫描（换行符会被当作空格处理）func Sscanf(str string, format string, a ...interface{}) (n int, err error)

func main() {
	s := "我的名字叫 Golang ，今年 4 岁"var name stringvar age int// 注意：这里必须传递指针 &name, &age// 要获取的数据前后必须有空格
	fmt.Sscanf(s, "我的名字叫 %s ，今年 %d 岁", &name, &age)
	fmt.Printf("%s  %d", name, age)
	// Golang  4
}

------------------------------------------------------------

// Fscan 用于扫描 r 中的数据，并将数据以空格为分割符进行分割// 然后填写到参数列表 a 中// 当 r 中的数据被全部扫描完毕或者参数列表 a 被全部填写完毕// 则停止扫描（换行符会被当作空格处理）func Fscan(r io.Reader, a ...interface{}) (n int, err error)

// Scan 用于扫描 os.Stdin 中的数据，并将数据以空格为分割符进行分割// 然后填写到参数列表 a 中// 当 r 中的数据被全部扫描完毕或者参数列表 a 被全部填写完毕// 则停止扫描（换行符会被当作空格处理）func Scan(a ...interface{}) (n int, err error)

// Sscan 用于扫描 str 中的数据，并将数据以空格为分割符进行分割// 然后填写到参数列表 a 中// 当 r 中的数据被全部扫描完毕或者参数列表 a 被全部填写完毕// 则停止扫描（换行符会被当作空格处理）func Sscan(str string, a ...interface{}) (n int, err error)

------------------------------------------------------------

// Fscanln 用于扫描 r 中的数据，并将数据以空格为分割符进行分割// 然后填写到参数列表 a 中// 当扫描过程中遇到 '\n' 或者参数列表 a 被全部填写完毕// 则停止扫描func Fscanln(r io.Reader, a ...interface{}) (n int, err error)

// Scanln 用于扫描 os.Stdin 中的数据，并将数据以空格为分割符进行分割// 然后填写到参数列表 a 中// 当扫描过程中遇到 '\n' 或者参数列表 a 被全部填写完毕// 则停止扫描func Scanln(a ...interface{}) (n int, err error)

// Sscanln 用于扫描 str 中的数据，并将数据以空格为分割符进行分割// 然后填写到参数列表 a 中// 当扫描过程中遇到 '\n' 或者参数列表 a 被全部填写完毕// 则停止扫描func Sscanln(str string, a ...interface{}) (n int, err error)

------------------------------------------------------------

// ScanState 会返回扫描状态给自定义的 Scanner// Scanner 可能会做字符的实时扫描// 或者通过 ScanState 获取以空格分割的 tokentype ScanState interface {
	// ReadRune 从输入对象中读出一个 Unicode 字符// 如果在 Scanln、Fscanln 或 Sscanln 中调用该方法// 该方法会在遇到 '\n' 或读取超过指定的宽度时返回 EOFReadRune() (r rune, size int, err error)
	// UnreadRune 撤消最后一次的 ReadRune 操作UnreadRune() error// SkipSpace 跳过输入数据中的空格// 在 Scanln、Fscanln、Sscanln 操作中，换行符会被当作 EOF// 在其它 Scan 操作中，换行符会被当作空格SkipSpace()
	// 如果参数 skipSpace 为 true，则 Token 会跳过输入数据中的空格// 然后返回满足函数 f 的连续字符，如果 f 为 nil，则使用 !unicode.IsSpace 来代替 f// 在 Scanln、Fscanln、Sscanln 操作中，换行符会被当作 EOF// 在其它 Scan 操作中，换行符会被当作空格// 返回的 token 是一个切片，返回的数据可能在下一次调用 Token 的时候被修改Token(skipSpace bool, f func(rune) bool) (token []byte, err error)
	// Width 返回宽度值以及宽度值是否被设置Width() (wid int, ok bool)
	// 因为 ReadRune 已经通过接口实现，所以 Read 可能永远不会被 Scan 例程调用// 一个 ScanState 的实现，可能会选择废弃 Read 方法，而使其始终返回一个错误信息Read(buf []byte) (n int, err error)
}

// Scanner 用于实现对象的自定义格式扫描type Scanner interface {
	// Scan 用来处理当对象遇到 c 标记时的扫描方式（c 相当于 %s 中的 s）// state 用来处理扫描状态// c 是要处理的标记Scan(state ScanState, verb rune) error
}





```

标签: [Golang包](http://www.cnblogs.com/golove/tag/Golang%E5%8C%85/)

来源： <<http://www.cnblogs.com/golove/p/3286303.html>>

