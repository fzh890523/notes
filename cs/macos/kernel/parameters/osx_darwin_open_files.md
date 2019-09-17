

* 看进程打开文件 `lsof -p ${pid}`
* 看系统打开文件 `lsof`
  注意，可能很慢

系统限制
* 查看系统级别限制 `sysctl kern.maxfiles`

* 查看进程级别限制 `sysctl kern.maxfilesperproc`

* 修改限制 `sysctl -w ${name}=${value}`

  ```sh
  # 注意要sudo
  sysctl -w kern.maxfiles=20480 (or whatever number you choose)
  sysctl -w kern.maxfilesperproc=18000 (or whatever number you choose)
  ```

* 修改持久化 `/etc/sysctl.conf`

  格式类似：

  ```properties
  kern.maxfiles=20480
  kern.maxfilesperproc=18000
  ```

* 修改shell的限制

  > 因为多数程序可能由shell spawn，会集成shell的...，所以有必要修改

  ```sh
  ulimit -S -n 2048 # or whatever number you choose
  # 也是临时生效，持久化需要加到shell配置文件，如 .bashrc, .zshrc or whatever
  ```


