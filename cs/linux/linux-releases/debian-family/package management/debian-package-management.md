

# tutorial



## list 

### installed packages

* `apt list —installed`

* `dpkg --get-selections | grep -v deinstall` for old versions

  Or `dpkg -l`



### upgradable packages



* `apt list —upgradable`





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
