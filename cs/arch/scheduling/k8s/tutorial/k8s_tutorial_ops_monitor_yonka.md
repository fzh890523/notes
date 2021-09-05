

# 手段



## 命令行


* [k8s](https://github.com/derailed/k9s)

  ```sh
  docker run --rm -it -v $KUBECONFIG:/root/.kube/config quay.io/derailed/k9s
  
  docker run --rm -it -v ~/.kube/config:/root/.kube/config quay.io/derailed/k9s
  ```

  > 似乎也依赖metric-server，安装后才有cpu/mem等信息展示

* `kubectl top pod <pod-name> -n <ns>`

  > 依赖metric-server

## [metric-server](https://github.com/kubernetes-sigs/metrics-server)

```sh
kubectl apply -f https://github.com/kubernetes-sigs/metrics-server/releases/latest/download/components.yaml

docker run --rm k8s.gcr.io/metrics-server/metrics-server:v0.3.7 --help
# 注意，用了gcr repo，所以k8s worker node的镜像拉取是个问题
```

> 报错`unable to fully scrape metrics: [unable to fully scrape metrics from node xxx: unable to fetch metrics from node xxx: Get "https://x.x.x.x:10250/stats/summary?only_cpu_and_memory=true": x509: cannot validate certificate for x.x.x.x because it doesn't contain any IP SANs,`
>
> `k edit -n kube-system deployment metrics-server`加入容器运行参数`--kubelet-insecure-tls` 即可
>
> 其他参考的参数：
>
> ```sh
> - command:
>      - /metrics-server
>      - --metric-resolution=30s
>      - --kubelet-insecure-tls
>      - --kubelet-preferred-address-types=InternalIP
> ```



## Prometheus



* [prometheus-operator](https://github.com/prometheus-operator/prometheus-operator)
* [kube-prometheus](https://github.com/prometheus-operator/kube-prometheus)
* [prometheus-helm-charts](https://github.com/prometheus-community/helm-charts/tree/main/charts/k)ube-prometheus-stack



# 方案

ref：

* [Kubernetes平台监控方案之：Exporters+Prometheus+Grafana](https://blog.csdn.net/liukuan73/article/details/78881008)

* [k8s-部署metrics-server + Prometheus](https://blog.csdn.net/wangmiaoyan/article/details/102973315)



## prometheus + grafana



> 基于istio已有的p和p，略去安装过程



* grafana添加k8s的dashboard

  https://grafana.com/grafana/dashboards?search=kubernetes 搜索

  添加了

  * [1 Kubernetes for Prometheus Dashboard CN 20201209](https://grafana.com/grafana/dashboards/13105)

    > 这个没拿到数据... 怀疑可能是因为ns不对

  * [1. Kubernetes Deployment Statefulset Daemonset metrics](https://grafana.com/grafana/dashboards/8588)

    > 注意： Kubernetes 1.16 以后不再使用 pod_name 和 container_name 标签，解决办法是下载 grafana dashboard 的 json 文件，将里面的 pod_name 和 container_name 全部替换为 pod 和 container ，再在你的 grafana 里面导入这个 json 文件就好了。

  安装方式： 在import里

  * 下载安装： 下载json
  * 网络安装： 输入id即可



