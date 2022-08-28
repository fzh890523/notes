

https://stedolan.github.io/jq/manual/



ref：

* [JSON格式化输出和解析工具 - jq](https://www.cnblogs.com/kevingrace/p/7565371.html)



# 参数



```sh
        jq is a tool for processing JSON inputs, applying the
        given filter to its JSON text inputs and producing the
        filter's results as JSON on standard output.
        The simplest filter is ., which is the identity filter,
        copying jq's input to its output unmodified (except for
        formatting).
        For more advanced filters see the jq(1) manpage ("man jq")
        and/or https://stedolan.github.io/jq
 
        Some of the options include:
         -c             compact instead of pretty-printed output;
         -n             use `null` as the single input value;
         -e             set the exit status code based on the output;
         -s             read (slurp) all inputs into an array; apply filter to it;
         -r             output raw strings, not JSON texts;
         -R             read raw strings, not JSON texts;
         -C             colorize JSON;
         -M             monochrome (don't colorize JSON);
         -S             sort keys of objects on output;
         --tab  use tabs for indentation;
         --arg a v      set variable $a to value <v>;
         --argjson a v  set variable $a to JSON value <v>;
         --slurpfile a f        set variable $a to an array of JSON texts read from <f>;
```



* `-r`

  > 该选项控制 jq 是输出 raw 格式内容或 JSON 格式内容。所谓的 JSON 格式是指符合 JSON 标准的格式。
  > 例如我们要查询 JSON 字符串{``"name"``:``"tom"``}中 name 的值. 使用-r 选项时返回的是``'tom'``. 不使用-r 选项时，返回的是``'"tom"'``.返回值多了一对双引号。

* `-s`

  > jq 可以同时处理空格分割的多个 JSON 字符串输入。默认情况下，jq 会将 filter 分别对每个 JSON 输入应用，并返回结果。
  > 使用-s 选项，jq会将所有的 JSON 输入放入一个数组中并在这个数组上使用 filter。``"-s"``选项不但影响到 filter 的写法。
  > 如果在 filter 中需要对数据进行选择和映射，其还会影响最终结果。

* `--arg`

  > jq 通过该选项提供了和宿主脚本语言交互的能力。该选项将值(``v``)绑定到一个变量(a)上。在后面的 filter 中可以直接通过变量引用这个值。
  > 例如，filter ``'.$a'``表示查询属性名称等于变量 a 的值的属性。



# 介绍



## **基础表达式**

基础表达式（Basic filters)是 jq 提供的基本过滤器，用来访问 JSON 对象中的属性。基础表达式也是实现更复杂查询功能的基础。基础表达式主要有以下几种：

### **'.' 符号**

单独的一个'.'符号用来表示对作为表达式输入的整个 JSON 对象的引用。

### **JSON 对象操作**

jq 提供两种基本表达式用来访问 JSON 对象的属性：`'.<attributename>'`和`'.<attributename>?'`。正常情况下，这两个表达式的行为相同：都是访问对象属性，如果 JSON 对象不包含指定的属性则返回 null。区别在于，当输入不是 JSON 对象或数组时，第一个表达式会抛出异常。第二个表达式无任何输出。

```sh
jq -n '1 | .+1,.b?,.-1'
2
0
jq -n '1 | .+1,.b,.-1'
2
jq: error (at <unknown>): Cannot index number with string "b"

jq -n '.a.b'
null
```



### **数组操作**

jq 提供三种基础表达式来操作数组：

* 迭代器操作(`'.[]'`). 该表达式的输入可以是数组或者 JSON 对象。输出的是基于数组元素或者 JSON 对象属性值的 iterator。

* 访问特定元素的操作(`'.[index]'`或`'.[attributename]'`)。用来访问数组元素或者 JSON 对象的属性值。输出是单个值

  ```sh
  jq -n '{"a":1} | .["a"],.a'
  1
  1
  
  jq -n '[4,5] | .1'
  0.1
  jq -n '[4,5] | .[1]'
  5
  ```

  

