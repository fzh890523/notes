



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



## wsl2



ref： https://docs.microsoft.com/zh-cn/windows/wsl/install-win10#update-to-wsl-2





