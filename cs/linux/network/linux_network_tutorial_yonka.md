


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



### register hostname



”自动“注册/广播本机hostname的方式： dhcp

> dhcp支持`send hostname`特性，也即在流程中将本机hostname告知dhcp server。 而如果dhcp server支持该特性且跟dns server打通的话，会自动加入该host条目
>
> 而一般常见场景，dhcp server往往是local dns server



配置如： `cat /etc/dhcp/dhclient.conf`

```s
send host-name = gethostname();
request subnet-mask, broadcast-address, time-offset, routers,
        domain-name, domain-name-servers, domain-search, host-name,
        dhcp6.name-servers, dhcp6.domain-search, dhcp6.fqdn, dhcp6.sntp-servers,
        netbios-name-servers, netbios-scope, interface-mtu,
        rfc3442-classless-static-routes, ntp-servers;
```



然后： 

```sh
nslookup iMac.localdomain  # 即可
```



有些dhcp server可能没实现，比如：

* vmware的nat网络模式下的dhcp server

  此时dns server都不是gateway，囧。 比如gateway `192.168.64.1`而dns server是`192.168.64.2`

  vmware的hostonly模式下，都不会起dns server。 虽然client侧默认认为gateway（dhcp server？）是dns server，但对方端口没开。。。



### resolve hostname



* glib库行为

  读取`/etc/resolv.conf` 根据配置来确定行为，详见 [linux_name resolution_resolv.conf.md](name resolution/linux_name resolution_resolv.conf.md)

  读取`/etc/hosts` 文件...

* 常见架构

  * 本地dns代理

    比如较新版本的ubuntu就会使用`systemd-resolved`来做代理，具体会：

    * 接管`resolv.conf`配置，不能/不应再人工修改该配置，而是通过配置`systemd-resolved`来...

    * 监听在 `127.0.0.53:53`

    * 会管理各个接口上的dns resolve

      应该能配置让指定接口脱离管理
      
      `systemd-resolve --status`可以看到类似如下的内容
      
      ```s
      Link 3 (ens38)
            Current Scopes: DNS
             LLMNR setting: yes
      MulticastDNS setting: no
            DNSSEC setting: no
          DNSSEC supported: no
               DNS Servers: 192.168.153.1
                DNS Domain: localdomain
      ```
      
      
      
      

## route



### 设置（指定接口）默认路由权重



* 立即生效但不持久化： `ip route`

* 持久化

  * netplan方案

    ```yaml
    # 接口级别：
    dhcp4-overrides:
    	route-mertic: 99  # 默认100
    ```

    要生效的话见netplan命令 tutorial。