* 数组切片操作(`'.[startindex:endindex]'`)，其行为类似于 python 语言中数组切片操作。

  ```sh
  jq -n '[4,5,6] | .[1:]'
  [
    5,
    6
  ]
  jq -n '[4,5,6] | .[1:1]'
  []
  ```

  

### **表达式操作(','和 '|')**

表达式操作是用来关联多个基础表达式。

其中逗号表示对同一个输入应用多个表达式。

```sh
jq -n '5 | .+1,.-1'
6
4
```



管道符表示将前一个表达式的输出用作后一个表达式的输入。当前一个表达式产生的结果是迭代器时，会将迭代器中的每一个值用作后一个表达式的输入从而形成新的表达式。例如`'.[]|.+1'`, 在这个表达式中，第一个子表达式`'.[]'`在输入数组上构建迭代器，第二个子表达式则在迭代器的每个元素上加 1。

```sh
jq -n '[4,5] | .+1,.-1'
jq: error (at <unknown>): array ([4,5]) and number (1) cannot be added

jq -n '[4,5] | .[] | .+1,.-1'
5
3
6
4
```





## **内置运算支持**

jq 内部支持的数据类型有：数字，字符串，数组和对象(object)。并且在这些数据类型的基础上, jq 提供了一些基本的操作符来实现一些基本的运算和数据操作。列举如下：

### **数学运算**

对于数字类型，jq 实现了基本的加减乘除(/)和求余(%)运算。对于除法运算，jq 最多支持 16 位小数。

### 字符串操作

jq 提供

* 字符串的连接操作(运算符为`'+'`，例如：`"tom "+"jerry"`结果为`"tom jerry"`)，

* 字符串的复制操作(例如：`'a'*3` 结果为`'aaa'`)

