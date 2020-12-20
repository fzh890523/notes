

### docker vm

```sh
screen ~/Library/Containers/com.docker.docker/Data/vms/0/tty
```

### docker image path

`~/Library/Containers/com.docker.docker/Data/vms/0/Docker.[qcow2, raw]`

ref: https://stackoverflow.com/questions/19234831/where-are-docker-images-stored-on-the-host-machine

If you keep in mind that Docker is still running in a VM, the system paths are relative to the VM and not from the Mac Osx system. As it says all is contained in a VM file :

```sh
/Users/MyUserName/Library/Containers/com.docker.docker/Data/com.docker.driver.amd64-linux/Docker.qcow2
```

Try to run Alpine image with this volume option and the ls command you are able to list the VM host:

```sh
docker run --rm -it -v /:/vm-root alpine:edge ls -l /vm-root
```

After this just try :

```sh
docker run --rm -it -v /:/vm-root alpine:edge ls -l /vm-root/var/lib/docker
```

Now, you are able to list the docker folder from the WM host


mac -> vm -> docker



### pull images via proxy

估计是因为mac下是通过vm来实际使用docker的，所以在mac设置的proxy env（`http_proxy`,`https_proxy`）没办法传递进去（虽然可以做，但是没实现）。

需要在docker程序的设置里显示设置。 `docker - preferences - proxies`





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







