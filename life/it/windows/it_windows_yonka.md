







# FS



## (软)链接



参考《windows_command_mklink.md》



## network



### smb

一些命令：

```sh
 net use /?
此命令的语法是:

NET USE
[devicename | *] [\\computername\sharename[\volume] [password | *]]
        [/USER:[domainname\]username]
        [/USER:[dotted domain name\]username]
        [/USER:[username@dotted domain name]
        [/SMARTCARD]
        [/SAVECRED]
        [/REQUIREINTEGRITY]
        [/REQUIREPRIVACY]
        [/WRITETHROUGH]
        [[/DELETE] | [/PERSISTENT:{YES | NO}]]

NET USE {devicename | *} [password | *] /HOME

NET USE [/PERSISTENT:{YES | NO}]
```

如：

```sh
# 注意： sharename好像是必须的
net use Q: \\192.168.1.2\xxx /user:xxx xxx
net use \\192.168.1.2 /del  # 清除该server的已有信息，比如username
```



#### （低版本qnap nas）无法直接ip拉出共享目录问题

表现为：

* qnap nas不行
* mac、其他windows的可以



原因：这个nas版本比较低(4.3.3)，只支持smb 1(也可能是bug，比如在smb1被禁用时没有在高版本smb上提供discovery，导致client侧这个server的共享目录list为空)

解法：

* 升级nas到4.3.4+(但可能nas硬件太老不支持)
* 让windows重新安装启用smb1
  1. windows功能里开启smb1
  
  2. 组策略里开启`不安全的来宾登录`(`insecure guest login`)
    主要是没找到输入用户名的地方，只能。。。
    
    `gpedit.msc - 计算机配置 - 管理模板 - 网络 - Lanman工作站 - ...`
    
  3. win-r打开运行窗口，输入： `\\<nas-ip>` 会打开这个nas的网络邻居窗口，其中会有提供的共享目录
    这里不能输入类似 `\\<user>@<nas-ip>`
    比较烦的是，无论是映射网络硬盘还是新增网络位置，都需要指定共享目录而不能是整个。。。