* 字符串分割操作(将字符串按照指定的分割符分成数组，例如`"sas"/"s"`的结果为`["","a",""]`，而`"sas"/"a"`的结果为`["s","s"]`。

* 其他数据类型转为string： `tostring`

  如 `.items | length | tostring`

### **数组操作**

jq 提供两种数组运算：

* 并集`('+')`运算，结果数组中包含参与运算的数组的所有元素。
* 差集运算`('-')`，例如：有数组 a,b, a-b 的结果为所有在 a 中且不包含在 b 中的元素组成的数组。



```sh
jq -n '[1,2] + [2,4]'
[
  1,
  2,
  2,
  4
]
# 还不同于set的并集

jq -n '[1,2] - [2,4]'
[
  1
]
```



#### 数组函数



* 扁平化 flatten

  ```sh
  jq -n '[1, [2,3]] | flatten'
  [
    1,
    2,
    3
  ]
  ```

  

* 反序 reverse

  ```sh
  jq -n '[1, [2,3]] | reverse'
  [
    [
      2,
      3
    ],
    1
  ]
  
  jq -n '[1, [2,3]] | flatten | reverse'
  [
    3,
    2,
    1
  ]
  ```

  

* 排序 sort, sort_by

  ```sh
  jq -n '[1, [2,3], 4] | sort'
  [
    1,
    4,
    [
      2,
      3
    ]
    # 看起来似乎认为数组比数字大
  ]
  
  jq -n '[{"a":2},{"a":1}]|sort_by(.a)'
  [
    {
      "a": 1
    },
    {
      "a": 2
    }
  ]
  ```

  

* 比较 min, min_by, max, max_by

  ```sh
   jq -n '[1,2,3,2] | min'
  1
  jq -n '[1,2,3,2] | max'
  3
  
  jq -n '[{"a":2},{"a":1}]|min_by(.a)'
  {
    "a": 1
  }
  jq -n '[{"a":2},{"a":1}]|max_by(.a)'
  {
    "a": 2
  }
  ```

  

* 查找 indices, index, rindex

  ```sh
  jq -nr '"abcb"|indices("b")'
  [
    1,
    3
  ]
   
  jq -nr '[1,3,2,3]|indices(3)'
  [
    1,
    3
  ]
   
  jq -nr '[1,"a",2,3,"a",4,5,"b",8,9]|indices("a")'
  [
    1,
    4
  ]
  ```

  ```sh
  jq -n '[1,2,3,2] | index(2)'
  1
  jq -n '[1,2,3,2] | index(0)'
  null
  
  jq -n '[1,2,3,2] | rindex(2)'
  3
  jq -n '[1,2,3,2] | rindex(0)'
  null
  ```

  

* 取长度： `length`

  `.items | length`





### 对象操作

jq 实现了两个 JSON 对象的合并操作(merge)。当两个参与运算的对象包含相同的属性时则保留运算符右侧对象的属性值。

有两种合并运算符：`'+'`和`'*'`。所不同的是，运算符`'+'`只做顶层属性的合并，运算符`'*'`则是递归合并。例如：有对象 `a={"a":{"b":1}}, b={"a":{"c":2}}`，`a+b` 的结果为`{"a":{"c":2}}`，而 `a*b` 的结果为`{"a":{"b":1,"c":2}}`



```sh
jq -n '{"a":{"b":1}} + {"a":{"c":2}}'
{
  "a": {
    "c": 2
  }
}

jq -n '{"a":{"b":1}} * {"a":{"c":2}}'
{
  "a": {
    "b": 1,
    "c": 2
  }
}
```





### **比较操作**

jq 内部支持的比较操作符有`==`, `!=`, `>`, `>=`, `<=`和 `<`。其中，`'=='`的规则和 javascript 中的恒等(`'==='`)类似，只有两个操作数的**类型和值均相同**时其结果才是 true。

### **逻辑运算符**`and`/`or`/`not`

在 jq 逻辑运算中，除了 **false 和 null** 外，其余的任何值都等同于 true。

### **默认操作符**(`'//'`)

表达式`'a//b'`表示当表达式 a 的值不是 false 或 null 时，a//b 等于 a，否则等于 b。

> 和py的`a or b`差不多



### 迭代器运算

jq 中有一种很特殊的运算规则：当运算符的一个或两个操作数是迭代器时，其运算以类似与笛卡尔乘积的方式进行，即把两个操作数中的每一个元素拿出来分别运算



```sh
jq -n '([1,2]|.[]) + ([2,4]|.[])'
3
4
5
6
# 笛卡尔积

jq -n '([1,2]) + [2,4]'
[
  1,
  2,
  2,
  4
]
# extend
```



### 映射操作

在数据处理过程中，经常需要将数据从一种形式转换成另外一种形式，或者改变数据的值。jq 提供了两个内置映射函数来实现这种转换：**map** 和 **map_values**。其中，**map 处理的对象是数组，而 map_values 则处理对象属性的值**。map 函数的**参数为 filter 表达式**。在该 filter 表达式中，**'.'**代表被映射的元素或值。

> 怀疑 filter 函数签名如下：
>
> ```go
> func Filter(item) (newItem, ok)
> 
> // map类似
> var ret []Obj
> for _, item in arr {
>   if item, ok := filter(item); ok {
>     ret = append(ret, item)
>   }
> }
> return ret
> 
> // select类似特殊filter
> func Select(item) (newItem, ok) {
>   if match(item) {
>     return item, true
>   }
>   return nil, false
> }
> ```
>
> 



```sh
jq -n '[1,2,3,4] | map(.+1)'
[
  2,
  3,
  4,
  5
]

# 等于： jq -n '[[1,2,3,4] | .[] | .+1 ]'
# 这个写法确实啰嗦


jq -n '{"a":1} | map_values(.+1)'
{
  "a": 2
}
```









### 过滤操作

在 jq 中有两种类型的选择过滤操作。

* 第一种是基于数据类型的过滤，如表达式 '.[]|arrays' 的结果只包含数组。可以用来过滤的类型过滤器有：

  * arrays
  * objects
  * iterables
  * booleans
  * numbers
  * normals
  * finites
  * strings
  * nulls
  * values
  * scalars

  ```sh
  jq -n '[1, "2"] | numbers'
  # nothing
  jq -n '[1, "2"] | arrays'
  [
    1,
    "2"
  ]
  
  jq -n '[1, "2"] | .[] | numbers'
  1
  jq -n '[1, "2"] | .[] | arrays'
  # nothing
  ```

  

* 第二种是 select 函数。select 接受一个条件表达式作为参数。其输入可以是迭代器，或者和 map 函数配合使用来处理数组。当输入中的某个元素使 select 参数中的条件表达式结果为真时，则在结果中保留该元素，否则不保留该元素。

  ```sh
  jq -n '[1,2,3] | map(select(.>1))'
  [
    2,
    3
  ]
  ```

  

### 路径操作

在 jq 中的 path 也是指从根到某个目录属性的访问路径。在 jq 中有两种表示路径的方式：数组表示法和属性表示法。属性表示法类似于在 filter 中访问某个属性值的方式，如'.a.b'。数组表示法是将路径中的每一部分表示为数组的一个元素。jq 提供了一个内置函数 path 用来实现路径从属性表示法到数组表示法的转换。

jq 还提供了函数用来读取路径的值(getpath), 设置路径的值(setpath)和删除路径(del)。不过遗憾的是，这三个函数对路径的处理并不一致。其中 getpath 和 setpath 只接受数组表示法的路径，而 del 函数只能正确处理属性表示法的路径。

jq 还提供了一个函数 paths 用来枚举可能存在的路径。在没有参数的情况下，paths 函数将输出 JSON 数据中所有可能的路径。paths 函数可以接受一个过滤器，来只输出满足条件的路径。



## 控制结构





# 格式转换



## 转为yaml

https://stackoverflow.com/questions/53315791/how-to-convert-a-json-response-into-yaml-in-bash

* 用类似的工具： `yq`

* 用py



## 筛选/过滤



`jq '.items[] | select(.metadata.annotations."sidecar.istio.io/inject" == "true")'`

```shell
jq '.result 
  | select(.property_history != null) 
  | .property_history 
  | map(select(.event_name == "Sold"))[0].date'
```





# 输出



## pretty、compact

* ` | jq .`： 默认pretty
  * ` | jq -c .` 一行输出



# 数据类型



## 特殊key： .["key"] or ."key"



## 特殊处理

### 空/null处理

* `try E` is equivalent to `try E catch empty`
* `try E` is equivalent to `(E)?`

## 数组

* `arr[]` 展开
  
  ```sh
  jq '.a[] as $i | $i+1 ' <<< '{"a": [1,2,3,null]}'
  2
  3
  4
  1  # null as 0 -> +1 = 1
  jq '.b[] as $i | $i+1 ' <<< '{"a": [1,2,3,null]}'
  jq: error (at <stdin>:1): Cannot iterate over null (null)
  jq '.b[]? as $i | $i+1 ' <<< '{"a": [1,2,3,null]}'  # empty output
  ```
  
  
  
* `[ <expr> ]` 收集

* `[ arr[] | select(<expr>) ]` 过滤筛选数组元素然后收集为数组

* **slice**： `.configs[0:2]`

* **length**： `.configs[0:2] | length`

  > arr如果不存在/null， `.items | length` 可能报错，可以：
  >
  > * `.items | .[]? | length`
  > * `try .items[]` 等于 `(.items[])?` 不够不等于 `.items[]?`



## 字符串

* ` -r               output raw strings, not JSON texts;`

  比如`jq '."results"[]["name"]'`可能得到

  ```
  "v1"
  "v2"
  ```

  带上`-r`之后双引号就没了



# 高级



## 变量

jq 内部支持两种变量的定义方式：
第一种定义方式：在前边 jq 的使用部分讲过，可以通过命令行参数（`--arg`)定义。这种方式用来从外部（如：shell)传入数据以供 filter 表达式使用。
第二种定义方式：在 jq 表达式内部，可以自己声明变量用来保存表达式的结果以供表达式其余部分使用。

