

## 安装方式

### U盘启动安装

### 应用市场安装

1. 搜索`macOS`，选择GET
2. 。。。

application里多了 `install*` 的app，需要找到然后手动执行（也有可能是自动执行出错了。。。


## 制作安装U盘

ref:

https://jingyan.baidu.com/album/75ab0bcb8f9be4d6864db2ac.html?picindex=7



下载dmg or appStore get。
`{/Applications, /Volumes/...}/Install macOS Mojave.app/Contents/Resource/createinstallmedia --volume ${output_volume} {/Applications, /Volumes/...}/Install macOS Mojave.app --nointeraction`
需要sudo，如：
```sh
sudo "/Volumes/Install macOS Mojave 10.14/Install macOS Mojave.app/Contents/Resource/createinstallmedia" --volume "/Volumes/TOOLS /Volumes/Install macOS Mojave 10.14/Install macOS Mojave.app" --nointeraction
```

在此之前可能需要清理u盘，比如原先fs格式可能是ms的。
在`磁盘工具/disk utility`里，选择u盘，然后`erase`，选择`Mac OS Extended(Journaled)`。


