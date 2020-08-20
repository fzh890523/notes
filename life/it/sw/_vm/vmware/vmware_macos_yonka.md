

# 避坑



## 硬件相关



* 频繁usb连接/断开容易造成虚机坏掉

  看日志好像是需要先在os里unmount再。。。





# ref



## [vmware文档： 安装macOS 10.12](http://partnerweb.vmware.com/GOSIG/MacOS_10_12.html#installation1)

**就是这篇坑了我，当时没看仔细**

如下面说的：

* 对于vmware exsi，需要先安装10.8然后再升级到10.12。 安装10.8可以用`InstallESD.dmg`来boot

  > 实际分别用10.15的`install.app`和`InstallEDS.dmg`，甚至官方下载的update.dmg都试了下，都不能boot

* 对于vmware fusion，把`install <version>.app`拖进去创建vm的时候，会build出一个磁盘文件（`Temporary Installation Source Disk.vmdk`和`Temporary Installation Source Disk.dmg`）然后添加一个磁盘设备来挂载，然后boot with it然后启动安装



**（估计是10.8之后的）app中的`InstallESD.dmg`是non-bootable的，需要特殊build**

后来实际用的方式是：

1. 用fusion（的创建vm流程来）build一份

   在路径`${HOME}/Virtual Machines.localized/macOS 10.15.vmwarevm/`下

   > 这个build也许也有其他方式，参见其他ref

2. 把文件（.vmdk和.dmg）copy出来使用，添加为实际创建的虚拟机的磁盘

   > 文件名写到vmdk内容里了，如果修改的话要一起改内容

3. 启动，继续流程





The macOS 10.12 documentation covers information on how to install the operating system in a virtual machine.

The macOS 10.12 documentation includes the following topics:

