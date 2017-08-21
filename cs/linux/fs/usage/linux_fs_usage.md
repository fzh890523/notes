## FS block大小



`tune2fs -l /dev/${dev} | grep "Block size"`

如：

```shell
sudo /sbin/tune2fs -l /dev/sda1 | grep "Block size"
Block size: 4096
```









