

## 概念实体



### 接口设计

* `ConfigStore`

  impls:

  * `config/aggregate.store` 实现了对多个configstore的聚合，支持的类型是其中的stores支持的类型的并集

* `ConfigStoreCache` embed `ConfigStore`

  > 这个名字没取好，扩展了几个方法基本都是管控相关的，如`RegisterEventHandler`, `Run`, `HasSynced` 等，更适合叫`ConfigStoreController`

  impls:

  * `config/kube/crd/controller.controller`

  * `config/kube/ingress.controller`

  * `config/memory.controller`

  * `config/aggregate.storeCache` 实现了对多个configstorecache的聚合，支持的类型是其中的stores支持的类型的并集

    实现上实际embed了一个`aggregate.store`，然后其他几个方法实现为对子storecache的分别调用

* `IstioConfigStore` embed `ConfigStore` 增加了对特定istio config types的资源的访问方法

  impls：

  * `DiscoveryService` embed `Environment` embed `IstioConfigStore`（实际是`model.istioConfigStore`）
  
  * `model.istioConfigStore` embed `ConfigStore` 对istio config resource的访问的方法做了实现
  
    资源类型传入啊、结果处理啊
  
* `CoreDataModel` embed `ConfigStoreCache`

  ```go
// CoreDataModel is a combined interface for ConfigStoreCache
  // MCP Updater and ServiceDiscovery
  type CoreDataModel interface {
  	model.ConfigStoreCache
  	sink.Updater
  }
  // Updater: Apply(*Change) error
  ```
  
  > 从这个接口既能获取，又能apply，大约猜到是个cache
  
  impls:
  
  * `coredatamodel.Controller` 实际上是个mem cache（map）
  
    这个主要用在mcp（mcp sink.Updater），mcp sink client的数据apply到这个store里
  
    

### 各种store

从`Server.configController`实际是一个聚合的configStoreCache（configStoreController），初始化时支持从`s.mesh.ConfigSources`这个列表里初始化各个source然后进行聚合。

不过，代码里支持的只有两种：

* file source： mem configStoreController + file monitor

  也即以文件作为输入，以memstore作为存储

* mcp source：coredatamodel.Controller  + mcp sink client

  也即以mcp作为输入，以coredatamodel作为存储（实质也是基于mem的store）







## 流程



### config controller初始化

以下store/controller会被聚合成一个作为总的configController，同时会用于构建总的istioConfigStore

* 主controller

  以下按序取其一

  1. 优先使用configsource来初始化（非空）

  2. 其次使用mock的`args.Config.Controller`

     > 有点莫名其妙

  3. 尝试从文件读取： `args.Config.FileDir`
  4. 从kube取

* 用于测试的memory.NewController

* ingress.NewController，也是从kube

  > `if hasKubeRegistry(args) && s.mesh.IngressControllerMode != meshconfig.MeshConfig_OFF` 才...
  >
  > yonka疑问: 这个不明白干嘛的，似乎只取virtualservice



### 配置获取

直接获取配置的操作不多，更多是通过配置变更回调，见下文

> 准确的说，其实是IstioConfigStore的形式用的不多，更多是直接调ConfigStore的`List`等接口： 大部分在pushcontext的initXX流程里 和 istioconfigstore的实现里



### 配置变化通知、处理

* `NewDiscoveryServer` 中对每种资源类型： `configCache.RegisterEventHandler(descriptor.Type, configHandler)`

  回调内容就是`ConfigUpdate`（full push），带上config描述

* `initServiceControllers`中`external.NewServiceDiscovery`时，对`ServiceEntry`类型，注册了事件回调： 转换内容为services、instances，然后调对应的handlers

  > **config的handler，并不（都？）是要多重处理的，所以这个模型可能有问题**。
  >
  > 比如这里，se类型被该handler处理，还会被全局的handler处理从而触发full-push，这个是没必要的，在新版本里已经在全局里跳过se类型了。 但这样的操作还是比较脏，是不是用 <u>覆盖</u> 或者 <u>链式+前处理器决定是不是后处理器继续处理</u> 比较好？

  > **这个处理方式可能性能非常差**： 过多的调用，虽然可能会被push debounce挡住



## 各种配置



