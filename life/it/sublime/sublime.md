# regular expression



## 多行



## 非贪婪匹配

在...后加上`?`改变贪婪行为

```
href="http://.*?/113\.png"
```



## capture替换

```
href="http://.*?/(\d+)\.png"
# replace to
href="1-基础/$1.png"
```



```
src=".*?/(\d+)\.png
# replace to
src="1-基础/$1.png
```









