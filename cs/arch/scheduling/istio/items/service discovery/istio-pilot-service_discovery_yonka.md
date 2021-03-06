

## 概念实体



### servicediscovery

这个接口大约意思就是： `ServiceDataStore`，定义了服务相关的数据的访问、获取接口



```go
// ServiceDiscovery enumerates Istio service instances.
// nolint: lll
//go:generate $GOPATH/src/istio.io/istio/bin/counterfeiter.sh -o $GOPATH/src/istio.io/istio/pilot/pkg/networking/core/v1alpha3/fakes/fake_service_discovery.go --fake-name ServiceDiscovery . ServiceDiscovery
type ServiceDiscovery interface {
	// Services list declarations of all services in the system
	Services() ([]*Service, error)

	// GetService retrieves a service by host name if it exists
	// Deprecated - do not use for anything other than tests
	GetService(hostname host.Name) (*Service, error)

	// InstancesByPort retrieves instances for a service on the given ports with labels that match
	// any of the supplied labels. All instances match an empty tag list.
	//
	// For example, consider the example of catalog.mystore.com as described in NetworkEndpoints
	// Instances(catalog.myservice.com, 80) ->
	//      --> NetworkEndpoint(172.16.0.1:8888), Service(catalog.myservice.com), Labels(foo=bar)
	//      --> NetworkEndpoint(172.16.0.2:8888), Service(catalog.myservice.com), Labels(foo=bar)
	//      --> NetworkEndpoint(172.16.0.3:8888), Service(catalog.myservice.com), Labels(kitty=cat)
	//      --> NetworkEndpoint(172.16.0.4:8888), Service(catalog.myservice.com), Labels(kitty=cat)
	//
	// Calling Instances with specific labels returns a trimmed list.
	// e.g., Instances(catalog.myservice.com, 80, foo=bar) ->
	//      --> NetworkEndpoint(172.16.0.1:8888), Service(catalog.myservice.com), Labels(foo=bar)
	//      --> NetworkEndpoint(172.16.0.2:8888), Service(catalog.myservice.com), Labels(foo=bar)
	//
	// Similar concepts apply for calling this function with a specific
	// port, hostname and labels.
	//
	// Introduced in Istio 0.8. It is only called with 1 port.
	// CDS (clusters.go) calls it for building 'dnslb' type clusters.
	// EDS calls it for building the endpoints result.
	// Consult istio-dev before using this for anything else (except debugging/tools)
	InstancesByPort(svc *Service, servicePort int, labels labels.Collection) ([]*ServiceInstance, error)

	// GetProxyServiceInstances returns the service instances that co-located with a given Proxy
	//
	// Co-located generally means running in the same network namespace and security context.
	//
	// A Proxy operating as a Sidecar will return a non-empty slice.  A stand-alone Proxy
	// will return an empty slice.
	//
	// There are two reasons why this returns multiple ServiceInstances instead of one:
	// - A ServiceInstance has a single NetworkEndpoint which has a single Port.  But a Service
	//   may have many ports.  So a workload implementing such a Service would need
	//   multiple ServiceInstances, one for each port.
	// - A single workload may implement multiple logical Services.
	//
	// In the second case, multiple services may be implemented by the same physical port number,
	// though with a different ServicePort and NetworkEndpoint for each.  If any of these overlapping
	// services are not HTTP or H2-based, behavior is undefined, since the listener may not be able to
	// determine the intended destination of a connection without a Host header on the request.
	GetProxyServiceInstances(*Proxy) ([]*ServiceInstance, error)
    
    // ...
}
```



#### 持有关系

* 1.5里
  * `Server` 
    * -持有-> `environment *model.Environment` -embed-> ServiceDiscovery(aggr registry) -聚合-> registries
    * -持有-> `EnvoyXdsServer *envoyv2.DiscoveryServer` 
      * -持有-> env

* 1.3里

  多一个`DiscoveryService`这个角色不明朗的实体，不被直接持有、持有env；



#### impls



* env

  靠embed aggr registry来impl，作为很多地方的sd访问入口

* aggr registry

* 各种实际registry

  * kube
  * consul
  * ...

* external registry

  config controller -> istio config store -> ...



### controller

这里的controller实际定义的是： 服务数据相关的事件通知（功能）

这个模型**部分被打破**了，改为注入`XDSUpdater`来让registry自己更精细的控制要触发什么操作

