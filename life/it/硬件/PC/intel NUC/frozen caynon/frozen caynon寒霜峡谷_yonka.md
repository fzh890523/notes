

## 问题

### 外接usb sata ssd（macos）但无法boot

表现为：

1. boot menu里没有接各个口啊，关闭secure boot啊
  各种都试了也不行，
2. 进esxi系统里lsusb能看到这个usb设备
3. 用烧录esxi的bootable u盘，是可以检测到并出现在boot menu的

目前猜测可能的原因：

* usb sata ssd可能不支持？
  这个打算换到内置sata位试试
* mac的boot没被识别（？！）
  这里有个启发是： vmware的vm如果类型设为windows的话，boot时会因为没找到正确的boot loader而跳过安装好的macos的磁盘。 改vm类型之后才ok。 那类似的是不是哪里能改？

