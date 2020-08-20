

# tutorial



## 源配置



### 改用国内源



大约步骤：

```sh
cd /etc/apt
cp source.list source.list.bak  # 备份
:> source.list  # 清空
vim source.list  # 加入需要的源，比如阿里的。 见下文
apt-get update
```



```sh
deb http://mirrors.aliyun.com/ubuntu/ bionic main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ bionic-security main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ bionic-updates main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ bionic-proposed main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ bionic-backports main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ bionic main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ bionic-security main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ bionic-updates main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ bionic-proposed main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ bionic-backports main restricted universe multiverse

# 这里以bionic版本为例。 不同的版本，需要单独找
```







## list 

### installed packages

* `apt list —installed`

* `dpkg --get-selections | grep -v deinstall` for old versions

  Or `dpkg -l`



### upgradable packages



* `apt list —upgradable`





## install



### install from deb file



## what provides file

### `dpkg -S`

```
-S, --search filename-search-pattern...
                  Search for a filename from installed packages.
```

```sh
$ dpkg -S /bin/ls
coreutils: /bin/ls
```

### `apt-file find`

```sh
sudo apt-get install apt-file
apt-file update

$ apt-file find kwallet.h
kdelibs5-dev: /usr/include/kwallet.h
libkf5wallet-dev: /usr/include/KF5/KWallet/kwallet.h
```

### website: [Ubuntu Packages Search](http://packages.ubuntu.com/)

## package files

```sh
apt-file list packagename  # for package not installed
```

```sh
dpkg-query -L ${packagename}  # name

# or 

dpkg-deb -c ${package_name.deb}  # file
```





## package versions



* `aptitude version <package>`

  `aptitude -V -s install <package>` Passing `-V` will show detailed information about versions

* `aptitude search <package> -F "%c %p %d %V"

  `%c` = status (package installed or not)
  `%p` = package's name
  `%d` = package's description
  `%V` = available package's version

* `apt-show-versions <package>` 

  if installed





