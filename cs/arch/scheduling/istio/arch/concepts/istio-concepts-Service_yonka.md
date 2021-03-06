

## ServiceEntry



### SyntheticServiceEntry



#### vs ServiceEntry

参考 [Clarification between Service Entry and Synthetic Service Entry #12311](https://github.com/istio/istio/issues/12311)

从字面意义理解：

* `ServiceEntry` 

* `SyntheticServiceEntry` **平台合成**的serviceEntry

  对应到实现上，galley从k8s拿到的service+endpoints组装成的ServiceEntry，source会设为`IstioNetworkingV1Alpha3SyntheticServiceentries`
  
  galley侧： sse和se type一样； collection不一样； group不一样

但如上面链接里提到的：

* sse和se要表达的东西是一样的
* 并不是平台合成的就是sse，有些实现选择直接生成/合成为se

实在是...



再看sse相关的pilot、galley侧代码演进，感觉社区对sse的概念、定位也比较模糊和摇摆。





## service实例的获取、筛选

* registry： 给出自己的全部services

  不过对于每个service，namespace只能是某个；

  没有clusterId字段，但实际上，现在没限制多个registry都提供（假设同ns的）同一个服务（的实例）；

  从 `Services()` 和 `GetService(hostname)` 接口的一致性来说，要求对于一个服务，只有一个实例（属于某一个ns）；

  > 不过考虑到 `GetService` deprecated 了，且描述为只适合用于测试。
  > 那是不是意味着，这个约束不需要成立了

* 聚合registry

  * 对于ServiceInstances： 简单聚合

  * 对于Services： 除k8s registry，也是简单聚合
  
    k8s会实际聚合为一个。 但这里有点问题： **不同ns的也聚合为1个了**
    除非说k8s没有这个情况。 但，如果`namespace`映射到`tenant`的话，总不能说用k8s registry就不用多tenant啊

    > 注释里说本意是为了聚合多cluster的。。。

  * GetService： 取第一个（但既然deprecated，忽略）  

* pushContext级别

  取所有服务，按照namespace、exportTo来分组为 privateServicesByNamespace （`map[string][]*Service`） 和 publicServices （`[]*Service`）；

  但从 `ServiceByHostnameAndNamespace map[host.Name]map[string]*Service` 来看，是**没有给同ns不同cluster（registry）的同一service留下位置的**；

* sidecarScope级别

  根据configNamespace从前面的private/public服务中选择对应的服务，再根据import条件来匹配；

  最后如果一个hostname匹配到多个ns的Service的话，只取第一个ns并记录ns依赖关系；

  > 这时理论上不排除选择了一个ns内的多个cluster的Service实例，前提是允许... 

## 同hostname的不同Service实例

* 多ns的service实例

  除了k8s registry比较奇怪以外，其他应该ok

  ```
  Also I notice that aggregate.Controller.Service()  will group all Services of k8s multicluster registries to a single one which means the namespace info is lost.
  As I understand namespace in istio as a concept similar to tenant . We could not use tenant for k8s multicluster registries case?
  ```


  Only for ServiceEntry it has multiple
  if you have a k8s registry how do you have the same hostname in different namespace though
  namespace is part of the hostname
  ```

* 多cluster的service实例

  以上，应该是不支持的

  ```
  Hey, could anyone help explain if multiple cluster(registry)s can provide model.Service instances of the same hostname (in a single namespace).

  I see `ServiceByHostnameAndNamespace map[host.Name]map[string]*Service` does not leave position for that. But obviously multiple clusters can provide model.ServiceInstances of same service.

  So cluster  concept is just a hack for k8s registry and not a formal concept of istio ?

  ```

  ```



## external/internal



* `model.Service.MeshExternal bool`

  `MeshExternal (if true) indicates that the service is external to the mesh`，后面又说`These services are defined using Istio's ServiceEntry spec`。 但显然后面这个补充是不对的。

  

  对于registry来说，有自己的理解“mesh external service”的方式； 对于serviceentry来说也有

  各处的“确定”方式：

  * external registry（from seviceentry）： `MeshExternal: serviceEntry.Location == networking.ServiceEntry_MESH_EXTERNAL`，也即完全对齐
  * kube registry： `svc.Spec.Type == coreV1.ServiceTypeExternalName && svc.Spec.ExternalName != ""`，也即只认为externalName类型的服务是外部的
  * consul registry： `endpoint.ServiceMeta[externalTagName] != ""` 

* `ServiceEntry.Location ServiceEntry_Location` 枚举，`ServiceEntry_MESH_EXTERNAL`/`ServiceEntry_MESH_INTERNAL`

  注释里定义为： `external to mesh` vs `part of mesh`。 

  补充说明了： `typically used to indicate external services consumed through APIs` vs `typically used to indicate services added explicitly as part of expanding the service mesh to include unmanaged infrastructure (e.g., VMs added to a Kubernetes based service mesh)`。 后面半句更有意义一些，强调那些 **（尤其k8s吧）unmanaged inf** 但又需要（一般是显式）接入istio的则是internal。

  从这个角度理解： <u>se配置是external、sse则是internal，确认代码逻辑如此。 当然了，dive into k8s服务模型后，sse里的externalName类型也是external</u>。

  > 但galley的source实现（其实除了kube合成sse以外只有eureka了）在构建se时没有给Location赋值，那就等于默认用了external了。 这个可能是实现混乱（vs pilot consul registry）



### 影响/作用



使用到的地方有：

* `/v1/registration`接口： 只取了internal的服务

  意思是，“系统内注册上来的服务”吧，这么说还是 <u>把“自动注册/发现”和“显式配置加入”区分</u>开

* memory registry，估计是测试等用途

> 好像没啥用，只是个概念上的区分？



## Service字段数据



### resolution



就这么几种：

* istio/proxy 不干预路由（请求发往的目标）： none/passthrough/orig_dest

* istio/proxy 把请求转给指定的目标，但该目标是hostname，所以需要通过dns来拿到具体地址，那么这个过程可以实现一个LB： dns/dnslb/strict_dns

* istio/proxy 把请求转给指定的目标**们**，可以根据一些LB算法实现它们之间的具体选择： static/clientsidelb/{static,eds}



```go
	// Resolution indicates how the service instances need to be resolved before routing
	// traffic. Most services in the service registry will use static load balancing wherein
	// the proxy will decide the service instance that will receive the traffic. Service entries
	// could either use DNS load balancing (i.e. proxy will query DNS server for the IP of the service)
	// or use the passthrough model (i.e. proxy will forward the traffic to the network endpoint requested
	// by the caller)
	Resolution Resolution
```



```go
// pilot/pkg/serviceregistry/external/conversion.go
// ServiceEntry.Resolution -> Service.Resolution

	var resolution model.Resolution
	switch serviceEntry.Resolution {
	case networking.ServiceEntry_NONE:
		resolution = model.Passthrough
	case networking.ServiceEntry_DNS:
		resolution = model.DNSLB
	case networking.ServiceEntry_STATIC:
		resolution = model.ClientSideLB
	}


const (
	// ClientSideLB implies that the proxy will decide the endpoint from its local lb pool
	ClientSideLB Resolution = iota
	// DNSLB implies that the proxy will resolve a DNS address and forward to the resolved address
	DNSLB
	// Passthrough implies that the proxy should forward traffic to the destination IP requested by the caller
	Passthrough
)


// Service.Resolution -> envoy side cluster discovery type
func convertResolution(resolution model.Resolution) apiv2.Cluster_DiscoveryType {
	switch resolution {
	case model.ClientSideLB:
		return apiv2.Cluster_EDS
	case model.DNSLB:
		return apiv2.Cluster_STRICT_DNS
	case model.Passthrough:
		return apiv2.Cluster_ORIGINAL_DST
	default:
		return apiv2.Cluster_EDS
	}
}


const (
	// Refer to the :ref:`static discovery type<arch_overview_service_discovery_types_static>`
	// for an explanation.
	Cluster_STATIC Cluster_DiscoveryType = 0
	// Refer to the :ref:`strict DNS discovery
	// type<arch_overview_service_discovery_types_strict_dns>`
	// for an explanation.
	Cluster_STRICT_DNS Cluster_DiscoveryType = 1
	// Refer to the :ref:`logical DNS discovery
	// type<arch_overview_service_discovery_types_logical_dns>`
	// for an explanation.
	Cluster_LOGICAL_DNS Cluster_DiscoveryType = 2
	// Refer to the :ref:`service discovery type<arch_overview_service_discovery_types_eds>`
	// for an explanation.
	Cluster_EDS Cluster_DiscoveryType = 3
	// Refer to the :ref:`original destination discovery
	// type<arch_overview_service_discovery_types_original_destination>`
	// for an explanation.
	Cluster_ORIGINAL_DST Cluster_DiscoveryType = 4
)

