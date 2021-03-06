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





## 数据类型



### 字符串



* `-z "${var}"` 判空

* `-n "${var}"` 非空

  > 部分场景不行，噗，此时需要 `! -z "${var}"`



`==` is a bash-ism. The POSIX form is `=`. If portability to non-bash shells is important, use `=`.



#### 包含子串判断



```shell
string='My long string'
if [[ $string = *"My long"* ]]; then  # 不能用 [ ]
  echo "It's there!"
fi
```



Example:

```shell
# go path
home_go_path=${HOME}/.go
if [[ -z "$GOPATH" ]]; then
    export GOPATH=$home_go_path
elif [[ "$GOPATH" != *"$home_go_path":* && "$GOPATH" != *"$home_go_path" ]]; then
    export GOPATH=$GOPATH:$home_go_path
fi
```

或者：

```sh
function add_to_env()
{
  local env_name=$1
  local new_part=$2
  local before=$3
  if [[ -z "${!env_name}" ]]; then
      export ${env_name}=$new_part
  elif [[ "${!env_name}" != *"$new_part":* && "${!env_name}" != *"$new_part" ]]; then
      if [[ -z "$before" ]]; then
          export ${env_name}=${!env_name}:$new_part
      else
          export ${env_name}=$new_part:${!env_name}
      fi
  fi  
}
# zsh: ${!env_name} -> ${(P)env_name}

### below for test
add_to_env b middle
add_to_env b after
add_to_env b before 1
echo $b  # should be before:middle:after
### above for test
```



#### 截取子串substr: ${varname:offset:length}

* offset： 从0开始
* length： 子串长度



```sh
s="abcd"
echo ${s:1}  # bcd
echo ${s:-3}  # a 去掉了后面3个
echo ${s:2:1} # c
echo ${s:2:}  # 空
```



#### 子串替换


以下示例中`s=abc123abc`

* `${varname/substr/replacement}` 替换一次
  
  `${s/abc/ }` -> ` 123abc`
  
* `${varname//substr/replacement}` 全部替换

  `${s//abc/ }` -> ` 123 `
  
* `${varname/#substr/replacement}` 替换开头的(类似`^`)

  `${s/#abc/ }` -> ` 123abc`
  
* `${varname/%substr/replacement}` 替换末尾的(类似`$`)

  `${s/%abc/ }` -> `abc123 `



还有一些外部命令的方式：

* `tr "\0" "\n"`
* `awk `





#### 编码转换

* str -> hex

  `echo -n "Hello" | od -A n -t x1 | sed 's/ *//g'` 

  sed的部分是去掉空格，也可以根据需要替换成`%`等等

  还可以加上 `tr -d '\n'` 来去掉换行



#### 特殊字符处理

ascii可见字符以外的，尤其指不可见、不好输入的字符



* 输入

  * `echo -e`

    ```
                  \0nnn  the eight-bit character whose value is the octal value nnn (zero  to
                         three octal digits)
                  \xHH   the eight-bit character whose value is the hexadecimal value HH (one
                         or two hex digits)
                  \uHHHH the Unicode (ISO/IEC 10646) character whose value is the hexadecimal
                         value HHHH (one to four hex digits)
                  \UHHHHHHHH
                         the Unicode (ISO/IEC 10646) character whose value is the hexadecimal
                         value HHHHHHHH (one to eight hex digits)
    ```

    

    examples：

    ```sh
    echo -e 'toto\010\010ti'   # OUTPUTS: toti
    echo -e '\x41'             # OUTPUTS: A
    echo -e '\u03B1'           # OUTPUTS: α
    echo -e '\U1F413 <= \U1F1EB\U1F1F7' # OUTPUTS 🐓 <= 🇫🇷
    ```

  * `printf`

    ```sh
    printf "\u25ba"  # OUTPUTS: ►
    ```

    

* 输出



##### 一些工具

* 输出完整unicode字符列表

  ```sh
  #!/bin/bash
  for y in $(seq 0 524287); do
    for x in $(seq 0 7); do
      a=$(expr $y \* 8 + $x)
      echo -ne "$a \\u$a "
    done
    echo
  done
  ```

  



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



### 数组array/map

Bash array的索引<del>从1开始</del>>。。。

* array： indexed array
* map： associative array



#### 声明

```sh
local pids=()
pids=(1 2 3)
declare -a pids=()  # cannot convert associative to indexed array. 二者之间不能互相转换

# map需要声明，array可以不用
declare -A items
items=(["key1"]="value1" ["key2"]="value2")
```

