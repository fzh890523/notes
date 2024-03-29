



# tutorial



## delete



`d`操作

ref：<https://stackoverflow.com/questions/5410757/delete-lines-in-a-text-file-that-contain-a-specific-string>

> sed是per-line op，对于没有换行符的内容，还是会追加换行符，这个行为比较烦。 如：
>
> `echo 1 | sed 's/\n$//'`
>
> 可以：
>
> echo 1 | tr -d '\n'



To remove the line and print the output to standard out:

```sh
sed '/pattern to match/d' ./infile
```

To directly modify the file – does not work with BSD sed:

```sh
sed -i '/pattern to match/d' ./infile
```

Same, but for BSD sed (Mac OS X and FreeBSD) – does not work with GNU sed:

```sh
sed -i '' '/pattern to match/d' ./infile
```

To directly modify the file (and create a backup) – works with BSD and GNU sed:

```sh
sed -i.bak '/pattern to match/d' ./infile
```





## replace



### 先match line再replace

```sh
/<line-match-pattern>/s/<replace-match-pattern>/<replacement>/
```





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



### 多个替换分支: -e



```shell
sed -i -e 's/123/456/' -e 's/abc/ABC/' a.txt
```



### 替换为换行符



BSD上（macos也是）比较麻烦，估计要写为：

```sh
sed -i -e 's/\.\.\./\
/g'
```







## 写回原文件: -i (in-place)

```sh
sed -i ${expr} ${file}
```



## print



### print指定行（范围）



```shell
sed -n "${first_line},${last_line}p" filename
sed -n "${first_line},${last_line}p;${last_next_line}q" filename
# 这里加上 `${last_next_line}q` 是为了 “及早退出”，不然会一直scan到eof。 对于大文件，这样有意义！
```















