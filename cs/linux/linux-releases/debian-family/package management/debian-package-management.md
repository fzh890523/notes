

# tutorial



## 源



### 源访问



#### 使用代理



有时可能包含google等被q的源，这时可以考虑用代理：

```sh
sudo apt-get -o Acquire::http::proxy="http://127.0.0.1:8087" update  # 假设代理为 808 这个
```





### 源配置



#### 改用国内源



大约步骤：

```sh
cd /etc/apt
cp source.list source.list.bak  # 备份
:> source.list  # 清空
vim source.list  # 加入需要的源，比如阿里的。 见下文
apt-get update

# 也可以直接改，如：
cp source.list source.list.bak  # 备份
sed -i 's/http.*ubuntu.com/http:\/\/mirrors.aliyun.com/g' sourece.list
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

### specify version

`apt-get install <package>=<version>`



### install from deb file

* `sudo apt install ./xxx.deb`



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



## package info

### package files

```sh
apt-file list packagename  # for package not installed
```

```sh
dpkg-query -L ${packagename}  # name

# or 

dpkg-deb -c ${package_name.deb}  # file
```





### package versions



* `aptitude version <package>`

  `aptitude -V -s install <package>` Passing `-V` will show detailed information about versions

* `aptitude search <package> -F "%c %p %d %V"

  `%c` = status (package installed or not)
  `%p` = package's name
  `%d` = package's description
  `%V` = available package's version

* `apt-show-versions <package>` 

  if installed



# tools



## 同步其他机器上安装的包

* 按目标版本安装
* 已有的跳过



```sh
apt list --installed | awk -F/ '{print $1}' > /tmp/xx.log

```