- [Installation Instructions for VMware ESXi 6.0](http://partnerweb.vmware.com/GOSIG/MacOS_10_12.html#installation1)
- [Installation Instructions for VMware Fusion 7 and Later](http://partnerweb.vmware.com/GOSIG/MacOS_10_12.html#installation2)
- [VMware Tools in an macOS 10.12 Guest](http://partnerweb.vmware.com/GOSIG/MacOS_10_12.html#Tools)
- [Knowledge Base Articles for macOS 10.12](http://partnerweb.vmware.com/GOSIG/MacOS_10_12.html#knownissues)
- [VMware Compatibility Guide](http://partnerweb.vmware.com/GOSIG/MacOS_10_12.html#VMCG)



You install OS X 10.8 in a virtual machine using the installation media, then upgrade the virtual machine to macOS 10.12.

### Prerequisites

Before you begin, verify that the following tasks are complete:

- Read [General Installation Instructions for All VMware Products](http://partnerweb.vmware.com/GOSIG/general_instructions.html).
- You already have **Install OS X Mountain Lion.app** for any version of OS X 10.8.
- Ensure your physical system is an Apple-labeled computer. This is required to install or run OS X 10.8, OS X 10.9, OS X 10.10, OS X 10.11 or macOS 10.12 in a virtual machine.
- Ensure your physical system can support an additional virtual machine with at least 2GB of RAM.

### Installation Steps

1. Create a new virtual machine and select the **Apple Mac OS X 10.12 (64-bit)** option. If Apple Mac OS X 10.12 (64-bit) is not available, select the most recent version of OS X supported by your product.
2. Use the virtual machine CD/DVD drive for installation.
3. Connect the CD/DVD drive to a datastore image file, complete the following steps:
   1. Add **Install OS X Mountain Lion.app** to a datastore that the virtual machine can access.
   2. Set the CD/DVD drive to connect to the **Install OS X Mountain Lion.app/Contents/SharedSupport/InstallESD.dmg** image file.
4. Power on the virtual machine.
5. Select your language and click the right arrow.
6. Select **Disk Utility** and click **Continue**.
7. In the left hand pane, select the hard disk at the top of the list.
8. Select the **Erase** tab.
9. Click the **Erase...** button and click the confirming **Erase** button. Wait for the process to complete.
10. Quit the **Disk Utility** application.
11. Select **Reinstall OS X** and click **Continue**.
12. Follow the prompts to complete the installation.
13. Download **Install macOS Sierra.app** in the virtual machine using the Mac App Store, or download it onto another Mac using the Mac App Store, and copy **Install macOS Sierra.app** to the virtual machine.
14. Execute **Install macOS Sierra.app** in the virtual machine to update the operating system to macOS 10.12.
15. Install VMware Tools.



You install macOS 10.12 in a virtual machine using the installation media.

### Prerequisites

Before you begin, verify that the following tasks are complete:

- Read [General Installation Instructions for All VMware Products](http://partnerweb.vmware.com/GOSIG/general_instructions.html).
- Download **Install macOS Sierra.app** from the Mac App Store.
- Ensure your physical system is an Apple-labeled computer. This is required to install or run macOS 10.12 in a virtual machine.
- Ensure your physical system can support an additional virtual machine with at least 2GB of RAM.

### Installation Steps

1. Start VMware Fusion.
2. Select **File > New**.
3. Drag **Install macOS Sierra.app** and drop it into the **Create a Virtual Machine** window.
4. Click **Continue**.
5. Follow the prompts to complete the installation.
6. Install VMware Tools.



## 制作macos安装镜像



### 制作Mojave的安装镜像



Here are the steps to convert that *Install macOS Mojave.app*

* Open Terminal on your Mac and enter the command below:

  ```sh
  hdiutil create -o /tmp/Mojave.cdr -size 12000m -layout SPUD -fs HFS+J
  ```

* This is going to create a temporary virtual disk file in your /tmp directory. Once its done you can attach and mount the virtual disk:

  ```sh
  hdiutil attach /tmp/Mojave.cdr.dmg -noverify -mountpoint /Volumes/install_build
  ```

* Now we’re ready to extract and create an installable image from the Mojave download. Enter the command shown below to extract an installable image onto the mounted virtual disk. The command needs to run elevated so it is going to prompt you for credentials. After you’ve entered your credentials it is going to ask you to confirm, confirm with “Y” and “Enter”:

  ```sh
  sudo /Applications/Install\ macOS\ Mojave.app/Contents/Resources/createinstallmedia --volume /Volumes/install_build
  ```

* Next we’re going to move the contents of the virtual disk into a .dmg file on your desktop:

  ```sh
  mv -v /tmp/Mojave.cdr.dmg ~/Desktop/InstallSystem.dmg
  ```

* Now we’re almost ready. Next detach the virtual disk (installer):

  ```sh
  hdiutil detach /Volumes/Install\ macOS\ Mojave
  ```

* One of the last steps we have to take is to convert the newly created “InstallSystem.dmg” into a .iso file, enter the command below to achieve this:

  ```sh
  hdiutil convert ~/Desktop/InstallSystem.dmg -format UDTO -o ~/Desktop/Mojave.iso
  ```

  



Lastly we need to change the filetype of the file “Mojave.iso.cdr” on your desktop. Select the file and hit enter, now rename the file to “Mojave.iso” it is going to prompt you if you are sure; select “Use .iso” and we’re done!

Now you can proceed to boot and install macOS Mojave in your virtual machine.

Here's my Terminal.app output when creating my .ISO:



```sh
Last login: Fri Apr 12 12:41:55 on console
macmini:~ andy$ hdiutil create -o /tmp/Mojave.cdr -size 12000m -layout SPUD -fs HFS+J
created: /tmp/Mojave.cdr.dmg
macmini:~ andy$ hdiutil attach /tmp/Mojave.cdr.dmg -noverify -mountpoint /Volumes/install_build
/dev/disk3              Apple_partition_scheme        
/dev/disk3s1            Apple_partition_map            
/dev/disk3s2            Apple_HFS                          /Volumes/install_build
macmini:~ andy$ sudo /Applications/Install\ macOS\ Mojave.app/Contents/Resources/createinstallmedia --volume /Volumes/install_build
Password:
Ready to start.
To continue we need to erase the volume at /Volumes/install_build.
If you wish to continue type (Y) then press return: Y
Erasing disk: 0%... 10%... 20%... 30%... 100%
Copying to disk: 0%... 10%... 20%... 30%... 40%... 50%... 60%... 100%
Making disk bootable...
Copying boot files...
Install media now available at "/Volumes/Install macOS Mojave"
macmini:~ andy$ mv -v /tmp/Mojave.cdr.dmg ~/Desktop/InstallSystem.dmg
/tmp/Mojave.cdr.dmg -> /Users/andy/Desktop/InstallSystem.dmg
macmini:~ andy$ hdiutil detach /Volumes/Install\ macOS\ Mojave
"disk3" ejected.
macmini:~ andy$ hdiutil convert ~/Desktop/InstallSystem.dmg -format UDTO -o ~/Desktop/Mojave.iso
Reading Driver Descriptor Map (DDM : 0)…
Reading Apple (Apple_partition_map : 1)…
Reading  (Apple_Free : 2)…
Reading disk image (Apple_HFS : 3)…
..............................................................................
Elapsed Time: 56.989s
Speed: 210.6Mbytes/sec
Savings: 0.0%
created: /Users/andy/Desktop/Mojave.iso.cdr
macmini:~ andy$ mv -v /Users/andy/Desktop/Mojave.iso.cdr /Users/andy/Desktop/Mojave.iso
/Users/andy/Desktop/Mojave.iso.cdr -> /Users/andy/Desktop/Mojave.iso
macmini:~ andy$
```















