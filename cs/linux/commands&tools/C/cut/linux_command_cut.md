
```sh
CUT(1)                                              User Commands                                              CUT(1)

NAME
       cut - remove sections from each line of files

SYNOPSIS
       cut OPTION... [FILE]...

DESCRIPTION
       Print selected parts of lines from each FILE to standard output.

       With no FILE, or when FILE is -, read standard input.

       Mandatory arguments to long options are mandatory for short options too.

       -b, --bytes=LIST
              select only these bytes

       -c, --characters=LIST
              select only these characters

       -d, --delimiter=DELIM
              use DELIM instead of TAB for field delimiter

       -f, --fields=LIST
              select  only these fields;  also print any line that contains no delimiter character, unless the -s op‐
              tion is specified

       -n     (ignored)

       --complement
              complement the set of selected bytes, characters or fields

       -s, --only-delimited
              do not print lines not containing delimiters

       --output-delimiter=STRING
              use STRING as the output delimiter the default is to use the input delimiter

       -z, --zero-terminated
              line delimiter is NUL, not newline

       --help display this help and exit

       --version
              output version information and exit

       Use one, and only one of -b, -c or -f.  Each LIST is made up of one range, or many ranges separated by commas.
       Selected  input  is written in the same order that it is read, and is written exactly once.  Each range is one
       of:

       N      N'th byte, character or field, counted from 1

       N-     from N'th byte, character or field, to end of line

       N-M    from N'th to M'th (included) byte, character or field

       -M     from first to M'th (included) byte, character or field       
```       

## cookbook

### 取特定的字节(范围)

```sh
echo 12345 | cut -c 3
3
echo 12345 | cut -c 6

echo 12345 | cut -c-2
12
echo 12345 | cut -c2-
2345
echo 12345 | cut -c2-4
234
```

但由于**不支持负数作为index**，所以无法直接做到 `cut -c--2`去掉后两个字节 。 不过可以通过结合`rev`命令来实现：

```sh
# -cN- 在下面实现 trim掉后(N-1)个字节的效果
echo 12345 | rev | cut -c3- | rev
123

# 类似的
echo 12345 | rev | cut -c3  # 实现取 last 3th 个元素
```

> 不过要注意，rev是对per-line做reverse操作，也即如果str里有换行的话，行为可能与预期不符
> 不过cut也是per-line所以二者组合使用没问题

