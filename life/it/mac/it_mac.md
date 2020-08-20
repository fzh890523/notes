

# 了解



## 机型



### mbp



ref： [识别 MacBook Pro 机型](https://support.apple.com/zh-cn/HT201300)

* `MacBookPro11,4` 

  **MacBook Pro（视网膜显示屏，15 英寸，2015 年中）**
  机型标识符：MacBookPro11,4
  部件号：MJLQ2xx/A
  技术规格：[MacBook Pro（视网膜显示屏，15 英寸，2015 年中）](https://support.apple.com/kb/SP719?locale=zh_CN)



### imac



ref: [识别您的 iMac 机型](https://support.apple.com/zh-cn/HT201634)

* `iMac14,2`

  **iMac（27 英寸，2013 年末）**
  机型标识符：iMac14,2
  部件号：ME086xx/A、ME088xx/A
  技术规格：[iMac（27 英寸，2013 年末）](https://support.apple.com/kb/SP688?locale=zh_CN)

* `14,3` 文里没有，囧

* `iMac14,4` 

  **iMac（21.5 英寸，2014 年中）**
  机型标识符：iMac14,4
  部件号：MF883xx/A、MG022xx/A
  技术规格：[iMac（21.5 英寸，2014 年中）](https://support.apple.com/kb/SP701?locale=zh_CN)



## 保修



https://checkcoverage.apple.com/cn/zh/

输入序列号(从关于本机那获取)






# network



## wireless



### 问题



#### 抖动过大

**背景**

* 14版本imac
* 连2.4/5g wifi ssid，卡顿明显，开始怀疑是vnc的锅，后来发现ssh也是
* 测试发现： ping 90ms、抖动57ms； 带宽正常 下行200多、上行五六十，跟mbp上差不多
* 切到单2.4g，49ms、21；76MB、28 
* 切到单5g，ping 38、抖动3.27； 下载218MB、上行38

**结论**

mac（尤其是老版本）对于双模ssid的接入是支持但有问题的。 坑。



# device



## disk



### mount



* 可以用linux风格
  ```sh
  sudo mount -t apfs /dev/disk5s1 ~/xx
  vim /etc/fstab
  ```



#### mount local directory



> macos上mount好像不支持bind，可以：
>
> Ref: <https://apple.stackexchange.com/questions/197029/how-do-you-mount-bind-a-local-directory>
>
> - 使用第三方FS支持，比如FUSE/bindfs
>
> ```sh
> # install osxfuse first, and then:
> brew install homebrew/fuse/bindfs
> ```
>
> - use NFS mound to do this
>
> ```sh
> sudo tee -a /etc/exports <<< "/ -alldirs -mapall=$USER localhost"
> sudo launchctl start com.apple.rpcbind
> sudo nfsd start
> sudo mount localhost:/path/to/target ./mnt
> # sudo umount ./mnt
> ```
>
> - use buildin file-sharing(afp/smb)
>
>   ```sh
>   # share target directory
>   # As finder refuse to connect to a local "network-location", we mount it manually
>   mount -t afp afp://${user}:${password}@127.0.0.1/${shareName} ${mntPoint}
>   ```
>
>   





### **fstab**

* mac上不建议使用`/etc/fstab`，而是建议使用`diskutil`
* 但mac还是支持fstab的功能，也许文件不存在，可以手动创建一下
  
  > https://superuser.com/questions/336455/mac-lion-fstab-is-deprecated-so-what-replaces-it-to-prevent-a-partition-from-m 提到不是不建议使用fstab，而是不建议直接编辑它 --- 应该通过`vifs` 命令来编辑。



**uuid**



### ntfs



#### ntfs mount rw



* `mount`命令有点半废

  对于`mount -t ntfs uuid=xxx ~/mount/hdd-test-entertainment`会报错`No such file or directory`；

  对于fstab中`UUID=xxx none ntfs rw,auto,nobrowse`然后`mount -a`也一样

* fstab + `diskutil mount`

  `diskutil mount /dev/disk6s2` 会读fstab，但如果没有rw的话ok，有rw的话会报错：

  ```
  Volume on disk6s2 failed to mount
  If the volume is damaged, try the "readOnly" option
  ```

  这一点与搜到的信息不一样，网上这样ok

  > 其实是文件系统dirty。。。修复一下就好了，见下文
  >
  > 后来又不行了，噗。。。弃疗

* 用三方方案

  Ref： 

  * <https://www.howtogeek.com/236055/how-to-write-to-ntfs-drives-on-a-mac/>
  * <https://www.makeuseof.com/tag/write-ntfs-drives-el-capitan-free/>

  

  * 收费： Paragon NTFS

  * 免费： [FUSE for macOS](https://github.com/osxfuse/osxfuse/releases) + NTFS-3G

    `wget https://github.com/osxfuse/osxfuse/releases/download/osxfuse-3.9.0/osxfuse-3.9.0.dmg`

    选装打钩 **MacFUSE Compatibility Layer**

    `brew install ntfs-3g`

    ```sh
    sudo /usr/local/bin/ntfs-3g /dev/disk6s2 /Volumes/xxx -olocal -oallow_other
    
    The disk contains an unclean file system (0, 0).
    The file system wasn't safely closed on Windows. Fixing.
    
    # ok了
    # <del>并且之后用 fstab + diskutil mount 也ok了</del>
    ```

    



### ext





# smb


## 权限

slbj的mac，不希望login user能rw访问sharing dir所以删掉了这一行授权。。
于是。。 在本机login user无法访问这个dir了。 简直智障。。。

  

## 用户

可以在用户里创建sharing only的用户，来避免login用户信息外泄。

## win兼容性

部分client以”windows“方式访问，需要勾选file sharing - options - windows file sharing。
> 有提示说启用该模式需要使得用户密码以更不安全方式存储
> 所以也只能是专用用户了

## 问题

**用户名密码报错**

需要重置一下密码（同样的密码）。

我这边复现是：
1. 在sharing里添加用户时用contacts里选择、设置密码，然后会自动创建sharing only的用户
2. 连接时密码报错
3. 在用户里重置密码为同样内容，ok

估计是实现bug。



# sw

## item2







见 《it_mac_sw.md》














