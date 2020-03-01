


## interfaces

### 网卡默认没启用

遇到的场景是：

* vmware

* ubuntu 18.04.13

* 双网卡： 一个nat、一个vmnet

问题：

* vmnet的网卡没启动，`ifconfig`和`netstats -i`看不到，更没拿到ip

跟进：

* `ip link`看到该网卡

* `ip link set ens38 up`后可以启动，但没有分配ip

* `/etc/network/interfaces`里看到被netplan托管，继续跟进，了解到这里是因为netplan默认只启用/配置 配置中的接口

  如下图，cloud-init生成的那个netplan配置里，只有`ens33`

处理办法：

```sh
vim /etc/netplan/01-netcfg.yaml

network:
    ethernets:
        ens33:
            dhcp4: true
        ens38:
            dhcp4: true
    version: 2

netplan generate
netplan apply
# ok 了，可以看到分配地址
```


## dhcp

### renew dhcp addr

```sh
dhclient -r
# or 
dhclient -r ${interface_name}  # dhclient -r eth0
```



### dhcp broadcast hostname

```sh
cat /etc/dhcp/dhclient.conf | grep send
# 确保其中有： send host-name = gethostname();
# 如果希望写死的话，也可以 send host-name = "xxx";
```





## hostname



### set hostname



* `sudo hostname ${new_hostname}` 

  当前生效，重启丢失

*  `sudo vim /etc/hostname` 修改其中内容

  持久有效(但当前要生效的话，按前一条操作或者`sudo service hostname restart`)

* 在`/etc/network/interfaces`里针对特定接口进行配置







