# Sample configuration of virtual switch VLAN tagging (VST Mode) (1004074)

**Last Updated:** 2021/1/15**Categories:** How to**Total Views:** 640721 32**Language:**           Chinese (Simplified)Portuguese (European)JapaneseSpanishEnglish                 [subscribe](javascript:void(0);)



 Purpose

This article provides a sample network configuration for isolation and segmentation of virtual machine network traffic.

 Resolution

To configure Virtual Switch (vSwitch) VLAN Tagging (VST) on an ESXi/ESX host:

1. Assign a VLAN to a portgroup(s). The supported VLAN range is 1-4094.

   Reserved VLAN IDs:

   - VLAN ID 0 (zero) Disables VLAN tagging on port group (EST Mode)

   - VLAN ID 4095 Enables trunking on port group (VGT Mode)
      

2. Set the switch NIC teaming policy to Route based on originating virtual port ID (this is set by default).


To configure the physical switch settings:

1. Define ESXi/ESX VLANs on the physical switch.

2. Allow the proper range to the ESXi/ESX host.

3. Set the physical port connection between the ESXi/ESX host and the physical switch to TRUNK mode. ESXi/ESX only supports IEEE 802.1Q (dot1q) trunking.

   - Physical switch is set to TRUNK mode

   - dot1q encapsulation is enabled

   - Spanning-tree is set to portfast trunk (for example, port forwarding, skips other modes)

   - Define VLAN interface

   - Assign IP Range to VLAN interface

   - VLAN Routing – and VLAN Isolation

     **Caution**: Native VLAN ID on ESXi/ESX VST Mode is not supported. Do not assign a VLAN to a port group that is same as the native VLAN ID of the physical switch. Native VLAN packets are not tagged with the VLAN ID on the outgoing traffic toward the ESXi/ESX host. Therefore, if the ESXi/ESX host is set to VST mode, it drops the packets that are lacking a VLAN tag.


This sample is a supported Cisco Trunk Port configuration:

> `interface GigabitEthernet1/2`
> `switchport `(Set to layer 2 switching)
> `switchport trunk encapsulation dot1q `(ESXi/ESX only supports dot1q, not ISL)
> `switchport trunk allowed vlan 10-100 `(Allowed VLAN to ESXi/ESX. Ensure ESXi/ESX VLANs are allowed)
> `switchport mode trunk `(Set to Trunk Mode)
> `switchport nonegotiate `(DTP is not supported)
> `no ip address`
> `no cdp enable `(ESXi/ESX 3.5 or higher supports CDP)
> `spanning-tree portfast trunk `(Allows the port to start forwarding packets immediately on linkup)
>
> **Note**: For more information on configuring your physical network switch, contact your switch vendor.


To assign a VLAN to a port group, there must be a corresponding VLAN interface for each VLAN on a physical switch with a designated IP range.

> For example:
>
> `interface Vlan200`
> `ip address 10.10.100.1 255.255.255.0` (This IP can be used as VLAN 200 Gateway IP)
>
> **Note**: When the VLAN ID is defined on the physical switch, it can be configured for ESX. If the IP range is assigned to a VLAN, decide if any routing may be required to reach other nodes on the network.


To configure a VLAN on the portgroup using the VMware Infrastructure/vSphere Client:

1. Click the ESXi/ESX host.

2. Click the **Configuration** tab.

3. Click the **Networking** link.

4. Click **Properties**.

5. Click the virtual switch / portgroups in the **Ports** tab and click **Edit**.

6. Click the **General** tab.

7. Assign a VLAN number in VLAN ID (optional).

8. Click the **NIC Teaming** tab.

9. From the **Load Balancing** dropdown, choose **Route based on originating virtual port ID**.

10. Verify that there is at least one network adapter listed under Active Adapters.

11. Verify the VST configuration using the ping command to confirm the connection between the ESXi/ESX host and the gateway interfaces and another host on the same VLAN.

    **Note:** For additional information on VLAN configuration of a VirtualSwitch (vSwitch) port group, see [Configuring a VLAN on a portgroup (1003825)](https://kb.vmware.com/s/article/1003825). and [VLAN configuration on virtual switches, physical switches, and virtual machines](https://kb.vmware.com/s/article/1003806) (1003806)

To configure via the command line:

> ```
> esxcfg-vswitch -p "*portgroup_name*" -v *VLAN_ID virtual_switch_name*
> ```


**Note**: The illustration attached to this article is a sample VST mode topology and configuration with two ESXi/ESX hosts, each with two NICs connecting to the Cisco switch.

 Related Information

For related information, see the [VMware ESX Server 3 802.1Q VLAN Solutions](http://www.vmware.com/pdf/esx3_vlan_wp.pdf) white paper or [local](VMware ESX Server 3 802.1Q VLAN Solutions white paper.pdf).



