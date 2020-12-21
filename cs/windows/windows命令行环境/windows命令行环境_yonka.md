

## process 进程管理



### 环境变量



* 命令行设置

  *  `setx <key> <value>` 写回注册表但不是当前生效

    > 大约等于 echo k=v >> ~/.bashrc 吧

    * `/S <system>`
    * `/U <user>` 默认为当前用户，也即加到当前用户的env
      * `/M` 设置为系统级别（而不是用户级别）

  * `set <key>=<value>` 当前生效（类似`export`吧）

* 查看

  * `set <key>`
  * `echo %key%`

* 删除
  * `set <key>=` 当前生效
  * `setx <key> ""` 保存（但不当前生效）



## 软件



### shell



现在主要用powershell（还不太熟悉

除了shell差异以外，命令工具差异比较大这个比较蛋疼。

可以用



### 终端软件



* cmder
* mobaxterm
* conemu









## 常用命令



### 文本



#### 文本打印： cat -> type



#### 文本处理



#### 文本编辑



### 进程管理



* tasklist

  ```sh
  C:\Documents and Settings\Administrator>tasklist /?
  
  TASKLIST [/S system [/U username [/P [password]]]]
           [/M [module] | /SVC | /V] [/FI filter] [/FO format] [/NH]
  
  Description:
      This command line tool displays a list of application(s) and
      associated task(s)/process(es) currently running on either a local or
      remote system.
  
  Parameter List:
     /S     system           Specifies the remote system to connect to.
  
     /U     [domain\]user    Specifies the user context under which
                             the command should execute.
  
     /P     [password]       Specifies the password for the given
                             user context. Prompts for input if omitted.
  
     /M     [module]         Lists all tasks that have DLL modules loaded
                             in them that match the given pattern name.
                             If the module name is not specified,
                             displays all modules loaded by each task.
  
     /SVC                    Displays services in each process.
  
     /V                      Specifies that the verbose information
                             is to be displayed.
  
     /FI    filter           Displays a set of tasks that match a
                             given criteria specified by the filter.
  
     /FO    format           Specifies the output format.
                             Valid values: "TABLE", "LIST", "CSV".
  
     /NH                     Specifies that the "Column Header" should
                             not be displayed in the output.
                             Valid only for "TABLE" and "CSV" formats.
  
     /?                      Displays this help/usage.
  
  Filters:
      Filter Name     Valid Operators           Valid Value(s)
      -----------     ---------------           --------------
      STATUS          eq, ne                    RUNNING | NOT RESPONDING
      IMAGENAME       eq, ne                    Image name
      PID             eq, ne, gt, lt, ge, le    PID value
      SESSION         eq, ne, gt, lt, ge, le    Session number
      SESSIONNAME     eq, ne                    Session name
      CPUTIME         eq, ne, gt, lt, ge, le    CPU time in the format
                                                of hh:mm:ss.
                                                hh - hours,
                                                mm - minutes, ss - seconds
      MEMUSAGE        eq, ne, gt, lt, ge, le    Memory usage in KB
      USERNAME        eq, ne                    User name in [domain\]user
                                                format
      SERVICES        eq, ne                    Service name
      WINDOWTITLE     eq, ne                    Window title
      MODULES         eq, ne                    DLL name
  
  Examples:
      TASKLIST
      TASKLIST /M
      TASKLIST /V
      TASKLIST /SVC
      TASKLIST /M wbem*
      TASKLIST /S system /FO LIST
      TASKLIST /S system /U domain\username /FO CSV /NH
      TASKLIST /S system /U username /P password /FO TABLE /NH
      TASKLIST /FI "USERNAME ne NT AUTHORITY\SYSTEM" /FI "STATUS eq running"
  ```

  



## network



### arp



* 老版本： arp命令

  * 新增arp条目： `arp -s 192.168.1.5 xx-xx-xx-xx-xx-xx`
  * 删除。。。： `arp -d ...`
  * 查看arp条目： `arp -a`

* 新版本（win7后）： netsh命令。 不能再用arp命令来改动arp条目，不过查看还是可以用

  * 新增arp条目： `netsh -c "i i" add neighbors ${idx} ${ip} ${mac}`

  * 查看接口： `netsh i i show in` （上面命令的idx从这里获取）

    



### ssh



win10 1089开始支持openssh，不过可能需要手动打开。

参考： https://www.cnblogs.com/sparkdev/p/10166061.html
https://docs.microsoft.com/en-us/windows-server/administration/openssh/openssh_install_firstuse

主要包括：

* 安装openssh server（默认带了包，需要在可选功能里安装）

  `Apps > Apps and Features > Manage Optional Features` 注意不是windows功能那个。

* 启动服务

* `~/.ssh`下： `ssh-keygen`生成公私钥、`authorized_keys`加入信任公钥

* `C:\ProgramData\ssh\sshd_config`中注释掉（应该是最末），然后重启服务：

  ```sh
  ## 如下注释
  
  # Match Group administrators
  #        AuthorizedKeysFile __PROGRAMDATA__/ssh/administrators_authorized_keys
  ```

  >  这样才能使用`~/.ssh`下的

#### 修改端口号

配置还是上面那个 `sshd_config` ，比如`port 2222`，改完重启服务即可。
不过原来的防火墙只放行了22，需要额外放行2222。

> 加防火墙规则时被defender干了，然后又是惯例的窗口卡死，算了。。。



可以直接把sshd binary加进去： `C:\Windows\System32\OpenSSH\sshd.exe`

z

#### linux子系统ubuntu openssh

参考： https://superuser.com/questions/1111591/how-can-i-ssh-into-bash-on-ubuntu-on-windows-10

考虑到跟host windows openssh server的端口冲突，需要改下端口。
我这边选择把windows的端口改了。



> 不过WSL好像还没解决服务启动的问题，需要手动启动服务。 也即意味着每次都要。。。
>
> 如： sudo service ssh start



有些版本里可能没有生成初始数据，这里启动会报错：

```s
 * Starting OpenBSD Secure Shell server sshd                                                                            sshd: no hostkeys available -- exiting.
```



需要：

```sh
sudo ssh-keygen -A
# 生成了：/etc/ssh/ssh_host_*
# 然后再start
```



默认好像没开密码登录

authorized_keys文件注意权限需要为600



#### ssh key pair生成



好像cmder等终端shell中ssh-keygen能执行成功，但对应目录没文件生成。。。 

```
                                     λ ssh-keygen.exe
Generating public/private rsa key pair.
Enter file in which to save the key (C:\Users\xxx/.ssh/id_rsa): yes
Enter passphrase (empty for no passphrase):
Enter same passphrase again:
Your identification has been saved in yes.
Your public key has been saved in yes.pub.
The key fingerprint is:
SHA256:xxx xxx@xxx
The key's randomart image is:
+---[RSA 2048]----+
xxx
+----[SHA256]-----+

```



换在powershell里执行就可以了。。。

























