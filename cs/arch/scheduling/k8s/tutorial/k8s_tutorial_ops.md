

## 普通管理操作



### 重启master

老版本里api-server等做成单独的服务，可以`systemctl restart kube-apiserver`这样操作。

新版本里是deployment/pod形式，那么：

```sh
kubectl -n kube-system delete pod kube-apiserver-xxx
kubectl -n kube-system delete pod kube-scheduler-xxx
# xxx一般是node hostname

# 会自动start
```





