

# tutorial



## special var



### line number: `NR`

index

```sh
# 把每行的某个field提取出来再处理计算
awk '{arr[NR]=$3} END {print (arr[2]-arr[1])/5}'
```





### field number: `NF`

total



## field



### 选择/删除指定的fields

```sh
echo "1 2 3" | awk '{$1 = ""; print $0}'
 2 3  // 可以看到那个空格还在
echo "1  2 3" | awk '{$1 = ""; print $0}' 
 2 3
```

```sh
echo '1 2 3' | awk '{for (i=2; i<=NF; i++) printf("%s ", $i)}'
2 3   // 多了一个空格
```



### 取“剩下”所有的fields

类似的做法：

```sh
echo "1 2 3 4 5" | awk '{$1=$2=""; print $0}'
# 不过分隔符还在
```



循环法：

```sh
awk '{out=""; for(i=2;i<=NF;i++){out=out" "$i}; print out}'

awk '{for(i=2;i<=NF;i++){printf "%s ", $i}; printf "\n"}'
```





### Field separator



多separator：

```sh
awk -F'[/=]' '{print $3 "\t" $5 "\t" $8}' file
```



## line



### line number： NR



### line range： `NR>3 && NR<10`



```sh
awk 'NR>3 && NR<10 {print}' file.txt

awk '/keyword1/,/keyword2/{print}' file.txt
```









## array

用法接近map，int作为key/index。



## map/dict



* 可以直接用不需要定义、声明

* 判断是否存在： `if($5 in m)`

  `not in` 似乎无效，`! $5 in m`也不对，需要`!($5 in m)`

  没有预先声明的dict，似乎怎么判断都为false。

  如：

  ```sh
  echo "1 2 3" | awk 'BEGIN{} {if($1 in m){print "haha"}}'  # 无输出
  echo "1 2 3" | awk 'BEGIN{} {if(! $1 in m){print "haha"}}'  # 无输出
  
  echo "1 2 3" | awk 'BEGIN{m[1]=1} {if($1 in m){print "haha"}}'  # 有输出
  echo "1 2 3" | awk 'BEGIN{m[1]=1} {if(! $1 in m){print "haha"}}'  # 无输出
  
  echo "1 2 3" | awk 'BEGIN{m[1]=1} {if($1 not in m){print "haha"}}'  # 有输出，囧
  ```

  



```sh
grep xxx xxx.log | awk -F "," '{print substr($1, 1,19) "/" $2 "/" $NF}'  | awk -F'/' '{if($3 == 15102){print}}' | awk -F'/' '{
if($5 in m && m[$5] != $6 && (m[$5] == 0 || $6 == 0)){
	printf("%s %s %d %d\n", $1, $5, m[$5], $6)
}
m[$5]=$6
}'
```



```sh
for(key in m) {
    print key, ":" m[key]
}


delete m[key]
```







## func



### define func

```shell
xxx | awk '
function do_sth(arg) {
    return 1;
}
BEGIN {}
{
    // body
}
'
```





### string



#### index (find sub string)

```c
// awk的index从1开始

i = index(s, sub)  // 123.456 .
if(i == 0) {  // not found
    return s;
} else {  // 4
    return substr(s, 1, i - 1);  // 123
}
```







#### substr



```c
substr(s, 1, i)  // index从1开始，表示截取第i个字符之前的部分
```





#### endswith/hasSuffix: 需要"造"



```sh
substr(s, length(s) - 2) == "foo"  # 因为index从1开始算，所以 [len-2, len] 是最后三个字符
# 如果s长度不够也没关系，不会报错，符合预期
```





#### `split(string, array [, fieldsep [, seps ] ])`

* `fieldsep` 正则
* `seps` 匹配出来的每个separator string（的数组）
* `array` split出来的每个元素（的数组）



```sh
BEGIN {
  count = split("a,b,c", a, ",");

  for(i = 1; i <= count; i++) {
    print a[i];
  }
}
```





## number

​	

### float to int





## time

> gawk才有



```sh
awk 'BEGIN {
   print strftime("Time = %m/%d/%Y %H:%M:%S", systime())
}'
# Time = 12/14/2014 22:08:42
# 这个的问题是只有s级别精度

awk 'BEGIN {
   print "Number of seconds since the Epoch = " mktime("2014 12 14 30 20 10")
}'
# Number of seconds since the Epoch = 1418604610
# 这个只是解析而不是获取当前时间
```





