# [Golang学习 - unicode/utf8 包](http://www.cnblogs.com/golove/p/3271597.html)

**unicode/utf8 包中的函数和方法**

 

```go
// unicode/utf8

------------------------------------------------------------

const (
	RuneError = '\uFFFD'     // “错误”的 Rune 或“Unicode 的替换字符”
	RuneSelf  = 0x80         // 低于 0x80（128）的字符被认为是单字节字符
	MaxRune   = '\U0010FFFF' // Unicode 字符的最大值
	UTFMax    = 4            // 单字符 UTF8 编码的最大字长度
)

------------------------------------------------------------

// EncodeRune 对字符 r 进行编码，然后写入 p（p 的长度必须足够长）// 如果 r 是无效的字符，则写入 RuneError 字符// 它返回写入的字节数func EncodeRune(p []byte, r rune) intfunc main() {
	b := make([]byte, 6)
	n := utf8.EncodeRune(b, '好')
	fmt.Println(b, n)
	// [229 165 189 0 0 0] 3
}

------------------------------------------------------------

// 解码 p 中的第一个字符，返回解码后的字符和 p 中被解码的字节数// 如果 p 为空，则返回（RuneError, 0）// 如果 p 中的编码无效，则返回（RuneError, 1）// 如果 p 是不正确的 UTF8 编码，或者不是 UTF8 编码的最短格式，// 则认为编码是无效的，不进行其它验证func DecodeRune(p []byte) (r rune, size int)

func main() {
	b := []byte{229, 164, 167, 229, 174, 182, 229, 165, 189}
	r, n := utf8.DecodeRune(b)
	fmt.Printf("%c, %v", r, n)
	// 大, 3
}

------------------------------------------------------------

// 功能同上，参数为字符串func DecodeRuneInString(s string) (r rune, size int)

func main() {
	s := string([]byte{229, 164, 167, 229, 174, 182, 229, 165, 189})
	fmt.Println(s)
	// 大家好
	r, n := utf8.DecodeRuneInString(s)
	fmt.Printf("%c, %v", r, n)
	// 大, 3
}

------------------------------------------------------------

// 解码 p 中的最后一个字符，返回解码后的字符，和 p 中被解码的字节数// 如果 p 为空，则返回（RuneError, 0）// 如果 p 中的编码无效，则返回（RuneError, 1）func DecodeLastRune(p []byte) (r rune, size int)

func main() {
	b := []byte{229, 164, 167, 229, 174, 182, 229, 165, 189}
	r, n := utf8.DecodeLastRune(b)
	fmt.Printf("%c, %v", r, n)
	// 好, 3
}

------------------------------------------------------------

// 功能同上，参数为字符串func DecodeLastRuneInString(s string) (r rune, size int)

func main() {
	s := string([]byte{229, 164, 167, 229, 174, 182, 229, 165, 189})
	fmt.Println(s)
	// 大家好
	r, n := utf8.DecodeLastRuneInString(s)
	fmt.Printf("%c, %v", r, n)
	// 好, 3
}

------------------------------------------------------------

// FullRune 检测 p 中第一个字符的 UTF-8 编码是否“有头无尾”。// 如果编码完整，则返回 true，如果“有头无尾”则返回 false，// 如果编码无效（超出范围 或 无头有尾），也返回 true。// 不知道这个函数有什么用func FullRune(p []byte) boolfunc main() {
	b := []byte{229, 165, 189}    // 完整
	fmt.Println(utf8.FullRune(b)) // true
	b = []byte{229, 165}          // 有头无尾
	fmt.Println(utf8.FullRune(b)) // false
	b = []byte{165, 189}          // 无头有尾
	fmt.Println(utf8.FullRune(b)) // true
	b = []byte{255, 255, 255}     //超出范围
	fmt.Println(utf8.FullRune(b)) // true
}

------------------------------------------------------------

// 功能同上，参数为字符串func FullRuneInString(s string) boolfunc main() {
	s := string([]byte{229, 165, 189})    // 完整
	fmt.Println(utf8.FullRuneInString(s)) // true
	s = string([]byte{229, 165})          // 有头无尾
	fmt.Println(utf8.FullRuneInString(s)) // false
	s = string([]byte{165, 189})          // 无头有尾
	fmt.Println(utf8.FullRuneInString(s)) // true
	s = string([]byte{255, 255, 255})     //超出范围
	fmt.Println(utf8.FullRuneInString(s)) // true
}

------------------------------------------------------------

// 返回 p 中的字符个数// 错误的 UTF8 编码和长度不足的 UTF8 编码将被当作单字节的 RuneError 处理func RuneCount(p []byte) intfunc main() {
	b := []byte("Hello 世界！")
	fmt.Println(utf8.RuneCount(b)) // 9
}

------------------------------------------------------------

// 功能同上，参数为字符串func RuneCountInString(s string) (n int)

func main() {
	b := "Hello 世界！"
	fmt.Println(utf8.RuneCountInString(b)) // 9
}

------------------------------------------------------------

// RuneLen 返回需要多少字节来编码字符 r，如果 r 是无效的字符，则返回 -1func RuneLen(r rune) intfunc main() {
	fmt.Println(utf8.RuneLen('A'))          // 1
	fmt.Println(utf8.RuneLen('\u03A6'))     // 2
	fmt.Println(utf8.RuneLen('好'))         // 3
	fmt.Println(utf8.RuneLen('\U0010FFFF')) // 4
	fmt.Println(utf8.RuneLen(0x1FFFFFFF))   // -1
}

------------------------------------------------------------

// 判断 b 的最高位(bit)是不是 10，如果不是，则返回 true// 表示 b 有可能是 UTF8 编码的第一个字节（头字节）func RuneStart(b byte) boolfunc main() {
	s := "你好"
	fmt.Println(utf8.RuneStart(s[0])) // true
	fmt.Println(utf8.RuneStart(s[1])) // false
	fmt.Println(utf8.RuneStart(s[2])) // false
	fmt.Println(utf8.RuneStart(s[3])) // true
	fmt.Println(utf8.RuneStart(s[4])) // false
	fmt.Println(utf8.RuneStart(s[5])) // false
}

------------------------------------------------------------

// Valid 返回 p 中是否完全包含有效的 UTF8 字符func Valid(p []byte) boolfunc main() {
	b := []byte("你好")
	fmt.Println(utf8.Valid(b))     // true
	fmt.Println(utf8.Valid(b[1:])) // false
	fmt.Println(utf8.Valid(b[:5])) // false
	fmt.Println(utf8.Valid(b[3:])) // true
}

------------------------------------------------------------

// 功能同上，参数为字符串func ValidString(s string) boolfunc main() {
	b := "你好"
	fmt.Println(utf8.ValidString(b))     // true
	fmt.Println(utf8.ValidString(b[1:])) // false
	fmt.Println(utf8.ValidString(b[:5])) // false
	fmt.Println(utf8.ValidString(b[3:])) // true
}

------------------------------------------------------------

// ValidRune 返回 r 是否能被正确的转换为 UTF8 编码// 超出 Unicode 范围的码点或 UTF-16 代理区中的码点是不能转换的func ValidRune(r rune) boolfunc main() {
	fmt.Println(utf8.ValidRune('好'))       // true
	fmt.Println(utf8.ValidRune(0))          // true
	fmt.Println(utf8.ValidRune(0xD800))     // false  // 代理区字符
	fmt.Println(utf8.ValidRune(0x10FFFFFF)) // false  // 超出范围
}




```

标签: [Golang包](http://www.cnblogs.com/golove/tag/Golang%E5%8C%85/)

来源： <<http://www.cnblogs.com/golove/p/3271597.html>>