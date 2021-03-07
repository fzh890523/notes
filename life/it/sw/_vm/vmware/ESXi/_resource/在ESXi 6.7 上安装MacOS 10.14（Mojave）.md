
http://bbs.pcbeta.com/viewthread-1803777-1-1.html

ESXi 6.7已安装并正常运行。
我的硬件：Dell R710（intel 5520），X5650 * 2,64GB内存，GT730 pcie x8,250 GB SSD和300 GB SAS

目标：在ESXi上安装macOS Mojave 直通显卡和声卡当主机用。为什么想这样弄？因为没有台式机。


高级步骤

安装ESXi（此处不讲解）
安装Unlocker
创建一个新的虚拟机
安装macOS
安装VMware Tools


所需文件

Unlocker 3.0（VMware 15也可以用  https://github.com/DrDonk/unlocker）
（需要解压一次，因为论坛不允许上传tgz文件）
MacOS Mojave （ https://pan.baidu.com/s/16LOrqQV1JVNemrMVyEevWg 提取码: y66b ）
VMware Tools for macOS（ https://pan.baidu.com/s/1p-1oejLCwgY7iuryG9jP_Q 提取码: h9a2 ）
（需要解压一次，因为论坛不允许上传iso文件）

步骤 1：安装Unlocker（需要有linux的基础知识，很容易的）
通常，macOS只能安装在Apple硬件上。unlocker将修改ESXi以接受macOS VM的非Apple硬件。

使用上面的链接，下载Unlocker并将文件上传到ESXi系统
在ESXi中启用SSH
打开终端窗口
SSH到ESXi（“用户名@ESXI IP”）
cd 到您上传 Unlocker 的文件夹（数据存储区位于“/ vmfs / volumes /”中）
解压 Unlocker（“tar xzvf esxi-unlocker-xxx.tgz”）
您可能需要让脚本变为可执行。这可以通过“sudo chmod -x esxi-install.sh”来完成（可能会被要求输入密码）
使用“./esxi-install.sh”运行脚本
重新启动ESXi


步骤 2：下载 MacOS Mojave 并上传到 ESXI


如果不需要直通可以直接跳到步骤 4

步骤 3：导航器 - >管理 - >硬件 - > PCI设备，找到要直筒的pcie卡 勾选上 点击切换直通并重启ESXI （需要直通显卡的必须直通多一个usb口进入虚拟机，因为浏览器控制台的鼠标移不到第二个屏幕上）


步骤 4：创建一个新的虚拟机

登录ESXi Web界面
从“虚拟机”处单击“创建/注册虚拟机”
单击“创建新虚拟机”
起一个蛇皮的虚拟机名字（emmmm）
选择“Mac OS”作为来宾操作系统
选择“Apple Mac OS X 10.14（64位）”作为客户操作系统版本
选择要使用的数据存储
自定义设置：
        CPU 随意发挥
        内存：随意发挥 （如果需要直通显卡最好点开把  预留所有客户机内存 (全部锁定) 选项勾上）
        单击“完成”以创建VM
        删掉自动创建的SCSI控制器和硬盘，在 添加其他设备 里添加一个 NVMe控制器
        添加硬盘--新标准硬盘，点开新硬盘把控制器位置从 sata控制器 改为 NVMe控制器 （其实sata也是可以正常用的，个人喜好设置为NVme）
        USB控制器千万不要删掉（删掉后就没有办法在浏览器控制台控制虚拟机）
        添加其他设备--PCI设备
        CD/DVD驱动器改为 数据存储ISO文件 选择你上传到esxi的iso
点击下一页，完成 到此就完成了创建虚拟机



步骤 5：启动虚拟机以安装MacOS，在安装程序中，你可以看到显示器亮起来了，表示你的显卡已经被驱动且正常工作。（安装就不细说了)我用的GT730貌似是免驱的？？？
安装完系统 重启后会发现 浏览器控制台 一直卡在苹果进度条 但显示器是正常工作的，看显示器可以发现已经进入到 用户向导的页面 由于不是主屏幕，显示器输出的画面是灰色背景。
此时直接关掉虚拟机。在虚拟机设置里把添加进去的 PCI设备 删掉（如果直通了 usb拓展器 和 显卡，只需要把 显卡 删掉就可以了）


步骤 6：开机完成向导后，在设置中打开自动登录（想尝试摸黑拖系统设置到第二屏幕的兄嘚可以试试）并将系统设置窗口拖到最右上侧（千万不要关闭），然后关机（记得勾上开启式打开之前的窗口）。

步骤6：安装VMware Tools （emmm 装不装我感觉都没什么区别）


注意：需要直通显卡的必须直通多一个usb口进入虚拟机并接上鼠标，因为浏览器控制台的鼠标移不到第二个屏幕上。

我试过在vmx中将svga.present =“TRUE”设置为FALSE来关闭虚拟显示器，这导致了显卡也不能正常工作。



在vmx中添加：（详见http://bbs.pcbeta.com/forum.php? ... 311%26typeid%3D1311）

hw.model =“MacPro6,1”
uuid.action =“keep”

直通显卡不可以设置Board-id，否则无法显示。serialnumber不确定会不会对直通显卡造成影响
CPU参数，我貌似没成功过，是我的cpu太旧了么



有人尝试过在esxi或vm中自定义cpu的型号吗？？？？？？？