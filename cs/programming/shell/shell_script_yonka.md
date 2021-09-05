# shell 语法



### escape



单引号只要关心（之间的）：

* 单引号（`'`）
* 前后两个单引号之前的斜线（`\`）



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



### (single) quote in quote怎么escape

`$'a\'b'`



#### `' -> '"'"'`



如：

```sh
# 'abc'def'
'abc'"'"'def'
```





#### `' -> \'` - 好像不行

* 以py为例，可以：

  ```python
  cmd = "echo '%s'" % line.replace("'", "'\\''")  # ' -> '\''
  ```

* 以py为例，可以：

  ```python
  cmd = "echo $'%s'" % line.replace("'"， "\\'")  # ' -> \'
  ```

  但还有点问题，见下文：

  

  ```
  From man bash
  Words of the form $'string' are treated specially. The word expands to string, with backslash-escaped characters replaced as specified by the ANSI C standard. Backslash escape sequences, if present, are decoded as follows:
  
            \a     alert (bell)
            \b     backspace
            \e
            \E     an escape character
            \f     form feed
            \n     new line
            \r     carriage return
            \t     horizontal tab
            \v     vertical tab
            \\     backslash
            \'     single quote
            \"     double quote
            \nnn   the eight-bit character whose value is the octal value nnn (one to three digits)
            \xHH   the eight-bit character whose value is the hexadecimal value HH (one or two hex digits)
            \cx    a control-x character
  ```

  



## 控制语句



### loop循环

* while/do
* until/do
* continue/break



```sh
while command1 ; # this is loop1, the outer loop
do
   Statement(s) to be executed if command1 is true

   while command2 ; # this is loop2, the inner loop
   do
      Statement(s) to be executed if command2 is true
   done

   Statement(s) to be executed if command1 is true
done
```



```sh
#!/bin/sh

a=10

until [ $a -lt 10 ]
do
   echo $a
   a=`expr $a + 1`
done
```







### if

```sh
if [ expression 1 ]
then
   Statement(s) to be executed if expression 1 is true
elif [ expression 2 ]
then
   Statement(s) to be executed if expression 2 is true
elif [ expression 3 ]
then
   Statement(s) to be executed if expression 3 is true
else
   Statement(s) to be executed if no expression is true
fi
```









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



### case



```sh
case 值 in
模式1)
    command1
    command2
    command3
    ;;
模式2）
    command1
    command2
    command3
    ;;
模式3|模式4)
    command4
    ;;
*)
    command1
    command2
    command3
    ;;
esac
```





### 多条件



```shell
if [ \( "$g" -eq 1 -a "$c" = "123" \) -o \( "$g" -eq 2 -a "$c" = "456" \) ]
then echo abc
else echo efg
fi
```



或者



```shell
check_permission()
{
    [[ ( -d "${S_HOME}" && -w "${S_HOME}" ) || ( ! -e "${S_HOME}" && -d "${S_PARENT}" && -w "${S_PARENT}" ) ]] \
        || exit_on_err 1 "permission denied, ${HOME} is not writeable."
}
```







## 变量


### 特殊变量

ref： https://www.cnblogs.com/zhangjiansheng/p/8318042.html

1. `$$`
    Shell本身的PID（ProcessID）
2. `$!`
    Shell最后运行的后台Process的PID
3. `$?`
    最后运行的命令的结束代码（返回值）
4. `$-`
    使用Set命令设定的Flag一览
5. `$*`
    所有参数列表。如"$*"用「"」括起来的情况、以"$1 $2 … $n"的形式输出所有参数。
6. `$@`
    所有参数列表。如"$@"用「"」括起来的情况、以"$1" "$2" … "$n" 的形式输出所有参数。
7. `$#`
    添加到Shell的参数个数
8. `$0`
    Shell本身的文件名
9. `$1～$n`
    添加到Shell的各参数值。$1是第1参数、$2是第2参数…。


### default value

`var1=${var2:-defaultvalue}`

> var2不存在 或者 为空 都会用 后面的默认值



### 遍历变量

ref: <https://stackoverflow.com/questions/1305237/how-to-list-variables-declared-in-script-in-bash>



