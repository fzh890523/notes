

* vSphere
  [VMware vSphere 7.0](https://my.vmware.com/cn/web/vmware/info/slug/datacenter_cloud_infrastructure/vmware_vsphere/7_0)

  * Essentials

    * `VMware vSphere Hypervisor (ESXi) 7.0.0`
    * `VMware vCenter Server 7.0.0a`

  * Essentials Plus

    +

    * `VMware vSphere Replication 8.3.0.2`

  * Standard

    + 

    * `VMware vRealize Orchestrator Appliance 8.1.0`

  * Enterprise

    好像组件和standard一样

  * Enterprise Plus

    + 

    * `VMware vRealize Operations 8.1.0`
    * `VMware vSphere Integrated Containers 1.5.5`

  * Desktop

    组件也和standard一样

  * vSphere Scale-Out

    ...

* vSphere CLI

  a composite package of tools and libraries. We need to modernize our packaging to individual tool installers, and move more toward API centric tools.

  Starting with vSphere 7.0, vSphere CLI package will not be released and it will be end of life, but all existing capabilities of vSphere CLI will be supported with more API centric tools such as esxcli and Perl SDK.

  ref: 
  
  * [Announcing End of Life of vSphere CLI package in vSphere 7.0 (78473)](https://kb.vmware.com/s/article/78473)

  * [vSphere CLI 6.7](https://code.vmware.com/web/tool/6.7/vsphere-cli)

    下载链接 和 文档

* esxcli

  Allows for remote management of ESXi hosts using the ESXCLI command set. You can download the esxcli tool compatible with vSphere 7.0 from VMware Code page.