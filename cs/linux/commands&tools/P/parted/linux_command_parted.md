



## 一览



* `sudo parted -a optimal /dev/sdb print all`



## cases



### **aligned很难设置**

报错：`the resulting partition is not properly aligned for best performance`



<https://unix.stackexchange.com/questions/38164/create-partition-aligned-using-parted>



In order to align partition with `parted` you can use `--align` option. Valid alignment types are:

- **none** - Use the minimum alignment allowed by the disk type.
- **cylinder** - Align partitions to cylinders.
- **minimal** - Use minimum alignment as given by the disk topology information. This and the opt value will use layout information provided by the disk to align the logical partition table addresses to actual physical blocks on the disks. The min value is the minimum alignment needed to align the partition properly to physical blocks, which avoids performance degradation.
- **optimal** Use optimum alignment as given by the disk topology information. This aligns to a multiple of the physical block size in a way that guarantees optimal performance.

Other useful tip is that you can set the size with percentages to get it aligned. Start at 0% and end at 100%. For example:

```
parted -a optimal /dev/sda mkpart primary 0% 4096MB
```





```sh
sudo parted -a optimal /dev/sdb print all
Model: ASMT 2135 (scsi)
Disk /dev/sdb: 8002GB
Sector size (logical/physical): 512B/4096B
Partition Table: gpt
Disk Flags:

Number  Start   End     Size    File system  Name     Flags
 1      33.6MB  4000GB  4000GB  ext4         primary
 2      4000GB  6000GB  2000GB  ext4         primary
 3      6000GB  8002GB  2002GB  ext4         primary
```



之后

```sh
sudo mkfs.ext4 /dev/sdb1
mke2fs 1.44.1 (24-Mar-2018)
/dev/sdb1 alignment is offset by 512 bytes.
This may result in very poor performance, (re)-partitioning suggested.
...

# use gdisk
Command (? for help): print
Disk /dev/sdb: 15628053168 sectors, 7.3 TiB
Model: 2135
Sector size (logical/physical): 512/4096 bytes
Disk identifier (GUID): 4317292F-D164-498F-9787-424E80D007F9
Partition table holds up to 128 entries
Main partition table begins at sector 2 and ends at sector 33
First usable sector is 34, last usable sector is 15628053134
Partitions will be aligned on 8-sector boundaries
Total free space is 118256 sectors (57.7 MiB)

Number  Start (sector)    End (sector)  Size       Code  Name
   1           65535      7812492884   3.6 TiB     8300  primary
   2      7812492885     11718772094   1.8 TiB     8300  primary
   3     11718772095     15628000379   1.8 TiB     8300  primary
```



<https://www.linuxquestions.org/questions/slackware-14/alignment-is-offset-by-512-bytes-this-may-result-in-very-poor-performance-4175611722/> 提到：

```
I've always used fdisk or gdisk, which now will automatically select the best starting point for the drive to make sure it is aligned (I think older versions of at least fdisk didn't do proper alignment on the relatively new 4k sectored disks). It seems that parted doesn't do that automatically (or at least not with the way you called it... there might be different options that will attempt proper alignment).
```



**换用gdisk了，差点踩坑!**

