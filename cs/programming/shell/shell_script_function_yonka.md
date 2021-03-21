

## 参数



### shift

类似出队，按照访问（排列）顺序弹出参数。

* `shift 2` 弹出2个参数，则第三个变为第一个



### 所有参数传递



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




### 可变参数: `${stat_files[@]}`



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





### 命令执行输出作为参数



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







## 返回值



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





## namespace/scope

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

  



## tutorial



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