#### 增加元素

```sh
pids+=(${pid_item})  # append

items["key3"]="value3"
```

#### 数组长度

```sh
${#pids[@]}

# map也一样

# zsh里不要这个@也可以
```

#### 取元素

```sh
$pids[1]  # starts from 1 ...

local a=1
$pids[a]  # 囧...

local v=${items[$k]}  # bash支持 ${items["$k"]} 但zsh不支持
```

#### 索引赋值

```sh
pids[2]=b
```

#### 索引删除

```sh
# arr=(a b)
unset "arr[2]"
# echo $arr
## will print: a
```

#### 值删除

```sh
${array[@]/$delete}  # new string
# array=(a b); delete=a; echo ${array[@]/$delete}  
## will print: b
# echo arr
## will print: a b

array=( "${array[@]/$delete}" ) #Quotes when working with strings
```

* 索引传参： 数组

  ```sh
  items=(0 1)
  
  function test() {
      local -n arr=$1  # 注意 -n 需要，否则只能取到 [0] 元素
      arr[1]="test"
  }
  
  func test1() {
      echo $1;
  }
  
  test items  # 囧。。。 支持传ref，bash的语法太混乱了
  echo ${items[@]}  # 0 test
  
  test1 items  # items
  # 囧。。。 这会儿又不支持了。。。
  ```

  

* 全部传参

  ```sh
  bash a.sh ${arr[@]}
  # 会先expand然重新evaluate
  
  bash a.sh "${arr[@]}"
  # 只expand，然后就直接把数组传给进程了
  ```

  ```
  arr=(a "b c")
  
  bash a.sh ${arr[@]}
  "a"
  "b"
  "c"
  
  bash a.sh "${arr[@]}"
  "a"
  "b c"
  ```



#### 遍历

**bash**

```sh
for key in ${!items[@]}; do
  echo $key
  echo ${items[$key]}
done

for value in ${items[@]}; do
  echo $value
done
```



**zsh**

```sh
for k in ${(@k)items}; do
  echo $k
done

for v in ${(@k)items}; do
  echo $v
done

for k v in ${(@kv)items}; done
  echo "$k -> $v"
done

# 似乎以上那个 @ 有没有没区别？
#  @ inside quotes used to preserve empty elements
```



#### 赋值

数组无法直接赋值，比如：

```sh
arr=(1 2 3)
arr1=${arr}  # arr1为1，也即取了第一个元素
arr1=( "${arr[@]}" )  # 可能有坑

# 这个更稳妥
declare -a arr2=()
for i in ${!arr[@]}; do
    arr2[$i]="${arr[$i]}"
done

# zsh的方式
set -A arr2_zsh ${(kv)arr}
# 如果直接： arr2_zsh=${arr} 的话，就变成全量kv拼接的字符串了
declare -A arr2_zsh=(${(kv)arr})  # 直接=${(kv)arr}不行，会报错： inconsistent type for assignment

# 还可以用nameref来实现 - bash
declare -n arr3=arr

```





#### 转换（为数组)： 使用`()` 来做eval



如： `(1 2 3)`，不过既然是转换，所以就没有这个字面量了，那么 

```sh
a="1 2 3"
arr=($a)
# 注意: arr=("1 2 3")得到的是一个元素的数组，原因很简单。。。
```

如果得到的数据不符合要求的话怎么办呢？(默认应该是用`IFS`来切分的)

* 字符串替换

  见字符串部分

* (临时)修改`IFS`

  ```sh
  str="A,B,C,D,E,F,G,H"
  oldIFS=$IFS
  IFS=,
  arr=($str)  # 得到了8元素数组
  IFS=$oldIFS
  # 一般IFS默认为 ` \t\n`，也即空格+tab+换行。
  ```



#### 判空

注意： 

由于直接 `$arr` 取的是首元素，没元素则取到空字符，所以 `-z "$arr"` 无法区分 `()` 和 `("")` （显然可以区分`(1)`）

> 以上是对于普通数组，如果是map还不一样。好像直接取都是拿到空。如：
>
> ```sh
> declare -A arr=([a]=1)
> echo $arr  # empty
> echo ${arr[a]}  # 1
> ```

要保持统一的话，还是： 

```sh
if [[ ${#arr[@]} -eq 0 ]]; then
# 对于array、map都有效
# 对nameref有效
```

