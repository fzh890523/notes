

# tutorial



## map/dict



* 可以直接用不需要定义、声明
* 判断是否存在： `if($5 in m)`



```sh
grep xxx xxx.log | awk -F "," '{print substr($1, 1,19) "/" $2 "/" $NF}'  | awk -F'/' '{if($3 == 15102){print}}' | awk -F'/' '{
if($5 in m && m[$5] != $6 && (m[$5] == 0 || $6 == 0)){
	printf("%s %s %d %d\n", $1, $5, m[$5], $6)
}
m[$5]=$6
}'
```







## func



### define func

```shell
xxx | awk '
function do_sth(arg) {
    return 1;
}
BEGIN {}
{
    // body
}
'
```





### string



#### index (find sub string)

```c
// awk的index从1开始

i = index(s, sub)  // 123.456 .
if(i == 0) {  // not found
    return s;
} else {  // 4
    return substr(s, 1, i - 1);  // 123
}
```



#### substr



```c
substr(s, 1, i)  // index从1开始，表示截取第i个字符之前的部分
```









