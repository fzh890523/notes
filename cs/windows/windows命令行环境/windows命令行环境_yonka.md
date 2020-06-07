

## shell



现在主要用powershell（还不太熟悉

除了shell差异以外，命令工具差异比较大这个比较蛋疼。

可以用



## 常用命令



### 文本



#### 文本打印： cat -> type



#### 文本处理



#### 文本编辑



## network







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

