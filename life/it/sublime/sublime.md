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



# selection



## 列模式

1. 苹果：OS X

   －鼠标左键＋Option

   －或者鼠标中键

   －增加选择：Command，减少选择：Command+Shift

2. Windows：

   －鼠标右键＋Shift

   －或者鼠标中键

   －增加选择：Ctrl，减少选择：Alt

3. Linux：

   －鼠标右键＋Shift

   －增加选择：Ctrl，减少选择：Alt




# 插件



## 文本处理



### 文本比较



#### FileDiffs

可以比较标签页等。

* with clipboard
* with file in project
* with opened tab
* with previous tab










