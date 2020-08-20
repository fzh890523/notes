

## 进程管理

### 查看进程环境变量

`ps eww ${pid}`

因为没有 `/proc/${pid}/environ`



## 文件（系统）管理



### 部分目录无法访问问题： `Operation not permitted`

比如ls无法显示内容等等。是因为macos的`System Integrity Protection (the SIP)`。
应对方式：

* **不建议** 关掉SIP
  命令行执行`csrutil disable` 然后重启（反向操作为`csrutil enable`

* Go to System Preferences > Security & Privacy and give Full Disk Access to Terminal

  ** Apple menu** -> **System Preferences** -> **Security & Privacy** -> **Privacy** -> **Full Disk Access ->** +

  Choose:

  **Applications** -> **Utilities** -> **folder** -> **Terminal** -> **open/grant terminal full disk access privileges.** -> **Relaunch terminal**

  > 其他app同理



## 媒体管理



### 照片管理



#### 怎么备份照片库

官方说法：

* timemachine

* 人肉备份照片库目录

  在这个的基础上，可以用一些同步的软件，比如 qnap的qsync。

  不过这个目录比较特殊，需要给权限才能访问，详见上面
  
  > 给了权限能访问了，也选不中。 最后把父目录（Pictures）做了同步，囧



## 远程桌面



### windows远程桌面连接到macos



用标准vnc协议，软件是vnc viewer。 默认是`509x`端口，可以在macos上先看下（`sudo lsof -nP -sTCP:LISTEN -i:5900`），有时可能没打开。

暂时知道的：

* `screen sharing`会开vnc server

* `remote management`开启后`screen sharing`会默认灰掉，不清楚vnc server是开启还是“忘了关”

  遇到的case就是vnc server用不了，然后关掉这个重新打开ss就可以了，然后再打开这个。。





