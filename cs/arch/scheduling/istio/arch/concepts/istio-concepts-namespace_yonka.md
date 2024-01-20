



```go
Proxy {
  // ...

	// ConfigNamespace defines the namespace where this proxy resides
	// for the purposes of network scoping.
	// NOTE: DO NOT USE THIS FIELD TO CONSTRUCT DNS NAMES
    ConfigNamespace string
    
  // ...
}

SidecarScope {
  // ...
  
	// A given hostname should only be considered in a single namespace. This mapping determines which
	// namespace a hostname exists in
  NamespaceForHostname map[host.Name]string 
  // 注意： 这里是表示，对于export to current ns并且被sidecarScope选择的服务实例，只能是单个也即来自单个ns的。 并不是说，服务只能存在于某个ns。 从他处可以看到，是允许服务同时存在于多个ns的。
  
  // ...
}

PushContext {
  // ...
  
	// privateServices are reachable within the same namespace.
	privateServicesByNamespace map[string][]*Service
	// publicServices are services reachable within the mesh.
	publicServices []*Service  
	// ServiceByHostnameAndNamespace has all services, indexed by hostname then namespace.
	ServiceByHostnameAndNamespace map[host.Name]map[string]*Service `json:"-"`

  // ...
}

updateServiceShards {
  // ... 从这里的逻辑可以理解： 
  //  不同的registry可能属于不同的namespace，因此 registry.GetService(svc.Hostname) 是不传入namespace的
  //  Services() 有几层概念： 
  //    PushContext.Services(proxy *Proxy)  
  //      proxy == nil
  //        所有namespace的所有private服务 + 所有public服务
  //      proxy != nil
  //        proxy.SidecarScope != nil && proxy.Type == SidecarProxy
  //          -> proxy.SidecarScope.Services()
  //        该proxy所属namespace的所有private服务 + 所有public服务
  //    SidecarScope.Services()  某一sidecarScope范围内的服务
  //    registry.Services()  该registry提供的(都归属于某namespace的)所有服务
}
```

综上：

* namespace更接近`tenant`概念
  
  但一个host只能属于一个namespace，这也不符合tenant需求。
  感觉就是纯粹的隔离了
* registry -> namespace 的绑定需要注意 【疑问】
  严格的说，是不支持一个registry可以取到一个服务的多namespace版本。
  也即： 取到ns1的svc1和ns2的svc2是没问题的，但取不到ns1和ns2的svc1。 囧。
  
  **但，非mcp的controller/registry都是单份，不支持多namespace多份啊，也即这些registry都无法实现多namespace**


效果：

* <del>group resource</del>isolate resource

* isolate push

  `ProxyNeedsPush` 里会考虑 `pushEv.namespacesUpdated`，如果有，并且该proxy的`SidecarScope`.`namespaceDependencies` 有这个ns的话，则需要push

  `edsUpdate`参数namespace等 -> `PushRequest.NamespacesUpdated` -> `XdsRequest.namespacesUpdated`

  > PushRequest在merge时，会对`NamespacesUpdated`进行merge


## private/public

这个目前是通过 `Service.Attributes` 中的 `Namespace` 和 `ExportTo` 共同决定的：

1. 确定ns

2. 确定本服务export到哪些scope（`map[visibility.Instance]bool`），目前定义了：

  * `Private` 

    则本ns有

  * `Public`

    则public有

  > 可见这两个可见性是互斥的，不然merge的时候要去重就麻烦了

3. “服务的意愿”优先于“默认行为”，也即有个配置可以控制“服务的意愿”不存在时默认export到public还是private

4. 最后服务会分类为 nsPrivateServices 和 publicServices

5. 确定sidecarScope依赖的服务时，会根据其sidecar资源配置来从这些服务里选取



# 资源与ns的关系



## DR

> 背景： istio在`initDestinationRules`时会把export到特定ns的同host DRs进行合并，浅合并（trafficPolicy是set-if-not-set、subset则按name做set-if-not-set



> gw/sidecar一致

`func (ps *PushContext) DestinationRule(proxy *Proxy, service *Service) *config.Config`

按照下面的顺序选取（匹配则return）

1. `proxy.ConfigNamespace`中（与svc hostname）匹配的**local** DR
2. `service.Namespace` 中匹配的DR，并且 export到 `proxy.ConfigNamespace` 的
3. `RootNamespace` 中匹配的DR，并且 export到...





## VS



### gw



`func (ps *PushContext) VirtualServicesForGateway(proxy *Proxy, gateway string) []config.Config`



1. `proxy.ConfigNamespace`中该gw的相关vs
   1. ns private
   2. export to ns
2. public中该gw...



### sidecar



`egressListener.VirtualServices()`



1. 筛选作用`mesh`的所有vs
   * `proxy.ConfigNamespace` private的
   * export to `proxy.ConfigNamespace`的
   * public的
2. 筛选与 `listenerHosts` 匹配的vs







## service



## proxy （config ns）





