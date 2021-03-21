



### 镜像拉取



#### 被墙镜像拉取问题



* 用mirror

  但mirror里不一定全，尤其是一些按天/commit号出的镜像

* proxy

  * 修改daemon配置，reload

    用完改回来，不然所有镜像都走http proxy了太慢。

    所以比较麻烦

* 找第三方镜像

  如果第三方有，那么：

  ```sh
  
  ```

  

* 借第三方镜像服务来build

  



### image proxy

有时不方便设置全局proxy（如`https_proxy` env），比如还要做其他事情等等。

> docker命令的env proxy作用在client而实际拉取似乎是通过daemon，所以env无效...



> ubuntu下 /lib/systemd/system/docker.service

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



### 把容器“保存”为image

```sh
# docker run ...
# make some changes in this docker
docker commit <dockerId>  # output a image id
docker tag <imageId> <imageTag>
```





### 从image拷贝文件

* 运行为container然后用`docker cp`
* `docker run --rm --entrypoint cat yourimage  /path/to/file > path/to/destination`



### "进入"镜像查看



```sh
docker run --rm -it --entrypoint=/bin/bash ${image}
# 或者 /bin/sh
```



### 保存、加载 镜像“文件”

```sh
docker save couchbase > couchbase.tar
xz -9 couchbase.tar

docker load < couchbase.tar.xz

docker load -i ubuntu.tar

docker save --output saved-image.tar my-image:1.0.0
docker save my-image:1.0.0 > saved-image.tar
```

