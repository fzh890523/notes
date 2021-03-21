# ref 

## startup

* [How To Install and Use Docker on Ubuntu 18.04 - DO](https://www.digitalocean.com/community/tutorials/how-to-install-and-use-docker-on-ubuntu-18-04)



# cmd

## build

...

## tag
...

## push

...

## commit

...



## exec

...



## inspect

...



## run

...



## cp

...



## diff

...



## context： 多个remote

...



# tutorial



## config

* 在“使用端”也是c/s架构，很多工作是交给本地daemon来完成的
* 所以，配置在`/etc/docker/daemon.json`



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



见 [container_docker_tutorial_image_yonka.md](container_docker_tutorial_image_yonka.md)



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















