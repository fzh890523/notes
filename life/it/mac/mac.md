# 快捷键



## 截屏

### 截取全屏+保存到桌面： shift+cmd+3

### 截取指定区域+保存到桌面： shift+cmd+4

### 截取全屏+保存到剪切板： ctrl+shift+cmd+3

### 截取指定区域+保存到剪切板： ctrl+shift+cmd+4

## 文件操作

### 新建文件夹： cmd+shift+n

### 删除文件/目录： cmd+del

### 立即删除文件/目录（不进入废纸篓）： cmd+option+del

## 窗口操作

### 关闭当前窗口： cmd+f4（需要把function功能调整回来，否则还需要fn）

### 显示桌面： f11 或者 五指触摸板展开

### 显示最近应用： 五指触摸板收缩

# 交互

## finder相关

### 新建文本文档功能

```
作者：谢俊驰
链接：https://www.zhihu.com/question/20883777/answer/81780928
来源：知乎
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

Automator 新建一个 Application<img src="https://pic1.zhimg.com/78ce627667e5b8c7ab1bcff125e19968_b.png" data-rawwidth="97" data-rawheight="109" class="content_image" width="97">添加一个动作 "Run AppleScript"<img src="https://pic2.zhimg.com/b1931d5f1328d30559719088c9faffb1_b.png" data-rawwidth="1816" data-rawheight="1088" class="origin_image zh-lightbox-thumb" width="1816" data-original="https://pic2.zhimg.com/b1931d5f1328d30559719088c9faffb1_r.png">代码如下on run {input, parameters}

	tell application "Finder"
	set selection to make new file at (get insertion location)
	end tell

	return input
end run
保存到 "应用程序"文件夹, 名字姑且叫 "New File.app" 吧.Finder 工具栏右键, 自定义, 然后把 New File.app 拖上去, 大功告成
```



# 命令行

## netstat

### 替代品

BSD风格的netstat相比GNU的太难用了，要实现`netstat -alnp`的效果，在mac中可以：`lsof -nP -iTCP -sTCP:LISTEN` （虽然挺不一样的 = =）。

格式：`lsof -nP -iTCP:端口号 -sTCP:LISTEN`

类似的有：

* lsof -nP -iTCP -sTCP:LISTEN
* lsof -nP -iTCP:4000 -sTCP:LISTEN




# 文件

## 修改默认打开的程序

1. 类win方式
   * 操作： 右键 - 打开方式 - 其他，然后选择好程序后勾选中☑️“始终以此方式打开”
   * 效果： 似乎不行 = =（仅限一个文件？）
2. 右键 然后安卓alt，这时候打开方式会变成始终以此方式打开
   * 效果： 好像还是不行
3. 选中文件，cmd+i（或者finder的file - 显示简介）- 简介窗口里的打开方式选项 - 选择好后点一下“全部更改”
   * 效果： ok，但可能需要设置几次 = =

# sublime

## 列模式/列选择

option+鼠标左键（或触摸板）




