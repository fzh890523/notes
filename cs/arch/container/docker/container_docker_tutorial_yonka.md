# ref 

## startup

* [How To Install and Use Docker on Ubuntu 18.04 - DO](https://www.digitalocean.com/community/tutorials/how-to-install-and-use-docker-on-ubuntu-18-04)



# cmd



## build



```sh
 docker build -t helloworld .
```



### build时使用proxy

ref： https://stackoverflow.com/questions/27749193/how-to-build-docker-images-with-dockerfile-behind-http-proxy-by-jenkins

> **实在太麻烦了**
>
> 意思是：
>
> * 拉取镜像用`--build-arg`
> * RUN命令里的网络访问用`ENV`
> * config里配置proxy，但应该是对daemon生效而不是client
>
> **RUN apk add时，proxy怎么设置都不生效，即使RUN echo都打出来了**



Docker has multiple ways to set proxies that take effect at different times.

------

If your `docker build` has to **retrieve a base image through a proxy**, you'll want to specify `build-arg`s:

```xml
docker build --build-arg HTTP_PROXY=$http_proxy \
--build-arg HTTPS_PROXY=$http_proxy --build-arg NO_PROXY="$no_proxy" \
--build-arg http_proxy=$http_proxy --build-arg https_proxy=$http_proxy \
--build-arg no_proxy="$no_proxy" -t myContainer /path/to/Dockerfile/directory
```

where `$http_proxy` and `$no_proxy` were set in my bashrc. I used both `HTTP_PROXY` and `http_proxy` because different utilities will check different variables (`curl` checks both, `wget` only checks the lowercase ones, etc).

------

If your `docker build` has a **`RUN curl/wget/etc` command that has to go through the proxy**, you'll need to specify an environment variable inside your docker image:

```xml
ENV https_proxy=http://proxy-us02.company.com:8080
ENV http_proxy=http://proxy-us02.company.com:8080
ENV HTTP_PROXY=http://proxy-us02.company.com:8080
ENV HTTPS_PROXY=http://proxy-us02.company.com:8080
ENV no_proxy="localhost,localdomain,127.0.0.1,etc"
ENV NO_PROXY="localhost,localdomain,127.0.0.1,etc"
```

If you don't want this environment variable inside your image at runtime, you can remove all these at the end:

```xml
RUN unset http_proxy https_proxy no_proxy HTTP_PROXY HTTPS_PROXY NO_PROXY
```





```json
{
 "proxies":
 {
   "default":
   {
     "httpProxy": "http://127.0.0.1:3001",
     "httpsProxy": "http://127.0.0.1:3001",
     "ftpProxy": "http://127.0.0.1:3001",
     "noProxy": "*.test.example.com,.example2.com"
   }
 }
}
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

* `--env <name>=<value>`/`-e` 传入环境变量，可以多次指定

* `-v`/`--volume <local_path>:<container_path>` 将目录映射到容器的目录

* `--link <container>:<alias>` 当前容器连接到指定容器

  使得源容器（被链接的容器）和接收容器（主动去链接的容器）之间可以互相通信，并且接收容器可以获取源容器的一些数据，如源容器的环境变量。

  * 通信方式可以通过容器名或者这里的别名来dns解析到ip然后访问，实际是更新了hosts文件
  * 环境变量是以： `<alias>_NAME` 的名字继承过来

* `--entrypoint` 覆盖原来entrypoint

  清空的话，`--entrypoint=''` 

* `--cap-add` 为容器增加能力支持

  如要能正常运行iptables需要`--cap-add=NET_ADMIN`，否则报错类似

  ```sh
  can't initialize iptables table `filter': Permission denied (you must be root)
  ```

  



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

> 对于image，见下面 image tutorial 部分



## diff

```sh
Usage:  docker diff CONTAINER

Inspect changes to files or directories on a container's filesystem
```



```sh
docker diff 425xxx
C /var
C /var/run
A /var/run/secrets
A /var/run/secrets/kubernetes.io
A /var/run/secrets/kubernetes.io/serviceaccount
```



## context： 多个remote



```sh
docker context create home-dev --description "home dev node via tunnel" --docker "host=tcp://localhost:2376"
```







# tutorial



## config



## server/connect

### **监听方式的配置**

* `/etc/default/docker` 中加入： `DOCKER_OPTS="-H tcp://127.0.0.1:5000 -H unix:///var/run/docker.sock"`

* `system/docker.service`中启动命令里的`-H`同样的方式加入

* `/etc/docker/daemon.json`里`hosts`参数配置

  ```json
  {
    "debug": true,
    "hosts": ["tcp://127.0.0.1:2375", "unix:///var/run/docker.sock"]
  }
  ```

  

不过，dockerd不允许多处同时配置同一个hosts参数，而默认的`docker.service`配置了`-H fd://`，所以可以把这里的删掉统一从配置文件里配置：

```sh
sudo cp /lib/systemd/system/docker.service /etc/systemd/system/
sudo sed -i 's/\ -H\ fd:\/\///g' /etc/systemd/system/docker.service
sudo systemctl daemon-reload
sudo service docker restart
```



### 连接多个/指定daemon

* `docker --config <configPath>` 指定client config

* `docker --host <host>` 指定连接方式

  host支持：

  * `unix socket path`

  * `network`

    * `tcp://`

    * `ssh://`

      这里可以叠加ssh config里的proxy command来实现多层tunnel

* `docker context`

  * 通过`use`来切换当前context

    `docker context use home-dev && docker ps`

  * 通过`--context <context>`来实时指定

    `docker --context home-dev ps`



### 访问远端不直接可达docker

* 建立tunnel + 访问tunnel的本端端点
* 通过ssh proxy来访问+docker context ssh



## image

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





### 从image拷贝文件

* 运行为container然后用`docker cp`
* `docker run --rm --entrypoint cat yourimage  /path/to/file > path/to/destination`



### "进入"镜像查看



```sh
docker run --rm -it --entrypoint=/bin/bash ${image}
# 或者 /bin/sh
```





## container



### run





## ns



### 加入ns

* docker run的`--net`参数
* 系统调用setns

* `nsenter`

  ```sh
  docker ps | grep <img-name>
  docker inspect --format='{{  .State.Pid }}' <container_id>
  nsenter -t <pid> -n <cmd>  # 如： nsenter -t 29619 ip addr; nsenter -t 29619 bash:q
  ```

  



# docker on Mac



见： [docker_mac_yonka.md](mac/docker_mac_yonka.md)



# docker on windows

见： [docker_windows_yonka.md](mac/docker_windows_yonka.md)



# 在screen里
`ctrl+a` 进入命令模式
然后：
* `d` detach
* `k` kill















