

## 设置

```cmd
PS C:\Users\xx> ubuntu /?                                                                                             Launches or configures a Linux distribution.

Usage:
    <no args>
        Launches the user's default shell in the user's home directory.

    install [--root]
        Install the distribuiton and do not launch the shell when complete.
          --root
              Do not create a user account and leave the default user set to root.

    run <command line>
        Run the provided command line in the current working directory. If no
        command line is provided, the default shell is launched.

    config [setting [value]]
        Configure settings for this distribution.
        Settings:
          --default-user <username>
              Sets the default user to <username>. This must be an existing user.

    help
        Print usage information.
```

### 默认用户

```sh
ubuntu.exe config --default-user ${user}
```



## 图形界面



```sh
# sudo apt-get update
sudo apt-get install ubuntu-desktop
```



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



















