



# tutorial



## case



### different RE



* `PCRE` : `-P`

* `extended RE`: `-E` 

  == egrep



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







