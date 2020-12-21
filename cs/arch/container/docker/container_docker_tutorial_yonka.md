# ref 

## startup

* [How To Install and Use Docker on Ubuntu 18.04 - DO](https://www.digitalocean.com/community/tutorials/how-to-install-and-use-docker-on-ubuntu-18-04)



# cmd



## build



```sh
 docker build -t helloworld .
```



## tag

```sh
docker tag helloworld username/helloworld:v1
```



给镜像指定完整名，一般来说格式为： `<username>/<name>:<version>`

> yonka: 这里的helloworld算什么？ 如果多个叫这个的呢？



## push



```sh
docker push username/helloworld:v1
```



> yonka: 这里不指定域名，应该是默认到dockerhub吧（或者配置的其他的默认域名）



## commit



```sh
docker exec -it <container_id> /bin/sh
touch test.txt  # 文件变更
exit

docker commit <container_id> username/helloworld:v2  # 新建的文件被提交到新的版本
```



## exec



```sh
docker exec -it <container_id> /bin/sh
```



## inspect



```sh
docker inspect --format '{{.State.Pid}}' c1b7f8c6f85c  # 输出docker容器的进程号
82973

sudo ls -l /proc/82973/ns/
total 0
lrwxrwxrwx 1 root root 0 Oct 24 19:06 cgroup -> 'cgroup:[4026531835]'
lrwxrwxrwx 1 root root 0 Oct 24 19:06 ipc -> 'ipc:[4026532783]'
lrwxrwxrwx 1 root root 0 Oct 24 19:06 mnt -> 'mnt:[4026532781]'
lrwxrwxrwx 1 root root 0 Oct 24 19:06 net -> 'net:[4026532786]'
lrwxrwxrwx 1 root root 0 Oct 24 19:06 pid -> 'pid:[4026532784]'
lrwxrwxrwx 1 root root 0 Oct 24 19:06 pid_for_children -> 'pid:[4026532784]'
lrwxrwxrwx 1 root root 0 Oct 24 19:06 user -> 'user:[4026531837]'
lrwxrwxrwx 1 root root 0 Oct 24 19:06 uts -> 'uts:[4026532782]'
```





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



### 参数



* `--net` 指定ns

  `--net container:<container_id>`

  如果直接`--net host`则不使用ns直接使用宿主机网络栈

* `-it` interactive

* `--rm` 停止运行后，自动删除容器文件

* `-d` 容器启动后，在后台运行

* `--name <name>` 容器的名字叫

* `--env <name>=<value>` 传入环境变量，可以多次指定

* `-v`/`--volume <local_path>:<container_path>` 将目录映射到容器的目录

* `--link <container>:<alias>` 当前容器连接到指定容器

  使得源容器（被链接的容器）和接收容器（主动去链接的容器）之间可以互相通信，并且接收容器可以获取源容器的一些数据，如源容器的环境变量。

  * 通信方式可以通过容器名或者这里的别名来dns解析到ip然后访问，实际是更新了hosts文件
  * 环境变量是以： `<alias>_NAME` 的名字继承过来



examples：

* ```sh
  docker container run \
    --rm \
    --name wordpress \
    --volume "$PWD/":/var/www/html \
    --link wordpressdb:mysql \
    phpwithmysql
  ```

* ```sh
  docker container run \
    -d \
    --rm \
    --name wordpressdb \
    --env MYSQL_ROOT_PASSWORD=123456 \
    --env MYSQL_DATABASE=wordpress \
    mysql:5.7
  ```

* ```sh
  docker container run \
    --rm \
    --name wordpress \
    --volume "$PWD/":/var/www/html \
    php:5.6-apache
  ```

* 

* 





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





### image制作



```sh

```



1. 编写docker file: `Dockerfile`

   ```dockerfile
   FROM php:5.6-apache
   RUN docker-php-ext-install mysqli
   CMD apache2-foreground
   ```

2. build: `docker build -t phpwithmysql .`





## container



## ns



### 加入ns

* docker run的`--net`参数
* 系统调用setns



# docker on Mac



见： [docker_mac_yonka.md](mac/docker_mac_yonka.md)



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

  















