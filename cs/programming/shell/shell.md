# shell 语法



### escape



单引号只要关心（之间的）：

* 单引号（`'`）
* 前后两个单引号之前的斜线（`\`）
* ​



```
Single quotes won't interpolate anything, but double quotes will (for example variables, backticks, certain \ escapes, etc…)

The Bash manual has this to say:

3.1.2.2 Single Quotes

Enclosing characters in single quotes (') preserves the literal value of each character within the quotes. A single quote may not occur between single quotes, even when preceded by a backslash.

3.1.2.3 Double Quotes

Enclosing characters in double quotes (") preserves the literal value of all characters within the quotes, with the exception of $, `, \, and, when history expansion is enabled, !. The characters $ and ` retain their special meaning within double quotes (see Shell Expansions). The backslash retains its special meaning only when followed by one of the following characters: $, `, ", \, or newline. Within double quotes, backslashes that are followed by one of these characters are removed. Backslashes preceding characters without a special meaning are left unmodified. A double quote may be quoted within double quotes by preceding it with a backslash. If enabled, history expansion will be performed unless an ! appearing in double quotes is escaped using a backslash. The backslash preceding the ! is not removed.

The special parameters * and @ have special meaning when in double quotes (see Shell Parameter Expansion).

http://stackoverflow.com/questions/6697753/difference-between-single-and-double-quotes-in-bash
```







## 控制语句

```shell
while [ 1 ]; do sleep 1; ll; done # 无限循环
while [ $i -lt 10 ]; do echo $i;let "i=$i+1"; done # 有限循环
cat raw.txt | while read line; do echo $line; done # readline
until [ 1 = 0 ]; do sleep 1; ll; done # 无限循环，另一中语义
for i in /media/m* ; do ls -l $i; done  # 与目录资源结合
if [ 1 -eq 1 ]; then ll ;fi # test常用判断
if [[ 0 -eq 0 && 1 -eq 0 ]]; then ll ;fi  ## || or  多条件test的写法1  && and  (注意2层中括号)
if [ 0 -eq 0 -a 1 -eq 0 ]; then ll ;fi    ## -o or  多条件test的写法2  -a and
if [ ! -e /tmp/111 -a -z "$a" ]; then ll ;fi   ## 不存在111文件 且a变量长度为0 则执行ll（对应的否定是）
```



## 变量



## 数据类型



### 字符串













# cookbook



## File system



### 获取脚本当前位置



```shell
#!/usr/bin/env bash

BASEDIR=$(dirname "$0")
echo "$BASEDIR"
```



但可能通过symbol来，于是...

```shell
# Absolute path to this script, e.g. /home/user/bin/foo.sh
SCRIPT=$(readlink -f "$0")
# Absolute path this script is in, thus /home/user/bin
SCRIPTPATH=$(dirname "$SCRIPT")
```

但mac下readlink残废，可以：

```shell
brew install coreutils
# 然后用 greadlink
```













