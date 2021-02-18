

## kinds of mount



### **挂载光驱**

```sh
mount -t iso9660 /dev/sr0 /media/cdrom
```



### bind: mount a local directory to another

```sh
mount --bind /mnt/target /tmp/test/
```



> macos上mount好像不支持bind，alternatives 见 《it_mac.md》的mount部分。



> macos上mount好像不支持bind，可以：
>
> Ref: <https://apple.stackexchange.com/questions/197029/how-do-you-mount-bind-a-local-directory>
>
> * 使用第三方FS支持，比如FUSE/bindfs
>
> ```sh
> # install osxfuse first, and then:
> brew install homebrew/fuse/bindfs
> ```
>
> * use NFS mound to do this
>
> ```sh
> sudo tee -a /etc/exports <<< "/ -alldirs -mapall=$USER localhost"
> sudo launchctl start com.apple.rpcbind
> sudo nfsd start
> sudo mount localhost:/path/to/target ./mnt
> # sudo umount ./mnt
> ```
>
> * use buildin file-sharing(afp/smb)
>
>   ```sh
>   # share target directory
>   # As finder refuse to connect to a local "network-location", we mount it manually
>   mount -t afp afp://${user}:${password}@127.0.0.1/${shareName} ${mntPoint}
>   ```
>
>   



### mount samba



### mount nfs

> 可能需要安装对应的支持，否则可能没有 `mount.nfs`。
>
> debian： sudo apt install -y nfs-common
>
> rh： yum -y install nfs-utils



`mount [OPTIONS] NFS_SERVER:/PATH/TO/EXPORTED/DIR /MOUNT_POINT_ON_CLIENT`



```sh
mount -t nfs 10.43.138.1:/ISS /tmp/logs

# 可以 -o 指定额外参数，如： -o nfsvers=3,rsize=<val>,wsize=<val>
```



fstab

```
# NFS_SERVER:/PATH/TO/EXPORTED/DIR    /MOUNT_POINT_ON_CLIENT    TYPE_OF_FS   OPTIONS   DUMP	PASS
  10.10.0.10:/backups                 /var/backups              nfs          defaults    0       0
```





## fstab



### via UUID



* `blkid`的`UUID="xxx"`中xxx部分(fstab文档说明里明确提及)

  不是`PARTUUID` (= gdisk里的`Partition unique GUID`)

  也不是gdisk里的`Partition GUID code`

* `mkfs.ext4`时输出的`Filesystem UUID:`后的内容



