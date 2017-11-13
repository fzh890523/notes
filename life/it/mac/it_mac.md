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

### 在文件选择窗口选择/进入隐藏文件/目录： cmd+shift+g 然后输入要去的path



## 窗口操作

### 关闭当前窗口： cmd+f4（需要把function功能调整回来，否则还需要fn）

### 显示桌面： f11 或者 五指触摸板展开

### 显示最近应用： 五指触摸板收缩

### 同应用多窗口切换： cmd + ~



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



# script



## 执行



### 双击执行sh

- First in terminal make the script executable by typing:

  ```
  chmod a+x (yourscriptname)
  ```

- Then, in Finder, right-click your file and select "Open with" and then "Other...".

- Here you select the application you want the file to execute into, in this case it would be Terminal. To be able to select terminal you need to switch from "Recommended Applications" to "All Applications". (The Terminal.app application can be found in the Utilities folder)

- NOTE that unless you don't want to associate all files with this extension to be run in terminal you should not have "Always Open With" checked.

- After clicking OK you should be able to execute you script by simply double-clicking it.







# 命令行

## netstat

### 替代品

BSD风格的netstat相比GNU的太难用了，要实现`netstat -alnp`的效果，在mac中可以：`lsof -nP -iTCP -sTCP:LISTEN` （虽然挺不一样的 = =）。

格式：`lsof -nP -iTCP:端口号 -sTCP:LISTEN`

类似的有：

* lsof -nP -iTCP -sTCP:LISTEN
* lsof -nP -iTCP:4000 -sTCP:LISTEN


## sz rz

Ref: [[mac iterm2 安装 lrzsz  rz sz命令](http://blog.csdn.net/jack85986370/article/details/51382077)



首先mac自带的终端是不支持lrzsz的，需要下载安装iterm2，下载地址：

[http://www.iterm2.cn/download](http://www.iterm2.cn/download)

1. 安装lrzsz

```shell
brew install lrzsz
```

2. 安装脚本到mac指定目录，地址在： [https://github.com/mmastrac/iterm2-zmodem](https://github.com/mmastrac/iterm2-zmodem)

保存 iterm2-send-zmodem.sh 和 iterm2-recv-zmodem.sh 到mac的 /usr/local/bin/ 路径下 
`注意添加脚本可执行权限：`

```shell
chmod +x iterm2-send-zmodem.sh 
chmod +x iterm2-recv-zmodem.sh  1212
```

------

3. iterm2 添加 triggers

```
Regular expression: \*\*B0100
    Action: Run Silent Coprocess
    Parameters: /usr/local/bin/iterm2-send-zmodem.sh
Regular expression: \*\*B00000000000000
    Action: Run Silent Coprocess
    Parameters: /usr/local/bin/iterm2-recv-zmodem.sh 
```

添加步骤：command+“,” 组合键打开“Preferences”面板->Profiles选项卡->Advanced->Triggers（点击Edit即可）

![这里写图片描述](http://img.blog.csdn.net/20160512103326982)

> add、按照上面的内容在GUI里填入...

4. 重启iterm2，链接远程[Linux](http://lib.csdn.net/base/linux)，输入rz命令试一下吧（注意上传文件路径不能包含中文）。



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



# 文本操作

## 删除

### 删除光标之前（win的backspace）： delete

### 删除光标之后（win的del）： fn+delete

### 删除光标之前的一个词（英文有效）： alt+del

### 删除光标之后的一个词： fn+alt+del

### 删除光标之前的整行内容：cmd+del



# 媒体

## 音频

## 视频

### 屏幕录制

#### 自带quicktime

默认不能录制系统音频，只能选择麦克风作为音频输入，可以安装soundflower，原理类似作为一个虚拟音频设备，quicktime录制时可以选择该设备作为音频输入。

注意，使用时需要设置该设备作为音频输出。

`系统音频 - soundflower - quicktime`



但这样的话就没有音频输出到外设了，也就是听不到声音了。

解决办法：

在MIDI（音频设备）系统设置里创建多输出设备，选择内建输出和soundflower的输出然后把该多输出设备作为音频输出就OK了（如果用了外置声卡的话也是类似处理、选中）。



参考：

* [Mac 小贴士：整合多台音频设备](http://www.midifan.com/modulearticle-detailview-5413.htm)
* [Record your computer's screen with audio on a Mac](https://www.cnet.com/how-to/record-your-computers-screen-with-audio-on-a-mac/)
* [Mac 下有什么好用的屏幕录像软件？](https://www.zhihu.com/question/19595687)



# alfred

暂时放这里吧。



## file explorer

### 打开文件

空格或单引号'然后输入文件名即可

（性能开销不知道如何，哎，以后可以考虑定制，在指定目录下搜索。 目前还是挺快的）



### 打开目录

飘号~或者斜杠/然后就出现目录了，可以代替大部分finder，操作很便利。





# sublime



## 文本

### 列选择



# 用户



## root用户执行



### root用户执行/启动app

* sudo open

  如：

  ```shell
  sudo open /Applications/VisualVM.app
  ```

  好像不行，open后面又以普通用户运行了。

* sudo /Applications/ApplicationName.app/Contents/MacOS/ApplicationName

  `-b` : background

  这个可以。






# os



## kernel

### 类似strace



#### dtrace

```shell
sudo dtrace -ln 'syscall:::entry'

sudo dtrace -qn 'syscall::write:entry, syscall::sendto:entry /pid == $target/ { printf("(%d) %s %s", pid, probefunc, copyinstr(arg1)); }' -p $SERVER_PID

sudo dtrace -n 'syscall::open*:entry { printf("%s %s", execname, copyinstr(arg0)); }'
```



#### dtruss

```shell
sudo dtruss -t open_nocancel -p $SERVER_PID
# sudo strace -e open -p $SERVER_PID
```





## process



### 查看进程cwd - lsof

* `lsof -d cwd`

  查看所有进程的cwd

* `lsof -a -d cwd -p ${pid}`

  查看指定进程的cwd

> 需要具有要查看进程的权限，同user或者root