```sh
# 假设变量都是 prefix__ 开头
    for var in "${!prefix__@}"; do
        if [[ -n "${!var}" && -n "${var:8}" ]]; then
            cmd_parts+=("--${var:8}=${!var}")
        fi
    done
```



```sh
for i in _ {a..z} {A..Z}; do
   for var in `eval echo "\\${!$i@}"`; do
      echo $var
      # you can test if $var matches some criteria and put it in the file or ignore
   done 
done
```



### 嵌套变量/变量名（部分）取自变量

叫： `Variable Indirection`



```sh
a1=1
a2=2

i=1
echo ${!a$i}  # -bash: ${!a$i}: bad substitution
# 不支持字面量

v=a1
echo ${!v}  # 1

ar=(1 2 3)
v=ar
echo ${!v}  # 可以
echo ${!v[2]}  # 不行... 
# 比较新的bash支持 nameref，可以用于此
declare -n v1=$v
echo ${v1[2]}  # 可以
```



### 判空/set



```sh
+--------------------+----------------------+-----------------+-----------------+
|   Expression       |       parameter      |     parameter   |    parameter    |
|   in script:       |   Set and Not Null   |   Set But Null  |      Unset      |
+--------------------+----------------------+-----------------+-----------------+
| ${parameter:-word} | substitute parameter | substitute word | substitute word |
| ${parameter-word}  | substitute parameter | substitute null | substitute word |
| ${parameter:=word} | substitute parameter | assign word     | assign word     |
| ${parameter=word}  | substitute parameter | substitute null | assign word     |
| ${parameter:?word} | substitute parameter | error, exit     | error, exit     |
| ${parameter?word}  | substitute parameter | substitute null | error, exit     |
| ${parameter:+word} | substitute word      | substitute null | substitute null |
| ${parameter+word}  | substitute word      | substitute word | substitute null |
+--------------------+----------------------+-----------------+-----------------+
```

