



# 生态



## 管理工具



### etcdkeeper

* 支持api v3
* 能正常run（yonka）
* web-UI
* 可以run起来打开页面后指定url
* 交互还比较方便，有list、tree-view视图



```sh
docker run --rm -v `pwd`/etcd-browser/etcd-ca.crt:/etc/ssl/certs/etcd-ca.crt -p 8080:8080 evildecay/etcdkeeper

docker run --rm -v $BASE/etc/kubernetes/pki/etcd/ca.crt:/etc/ssl/certs/etcd-ca.crt -p 8080:8080 evildecay/etcdkeeper


./etcdkeeper --cacert ../kube/etc/kubernetes/pki/etcd/ca.crt -usetls

```



### etcdctl



```sh
ETCDCTL_API=3 ./etcd-download-test-v3.5.1/etcdctl --cacert=$BASE_DIR/etc/kubernetes/pki/etcd/ca.crt \
get /registry/namespaces -w=json
```





