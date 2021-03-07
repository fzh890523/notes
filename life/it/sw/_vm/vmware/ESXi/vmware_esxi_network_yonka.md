ref：

* vsphere-networking-basics
  * [part-1: vSS](https://www.altaro.com/vmware/vsphere-networking-basics-part-1/) 标准switch
  * [part-2: vDS](https://www.altaro.com/vmware/vsphere-networking-basics-part-2/) 分布式switch



# 概念



## 拓扑



* 以vs为中心

* vs可以连接多个uplink

  * 可以是独立的
  * 也可以有部分是成组做成链路聚合的

* vs可以连接多个port group

* 每个port group可以有多个“同样属性”的ports（包括vlan id，这个配在pg上）

* port必须在pg里，哪怕pg里只有一个port

* vm的网卡连接到的“目标”只能是pg

* vs之间似乎没有办法相连，哪怕vlan打通也不行

  那么等同于两个vs存在于两个L2网络里

  只能通过vm多网卡+开启路由来在L3联通两个网络（vs）吧

* esxi的虚拟网络机制里没有L3部分，无论是路由还是dhcp，需要用vm等来做

  也即只能接入、组成不同的L2网络：

  * vs + pgs + vm nics组成内部虚拟网络
  * vs + pg + 物理nic 接入外部网络
  * vs + pg + vlan + 物理nic 接入外部【某个】网络
  * ...



## port group

* 概念上其实是方便批量管理 **同样属性**的端口（们）
  * 策略
  * vlan
  * ...
* 形象的话，勉强能理解为一个连接到vs上某port的纯二层交换机

> Port groups allow us to logically carve up our virtual ports that are available on a particular vSwitch. We can apply traffic policy rules at the port group level – security rules and traffic shaping. Port groups are where we can also assign VLANs to our traffic. Below, take note of a configured port group called “TestPortGroup”.



## virtual switch



> A vSwitch is exactly that – a virtual switch. Though it is not a hardware device, the virtual switch passes traffic using the same concepts found in the OSI networking model as physical switches do. In other words, layer 2 frames (OSI data link layer) enter and exit the vSwitch much as they would a physical switch.
>
> The difference in the vSwitch and the physical switch networking in the VMware world is that the vSwitch does not keep a table of MAC addresses for devices that it does not directly know about as does a physical switch. It only knows and cares about the MAC addresses for VMs and VMkernel port devices that are directly connected to the virtual switch. The vSwitch will not flood unknown frames out all ports. Unknown frames are simply dropped by the vSwitch. Unknown MAC address destinations originating from a VM connected to the vSwitch are sent to the vSwitch physical uplink.







### 标准交换机 vSS

* 基本上对标 ”虚拟的“ **实体二层交换机**
* 不过部分功能拆分到port group了
  * vlan在pg级别配置 - pg视为vs的一个port

* 说了二层，所以没三层功能了，无论是管理ip还是dhcp

* 可以配置switch级别策略，pg级别可以单独配置（覆盖）或者沿用switch级别的

### 分布式交换机 vDS

The Distributed vSwitch requires Enterprise Plus licensing to be made available and is configured at the vCenter level



### virtual port

> Virtual ports on the vSwitch act like the ports on a physical switch. Each VM that is configured with a virtual network adapter uses one of these virtual ports on the vSwitch. Also, there are VMkernel ports, which are a special kind of ports that talk directly to the ESXi host’s VMkernel and are required to make storage, vMotion, and management network traffic possible.

应该只是概念而非实体，所以没有可配置的地方



### physical uplinks

交互里就叫`uplink`，完整名字更好理解，就是指直连的物理网卡。

因为对于esxi来说，所有物理网卡都是连向外部/upstream的。

>Virtual Switch traffic is uplinked on each ESXi host by the physical network cards in the ESXi host that are bound to that vSwitch in VMware. This provides the traffic path for traffic entering and exiting the vSwitch. The real physical interface on the ESXi host is mapped to a vmnic in ESXi. These mappings can be seen when managing the networking adapters on the ESXi host.



* esxi网络模型里，把物理网卡整个连接到vs

  没有对物理网卡做vlan的细分，而是所有流量导到vs里再转发

  **所以一个uplink只能连接到一个vs**

* 一个vs可以连接到多个uplink
* 多个uplink可以是独立的，也可以是做链路聚合模型存在（nic teaming）



## vlan/network label/tag 

> The VLAN ID restricts port group traffic to a logical Ethernet segment within the physical network. 
>
> Port groups may have a VLAN ID of 0 to 4095. VLAN IDs that have VLAN ID 4095 are able reach other port groups located on other VLANs. Basically, **VLAN ID 4095 specifies that the port group should use trunk mode or VGT mode**, which allows the guest operating system to manage its own VLAN tags. Guest operating systems typically do not manage their VLAN membership on networks. VLAN 1001 through 1024 are Cisco reserved VLANs. VLANs 1, 1001 to 1024, and 4095 will be not be used for virtual switch port groups since they may cause an unexpected operation.



从vm - pg/vs - 物理端口/外部交换机，分别有几种：

> esxi对物理网卡的配置似乎就是trunk，没看到地方能改，估计不能改

* vm自己确定tag - VGT

  * vm确定tag

  * pg vlan配置为`4095`（`1111 1111 1111`）

    应该是通配的意思，此时pg估计trunk mode

  * pg/vs出流量带tag，物理口trunk转发
  * 物理sw侧根据端口配置接收转发到对应vlan

  > 物理交换机端口 需要配置为trunk

* pg确定tag - VST

  * vm -untagged流量-> pg
  * pg根据配置打好tag -vs-> 物理口
  * 物理口trunk转发
  * ...

  > 物理交换机端口 需要配置为trunk

* 外部确定 - EST

  * vm -untagged流量 -> pg/vs -> 物理口 -> 物理sw
  * 物理sw根据端口配置进行转发

  > 物理交换机端口 可以配置为access； 要配为trunk似乎也可以，只是这时走的是PVID

详见： [VMware ESX Server 3 802.1Q VLAN Solutions white paper.pdf](_resource/VMware ESX Server 3 802.1Q VLAN Solutions white paper.pdf) 和 [VLAN configuration on virtual switches, physical switches, and virtual machines (1003806).md](_resource/VLAN configuration on virtual switches, physical switches, and virtual machines (1003806).md)



### vlan背景知识

见： 《network_vlan_yonka.md》