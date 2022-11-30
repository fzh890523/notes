
* `iconv -l` 列出支持的字符集
* 默认输出到stdout，需要`-o <file>`来指定输出（低版本可能不支持，就只能自己重定向stdout了）
* 不支持`in-place`，可以先输出到临时文件再mv回来覆盖原文件
  有个`recode`命令可以in-place
* `iconv -f gbk -t utf-8 "$file" > "$file.new"`