Source: [POSIX: Parameter Expansion](http://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html#tag_18_06_02):



```sh
+--------------------+----------------------+-----------------+-----------------+
|   Expression       |  FOO="world"         |     FOO=""      |    unset FOO    |
|   in script:       |  (Set and Not Null)  |  (Set But Null) |     (Unset)     |
+--------------------+----------------------+-----------------+-----------------+
| ${FOO:-hello}      | world                | hello           | hello           |
| ${FOO-hello}       | world                | ""              | hello           |
| ${FOO:=hello}      | world                | FOO=hello       | FOO=hello       |
| ${FOO=hello}       | world                | ""              | FOO=hello       |
| ${FOO:?hello}      | world                | error, exit     | error, exit     |
| ${FOO?hello}       | world                | ""              | error, exit     |
| ${FOO:+hello}      | hello                | ""              | ""              |
| ${FOO+hello}       | hello                | hello           | ""              |
+--------------------+----------------------+-----------------+-----------------+
```





### variable expansion

ref:

* https://unix.stackexchange.com/questions/171346/security-implications-of-forgetting-to-quote-a-variable-in-bash-posix-shells
* https://unix.stackexchange.com/questions/461360/glob-character-within-variable-expands-in-bash-but-not-zsh



对于多数shell来说，变量表达式没用引号括起来（如`"$var"`）的话，会隐式的apply `word split`+`glob` 操作。 不过zsh里这两个特性都是默认关闭了，需要显式打开（`globsubst`, `shwordsplit`）

* word split

  ```sh
  v="f1 f2 f3"
  ls $v  # bash会split to `ls f1 f2 f3`; zsh不会，需要 `ls $=v`
  ```

* glob

  ```sh
  v="f*"
  ls $v  # bash会（根据当前目录下文件情况）glob expand to `ls f1 f2 f3`; zsh不会，需要 `ls $~v`
  ```

  > glob不限于文件，字符串比较时也可以，如： `p="*2*"; [[ 123 = $p ]] # for bash`

> zsh: 两个可以一起开启，如 `ls $=~v`





## 数据类型



### 字符串

见 《shell_script_data_string_yonka.md》



### 数字



#### 比较



* `if [ int1 -eq int2 ]`    如果int1等于int2

  注意，不能用于比较非数字形式的。。，普通字符串需要用 `=`

* `if [ int1 -ne int2 ]`    如果不等于    

* `if [ int1 -ge int2 ]`     如果>=

* `if [ int1 -gt int2 ]`     如果>

* `if [ int1 -le int2 ]`     如果<=

* `if [ int1 -lt int2 ]`     如果<





#### 加减

arithmetic expansion方式

```sh
var=$((var+1))
((var=var+1))
((var+=1))
((var++))
```



let方式

```sh
let "var=var+1"
let "var+=1"
let "var++"
```

#### 随机数

ref: https://www.cnblogs.com/ginvip/p/6363120.html

* `$RANDOM`
  `$(($RANDOM+1000000000))`
* `$(rand 400000 500000) `
* 使用awk的随机函数 `awk 'BEGIN{srand();print rand()*1000000}'`
* openssl rand产生随机数 `openssl rand -base64 8`
  `openssl rand -base64 8|md5sum`
* 通过时间获得随机数（date） `date +%s%N`
  ```sh
  function rand(){   
      min=$1   
      max=$(($2-$min+1))   
      num=$(date +%s%N)   
      echo $(($num%$max+$min))   
  }    
  ```
* 通过系统内唯一数据生成随机数（/dev/random及/dev/urandom）
  `cat /dev/urandom|head -n 10|md5sum|head -c 10`
  `cat /dev/urandom|strings -n 8|head -n 1`
* 读取Linux的uuid码
  `cat /proc/sys/kernel/random/uuid |cksum|cut -f1 -d " "`
  ```sh
  function rand(){   
      min=$1   
      max=$(($2-$min+1))   
      num=$(cat /proc/sys/kernel/random/uuid | cksum | awk -F ' ' '{print $1}')   
      echo $(($num%$max+$min))   
  }   
     
  ```

rnd=$(rand 100 500)  
```  
* 从元素池中随机抽取取
  ```sh
  pool=(a b c d e f g h i j k l m n o p q r s t 1 2 3 4 5 6 7 8 9 10)
  num=${#pool[*]}
  result=${pool[$((RANDOM%num))]}
```



### 数组、map

见 《shell_script_data_array_map_yonka.md》




## function

见 《shell_script_function_yonka.md》



### 代码块/语句块

* `{}` 当前shell内执行

  可以`return`

* `()` 子shell内执行





## io



### 文件

#### 判断

- `[ -e "$var" ]` 判存在（文件or目录）
- `[ -f "$var" ]` 判文件存在
- `[ -d ""$var" ]` 判目录存在



### 重定向



#### 保存、恢复

```sh
exec 6>&1 7>&2  # save 1 2 to 6 7
exec &>> a.log   # redirect。 May be ">> a.log 2>> a.log" for some bash versions.
# do sth
exec >&6 2>&7 6>&- 7>&-  # Restore stdout and stderr and close backups.
```



注意： 如果在`do sth`阶段执行了子命令（尤其是后台执行的），那么6 7会被继承。

而当ssh执行时，stdout、stderr其实是pipe，那么会导致这个pipe被后台任务继承而无法关闭（即使此时shell已经退出），从而hang住ssh。





## cmd/process



### exit code



* 管道符号场景，返回值由后面的覆盖 = =

  ```sh
  cat /tmp1 | awk '{print $2}'  # awk没获取到输入，不报错
  echo $?  # 0
  ```

* `if`也会生成新的exit code覆盖前面的`$?`，囧

  ```sh
  do_sth  # return 0
  if [ $? = 1 ]; then
  	return 0
  else
  	return $?  # return 1
  fi
  # got 1
  
  # 只能
  do_sth
  local res=$?
  if [ "${res}" = 1 ]; then
  	return 0
  else
  	return "$res"
  fi
  ```



### 重定向













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



## process





### 等待进程退出



> 没有内建方式，用 `kill -0` 的返回值来判断 = =



```sh
function wait_pid() {
    local pid=$1
    local wait=$2

    if [ -z "${wait}" ]; then
        wait=0
    fi

    while kill -0 "$pid"; do
        ((wait--))
        if [ "$wait" -lt 0 ]; then  # for POSIX; (( < )) for bash
            return 1  # non-exit
        fi
    done
}

function kill_and_wait() {
    local sig=$1
    local pid=$2
    local wait=$3

    kill -${sig} ${pid} && wait_pid ${pid} ${wait} || return 1
}
```





## 终端颜色



```sh
Black        0;30     Dark Gray     1;30
Red          0;31     Light Red     1;31
Green        0;32     Light Green   1;32
Brown/Orange 0;33     Yellow        1;33
Blue         0;34     Light Blue    1;34
Purple       0;35     Light Purple  1;35
Cyan         0;36     Light Cyan    1;36
Light Gray   0;37     White         1;37
```



```sh
#    .---------- constant part!
#    vvvv vvvv-- the code from above
RED='\033[0;31m'
NC='\033[0m' # No Color
printf "I ${RED}love${NC} Stack Overflow\n"

# Continued from above example
echo -e "I ${RED}love${NC} Stack Overflow"
```





```sh
# Foreground & background colour commands
tput setab [1-7] # Set the background colour using ANSI escape
tput setaf [1-7] # Set the foreground colour using ANSI escape

# Colours are as follows:
Num  Colour    #define         R G B

0    black     COLOR_BLACK     0,0,0
1    red       COLOR_RED       1,0,0
2    green     COLOR_GREEN     0,1,0
3    yellow    COLOR_YELLOW    1,1,0
4    blue      COLOR_BLUE      0,0,1
5    magenta   COLOR_MAGENTA   1,0,1
6    cyan      COLOR_CYAN      0,1,1
7    white     COLOR_WHITE     1,1,1

# Text mode commands
tput bold    # Select bold mode
tput dim     # Select dim (half-bright) mode
tput smul    # Enable underline mode
tput rmul    # Disable underline mode
tput rev     # Turn on reverse video mode
tput smso    # Enter standout (bold) mode
tput rmso    # Exit standout mode

# Cursor movement commands
tput cup Y X # Move cursor to screen postion X,Y (top left is 0,0)
tput cuf N   # Move N characters forward (right)
tput cub N   # Move N characters back (left)
tput cuu N   # Move N lines up
tput ll      # Move to last line, first column (if no cup)
tput sc      # Save the cursor position
tput rc      # Restore the cursor position
tput lines   # Output the number of lines of the terminal
tput cols    # Output the number of columns of the terminal

# Clear and insert commands
tput ech N   # Erase N characters
tput clear   # Clear screen and move the cursor to 0,0
tput el 1    # Clear to beginning of line
tput el      # Clear to end of line
tput ed      # Clear to end of screen
tput ich N   # Insert N characters (moves rest of line forward!)
tput il N    # Insert N lines

# Other commands
tput sgr0    # Reset text format to the terminal's default
tput bel     # Play a bell
```



```sh
tput setaf 1; echo "this is red text"


red=`tput setaf 1`
green=`tput setaf 2`
reset=`tput sgr0`
echo "${red}red text ${green}green text${reset}"

echo "$(tput setaf 1)Red text $(tput setab 7)and white background$(tput sgr 0)"
```







# 快捷键



## 光标移动



* by character

  * `ctrl + f` forward
  * `ctrl + b` backward

* by word

  meta需要设置，iterm的话要在 profile-keys里设置，不是选meta那个，而是 `esc+` 那个

  我设置了 `右option` 作为 `esc+`。

  * `meta + f`
  * `meta + b`

* 删除

  * by word `ctrl + w`

  * by char `ctrl + d`

    区别于`del`，`del`是删除光标之前的，而该是删除光标处的，估计等于 `option + del`

* undo `ctrl+ -`



## 历史命令



* `ctrl + r` 开启搜索

  重复： 继续前一个

* `ctrl + f两次` 直接跳到上一次搜索成功的结果

* `ctrl + j` 确认搜索内容（并且回车 = =）

* `ctrl + g` 取消搜索，恢复之前的命令行内容





















