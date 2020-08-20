

# ref



## documentation



* [envoy-api-xds_protocol](https://www.envoyproxy.io/docs/envoy/latest/api-docs/xds_protocol)





### 部分、全量、差量推送

区分这几个概念：

> **非标准概念**，这里引入是为了帮助理解

* `部分推送`  推送某类资源的部分内容（items）

  比如EDS可能推部分clusters（的endpoints）

* `全量推送` 推送某类资源的全部内容

  比如LDS、RDS推送的是全部的数据

* `差量推送` 推送某个资源的变更部分

  所以，也叫`patching`

  比如要实现EDS推送某个cluster内容的变化（增删多少端点等）





#### 为什么在基本xDS协议里EDS能部分推送



**强行理解**

因为依赖多少clusters是由上层协议决定的（CDS、route config的静态部分）