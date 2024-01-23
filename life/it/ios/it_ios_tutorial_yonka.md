

# storage



## smb



自带的文件即可支持，虽然好像不支持扫描需要手动输入服务器信息



## 应用存储



* 每个应用一个存储空间（类似挂载点），默认情况下类似chroot只能看到自己的存储空间

  类似： `我的iPad/<app>/`

* ios自带的`文件`app有特殊权限，能看到 `我的iPad` 这一级，也即能看到各app的存储

* 应用可以按需“申请”访问其他文件

  以`es文件浏览器`为例，它的`导入文件`的入口，可以唤起系统自带的交互（UI是`文件`）选择指定的文件/目录“导入”到es

  实际效果类似 `mount <otherAPP>/dir/file myDir/myFile` / `mount <otherAPP>/dir myDir1/myDir2`。 这个过程需要用户显式操作/选取，等于显式授权

* 应用可以在自己的“存储空间”中增加挂载点暴露给“系统”，类似的也可以暴露给其他app来“申请访问”

  以`termius`为例，它把已知host的`HOME` dir给暴露出来了，类似 `我的iPad/termius/host1` （这个host1是alias） 指向该host的登陆用户`HOME` dir

* 似乎有“存储需求”的app才会建上述目录，其他的则在`文件`中不作显示



## 文件共享



从以上以及es的功能实例来看，是允许app“主动申请”访问其他（app的目录），但前提也是app做了这个交互UI。

暂时没发现有系统层面“外加”共享的支持，也即 如 在“系统”/`文件`中来把app2的某目录“导入”到app1的某目录。





## 基础访问



### .开头文件不可见



* 各file explorer都没法看到（自带，es等）
* 网上说<del>这是ios比较底层限制，无法绕过</del>
* ish中能访问 `ls -la` 到
* obsidian-git能正常work，说明肯定能读写`.git/`





# 外设



## 打印



* 似乎原生只支持air printer

* 不支持air printer的设备，可以 `输出pdf - 分享给打印app（如hp smart） - ...`

  如notability就可以

