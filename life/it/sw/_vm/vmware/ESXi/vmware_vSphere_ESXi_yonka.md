

## vm交互



### 剪切板



#### host/client -> vm

ref：

* [zhihu-VMware虚拟机下的linux复制粘贴](https://zhuanlan.zhihu.com/p/74011914)

* [esxi(vsphere):打开虚拟机和主机之间的copy/paste功能](https://blog.csdn.net/10km/article/details/53066118)
* [Enable content Copy/Paste between VMRC client and Windows/Linux Virtual Machine (57122)](https://kb.vmware.com/s/article/57122)



```
Name:                                 Value:
isolation.tools.copy.disable          FALSE
isolation.tools.paste.disable         FALSE
isolation.tools.setGUIOptions.enable  TRUE
```



```sh
# /etc/vmware/config
vmx.fullpath="/bin/vmx"
isolation.tools.copy.disable="FALSE"
isolation.tools.paste.disable="FALSE"
```





然而。。。 统统没用。。。

<del>怀疑可能是浏览器兼容性问题，或者终端</del>

试了windows也不行； 试了vmrc也不行。



可能还是哪里姿势不对。。。





#### vm之间



# hardware

## storage

见： [vmware_esxi_storage_yonka.md](vmware_esxi_storage_yonka.md)





## network

* esxi部分（主要单node内虚机网络） 见： [vmware_esxi_network_yonka.md](vmware_esxi_network_yonka.md)

* vSphere部分（主要esxi集群内网络互通、分布式虚拟网络） 见： [vmware_vSphere_network_yonka.md](vmware_vSphere_network_yonka.md)

