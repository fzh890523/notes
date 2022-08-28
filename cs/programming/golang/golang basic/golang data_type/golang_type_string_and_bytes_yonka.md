

# bytes



* `nil` `[]byte`直接转string可以得到空string
* `nil` `[]byte`可以做`[0:0]`切片



# rune

https://go.dev/ref/spec#Rune_literals



* "char" == `rune`
* `rune` =(alias of)= `int32`
* 用`'*'`格式，如 `'a'`

* 支持一些转义格式

  * `'\n'`

  * `'\x0a'`

  * `\006`

  * `'\u016b'` （只表示格式，没确认这个是否合法）

  * `'\U016b016b'` （只表示格式，没确认这个是否合法）

    4 byte是unicode最大范围，so。。够用

* 原生（也即代码中的字面量）为**utf-8**编码的unicode

  **所以一个`'*'`实际长度要根据字符而定，下面也举了例子，不同的字符（的utf-8）表示，可能是一个字节（如`'a'`），而`'ä'`是两个字节，而一个汉字如`'我'`则可能更长**

  > 但是，默认类型其实是int32（rune）。
  >
  > ```go
  > 	var a rune = 'a'  // 这里提示 类型为rune，且显式类型rune 可以去掉
  > 	var b = 'a'  // 这里提示 类型为int32
  > 	var c byte = 'a'
  > 	var d = '我'  // int32
  > 	var e rune = '\U00101234'
  >   c = byte(b)  // 这里需要显式转换
  > // 这些都合法
  > ```





