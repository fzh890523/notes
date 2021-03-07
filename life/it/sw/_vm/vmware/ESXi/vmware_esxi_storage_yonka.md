



## datastore



### 从特定分区而不是整个硬盘创建datastore

* ui操作（？待确认是否可以）
* `vmkfstools -C` 具体用法要查查



## 硬盘直通



ref: 

* [[VMware]ESXI下硬盘的两种直通方式](https://rmbz.net/archives/vmware-esxi-passthrough)

* [ESXi 6.7 + DSM 6.2 搭建黑群晖的研究](https://www.tqwba.com/x_d/jishu/61154.html)



* RDM直通

  ```
  通过RDM（Raw Device Mapping）方式，将磁盘应设为本地VMDK
  优点：不需要增加硬件和修改任何配置文件，配置起来很易用
  缺点：如果需要对硬盘调整，需要重新映射；群晖无法获取到SMART信息
  ```

  1.通过命令`ls -l /dev/disks`查看当前挂载的所有磁盘信息。

  2.通过命令`vmkfstools -z /vmfs/devices/disks/<直通硬盘的标识符> /vmfs/volumes/<保存vmdk的硬盘标识符>/<VMDK名字>.vmdk`，进行挂载；其中<直通硬盘的标识符>为上一个命令中的类似这样的值`t10.ATA_____ST3500413AS_________________________________________5VMQDMFJ`；<保存vmdk的硬盘标识符>可在`/vmfs/volumes/`查看的到；<VMDK名字>名字随意。

  ```
  [root@HOME-ESXI:~] cd /vmfs/volumes/
  [root@HOME-ESXI:/vmfs/volumes] ls
  1eeffc73-bcd27263-1e02-52d4b71eff6e  5e2966c6-05e642a6-2fa3-7085c2860b45  6d530177-2222d68b-7c96-8146fdca8e095e2966c0-51442ba6-628a-7085c2860b45  5e2966c6-ee8383fe-dd4c-7085c2860b45  local.vm.store
  [root@HOME-ESXI:/vmfs/volumes] cd local.vm.store/
  [root@HOME-ESXI:/vmfs/volumes/5e2966c6-ee8383fe-dd4c-7085c2860b45] mkdir DMS.store
  [root@HOME-ESXI:/vmfs/volumes/5e2966c6-ee8383fe-dd4c-7085c2860b45/DMS.store] vmkfstools -z /vmfs/devices/disks/t10.ATA_____ST3500413AS_________________________________________5VMQDMFJ /vmfs/volumes/local.vm.store/DMS.store/disk4.vmdk
  ```

  > yonka： 这个命令应该只做映射，不影响原磁盘内容

  3.设置群晖磁盘
  ![WX202001251204292x.png](https://cdn.rmbz.net/halo/WX20200125-120429@2x_1579925084016.png!halo.image)

  至此，RDM直通设置完成；多磁盘与此类似。

* PCI-e直通 (适用所有不能直通的PCIE设备)

  ```
  通过修改/etc/vmware/passthru.map配置文件实现
  优点：可以对几乎任何所有的PCIE设备直通；不需要担心磁盘顺序等
  缺点：如果对AHCI设备配置，会对整个设备映射，无法单独对某块硬盘；需要修改ESXI系统配置文件；部分不支持SMART信息；需要预留内存空间
  ```

  1.打开直通映射文件`vi /etc/vmware/passthru.map`
  2.在esxi上查看设备供应商ID(VID)、设备ID(PID)等信息
  ![WX202001251331592x.png](https://cdn.rmbz.net/halo/WX20200125-133159@2x_1579930603436.png!halo.image)
  3.在`passthru.map`中增加对应配置

  ```
  # <供应商ID> <设备ID> d3d0 default# Intel Corporation Union Point-H AHCI Controller8086  a282  d3d0     default
  ```

  ![WX202001251358392x.png](https://cdn.rmbz.net/halo/WX20200125-135839@2x_1579931943176.png!halo.image)
  4.重启ESXI服务
  5.开启直通
  ![WX202001251405412x.png](https://cdn.rmbz.net/halo/WX20200125-140541@2x_1579932366281.png!halo.image)
  6.配置群晖，并预留内存空间
  ![WX202001251425032x.png](https://cdn.rmbz.net/halo/WX20200125-142503@2x_1579933521908.png!halo.image)
  7.群晖对硬盘识别正常
  ![WX202001251430332x.png](https://cdn.rmbz.net/halo/WX20200125-143033@2x_1579933857203.png!halo.image)

* 磁盘控制器透传

  ```
  这种玩法稍微高级一些，当然也需要点额外投资，你可能需要买一张连接硬盘的卡，而且不是所有卡都被ESXi所支持，这点需要特别注意。我买了两张卡来做这个研究，一张是LSI SAS2008 9211-8i HBA卡，另一张是SU-SA3014 Marvell 9215的SATA转接卡。
  
  为了阐述方便，下面我就用芯片型号代替这两张卡了。
  
  9211-8i实际上是硬盘阵列卡， 是HBA卡，有自己的管理系统，可玩性相对高一些，可以连接8块SAS硬盘（SATA是SAS的子类），需要额外购买一拖四的转接线（MINI SAS 36P SFF 8087转SATA），给ESXi和群晖使用时需要刷IT固件（还有一种IR固件是用来做RAID的），购买时需要跟卖家说明用途，一般会给你免费刷好的。这张卡某宝100元左右，数据线20元左右。
  9215相对简单很多，就是单纯的SATA转接卡，数据线就用普通的SATA线即可，到处都有非常好找。这张卡某宝90多元可入手，数据线也就几块钱。
  透传的好处是群晖可以读到硬盘的S.M.A.R.T信息，DSM用这个信息来进行失效预警，这是比较有用的东西。此外，读取到S.M.A.R.T信息是磁盘休眠的基础，但至于磁盘休眠有没有意义就仁者见仁了，有些人觉得休眠可以省点，不转省电机，而另一些人觉得频繁启动反而加速老化，他们甚至会说这是导致磁盘挂掉的重要原因。我现在已经不那么迷信磁盘休眠了，但我还是会根据自己的使用频率选择这个功能。
  
  透传方法也不难，首先是把这个磁盘控制器（也就是你的卡）切换为透传（passthrough）模式，然后重启ESXi。
  ```

  > 也就是，原磁盘控制器不支持的话，加个转接让它支持就行了

  



[Raw Device Mapping option in the vSphere Client is grayed out (1017704)](https://kb.vmware.com/s/article/1017704)



```
Your Sata controller does not support the calls required for RDM to work. Your only recourse is to get a LSI controller (9211-8i for example) and do the RDM through that, or format the drive for ESXi with VMFS and use VMDK's to share the data out (Backup and restore of the data on the drive today).
```





