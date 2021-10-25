







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



> 注意，这个不要用管理员权限运行。 否则就挂在给了admin而当前用户还是看不到



#### 不可用、超时带来的文件浏览器的卡顿

ref：

* https://docs.microsoft.com/en-us/archive/blogs/openspecification/cifs-and-smb-timeouts-in-windows
* https://docs.microsoft.com/en-us/archive/blogs/openspecification/smb-2-x-and-smb-3-0-timeouts-in-windows

主要的是及时断掉连接避免不必要的重试和超时等待： `net use Z: /delete`

因为gui一点上去就卡住，没办法正常操作（bug



#### windows server等能发现network share但点开提示无效网络路径



ref： https://docs.microsoft.com/en-us/troubleshoot/windows-server/networking/guest-access-in-smb2-is-disabled-by-default

`[HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\LanmanWorkstation\Parameters] "AllowInsecureGuestAuth"=dword:0`

改为1



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





# services



## nfs

> 不提供nfs server，可以用 hanewin_nfs_server_V1.1.69 
>
> 部分版本提供nfs client（需要开启）



### client



* `show -e <ip>` 查看指定nfs server的（shares）信息

  ```
  showmount -e [server]    显示 NFS 服务器导出的所有共享。
  showmount -a [server]    列出客户端主机名或 IP 地址，以及使用“主机:目录”格式显示的安装目录。
  showmount -d [server]    显示 NFS 服务器上当前由某些 NFS 客户端安装的目录。
  ```

  

* GUI操作映射网络驱动器（如`G:`）为`\\<ip>\<share>` 即可（类似samba的操作）

  > 需要**读写权限**的需要修改注册表
  >  通过修改注册表将windows访问NFS时的UID和GID改成0即可，步骤如下
  >  1、在运行中输入regedit，打开注册表编辑器；
  >  2、进入HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\ClientForNFS\CurrentVersion\Default条目；
  >  3、选择新建----QWORD值，新建AnonymousUid，AnonymousGid两个值，值为0；
  >  4、`重启电脑` 注册表才会生效；
  >
  > ref： https://blog.csdn.net/qq_34158598/article/details/81976063

* 命令行方式挂载： `mount <ip>:<share> <盘符>` 

  如： `mount 192.168.1.2:/public F:`

  > 用cmd吧，psh好像有些莫名其妙的问题比如prompt让输入什么什么...

* 卸载： 命令行的`umount <盘符>` 或者全部 `umount -f -a`

  > **不要使用资源管理器的“断开网络驱动器”**
  >
  > > yonka: 这个好像经常卡顿，所以才不建议使用吧





### hanewin nfs server

几个注意点：

* 入口在start menu里，而不是安装目录里

* 需要管理员权限运行

  > 配置修改处，如果不这样的话，可能导致reload失效，保存配置和加载配置不一致等



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

  



## hyper-v



* ` bcdedit /set hypervisorlaunchtype off`

  `auto`为启用



## services

见 [it_windows_services.md](it_windows_services.md)



# remote desktop



## items



### 让远程电脑本地播放音频

默认是使用（它的）远程音频（也即在连接的client机器上播放），在远程桌面里无法修改。

而是要在远程桌面的连接属性里去改。





### 不允许用保存的凭据登陆

```
Your system administrator does not allow the use of saved credentials to log on to the remote computer
```

https://serverfault.com/questions/396722/your-system-administrator-does-not-allow-the-use-of-saved-credentials-to-log-on



可以用下面方法之一

* 在 控制面板 - 凭据管理器 里把凭据从windows凭据 移到 普通凭据（删除、再添加）

* 在组策略里改... 

  ```
  Enable the each shown policy and then click on the “Show” button to get to the server list and add “TERMSRV/* to the server. In my case it’s ‘*’ which indicates that cached credentials will be allowed to all servers.
  ```

