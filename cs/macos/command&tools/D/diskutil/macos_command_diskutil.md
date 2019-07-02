



# intro

## device

```
DEVICES

A device parameter to any of the above commands (except where explicitly required otherwise) can usually be any of the following:

o The disk identifier (see below). Any entry of the form of disk*, e.g. disk1s9.

o The device node entry containing the disk identifier. Any entry of the form of /dev/disk*, e.g. /dev/disk2.

o The volume mount point. Any entry of the form of /Volumes/*, e.g. /Volumes/Untitled. In most cases, a "custom" mount point e.g. /your/custom/mountpoint/here is also accepted.

o The URL form of any of the volume mount point forms described above. E.g. file:///Volumes/Untitled or file:///.

o A UUID. Any entry of the form of e.g. 11111111-2222-3333-4444-555555555555. The UUID can be a "media" UUID which IOKit places in an IOMedia node as derived from e.g. a GPT map's partition UUID, or it can be an AppleRAID (or CoreStorage) set (LV) or member (PV) UUID.

From man diskutil, section 'Devices'.
```

# tutorial

## list



```sh
/dev/disk0 (internal, physical):
   #:                       TYPE NAME                    SIZE       IDENTIFIER
   0:      GUID_partition_scheme                        *1.0 TB     disk0
   1:                        EFI EFI                     209.7 MB   disk0s1
   2:                  Apple_HFS Macintosh HD            880.3 GB   disk0s2
   3:                 Apple_Boot Recovery HD             650.0 MB   disk0s3
   4:                  Apple_HFS reinstall               118.9 GB   disk0s4

/dev/disk2 (external, physical):
   #:                       TYPE NAME                    SIZE       IDENTIFIER
   0:      GUID_partition_scheme                        *1.0 TB     disk2
   1:                        EFI EFI                     209.7 MB   disk2s1
   2:                 Apple_APFS Container disk3         1000.0 GB  disk2s2

/dev/disk3 (synthesized):
   #:                       TYPE NAME                    SIZE       IDENTIFIER
   0:      APFS Container Scheme -                      +1000.0 GB  disk3
                                 Physical Store disk2s2
   1:                APFS Volume ssd-system              15.9 GB    disk3s1
   2:                APFS Volume Preboot                 44.7 MB    disk3s2
   3:                APFS Volume Recovery                512.4 MB   disk3s3
   4:                APFS Volume VM                      4.3 GB     disk3s4

/dev/disk4 (external, physical):
   #:                       TYPE NAME                    SIZE       IDENTIFIER
   0:      GUID_partition_scheme                        *8.0 TB     disk4
   1:                        EFI EFI                     209.7 MB   disk4s1
   2:                 Apple_APFS Container disk5         8.0 TB     disk4s2

/dev/disk5 (synthesized):
   #:                       TYPE NAME                    SIZE       IDENTIFIER
   0:      APFS Container Scheme -                      +8.0 TB     disk5
                                 Physical Store disk4s2
   1:                APFS Volume hdd-timemachine         70.9 GB    disk5s1
   2:                APFS Volume storage                 6.8 GB     disk5s2

/dev/disk6 (external, physical):
   #:                       TYPE NAME                    SIZE       IDENTIFIER
   0:      GUID_partition_scheme                        *4.0 TB     disk6
   1:         Microsoft Reserved                         134.2 MB   disk6s1
   2:       Microsoft Basic Data entertaiment            1.1 TB     disk6s2
   3:       Microsoft Basic Data study                   1.1 TB     disk6s3
   4:       Microsoft Basic Data storage                 1.1 TB     disk6s4
   5:       Microsoft Basic Data other                   779.4 GB   disk6s5
```





## info



```sh
diskutil info
Usage:  diskutil info[rmation] [-plist]
        MountPoint|DiskIdentifier|DeviceNode|UUID | -all
Display detailed information about a disk or partition.
```



