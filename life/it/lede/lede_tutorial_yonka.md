
## 了解

### 网络

* eth0

  默认静态ip，需要按需配置

* br-lan

  虚拟设备，用于LAN口设备桥接

### 服务

## 配置、使用

参见： https://openwrt.org/docs/guide-user/start

### startup

1. 网络连通

  默认的`192.168.1.1`基本不可用，可以根据需要修改为dhcp或者其他静态地址。
  我这边因为这个接口后面做其他用途，所以先不改这个。

  1. 新增网卡

  2. 启用 `ip link set eth1 up`

  3. 改为dhcp

    ```sh
    # vim /etc/config/network
    # add content below:
    
    config interface 'nat'
            option type 'bridge'
            option ifname 'eth1'
            option proto 'dhcp'
            
    config interface 'brg'
            option type 'bridge'
            option ifname 'eth2'
            option proto 'dhcp'        
    ```

  4. 重启网络 `/etc/init.d/network restart`

2. 安装luci

  ```sh
  opkg update
  opkg install luci
  /etc/init.d/uhttpd start
  /etc/init.d/uhttpd enable  
  ```

  然后通过ip访问（uhttpd默认启动、监听在80）



### 自定义hostname

在`network-hostnames`里配置，但apply好像不生效，在`system-startup`里把dnsmasq restart了就生效了，晕。



## 安装

### vmware下安装

https://openwrt.org/docs/guide-user/virtualization/vmware

```sh
yum -y install qemu-img
wget https://downloads.openwrt.org/chaos_calmer/15.05/x86/64/openwrt-15.05-x86-64-combined-ext4.img.gz
gunzip openwrt-15.05-x86-64-combined-ext4.img.gz
qemu-img convert -f raw -O vmdk openwrt-15.05-x86-64-combined-ext4.img openwrt-15.05-x86-64-combined-ext4.vmdk

# for macos
brew install qemu
qemu-img convert -f raw -O vmdk ~/Downloads/openwrt-15.05-x86-64-combined-ext4.img openwrt-15.05-x86-64-combined-ext4.vmdk
```

把转换出来vmdk文件作为磁盘文件挂载即可。
详见链接。



### 升级安装

https://openwrt.org/docs/guide-user/installation/openwrt_x86



`https://downloads.openwrt.org/releases/<version>/targets/x86/64` 下载 名如 `https://downloads.openwrt.org/releases/19.07.7/targets/x86/64/openwrt-19.07.7-x86-64-combined-squashfs.img.gz` 的那个



 

