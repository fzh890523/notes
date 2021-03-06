

# view system info

## codename

```sh
lsb_release -c | awk  '{print $2}'
```





# 升级发行版



```sh
sudo apt-get update
sudo apt-get upgrade  # 这一步会升级kernel吧，可能出现grub分区选择错误，处理方式见下文
sudo apt-get dist-upgrade
sudo apt-get install update-manager-core

sudo reboot

sudo do-release-upgrade  # 如果eol不再支持的版本，可能会提示... 而无法... 这时要按下面的操作：
# 执行这一步实际升级的过程中又会install grub一次，vmware的磁盘顺序可能会变，所以还可能报错，再选择回正确的盘符即可
```



```sh
# 修改source里的ubuntu域名为 old-releases.ubuntu.com，这个域名不支持国家子域名
sudo sed -i.save -e 's/\/\/.*ubuntu.com/\/\/old-releases.ubuntu.com/g' /etc/apt/sources.list
# 然后再执行 do-release-upgrade
# 如果因为eol而apt不可用从而前面几条命令没执行成功的话，都执行一遍
```



**grub分区选择错误**

会提示问要不要继续，选择no会转到分区选择菜单，这时候要确认哪个分区是用（原来）用于装grub的。

1. fdisk看下那个盘是系统盘（vmware最开始那个盘）

2. 选择分区，一般是盘那个（比如 `sdb` vs `sdb2`）

   `sudo dd if=/dev/sdb bs=512 count=1 | xxd` 可以查看内容，输出里有GRUB的那个吧

   我这里`sdb2`是root，不等于...

