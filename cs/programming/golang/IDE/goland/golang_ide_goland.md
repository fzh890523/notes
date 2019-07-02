







## Debug



```
please go to "Help | Debug Log Settings..." and add the following line "#com.goide.dlv.DlvVm" (please preserve the "#" symbol).
Then go run the debug session as before, let the issue be encountered, then go to "Help | Debug Log Settings..." and remove that line, then go to "Help | Compress Logs and Show in...", take the archive and publish it here.
You can set the visibility to "jetbrains-team" only so that only we can see the contents of the archive.

from https://intellij-support.jetbrains.com/hc/en-us/community/posts/360000381100-goland-debugger-does-not-work-correctly-with-GoLand-2018-2
```



## items



### watch global variables

点`+`号，然后手动输入变量名。 一次只能加一个，囧。
没办法直接设置列出所有全局变量。 = =



类似的可以`+`各种。。。



### evaluate

#### 各种常见evaluate都用不了，不过可以强行取址访问。

比如`[]byte`类型变量/字段上右键`1`，会出现类似`*(*"[]uint8")(842351617008)`，可以对其取切片，如`(*(*"[]uint8")(842351617008))[823:]`，或者根据地址偏离来加减该地址内容。



#### `a.b == nil` 好像可以



#### function call: 暂不支持(19-03-27)

https://youtrack.jetbrains.com/issue/GO-3433



最近评论是19-01-10，还么支持。



### breakpoint



https://www.jetbrains.com/help/go/using-breakpoints.html


