```
IstioConfigTypes = ConfigDescriptor{
   VirtualService,
   Gateway,
   ServiceEntry,
   SyntheticServiceEntry,
   DestinationRule,
   EnvoyFilter,
   Sidecar,
   HTTPAPISpec,
   HTTPAPISpecBinding,
   QuotaSpec,
   QuotaSpecBinding,
   AuthenticationPolicy,
   AuthenticationMeshPolicy,
   ServiceRole,
   ServiceRoleBinding,
   RbacConfig,
   ClusterRbacConfig,
}
```



1.5里更详细，也做了一些分类：

* istio

  > 带`P`表示pilot需要的资源类型

  * authentication

    * meshPolicies P
    * policies P

  * config

    * adapters
    * httpApiSpecBindings P
    * httpApiSpecs P
    * templates

  * mesh

    * meshConfig

  * mixer

    * configClientQuotaSpecBindings P
    * configClientQuotaSpecs P

  * networking

    * destinationRules P

    * envoyFilters P

    * gateways P

    * serviceEntries P

    * sidecars P

    * syntheticServiceEntries P

      > 这个没有加到`collection.Pilot`里，但肯定是pilot需要的。
      >
      > 实际是： mcpController请求`Pilot`里的collections； 单独的sseMcpController请求sse

    * virtualServices P

  * policy

    * attributeManifests
    * handlers
    * instances
    * rules

  * rbac

    * clusterRbacConfigs P
    * rbacConfigs P
    * serviceRoleBindings P
    * serviceRoles P

  * security

    * authenticationPolicies P
    * peerAuthentication P
    * requestAuthentication P



