# ref 

## startup

* [How To Install and Use Docker on Ubuntu 18.04 - DO](https://www.digitalocean.com/community/tutorials/how-to-install-and-use-docker-on-ubuntu-18-04)

## 

# cmd



## history



```sh
docker history a43f156372a7
IMAGE               CREATED             CREATED BY                                      SIZE                COMMENT
a43f156372a7        7 weeks ago         /bin/sh -c #(nop)  ENTRYPOINT ["/usr/local/bi   0 B
<missing>           7 weeks ago         /bin/sh -c useradd -m --uid 1337 istio-proxy    447.9 kB
<missing>           7 weeks ago         /bin/sh -c #(nop) COPY file:35665fb734764cccd   2.906 kB
```





## run



```sh
docker run -d <image>  # run in detach(from stdout/stdin) mode
```







## cp



```Sh
docker cp foo.txt mycontainer:/foo.txt
docker cp mycontainer:/foo.txt foo.txt
```







# tutorial



## image



### image proxy

有时不方便设置全局proxy（如`https_proxy` env），比如还要做其他事情等等。



```sh
Here is a link to the official Docker documentation for proxy HTTP: https://docs.docker.com/config/daemon/systemd/#httphttps-proxy

A quick outline:

First, create a systemd drop-in directory for the Docker service:

mkdir /etc/systemd/system/docker.service.d
Now create a file called /etc/systemd/system/docker.service.d/http-proxy.conf that adds the HTTP_PROXY environment variable:

[Service]
Environment="HTTP_PROXY=http://proxy.example.com:80/"
If you have internal Docker registries that you need to contact without proxying you can specify them via the NO_PROXY environment variable:

Environment="HTTP_PROXY=http://proxy.example.com:80/"
Environment="NO_PROXY=localhost,127.0.0.0/8,docker-registry.somecorporation.com"
Flush changes:

$ sudo systemctl daemon-reload
Verify that the configuration has been loaded:

$ sudo systemctl show --property Environment docker
Environment=HTTP_PROXY=http://proxy.example.com:80/
Restart Docker:

$ sudo systemctl restart docker
```





### registry-mirror


参考： https://yq.aliyun.com/articles/29941

* 比较新的docker版本： `vim /etc/docker/daemon.json`

  ```json
  {
    "registry-mirrors": ["<your accelerate address>"]
  }
  ```
  改完reload：
  ```sh
  sudo systemctl daemon-reload
  sudo systemctl restart docker
  ```
  
* 否则： 根据系统来配置

  * Ubuntu 12.04 14.04
  * Ubuntu 15.04 15.10
  * CentOS 7
  * Redhat 7
  * Redhat 6、CentOS 6


可以去aliyun申请： https://cr.console.aliyun.com/cn-hangzhou/instances/mirrors
得到有个专属url。
这个页面也有不同系统的配置方式介绍。











# docker on Mac



## 安装

docker for mac

http://mirrors.aliyun.com/docker-toolbox/mac/docker-for-mac/stable/







## 访问



运行在vm中，暴露出tty文件，可以通过screen来访问。



```sh
# 直接开screen并且attach过去
screen ~/Library/Containers/com.docker.docker/Data/com.docker.driver.amd64-linux/tty

# 开screen并且命名为docker，但不attach过去
screen -AmdS docker ~/Library/Containers/com.docker.docker/Data/com.docker.driver.amd64-linux/tty
# 回到docker这个screen
screen -r docker

# 看起来是干掉这个screen
screen -S docker -X quit

# list screens
screen -ls
```



# 在screen里
`ctrl+a` 进入命令模式
然后：
* `d` detach
* `k` kill





# info



## image location



* mac

  docker在虚拟机里，所以镜像也在，o(╯□╰)o

  ```sh
  ~/Library/Containers/com.docker.docker/Data/com.docker.driver.amd64-linux/Docker.qcow2

  screen ~/Library/Containers/com.docker.docker/Data/com.docker.driver.amd64-linux/tty  # ctrl-c 退出，会引起终端显示配置的一些变化 = =
  ```

* linux

  The contents of the `/var/lib/docker` directory vary depending on the [driver Docker is using for storage](https://github.com/docker/docker/blob/990a3e30fa66e7bd3df3c78c873c97c5b1310486/daemon/graphdriver/driver.go#L37-L43).

  By default this will be `aufs` but can fall back to `overlay`, `overlay2`, `btrfs`, `devicemapper`or `zfs` depending on your kernel support. In most places this will be `aufs` but the [RedHats went with `devicemapper`](http://developerblog.redhat.com/2014/09/30/overview-storage-scalability-docker/).

  You can manually set the storage driver with the [`-s` or `--storage-driver=`](https://docs.docker.com/engine/reference/commandline/dockerd/#/daemon-storage-driver-option) option to the [Docker daemon](https://docs.docker.com/engine/reference/commandline/dockerd/).

  - `/var/lib/docker/{driver-name}` will contain the driver specific storage for contents of the images.
  - `/var/lib/docker/graph/<id>` now only contains metadata about the image, in the `json`and `layersize` files.

  In the case of `aufs`:

  - `/var/lib/docker/aufs/diff/<id>` has the file contents of the images.
  - `/var/lib/docker/repositories-aufs` is a JSON file containing local image information. This can be viewed with the command `docker images`.

  In the case of `devicemapper`:

  - `/var/lib/docker/devicemapper/devicemapper/data` stores the images
  - `/var/lib/docker/devicemapper/devicemapper/metadata` the metadata
  - Note these files are thin provisioned "sparse" files so aren't as big as they seem.

  















