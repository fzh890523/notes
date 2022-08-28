

* `-z "${var}"` 判空

* `-n "${var}"` 非空

  > 部分场景不行，噗，此时需要 `! -z "${var}"`



`==` is a bash-ism. The POSIX form is `=`. If portability to non-bash shells is important, use `=`.





### 字符串匹配

* glob

  ```sh
  if [[ "$v" = *2* ]]; then  # zsh/bash都可以
  if [[ "$v" = $pattern ]]; then  # bash可以
  if [[ "$v" = $~pattern ]]; then  # zsh的glob
  if [[ "$v" = "*2*" ]]; then  # 都不行，就成单纯的字符串匹配了
  # [ "$v" = $p ] 也不行，这样的话会自然 glob files然后填充，如变成 [ "$v" = 1.txt 2.txt 3.txt ] 然后就会报错 bash: [: too many arguments
  ```

  

* re

  `=~` （`~`不是有效写法）

  ```sh
  p1=".*2.*"
  [[ 123 =~ $p1 ]] && echo haha  # bash/zsh 都可以
  [[ 123 =~ "$p1" ]] && echo haha  # zsh可以
  ```

  

### 格式判断



#### 是否为数字



* 正则比较`if ! [[ $yournumber =~ '^[0-9]+$' ]]`

* 格式匹配： `[[ $1 == ?(-)+([0-9]) ]]` or `[[ $1 == ?(-)+([0-9]) ]]`

  ```sh
  case $string in
      ''|*[!0-9]*) echo bad ;;
      *) echo good ;;
  esac
  ```

* 算术表达式： `[[ $(($v+1-1)) -eq $v ]]`

* `-eq`： `[ -n "$var" ] && [ "$var" -eq "$var" ]`



### split

* 用IFS

  ```sh
  IFS=';' read -ra ADDR <<< "$IN"
  for i in "${ADDR[@]}"; do
    # process "$i"
  done
  
  while IFS=';' read -ra ADDR; do
    for i in "${ADDR[@]}"; do
      # process "$i"
    done
  done <<< "$IN"
  ```

* shell语法

  ```sh
  IN="bla@some.com;john@home.com"
  arrIN=(${IN//;/ })
  echo ${arrIN[1]}                  # Output: john@home.com
  # This construction replaces all occurrences of ';' (the initial // means global replace) in the string IN with ' ' (a single space), then interprets the space-delimited string as an array (that's what the surrounding parentheses do).
  
  # 类似的
  arrIN=(${IN//\// })  # 可以用 / 来分割
  
  echo ${arrIN[-1]}  # 取最后一个，需要
  ```

  





### 包含子串判断



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



### 截取子串substr: ${varname:offset:length}

* offset： 从0开始
* length： 子串长度



```sh
s="abcd"
echo ${s:1}  # bcd
echo ${s:-3}  # a 去掉了后面3个
echo ${s:2:1} # c
echo ${s:2:}  # 空
```



### 子串替换


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





### 编码转换

* str -> hex

  `echo -n "Hello" | od -A n -t x1 | sed 's/ *//g'` 

  sed的部分是去掉空格，也可以根据需要替换成`%`等等

  还可以加上 `tr -d '\n'` 来去掉换行



### 特殊字符处理

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



## 一些工具

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

  

