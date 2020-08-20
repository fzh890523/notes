



## install


mac上安装报错

```
Installation fails on 10.13.x, 10.14.x (rc=-1908)
```

```
Install Virtual box on mac OS +10.3 with security kernel can failed. After you have this message :

Kernel driver not installed (rc=-1908)

because you have to add editor Oracle in the list of security kext.

If you have chance, you can see the ask for authorisation in Preference -> "Security and Privacy".

But it was not my case. I want to share the solution, because it was a really pain for me: Add Oracle editor in this authorization list.

Restart your mac in Recovery mode (cmd + R)

Then open a Terminal and enter : spctl kext-consent add VB5E2TV963

Restart your mac.

VB5E2TV963 is the code for oracle.

It's ok for me with Mojave and Virtual Box 6.
```

```sh
# 在恢复模式下执行
## 恢复模式进入： 重启 - cmd-R直到出现苹果logo后松开 - macOS utilities界面菜单栏-utilities里选择打开terminal
spctl kext-consent add VB5E2TV963
```







## guest additions



有一些feature（比如share folder）依赖这个，类似vmware的。。。



1. 参考[链接](https://www.virtualbox.org/manual/ch04.html#mountingadditionsiso)找到这个iso(随vbox软件一起分发的)

2. 参考[链接](https://www.virtualbox.org/manual/ch04.html#additions-linux)安装

   因为要build module，可能依赖一些dev工具链





## 问题



### 只支持usb 1.1问题



需要安装额外的 `Oracle VM VirtualBox Extension Pack`

https://www.virtualbox.org/wiki/Downloads



```
Support for USB 2.0 and USB 3.0 devices, VirtualBox RDP, disk encryption, NVMe and PXE boot for Intel cards. See this chapter from the User Manual for an introduction to this Extension Pack. The Extension Pack binaries are released under the VirtualBox Personal Use and Evaluation License (PUEL). Please install the same version extension pack as your installed version of VirtualBox.
```



