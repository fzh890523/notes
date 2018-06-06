# [Golang学习 - unicode/utf16 包](http://www.cnblogs.com/golove/p/3272007.html)

**unicode/utf16 包中的函数和方法**

 

```Go
// utf16.go

------------------------------------------------------------

// IsSurrogate 判断 r 是否为代理区字符// 两个代理区字符可以用来组合成一个 utf16 编码func IsSurrogate(r rune) boolfunc main() {
	fmt.Println(utf16.IsSurrogate(0xD800)) // true
	fmt.Println(utf16.IsSurrogate(0xDC00)) // true
	fmt.Println(utf16.IsSurrogate(0xDFFF)) // true
}

------------------------------------------------------------

// EncodeRune 将字符 r 编码成 UTF-16 代理对// r：要编码的字符// 如果 r < 0x10000 ，则无需编码，其 UTF-16 序列就是其自身// r1：编码后的 UTF-16 代理对的高位码元// r2：编码后的 UTF-16 代理对的低位码元// 如果 r 不是有效的 Unicode 字符，或者是代理区字符，或者无需编码// 则返回 U+FFFD, U+FFFDfunc EncodeRune(r rune) (r1, r2 rune)

func main() {
	r1, r2 := utf16.EncodeRune('\U0010F000')
	fmt.Printf("%x, %x", r1, r2)
	// dbfc, dc00
}

------------------------------------------------------------

// DecodeRune 将 UTF-16 代理对解码成一个 Unicode 字符// r1：是 UTF-16 代理对的高位码元// r2：是 UTF-16 代理对的低位码元// 返回值为解码后的 Unicode 字符// 如果 r1 或 r2 不是有效的 UTF-16 代理区字符，// 则返回 U+FFFDfunc DecodeRune(r1, r2 rune) runefunc main() {
	r := utf16.DecodeRune(0xDBFC, 0xDC00)
	fmt.Printf("%U", r)
	// U+10F000
}

------------------------------------------------------------

// Encode 将 s 编码成 UTF-16 序列并返回func Encode(s []rune) []uint16func main() {
	s := []rune("Hello 世界")
	u := utf16.Encode(s)
	fmt.Printf("%v", u)
	// [72 101 108 108 111 32 19990 30028]
}

------------------------------------------------------------

// Decode 将 UTF-16 序列 s 解码成 Unicode 字符序列并返回func Decode(s []uint16) []runefunc main() {
	u := []uint16{72, 101, 108, 108, 111, 32, 19990, 30028}
	s := utf16.Decode(u)
	fmt.Printf("%c", s)
	// [H e l l o   世 界]
}




```

标签: [Golang包](http://www.cnblogs.com/golove/tag/Golang%E5%8C%85/)

来源： <<http://www.cnblogs.com/golove/p/3272007.html>>