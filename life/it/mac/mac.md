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

