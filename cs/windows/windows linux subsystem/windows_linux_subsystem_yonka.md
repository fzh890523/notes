



## 互操作

参考： https://docs.microsoft.com/en-us/windows/wsl/interop



### windows调linux： `wsl {cmd}`/`wsl -- {cmd}` 



如： `wsl ls -l`

而`wsl {wsl_args} -- {cmd}`是更准确的方式，可以清晰的分割wsl自己的参数和要执行的命令



### 在指定目录启动linux shell



1. 在目录下空白处，shift+右键
2. 点击选择`在此处打开linux shell`



## 存储



### distro位置



#### 迁移

参考 《docker_windows_yonka.md》 中docker镜像存储位置迁移的操作。



### 磁盘/分区挂载



> DrvFs now allows you to mount external drives like USB sticks, CDs, and DVDs. These devices must use a Windows file system like NTFS, [ReFS](https://www.howtogeek.com/311840/what-is-refs-the-resilient-file-system-on-windows/), or [FAT](https://www.howtogeek.com/235596/whats-the-difference-between-fat32-exfat-and-ntfs/). You still can’t mount devices formatted with a [Linux file system](https://www.howtogeek.com/howto/33552/htg-explains-which-linux-file-system-should-you-choose/) like ext3 or ext4.



```sh
# mount
$ sudo mkdir /mnt/d
$ sudo mount -t drvfs D: /mnt/d

# unmount
$ sudo umount /mnt/d

# mount network drive/share
$ sudo mount -t drvfs '\\server\share' /mnt/share
```



要自动mount的话：

```
# fstab里加入类似如下
Y: /mnt/y drvfs defaults 0 0

# 重启 or sudo mount -a
```



还可以结合： `ln -s /mnt/y /c` 来更方便的访问。



## 网络

也可参考 《windows命令行环境》 那篇。

wsl2之后用的是端口映射，不太一样了。



## wsl2

>Both your own WSL 2 distro and docker-desktop run on the same utility VM. They share the same Kernel, VFS cache etc. They just run in separate namespaces so that they have the illusion of running totally independently. Docker Desktop leverages that to handle bind mounts from a WSL 2 distro without involving any remote file sharing system. This means that when you mount your project files in a container (with `docker run -v ~/my-project:/sources <...>`), docker will propagate inotify events and share the same cache as your own distro to avoid reading file content from disk repeatedly.
>
>A little warning though: if you mount files that live in the Windows file system (such as with `docker run -v /mnt/c/Users/Simon/windows-project:/sources <...>`), you won’t get those performance benefits, as /mnt/c is actually a mountpoint exposing Windows files through a Plan9 file share. 
>
>from: https://www.docker.com/blog/docker-desktop-wsl-2-best-practices/



### 安装、启用



ref： https://docs.microsoft.com/zh-cn/windows/wsl/install-win10#update-to-wsl-2



```sh
# as admin
dism.exe /online /enable-feature /featurename:VirtualMachinePlatform /all /norestart

wsl --set-default-version 2

wsl --list --verbose
wsl --set-version <distribution name> <versionNumber>
# 如： wsl --set-version Ubuntu 2
```



```sh
PS C:\WINDOWS\system32> dism.exe /online /enable-feature /featurename:VirtualMachinePlatform /all /norestart

部署映像服务和管理工具
版本: 10.0.19041.329

映像版本: 10.0.19041.508

启用一个或多个功能
[==========================100.0%==========================]
操作成功完成。
PS C:\WINDOWS\system32> wsl --set-default-version 2
WSL 2 需要更新其内核组件。有关信息，请访问 https://aka.ms/wsl2kernel
有关与 WSL 2 的主要区别的信息，请访问 https://aka.ms/wsl2
PS C:\WINDOWS\system32> wsl --list --verbose
  NAME      STATE           VERSION
* Ubuntu    Running         1
PS C:\WINDOWS\system32> wsl --set-version Ubuntu 2
正在进行转换，这可能需要几分钟时间...
有关与 WSL 2 的主要区别的信息，请访问 https://aka.ms/wsl2
WSL 2 需要更新其内核组件。有关信息，请访问 https://aka.ms/wsl2kernel
```



这时，使用docker时可能提示：

```
WSL 2 installation is incomplete.

Ths WSL 2 Linux kernel is now installed using a separate MSI update package ...
```

正文里告知需要去 https://docs.microsoft.com/zh-cn/windows/wsl/wsl2-kernel 下载 linux kernel。



## config

* global级别
* distro级别



详见： [windows_wls_config_yonka.md](windows_wls_config_yonka.md)



## vm管理

好像wsl1是可以在hypervisor里管理虚拟机； wsl2只用了h的虚拟化底层功能但没映射为h vm，所以无法从h中看到...



管理手段... 只有wsl命令（和相关配置）了...





## 版本行为差异



### login shell问题

`wsl -- zsh -c "xxx"` 这样执行，老版本可能是non-login shell，新版本则可能是login-shell。

对于前者，如果希望是login shell，则： `wsl -- zsh --login -c "xxx"`







