ref: https://kubernetes.io/docs/setup/production-environment/container-runtimes/



## install container runtime

1. install docker

  ```sh
  # Install Docker CE
  ## Set up the repository:
  ### Install packages to allow apt to use a repository over HTTPS
  apt-get update && apt-get install -y \
    apt-transport-https ca-certificates curl software-properties-common gnupg2
  # 可能block在apt🔐，lsof /var/lib/dpkg/lock-frontend然后杀掉对应进程和所有apt进程后重试  
  
  ### Add Docker’s official GPG key
  curl -fsSL https://download.docker.com/linux/ubuntu/gpg | apt-key add -
  
  ### Add Docker apt repository.
  add-apt-repository \
    "deb [arch=amd64] https://download.docker.com/linux/ubuntu \
    $(lsb_release -cs) \
    stable"
  
  ## Install Docker CE.
  apt-get update && apt-get install -y \
    containerd.io=1.2.13-1 \
    docker-ce=5:19.03.8~3-0~ubuntu-$(lsb_release -cs) \
    docker-ce-cli=5:19.03.8~3-0~ubuntu-$(lsb_release -cs)
  
  # Setup daemon.
  cat > /etc/docker/daemon.json <<EOF
  {
    "exec-opts": ["native.cgroupdriver=systemd"],
    "log-driver": "json-file",
    "log-opts": {
      "max-size": "100m"
    },
    "storage-driver": "overlay2"
  }
  EOF
  
  mkdir -p /etc/systemd/system/docker.service.d
  
  # Restart docker.
  systemctl daemon-reload
  systemctl restart docker
  ```

2. install CRI-O

   ```sh
   # Prerequisites
   
   modprobe overlay
   modprobe br_netfilter
   
   # Setup required sysctl params, these persist across reboots.
   cat > /etc/sysctl.d/99-kubernetes-cri.conf <<EOF
   net.bridge.bridge-nf-call-iptables  = 1
   net.ipv4.ip_forward                 = 1
   net.bridge.bridge-nf-call-ip6tables = 1
   EOF
   
   sysctl --system
   
   
   
   # Setup repository
   . /etc/os-release
   sudo sh -c "echo 'deb http://download.opensuse.org/repositories/devel:/kubic:/libcontainers:/stable/x${NAME}_${VERSION_ID}/ /' > /etc/apt/sources.list.d/devel:kubic:libcontainers:stable.list"
   wget -nv https://download.opensuse.org/repositories/devel:kubic:libcontainers:stable/x${NAME}_${VERSION_ID}/Release.key -O- | sudo apt-key add -
   sudo apt-get update
   
   # Install CRI-O
   sudo apt-get install cri-o-1.17
   
   
   # start
   
   systemctl daemon-reload
   systemctl start crio
   
   ## 启动失败，囧： msg="config provided AppArmor profile \"crio-default\" not loaded"
   ```

   

3. install containerd

   ```sh
   # Prerequisites
   
   cat > /etc/modules-load.d/containerd.conf <<EOF
   overlay
   br_netfilter
   EOF
   
   modprobe overlay
   modprobe br_netfilter
   
   # Setup required sysctl params, these persist across reboots.
   cat > /etc/sysctl.d/99-kubernetes-cri.conf <<EOF
   net.bridge.bridge-nf-call-iptables  = 1
   net.ipv4.ip_forward                 = 1
   net.bridge.bridge-nf-call-ip6tables = 1
   EOF
   
   sysctl --system
   
   
   
   
   # Install containerd
   ## Set up the repository
   ### Install packages to allow apt to use a repository over HTTPS
   apt-get update && apt-get install -y apt-transport-https ca-certificates curl software-properties-common
   
   ### Add Docker’s official GPG key
   curl -fsSL https://download.docker.com/linux/ubuntu/gpg | apt-key add -
   
   ### Add Docker apt repository.
   add-apt-repository \
       "deb [arch=amd64] https://download.docker.com/linux/ubuntu \
       $(lsb_release -cs) \
       stable"
   
   ## Install containerd
   apt-get update && apt-get install -y containerd.io
   
   # Configure containerd
   mkdir -p /etc/containerd
   containerd config default > /etc/containerd/config.toml
   
   # Restart containerd
   systemctl restart containerd
   
   
   # To use the systemd cgroup driver, set plugins.cri.systemd_cgroup = true in /etc/containerd/config.toml
   ```

   



