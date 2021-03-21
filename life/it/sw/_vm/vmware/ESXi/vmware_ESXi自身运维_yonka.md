

# install

参考：

* `01/24_餐边柜pc安装esxi.md`
* `01/24_已有vmware 虚拟机转换到esxi.md`

* `03/19_nuc8安装esxi.md`



## guide

* 比较合适的方式是安装到usb
  * esxi会使用整个硬盘，进行（自己的）分区，安装系统和创建datastore。 这样会导致数据和系统耦合。  用u盘安装系统，把硬盘只作为datastore则可解耦
  * 插拔方便
  * 故障恢复方便





## usb安装

**前提**

* 32GB应该就够
* swap、logfile改到datastore

* 估计写比较少，启动时读。 整体对u盘性能要求不高，读快点更好
* 显然对u盘稳定性要求高一些



**准备**

* bios设置里legacy boot打开，要能从u盘启动

* 制作exsi镜像（如果需要加驱动的话）。 烧录到u盘



# client



* <del>native client</del>
* web client



`There is no vSphere Client for 6.7. The old legacy C# client is long gone and not compatible with 6.7. The HTML5 embedded host client (https://FQDN/ui) is what's now available.`



## web client



### misc



#### idle timeout

`host - manage - system - advanced settings`

* key: UserVars.HostClientSessionTimeout
* name:  Default timeout for Host Client sessions in seconds
* 限制： 0 ≤ x ≤ 7200
* default: 60



最大只能设为2h



# network

## ssh



### add authorized_keys

ref： [Allowing SSH access to ESXi/ESX hosts with public/private key authentication (1002866)](https://kb.vmware.com/s/article/1002866)

不同版本或有差异。`For ESXi 5.x, 6.0, 6.5 and 6.7, the authorized_keys is located at: /etc/ssh/keys-<username>/authorized_keys`

> 默认只有 keys-root 目录，其他用户的要自己创建（有权限）



# storage



## data store



### data store 访问



* 可以通过scp： `scp <user>@<esxi_host>:/vmfs/volumes/<datastore>/path/to/files/* ./`

  > vm可以以这种方式访问



# user



`host` - `actions` - `permissions` - `add user`/`assign role`： 好像是esxi系统user，比如ssh进去后，administrator等同于root

`manage` - `security & users` - `users` - `add user`： 好像是esxi web console user