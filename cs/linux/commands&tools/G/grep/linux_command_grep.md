



# tutorial



## case



### match



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







