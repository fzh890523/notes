







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





## config

### code style



#### gofmt/goimports 自动处理



`go to preferences ->Tools ->File Watchers and enable go fmt . This way on each save it will format the file.`



* 可以配置一个gofmt和一个goimports，应该是pipeline处理
* 默认行为应该是save时触发
* scope默认是project files，应该不用特别的设置



#### Reformat code 跟 gofmt/goimports 可能的冲突



`alt+cmd+L` 会`reformat the code`，但对于imports的处理，跟goimports好像有些冲突：


* goimports 

  需要显式的 `a "xx.com/yy/a/client"`(当`client`包名为`a`时)，否则这个import item会被去掉

* reformat

  会自动分析出不必要的显式重命名包名并且去掉，于是 `a "xx.com/yy/a/client"` -> `"xx.com/yy/a/client"` 然后就会被goimports去掉了

处理办法： `shift+alt+cmd+L` 在dialog里把`optimize imports`去掉勾选



#### go lint使用



file watch那里有一个`go metalinter`，但似乎已经是deprecated项目了。

现在`golangci-lint`据说比较好。与goland的集成见https://github.com/golangci/golangci-lint，但还处于“调外部命令输出结果到console”阶段，而且输出比较奇怪，基本不可用。

有个 go linter的jb插件是基于golangci-lint，但第一个版本之后就没维护了，不兼容了。



感觉goland发力太少，都比vsc差很多了，囧。

y



























