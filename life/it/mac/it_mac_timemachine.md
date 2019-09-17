
## 支持的


## over APFS

* 好像只能是partition，不能是volume
* 所以，只有一个volume的APFS container是可以选，多volume的APFS container就不行，囧

## over network drive/NAS

**背景**

* 似乎老版本不支持backup-to/restore-from network location/drive
  好像是因为只支持mac的FS。网上迂回的方案是：
  1. create a image file（use mac filesystem）
  2. transfer to network drive
  3. mount network drive，open the image file（and mount it）

  ```sh
  defaults write com.apple.systempreferences TMShowUnsupportedNetworkVolumes 1
  
  hdiutil create -size 100g -fs HFS+J -volname "Time Machine" NAME_XXXXXXXXXXXX.sparsebundle  # 文里推荐用mac地址来。。。
  
  rsync -aE NAME_XXXXXXXXXXXX.sparsebundle /Volumes/DRIVE/.
  ```

* 新版本里timemachine支持更多FS了，所以可以直接使用network drive
  但挂载volume的话好像不行，挂载folder可以
* backup-to/resotore-from network drive这对正逆向操作OK
  **但把本机的timemachine backup以network drive的方式提供给其他机器**这个不性，好像是因为timemachine写local drive的方式不同或者是写入权限不同。
  尝试把本地directory mount为network drive，但timemachine竟然能识别，气！
  > 可以用 `sudo tmutil setdestination ...` 来绕过，不过无法在UI启动。。
  > 使用命令行方式`sudo tmutil startbackup` 也不行。
* timemachine新版本应该是不支持`over smb`了
  应该是会检查分区FS，什么`smbfs`、`osxfuse`什么的都不支持。。。
  基于fuse的bindfs也可以死心了
  [此文(本地有备份)](https://kirb.me/2018/03/24/using-samba-as-a-time-machine-network-server.html) 里提到用加载一些module的samba，但感觉还是比较虚，问题可能不是module能解决的
* 小结问题纠结点
  * 本地backup没办法通过nas分享出去 -> 只能考虑本地的backup也走nas
  * 不能连接本机的nas作为timemachine destination
  * 不能连接samba nas作为。。。

  最后： 本机开虚拟器起APF服务。。。，参考 [afp-and-bonjour-under-linux](https://mycyberuniverse.com/linux/afp-and-bonjour-under-linux.html)，简要摘录步骤如下：
  ```sh
  sudo apt-get install netatalk
  sudo nano /etc/netatalk/AppleVolumes.default
  # 在 `~/                              "Home Directory"` 后，加入行：
  # ${timeMachineDir} "${shareName}" allow:${user} options:tm
  ## 这个user好像是系统user。 暂时还不清楚APF能不能用单独的用户系统。
  sudo /etc/init.d/netatalk restart

  sudo apt-get install avahi-daemon
  sudo nano /etc/avahi/services/afpd.service
  # 加入下面xml中内容
  sudo /etc/init.d/avahi-daemon restart
  ```
  ```xml
  <?xml version="1.0" standalone='no'?><!--*-nxml-*-->
  <!DOCTYPE service-group SYSTEM "avahi-service.dtd">
  <service-group>
    <name replace-wildcards="yes">%h</name>
  
    <service>
      <type>_afpovertcp._tcp</type>
      <port>548</port>
    </service>
  
    <service>
      <type>_device-info._tcp</type>
      <port>0</port>
      <txt-record>model=TimeCapsule</txt-record>
    </service>
  
  </service-group>  
  ```

## restore

### restore system

1. 启动时`cmd+r`进入恢复模式
2. 通过`utilities-terminal`手动挂载
  ```sh
  mount -t afp afp://adminname:password@ServerIPAddress/ShareName /Volumes/TimeMachine
  # 下面的命令好像是： “打开” bundle文件。 执行了以后，后面才能搜到这个backup。。。
  hdid /Volumes/TimeMachine/NameOfYourSparseBundle
  ```
3. 然后用恢复模式的`restore from timemachine`来恢复
  其中可以选择前面挂载的网络分区

```sh
mount -t afp afp://${username}:${password}@127.0.0.1/wd-8t-2-timemachine/SharedTimeMachine /Volumes/SharedTimeMachine
```

#### 问题： 提示“This disk does not have enough space to restore your system”

SB苹果，各种bug。

https://apple.stackexchange.com/questions/339324/this-disk-does-not-have-enough-space-to-restore-your-system

```
For updating a date backup file size:

Use Disk Inventory X for finding the size of the backup date folder "/Volumes/Drive/Backups.backupdb/xxx\ MacBook\ Pro/2014-11-19-174311/Machintosh HD"

Open Terminal go to the folder of the backup date folder "cd "/Volumes/Drive/Backups.backupdb/xxx\ MacBook\ Pro/2014-11-19-174311/"

Enter the cmd "xattr -l Macintosh\ HD/"

It will show a bytes value like this "2123231232" next to "com.apple.backupd.VolumeBytesUsed"

Use Google Convertor GB to Bytes for convert the GB value to bytes and vice versa.

Check the value shows in terminal and disk inventory x same.

If different, use the convertor with disk inventory x value and get bytes value.

Use cmd "xattr -w com.apple.backupd.VolumeBytesUsed 219637221376 [Machintosh HD folder].

Again use the cmd "xattr -l Macintosh\ HD/" for the value is updated.

That is it.. Now your are ready to go..
```

实际操作：

1. `mount -t afp afp://${ip}/${shareName} /Volumes/${mountPoint}`
2. `hdid /Volumes/${mountPoint}`
  or `hdiutil attach ...`
  会挂载到类似`/Volumes/Time Machine Backups`
3. `xattr -l /Volumes/Time\ Machine\ Backups/Backups.backupdb/Xx’s\ iMac/2019-07-08-205937/ssd-system`
  `com.apple.backupd.VolumeBytesUsed: 831525089280`
  计算是700多GB，实际是备份时的硬盘使用容量。 但问题是**不是所有内容都备份了啊，实际就备份了几十GB**。
  吐血的是，**time machine会校验这个值和restore destination的硬盘大小**。 所以，必要的时候手动修改一下这个。
4. `xattr -w com.apple.backupd.VolumeBytesUsed 85899345920 /Volumes/Time\ Machine\ Backups/Backups.backupdb/Xx’s\ iMac/2019-07-08-205937/ssd-system`  
  改成了80GB。
5. `hdiutil detach "/Volumes/Time Machine Backups"`

然后就可以继续前面的restore流程了。 这时候（**仅限修改了该属性值的那个snap**）不会报错空间不够了。

# tools

## tmutil

* `tmutil destinationinfo`
* `tmutil setdestination ${path}`

## hdiutil
