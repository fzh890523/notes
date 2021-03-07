# Sample configuration of Windows virtual machine VLAN Tagging (VGT Mode) in ESX (1004252)



**Last Updated:** 2020/2/17**Categories:** How to**Total Views:** 201724 6**Language:**           Chinese (Simplified)JapaneseSpanishEnglish                 [subscribe](javascript:void(0);)



 Purpose

This article provides a sample configuration of a VLAN tagging at the virtual machine level.

 Resolution

**Overview**

- 802.1Q VLAN trunking driver is required inside the virtual machine
- 64bit Windows guest operating system automatically loads the 802.1q E1000 driver
- 32bit guest Operating Systems require manual configuration of the VMX file to point to the E1000 driver
- Physical switch is set to trunk mode by using VLAN ID 4095
- Windows: Only 64-bit Windows ships with the E1000 Drivers

**Configuration of VirtualSwitch (vSwitch)**

To set a standard vSwitch portgroup to trunk mode:

1. Edit host networking via the Virtual infrastructure Client.
2. Navigate to **Host** > **Configuration** > **Networking** > **vSwitch** > **Properties**.
3. Click **Ports** > **Portgroup** > **Edit**.
4. Click the **General** tab.
5. Set the VLAN ID to 4095. A VLAN ID of 4095 represents all trunked VLANs.
6. Click OK.

To set a distributed vSwitch portgroup to trunk mode:

1. Edit host networking via the Virtual infrastructure Client.

2. Navigate to **Home** > **Inventory** > **Networking**.

3. Right-click on the dvPortGroup and select **Edit Settings**.

4. Within that dvPortGroup, go to **Policies > VLAN**.

5. Set VLAN type to **VLAN Trunking** and specify a range of VLANs or specificy a list of VLANs to be passed to the Virtual machines connected to this portgroup.

   **Note:** To improve security, virtual Distributed Switches allow you to specify a range or selection of VLANs to trunk rather than allowing all VLANS via VLAN 4095.

**Configuration of Windows TCP/IP**

To configure the guest operating system for VGT:
 

1. Download the [e1000 NIC drivers](http://www.intel.com/support/network/sb/cs-006120.htm) from the Intel website into the 32-bit Windows virtual machine.

   *Note: The preceding link was correct as of September 15, 2015. If you find the link is broken, provide feedback and a VMware employee will update the link.*
    

2. Power off the virtual machine.

3. Configure the virtual machine to use the e1000 virtual NIC. Enter a new line (or replace the existing virtual NIC) in the .vmx file of the virtual machine:

   Ethernet*n*.virtualDev = "e1000"

   Replace *n* with the number of the Ethernet adapter. For example, the entry for the first Ethernet adapter that has number 0 is:

   Ethernet0.virtualDev = "e1000"
    

4. Power on the virtual machine.

5. Configure the e1000 network connection.

![img](_pics/rtaImage_VGT)


To install the driver manually within a Windows 2008 R2 guest operating system:

1. Download ProWinx64 from Intel located here [Network Adapter Driver for Windows](http://downloadmirror.intel.com/18718/eng/PROWinx64.exe)

   **Note**: Intel has moved some drivers to historical status, if the above link does not have the correct driver, see this link for specific driver versions:

   http://www.intel.com/support/network/sb/CS-023752.htm?wapkw=e1g6032e

   ***Note**: The preceding link was correct as of September 15, 2015. If you find the link is broken, provide feedback and a VMware employee will update the link.*
    

2. Run this command to extract the Intel drivers downloaded to the temp folder:

   Prowinx64.exe /s /e /f "C:\temp

   If 7zip is installed, run this as alternate command on Windows PowerShell

   Get-ChildItem "*.exe" | foreach { C:\Windows\System32\7z.exe e -y $_.fullname
    

3. Right-click the network adapter and click **Update Driver Software**.

4. Click **Browse my computer for driver software**.

5. Click **Let me pick from a list of device drivers on my computer**.

6. Click **Have Disk**.

7. Click **Browse**.

8. Browse to C:\temp\pro1000\winx64\ndis61\e1g6032e.inf.

9. Click **Next** to install the driver.

10. Repeat Steps 2-8 for each network adapter you have for the virtual machine.

11. After all the adapters are updated, run the Intel setup program. You should now be able to install the advanced network services software with VLANs.

**Notes**:

- You can also find the instructions in the manual/readme file for the driver.
- The Intel driver download contains ProWin64.exe. This is not a separate install.
- This process applies to the vmxnet3 driver as well. However to have multiple vlans you need to use multiple vmxnet3 NIC's.

 Related Information

You can set the VLAN ID in the vmxnet3 driver under Advanced setting of the NIC card. Set the vlan driver in Windows to 4095 when setting multiple specific vLANs in the vDS.

When enabling VGT by setting a VLAN range in vDS or VLAN 4095 in vSS, have one VMXNET3 VNIC per VLAN to use within the guest. The real benefit of using VGT is that the networking is configured entirely from within the guest OS. Where you can switch between physical networks based on VLAN tag and do not need to engage a vSphere or Network administrator to reconfigure the vSwitch or the virtual machine. This is beneficial for situations when you do not want to give someone vCenter Server access, but allow them to reconfigure the guest networking across multiple networks as required.

**Note**: Specify VLANS on port 4095 within the Cisco Nexux 1000v.
**Note**: Some Windows machines can strip VLAN tags unless Monitor mode is enabled for the guest OS NIC (VLAN pass-thru).

