



### image location





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



