

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



### run mode

* docker容器本身的运行方式： dockerd（or sth else）执行的 受限进程
* 那么显然，运行的镜像理论上需要同样的os支持
* 但从inspect镜像中可以看到，只限定了arch、os信息，如`linux` + `amd64`，所以校验应该是到此为止

* 那么运行时，实际遇到需要的能力（syscall等）kernel不支持的话，只能是报错了



#### 如何支持不同内核的镜像

这里的不同是指host os与image：

* 同一种系统（比如linux）的不同版本

  尤其host os版本更低呢？

  > 从上面可知，应该没有这个层面的强校验，只能运行时报错

* 不同操作系统，比如win上跑linux镜像

  > 从上面可知，直接执行是不支持的，会校验报错
  >
  > 只能另开os（虚机物理机都ok）运行docker再执行
  >
  > * 程序调度： 如docker desktop for windows
  > * 调度框架： k8s





### entrypoint vs cmd



* entrypoint

  * 选择： 按照以下顺序

    1. `--entrypoint`参数指定

       **一旦覆盖了ep，dockerfile中的`CMD`也将失效**。 此时执行的命令等于 `--entrypoint` + `cmd`

       `docker run --entrypoint="" <img>  # 后面没cmd部分了` 这样等于没有任何执行内容（无论dockerfile内容如何），会报错

    2. 镜像的dockerfile中`ENTRYPOINT`设置内容（如果有）

    3. 如果1、2都没有，则用默认的`/bin/sh -c`

  **暂时不知道**`--entrypoint`怎么传递多个元素，比如还原默认行为的`/bin/sh -c`，直接`--entrypoint="/bin/sh -c"`是不行的

* cmd

  run命令的`<img>`后面的部分就是作为对dockerfile中`CMD`的覆盖

  如：`docker run --entrypoint /bin/logwrap myservice /bin/service -e`表示

  * `ENTRYPOINT`覆盖为`/bin/logwrap`
  * `CMD`覆盖为`/bin/service -e`



实际执行的内容是： entrypoint + cmd

* `ENTRYPOINT ["/bin/chamber", "exec", "production", "--"]` + `CMD ["/bin/service", "-d"]` = `["/bin/chamber", "exec", "production", "--", "/bin/service", "-d"]`

* `ENTRYPOINT null`（也即没有） + `CMD ["/bin/service", "-d"]` = `["/bin/sh", "-c", /bin/service", "-d"]`



dockerfile中对二者的定义见 [container_docker_dockerfile_yonka.md](container_docker_dockerfile_yonka.md) 



## dockerfile

见 [container_docker_dockerfile_yonka.md](container_docker_dockerfile_yonka.md)



## image

见 [container_docker_image_yonka.md](container_docker_image_yonka.md)



## registry

同见...



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



## moby

见 [docker_moby_yonka.md](docker_moby_yonka.md)



# 生态



* Docker Compose： 管理多个容器的联动，详见 [docker compose_yonka.md](docker compose/docker compose_yonka.md)

* 管理工具

  * [portainer](https://documentation.portainer.io/v2.0/deploy/ceinstalldocker/)： 基于web的功能全面的管理平台（服务形式，`-d`）

    详见链接，简单运行样例如下：

    ```sh
    # linux/windows wsl
    docker volume create portainer_data
    
    docker run -d -p 8000:8000 -p 9000:9000 --name=portainer --restart=always -v /var/run/docker.sock:/var/run/docker.sock -v portainer_data:/data portainer/portainer-ce
    
    # 老版本没用这个volume，而是创建了目录挂载进去
    # win ps的话： -v C:\ProgramData\Portainer:/data
    ## win docker能同时识别 /var/run/docker.sock 和 C:\ProgramData\Portainer 也是神奇，不知道是不是代码做了特殊处理
    # wsl ubuntu的话： -v /mnt/c/ProgramData/Portainer:/data
    ```

    

  * lazyDocker： 基于console的管理工具（没有`-d`），及时退出

    ```sh
    docker run --rm -it -v \
    /var/run/docker.sock:/var/run/docker.sock \
    -v ~/.config/lazydocker:/.config/jesseduffield/lazydocker \
    lazyteam/lazydocker
    
    alias lzd='docker run --rm -it -v /var/run/docker.sock:/var/run/docker.sock -v ~/.config/lazydocker:/.config/jesseduffield/lazydocker lazyteam/lazydocker'
    ```

    

    也还比较好用

    ![5-min_(1)-min.gif](http://dockone.io/uploads/article/20200526/dacac961ca91840ebf78a0308b2918db.gif)
  
  * [`tomastomecek/sen`](https://github.com/TomasTomecek/sen)： 查看容器、镜像
  
    ```sh
    docker run -v /var/run/docker.sock:/run/docker.sock -ti -e TERM tomastomecek/sen
    ```
  
  * [dive](https://github.com/wagoodman/dive)： 查看镜像，**可以方便的看每个commit layer的内容变化，实用**
  
    ```sh
    wget https://github.com/wagoodman/dive/releases/download/v0.9.2/dive_0.9.2_linux_amd64.deb
    sudo apt install ./dive_0.9.2_linux_amd64.deb
    # 安装报错，最后用的docker方式运行 v0.10.0 安装没问题
    ```
  
    
  
    ```sh
    dive <your-image> --source <source>
    
    docker run --rm -it \
        -v /var/run/docker.sock:/var/run/docker.sock \
        wagoodman/dive:latest <dive arguments...>
        # 参数比如镜像名
    ```
  
    > console可能因为不够宽而显示功能不全，可以尝试拉宽



# manual



见 [container_docker_tutorial_yonka.md](container_docker_tutorial_yonka.md)





