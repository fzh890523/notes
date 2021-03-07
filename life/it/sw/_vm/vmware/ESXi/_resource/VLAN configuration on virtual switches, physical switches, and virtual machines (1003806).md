# VLAN configuration on virtual switches, physical switches, and virtual machines (1003806)



**Last Updated:** 2021/1/22**Categories:** Troubleshooting**Total Views:** 420311 40**Language:**           Chinese (Simplified)Portuguese (European)JapaneseEnglish                 [subscribe](javascript:void(0);)

**Skyline Health Diagnostics (SHD)** is a tool that analyses log files to detect known issues and recommend solutions/KBs. **Benefits include:**

- Instant resolution to known issues that occur in your vCenter/vSphere environment
- Significant reduction of time and efforts in resolving issues faced by Admins
- Stability of your vSphere environments

It is highly secure and doesn’t require online connectivity. Please visit[ here ](https://blogs.vmware.com/vsphere/2020/09/introducing-vmware-skyline-health-diagnostic-tool.html)for more details on SHD, including downloading and installation of this tool.

 Purpose

This article describes the various VLAN tagging methods used with ESXi/ESX.

Virtual LAN (VLAN) implementation is recommended in ESXi/ESX networking environments because:

- It integrates ESXi/ESX into a pre-existing network
- It secures network traffic
- It reduces network traffic congestion
- iSCSI traffic requires an isolated network

 Resolution

### Video tutorial for various VLAN tagging methods used with ESXi/ESX:

##   VLAN tagging methods used with ESXi/ESX:

There are three methods of VLAN tagging that can be configured on ESXi/ESX:

- [External Switch Tagging (EST)](https://kb.vmware.com/s/article/1003806#estPoints)
- [Virtual Switch Tagging (VST)](https://kb.vmware.com/s/article/1003806#vstPoints)
- [Virtual Guest Tagging (VGT)](https://kb.vmware.com/s/article/1003806#vgtPoints)

### External Switch Tagging (EST)

- All VLAN tagging of packets is performed on the physical switch.

- ESXi/ESX host network adapters are connected to access ports on the physical switch.

- The portgroups connected to the virtual switch must have their VLAN ID set to 0.

- For more information, see [Sample Configuration - ESXi/ESX connecting to physical switch via VLAN access mode and External Switch VLAN Tagging (EST Mode) (1004127)](https://kb.vmware.com/s/article/1004127).

- See this example snippet of a code from a Cisco switch port configuration:

  `switchport mode accessswitchport access vlan x`

### Virtual Switch Tagging (VST)

- All VLAN tagging of packets is performed by the virtual switch before leaving the ESXi/ESX host.

- The ESXi/ESX host network adapters must be connected to trunk ports on the physical switch.

- The portgroups connected to the virtual switch must have an appropriate VLAN ID specified.

- For more information, see:

  - [Configuring a VLAN on a portgroup (1003825)](https://kb.vmware.com/s/article/1003825)
  - [Configuring Virtual Switch VLAN Tagging (VST) mode on a vNetwork Distributed Switch (1010778)](https://kb.vmware.com/s/article/1010778)

- For a sample of VST, see [Sample configuration of virtual switch VLAN tagging (VST Mode) (1004074)](https://kb.vmware.com/s/article/1004074).

- See this example snippet of code from a Cisco switch port configuration:

  `switchport trunk encapsulation dot1qswitchport mode trunkswitchport trunk allowed vlan x,y,zspanning-tree portfast trunk`

**Note**: The Native VLAN is not tagged and thus requires no VLAN ID to be set on the ESXi/ESX portgroup.

### Virtual Guest Tagging (VGT)

- All VLAN tagging is performed by the virtual machine.

- You must install an 802.1Q VLAN trunking driver inside the virtual machine.

- VLAN tags are preserved between the virtual machine networking stack and external switch when frames are passed to/from virtual switches.

- Physical switch ports are set to trunk port.

- For more information, see [Sample configuration of virtual machine (VM) VLAN Tagging (VGT Mode) in ESX (1004252)](https://kb.vmware.com/s/article/1004252).

- See this example snippet of code from a Cisco switch port configuration:

  `switchport trunk encapsulation dot1qswitchport mode trunkswitchport trunk allowed vlan x,y,zspanning-tree portfast trunk`

For additional information on these configurations, see [VMware ESX Server 3: 802.1Q VLAN Solutions](http://www.vmware.com/pdf/esx3_vlan_wp.pdf).

