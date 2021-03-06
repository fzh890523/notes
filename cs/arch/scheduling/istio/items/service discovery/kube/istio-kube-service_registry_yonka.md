

## 初始化、启动

* new

  1. 构建controller
  
  2. 为每种资源类型创建informer，并且逐个注册回调。基本上各种资源都有对应的cache
  
     具体回调逻辑见下面数据获取部分

* run

  1. 异步在所有informer都synced之后启动queue
  2. 启动各个informer除了endpoints
  3. 同步等待除了endpoints以外的各个informer都synced
  4. 启动endpoints informer

  > 这里可能informer synced不代表数据都同步给了主逻辑，可能有个小gap，而在数据量大计算逻辑很多时，这个gap可能不小。
  >
  > 实践中有一些逻辑校验增强。 官方的做法在： [pull-sync initial resources in order when starting registry #26142](https://github.com/istio/istio/pull/26142/files)，相关的issue在： [pilot: grpc stream closures result in unwatched / lost ClusterLoadAssignments in Envoy; early readiness may result in incorrect configuration #24117](https://github.com/istio/istio/issues/24117)



## 模块、组件

* `kinds  map[string]cacheHandler` informers
* `queue  kube.Queue`
* queue worker： 



## 概念

### MeshNetworks

数据结构见： [mesh_network.proto](_resource/mesh_network.proto)

大致：

* k: v 方式描述的一些网络

* v（network）包含一系列endpoints和gateways

* gateway

  描述了一个网关服务，服务可以是`registry_service_name`（registry内，外部可能无法解析）或者`address`（ip或者可解析的dns名），以及port和locality信息。

  > 对于`registry_service_name`的情况，因为可能不可解析，所以实际是用这个svc name去拿到svc然后`svc.Attributes.ClusterExternalAddresses[registryName]`拿到地址，从下文可知，这个addresses是聚合多个cluster的同名服务得到的；
  >
  > 而对于可解析的`address`情况，会做数据预处理，把dns名转为ip；

* endpoint

  描述了某种属性： ip段 或者 fromRegistry。 显然ip段是网络的具体属性之一，而fromRegistry则描述了网络和registry（cluster）的关系

> 这个数据可能启用multicluster才会用到/有意义



**一些流程**

* 数据加载
  * `pilot/pkg/bootstrap/server.go::initMeshNetworks` <- `NewServer`
  * `initMeshNetworks`  同时会对`args.NetworksConfigFile`开一个fileWatcher，在文件变化时会重新加载

* 初始化入口： `Controller.InitNetworkLookup(meshNetworks *meshconfig.MeshNetworks)`

  做的事情其实是遍历所有network的所有endpoints，将其cidr网段描述都加入自己的cidranger，同时看哪个网络的fromRegistry和自己（这个Controller）的clusterId相同则确定自己属于那个网络

  

  这些信息使用的场景： 判断一个ip属于哪个网络

  1. 如果当前Controller所属的网络确定的话，那么就是这个网络

     也即默认要判断的ip都是“自己的”

  2. 如果不确定（没有拿到明确描述自己这个cluster的network），则通过cidranger来看ip在哪个网段、然后该网段对应的network

     > 如果ip属于多个网段，则打warn log，返回第一个网段的network

  

  而ip的network信息会用在`NetworkEndpoint.Network`，使用到的场景有：

  * `metadata.FilterMetadata[util.IstioMetadataKey].Fields["network"]`里下发（如果不为空）

  * 在生成eds推送（cluster endpoints）时有个networkview的过滤：只返回proxy需要的networks的eps

    这个“需要”是通过`node.Metadata[NodeMetadataRequestedNetworkView]`来描述的，没有的话会使用默认值： `UnnamedNetwork`（`""`）也即只取当前cluster的

* 初始化时机： 
  * 上面提到的`NetworksConfigFile`文件变化触发的重新加载，如果内容有变化会重新初始化： 分别对primary cluster和multicluster（的每个）
  * `initDiscoveryService`时会对primary cluster做初始化
  * multicluster的`AddMemberCluster`中会...





## 获取数据的逻辑

* 获取哪些数据：  `Services`, `Nodes`, `Pods`, `Endpoints`（1.5里允许二选一： `EndpointSlices`）

* ns： 可配置，默认（""）取全部



### 数据处理

* 除了endpoints都是通过`createCacheHandler`来注册事件（add、update、del）回调

  处理逻辑都是封装成task入queue，默认task handler时判断是否synced，如果non-synced则return err以延迟重试

  不同的资源类型，会按需append其他handler执行，见下面

* endpoints也类似，update时做equality check逻辑稍有不同，避免单纯的version变化带来的...

* 对于pod资源类型，额外构建podcache，并且把podcache.event append为handler

  逻辑见下文



**queue worker**

不断捞task，调其handler，失败（延迟）丢回queue里



### services

`AppendServiceHandler`的handler会封装以下逻辑然后append上去：

1. kube svc转为istio svc

   del的情况下可能是`DeletedFinalStateUnknown`，这时可能要从`obj.Obj`里提取

   对于externalName类型，会直接用externalName生成instances（one-per-port）

2. 根据不同的事件类型决定删除/更新缓存。 缓存有： servicesMap、externalNameSvcInstanceMap

3. `XDSUpdater.SvcUpdate` 来更新port-map
4. 调serviceHandler回调



### endpoints

类似的，逻辑如下：

1. `DeletedFinalStateUnknown`判断、提取

2. 通过`c.updateEDS`最后调`c.XDSUpdater`了，而**没有调InstanceHandler**

   > Endpoints资源中应该是包含service的全部endpoints，以`ep.Subsets`形式组织
   >
   > yonka: 但对于clusterIp类型svc，这个意义是： istio不用k8s服务模型，否则就没意义了... 晕

   逻辑如下：

   1. 遍历eps的`Subsets`，遍历ss的`Addresses`

   2. 根据ea.IP查找pod，如果不存在，则看`ea.TargetRef.Kind == Pod`，成立则说明该ea异常，跳过（其他情况允许pod不存在）

   3. 从pod里提取元信息： locality、serviceaccount、uid（mixerenabled才需要）

   4. 在遍历`ss.Ports`，以 ea + port 组合成 `IstioEndpoint`

   5. endpoints收集好后（如果是delete事件，以上流程跳过，以空endpoints作为结果来实现delete），如果启用`EnableHeadlessService`则： 判断服务是headless服务（clusterIp为None）的话，触发fullpush

      > yonka疑问: 为什么？是因为可能的inbound listener？其他的service类型没这个问题？
      >
      > 
      >
      > [Potential bug: missed EDS update for headless services #18943](https://github.com/istio/istio/issues/18943)
      >
      > > 这个误报，实际没问题
      >
      > We need full push for headless service, because it requires add/delete listeners per service instance.
      >
      > > but never updates the internal EDS data structures, because we return early
      >
      > This does not matter, as the Cluster type is `ORIGINAL_DST` rather than `EDS`.
      >
      > 
      >
      > 而且这一步似乎没必要这么晚执行？ 1.5版本优化了一下，直接触发fullPush而跳过updateEDS了，生成endpoints也不用了。
      >
      > master上又改回去了，原因是： 对于gateway，headless svc的cluster discover type是EDS（而不是orig_dest，所以还是要更新

   6. 调`XDSUpdater.EDSUpdate`



### nodes

似乎。。。1.3还没有，1.5也没。

master上有node cache以及相关逻辑了。



### pods

`PodCache.event` 主要是根据pod resource变化，更新 ip -> pod(name) 的缓存

> yonka: 能保证pod resource比endpoint resource先到吗？



## 服务数据



### controller/kube registry层

* service

  没什么特别的

* instance

  * byPort

    1. 先看`externalNameSvcInstanceMap`缓存里有没，有则（说明是externalName类型）用其（筛选后）返回

       过滤`i.Service.Attributes.Namespace == svc.Attributes.Namespace`，此外还要过滤port。 这个1.3没有而1.5有

       这里不做labels match，估计是没意义

       > 这里是防卫式写法吧，看生成这个缓存的时候，也是直接从Service实例生成的instances然后塞到缓存里覆盖了，没有考虑多ns的同Service的情况。 也即这里的instances只会是一个ns的，但极端情况有可能不是当前这个Service.ns的

    2. 从kube取endpoints resource（1.5开始也有endpointslice），转换为serviceinstance，期间会： 获取pod、从pod获取labels、做labels match过滤、从pod提取locality等元信息

  * proxyInstance

    > 这个逻辑有点复杂
    >
    > 1.5里简化成：
    >
    > * 有pod：
    >   * network不匹配： 返回空
    >   * 匹配：
    >     * 拿pod services成功且不为空： 遍历+`getProxyServiceInstancesByPod`+组合， 返回
    >     
>       **pod -> pod services -> ...**
    >     
>       1.5里虽然对podIP的认定只取第一个proxy addr，但**生成svc instances时却遍历了所有addresses**
    >     
>     * 失败或者空（认为是headless）： `GetProxyServiceInstances` 
    >     
>       **all endpoints -> match proxy ip(addresses) -> ...**
    >   
> * 没pod（认为数据还没到）： `getProxyServiceInstancesFromMetadata(proxy)`
    >
>   **dummyPod -> pod services -> ...**
    >
>   > 1.5这个分支没了对headless service的处理，没问题吗？ 

1. 根据proxyIP查pod（因为最终一致，也可能此时还不存在）
   
2. 如果存在，则尝试根据pod来生成proxy instances
   
   1. 先判断这个proxy是不是当前controller（的cluster）对应网络的，如果不是，直接返回空（不由该controller负责）
   
      > 但为什么到这里才处理？ 需要能找到pod？
   
   2. `getPodServices`（实际是拿所有services，然后筛选出selector能选中该pod的labels的），如果这里失败或者为空，也会fallback到后面的流程
   
   3. `getProxyServiceInstancesByPod` 通过 pod、svc、proxy 生成对应的instances，然后组合返回
   
      和下面dummy pod的分支的部分逻辑差不多。 根据portname查port直接传pod，然后在`pod.Spec.Containers`里遍历
   
3. 没有pod则尝试直接通过proxy来生成（`getProxyServiceInstancesFromMetadata`），如果生成失败，会继续后面的方式
   
   1. 构建一个dummyPod，传入proxy ns和`WorkloadLabels[0]`（说是k8s只用第一组labels）来请求podServices（实际上请求的是满足条件的service吧），如果没请求到服务（失败or空）都返回失败
   
      如果没有labels，无法请求，返回失败；
   
      如果`proxy.Metadata[model.NodeMetadataClusterID] != c.ClusterID`，也返回失败；
   
      > yonka疑问: 不理解的是为什么不在一开始就做这个判断？ 还是说如果网络能match表示是同一cluster？ 那为啥后续继续执行呢？
          >
      > 这个逻辑在新版本里也没改，应该没问题。 所以原因到底是？
   
   2. 从`proxy.Metadata[model.NodeMetadataPodPorts]`解析出podPorts
   
      这个metadata在启动sidecar时会通过env传入（`ISTIO_META_POD_PORTS`）
   
      podPorts不是强依赖，见下面
   
      > 1.5的podPorts是预先解析好的
   
   3. 遍历（kube）services，拿到hostname后从本地缓存拿到modelService（istio service），没找到返回失败
   
      > 也就是说如果svc不全的话，那就等下来svc来了触发的full-push吧
   
   4. 遍历`svc.Spec.Ports`，从`modelService.Ports`里取对应的，没有则返回失败。 构建serviceinstance
   
      > 所有的数据不一致都按失败处理
   
        这里如果svcPort是name类型的话，需要去podPort里查找，如果没找到也返回失败（此时依赖了podPort）
   
    4. 注释里表示，走到这里是因为headless service场景，对其进行处理
   
       > yonka： 为什么呢？ headless service前面哪个不满足？
    >
       > > 应该是service selector无法match pod吧，对于集群内的headless service，pod估计不是靠service的调度机制去部署的，所以... 
    > >
       > > 集群外的就更不用说了... 
   
       1. 遍历所有的endpoints，逐个转为service instances
        
          proxy 同ns的放在前面，注释里提到这样使得相同 addr+port 情况下，同ns的优先使用（而后面的被拒绝）
        
          1. 根据endpoints.Name、ns等生成hostname，然后拿modelService，没有则instances空
        
          2. 遍历endpoints.Subsets，遍历ss.Ports，遍历ss.Ports，跳过svc.Ports里没有的
        
          3. 对ss.Addresses和NotReadyAddresses都分别查看是否有proxyIP，有则生成对应的instance（`getProxyServiceInstancesByEndpoint`）
       
             > not ready也没问题吗？
       
       2. 所有的instances组合在一起

  



### aggregate registry层

这一层主要对几个情况作了特别处理：

* 多cluster同名服务的聚合： `ClusterVIPS`, `ClusterExternalAddress`
* proxy instances： 哪个有算哪个

详见《istio-pilot-service_discovery_yonka.md》



### xDS数据层（pilot主逻辑）



### 不同类型的服务对应的istio服务模型

#### clusterIP

> yonka: **istio不用k8s服务模型**，之前昏头了。 所以即使... 也需要拿到endpoints。



* instance

  * by port

    endpoints

  * proxy instances

    普通流程，pod services -> ...

* discover type



##### headless

> type还是clusterIp但clusterIp（或者describe输出里的IP字段）为`None`
>
> dns查询会得到endpoints



对于headless服务，istio的逻辑：

* mesh部分：

  * discover type： ORIG_DEST

    到envoy侧就对应是passthrough了吧

    > yonka疑问： 有必要吗？ 为什么不用EDS类型。 orig_dest的话没法路由？

* gateway部分： 

  * discover type： EDS



处理上的差别：

* 只能通过endpoints来得到proxy instances，无法像普通k8s服务能通过pod services



#### externalName



* instances

  * byport

    是特殊生成的，把externalName简单wrap成instance

  * proxy instances

    应该没这个概念，所以没有对应的逻辑处理这个branch

* discover type



#### lb



#### nodeport

