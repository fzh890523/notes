





### 编码/显示乱码问题

首先需要终端client设置好字符集，有些是在终端client软件里设置，有些可以通过命令修改。 比如win cmd环境中可以 `chcp 65001` 来切换codepage为utf-8。


**以ssh到macos中文显示???问题为例**

* ssh到macos显示中文为 `???` （win-ssh->macos, ubuntu-ssh->macos 都这样）
* macos GUI shell里显示正常
* win-ssh->ubuntu 显示中文正常
* cmd/powershell里`chcp 65001`执行了，还是不行。 执行后各种对终端侧的编码检测都能通过



后来发现是**macos的login-shell缺少字符集设置**。



ssh shell里：

```sh
locale
# 输出都是空或者 C
LANG=
LC_COLLATE="C"
LC_CTYPE="C"
LC_MESSAGES="C"
LC_MONETARY="C"
LC_NUMERIC="C"
LC_TIME="C"
LC_ALL=
```



而在macos的gui terminal的shell里可以看到

```sh
locale
# 输出
LANG="zh_CN.UTF-8"
LC_COLLATE="zh_CN.UTF-8"
LC_CTYPE="zh_CN.UTF-8"
LC_MESSAGES="zh_CN.UTF-8"
LC_MONETARY="zh_CN.UTF-8"
LC_NUMERIC="zh_CN.UTF-8"
LC_TIME="zh_CN.UTF-8"
LC_ALL=
```
看了下`/etc/profile`里也没，不清楚是从哪里加载的。




在`~/.bash_profile`上加入如下内容即可 （因为是login shell，所以不能在rc文件里加。 除非profile里source了rc文件）

```sh
export LANG="en_US.UTF-8"
export LC_COLLATE="en_US.UTF-8"
export LC_CTYPE="en_US.UTF-8"
export LC_MESSAGES="en_US.UTF-8"
export LC_MONETARY="en_US.UTF-8"
export LC_NUMERIC="en_US.UTF-8"
export LC_TIME="en_US.UTF-8"
export LC_ALL="en_US.UTF-8"
```

