

# 介绍



## 概念



### image vs container



>一个正在运行的 Linux 容器，其实可以被“一分为二”地看待：
>
>1. 一组联合挂载在 /var/lib/docker/aufs/mnt 上的 rootfs，这一部分我们称为“容器镜
>
>像”（Container Image），是容器的静态视图；
>
>2. 一个由 Namespace+Cgroups 构成的隔离环境，这一部分我们称为“容器运行
>
>时”（Container Runtime），是容器的动态视图











## image



### image url



如：

* `docker pull ubuntu`
* `docker push samalba/hipache`
* `docker push localhost.localdomain:5000/ubuntu`



url表示：

* 无hostname则用默认域名

  该默认值为docker官方的repo，但可覆盖

* 有hostname则访问该repo

  有特殊参数可以replace特定域名下的images为其他域名

  > 这个功能其实可以（但没有）做成通用参数，如 --replace xxx.com --replace-to yyy.com，甚至 --replace xxx.com/xx --replace-to yyy.com/xx



### 镜像源 repository

ref： https://www.cnblogs.com/doraman/p/9570891.html



**官方docker hub**

官方：https://hub.docker.com/explore/

**常用docker 国内镜像源：**

网易镜像中心：http://hub-mirror.c.163.com

阿里镜像中心：https://dev.aliyun.com

Docker 官方中国区：https://registry.docker-cn.com

ustc： https://docker.mirrors.ustc.edu.cn

daocloud：https://hub.daocloud.io/

**docker国内免费加速器：**

daocloud：`https://***(自己注册的私有账号).m.daocloud.io`

aliyun：`https://***(自己注册的私有账号).mirror.aliyuncs.com`

**修改方法**

- - 直接设置 `–registry-mirror` 参数，仅对当前的命令有效 
    `docker run hello-world --registry-mirror=https://docker.mirrors.ustc.edu.cn`
  - 修改 `/etc/default/docker`，加入 `DOCKER_OPTS=”镜像地址”`，可以有多个 
    `DOCKER_OPTS="--registry-mirror=https://docker.mirrors.ustc.edu.cn"`
  - 支持 `systemctl` 的系统，通过 `sudo systemctl edit docker.service`，会生成  `/etc/systemd/system/docker.service.d/override.conf` 覆盖默认的参数，在该文件中加入如下内容： 
    `[Service] ExecStart= ExecStart=/usr/bin/docker -d -H fd:// --registry-mirror=https://docker.mirrors.ustc.edu.cn`
  - 新版的 Docker 推荐使用 json 配置文件的方式，默认为` /etc/docker/daemon.json`，非默认路径需要修改 dockerd 的 `–config-file`，在该文件中加入如下内容： 
    `{ "registry-mirrors": ["https://docker.mirrors.ustc.edu.cn"] }`









### 其他



#### `--registry-mirror` 和 `--image-repository` 的区别？



* `registry-mirror` 影响的是public user images拉取策略，也即没有域名的image url

  默认从docker。。。拉取，而设置该参数可以覆盖该行为，从指定repo拉取public images

* `image-repository` 影响的是`gcr.io/google_containers` 下面的images

  这个属于特定功能了，可能主要针对无法访问`gcr.io`的场景，比如企业内网、中国大陆等





## volume



### 映射主机目录到容器



大致流程是这样：

1. 创建容器进程，开启mount ns

2. 容器内创建目录作为挂载点

3. 执行mount --bind操作把inode替换为宿主机的目录

   这个挂载只在容器内可见



效果：

* 对该目录下内容的变更，实际生效于宿主机目录

* 容器本身内容的变化，其实仅限于创建的那个目录

  那么，commit时也只是这个目录，而不是其下内容变化



## fs、file hierarchy



```sh
sudo ls /var/lib/docker/
builder  buildkit  containers  image  network  overlay2  plugins  runtimes  swarm  tmp  trust  volumes
```



`/var/lib/docker/volumes/[VOLUME_ID]`

`/var/lib/docker/aufs/mnt/[可读写层ID]`



# 生态



* Docker Compose： 管理多个容器的联动，详见 [docker compose_yonka.md](docker compose/docker compose_yonka.md)



# manual



见 [container_docker_tutorial_yonka.md](container_docker_tutorial_yonka.md)





