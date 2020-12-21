



# intro

* k8s项目原生支持
* 代码在k8s下： ` kubernetes/cmd/kubeadm`

* 使用场景： 非生产

   目前最欠缺的是，一键部署一个高可用的 Kubernetes 集群，即：Etcd、Master 组件都应该是多节点集群，而不是现在这样的单点。这，当然也正是 kubeadm 接下来发展的主要方向



# manual



## 常用操作、命令



* `kubeadm init` init a master node
* `kubeadm join <master node ip> ` join an existing cluster

* `config`

  * `images`

    * `pull` 拉取

      可以提前拉取准备好需要的镜像

    * `list`

      print 镜像列表

      如：

      ```
      k8s.gcr.io/kube-apiserver:v1.18.10
      k8s.gcr.io/kube-controller-manager:v1.18.10
      k8s.gcr.io/kube-scheduler:v1.18.10
      k8s.gcr.io/kube-proxy:v1.18.10
      k8s.gcr.io/pause:3.2
      k8s.gcr.io/etcd:3.4.3-0
      k8s.gcr.io/coredns:1.6.7
      ```

      





## 安装



准备好这三个binary即可：

* kubeadm
* kubelet
* kubectl





## 配置



### 定制集群组件参数

`kubeadm init --config kubeadm.yaml`

其中`apiServerExtraArgs`字段可以为apiserver指定参数，类似：

```yaml
apiServerExtraArgs: 
	advertise-address: 192.168.0.xx 
	anonymous-auth: false 
	enable-admission-plugins: AlwaysPullImages,DefaultStorageClass 
```

那么在生成apiserver配置时会用该内容去替换。



典型的使用方式

* 替换master组件的镜像地址，比如gcr无法访问时。。。
* 指定证书文件
* 指定特殊的容器运行时
* ...





# 实现细节



* kubelet运行在宿主机，用容器部署其他k8s组件

  因为kubelet如果在容器内，很难操作宿主机资源了（fs挂载等）



## init流程



1. 环境检查

   是否符合条件，比如内核能力是否满足功能要求等

   * kernel 3.10+
   * cgroups可用
   * hostname是否标准
   * kubeadm和kubelet版本匹配
   * 已安装的k8s
   * k8s端口是否占用
   * 需要的linux命令： ip、mount等
   * docker是否安装
   * ...

2. 准备证书

   证书给https访问apiserver用（默认https，除非显式开启”不安全模式“）： ` /etc/kubernetes/pki`下（如果有（比如提前准备了）则不生成）

   * `ca.` `crt|key`： 应该是 kubelet -> apiserver 方向
   * `apiserver-kubelet-client.` `crt|key`： apiserver -> kubelet 方向

3. 为其他组件生成访问 kube-apiserver 所需的配置文件： `/etc/kubernetes/xxx.conf`

   ```sh
   ls /etc/kubernetes/ 
   admin.conf controller-manager.conf kubelet.conf scheduler.conf
   ```

   记录的是，当前这个 Master 节点的服务器地址、监听端口、证书目录等信息

4. 为master组件生成pod配置文件： apiserver、controller-manager、scheduler。在`/etc/kubernetes/manifests`下

   > 有一种特殊的容器启动方法叫做“Static Pod”:
   >
   > 它允许你把要部署的 Pod 的YAML 文件放在一个指定的目录里。这样，当这台机器上的 kubelet 启动时，它会自动检查这个目录，加载所有的 Pod YAML 文件，然后在这台机器上启动它们

   ```sh
   ls /etc/kubernetes/manifests/ 
   etcd.yaml kube-apiserver.yaml kube-controller-manager.yaml kube-scheduler.yaml
   ```

5. 启动master组件们

   会通过检查 localhost:6443/healthz 这个 Master 组件的健康检查URL，等待 Master 组件完全运行起来

6. 就会为集群生成一个 bootstrap token

   在后面，只要持有这个 token，任何一个安装了 kubelet 和 kubadm 的节点，都可以通过 kubeadm join 加入到这个集群当中

   > 这个 token 的值和使用方法会，会在 kubeadm init 结束后被打印出来

7. 会将 ca.crt 等 Master 节点的重要信息，通过 ConfigMap 的方式保存在 Etcd 当中，供后续部署 Node 节点使用

   这个 ConfigMap 的名字是 `cluster-info`

8. 安装默认插件

   Kubernetes 默认 kube-proxy 和 DNS 这两个插件是必须安装的。它们分别用来提供整个集群的服务发现和 DNS 功能



## join流程



前提： 拿到init阶段生成的token，可以通过该token去 ”不安全的“访问apiserver拿到ca文件（其实是cluster-info），之后就可以通过ca文件 ”安全的“访问apiserver了。

> 所以叫做： bootstrap token



