[The following time formats are supported by AWK](https://www.tutorialspoint.com/awk/awk_time_functions.htm)

| S.No. |           Date format specification & Description            |
| :---- | :----------------------------------------------------------: |
| 1     |         **%a**The locale’s abbreviated weekday name.         |
| 2     |            **%A**The locale’s full weekday name.             |
| 3     |          **%b**The locale’s abbreviated month name.          |
| 4     |             **%B**The locale’s full month name.              |
| 5     | **%c**The locale’s appropriate date and time representation. (This is %A %B %d %T %Y in the C locale.) |
| 6     | **%C**The century part of the current year. This is the year divided by 100 and truncated to the next lower integer. |
| 7     |   **%d**The day of the month as a decimal number (01–31).    |
| 8     |           **%D**Equivalent to specifying %m/%d/%y.           |
| 9     | **%e**The day of the month, padded with a space if it is only one digit. |
| 10    | **%F**Equivalent to specifying %Y-%m-%d. This is the ISO 8601 date format. |
| 11    | **%g**The year modulo 100 of the ISO 8601 week number, as a decimal number (00–99). For example, January 1, 1993 is in week 53 of 1992. Thus, the year of its ISO 8601 week number is 1992, even though its year is 1993. Similarly, December 31, 1973 is in week 1 of 1974. Thus, the year of its ISO week number is 1974, even though its year is 1973. |
| 12    | **%G**The full year of the ISO week number, as a decimal number. |
| 13    |                   **%h**Equivalent to %b.                    |
| 14    | **%H**The hour (24-hour clock) as a decimal number (00–23).  |
| 15    | **%I**The hour (12-hour clock) as a decimal number (01–12).  |
| 16    |   **%j**The day of the year as a decimal number (001–366).   |
| 17    |         **%m**The month as a decimal number (01–12).         |
| 18    |        **%M**The minute as a decimal number (00–59).         |
| 19    |            **%n**A newline character (ASCII LF).             |
| 20    | **%p**The locale’s equivalent of the AM/PM designations associated with a 12-hour clock. |
| 21    | **%r**The locale’s 12-hour clock time. (This is %I:%M:%S %p in the C locale.) |
| 22    |            **%R**Equivalent to specifying %H:%M.             |
| 23    |        **%S**The second as a decimal number (00–60).         |
| 24    |                    **%t**A TAB character.                    |
| 25    |        **%T**<br />Equivalent to specifying %H:%M:%S.        |
| 26    | **%u**<br />The weekday as a decimal number (1–7). Monday is day one. |
| 27    | **%U**<br />The week number of the year (the first Sunday as the first day of week one) as a decimal number (00–53). |
| 28    | **%V**<br />The week number of the year (the first Monday as the first day of week one) as a decimal number (01–53). |
| 29    | **%w**<br />The weekday as a decimal number (0–6). Sunday is day zero. |
| 30    | **%W**<br />The week number of the year (the first Monday as the first day of week one) as a decimal number (00–53). |
| 31    | **%x**The locale’s appropriate date representation. (This is %A %B %d %Y in the C locale.). |
| 32    | **%X**The locale’s appropriate time representation. (This is %T in the C locale.). |
| 33    |    **%y**The year modulo 100 as a decimal number (00–99).    |
| 34    |     **%Y**The full year as a decimal number (e.g. 2011).     |
| 35    | **%z**The time-zone offset in a +HHMM format (e.g., the format necessary to produce RFC 822/RFC 1036 date headers). |
| 36    | **%Z**The time zone name or abbreviation; no characters if no time zone is determinable. |



### 获得ms精度时间

目前能想到的只有调用外部命令了

```sh
echo "" | awk '{"date +%T.%6N" | getline d; print d}'
# output:
12:32:39.999831
```





## comment



* `# xxx`



## flow control



### Continue current line: next



```sh
awk '{
  if(index($0, "haha") == 0) {
  	next;
  }
  
  do_sth()
}'
```



## 调用外部命令



```sh
echo "" | awk '{"date +%T.%6N" | getline d; print d}'
```

这个命令要用引号括起来，直接 `date | getline d`不行，会deref var报错



## 输出到管道/非终端文件



终端是不带缓冲的字节设备吧，其他的会带缓冲，导致如果数量比较少、慢的话（比如一些中间计算再输出的情况），会迟迟没有输出（可能等大量一起flush或者干脆等处理完（stdin eof）再flush），影响观察。

手动flush方式：

* `system("")`

* `fflush()`

  据说gawk才有