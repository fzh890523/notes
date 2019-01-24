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





### default value

`var1=${var2:-defaultvalue}`

> var2不存在 或者 为空 都会用 后面的默认值





## 数据类型



### 字符串



* `-z "${var}"` 判空

* `-n "${var}"` 非空

  > 部分场景不行，噗，此时需要 `! -z "${var}"`



#### 包含子串判断



```shell
string='My long string'
if [[ $string = *"My long"* ]]; then
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





### 数字



#### 比较



* `if [ int1 -eq int2 ]`    如果int1等于int2
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





#### 数组

Bash array的索引<del>从1开始</del>>。。。



* 声明

  ```sh
  local pids=()
  ```

* 增加元素

  ```sh
  pids+=(${pid_item})  # append
  ```

* 数组长度

  ```sh
  ${#pids[@]}
  ```

* 取元素

  ```sh
  $pids[1]  # starts from 1 ...
  
  local a=1
  $pids[a]  # 囧...
  ```

* 索引赋值

  ```sh
  pids[2]=b
  ```

* 索引删除

  ```sh
  # arr=(a b)
  unset "arr[2]"
  # echo $arr
  ## will print: a
  ```

* 值删除

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







## function



### 参数



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

```
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





### 返回值



可以： `if func arg; then do_sth; fi` 这样。



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



## io



### 文件

#### 判断

- `[ -e "$var" ]` 判存在（文件or目录）
- `[ -f "$var" ]` 判文件存在
- `[ -d ""$var" ]` 判目录存在



#### 





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





















