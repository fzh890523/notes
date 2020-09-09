## windows安装

ref： https://kubernetes.io/docs/tasks/tools/install-minikube/#install-minikube-using-an-installer-executable



1. 安装kubectl: https://kubernetes.io/docs/tasks/tools/install-kubectl/

2. 准备vm环境

   * hyper-V

     ```
     systeminfo  # 查看
     # windows程序与功能里打开hyper-v
     ```

     

   * virtualBox

3. 安装minikube

   * chocolatey安装

     1. 如果没安装cho，进行安装

        ```powershell
         Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://chocolatey.org/install.ps1'))
        ```
   
        ref: https://chocolatey.org/install

     2. 安装：`choco install minikube`

   * installer安装或者安装包安装

4. 启动： `minikube start --driver=<driver_name>`

   直接 `minikube start` 了。。。

5. 访问 minikube dashboard： `minikube dashboard`



子命令（`minikube ...`）：

* `start`
* `stop`
* `delete`





# 安装后可能遇到的问题



## 拉取镜像问题



`--registry-mirror` 和 `--image-repository` 的区别？



```
      --image-mirror-country='':
需要使用的镜像镜像的国家/地区代码。留空以使用全球代码。对于中国大陆用户，请将其设置为
cn。
      --image-repository='': Alternative image repository to pull docker images from. This can be used when you have
limited access to gcr.io. Set it to "auto" to let minikube decide one for you. For Chinese mainland users, you may use
local gcr.io mirrors such as registry.cn-hangzhou.aliyuncs.com/google_containers
      --insecure-registry=[]: Insecure Docker registries to pass to the Docker daemon.  The default service CIDR range
will automatically be added.
      --registry-mirror=[]: 传递给 Docker 守护进程的注册表镜像
```



https://stackoverflow.com/questions/54803393/run-minikube-start-cannot-pull-k8s-gcr-images-but-docker-pull-can-do 的解释：

```
We may use --image-repository as an ultimate workaround if none of mirrors provided is accessible in user's environment.
I agree that maintaining a mirror list as like the APT / pacman world also helps, or even trying to guess a reliable mirror by IP address during the setup.

--registry-mirror only impacts those images with no repository prefix - images that come from the Docker official registry.
It won't work on images from private registries, which is the case like gcr.io/kube-proxy. For private images, docker will still go to the private registry and fetch them.

If you set up minikube with --registry-mirror, it could work for pods/deployments that use ubuntu/18.04, but not for gcr.io/ - you will need to wipe the "gcr.io/" out from the references to make it look like an image from the official registry.
For the latter case, it could be achieved using --registry-mirror https://private_server --image-registry [private_server/]google_containers
```



```
--image-mirror-country='cn' --image-repository='registry.cn-hangzhou.aliyuncs.com/google_containers'
```



