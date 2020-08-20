









### 查看分区文件系统类型的方式

ref： https://www.tecmint.com/find-linux-filesystem-type/





* 从挂载信息里（只适用于已挂载或者计划挂载（有挂载配置）的情况）

  * `df -Th`

    ```sh
    df -Th
    Filesystem     Type              Size  Used Avail Use% Mounted on
    udev           devtmpfs          436M     0  436M   0% /dev
    tmpfs          tmpfs              96M  3.4M   92M   4% /run
    /dev/sda2      ext4               59G  9.7G   47G  18% /
    ```

    

  * `mount`

    ```sh
    mount
    ...
    /dev/sdc3 on /mnt/wd-xxx type ext4 (rw,relatime,stripe=8191)
    ```

    

  * `cat /etc/fstab`

* 读取fs信息

  * fsck

    ```sh
    fsck -N /dev/sdb2
    fsck from util-linux 2.34
    [/usr/sbin/fsck.ext2 (1) -- /dev/sdb2] fsck.ext2 /dev/sdb2
    # 但输出不太明确，比如此时为 没有fs的情况
    ```

  * `blkid /dev/sdb1`

    ```sh
    sudo blkid /dev/sdc1
    /dev/sdc1: UUID="xxx1" TYPE="ext4" PARTLABEL="nnn1" PARTUUID="yyy1"
    sudo blkid /dev/sdb1
    /dev/sdb1: PARTLABEL="nnn2" PARTUUID="yyy2"
    ```

  * `lsblk -f`

    ```sh
    lsblk -f
    NAME   FSTYPE   LABEL UUID                                 FSAVAIL FSUSE% MOUNTPOINT
    fd0
    loop0  squashfs                                                  0   100% /snap/lxd/16100
    ...
    sda
    ├─sda1
    └─sda2 ext4           uuida2   46.2G    16% /
    sdb
    ├─sdb1
    ├─sdb2
    └─sdb3
    sdc
    ├─sdc1 ext4           uuidc1    3.1T    16% /mnt/nc1
    ├─sdc2 ext4           uuidc2  515.5G    69% /mnt/nc2
    └─sdc3 ext4           uuidc3    1.2T     0% /mnt/nc3
    sr0
    sr1
    ```

  * `sudo file -sL /dev/sdb1`

    ```sh
    sudo file -sL /dev/sdb1
    /dev/sdb1: data
    sudo file -sL /dev/sdc1
    /dev/sdc1: Linux rev 1.0 ext4 filesystem data, UUID=uuidc1 (needs journal recovery) (extents) (64bit) (large files) (huge files)
    ```

    