## install kubeadm



前提

* 关掉swap

  ```sh
  swapoff -a  # 对应的打开为： swapon -a
  # 注释掉 /etc/fstab 中的swap条目
  ```

* 让iptables ”看见“ 桥接流量

  ```sh
  cat <<EOF > /etc/sysctl.d/k8s.conf
  net.bridge.bridge-nf-call-ip6tables = 1
  net.bridge.bridge-nf-call-iptables = 1
  EOF
  sysctl --system
  
  lsmod | grep br_netfilter
  # 有必要的话： modprobe br_netfilter
  ```



1. install

   ```sh
   sudo apt-get update && sudo apt-get install -y apt-transport-https curl
   # 下一步可能因为google无法访问而启用代理
   curl -s https://packages.cloud.google.com/apt/doc/apt-key.gpg | sudo apt-key add -
   # 以下步骤的 xenial，好像即使是更新的bionic(18.04)也需要是xenial
   cat <<EOF | sudo tee /etc/apt/sources.list.d/kubernetes.list
   deb https://apt.kubernetes.io/ kubernetes-xenial main
   EOF
   # 以下步骤需要代理
   sudo apt-get update
   # 以下步骤需要代理，因为部分要访问 https://packages.cloud.google.com/apt kubernetes-xenial/main amd64 kube*
   sudo apt-get install -y kubelet kubeadm kubectl
   sudo apt-mark hold kubelet kubeadm kubectl
   ```

   

2. 配置使用systemd cgroup driver

   ```sh
   vim /etc/default/kubelet  # 对于debian/ubuntu
   
   # 加入如下行
   KUBELET_EXTRA_ARGS=--cgroup-driver=systemd
   
   # 重启kubelet
   systemctl daemon-reload
   systemctl restart kubelet
   ```

   

## Creating a single control-plane cluster with kubeadm



## Creating Highly Available clusters with kubeadm

需要更多的外部依赖：

* etcd

  不过也有两种模式： 独立etcd集群、混合etcd和控制面(也即etcd和控制节点一起部署，算整套方案)

* lb

  可以用云服务商的lb，也可以自建，比如haproxy等
  `-> lb -> control nodes`

### 安装控制面 - Stacked control plane and etcd nodes

* `sudo kubeadm init --control-plane-endpoint "LOAD_BALANCER_DNS:LOAD_BALANCER_PORT" --upload-certs`

  如： 
  
  `Found multiple CRI sockets, please use --cri-socket to select one: /var/run/dockershim.sock, /var/run/crio/crio.sock` 问题：


## 依赖准备、安装



### 准备apiserver的loadbalancer



1. 安装haproxy

   `aptitude install -y haproxy`

2. 增加配置

   `vim /etc/haproxy/haproxy.cfg`

   加上如下

   ```s
   frontend k8s_apiserver_front_8080
     bind *:8080
     stats uri /haproxy?stats
     default_backend k8s_apiserver_back_8080
   
   backend k8s_apiserver_back_8080
     balance source
     server m01 xxx-host-1:8080 check
     server m02 xxx-host-2:8080 check
     server m03 xxx-host-3:8080 check
   ```

   `system reload haproxy` 即可



## cleanup

参考： https://kubernetes.io/docs/setup/production-environment/tools/kubeadm/create-cluster-kubeadm/#tear-down



* （从cluster drain node，访问控制面）`kubectl drain <node name> --delete-local-data --force --ignore-daemonsets`

* （在node上）`kubeadm reset`

* clean iptables规则等

  ```sh
  iptables -F && iptables -t nat -F && iptables -t mangle -F && iptables -X
  
  ipvsadm -C  # 如果用了ipvs
  ```

* `kubectl delete node <node name>`

