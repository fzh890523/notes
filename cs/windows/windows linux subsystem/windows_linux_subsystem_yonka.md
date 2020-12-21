



## 互操作

参考： https://docs.microsoft.com/en-us/windows/wsl/interop



### windows调linux： `wsl {cmd}` 



如： `wsl ls -l`



### 在指定目录启动linux shell



1. 在目录下空白处，shift+右键
2. 点击选择`在此处打开linux shell`



## 存储



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