* 直接用注册表脚本

  ```reg
  Windows Registry Editor Version 5.00
  
  [HKEY_LOCAL_MACHINE\SOFTWARE\Policies\Microsoft\Windows\CredentialsDelegation] "AllowDefCredentialsWhenNTLMOnly"=dword:00000001 "ConcatenateDefaults_AllowDefNTLMOnly"=dword:00000001 "AllowDefaultCredentials"=dword:00000001 "ConcatenateDefaults_AllowDefault"=dword:00000001 "AllowSavedCredentialsWhenNTLMOnly"=dword:00000001 "ConcatenateDefaults_AllowSavedNTLMOnly"=dword:00000001 "AllowSavedCredentials"=dword:00000001 "ConcatenateDefaults_AllowSaved"=dword:00000001
  
  [HKEY_LOCAL_MACHINE\SOFTWARE\Policies\Microsoft\Windows\CredentialsDelegation\AllowDefaultCredentials] "1"="TERMSRV/*"
  
  [HKEY_LOCAL_MACHINE\SOFTWARE\Policies\Microsoft\Windows\CredentialsDelegation\AllowDefCredentialsWhenNTLMOnly] "1"="TERMSRV/*"
  
  [HKEY_LOCAL_MACHINE\SOFTWARE\Policies\Microsoft\Windows\CredentialsDelegation\AllowSavedCredentials] "1"="TERMSRV/*"
  
  [HKEY_LOCAL_MACHINE\SOFTWARE\Policies\Microsoft\Windows\CredentialsDelegation\AllowSavedCredentialsWhenNTLMOnly] "1"="TERMSRV/*"
  
  [HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Policies\Microsoft\Windows\CredentialsDelegation] "AllowDefCredentialsWhenNTLMOnly"=dword:00000001 "ConcatenateDefaults_AllowDefNTLMOnly"=dword:00000001 "AllowDefaultCredentials"=dword:00000001 "ConcatenateDefaults_AllowDefault"=dword:00000001 "AllowSavedCredentialsWhenNTLMOnly"=dword:00000001 "ConcatenateDefaults_AllowSavedNTLMOnly"=dword:00000001 "AllowSavedCredentials"=dword:00000001 "ConcatenateDefaults_AllowSaved"=dword:00000001
  
  [HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Policies\Microsoft\Windows\CredentialsDelegation\AllowDefaultCredentials] "1"="TERMSRV/*"
  
  [HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Policies\Microsoft\Windows\CredentialsDelegation\AllowDefCredentialsWhenNTLMOnly] "1"="TERMSRV/*"
  
  [HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Policies\Microsoft\Windows\CredentialsDelegation\AllowSavedCredentials] "1"="TERMSRV/*"
  
  [HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Policies\Microsoft\Windows\CredentialsDelegation\AllowSavedCredentialsWhenNTLMOnly] "1"="TERMSRV/*"
  ```

  

### 部分快捷键无效，如 `ctrl-alt-arrow` 

> windows的 设置-易用-键盘 里可以打开屏幕键盘，可以测试按键
>
> microsoft store的这个rdp版本相比原来那个有点卡（延迟），不知道是实现的不好还是默认参数差异



ref： https://superuser.com/questions/327866/remote-desktop-sending-ctrl-alt-left-arrow-ctrl-alt-right-arrow-to-the-remote-p



The hotkeys Ctrl+Alt+Left Arrow and Ctrl+Alt+Right Arrow are eaten up by the Remote Desktop Client. Their only effect is to switch you to back to the host computer.

It looks like this was some intended feature that was never fully programmed and completed, but there is no way to turn it off. These hotkeys are not even listed by Microsoft in its official documentation at [Remote Desktop Services Shortcut Keys](https://docs.microsoft.com/en-us/windows/win32/termserv/terminal-services-shortcut-keys?redirectedfrom=MSDN).

**Solution 1 : Use the Microsoft Store version**

Another version of RDP can be found in the Microsoft Store at [Microsoft Remote Desktop](https://www.microsoft.com/en-us/p/microsoft-remote-desktop/9wzdncrfj3ps?activetab=pivot:overviewtab#).

This version does not have this semi-implemented feature, so it lets through these hotkeys without a problem. This was verified on Windows 10 version 1903.

**Solution 2 : Translate the hotkeys on both client and server**

This solution will use [AutoHotkey](https://www.autohotkey.com/) installed on both client and server, to:

- On the client, translate the above hotkeys to others that are not intercepted by RDP
- On the server, translate these keys back to the above hotkeys.





# network



## dns



### dns server

#### dns server selection/order

https://serverfault.com/questions/84291/how-does-windows-decide-which-dns-server-to-use-when-resolving-names

* 老版本里是按照网卡顺序
* 新版本（10）是按照metric排序





## route



### 工具

http://www.nirsoft.net/utils/network_route_view.html



### static route



#### 静态路由优先级metric问题



现象： 

* 添加的静态路由的metric低于dhcp gateway
* 手动设置静态路由metric更低，之后一小会儿就自动调整了，大概率比dhcp gateway低



原因： windows默认在网络接口上设置为`自动跃点`，so...



解法：

* 网卡设置

* 高级

* 取消自动跃点，设置好接口跃点

* 设置网关的跃点，如果没有dhcp gateway，则手动加进去。 保证静态路由比dhcp gw的低

* 目前看是有效的

  但后来再看，dhcp gw这一条目又没了。 不过 `route print`看到metric还是设置的值