jq 中定义变量的语句为：`fiterexp as $variablename`



```sh
### 在下面的表达式中变量$arraylen 用来保存数组长度，整个表达式结果为 4
jq -nr '[1,2,3]'
[
  1,
  2,
  3
]
jq -nr '[1,2,3]|length as $arraylen|$arraylen+1'
4
 
### 还可以同时定义多个变量
jq -nr '{"firstname":"shibo","lastname":"kevin"}'
{
  "firstname": "shibo",
  "lastname": "kevin"
}
 
[jq -nr '{"firstname":"shibo","lastname":"kevin"}|. as {firstname:$fn, lastname:$ln}|"author is "+$fn+" "+$ln'
author is shibo kevin
jq -nr '{"firstname":"shibo","lastname":"kevin"}|. as {firstname:$fn, lastname:$ln}|"author is "+$fn+" 和 "+$ln'
author is shibo 和 kevin
```



### 作用域

jq 中同样存在变量作用域问题。在 jq 中，有两种方法分隔变量作用域：
第一种方法：用括号包围部分表达式。括号内部的表达式与外部的表达式不在同一个作用域范围内。
第二种方法：定义函数。默认情况下，声明的变量对其后的表达式可见。但是，如果变量在特定作用域内声明，则对作用域外部的表达式不可见。