Note that since the `rune` type is an alias for `int32`, we must use `%c` instead of the usual `%v` in the `Printf` statement, or we will see the integer representation of the Unicode code point (see [A Tour of Go](https://tour.golang.org/basics/11)).



```
 A rune literal represents a rune constant, an integer value identifying a Unicode code point. A rune literal is expressed as one or more characters enclosed in single quotes, as in 'x' or '\n'. Within the quotes, any character may appear except newline and unescaped single quote. A single quoted character represents the Unicode value of the character itself, while multi-character sequences beginning with a backslash encode values in various formats.

The simplest form represents the single character within the quotes; since Go source text is Unicode characters encoded in UTF-8, multiple UTF-8-encoded bytes may represent a single integer value. For instance, the literal 'a' holds a single byte representing a literal a, Unicode U+0061, value 0x61, while 'ä' holds two bytes (0xc3 0xa4) representing a literal a-dieresis, U+00E4, value 0xe4.

Several backslash escapes allow arbitrary values to be encoded as ASCII text. There are four ways to represent the integer value as a numeric constant: \x followed by exactly two hexadecimal digits; \u followed by exactly four hexadecimal digits; \U followed by exactly eight hexadecimal digits, and a plain backslash \ followed by exactly three octal digits. In each case the value of the literal is the value represented by the digits in the corresponding base.

Although these representations all result in an integer, they have different valid ranges. Octal escapes must represent a value between 0 and 255 inclusive. Hexadecimal escapes satisfy this condition by construction. The escapes \u and \U represent Unicode code points so within them some values are illegal, in particular those above 0x10FFFF and surrogate halves. 
```





## literal





```
 A rune literal represents a rune constant, an integer value identifying a Unicode code point. A rune literal is expressed as one or more characters enclosed in single quotes, as in 'x' or '\n'. Within the quotes, any character may appear except newline and unescaped single quote. A single quoted character represents the Unicode value of the character itself, while multi-character sequences beginning with a backslash encode values in various formats.

The simplest form represents the single character within the quotes; since Go source text is Unicode characters encoded in UTF-8, multiple UTF-8-encoded bytes may represent a single integer value. For instance, the literal 'a' holds a single byte representing a literal a, Unicode U+0061, value 0x61, while 'ä' holds two bytes (0xc3 0xa4) representing a literal a-dieresis, U+00E4, value 0xe4.

Several backslash escapes allow arbitrary values to be encoded as ASCII text. There are four ways to represent the integer value as a numeric constant: \x followed by exactly two hexadecimal digits; \u followed by exactly four hexadecimal digits; \U followed by exactly eight hexadecimal digits, and a plain backslash \ followed by exactly three octal digits. In each case the value of the literal is the value represented by the digits in the corresponding base.

Although these representations all result in an integer, they have different valid ranges. Octal escapes must represent a value between 0 and 255 inclusive. Hexadecimal escapes satisfy this condition by construction. The escapes \u and \U represent Unicode code points so within them some values are illegal, in particular those above 0x10FFFF and surrogate halves. 
```







https://golang.org/ref/spec#Rune_literals



```
\a   U+0007 alert or bell
\b   U+0008 backspace
\f   U+000C form feed
\n   U+000A line feed or newline
\r   U+000D carriage return
\t   U+0009 horizontal tab
\v   U+000b vertical tab
\\   U+005c backslash
\'   U+0027 single quote  (valid escape only within rune literals)
\"   U+0022 double quote  (valid escape only within string literals)
```



```
rune_lit         = "'" ( unicode_value | byte_value ) "'" .
unicode_value    = unicode_char | little_u_value | big_u_value | escaped_char .
byte_value       = octal_byte_value | hex_byte_value .
octal_byte_value = `\` octal_digit octal_digit octal_digit .
hex_byte_value   = `\` "x" hex_digit hex_digit .
little_u_value   = `\` "u" hex_digit hex_digit hex_digit hex_digit .
big_u_value      = `\` "U" hex_digit hex_digit hex_digit hex_digit
                           hex_digit hex_digit hex_digit hex_digit .
escaped_char     = `\` ( "a" | "b" | "f" | "n" | "r" | "t" | "v" | `\` | "'" | `"` ) .
```



```
'a'
'ä'
'本'
'\t'
'\000'
'\007'
'\377'
'\x07'
'\xff'
'\u12e4'
'\U00101234'
'\''         // rune literal containing single quote character
'aa'         // illegal: too many characters
'\xa'        // illegal: too few hexadecimal digits
'\0'         // illegal: too few octal digits
'\uDFFF'     // illegal: surrogate half
'\U00110000' // illegal: invalid Unicode code point
```







# string

https://go.dev/blog/strings



* 一份`[]byte`拷贝

* 只读

* 以`utf-8`去"理解"(如iterate)

  但len不行，`len(s)` == `len(underlying_bytes)`



> go里没有类似java的专门"String"(unicode codepoint集合)
>
> string只是`[]byte`的特殊view
>
> 所以go里非utf-8字符集的转换，也不需要经过unicode这个"中间态"对象。 但从API来看，是经过utf-8这个中间编码(`[]byte`类型)的



## convert

Ref: <https://stackoverflow.com/questions/10277933/reading-a-non-utf-8-text-file-in-go>

* 可以 `[]byte(s)`

* 可以 `[]rune(s)`

  等效的，也可以 `[]int32(s)`

  可以知道，这个操作是相对费时的，大约等同于：

  ```go
  var rs []rune
  for _, r := range s {  // 这个过程做utf-8解码
    rs = append(rs, r)
  }
  ```

  同时：

  ```go
  s := "我是中国人"
  rs := []rune(s)
  // rs[1] 为 '是'
  // 显然无法通过 []byte(s) 然后index得到
  ```

  





```go
package main

import (
    "bufio"
    "fmt"
    "log"
    "os"

    "golang.org/x/text/encoding/simplifiedchinese"
    "golang.org/x/text/transform"
)

// Encoding to use. Since this implements the encoding.Encoding
// interface from golang.org/x/text/encoding you can trivially
// change this out for any of the other implemented encoders,
// e.g. `traditionalchinese.Big5`, `charmap.Windows1252`,
// `korean.EUCKR`, etc.
var enc = simplifiedchinese.GBK

func main() {
    const filename = "example_GBK_file"
    exampleWriteGBK(filename)
    exampleReadGBK(filename)
}

func exampleReadGBK(filename string) {
    // Read UTF-8 from a GBK encoded file.
    f, err := os.Open(filename)
    if err != nil {
        log.Fatal(err)
    }
    r := transform.NewReader(f, enc.NewDecoder())

    // Read converted UTF-8 from `r` as needed.
    // As an example we'll read line-by-line showing what was read:
    sc := bufio.NewScanner(r)
    for sc.Scan() {
        fmt.Printf("Read line: %s\n", sc.Bytes())
    }
    if err = sc.Err(); err != nil {
        log.Fatal(err)
    }

    if err = f.Close(); err != nil {
        log.Fatal(err)
    }
}

func exampleWriteGBK(filename string) {
    // Write UTF-8 to a GBK encoded file.
    f, err := os.Create(filename)
    if err != nil {
        log.Fatal(err)
    }
    w := transform.NewWriter(f, enc.NewEncoder())

    // Write UTF-8 to `w` as desired.
    // As an example we'll write some text from the Wikipedia
    // GBK page that includes Chinese.
    _, err = fmt.Fprintln(w,
        `In 1995, China National Information Technology Standardization
Technical Committee set down the Chinese Internal Code Specification
(Chinese: 汉字内码扩展规范（GBK）; pinyin: Hànzì Nèimǎ
Kuòzhǎn Guīfàn (GBK)), Version 1.0, known as GBK 1.0, which is a
slight extension of Codepage 936. The newly added 95 characters were not
found in GB 13000.1-1993, and were provisionally assigned Unicode PUA
code points.`)
    if err != nil {
        log.Fatal(err)
    }

    if err = f.Close(); err != nil {
        log.Fatal(err)
    }
}
```





## literal



https://golang.org/ref/spec#String_literals



```
string_lit             = raw_string_lit | interpreted_string_lit .
raw_string_lit         = "`" { unicode_char | newline } "`" .
interpreted_string_lit = `"` { unicode_value | byte_value } `"` .
```



```
`abc`                // same as "abc"
`\n
\n`                  // same as "\\n\n\\n"
"\n"
"\""                 // same as `"`
"Hello, world!\n"
"日本語"
"\u65e5本\U00008a9e"
"\xff\u00FF"
"\uD800"             // illegal: surrogate half
"\U00110000"         // illegal: invalid Unicode code point
```



```
"日本語"                                 // UTF-8 input text
`日本語`                                 // UTF-8 input text as a raw literal
"\u65e5\u672c\u8a9e"                    // the explicit Unicode code points
"\U000065e5\U0000672c\U00008a9e"        // the explicit Unicode code points
"\xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e"  // the explicit UTF-8 bytes
```



## iterate



```go
package main

import "fmt"

func main() {
        for i, rune := range "Hello, 世界" {
                fmt.Printf("%d: %c\n", i, rune)
        }
}
```



```
0: H
1: e
2: l
3: l
4: o
5: ,
6:  
7: 世
10: 界
```













