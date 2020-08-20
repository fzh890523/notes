



## tutorial



Ref: https://docs.fedoraproject.org/ro/Fedora_Draft_Documentation/0.1/html/RPM_Guide/ch02s03.html

Tabel 2.1. The main `rpm` operations

| Operation                                   | Short Option | Long Option |
| ------------------------------------------- | ------------ | ----------- |
| Upgrade/install                             | -U           | --upgrade   |
| Install                                     | -I           | --install   |
| Remove                                      | -e           | --erase     |
| Query                                       | -q           | --query     |
| Verify                                      | -V           | --verify    |
| Check signature                             | -K           | --checksig  |
| Freshen (upgrade) already-installed package | -F           | --freshen   |
| Initialize database                         | None         | --initdb    |
| Rebuild database                            | None         | --rebuilddb |



### Install file





### provider file



* 已安装（存在的）文件

  ```shell
  rpm -qf ${file_path}
  # 如
  rpm -qf /etc/sudoers
  ```

* 未安装（不存在的）文件

  ```shell
  yum whatprovides "/etc/sudoers"
  # 如
  yum whatproviders nslookup
  ```


### list files

* installed package

  ```sh
  rpm -ql package-name
  ```

  ```sh
  Sample outputs:
  
  /usr/bin/gpg
  /usr/bin/gpgsplit
  /usr/bin/gpgv
  ...
  ```



### Extract file



* rpm2cpio

  ```sh
  rpm2cpio ./packagecloud-test-1.1-1.x86_64.rpm | cpio -idmv
  ```

  








