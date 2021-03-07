# Sample Configuration - ESX/ESXi connecting to physical switch via VLAN access mode and External Switch VLAN Tagging (EST Mode) (1004127)



**Last Updated:** 2014/11/9**Categories:** How to**Total Views:** 142628 6**Language:**           Chinese (Simplified)JapaneseEnglish                 [subscribe](javascript:void(0);)



 Purpose

This article describes connecting ESXi/ESX to Cisco IOS switches VLAN port.

 Resolution

### VLAN Access and Layer 2 switching

- Physical LAN switch is configured for Virtual LAN (VLAN) segmentation.
- ESXi/ESX simply connects to a switch port that belongs to a VLAN at physical switch level.
- Cisco defines a VLAN as a broadcast domain within a switched network.
- VLANs allow you to segment your switched network so that broadcast domains are smaller, leaving more bandwidth for your end nodes.
- Devices that are in one VLAN do not receive broadcasts from devices in another VLAN.
- For devices on different VLANs to communicate, a layer 3 device (usually a router) must be used.
- VLAN configuration is recommended for securing network traffic.

### Virtual Switch (ESX/ESXi) configuration to connect to VLAN switch port

- No VLAN configuration is required on ESXi/ESX side.
- Set ESX port group to belong to VLAN 0.
- Value zero as VLAN means that the vSwitch is not VLAN tagging the ESXi/ESX packets.
- Virtual switch NIC teaming policy set to route based on originating virtual port ID by default depending if ether-channel is enabled on switch port.
- If Ether-channel is configured on Cisco switch port virtual switch NIC teaming policy is set to route base on IP HASH.

### Physical Switch Configuration (Cisco, HP, DELL, etc)

- Physical switch Mode Access (Layer2)
- Switch port access VLAN tagging (ID)
- Define VLAN interface
- Assign IP range to VLAN interface
- VLAN routing may be required or VLAN Isolation – (refer to VLAN layer 3 routing Article)

These commands are applied on Cisco IOS to configure switch port for VLAN access:

> interface GigabitEthernet1/15
>
> > switchport ( Configures the LAN port for Layer 2 switching)
> >
> > switchport access vlan vlan_ID ( The value can be 1 through 4094, except reserved VLANs)
> > switchport mode access ( Configures the port to be an access port to prevent trunk negotiation delays)
> >
> > spanning-tree portfast ( Configure port-fast for initial STP delay)

This image is a sample EST topology:

![img](_pics/rtaImage_EST)

### Configuration within the vSphere/VMware Infrastructure Client or vSphere Web Client

To configure this within the vSphere/VMware Infrastructure Client:

1. Highlight the ESXi/ESX Server host.

2. Click **Configuration** > **Networking** > **Properties**.

3. Highlight the virtual switch in the **Ports** tab.

4. Click **Edit**.

5. Click the **General** tab.

6. Type a value of **0** or **NONE** in the VLAN ID field. This indicates that VLAN Tagging is off.

7. Click the **NIC Teaming** tab.

8. From the **Load Balancing** dropdown, choose **Route based on originating virtual port ID**

9. Verify that there is at least one network adapter listed under ActiveAdapters

   

To configure within the vSphere Web Client.

1. In the vSphere Web Client, navigate to the host.
2. On **Manage** tab, click **Networking**
3. Click **Virtual Switches** in the left panel and click the virtual switch that requires a change in its load balancing policy.
4. Click **Edit Setting** > **Teaming and Failover**.
5. From the Load Balancing dropdown, select **Route based on originating virtual port ID**.
6. Click **OK**.


To perform VLAN configuration and verification via the command line:

- Run the command:

  esxcfg-vswitch –l

  The results appear similar to:

  Switch Name Num Ports Used Ports Configured Ports MTU Uplinks

  vSwitch0 64 5 64 1500 vmnic3,vmnic1

  
  PortGroup Name VLAN ID Used Ports Uplinks

  VM Network **0** 0 vmnic1,vmnic3
  Service Console **0** 1 vmnic1,vmnic3

  
  Switch Name Num Ports Used Ports Configured Ports MTU Uplinks

  vSwitch2 64 8 64 1500 vmnic2,vmnic0

  PortGroup Name VLAN ID Used Ports Uplinks

  VMkernel **0** 1 vmnic0,vmnic2

  Service Console 2 **0** 1 vmnic0,vmnic2

  Production **0** 2 vmnic0,vmnic2

> 

- If the VLAN ID is missing or incorrect, correct the VLAN ID value with the command:

  esxcfg-vswitch -v *VLAN* -p “Service Console” vSwitch0