```sh
### 下面会抛出 arraylen 没定义的异常
jq -nr '[1,2,3]|(length as $arraylen|$arraylen)|$arraylen+1'
jq: error: arraylen/0 is not defined at <top-level>, line 1:
[1,2,3]|(length as $arraylen|$arraylen)|$arraylen+1                                       
jq: 1 compile error
 
### 下面正常执行，结果为 4.
jq -nr '[1,2,3]|(length as $arraylen|$arraylen+1)'
4
 
### 函数作用域。该表达式会抛出异常，因为变量$fn 是在函数 fname 中定义，对最后一个子表达式##来说，$fn 是不可见的。
jq -nr '{"firstname":"shibo","lastname":"kevin"}|def fname:. as {firstname:$fn, lastname:$ln}|$fn; fname|$fn'        
jq: error: fn/0 is not defined at <top-level>, line 1:
{"firstname":"shibo","lastname":"kevin"}|def fname:. as {firstname:$fn, lastname:$ln}|$fn; fname|$fn
 
jq -nr '{"firstname":"shibo","lastname":"kevin"}|. as {firstname:$fn, lastname:$ln}|$fn'          
shibo
```







## reduce

jq有一种特殊的数据类型：迭代器。通常有迭代器参与的运算，其结果也是一个迭代器。jq 提供了一些特殊的语法和内置函数用来缩减迭代器运算结果的个数。

reduce 关键字用来通过运算将迭代器的所有值合并为一个值。其调用形式为：`reduce <itexp> as $var (INIT; UPDATE)`。其中，

* 表达式 itexp 产生的迭代器被赋值给变量 var

* UPDATE 是关于变量 var 的表达式

  相对于 itexp 结果中的每个元素，UPDATE 表达式被调用一次，计算出结果用作下一次 UPDATE 调用的输入

* INIT 是该表达式的初始输入

 

```sh
jq -n 'reduce ([1,2,3,4] | .[]) as $var (0; .+$var)'
10

jq -n 'reduce ([1,2,3,4]) as $var (0; .+$var)'
jq: error (at <unknown>): number (0) and array ([1,2,3,4]) cannot be added
```





### foreach

> jq的foreach跟一般语言的有些差异。 它更多是**一种特殊reduce**： 允许把每次reduce的结果也提取出来 用一个**迭代器**来输出

关键字 foreach 的作用和 reduce 类似。其调用形式为 `foreach EXP as $var (INIT; UPDATE; EXTRACT)`。和 reduce 关键字不同的是，foreach 关键字的每次迭代是先调用 UPDATE 再调用 EXTRACT，并以一个迭代器保留每一次的中间结果。该迭代器最后作为整个表达式的结果输出。



