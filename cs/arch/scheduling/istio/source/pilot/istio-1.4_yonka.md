

# 记录

## mcp



### 初始化、建连、交互



#### 初始化



* `initConfigController`时如果`ConfigSources`有则选用`MCPConfigController` 

* `initMCPConfigController`中迭代所有`ConfigSources`，逐个建立mcpController

  * 每个会有一租 grpcConn、sink.Client（as client）和coredatamodel.Controller（as configStore）

  * 这个controller的sink client关注 **除了** `schemas.SyntheticServiceEntry.Collection` 以外的所有istio资源 且是 **非**incremental的 

  * 如果这个`ConfigSource`订阅的资源里有`Resource_SERVICE_REGISTRY`，则会额外创建一个sseMCPController

    * 实际为同样的一组 grpcConn、sink.Client 和 coredatamodel.SyntheticServiceEntryController（as configStore）

    * 关于`Resource_SERVICE_REGISTRY`

      ```go
      const (
      	// Set to only receive service entries that are generated by the platform.
      	// These auto generated service entries are combination of services and endpoints
      	// that are generated by a specific platform e.g. k8
      	Resource_SERVICE_REGISTRY Resource = 0
      )
      ```

      解释里只包含`合成服务条目`，那么这个命名就欠妥了

    * 同时也可以看到，这个sink client只关注`schemas.SyntheticServiceEntry.Collection`且是incremental的

    【疑问】 这里代码写的乱七八糟，每个source的SSEMCPController都会：

    ```go
    	s.mcpDiscovery = coredatamodel.NewMCPDiscovery(s.discoveryOptions)
    	s.incrementalMcpOptions = &coredatamodel.Options{
    		DomainSuffix: args.Config.ControllerOptions.DomainSuffix,
    	}
    ```
  
    
  
  * 如上，对于sspMCPController，设计的语义功能跟实际实现和使用的功能有了偏差。 本打算用来传输特殊的数据，结果只是作为增量通道了
  
  * 最后，mcp场景下使用的configController实际上是聚合了以上所有mcp相关的configStore的`configaggregate.storeCache`



#### 建连/交互

* start时调用每个client的Run

  `initMCPConfigController`时add了startFunc

* `Run`里： conn、processStream

  提供了断连重连的机制

* `ProcessStream`

  * 尝试把initialRequests发送完，然后接收...进行处理

  * 每个接收的消息，调handleResponse来处理，处理完回复一个req

    > 这里req/resp乱七八糟的，把 S -> C的都叫resp，而 C -> S 的都叫req







### 协议通讯

* over GRPC
* 交互方式
  1. 先把req逐个发出
    init reqs来自： `state map[string]*perCollectionState`
  2. 接受resp、处理resp
  3. 对resp进行ack/nack等

```go
type ResourceSource_EstablishResourceStreamClient interface {
	Send(*RequestResources) error
	Recv() (*Resources, error)
	grpc.ClientStream
}
```

```go
// A RequestResource can be sent in two situations:
//
// Initial message in an MCP bidirectional change stream
// as an ACK or NACK response to a previous Resources. In
// this case the response_nonce is set to the nonce value
// in the Resources. ACK/NACK is determined by the presence
// of error_detail.
//
// * ACK  (nonce!="",error_details==nil)
// * NACK (nonce!="",error_details!=nil)
// * New/Update request (nonce=="",error_details ignored)
//
type RequestResources struct {
	// The sink node making the request.
	SinkNode *SinkNode `protobuf:"bytes,1,opt,name=sink_node,json=sinkNode,proto3" json:"sink_node,omitempty"`
	// Type of resource collection that is being requested, e.g.
	//
	// istio/networking/v1alpha3/VirtualService
	// k8s/<apiVersion>/<kind>
	Collection string `protobuf:"bytes,2,opt,name=collection,proto3" json:"collection,omitempty"`
	// When the RequestResources is the first in a stream, the initial_resource_versions must
	// be populated. Otherwise, initial_resource_versions must be omitted. The keys are the
	// resources names of the MCP resources known to the MCP client. The values in the map
	// are the associated resource level version info.
	InitialResourceVersions map[string]string `protobuf:"bytes,3,rep,name=initial_resource_versions,json=initialResourceVersions,proto3" json:"initial_resource_versions,omitempty" protobuf_key:"bytes,1,opt,name=key,proto3" protobuf_val:"bytes,2,opt,name=value,proto3"`
	// When the RequestResources is an ACK or NACK message in response to a previous RequestResources,
	// the response_nonce must be the nonce in the RequestResources. Otherwise response_nonce must
	// be omitted.
	ResponseNonce string `protobuf:"bytes,4,opt,name=response_nonce,json=responseNonce,proto3" json:"response_nonce,omitempty"`
	// This is populated when the previously received resources could not be applied
	// The *message* field in *error_details* provides the source internal error
	// related to the failure.
	ErrorDetail *rpc.Status `protobuf:"bytes,5,opt,name=error_detail,json=errorDetail,proto3" json:"error_detail,omitempty"`
	// Request an incremental update for the specified collection. The source may choose to
	// honor this request or ignore and and provide a full-state update in the corresponding
	// `Resource` response.
	Incremental          bool     `protobuf:"varint,6,opt,name=incremental,proto3" json:"incremental,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`