```go

	// All contains all collections in the system.
	All = collection.NewSchemasBuilder().
		MustAdd(IstioAuthenticationV1Alpha1Meshpolicies).
		MustAdd(IstioAuthenticationV1Alpha1Policies).
		MustAdd(IstioConfigV1Alpha2Adapters).
		MustAdd(IstioConfigV1Alpha2Httpapispecbindings).
		MustAdd(IstioConfigV1Alpha2Httpapispecs).
		MustAdd(IstioConfigV1Alpha2Templates).
		MustAdd(IstioMeshV1Alpha1MeshConfig).
		MustAdd(IstioMixerV1ConfigClientQuotaspecbindings).
		MustAdd(IstioMixerV1ConfigClientQuotaspecs).
		MustAdd(IstioNetworkingV1Alpha3Destinationrules).
		MustAdd(IstioNetworkingV1Alpha3Envoyfilters).
		MustAdd(IstioNetworkingV1Alpha3Gateways).
		MustAdd(IstioNetworkingV1Alpha3Serviceentries).
		MustAdd(IstioNetworkingV1Alpha3Sidecars).
		MustAdd(IstioNetworkingV1Alpha3SyntheticServiceentries).
		MustAdd(IstioNetworkingV1Alpha3Virtualservices).
		MustAdd(IstioPolicyV1Beta1Attributemanifests).
		MustAdd(IstioPolicyV1Beta1Handlers).
		MustAdd(IstioPolicyV1Beta1Instances).
		MustAdd(IstioPolicyV1Beta1Rules).
		MustAdd(IstioRbacV1Alpha1Clusterrbacconfigs).
		MustAdd(IstioRbacV1Alpha1Rbacconfigs).
		MustAdd(IstioRbacV1Alpha1Servicerolebindings).
		MustAdd(IstioRbacV1Alpha1Serviceroles).
		MustAdd(IstioSecurityV1Beta1Authorizationpolicies).
		MustAdd(IstioSecurityV1Beta1Peerauthentications).
		MustAdd(IstioSecurityV1Beta1Requestauthentications).
		MustAdd(K8SApiextensionsK8SIoV1Beta1Customresourcedefinitions).
		MustAdd(K8SAppsV1Deployments).
		MustAdd(K8SAuthenticationIstioIoV1Alpha1Meshpolicies).
		MustAdd(K8SAuthenticationIstioIoV1Alpha1Policies).
		MustAdd(K8SConfigIstioIoV1Alpha2Adapters).
		MustAdd(K8SConfigIstioIoV1Alpha2Attributemanifests).
		MustAdd(K8SConfigIstioIoV1Alpha2Handlers).
		MustAdd(K8SConfigIstioIoV1Alpha2Httpapispecbindings).
		MustAdd(K8SConfigIstioIoV1Alpha2Httpapispecs).
		MustAdd(K8SConfigIstioIoV1Alpha2Instances).
		MustAdd(K8SConfigIstioIoV1Alpha2Quotaspecbindings).
		MustAdd(K8SConfigIstioIoV1Alpha2Quotaspecs).
		MustAdd(K8SConfigIstioIoV1Alpha2Rules).
		MustAdd(K8SConfigIstioIoV1Alpha2Templates).
		MustAdd(K8SCoreV1Configmaps).
		MustAdd(K8SCoreV1Endpoints).
		MustAdd(K8SCoreV1Namespaces).
		MustAdd(K8SCoreV1Nodes).
		MustAdd(K8SCoreV1Pods).
		MustAdd(K8SCoreV1Secrets).
		MustAdd(K8SCoreV1Services).
		MustAdd(K8SExtensionsV1Beta1Ingresses).
		MustAdd(K8SNetworkingIstioIoV1Alpha3Destinationrules).
		MustAdd(K8SNetworkingIstioIoV1Alpha3Envoyfilters).
		MustAdd(K8SNetworkingIstioIoV1Alpha3Gateways).
		MustAdd(K8SNetworkingIstioIoV1Alpha3Serviceentries).
		MustAdd(K8SNetworkingIstioIoV1Alpha3Sidecars).
		MustAdd(K8SNetworkingIstioIoV1Alpha3Virtualservices).
		MustAdd(K8SRbacIstioIoV1Alpha1Clusterrbacconfigs).
		MustAdd(K8SRbacIstioIoV1Alpha1Policy).
		MustAdd(K8SRbacIstioIoV1Alpha1Rbacconfigs).
		MustAdd(K8SRbacIstioIoV1Alpha1Serviceroles).
		MustAdd(K8SSecurityIstioIoV1Beta1Authorizationpolicies).
		MustAdd(K8SSecurityIstioIoV1Beta1Peerauthentications).
		MustAdd(K8SSecurityIstioIoV1Beta1Requestauthentications).
		Build()

	// Istio contains only Istio collections.
	Istio = collection.NewSchemasBuilder().
		MustAdd(IstioAuthenticationV1Alpha1Meshpolicies).
		MustAdd(IstioAuthenticationV1Alpha1Policies).
		MustAdd(IstioConfigV1Alpha2Adapters).
		MustAdd(IstioConfigV1Alpha2Httpapispecbindings).
		MustAdd(IstioConfigV1Alpha2Httpapispecs).
		MustAdd(IstioConfigV1Alpha2Templates).
		MustAdd(IstioMeshV1Alpha1MeshConfig).
		MustAdd(IstioMixerV1ConfigClientQuotaspecbindings).
		MustAdd(IstioMixerV1ConfigClientQuotaspecs).
		MustAdd(IstioNetworkingV1Alpha3Destinationrules).
		MustAdd(IstioNetworkingV1Alpha3Envoyfilters).
		MustAdd(IstioNetworkingV1Alpha3Gateways).
		MustAdd(IstioNetworkingV1Alpha3Serviceentries).
		MustAdd(IstioNetworkingV1Alpha3Sidecars).
		MustAdd(IstioNetworkingV1Alpha3SyntheticServiceentries).
		MustAdd(IstioNetworkingV1Alpha3Virtualservices).
		MustAdd(IstioPolicyV1Beta1Attributemanifests).
		MustAdd(IstioPolicyV1Beta1Handlers).
		MustAdd(IstioPolicyV1Beta1Instances).
		MustAdd(IstioPolicyV1Beta1Rules).
		MustAdd(IstioRbacV1Alpha1Clusterrbacconfigs).
		MustAdd(IstioRbacV1Alpha1Rbacconfigs).
		MustAdd(IstioRbacV1Alpha1Servicerolebindings).
		MustAdd(IstioRbacV1Alpha1Serviceroles).
		MustAdd(IstioSecurityV1Beta1Authorizationpolicies).
		MustAdd(IstioSecurityV1Beta1Peerauthentications).
		MustAdd(IstioSecurityV1Beta1Requestauthentications).
		Build()

	// Kube contains only kubernetes collections.
	Kube = collection.NewSchemasBuilder().
		MustAdd(K8SApiextensionsK8SIoV1Beta1Customresourcedefinitions).
		MustAdd(K8SAppsV1Deployments).
		MustAdd(K8SAuthenticationIstioIoV1Alpha1Meshpolicies).
		MustAdd(K8SAuthenticationIstioIoV1Alpha1Policies).
		MustAdd(K8SConfigIstioIoV1Alpha2Adapters).
		MustAdd(K8SConfigIstioIoV1Alpha2Attributemanifests).
		MustAdd(K8SConfigIstioIoV1Alpha2Handlers).
		MustAdd(K8SConfigIstioIoV1Alpha2Httpapispecbindings).
		MustAdd(K8SConfigIstioIoV1Alpha2Httpapispecs).
		MustAdd(K8SConfigIstioIoV1Alpha2Instances).
		MustAdd(K8SConfigIstioIoV1Alpha2Quotaspecbindings).
		MustAdd(K8SConfigIstioIoV1Alpha2Quotaspecs).
		MustAdd(K8SConfigIstioIoV1Alpha2Rules).
		MustAdd(K8SConfigIstioIoV1Alpha2Templates).
		MustAdd(K8SCoreV1Configmaps).
		MustAdd(K8SCoreV1Endpoints).
		MustAdd(K8SCoreV1Namespaces).
		MustAdd(K8SCoreV1Nodes).
		MustAdd(K8SCoreV1Pods).
		MustAdd(K8SCoreV1Secrets).
		MustAdd(K8SCoreV1Services).
		MustAdd(K8SExtensionsV1Beta1Ingresses).
		MustAdd(K8SNetworkingIstioIoV1Alpha3Destinationrules).
		MustAdd(K8SNetworkingIstioIoV1Alpha3Envoyfilters).
		MustAdd(K8SNetworkingIstioIoV1Alpha3Gateways).
		MustAdd(K8SNetworkingIstioIoV1Alpha3Serviceentries).
		MustAdd(K8SNetworkingIstioIoV1Alpha3Sidecars).
		MustAdd(K8SNetworkingIstioIoV1Alpha3Virtualservices).
		MustAdd(K8SRbacIstioIoV1Alpha1Clusterrbacconfigs).
		MustAdd(K8SRbacIstioIoV1Alpha1Policy).
		MustAdd(K8SRbacIstioIoV1Alpha1Rbacconfigs).
		MustAdd(K8SRbacIstioIoV1Alpha1Serviceroles).
		MustAdd(K8SSecurityIstioIoV1Beta1Authorizationpolicies).
		MustAdd(K8SSecurityIstioIoV1Beta1Peerauthentications).
		MustAdd(K8SSecurityIstioIoV1Beta1Requestauthentications).
		Build()

	// Pilot contains only collections used by Pilot.
	Pilot = collection.NewSchemasBuilder().
		MustAdd(IstioAuthenticationV1Alpha1Meshpolicies).
		MustAdd(IstioAuthenticationV1Alpha1Policies).
		MustAdd(IstioConfigV1Alpha2Httpapispecbindings).
		MustAdd(IstioConfigV1Alpha2Httpapispecs).
		MustAdd(IstioMixerV1ConfigClientQuotaspecbindings).
		MustAdd(IstioMixerV1ConfigClientQuotaspecs).
		MustAdd(IstioNetworkingV1Alpha3Destinationrules).
		MustAdd(IstioNetworkingV1Alpha3Envoyfilters).
		MustAdd(IstioNetworkingV1Alpha3Gateways).
		MustAdd(IstioNetworkingV1Alpha3Serviceentries).
		MustAdd(IstioNetworkingV1Alpha3Sidecars).
		MustAdd(IstioNetworkingV1Alpha3Virtualservices).
		MustAdd(IstioRbacV1Alpha1Clusterrbacconfigs).
		MustAdd(IstioRbacV1Alpha1Rbacconfigs).
		MustAdd(IstioRbacV1Alpha1Servicerolebindings).
		MustAdd(IstioRbacV1Alpha1Serviceroles).
		MustAdd(IstioSecurityV1Beta1Authorizationpolicies).
		MustAdd(IstioSecurityV1Beta1Peerauthentications).
		MustAdd(IstioSecurityV1Beta1Requestauthentications).
		Build()
)
```





### ServiceEntry/SSE



### VirtualService



详见：

* istio-concepts-VirtualService_yonka.md

* istio-impl-item-VirtualService_yonka.md



### DestinationRule



### Gateway



### EnvoyFilter



### Sidecars

