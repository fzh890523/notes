

# intro



## partition name



### vs filesystem label



[https://unix.stackexchange.com/questions/289477/what-is-the-difference-between-a-name-and-a-label-in-gparted#:~:text=A%20partition%20name%20is%20a,you%20can%20manipulate%20with%20e2label%20.](https://unix.stackexchange.com/questions/289477/what-is-the-difference-between-a-name-and-a-label-in-gparted#:~:text=A partition name is a,you can manipulate with e2label .)

A partition name is a [name given in the GPT](http://gparted.org/display-doc.php?name=help-manual#gparted-name-partition); it's external to the partition itself. A partition label is a label stored inside the filesystem; for example with `ext`-family filesystems, this is the label you can manipulate with [`e2label`](http://man7.org/linux/man-pages/man8/e2label.8.html).

You can then use filesystem labels or partition names to mount the filesystems, which helps avoid issues with disk name changes. [`mount(8)`](http://man7.org/linux/man-pages/man8/mount.8.html) has more information on this (search for "label"). It probably makes sense for the name to match the label...





https://superuser.com/questions/1099232/what-is-the-difference-between-a-partition-name-and-a-partition-label

The GPT disk format can simply contain more data than its predecessors. One of these fields is the partition name, which, as you have found, can be set by `gdisk` or similar. However, before GPT, there were formats which didn't support labels in the partition table (e.g. MBR), so the data was stored in the fs as a filesystem label. Using GPT with a filesystem that supports labels (i.e. basically anything) therefore produces the slightly confusing combination of *partition* name/label and *filesystem* name/label.

These values are totally independent, (although I don't exactly recommend making them totally different) and most programs will use the fs label, as that one is available across basically all file systems, unlike the partition name, which is only on GPT. However, some programs can use the partition name too. (E.g. `/dev/disk/` contains `by-label/` as well as `by-partlabel/`.)

I'm not quite sure as to *why* you'd ever have to use the partition label, but a contrived example could be an encrypted `/home` partition. Because the partition is encrypted, the label (or UUID for that matter) cannot be extracted without decrypting it first. (Note that common containers do support these values.) Therefore, you'd have to hardcode the partition number somewhere. Because no one likes hardcoded numbers, you could instead set the partition label and access the partition like that when you decrypt.



`partition name` 是gpt对分区的描述信息里的字段；

`filesystem label` 则是分区中（内容）文件系统自身的字段；



简单的说： 不能识别filesystem就无法读出该label，而只要能识别gpt（基本都可以了）就可以读出partition name。



老的分区方式比如mbr，本身不支持存储描述字段，所以就没有partition name这个东西，所以就只好用filesystem label了。



# manual



## mount/unmount



* `mount`
  * 不带参数： print list
  * `-a` mount all，需要root权限
* `umount`
  * `-l` lazily，也即先从file hierarchy里删掉使得fs不可见（不能新open），等已有操作自然结束
  * `-f` force，强制。。也即即使有进行中的操作也。。。
  * `-a` unmount all
  * `-v` verbose，输出更多信息（比如排查为什么unmount失败等等）



只能对filesystem/partition操作，有点烦，比如分区比较多的时候。。。



## “安全移除”



unmount + poweroff



```sh
udisksctl unmount -b /dev/sda1
udisksctl power-off -b /dev/sda

# for non-systemd user
udisks --unmount /dev/sda1
udisks --detach /dev/sda
```





### unmount vs eject

https://unix.stackexchange.com/questions/178638/eject-safely-remove-vs-umount

文中提到在安全性上，unmount已经足够安全，跟eject的差别只在于比如供电电压（5v vs 0.5v）等。

但也有评论说 unmount和udiskctl power-off都不是严格意义的安全移除（杠精？）。



