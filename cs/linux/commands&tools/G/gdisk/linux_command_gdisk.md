

```sh
Command (? for help): help
b	back up GPT data to a file
c	change a partition's name
d	delete a partition
i	show detailed information on a partition
l	list known partition types
n	add a new partition
o	create a new empty GUID partition table (GPT)
p	print the partition table
q	quit without saving changes
r	recovery and transformation options (experts only)
s	sort partitions
t	change a partition's type code
v	verify disk
w	write table to disk and exit
x	extra functionality (experts only)
?	print this menu
```

```sh
Command (? for help): p
Disk /dev/sdb: 15628053168 sectors, 7.3 TiB
Model: 2135
Sector size (logical/physical): 512/4096 bytes
Disk identifier (GUID): 4317292F-D164-498F-9787-424E80D007F9
Partition table holds up to 128 entries
Main partition table begins at sector 2 and ends at sector 33
First usable sector is 34, last usable sector is 15628053134
Partitions will be aligned on 8-sector boundaries
Total free space is 6 sectors (3.0 KiB)

Number  Start (sector)    End (sector)  Size       Code  Name
   1              40      8589934631   4.0 TiB     8300  Linux filesystem
   2      8589934632     12884901927   2.0 TiB     8300  Linux filesystem
   3     12884901928     15628053134   1.3 TiB     8300  Linux filesystem
```


```sh
Command (? for help): i
Partition number (1-3): 1
Partition GUID code: 0FC63DAF-8483-4772-8E79-3D69D8477DE4 (Linux filesystem)
Partition unique GUID: 8D209CEF-FE7A-477D-83D4-30439C987A40  # 这个是和blkid输出的 PARTUUID一致
First sector: 40 (at 20.0 KiB)
Last sector: 8589934631 (at 4.0 TiB)
Partition size: 8589934592 sectors (4.0 TiB)
Attribute flags: 0000000000000000
Partition name: 'Linux filesystem'
```