```sh
jq -n 'foreach ([1,2,3,4] | .[]) as $var (0; .+$var; . * 2)'
2
6
12
20

jq -n 'foreach ([1,2,3,4] | .[]) as $var (0; .+$var; . * 2) | . + 1'
3
7
13
21
```





## 函数

作为一个类似于编程语言的表达式系统，jq 也提供了定义函数的能力。其语法规则为：`def funcname(arguments) : funcbodyexp;` 在定义函数时，需要注意下面几条规则。

* 函数名或者参数列表后面应该跟冒号以标志函数体开始。
* 如果不需要参数，可以直接把整个参数列表部分省去。
* 参数列表中，**参数之间以分号(";")分隔**。
* 函数体**只能是一个表达式**，且表达式需以分号结尾。
* 如果在表达式内部定义函数，整个子表达式部分不能只包含函数定义，否则 jq 会抛出语法错误。

在很多情况下，**函数的参数都是被当作表达式引用的**，类似于编程其他语言中的 callback 函数。 如果希望传入的参数只被当作一个简单的值来使用，则需要把参数的值定义为一个同名变量，并按照使用变量的方式引用。



* `map`类似： `def map(f): [.[] | f];`



```sh
jq -nr '5|def foo(f): f|f;foo(.*2)'  # 等于 5 | .*2 | .*2
20
jq -nr '5|def foo(f): f|f;foo(.*3)'
45

# 值参数
jq -nr '5|def foo($f): $f|f;foo(.*3)'  # 等于 5 | 15 | .*2
45
jq -nr '5|def foo($f): $f|$f;foo(.*3)'  # 等于 5 | 15 | 15
15

jq -nr '5|def foo(f): f;foo(.*3)'  # 等于 5 | .*3
15
```



### 递归函数

函数内部可以定义子函数。利用这个特性我们可以实现递归函数。



```sh
# sum 实现（reduce）

jq -nr '[1,2,3,4,5] | 
def total: 
	def _t: . | first + (if length>1 then .[1:] | _t else 0 end);
  _t;
total'
# first 和 length 特殊函数吧

jq -nr '[1,2,3,4,5] | first, length'
1
5
```







## 模块



除了在表达式内部定义函数外，还可以把自定义函数写在外部文件中形成单独的类库。jq 有一套完整的模块系统来支持自定义类库。
1）首先可以通过命令行参数'-L'来指定 jq 搜索模块时需要搜索的路径。
2）其次在模块内部，可以通过 import 指令和 include 指令来实现互相引用。在引用指令中，有几个特殊的路径前缀需要说明。
   **'～'**， 表示当前用户的 home 目录
   **'$ORIGIN'**，表示 jq 命令的可执行文件所在的目录
   **'.'**，表示当前目录，该前缀只能用在 include 指令中。

当通过 import 指令引用一个模块 foo/bar 时, jq 会在搜素路径中查找 foo/bar.jq 或者 foo/bar/bar.jq。







# interactive jq



## [jiq](https://github.com/fiatjaf/jiq)



```s
[jq]> .configs[] | ."@type"
                                                                                       "type.googleapis.com/envoy.admin.v3.BootstrapConfigDump"
"type.googleapis.com/envoy.admin.v3.ClustersConfigDump"
"type.googleapis.com/envoy.admin.v3.DubboRoutesConfigDump"
"type.googleapis.com/envoy.admin.v3.ListenersConfigDump"
"type.googleapis.com/envoy.admin.v3.ScopedRoutesConfigDump"
"type.googleapis.com/envoy.admin.v3.RoutesConfigDump"
"type.googleapis.com/envoy.admin.v3.SecretsConfigDump"
"type.googleapis.com/envoy.admin.v3.ThriftRoutesConfigDump"


[jq]> .configs[0].bootstrap.
 admin dynamic_	resources layered_runtime node static_resources stats_config tracing 
 # 只是访问的话可
```



有个很重要的功能没有： enter/dive-into

比如以上，如果支持箭头上下，选中-enter进去 以 选中条目为条件搜索的结果



```s
[jq]> [ .configs[] | ."@type" ][0]
```



