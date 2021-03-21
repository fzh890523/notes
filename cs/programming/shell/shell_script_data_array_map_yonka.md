

## intro

* start index

  * bash: 0

  * zsh: 1

    行为可以改：

    ```sh
    # 从1开始
    setopt ksharrays
    # 从0开始了
    unsetopt ksharrays
    # 从1开始
    ```

    也可以人工适配





* array： indexed array
* map： associative array



## 操作

### 声明

```sh
local pids=()
pids=(1 2 3)
declare -a pids=()  # cannot convert associative to indexed array. 二者之间不能互相转换

# map需要声明，array可以不用
declare -A items
items=(["key1"]="value1" ["key2"]="value2")
```

### 增加元素

```sh
pids+=(${pid_item})  # append

items["key3"]="value3"
```

### 数组长度

```sh
${#pids[@]}

# map也一样

# zsh里不要这个@也可以
```

### 取元素

```sh
$pids[1]  # starts from 1 ...

local a=1
$pids[a]  # 囧...

local v=${items[$k]}  # bash支持 ${items["$k"]} 但zsh不支持
```

### 索引赋值

```sh
pids[2]=b
```

### 索引删除

```sh
# arr=(a b)
unset "arr[2]"
# echo $arr
## will print: a
```

### 值删除

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



### 遍历

#### array

```sh
for v in ${a[@]}; do echo element $v; done
# 如果不想数组元素被split的话，要用双引号括起来，如下（zsh默认不开启word split，所以不需要。（加上也ok））
for v in "${a[@]}"; do echo element $v; done

# zsh甚至能（bash不能，只会取到第一个元素
for v in ${a}; do echo element $v; done
# 不过以下这样不行
for v in "${a}"; do echo element $v; done  # 会取到全部内容作为一个item

for ((i=0;i<${#a[@]};i++)); do echo ${a[$i]}; done  # 通用的array（而不是map）遍历方式

# ！！！ 但是，zsh和bash的起始index不同
# 一种兼容的方式如下：
# ais 为 start_index。 对于bash为0; 对于zsh为 1
sh_name=$(echo $SHELL | awk -F/ '{print $NF}')
ZSH_NAME=zsh

arr_idx_start=0
if [[ "${sh_name}" = "${ZSH_NAME}" ]]; then
  arr_idx_start=1
fi
ais=${arr_idx_start}

for ((i=$ais;i<$((${#proxy_names[@]}+$ais));i++)); do
  export ${proxy_names[$i]}=${yk_proxy}
done
```



#### map

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
# 对于数组，这样还是value而不是index

for v in ${(@v)items}; do
  echo $v
done

for k v in ${(@kv)items}; done
  echo "$k -> $v"
done
# 对于数组，这样会把values按pair取出，而不是 idx: value

# 似乎以上那个 @ 有没有没区别？
#  @ inside quotes used to preserve empty elements
```



### 赋值

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





### 转换（为数组)： 使用`()` 来做eval



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



### 判空

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



