
ref: 

* [Netplan —— 抽象网络配置生成器](https://learn-linux.readthedocs.io/zh_CN/latest/administration/network/netplan.html)

* [如何在 Linux 上使用网络配置工具 Netplan](https://linux.cn/article-10095-1.html?pr)

* [netplan.io/reference](https://netplan.io/reference/)





[Netplan](https://netplan.io/) —— **抽象网络配置生成器** ，是一个用于配置 Linux 网络的简单工具。 通过 Netplan ，你只需用一个 YAML 文件描述每个网络接口需要配置成啥样即可。 根据这个配置描述， Netplan 便可帮你生成所有需要的配置，不管你选用的底层管理工具是啥。

 [![../../_images/fc4aa5b7ef0aee13712fc03d0f1d6e4e.svg](https://learn-linux.readthedocs.io/zh_CN/latest/_images/fc4aa5b7ef0aee13712fc03d0f1d6e4e.svg)](https://learn-linux.readthedocs.io/zh_CN/latest/_images/fc4aa5b7ef0aee13712fc03d0f1d6e4e.svg) 

## 工作原理

Netplan 从 /etc/netplan/*.yaml 读取配置，配置可以是管理员或者系统安装人员配置； 也可以是云镜像或者其他操作系统部署设施自动生成。 在系统启动阶段早期， Netplan 在 /run 目录生成好配置文件并将设备控制权交给相关后台程序。

![4edd4dbf5fb460855d080f180f3138d6.svg](_pics/4edd4dbf5fb460855d080f180f3138d6.svg)



Netplan 工作原理

Netplan 目前支持以下两种 **网络管理工具** ：

- NetworkManager
- Systemd-networkd

一言以蔽之，从前你需要根据不同的管理工具编写网络配置，现在 Netplan 将管理工具差异性给屏蔽了。 你只需按照 Netplan 规范编写 YAML 配置，不管底层管理工具是啥，一份配置走天下！

## 使用指引

ref:

* `man netplan` 里很详细



### 配置

很显然，没有配置， Netplan 啥都做不了。 最简单有用的配置片段如下：

```yaml
network:
    version: 2
    renderer: NetworkManager
```

这个配置让 NetworkManager 管理所有网络设备 （默认，只要检测到以太网设备接线，便以 DHCP 模式启动该设备）。

使用 Systemd-networkd ，则不会自动启动网络设备； 每个需要启用的网卡均需要在 /etc/netplan 配置文件中指定配置。 网络配置示例如下：

```yaml
network:
    ethernets:
        enp0s3:
            addresses: []
            dhcp4: true
            optional: true
        enp0s8:
            addresses: [192.168.56.3/24]
            dhcp4: no
            optional: true
    version: 2
```

这个配置为 enp0s3 网卡开启 DHCP 自动获取地址； 为 enp0s8 网卡配置了一个静态 IP 192.168.56.3 ，掩码是 24 位。



```yaml
network:
    Version: 2
    Renderer: networkd
    ethernets:
       DEVICE_NAME:
          Dhcp4: yes/no
          Addresses: [IP/NETMASK]
          Gateway: GATEWAY
          Nameservers:
             Addresses: [NAMESERVER, NAMESERVER]
```

其中：

* `DEVICE_NAME` 是需要配置设备的实际名称。

* `yes`/`no` 代表是否启用 dhcp4。

* `IP` 是设备的 IP 地址。

* `NETMASK` 是 IP 地址的掩码。

* `GATEWAY` 是网关的地址。

* `NAMESERVER` 是由逗号分开的 DNS 服务器列表。

这是一份 .yaml 文件的样例：

```yaml
network:
    version: 2
    renderer: networkd
    ethernets:
       ens5:
       dhcp4: no
       addresses: [192.168.1.230/24]
       gateway4: 192.168.1.254
       nameservers:
          addresses: [8.8.4.4,8.8.8.8]
```


### 命令

netplan 操作命令提供两个子命令：

- netplan generate ：以 /etc/netplan 配置为管理工具生成配置；

  如 systemd 作为backend时，写入了 类似 `/run/systemd/network/10-netplan-ens33.network` 的文件，内容类似如下：

  ```
  [Match]
  Name=ens33
  
  [Network]
  DHCP=ipv4
  LinkLocalAddressing=ipv6
  
  [DHCP]
  RouteMetric=100
  UseMTU=true  
  ```

- netplan apply ：应用配置(以便生效)，必要时重启管理工具；

因此，调整 /etc/netplan 配置后，需要执行以下命令方能生效：

```sh
$ netplan apply
```


此外还有个`netplan try`用于测试配置正确性。

### 配置示例



#### link

##### status： up/down - 没有

似乎没有相关配置，也即link up/down不是netplan控制/配置而是额外操作



#### default route

* 可以dhcp override设置，影响dhcp下发的路由

  ```yaml
  dhcp4-overrides:
    use-routes: true  # 默认true，如果false则禁用dhcp下发的路由
    route-metric: 30  # 顾名思义
  ```

  

* 也可以额外配置

  ```yaml
      ens160:
        routes:
        - to: 0.0.0.0/0
          via: 192.168.2.144
          on-link: true
          metric: 50
  ```

  



#### DHCP

```yaml
network:
    ethernets:
        enp0s3:
            addresses: []
            dhcp4: true
    version: 2
```

#### 静态配置

```yaml
network:
    ethernets:
        enp0s8:
            addresses: [10.0.0.2/24]
            gateway4: 10.0.0.1
            nameservers:
                addresses: [8.8.8.8,8.8.4.4]
            dhcp4: no
    version: 2
```



#### DNS

```
       nameservers (mapping)
              Set DNS servers and search domains, for manual address configuration.  There are two supported  fields:
              addresses: is a list of IPv4 or IPv6 addresses similar to gateway*, and search: is a list of search do‐
              mains.

              Example:

                     ethernets:
                       id0:
                         [...]
                         nameservers:
                           search: [lab, home]
                           addresses: [8.8.8.8, "FEDC::1"]
```

文档里没说这里配置`addresses`后是和dhcp下发的merge还是override？ 





##### 配置”默认“dns

> 还有一个思路是： 配置一个独立于link的global dns，让它去匹配`~.`
>
> **但似乎netplan不支持**，可以单独配置在systemd-resolve的配置里： `/etc/systemd/resolved.conf`，详见《dns_systemd-resolved了解_yonka.md》



主要场景是： 多接口都能获取到dns server的话，怎么选用呢？

```yaml
    ens224:
      dhcp4: yes
      dhcp-identifier: mac
      nameservers:
        search: [~.]  # 这个指定match所有（但因为最短，所以优先级最低，也即是default branch）
        # 最后会和dhcp里告知的server的search一起merge为最终的`DNS Domain`结果
```



最后得到的：

```yaml
          DNS Domain: lan  # 这个来自dhcp
                      ~.  # 这个来自配置
```







# config from cloud-init



https://askubuntu.com/questions/1117496/how-do-i-use-cloud-init-to-apply-netplan



After I manually edit /etc/cloud/cloud.cfg.d/50-curtin-networking.cfg, how do I tell cloud-init to update /etc/netplan/50-cloud-init.yaml? In other words, it doesn't make sense to manually update both files; what's the standard process to re-apply the netplan?



The cloud-init confuguration files are not the place the change your network configuration after first boot, because the network configuration will only be generated once by cloud-init (on first boot).
You can however change the netplan config file directly. Then use the following commands as root to enable your changes. They will stick across boots.

```sh
netplan generate
netplan apply
```

Only the command 'cloud-init clean' will regenerate the 50...yaml file, but I dont think you want that. That would reset everything, including hostname en machine-id (resulting in a new max-address and a new dhcp ip address).





