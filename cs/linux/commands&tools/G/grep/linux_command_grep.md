



# tutorial



## case



### output



### （不）带文件名

grep多个文件时，默认会带上 `xxx.txt:` 这样的行前缀



* `-H` 带

  默认行为

* `-h` 不带



#### 到管道/非终端设备

类似awk，对于带缓冲的设备，会有迟迟无内容显式的情况，尤其是pipe后面的命令还等着输入的时候。

去掉缓冲、设置缓冲的方式：

* `--line-buffered` 顾名思义，每行flush



### match



#### 特殊字符



##### tab `\t`

* 可以用pcre正则： 

  ```sh
  grep -P "\\t"
  ```

* 也可以“真的”传个tab字符过去

  ```sh
  grep $'\t' sample.txt
  # $这个语法应该是当前shell支持的，解释转义。 可以用 `echo -n -E $"\t   "` 来查看，对照组： `echo -n -E "\t   "`
  ```

* 用printf来代替shell内建的`$''`： `grep "$(printf '\t')"`

* 直接使用tab键： `grep "^V<tab>" <filename>` 

  但可能依赖终端。 比如一些终端软件自己就支持了`ctrl-v`那就不行了





#### binary file



有些时候，文件里包含一些特殊字符，导致grep时会报：

`Binary file (standard input) matches`。



这时候如果确认还是文本文件的话，可以强行用`-a`来指示其当做文本处理。

```
-a, --text
Process a binary file as if it were text; this is equivalent to the ‘--binary-files=text’ option.
```





#### simple contains/fixed string

* `grep -F ${fixStr}`
* `fgrep`



```sh
echo 1223 | grep "2*"
1223
echo 1223 | grep -F "2*"
echo 122*3 | grep -F "2*"
122*3
```



#### multiple + fixed string: `grep -F -e -e`

`egrep`不支持-F



#### Multiple match(or)

* `grep -E` 

  == egrep

*  `grep -e -e …` 

  ```sh
  echo -e "123\n456\n789" | grep -e "12" -e "56"  # 这里就不能用 "12|56" 了
  ```

  

*  `egrep`

  ```sh
  echo -e "123\n456\n789" | egrep "12|56"
  ```

  

#### multiple + only-matching: `grep -o -e -e`

必要时还可以加上`-E`来扩展匹配。



```sh
ps aux | grep envoy |  grep -o -E -e "^root +" -e "restart-epoch [^ ]+"
# 不过问题是会多行输出
```





### different RE



* `PCRE` : `-P`

* `extended RE`: `-E` 

  == egrep



### 目录查找 `-r`



#### 排除指定目录 `--exclude-dir`

如： `grep -rn hahaha --exclude-dir=.git --exclude-dir=aa/bb ./`



# detail



## exit code



grep很烦，用有没有match来确定返回值，即使对于`-v`，如：

```sh
echo 123 > a.txt
grep 456 a.txt  # exit 1
grep 123 a.txt  # exit 0
grep -v 123 a.txt  # exit 0
grep -v 456 a.txt  # exit 1

# 其他文件读取错误等另说
```







