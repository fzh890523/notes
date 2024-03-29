


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



### 添加虚拟网卡

> 需要root

```sh
ip link add veth1 type veth
# 好像默认需要成对创建，需要只能是计数数。 也许这个行为可以修改（？）
# 这种是默认broadcast，所以对端能直接拿到mac，也即不需要路由
# lookback类型的则不是

ip link
# output:
## veth0@veth1
## veth1@veth0

ip addr add 192.168.188.199/24 broadcast + dev veth0

sysctl -w net.ipv4.conf.veth0.forwarding=1  # 实际这里不需要
```





## dhcp

### renew dhcp addr

```sh
# release
dhclient -r
# or 
dhclient -r ${interface_name}  # dhclient -r eth0

# 然后就没地址了。 也许有些系统里会自动触发重新获取
# 经常莫名其妙的执行不成功也不报错，多试几次也许ok了
## 成功的输出示例
sudo dhclient -r ens224
Killed old client process


# retrive
dhclient ${interface_name}
```

详见 `man dhclient`



> dhcp option里下发的路由，如果要更新（比如原来下发有误），也需要用该方式来触发更新





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



一般还要修改`/etc/hosts`中`127.0.0.1`对应条目内容



### register/broadcast hostname



”自动“注册/广播本机hostname的方式： dhcp

> dhcp支持`send hostname`特性，也即在流程中将本机hostname告知dhcp server。 而如果dhcp server支持该特性且跟dns server打通的话，会自动加入该host条目
>
> 而一般常见场景，dhcp server往往是local dns server

> 这个因为是在dhcp流程里的，所以要**立即让新host同步**到dhcp server/dns server的话，要重新发起dhcp流程。
>
> 比如： 重启网络服务、接口关闭+开启、人工出发dhcpclient操作等



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
    # 接口级别： 如 network - ethernets - ens33
    dhcp4-overrides:
    	route-mertic: 99  # 默认100
    ```

    要生效的话见netplan命令 tutorial。



### 增加路由

* 临时：`sudo ip route add default via 192.168.2.144 metric 30`

* 永久：

  * 低版本： `sudo nano /etc/network/interfaces`

    加入类似如下的条目

    ```sh
    up route add -net 192.168.40.0/24 gw 192.168.30.1 dev em1
    ```

  * 高版本： netplan方式

    `sudo nano /etc/netplan/01-netcfg.yaml`  （不一定是这个名字，根据实际情况确定配置文件）

    ```yaml
    # This file describes the network interfaces available on your system
    # For more information, see netplan(5).
    network:
      version: 2
      renderer: networkd
      ethernets:
        eno1:
          dhcp4: yes
    # 以上为已有内容样例，以下为加入内容样例
          routes:
          - to: 192.168.44.0/24
            via: 192.168.0.1
    ```

    然后：

    ```sh
    # 有必要的话先测试一下
    sudo netplan try
    
    sudo netplan apply
    ```

    



## tcp 



### kill conn

https://unix.stackexchange.com/questions/71940/killing-tcp-connection-in-linux

On linux kernel >= 4.9 you can use the `ss` command from iproute2 with key `-K`

```
ss -K dst 192.168.1.214 dport = 49029
```

the kernel have to be compiled with `CONFIG_INET_DIAG_DESTROY` option enabled.



以及



`tcpkill` might do it for you. In Ubuntu it is in the `dsniff` package.

Something like:

```
$ sudo tcpkill -i wlan0 host 192.168.1.214
```

(or some other `tcpdump` like expression for what connection to kill).

> This works only if the connection is transmitting anything. It will not work for hanged/idle TCP connections (see my answer for details) 



