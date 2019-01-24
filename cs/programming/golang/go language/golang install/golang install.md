



# platforms



## ubuntu



Ref: https://github.com/golang/go/wiki/Ubuntu



* 普通安装： `sudo apt-get install golang-go`

* 安装新版

  ```sh
  $ sudo add-apt-repository ppa:gophers/archive
  $ sudo apt-get update
  # 可以先 aptitude search golang 搜到版本后指定安装需要的版本
  $ sudo apt-get install golang-1.10-go  # 1.10 可以是需要的版本
  # 如果之前有低版本的话，可能需要覆盖一下软链接，类似 ln -s /usr/lib/go-1.9/bin/go /usr/bin/go
  ```


