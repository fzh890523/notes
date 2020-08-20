







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


net use [drive] [share] /user:[user] [password]
```

如：

```sh
# 注意： sharename好像是必须的
net use Q: \\192.168.1.2\xxx /user:username password  # password为*则会prompt提示输入
# 可以不加映射盘符，也即上面的 Q: 可以不要
net use \\192.168.1.2 /del  # 清除该server的已有信息，比如username

net use * /del
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





# system admin



## reboot/restart



### 查看重启记录



事件查看器 - windows日志 - 系统，过滤事件ID为`1074`的事件即可。

可以看到类似 

```
进程 C:\Windows\System32\usocoreworker.exe (DESKTOP-N8VIQBJ) 由于以下原因已代表用户 NT AUTHORITY\SYSTEM 启动计算机 DESKTOP-xxx 的 重启: 操作系统: Service Pack (计划内)
 原因代码: 0x80020010
 关机类型: 重启
 注释: 
```

的信息，以及更详细信息。



## system update



### 禁止更新后自动重启



https://answers.microsoft.com/en-us/windows/forum/all/disable-windows-10-automatic-restart-after-updates/16f1826d-a796-4de8-ac99-1d625420d265





* <del>设置活动时间段（重启只会在时间段外，最大18h）</del>

* 在调度任务管理中把reboot禁用 - 也不太行

  但新版本里好像改了，reboot默认禁用，而是通过 USO_UxBroker 来触发， broker这个任务没权限改动

* 组策略里设置： 启用`NoAutoRebootWithLoggedOnUsers `

  ```
  Computer Configuration ->
  ->Administrative Templates->
  -> Windows Components->
  -> Windows Update
  -> 对于有已登录用户的计算机，计划的自动更新安装不执行重新启动
  
  # 还没验证，这个设置的说明里提到： 只有在将自动更新配置为执行计划的更新安装时，此策略才适用。如果禁用了“配置自动更新”策略，则此策略不起作用。  而这个默认没配置，也即采用其选项  “3 = (默认设置)自动下载更新，并在准备好安装时发出通知” ，可能和本设置冲突
  ```

  

  ```sh
  # 注册表方式
  reg add "HKLM\SOFTWARE\Policies\Microsoft\Windows\WindowsUpdate\AU" /v NoAutoRebootWithLoggedOnUsers /t REG_DWORD /d 1 /f
  ```

  