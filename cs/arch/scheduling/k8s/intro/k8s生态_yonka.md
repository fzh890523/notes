



# 部署



## 单机部署



ref: [在本地运行Kubernetes的3种主流方式](https://www.jianshu.com/p/b84d65b9de32)



* minikube

  > Kubernetes的一个版本，专为本地部署而设计。Minikube由Kubernetes开源社区开发，使用VM来创建本地单节点Kubernetes安装
  >
  > 安装非常简单，因为**Windows、Linux和macOS**安装程序会自动执行大部分流程，包括VM设置。但是，请先确保安装了诸如**Hyper-V、VirtualBox或KVM等VM平台** 
  >
  > > **依赖vm，所以虚拟机里无法安装**
  >
  > 主要局限性在于**本地集群只能包含一个节点**，因此对于生产级别的多节点Kubernetes集群来说，很难在本地进行模拟测试。

* microK8s

  > 由Canonical开发的Kubernetes发行版，其突出特点是部署快速简单，对于本地运行Kubernetes来说，十分方便。
  >
  > 在支持snap packages的任何Linux发行版上，MicroK8s的安装都很简单。Snap packages是由Canonical创建的一个应用程序的打包框架，**主要用于Ubuntu**（该公司的Linux发行版）。在**Windows和macOS上**安装MicroK8s也是很简单的，因为**这些系统的MicroK8s安装程序会创建基于Ubuntu的VM**，然后使用snap packages在其上安装MicroK8s。
  >
  > 与Minikube不同，IT管理员或开发人员**可以使用MicroK8s创建多节点集群。如果MicroK8s在Linux上运行，甚至不需要VM**。在Windows和macOS上，MicroK8s使用名为Multipass的VM框架为Kubernetes集群创建VM。
  >
  > 最大缺点可能是**很难在不支持snap packages的Linux发行版上安装**

* k3s

  > 





## 生产部署



* [kubespray](https://github.com/kubernetes-incubator/kubespray)
* 





# 资源



## 实验平台

* [magic sandbox]( https://console.magicsandbox.com) 需要fq
* 





