比如kube registry等实现



> 从如下接口定义可以知道，这个更适用于通用的事件处理逻辑



```go
type Controller interface {
	// AppendServiceHandler notifies about changes to the service catalog.
	AppendServiceHandler(f func(*Service, Event)) error

	// AppendInstanceHandler notifies about changes to the service instances
	// for a service.
	AppendInstanceHandler(f func(*ServiceInstance, Event)) error

	// Run until a signal is received
	Run(stop <-chan struct{})
}
```



### Instance

1.5里有（哪个版本引入没确认）的概念，**大约比较准确地对标了registry的概念**，而前面sd、controller准确的说都是独立功能描述接口。

这里**把aggre就排除出去**了，1.5里看只有：

* consul
* kube
* external （impl类是`ServiceEntryStore`）
* simple

> sd的话要多几个： mem、aggre、env



主要差别可以看到是：

* `Provider` 大约是类型的意思，因为之前只有clusterID概念

  之前的实现是从`aggregate.Registry`这个实现类里拿`Name`属性

* `Cluster` 等于把clusterID正式明确了

    之前只能通过`EDSUpdate`的参数传入、从`aggregate.Registry`里拿`ClusterID`属性



```go
// Instance of a service registry. A single service registry combines the capabilities of service discovery
// and the controller for managing asynchronous events.
type Instance interface {
	model.Controller
	model.ServiceDiscovery

	// Provider backing this service registry (i.e. Kubernetes, Consul, etc.)
	Provider() ProviderID

	// Cluster for which the service registry applies. Only needed for multicluster systems.
	Cluster() string
}
```



## 流程

### 数据交互流程

sd相关的组件/接口与pilot主逻辑的交互方式主要由：

* 主逻辑通过sd接口（尤其是聚合的上层接口）主动获取服务数据
* registry里在数据变更时，通过注册的svc/inst回调来把事件/变化传递给pilot主逻辑
* registry里自行通过`XDSUpdater`来传递精细的变更内容

下面逐个的看一下



#### 通过sd接口获取服务数据



* `Services() ([]*Service, error)`

  这里暂时只看通过接口（包含embed的接口）的调用，通过实现类直接调用先不看

  * `PushContext.initServiceRegistry` <- `PushContext.InitContext` <- stream连上来/要对conn进行推送时
  * 其他都是一些debug接口、aggre里对子registry的调用

* `GetService(hostname host.Name) (*Service, error)` deprecated了

  * `getGatewayAddresses` 

    1.3是pushEds是用到； 1.5是在pushContext initMeshNetworks中生成networkGateways用到

    > yonka: 有空看看差别

  * `DiscoveryServer.updateServiceShards`

    只有1.3用到： 通过registry是否有该服务来确定该服务是否属于该registry，对于不属于的，做了`s.edsUpdate(registry.ClusterID, string(svc.Hostname), svc.Attributes.Namespace, nil, true)`

    > yonka疑问： 都不属于了为啥还？

    1.5逻辑是直接判断服务的registry（name）是否与该registry同，不同则跳过

    > yonka： 极端情况，如果有同name的多个registry都提供多个该服务的话，这里迭代次数会是 `m*n` 。 不过估计istio的设计里，non-k8s registries本来就不支持multicluster，估计也不应该支持多instance 

* `GetProxyServiceInstances(*Proxy) ([]*ServiceInstance, error)`

  * `Proxy.SetServiceInstances` <- 初始conn和pushConn时调用

    结果存在`Proxy.ServiceInstances []*ServiceInstance`，这个使用的地方如下：

    * `BuildClusters`

      `inboundClusters := configgen.buildInboundClusters(env, proxy, push, instances, managementPorts)`

    * `buildGatewayListeners`

      似乎生成listener之后再拿匹配的proxy instance来作为参数内容传给plugin，没有实际的逻辑...

    * `buildSidecarInboundListeners`

      遍历每个proxy instance，...，来构建了一个listener

      > 具体的得看看生成listener的逻辑了

    * `buildSidecarOutboundListeners`

      看起来只是流程上构建build opts填充了这个数据，实际没有用到

    