```





### 交互数据格式

* `mcp.Resources`

  主要是：

  ```go
  Collection string `protobuf:"bytes,2,opt,name=collection,proto3" json:"collection,omitempty"`  // type
  Resources []Resource `protobuf:"bytes,3,rep,name=resources,proto3" json:"resources"`
  RemovedResources []string `protobuf:"bytes,4,rep,name=removed_resources,json=removedResources,proto3" json:"removed_resources,omitempty"`
  Nonce string `protobuf:"bytes,5,opt,name=nonce,proto3" json:"nonce,omitempty"`
  Incremental          bool     `protobuf:"varint,6,opt,name=incremental,proto3" json:"incremental,omitempty"`
  ```

* `mcp.Resource`

  ```go
  Metadata *Metadata `protobuf:"bytes,1,opt,name=metadata,proto3" json:"metadata,omitempty"`
  Body                 *types.Any `protobuf:"bytes,2,opt,name=body,proto3" json:"body,omitempty"`  // payload
  ```

* resource.Body -unmarshal-> types.DynamicAny -compose-> `Object` -compose-> `Change`

  ```go
  		object := &Object{
  			TypeURL:  resource.Body.TypeUrl,
  			Metadata: resource.Metadata,
  			Body:     dynamicAny.Message,
      }
  
  	change := &Change{
  		Collection:        resources.Collection,
  		Objects:           make([]*Object, 0, len(resources.Resources)),
  		Removed:           resources.RemovedResources,
  		Incremental:       resources.Incremental,
  		SystemVersionInfo: resources.SystemVersionInfo,
  	}
  ```

* `Object`(change.Objects) -compose-> `Config` -> `ServiceEntry`

  最后实际用的还是前面的`Body:     dynamicAny.Message`



```go
// Resources do not need to include a full snapshot of the tracked
// resources. Instead they are a diff to the state of a MCP client.
// Per resource versions allow sources and sinks to track state at
// the resource granularity. An MCP incremental session is always
// in the context of a gRPC bidirectional stream. This allows the
// MCP source to keep track of the state of MCP sink connected to
// it.
//
// In Incremental MCP the nonce field is required and used to pair
// Resources to an RequestResources ACK or NACK.
type Resources struct {
	// The version of the response data (used for debugging).
	SystemVersionInfo string `protobuf:"bytes,1,opt,name=system_version_info,json=systemVersionInfo,proto3" json:"system_version_info,omitempty"`
	// Type of resource collection that is being requested, e.g.
	//
	// istio/networking/v1alpha3/VirtualService
	// k8s/<apiVersion>/<kind>
	Collection string `protobuf:"bytes,2,opt,name=collection,proto3" json:"collection,omitempty"`
	// The response resources wrapped in the common MCP *Resource* message.
	// These are typed resources that match the type url in the
	// RequestResources message.
	//
	// When `incremental` is true, this contains an array of resources to add/update
	// for the specified collection. This modifies the existing collection at the sink
	//
	// When `incremental` is false, this contains the full set of resources for the
	// specified collection. This replaces any previously delivered resources.
	Resources []Resource `protobuf:"bytes,3,rep,name=resources,proto3" json:"resources"`
	// Names of resources that have been deleted and to be
	// removed from the MCP sink node. Removed resources for missing
	// resources can be ignored.
	//
	// When `incremental` is true, this contains an array of resource names to remove
	// for the specified collection. This modifies the existing resource collection at
	// the sink.
	//
	// When `incremental` is false, this field should be ignored.
	RemovedResources []string `protobuf:"bytes,4,rep,name=removed_resources,json=removedResources,proto3" json:"removed_resources,omitempty"`
	// Required. The nonce provides a way for RequestChange to uniquely
	// reference a RequestResources.
	Nonce string `protobuf:"bytes,5,opt,name=nonce,proto3" json:"nonce,omitempty"`
	// This resource response is an incremental update. The source should only send
	// incremental updates if the sink requested them.
	Incremental          bool     `protobuf:"varint,6,opt,name=incremental,proto3" json:"incremental,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}

// Resource as transferred via the Mesh Configuration Protocol. Each
// resource is made up of common metadata, and a type-specific resource payload.
type Resource struct {
	// Common metadata describing the resource.
	Metadata *Metadata `protobuf:"bytes,1,opt,name=metadata,proto3" json:"metadata,omitempty"`
	// The primary payload for the resource.
	Body                 *types.Any `protobuf:"bytes,2,opt,name=body,proto3" json:"body,omitempty"`
	XXX_NoUnkeyedLiteral struct{}   `json:"-"`
	XXX_unrecognized     []byte     `json:"-"`
	XXX_sizecache        int32      `json:"-"`
}


// Config is a configuration unit consisting of the type of configuration, the
// key identifier that is unique per type, and the content represented as a
// protobuf message.
type Config struct {
	ConfigMeta

	// Spec holds the configuration object as a gogo protobuf message
	Spec proto.Message
}

// Object contains a decoded versioned object with metadata received from the server.
type Object struct {
	TypeURL  string
	Metadata *mcp.Metadata
	Body     proto.Message
}

// Change is a collection of configuration objects of the same protobuf type.
type Change struct {
	Collection string

	// List of resources to add/update. The interpretation of this field depends
	// on the value of Incremental.
	//
	// When Incremental=True, the list only includes new/updateReceivedForStream resources.
	//
	// When Incremental=False, the list includes the full list of resources.
	// Any previously received resources not in this list should be deleted.
	Objects []*Object

	// List of deleted resources by name. The resource name corresponds to the
	// resource's metadata name (namespace/name).
	//
	// Ignore when Incremental=false.
	Removed []string

	// When true, the set of changes represents an requestIncremental resource update. The
	// `Objects` is a list of added/update resources and `Removed` is a list of delete
	// resources.
	//
	// When false, the set of changes represents a full-state update for the specified
	// type. Any previous resources not included in this update should be removed.
	Incremental bool

	// SystemVersionInfo is the version of the response data (used for debugging purposes only).
	SystemVersionInfo string
}

// Object.Body -> Config.Spec -> .(*networking.ServiceEntry)

// ServiceEntry enables adding additional entries into Istio's internal
// service registry.
//
// <!-- go code generation tags
// +kubetype-gen
// +kubetype-gen:groupVersion=networking.istio.io/v1alpha3
// +genclient
// +k8s:deepcopy-gen=true
// -->
type ServiceEntry struct {
	// The hosts associated with the ServiceEntry. Could be a DNS
	// name with wildcard prefix.
	//
	// 1. The hosts field is used to select matching hosts in VirtualServices and DestinationRules.
	// 2. For HTTP traffic the HTTP Host/Authority header will be matched against the hosts field.
	// 3. For HTTPs or TLS traffic containing Server Name Indication (SNI), the SNI value
	// will be matched against the hosts field.
	//
	// Note that when resolution is set to type DNS
	// and no endpoints are specified, the host field will be used as the DNS name
	// of the endpoint to route traffic to.
	Hosts []string `protobuf:"bytes,1,rep,name=hosts,proto3" json:"hosts,omitempty"`
	// The virtual IP addresses associated with the service. Could be CIDR
	// prefix. For HTTP traffic, generated route configurations will include http route
	// domains for both the `addresses` and `hosts` field values and the destination will
	// be identified based on the HTTP Host/Authority header.
	// If one or more IP addresses are specified,
	// the incoming traffic will be identified as belonging to this service
	// if the destination IP matches the IP/CIDRs specified in the addresses
	// field. If the Addresses field is empty, traffic will be identified
	// solely based on the destination port. In such scenarios, the port on
	// which the service is being accessed must not be shared by any other
	// service in the mesh. In other words, the sidecar will behave as a
	// simple TCP proxy, forwarding incoming traffic on a specified port to
	// the specified destination endpoint IP/host. Unix domain socket
	// addresses are not supported in this field.
	Addresses []string `protobuf:"bytes,2,rep,name=addresses,proto3" json:"addresses,omitempty"`
	// The ports associated with the external service. If the
	// Endpoints are Unix domain socket addresses, there must be exactly one
	// port.
	Ports []*Port `protobuf:"bytes,3,rep,name=ports,proto3" json:"ports,omitempty"`
	// Specify whether the service should be considered external to the mesh
	// or part of the mesh.
	Location ServiceEntry_Location `protobuf:"varint,4,opt,name=location,proto3,enum=istio.networking.v1alpha3.ServiceEntry_Location" json:"location,omitempty"`
	// Service discovery mode for the hosts. Care must be taken
	// when setting the resolution mode to NONE for a TCP port without
	// accompanying IP addresses. In such cases, traffic to any IP on
	// said port will be allowed (i.e. 0.0.0.0:<port>).
	Resolution ServiceEntry_Resolution `protobuf:"varint,5,opt,name=resolution,proto3,enum=istio.networking.v1alpha3.ServiceEntry_Resolution" json:"resolution,omitempty"`
	// One or more endpoints associated with the service.
	Endpoints []*ServiceEntry_Endpoint `protobuf:"bytes,6,rep,name=endpoints,proto3" json:"endpoints,omitempty"`
	// A list of namespaces to which this service is exported. Exporting a service
	// allows it to be used by sidecars, gateways and virtual services defined in
	// other namespaces. This feature provides a mechanism for service owners
	// and mesh administrators to control the visibility of services across
	// namespace boundaries.
	//
	// If no namespaces are specified then the service is exported to all
	// namespaces by default.
	//
	// The value "." is reserved and defines an export to the same namespace that
	// the service is declared in. Similarly the value "*" is reserved and
	// defines an export to all namespaces.
	//
	// For a Kubernetes Service, the equivalent effect can be achieved by setting
	// the annotation "networking.istio.io/exportTo" to a comma-separated list
	// of namespace names.
	//
	// NOTE: in the current release, the `exportTo` value is restricted to
	// "." or "*" (i.e., the current namespace or all namespaces).
	ExportTo []string `protobuf:"bytes,7,rep,name=export_to,json=exportTo,proto3" json:"export_to,omitempty"`
	// The list of subject alternate names allowed for workload instances that
	// implement this service. This information is used to enforce
	// [secure-naming](https://istio.io/docs/concepts/security/#secure-naming).
	// If specified, the proxy will verify that the server
	// certificate's subject alternate name matches one of the specified values.
	SubjectAltNames      []string `protobuf:"bytes,8,rep,name=subject_alt_names,json=subjectAltNames,proto3" json:"subject_alt_names,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}

// Endpoint defines a network address (IP or hostname) associated with
// the mesh service.
type ServiceEntry_Endpoint struct {
	// Address associated with the network endpoint without the
	// port.  Domain names can be used if and only if the resolution is set
	// to DNS, and must be fully-qualified without wildcards. Use the form
	// unix:///absolute/path/to/socket for Unix domain socket endpoints.
	Address string `protobuf:"bytes,1,opt,name=address,proto3" json:"address,omitempty"`
	// Set of ports associated with the endpoint. The ports must be
	// associated with a port name that was declared as part of the
	// service. Do not use for `unix://` addresses.
	Ports map[string]uint32 `protobuf:"bytes,2,rep,name=ports,proto3" json:"ports,omitempty" protobuf_key:"bytes,1,opt,name=key,proto3" protobuf_val:"varint,2,opt,name=value,proto3"`
	// One or more labels associated with the endpoint.
	Labels map[string]string `protobuf:"bytes,3,rep,name=labels,proto3" json:"labels,omitempty" protobuf_key:"bytes,1,opt,name=key,proto3" protobuf_val:"bytes,2,opt,name=value,proto3"`
	// Network enables Istio to group endpoints resident in the same L3
	// domain/network. All endpoints in the same network are assumed to be
	// directly reachable from one another. When endpoints in different
	// networks cannot reach each other directly, an Istio Gateway can be
	// used to establish connectivity (usually using the
	// AUTO_PASSTHROUGH mode in a Gateway Server). This is
	// an advanced configuration used typically for spanning an Istio mesh
	// over multiple clusters.
	Network string `protobuf:"bytes,4,opt,name=network,proto3" json:"network,omitempty"`
	// The locality associated with the endpoint. A locality corresponds
	// to a failure domain (e.g., country/region/zone). Arbitrary failure
	// domain hierarchies can be represented by separating each
	// encapsulating failure domain by /. For example, the locality of an
	// an endpoint in US, in US-East-1 region, within availability zone
	// az-1, in data center rack r11 can be represented as
	// us/us-east-1/az-1/r11. Istio will configure the sidecar to route to
	// endpoints within the same locality as the sidecar. If none of the
	// endpoints in the locality are available, endpoints parent locality
	// (but within the same network ID) will be chosen. For example, if
	// there are two endpoints in same network (networkID "n1"), say e1
	// with locality us/us-east-1/az-1/r11 and e2 with locality
	// us/us-east-1/az-2/r12, a sidecar from us/us-east-1/az-1/r11 locality
	// will prefer e1 from the same locality over e2 from a different
	// locality. Endpoint e2 could be the IP associated with a gateway
	// (that bridges networks n1 and n2), or the IP associated with a
	// standard service endpoint.
	Locality string `protobuf:"bytes,5,opt,name=locality,proto3" json:"locality,omitempty"`
	// The load balancing weight associated with the endpoint. Endpoints
	// with higher weights will receive proportionally higher traffic.
	Weight               uint32   `protobuf:"varint,6,opt,name=weight,proto3" json:"weight,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}
```

### data flow

* `Sink.ProcessStream`
  for循环。。
  * send
  * recv
    sink.handleResponse
    * `Sink.handleResponse(*mcp.Resource)`
      unmarshal
      sink.updater.Apply(change)
      
      * `Controller.Apply`
        
        不处理`SyntheticServiceEntry`类型
        
        确实是对于该类型，做了全量替换（`c.configStore[descriptor.Type]`）。 不过 【疑问】为什么还用到了`removed`字段，将对应的版本信息删除了
        
        * 如果是`ServiceEntry`类型，触发对应的事件
        
          会进行比较，得出add/del/update(version不一致时)
        
          对于configCache/configController的eventHandler：
        
          * 在`NewDiscoveryServer`时加了一个，对于所有资源类型的的所有event都是full-push
        
        * 在serviceregistry.external的`NewServiceDiscovery`时给`ServiceEntry`类型加了一个： mark dirty（会delay到访问时才update，更新本层缓存），转为Service和Instance事件然后调对应的回调
        
            【疑问】 这个和上面的重复了吧？
        
          【疑问】 这个convert简直莫名其妙啊。 生成了一堆svc事件之后，又加了一堆instances事件（比如2w个instance的svc。。。）
        
        * 否则触发一个full-push
        
          【疑问】 这个也不管eventHandlers了？ 直接裸full-push？  感觉代码乱七八糟啊
        
      * `SyntheticServiceEntryController.Apply`
      
        * if change.Incremental: c.removeConfig(change.Remove); c.incrementalUpdate(change.Objects)
          * `SyntheticServiceEntryController.removeConfig`
          * `SyntheticServiceEntryController.incrementalUpdate`
        * else: c.configStoreUpdate(change.Objects)
          * `SyntheticServiceEntryController.configStoreUpdate`
      
        目前增量和差量更新的差别：
        * 差量是update到已有数据，要在锁中进行； 增量则是替换，数据准备阶段不需要锁，替换时需要
          存储到： 	configStore map[string]map[string]*model.Config
          
          > namespace: name: config
        
        共同点：
        
        * 都比较前后版本，<del>服务</del>**实例版本**变化时会对这个服务触发edsUpdate
      * 服务变更时，都会触发一个全量更新
        判断服务变更的条件是： 多了新的服务 或者 有**服务版本**发生变化
          * `SyntheticServiceEntryController.edsUpdate`
              config.Spec -> .(*networking.ServiceEntry)，然后convert endpoints然后 c.XDSUpdater.EDSUpdate(c.ClusterID, hostname, config.Namespace, istioEndpoints)
        
        【疑问】 这个就不用更新extenal `ServiceEntryStore`了吗？
        
        > 这个也许在store之外。。 专用一个 mcpDiscovery

以上是数据推送，而实际对数据的使用，除了edsUpdate部分以外，就是full-update部分了。
见下面的。



##### mcpController VS sseMCPController







### 数据的使用： 目前基本是ServiceEntry

* `MCPController`、`sseMCPController`上传输的都是`ServiceEntry`

* 概念上（type）里区分 `ServiceEntry` 和 `SyntheticServiceEntry`

* 使用时会分别取然后汇总

  ```go
  func (store *istioConfigStore) ServiceEntries() []Config {
  	serviceEntries, err := store.List(schemas.ServiceEntry.Type, NamespaceAll)
  	if err != nil {
  		return nil
  	}
  	supportedTypes := store.ConfigDescriptor()
  	if _, ok := supportedTypes.GetByType(schemas.SyntheticServiceEntry.Type); ok {
  		syntheticServiceEntries, err := store.List(schemas.SyntheticServiceEntry.Type, NamespaceAll)
  		if err != nil {
  			return nil
  		}
  		return append(serviceEntries, syntheticServiceEntries...)
  
  	}
  	return serviceEntries
  }
  ```

  

* g



* `ServiceEntryStore` 作为一个service registry（实现`ServiceDiscovery`接口）添加
* `ServiceEntryStore` 实现上内嵌一个`model.IstioConfigStore`
  * 从 entries -> services （一个entry可能convert为多个service）
  * instance的计算是lazy的，变更时mark，get时判断是否需要进行update
    这里有点问题， 可能并行的无谓计算太多了。 - 优化点。
    会对convert的instances进行缓存
* 这个`IstioConfigStore`的实现： wrap了一个`ConfigStore` - 获取特定的config，做必要的convert
* `ConfigStore`： 优先MCP、。。、file、。。、kube
  MCP的其实是个 aggregateMcpController，wrap了多个 `ConfigStoreCache`： ...



`ServiceEntryStore` -> `model.IstioConfigStore` -> `ConfigStore`  ---> `aggregateMcpController` -> MCP config stores(controllers)



##### `SyntheticServiceEntryController` 与 `MCPDiscovery` 的关系

 

前者存储了来自差量的ServiceEntry数据； 并且会通知后者，看似后者那也有一份，但不清楚意义。



##### `ServiceEntryStore` 和 `MCPDiscovery` 的关系

* `MCPDiscovery` 作为 `serviceregistry.MCPRegistry` 时的registry（的SD、controller）

  `SyntheticServiceEntryController`在convertConfig时会更新`MCPDiscovery`的数据`notReadyEndpoints`，看起来这个SD是 **仅sseMCPController/差量的**

  【疑问】 这个`notReadyEndpoints`到底啥意思？ 只看添加、更新，没见减少

  > 猜测的一个用途是在`ServiceEntryStore`里没某服务时在这里可能查到，但这要求排序在更后。。

* `ServiceEntryStore` `是在后面额外加入的

  ```go
  	// add service entry registry to aggregator by default
  	serviceEntryRegistry := aggregate.Registry{
  		Name:             "ServiceEntries",
  		Controller:       serviceEntryStore,
  		ServiceDiscovery: serviceEntryStore,
  	}
  	serviceControllers.AddRegistry(serviceEntryRegistry)
  ```

  





### Clarification between Service Entry and Synthetic Service Entry

ref: https://github.com/istio/istio/issues/12311

issue里提到：

* gally的使用方式为： 平台产生的 -> 合成的； 用户添加的 -> 普通。
* 其他的平台未必，比如Cloud Foundry的平台生成的。。就以普通SE形式

下面有个似乎官方的解释是说二者语义一样，只是为了实现增量而又不影响已有的code path， 我。。。



## update

大体上还是 `ConfigUpdate` -pushChannel-> `handleUpdates` worker -做一些限流-> go processUpdate -> [doPush] -> `Push` -> `AdsPushAll` -> `startPush` -wg+gouroutine/queue+goroutine-> -client.pushChannel-> `pushConnection`

`handleUpdates`区别：
* 原来只有full bool，现在传入的PushRequest对象，可以进行merge
  原先全局存了个`edsUpdates map[string]struct{}`，现在这个移到PushRequest里了（在merge时。。。）。
  merge除了edsUpdates外，还支持对 NamespacesUpdated、ConfigTypesUpdated 的merge。
  
  > 这里有个问题： 影响了namespace A的edsUpdate 1 和 影响了namespace B的edsUpdate 2，合并完就变成了 影响了namespace A和B 的 edsUpdate 1、2。 这个显然**放大了影响**，虽然后面估计是可以根据namespace找出对应的服务，但多少有些额外开销
* 做了并发限制，从代码来看，只有对 **没开启debounce且non-full的变更** 才不限制，其他的限制为并行度1
  老版本直接开goroutine


`processUpdate`区别：
* 省了一个doPush（replace edsUpdate）过程

`Push`区别：
* 主要逻辑都是updateServiceShards，其他细微区别

`updateServiceShards`区别：
* 主要是没有在这里去尝试更新EndpointShardsByService里的serviceAccount了

`AdsPushAll`区别：
* 基本没区别，根据full bool决定继续。。。还是走edsIncremental

`edsIncremental`区别：

`startPush`差别：
差别比较大
* 老版本直接遍历conn来等待每个conn（前一个push完）接受这个push（带超时控制）
      这里有个问题是此时conn上的push才“刚开始”，当然这只是次要。 真正的问题是，这里会积压任务。 越慢越积压，要计算的频次蹭蹭的上去
* 新版本通过queue（merge） + semaphore + doneCallback的方式来实现并发控制和较为精确的“push完感知”

`pushConnection`差别：
差别主要在对 edsPush 的处理上： 
* 新版本允许根据scope信息（namespacesUpdated、configTypesUpdated）来判断该proxy是否需要pushEds
  只对 edsPush。 fullPush不生效。 = = 这个也是有问题的，按需加载咋生效？ -> 还是沿用老的吧。



## full-update





ref: https://github.com/istio/istio/issues/12311

issue里提到：
* gally的使用方式为： 平台产生的 -> 合成的； 用户添加的 -> 普通。
* 其他的平台未必，比如Cloud Foundry的平台生成的。。就以普通SE形式

下面有个似乎官方的解释是说二者语义一样，只是为了实现增量而又不影响已有的code path， 我。。。





# 疑问

* `NamespacesUpdated` namespace是service的字段，但我们的场景是“多对多”，这个怎么实现？
  `Service.Attributes.Namespace`
  * `app` as namespace
    这里有个问题是，不同app会消费同一个服务，那。。。 
  * 同`app`的不同consumer可能消费的服务不同
    比如，版本1消费A/B/C而版本2消费C/D/E。  最后可能导致给版本1和2的client都推下去所有。。？
  * 如果connId as namespace？
* `NamespacesUpdated` 

* Mcp怎么做service/instance回调？
  目前看，discovery那套大部分没实现，只实现了：
  * `GetProxyServiceInstances`
  * `GetService`
  * `InstancesByPort`

* 之前的scope怎么实现的？ 
  在plugin里生成listener的路由的时候，用的是`services, _ := model.GetScopedServices(env, node)`，于是...
  对于hsf，因为是tcp类型，不生成路由，由plugin在listener内生成；
  对于hsf-over-http，由plugin覆盖原有路由。

  原版本，在`func (ps *PushContext) Services(proxy *Proxy) []*Service {` 里也做了 `GetScopedServices(ps.Env, proxy)`。

* updateCluster 和 updateServiceShards 的关系？

* `edsIncremental`

* updateCluster 和 updateClusterInc 的关系
  前者从registry拿instance然后。。。； 后者从EndpointShardsByService拿。。然后。。。
  两者之间的差别？ 或者还是那个问题，EndpointShardsByService里存一份带来的好处是？
  没看出这个**inc**体现在哪里。





# 问题

* instanceHandler 目前还是full push，跟serviceHandler完全一样
  
