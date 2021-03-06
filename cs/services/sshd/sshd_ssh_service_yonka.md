

### authorized_keys

对`~/.ssh`, `~/.ssh/authorized_keys` 文件/目录的权限都有要求，`700` or `755`，也即g,o都不能有w权限



## 问题排查



* sshd开debug

* 单独运行方便排查

  ```sh
  sudo /usr/bin/sshd -d -p 2222
  
  ssh -v -p 2222 <ip>
  ```

  



