



# tutorial



## replace



### 替换多个pattern



```shell
's/pattern1\|pattern2\|pattern3/replacement/g'
```



如：

```shell
echo 123haha456 | sed -e 's/123\|456/nn/g'
# output:
#   nnhahann
```



但`\|`限于GNU sed，**mac sed**不支持。 不过可以通过brew手动安装`gnu-sed`，最后binary为`gsed`。



## print



### print指定行（范围）



```shell
sed -n "${first_line},${last_line}p" filename
sed -n "${first_line},${last_line}p;${last_next_line}q" filename
# 这里加上 `${last_next_line}q` 是为了 “及早退出”，不然会一直scan到eof。 对于大文件，这样有意义！
```















