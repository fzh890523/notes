



## tutorial



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










