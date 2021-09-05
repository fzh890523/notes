



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
```