```



#### 相关逻辑



#### 使用场景

* 1

* 特定逻辑

  * 生成http路由的virtualhost domains时，对于： resolution为`Passthrough`且来自kube registry的服务，把每个domain对应的prefix通配（`*.` + domain）都加进去了

    > yonka疑问： 为什么呢？



### `Address`



**来源**

* kube service
  * 默认`UnspecifiedIP`也即`0.0.0.0`
  * clusterIp有效的话设为clusterIp



**使用场景**

* aggre registry层，merge到clusterVIPs里

* `GetServiceAddressForProxy`里如果没找到clusterVIP就用这个

  这个函数用处见下文

* build一些路由时用来做config name的一部分



### `ClusterVIPs`



使用： `GetServiceAddressForProxy(*Proxy)` 优先返回proxy cluster对应的clusterVIP，没有的话则返回service.Address

* `buildSidecarOutboundTCPListenerOptsForPortOrUDS`如果`listenerOpts.bind`为空则尝试取proxy cluster vip来作为listenAddr
* `generateVirtualHostDomains` 里把ip也当host加上
* ...





### `Service.Attributes.ClusterExternalAddresses`



```go
	// For Kubernetes platform

	// ClusterExternalAddresses is a mapping between a cluster name and the external
	// address(es) to access the service from outside the cluster.
	// Used by the aggregator to aggregate the Attributes.ClusterExternalAddresses
	// for clusters where the service resides
	ClusterExternalAddresses map[string][]string
