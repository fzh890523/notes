

### pod如何调度到正确的arch/os上

> 背景：
>
> All Kubernetes nodes today have the following default labels:
>
> - kubernetes.io/os = [windows|linux]
> - kubernetes.io/arch = [amd64|arm64|...]



* 原则上，pod template里应该自觉的根据自己的...来加上 nodeSelector like `"kubernetes.io/os": windows`

  **但实际上现有的绝大部分都没加**，可能就没考虑这一点

* 那么，（权宜之计）只能：
  * 把一般占少数的windows节点加上taint： `os=windows:NoSchedule`
    * 可以在节点join时加入： `--register-with-taints='os=windows:NoSchedule'`
    * 运行时taint： `k taint node winsvr-1 os=windows:NoSchedule`
  * 把windows pod加上node selector

> 确实k8s应该自己内部解决这个问题的，比如读到镜像信息里的arch/os，然后根据默认或者额外配置的规则来schedule