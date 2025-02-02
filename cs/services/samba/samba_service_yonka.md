

# 概念



https://wiki.samba.org/index.php/Samba3/SMB2

https://en.wikipedia.org/wiki/Samba_(software)

## discovery



https://devanswers.co/discover-ubuntu-machines-samba-shares-windows-10-network/



SMBv1是通过NetBIOS来做服务发现，windows也支持，但是新版本开始默认关闭了。



All these services and protocols are frequently incorrectly referred to as just NetBIOS or SMB. The NBT (NetBIOS over TCP/IP) and WINS protocols, and their underlying SMB version 1 protocol, are deprecated on Windows. Since [Windows Vista](https://en.wikipedia.org/wiki/Windows_Vista) the [WS-Discovery](https://en.wikipedia.org/wiki/WS-Discovery) protocol has been included along with SMB2 and its successors, which supersede these. (WS-Discovery is  implemented on [Unix-like](https://en.wikipedia.org/wiki/Unix-like) platforms by third party [daemons](https://en.wikipedia.org/wiki/Daemon_(computing)) which allow Samba shares to be discovered when the deprecated protocols are disabled).





### SMB 1.0 (SMBv1)

Several guides and YouTube videos I’ve researched suggest manually enabling SMB 1.0 in Windows 10 in order to make other Ubuntu machines visible in Windows File Explorer. While this may work on older editions of Windows 10, it is not advised due to the security implications.

From Microsoft:

> “Hi folks, Ned here again and today’s topic is short and sweet: Stop using SMB1. Stop using SMB1 . STOP USING SMB1!” [source](https://techcommunity.microsoft.com/t5/storage-at-microsoft/stop-using-smb1/ba-p/425858)

With Windows 10 version 1511, support for SMBv1 and thus NetBIOS device discovery was disabled by default. Depending on the actual edition, later versions of Windows starting from version 1709 (“Fall Creators Update”) do not allow the installation of the SMBv1 client anymore.

This causes hosts running Samba not to be listed in the Explorer’s “Network (Neighborhood)” views. While there is no connectivity problem and Samba will still run fine, users might want to have their Samba hosts to be listed by Windows automatically.

### mDNS

Ubuntu 18.04/20.04 automatically register your samba server to the rest of the network using mDNS (Avahi). All other Linux machines as well as macOS can discover this registration automatically.

The reason Windows 10 cannot discover Ubuntu network shares is because although it can use mDNS it does not know how to scan the network for them automatically.

### WDS

Web Services for Devices (WSD) is a Microsoft API to enable programming connections to web service enabled devices, such as printers, scanners and file shares. It serves as a replacement for older Windows networking functions such as NetBIOS.

With the addition of a WSD service in Ubuntu, Windows 10 will discover the Linux Samba server using its native WSD protocol.



#### 安装WDS（在ubuntu）

见 `notes\cs\services\service discovery\WS-Discovery_yonka.md`





### Why Doesn’t Ubuntu Support WDS Natively?

This issue has been raised in the Samba bug tracker since 2015. So it may happen that this feature gets integrated into Samba at some time in the future. But for now, we have to manually install a patch ourselves.



### win10 无法“发现” linux samba server的问题



似乎linux/mac和windows在samba v1之后的版本里，对于“服务发现”的支持有了分歧/分叉，前者采用mDNS而后者采用WDS，于是。。。



但问题是，这个难道不是samba协议内的事情吗？ 或者说这个服务发现是独立于samba协议之外的？





## 用户/权限体系

低版本samba的用户体系是基于linux 用户体系的，高版本samba好像支持独立的用户体系了。



### 文件访问权限



#### 二级权限体系

> yonka理解：
>
> 应该是二级权限体系：
>
> 1. samba层
> 2. os层
>
> 需要两层都有权限才能进行对应的操作，常见的问题是： samba层给了写权限但os层没有，于是... 这个情况常发生于没有设置force user/group时“跨用户”读写操作，如
>
> > 用户a上传文件`/a/a1`，这时`/a`和`/a/a1`会是`userA/groupA` 并且按照a/os的mask来确定文件权限（比如644），那么用户b想上传`/a/b1`就会因为对`/a`目录没有写权限而gg
>
> 应对方法：
>
> > 1. 设置一个专门的group，如 share
> >
> > 2. samba用户都加到这个group，如 `add usera share; add userrb share`
> >
> > 3. 用force group的方式，让新文件都是这个group的
> >
> >    如果samba用户都。。的话，这里也不需要了。
> >
> >    这个可以应对samba用户不在这个group的情况
> >
> > 4. 老文件要刷一次group属性
>
> 其他思路：
>
> * 用特定的user，如 shareuser。 然后force user
>
>   这样的话就丢失了“谁上传的文件”这个信息了



#### 读写权限



* 写： `write list`

* 读： 默认允许，除非 `valid users` 做了设置（白名单）

  至于`user`选项（对应webmin的`Possible users`）暂时不知道干吗用的

* browser： `browserable = no`来关闭，默认打开

  这个如果要做到用户级别的配置的话，比较tricky

  ```
  # share级别
  [sharename]
  browseable = no
  include = /etc/samba/sharename.%U.conf
  
  # 然后在 sharename.${username}.conf 中
  [sharename]
  browseable = yes
  
  
  # 解释
     %U
         session username (the username that the client wanted, not
         necessarily the same as the one they got).
  ```

  

* 可用： `available = no`来关闭，默认可用



## 不同版本

* v1
  * portL 139
* v2+
  * port: 445



# 实现



Samba services are implemented as two [daemons](https://en.wikipedia.org/wiki/Daemon_(computing)):

- smbd, which provides the file and printer sharing services, and
- nmbd, which provides the NetBIOS-to-IP-address name service. NetBIOS over TCP/IP requires some method for mapping NetBIOS computer names to the IP addresses of a TCP/IP network.



## client

### smbclient （linux）

```sh
Usage: smbclient [-?EgqBVNkPeC] [-?|--help] [--usage] [-R|--name-resolve=NAME-RESOLVE-ORDER] [-M|--message=HOST]
        [-I|--ip-address=IP] [-E|--stderr] [-L|--list=HOST] [-m|--max-protocol=LEVEL] [-T|--tar=<c|x>IXFqgbNan]
        [-D|--directory=DIR] [-c|--command=STRING] [-b|--send-buffer=BYTES] [-t|--timeout=SECONDS] [-p|--port=PORT]
        [-g|--grepable] [-q|--quiet] [-B|--browse] [-d|--debuglevel=DEBUGLEVEL] [-s|--configfile=CONFIGFILE]
        [-l|--log-basename=LOGFILEBASE] [-V|--version] [--option=name=value] [-O|--socket-options=SOCKETOPTIONS]
        [-n|--netbiosname=NETBIOSNAME] [-W|--workgroup=WORKGROUP] [-i|--scope=SCOPE] [-U|--user=USERNAME]
        [-N|--no-pass] [-k|--kerberos] [-A|--authentication-file=FILE] [-S|--signing=on|off|required]
        [-P|--machine-pass] [-e|--encrypt] [-C|--use-ccache] [--pw-nt-hash] service <password>
```







```sh
# 登录，shell操作
smbclient \\\\127.0.0.1\\sharename  # \\ 转义了一份

# print info
smbclient -L　\\\\127.0.0.1\\sharename

# -U xx 指定用户（否则为当前登录用户）
# -W xx 指定当前workgroup。 企业用户可能加入了域/wg，这时需要指定，否则会access denied
```





# topics



## samba user 和 unix user打通



* user

  可以手动（命令行）或者自动同步

* passwd

  用`libpam-smbpass`可以使得samba利用unix passwd来鉴权，不过问题在于现代（比较新）的samba（client）都不会明文传输密码了，比如hash一次之后（而且每次hash盐也许还不同）拿到的“密码”，没办法通过pam来鉴权。。。

  ref： https://web.archive.org/web/20130530031518/http://jaka.kubje.org/infodump/2007-05-14-unix-samba-password-sync-on-debian-etch

  

  虽然可以配置为传输明文密码，但毕竟不安全。

  大约就是放弃的一起，可以同步user，然后手动再设置一下smb密码（smbpasswd命令）



## gui管理工具



* webmin的samba模块



## mount samba



```sh
smbmount \\\\172.16.1.3\\c$ /mnt/thumb -o username=jwhittal
                
# Smbumount is the command used to unmount the Windows share. In fact, smbmount and smbumount are just synonyms for the mount and umount commands in Linux. An alternative to the smbmount command above is:

mount -t smbfs -o username=jwhittal \\\\172.16.1.3\\c$ /mnt/thumb
# 有密码的情况下： -o username=xxx,password=yyy
# 新的linux内核，可能是cifs
# 对于windows linux subsystem不可用。详见下面。  还是只能在windows里map为盘符然后用 mount -t drvfs
```



```
In WSL (I'm using Ubuntu) it looks like that when you install the cifs-utils it doesn't create the module file that cifs needs when mounting. Type: "modinfo cifs" and you will see. Anyway, the work-around is to map a drive letter in Windows and then mount to that, as mentioned above. Thanks gabuzo.

Maybe its that cifs-utils is looking in the wrong place for the module file. Or MS intentionally disabled it. They don't want WSL to be too useful.
```



### on-boot/fstab



```
# 可能要先： sudo apt-get install cifs-utils
# /etc/fstab 里
//server/share /pathto/mountpoint cifs credentials=/home/username/.smbcredentials,uid=shareuser,gid=sharegroup 0 0
```



smbcredentials文件（这个文件`chmod 0600`）里：

```
username=shareuser
password=sharepassword
domain=domain_or_workgroupname
```





## access windows share



1. windows上开用户

   用已有用户也行，如果想用单独用户的话，暂时没看到单独创建samba user的地方，可以单独开个类似`smb-xxx`的windows user

2. 目录设置共享，可以搜索需要允许访问的用户，加入访问的权限（只读、读写等）
3. 在高级共享选项里可以设置共享名，默认为目录名



# 可能的问题



## 权限问题

### 因为用户无目录权限导致无法写入

几个层面的权限：

1. samba层的读写权限
2. 系统层的读写权限



```
rsync: mkstemp "/xxx/sw/.BaiduNetdisk_mac_2.2.2.dmg.5zT7ge" failed: Operation not permitted (1)

rsync error: some files/attrs were not transferred (see previous errors) (code 23) at main.c(1207) [sender=3.1.3]

# 似乎是因为mac的extended attributes引起，如 -rw-r--r--@ 这样的@结尾的permission
# 带上--no-perms后解决
# 这里有几个关系: -a 包含-p（带perms）； -A 则隐式-p，所以如果有这两个，则需要--no-perms。 如果都没有则不需要。 如： --avv --no-perms 或者 --vvAX --no-perms 或者 --vv

rsync: failed to set times on "/xxx/sw/.BaiduNetdisk_mac_2.2.2.dmg.g3XZnv": Operation not permitted (1)
# 这个还是会有，但不影响


```



