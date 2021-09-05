


## 不足

### 不支持hostnetwork的pod/deployment

[Add external-dns using the host PUBLIC-IP #632](https://github.com/kubernetes-sigs/external-dns/issues/632)

### `--publish-internal-services`时外部地址被内部地址覆盖

假设：
* 开启该选项
* clusterVIP类型的服务
* 在有externalIP的ingress等处暴露

此时，预期应该是externalIP优先于...，实际观察到拿到的是clusterIP。