```



**使用**

* `getGatewayAddresses`

  大致逻辑是： 优先使用网关配置里的地址（在加载时预先把host解析为ip了，所以此处只可能是ip str，如果有效的话），无效则尝试取gw的registry service name拿到svc，然后取其external addr

* `EndpointsByNetworkFilter`

  ```go
  // EndpointsByNetworkFilter is a network filter function to support Split Horizon EDS - filter the endpoints based on the network
  // of the connected sidecar. The filter will filter out all endpoints which are not present within the
  // sidecar network and add a gateway endpoint to remote networks that have endpoints (if gateway exists).
  // Information for the mesh networks is provided as a MeshNetwork config map.
  func EndpointsByNetworkFilter(endpoints []*endpoint.LocalityLbEndpoints, conn *XdsConnection, env *model.Environment) []*endpoint.LocalityLbEndpoints {
  
  ```

  注释里说的很清楚： 把所有不在proxy同网络的端点（s）替换成该网络的网关端点，当然了前提是网关存在，否则的话直接滤掉

  > 这个过程中比较有意思的是：好像会把某（其他）网络的端点的权重都加给对应的网关端点（？）

  一个network可能有多个网关； 一个网关可能有多个地址。 也即最后可能给这个network replace with 多个gw endpoints

* `pushEds`

  `if s.Env.MeshNetworks != nil && len(s.Env.MeshNetworks.Networks) > 0 {` 才会走到这个分支，多network的情况才需要考虑其他network如何访问