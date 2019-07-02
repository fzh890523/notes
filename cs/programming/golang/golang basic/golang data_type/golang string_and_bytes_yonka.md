

# bytes





# rune



## literal

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

* 一份`[]byte`拷贝
* 只读
* 以`utf-8`去"理解"(如iterate)



> go里没有类似java的专门"String"(unicode codepoint集合)
>
> string只是`[]byte`的特殊view
>
> 所以go里非utf-8字符集的转换，也不需要经过unicode这个"中间态"对象。 但从API来看，是经过utf-8这个中间编码(`[]byte`类型)的



## convert

Ref: <https://stackoverflow.com/questions/10277933/reading-a-non-utf-8-text-file-in-go>



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