这样比较好。 对于unset的name，取数组长度也是得到0




## function



### 参数



#### shift

类似出队，按照访问（排列）顺序弹出参数。

* `shift 2` 弹出2个参数，则第三个变为第一个



#### 所有参数传递



Pet peeve: when using `$@`, you should (almost) always put it in double-quotes to avoid misparsing of argument with spaces in them:

```
abc "$@"
```

[share](https://stackoverflow.com/a/3816747)[improve this answer](https://stackoverflow.com/posts/3816747/edit)

Gordon Davisson

63.4k97389

- 8

  Doesn't that only work for 1 argument? If you have multiple arguments, won't it be passed as one long string?– [hepcat72](https://stackoverflow.com/users/2057516/hepcat72) [Jul 20 '15 at 15:56](https://stackoverflow.com/questions/3811345/how-to-pass-all-arguments-passed-to-my-bash-script-to-a-function-of-mine#comment51003205_3816747)

- 70

  @hepcat72 No. `"$*"` works the way you describe, but `"$@"` is handled as a special case by the shell. From [the bash reference manual, special parameters section](http://www.gnu.org/software/bash/manual/bash.html#Special-Parameters): '@ ... Expands to the positional parameters, starting from one. When the expansion occurs within double quotes, each parameter expands to a separate word. That is, "$@" is equivalent to "$1" "$2" …' – [Gordon Davisson](https://stackoverflow.com/users/89817/gordon-davisson) [Jul 20 '15 at 19:10](https://stackoverflow.com/questions/3811345/how-to-pass-all-arguments-passed-to-my-bash-script-to-a-function-of-mine#comment51010008_3816747) 





```sh
function print_args() {
    echo \"$1\"
    echo \"$2\"
    echo \"$3\"
    echo \"$4\"
    echo \"$5\"
    echo \"$6\"
}

echo "# \$*"
bash a.sh $*
echo "###"
echo "# \"\$*\""
bash a.sh "$*"
echo "###"
echo "# \$@"
bash a.sh $@
echo "###"
echo "# \"\$@\""
bash a.sh "$@"
```



`bash c.sh "a" "b c"`

output:

```sh
# $*
"a"
"b"
"c"
""
""
""
###
# "$*"
"a b c"
""
""
""
""
""
###
# $@
"a"
"b"
"c"
""
""
""
###
# "$@"
"a"
"b c"
""
""
""
""
```



```sh
bash c.sh "1" "2.1   2.2" "3"  # 2.1 2.2中间三个空格
$@
1: 1
2: 2.1
3: 2.2
4: 3

"$@"
1: 1
2: 2.1   2.2
3: 3
4:
$*
1: 1
2: 2.1
3: 2.2
4: 3

"$*"
1: 1 2.1   2.2 3
2:
3:
4:
```

打开`-x`看下：

```sh
bash c.sh "1" "2.1   2.2" "3"
+ echo '$@'
$@
+ p 1 2.1 2.2 3  # 和$*一样（？）。 拼接 - 分割 - 重排
+ echo '1: 1'
1: 1
+ echo '2: 2.1'
2: 2.1
+ echo '3: 2.2'
3: 2.2
+ echo '4: 3'
4: 3
+ echo -e '\n"$@"'

"$@"
+ p 1 '2.1   2.2' 3  # 原貌/本色传递，参数个数、参数内容都不变
+ echo '1: 1'
1: 1
+ echo '2: 2.1   2.2'
2: 2.1   2.2
+ echo '3: 3'
3: 3
+ echo '4: '
4:
+ echo '$*'
$*
+ p 1 2.1 2.2 3  # 先拼接； 再根据IFS分割参数； 再用IFS重拍参数 --- 所以参数个数可能变化，并且中间都是标准IFS
+ echo '1: 1'
1: 1
+ echo '2: 2.1'
2: 2.1
+ echo '3: 2.2'
3: 2.2
+ echo '4: 3'
4: 3
+ echo -e '\n"$*"'

"$*"
+ p '1 2.1   2.2 3'  # 只拼接
+ echo '1: 1 2.1   2.2 3'
1: 1 2.1   2.2 3
+ echo '2: '
2:
+ echo '3: '
3:
+ echo '4: '
4:
```









总结：

* `$*`

  expand - eval

* `"$*"`

  * expand(separated with first character of the
    `IFS` special variable, 没有则为空格) - total as a arg
  * equivalent to `"$1c$2c…"`，c为上面所述。。。

* `$@`

  没看出和 `$?` 的差别来。

* `"$@"`

  * equivalent to `"$1" "$2" …`

  * 如果是在word内的话，如 a"$@"b 那么，第一个会和前部分拼接，最后一个回合后面部分拼接




#### 可变参数: `${stat_files[@]}`



* `bash a.sh ${params[@]}`

* `bash a.sh "${params[@]}"`

  两者有差别，类似： `$@` vs `"$@"`



类似：

```sh
# target_pid gap

function watch_cpu() {
	local target_pid=$1
	local gap=${2:-1}

    local pids=()
    local stat_files=()
    # ps -ef | grep nginx | awk '{if($2 == '${target_pid}' || $3 == '${target_pid}'){res=res$2","}}END{print substr(res, 1, length(res)-1)}'

    for pid_item in `ps -ef | awk '{if($2 == '${target_pid}' || $3 == '${target_pid}'){print $2}}'`; do
        pids+=(${pid_item})
        stat_files+=("/proc/"${pid_item}"/stat")
    done

    local v=0
    local tickets=`getconf CLK_TCK`

    while true; do
        # this is equivalent to: second foo bar baz bat
        cat ${stat_files[@]} | awk '{res+=$14; res+=$15; res+=$16; res+=$17;}END{t='${tickets}'; total_secs=res/t; mins=int(total_secs/60); secs=int(total_secs) - mins*60; left=int(total_secs*100)-int(total_secs)*100; printf("%.2f %d:%d:%d\n", total_secs, mins, secs, left)}'
        sleep ${gap}
    done    
}
```





#### 命令执行输出作为参数



```sh
for f in `ls`; do
  rm "$f"
fi

# 这个脚本的问题在于： 如果f有空格的话会有问题，如

touch {1,"2 2","3 3 3",4,"5 5"}.txt

for f in `ls`; do echo "$f"; done
1.txt
2
2.txt
3
3
3.txt
4.txt
5
5.txt

for f in "`ls`"; do echo "$f"; done
1.txt
2 2.txt
3 3 3.txt
4.txt
5 5.txt
# 这个也不对，实际变成了一个包含多个换行的字符串
```







### 返回值



* 可以： `if func arg; then do_sth; fi` 这样。

* 返回值的传递/有效期

  * 前一条命令/函数的返回值可以传递给（跟随其后的）下一个函数（当然了是第一行）

    ```sh
    b() {
      return 1
    }
    
    c() {
      echo $?
    }
    
    a() {
      b
      c  # print了1
    }
    
    a
    ```

    

  * if判断会产生返回值，所以会覆盖原有结果

    ```sh
    if [[ $? -ne 0 ]]; then
      echo $?  # 这个只可能输出0了
    else
      echo $?  # 这里只能输出1
    fi
    # 以上两个输出都是if产生的
    
    # 如果要如预期的print，要用变量保存
    local r=$?
    if [[ $r -ne 0 ]]; then
      echo $r
    fi
    ```

    

* 返回值的产生
  
  * 会用函数内最后一条语句的返回值作为函数返回值，有些时候可能需要显式`return 0`来覆盖



```sh
function b() {
    return 1
}

b
if [ "$?" != 0 ]; then
    echo 123
fi

if ! b; then
    echo 456
fi

# output
123
456
```







### 执行脚本内的函数



```sh
if [[ "$1" = "call_func" ]]; then
    declare -f -F "$2" &> /dev/null
    if [[ $? -eq 0 ]]; then
        shift
        "$@"
    fi
fi
```





### namespace/scope

* 会shadow掉source的内容
* 支持“dynamic dispath”，也即调用函数时是事实查找的，so...



如下：

a.sh

```sh
source b.sh

tt() {
  echo "tt in a"
}

tt
b
```



b.sh

```sh
tt() {
  echo "tt in b"
}

b() {
  echo "b in b"
}
```



* a.sh中有tt函数时：

  ```sh
  tt in a
  b in b: tt in a
  ```

* a.sh中没有tt函数时

  ```sh
  tt in b
  b in b: tt in b
  ```

  

### 判断函数是否存在

```sh
declare -f a  # exit code: 1
a() { do_nothing; } 
declare -f a  # exit code: 0. 这里会打印出函数内容

# 更合适的做法：
declare -f -F a &> /dev/null  # -F 只返回函数名
```



### 空函数体： 不允许

可以 `a() { : }` or `b() { return }`



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





















