

感觉主要是应对：

* 多registry协同

  可能提供同一个服务。 这是clusterId为registry名

* k8s的multicluster支持

  这是为k8s的clusterId


注意：

registry提供 不同namespace的服务 跟 不同cluster的服务 是不一样的，可见性不同。
前者只有对应namespace的才可见； 后者只是不同的组织/部署形式，可见性一样，会做合并。

具体可以参见 《istio_concepts_namespace_yonka.md》 和 《istio_concepts_ServiceShards_yonka.md》



## 数据



pilot侧没有定义自己的cluster描述，同时cluster也是proxy-envoy侧的概念，所以直接用了 xds的cluster数据结构



### 推送的cds数据

* `DiscoveryResponse` 

  `.Resources` -> `[]Cluster`

  * `Cluster`

    `.LoadAssignment` -> `ClusterLoadAssignment`





### `Cluster`



### `ClusterLoadAssignment`







### LbPolicy



### ClusterDiscoveryType



```go
	// Types that are valid to be assigned to ClusterDiscoveryType:
	//	*Cluster_Type
	//	*Cluster_ClusterType
	ClusterDiscoveryType isCluster_ClusterDiscoveryType `protobuf_oneof:"cluster_discovery_type"`

type Cluster_DiscoveryType int32

const (
	Cluster_STATIC       Cluster_DiscoveryType = 0
	Cluster_STRICT_DNS   Cluster_DiscoveryType = 1
	Cluster_LOGICAL_DNS  Cluster_DiscoveryType = 2
	Cluster_EDS          Cluster_DiscoveryType = 3
	Cluster_ORIGINAL_DST Cluster_DiscoveryType = 4
)
```



#### 各项解释

* static

  应该就是static cluster，显式指定cluster endpoints，同时需要是明确的内容（不需要再解析），比如 ip+port, unix socket等

* strict_dns

  这里strict的意义是严格遵守，意味着： dns查询返回0结果那么就没endpoints； 返回重复的ip那么就出现多次； 多个域名解析到同一个ip也...

  使用方式是： 把dns查询结果作为destinations，等于`destination_provider` impl是 **pick one of resolve results**

  > 这样应该会cache resolve result甚至只resolve一次（仔细看了下，似乎会有个周期性resolve），至于怎么pick one可以看策略
  >
  > 这种方式不适合dns做了选择性返回（由dns控制client请求哪些endpoints）的情况，因为这样会导致envoy频繁的调整upstreams

  > yonka疑问：这种可以多地址吧？ 跟static的区别在于endpoint是具体目标还是可（再）解析内容？

* logical_dns

  使用方式是： 把dns查询结果作为每次发起连接（比如pool中idle conns为0又没达到max conns等等）时的`destination_provider`，或者说等于其impl是： **pick the first of resolve results** 

  > 这个会导致频繁resolve，而且这样似乎不好与其他LB策略结合了
  >
  > 这个适合用在超大规模场景，这个场景下，与其dns返回大量的端点给使用方造成负担，不如考虑在dns侧先确定好为这个使用方服务的端点 --- **这个是pilot-envoy之间大量数据推送问题的一个可借鉴思路**

* eds

* original_dst

  按照能拿到的原始请求地址来转发，应该无法重路由了吧，只能做一些中间处理



#### discover type的确定方式

* 特殊场景需求
  * 比如一些特定的...需要static
    * inbound cluster
    * 特殊clusters： management、blackhole
  * mixer的cluster采取了先默认生成再在plugin里强行改为`Cluster_STRICT_DNS`的方式
* `Service.Resolution` 转换而来，除了static以外的都可以转换而来（logic_dns还没用到，不算）
* 