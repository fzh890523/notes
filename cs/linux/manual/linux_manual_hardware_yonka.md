





## 设备识别



### scsi设备



#### 新设备识别/scan



`echo "- - -" >> /sys/class/scsi_host/host_$i/scan`

见： `rescan_scsi_hosts.sh`





### 块设备



#### 块设备查看



1. 根据设备类型，比如硬盘为块设备

   ```sh
   $ sudo lsblk
   NAME   MAJ:MIN RM  SIZE RO TYPE MOUNTPOINT
   fd0      2:0    1  1.4M  0 disk
   loop0    7:0    0 71.3M  1 loop /snap/lxd/16100
   loop1    7:1    0   55M  1 loop /snap/core18/1754
   loop2    7:2    0   97M  1 loop /snap/core/9665
   loop3    7:3    0 96.5M  1 loop /snap/core/9436
   loop4    7:4    0 71.2M  1 loop /snap/lxd/15896
   loop5    7:5    0   55M  1 loop /snap/core18/1880
   sda      8:0    0   60G  0 disk
   ├─sda1   8:1    0    1M  0 part
   └─sda2   8:2    0   60G  0 part /
   sr0     11:0    1 1024M  0 rom
   sr1     11:1    1 1024M  0 rom
   
   $ sudo blkid
   /dev/fd0: SEC_TYPE="msdos" UUID="xxxx-yyyy" TYPE="vfat"
   /dev/sda2: UUID="xxxxxxxx-7a32-4cb2-b3ce-yyyyyyyyyyyy" TYPE="ext4" PARTUUID="xxxxxxxx-ac37-4006-9275-yyyyyyyyyyyy"
   /dev/loop0: TYPE="squashfs"
   /dev/loop1: TYPE="squashfs"
   /dev/loop2: TYPE="squashfs"
   /dev/loop3: TYPE="squashfs"
   /dev/loop4: TYPE="squashfs"
   /dev/loop5: TYPE="squashfs"
   /dev/sda1: PARTUUID="xxxxxxxx-299a-4fc1-a436-yyyyyyyyyyyy"
   ```

   

### USB设备



#### USB设备查看

ref: [askubuntu: How do I know the device path to an USB-stick?](https://askubuntu.com/questions/311772/how-do-i-know-the-device-path-to-an-usb-stick)



**Device names of flash drives**

A flash drive can be connected

- **via USB** (typically a USB stick or a memory card via a USB adapter)
- the device name is the same as for SATA drives, `/dev/sdx`
- and partitions are named `/dev/sdxn`

where **`x`** is the device letter and **`n`** the partition number, for example `/dev/sda1`

- **via PCI** (typically a memory card in a built-in slot in a laptop)
- the device name is `/dev/mmcblkm`
- and partitions are named `/dev/mmcblkmpn`

where **`m`** is the device number and **`n`** the partition number, for example `/dev/mmcblk0p1`

**Example with an SSD, HDD, USB pendrive and an SD card**

**lsblk**

```sh
$ sudo lsblk -o model,name,fstype,size,label,mountpoint
MODEL            NAME        FSTYPE    SIZE LABEL              MOUNTPOINT
Samsung SSD 850  sda                 232,9G                    
                 ├─sda1      vfat      300M EFI                /boot/efi
                 ├─sda2                  1M                    
                 ├─sda3      ext4      100G root               
                 └─sda4      swap        5G                    [SWAP]
00BEKT-00PVMT0   sdb                 298,1G                    
                 ├─sdb1      vfat      480M                    
                 ├─sdb2      ext4       80G lubuntu-xenial64   /media/tester/lubuntu-xenial64
                 ├─sdb3      swap      3,9G                    [SWAP]
                 ├─sdb4      ext4      100G ubuntu-artful64x   /
                 └─sdb5      ext4    113,7G ubuntu-artful64w   /media/tester/ubuntu-artful64w
Extreme          sdc                  14,6G                    
                 ├─sdc1      ntfs      6,7G usbdata            /media/tester/usbdata1
                 ├─sdc2                  1M                    
                 ├─sdc3      vfat      244M usbboot            
                 ├─sdc4      iso9660   948M Lubuntu 17.10 i386 /media/tester/Lubuntu 17.10 i386
                 └─sdc5      ext4      6,7G casper-rw          /media/tester/casper-rw
                 mmcblk0               3,7G                    
                 ├─mmcblk0p1 ntfs      661M usbdata            /media/tester/usbdata
                 ├─mmcblk0p2             1K                    
                 ├─mmcblk0p3 vfat      122M usbboot            /media/tester/usbboot
                 ├─mmcblk0p4 iso9660   355M 9w-dus             /media/tester/9w-dus
                 └─mmcblk0p5 ext4      2,6G persistence        /media/tester/persistence
```

Edit for `lsblk` list: With modern versions of Ubuntu you need not use `sudo`, and when there are (many) snaps, and we don't want them to clutter the list, you the following command line,

```sh
lsblk -o model,name,fstype,size,label,mountpoint | grep -v " loop.*snap"
```

**parted**

```sh
$ sudo parted -ls
[sudo] password for tester: 
Model: ATA Samsung SSD 850 (scsi)
Disk /dev/sda: 250GB
Sector size (logical/physical): 512B/512B
Partition Table: gpt
Disk Flags: 

Number  Start   End    Size    File system     Name  Flags
 1      1049kB  316MB  315MB   fat32                 boot, esp
 2      316MB   317MB  1049kB
 3      317MB   108GB  107GB   ext4            root
 4      244GB   249GB  5369MB  linux-swap(v1)


Model: WDC WD32 00BEKT-00PVMT0 (scsi)
Disk /dev/sdb: 320GB
Sector size (logical/physical): 512B/512B
Partition Table: gpt
Disk Flags: 

Number  Start   End     Size    File system     Name                  Flags
 1      33,6MB  537MB   503MB   fat32           EFI System Partition  boot, esp
 2      537MB   86,4GB  85,9GB  ext4
 4      86,4GB  194GB   107GB   ext4
 5      194GB   316GB   122GB   ext4
 3      316GB   320GB   4161MB  linux-swap(v1)


Model: SanDisk Extreme (scsi)
Disk /dev/sdc: 15,7GB
Sector size (logical/physical): 512B/512B
Partition Table: gpt
Disk Flags: 

Number  Start   End     Size    File system  Name     Flags
 2      1049kB  2097kB  1049kB               primary  bios_grub
 3      2097kB  258MB   256MB   fat32        primary  boot, esp
 4      258MB   1252MB  994MB                primary
 5      1252MB  8473MB  7221MB  ext2         primary
 1      8473MB  15,7GB  7221MB  ntfs         primary  msftdata


Model: SD SD04G (sd/mmc)
Disk /dev/mmcblk0: 3965MB
Sector size (logical/physical): 512B/512B
Partition Table: msdos
Disk Flags: 

Number  Start   End     Size    Type      File system  Flags
 3      2097kB  130MB   128MB   primary   fat32        boot
 4      130MB   502MB   372MB   primary
 2      502MB   3272MB  2769MB  extended               lba
 5      503MB   3272MB  2768MB  logical   ext2
 1      3272MB  3965MB  693MB   primary   ntfs
```











