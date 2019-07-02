

**挂载光驱**

```sh
mount -t iso9660 /dev/sr0 /media/cdrom
```





## fstab



### via UUID



* `blkid`的`UUID="xxx"`中xxx部分(fstab文档说明里明确提及)

  不是`PARTUUID` (= gdisk里的`Partition unique GUID`)

  也不是gdisk里的`Partition GUID code`

* `mkfs.ext4`时输出的`Filesystem UUID:`后的内容