* `GetProxyWorkloadLabels(*Proxy) (labels.Collection, error)`

  * `Proxy.SetWorkloadLabels` 

    > 不过这里优先用proxy解析出来的，但1.3和1.5都有bug，解析出来和设置的用同一个变量，后面就分不清了。 master上改掉了，前者放在了 `node.Metadata.Labels`

    一样的，initConn和pushConn时...

    用到`proxy.WorkloadLabels`的地方：

    * `ps.getSidecarScope` 
    * `ps.EnvoyFilters` 里遍历ns的envoyFilters然后通过labels匹配来过滤
    * `ps.mergeGateways` 看哪些gateways match到该proxy
    * `IstioConfigStore` 的一些接口支持传入workloadLabels来过滤需要的资源
    * `translateRoute`中`sourceMatchHTTP(match, node.WorkloadLabels, gatewayNames)`
    * ... 一些xds资源生成等...

    

#### Controller的回调（svc、inst）

* `NewDiscoveryServer`（or init）里给Controller（实际是aggre）注册回调

  * svc handler

    ConfigUpdate，full push，带上`ConfigsUpdated` `ServiceEntryKind`

  * inst handler

    也是 ConfigUpdate，full push，但实际上registry里都绕开了，自行调`XDSUpdater`

    > 额理想情况，1.3上来说只有kube绕开了。 consul、external都没...

> 只有这一处...



## service registry



pilot的



### aggregate

主要职责：聚合多个registries为一个，大部分逻辑都是简单的组合，**少部分内容会进行合并，几乎都是kube multicluster相关的**



#### 内容合并

再看aggre registry这一层：



##### svc



* istio只考虑k8s可能有重名的svc，而且认为重名是multicluster带来的： 对这个情况做聚合，如下

  > 其他情况都直接append，也即一旦有的话...

  * `ClusterVIPs`里聚合每个clusterID对应的clusterVIP
  * `Attributes.ClusterExternalAddresses`里聚合每个clusterID对应的外部地址，该地址可用于从外部访问该服务

  > 这两个数据的用途见《istio-concepts-Service_yonka.md》
  
  > 要求registry按序，也即primary cluster应该在前面。 而其他cluster的该服务的信息应该聚合到第一个（primary）的service实例中



##### instance

* proxy instances

  这里根据proxy instances确定了proxy的clusterID，哪个有就算哪个，神奇

  1.5里加了点逻辑，如果proxy报上来cluster（不为空）并且pilot也知道自己是哪个cluster（对于primary cluster，会用这个），那么跳过clusterId不一致的

  master上又把pilot cluster这个逻辑似乎简化掉了（待确认）





### external（`ServiceEntryStore`）



这个各版本差异较大，分版本来介绍



#### 1.3版本

> 感觉这个版本的ses实现的很糙



内容主要包括几部分：

* 一个istioConfigStore： 用于读取数据（主要是se）
* 一些本地缓存，用于自己的数据处理、提供对外接口使用： ip -> instances; svcName + ns -> instances; svcName -> svc
* 注册进来的svcHandlers、instHandlers
* 一些逻辑中用到的锁、状态标志等



##### 数据交互



* 触发、输入： new时会对configController注册se类型的事件回调，该回调作为触发和数据输入源

  `config model.Config, event model.Event`

  > 这个configController只做这个用途，所以只在new时作为参数传入， 没有持有

* 持有的istioConfigStore： 一些逻辑时需要再获取se resource



##### 数据流程



* se resource event： 转为services、instances然后调用svcHandlers、instHandlers。 同时会标记为<u>需要更新</u>，然后在对应的读取时机实际执行更新

  这样实现lazy-update，同时也避免多次update

* 数据缓存：

  * svc缓存

    * 写/更新： `GetService` 如果<u>需要更新</u>或者缓存没初始化过的话，会进行初始化 - 获取所有se然后转为services，存入缓存

      > 实现的乱七八糟，这里有只做了svc缓存更新，同时也没重置标志位

    * 读/使用： `GetService` 里取

      > `Services`方法竟然每次都直接取se然后转换，都不知道这缓存要来何用

  * inst缓存

    * 读：

      * `ip2instance`:  `GetProxyWorkloadLabels`, `GetProxyServiceInstances`
      * `instances`: `InstancesByPort`

      > 读取之前，都会判断-尝试更新缓存

    * 写：`update`

      更新逻辑没什么特别的，标志位判断 - 执行更新 - 重置标志位。

      主要逻辑是`convertInstances`

* 缓存更新 - 因为实现原因，这个特指inst缓存更新，因为只做了...

  

  

##### 数据转换、数据关系



**convertServices**



**convertInstances**