```sh
$ diskutil info disk6
   Device Identifier:         disk6
   Device Node:               /dev/disk6
   Whole:                     Yes
   Part of Whole:             disk6
   Device / Media Name:       BUP BL

   Volume Name:               Not applicable (no file system)
   Mounted:                   Not applicable (no file system)
   File System:               None

   Content (IOContent):       GUID_partition_scheme
   OS Can Be Installed:       No
   Media Type:                Generic
   Protocol:                  USB
   SMART Status:              Not Supported

   Disk Size:                 4.0 TB (4000787029504 Bytes) (exactly 7814037167 512-Byte-Units)
   Device Block Size:         512 Bytes

   Read-Only Media:           No
   Read-Only Volume:          Not applicable (no file system)

   Device Location:           External
   Removable Media:           Fixed

   Virtual:                   No
   
   
$ diskutil info disk6s1
   Device Identifier:         disk6s1
   Device Node:               /dev/disk6s1
   Whole:                     No
   Part of Whole:             disk6

   Volume Name:               Not applicable (no file system)
   Mounted:                   Not applicable (no file system)
   File System:               None

   Partition Type:            Microsoft Reserved
   OS Can Be Installed:       No
   Media Type:                Generic
   Protocol:                  USB
   SMART Status:              Not Supported
   Disk / Partition UUID:     82EA9757-68D8-4431-BE43-5E3B15D83DBF
   Partition Offset:          17408 Bytes (34 512-Byte-Device-Blocks)

   Disk Size:                 134.2 MB (134217728 Bytes) (exactly 262144 512-Byte-Units)
   Device Block Size:         512 Bytes

   Read-Only Media:           No
   Read-Only Volume:          Not applicable (no file system)

   Device Location:           External
   Removable Media:           Fixed
   

$ diskutil info disk6s2
   Device Identifier:         disk6s2
   Device Node:               /dev/disk6s2
   Whole:                     No
   Part of Whole:             disk6

   Volume Name:               entertaiment
   Mounted:                   Yes
   Mount Point:               /Volumes/entertaiment

   Partition Type:            Microsoft Basic Data
   File System Personality:   NTFS
   Type (Bundle):             ntfs
   Name (User Visible):       Windows NT File System (NTFS)


   OS Can Be Installed:       No
   Media Type:                Generic
   Protocol:                  USB
   SMART Status:              Not Supported
   Volume UUID:               C98E7E74-AA67-4C7C-84DF-BF1EAE9E642E
   Disk / Partition UUID:     A68F61CE-8E60-4C2B-958E-8DFCB28F206E
   Partition Offset:          135266304 Bytes (264192 512-Byte-Device-Blocks)

   Disk Size:                 1.1 TB (1073741824000 Bytes) (exactly 2097152000 512-Byte-Units)
   Device Block Size:         512 Bytes

   Volume Total Space:        1.1 TB (1073741819904 Bytes) (exactly 2097151992 512-Byte-Units)
   Volume Used Space:         254.5 GB (254497488896 Bytes) (exactly 497065408 512-Byte-Units) (23.7%)
   Volume Free Space:         819.2 GB (819244331008 Bytes) (exactly 1600086584 512-Byte-Units) (76.3%)
   Allocation Block Size:     4096 Bytes

   Read-Only Media:           No
   Read-Only Volume:          Yes

   Device Location:           External
   Removable Media:           Fixed
```





## mount

```
Usage:   diskutil mount [readOnly] [-mountPoint Path] DiskIdentifier|DeviceNode
Mount will mount the volume in the "standard" place (/Volumes), unless
an optional mount point is specified.


     mount [readOnly] [-mountPoint path] device
                Mount a single volume.  If readOnly is specified, then the file system is
                mounted read-only, even if the volume's underlying file system and/or
                device and/or media supports writing; even the super-user may not write
                to it; this is the same as the rdonly option to mount (8).  If a
                -mountPoint is specified, then that path, rather than the standard path
                of /Volumes/VolumeName, will be used as the view into the volume file
                content; a directory at that path must already exist.
```

可以通过几种identifier：
* volume
  ```sh
  diskutil mount /dev/diskXsY
  ```
* disk
  ```sh
  diskutil mountDisk /dev/diskX
  ```
* uuid
  ```sh
  diskutil mount [Volume/Partition UUID]
  ```
* label
  ```sh
  diskutil mount [label]
  ```



## unmount



```sh
diskutil unmount /Volumes/xxx
diskutil unmount disk6s2
```





# ref

* https://apple.stackexchange.com/questions/235230/how-to-mount-disk-by-uuid-or-label-in-os-x-el-capitan
