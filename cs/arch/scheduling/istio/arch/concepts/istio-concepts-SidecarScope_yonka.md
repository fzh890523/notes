

## sidecarScope 

sidecarScope理解： 一组同类sidecar复用的resource scope

* 预先计算，可以避免对每个sidecar进行计算

* 多个sidecar可以match同一个sidecarScope

* 持有资源、内容

  * `EgressListeners`
  * `HasCustomIngressListeners`
  * `services`
  * `destinationRules`
  * `NamespaceForHostname`
  * `CDSOutboundClusters`
  * `OutboundTrafficPolicy`
  * `namespaceDependencies`


```go

// SidecarScope is a wrapper over the Sidecar resource with some
// preprocessed data to determine the list of services, virtualServices,
// and destinationRules that are accessible to a given
// sidecar. Precomputing the list of services, virtual services, dest rules
// for a sidecar improves performance as we no longer need to compute this
// list for every sidecar. We simply have to match a sidecar to a
// SidecarScope. Note that this is not the same as public/private scoped
// services. The list of services seen by every sidecar scope (namespace
// wide or per workload) depends on the imports, the listeners, and other
// settings.
//
// Every proxy workload of SidecarProxy type will always map to a
// SidecarScope object. If the proxy's namespace does not have a user
// specified Sidecar CRD, we will construct one that has a catch all egress
// listener that imports every public service/virtualService in the mesh.
type SidecarScope struct {
	// The crd itself. Can be nil if we are constructing the default
	// sidecar scope
	Config *Config

	// Set of egress listeners, and their associated services.  A sidecar
	// scope should have either ingress/egress listeners or both.  For
	// every proxy workload that maps to a sidecar API object (or the
	// default object), we will go through every egress listener in the
	// object and process the Envoy listener or RDS based on the imported
	// services/virtual services in that listener.
	EgressListeners []*IstioEgressListenerWrapper

	// HasCustomIngressListeners is a convenience variable that if set to
	// true indicates that the config object has one or more listeners.
	// If set to false, networking code should derive the inbound
	// listeners from the proxy service instances
	HasCustomIngressListeners bool

	// Union of services imported across all egress listeners for use by CDS code.
	// Right now, we include all the ports in these services.
	// TODO: Trim the ports in the services to only those referred to by the
	// egress listeners.
	services []*Service

	// Destination rules imported across all egress listeners. This
	// contains the computed set based on public/private destination rules
	// as well as the inherited ones, in addition to the wildcard matches
	// such as *.com applying to foo.bar.com. Each hostname in this map
	// corresponds to a service in the services array above. When computing
	// CDS, we simply have to find the matching service and return the
	// destination rule.
	destinationRules map[host.Name]*Config

	// A given hostname should only be considered in a single namespace. This mapping determines which
	// namespace a hostname exists in
	NamespaceForHostname map[host.Name]string

	// CDSOutboundClusters is the CDS output for sidecars that map to this
	// sidecarScope object. Contains the outbound clusters only, indexed
	// by localities
	CDSOutboundClusters map[string][]*xdsapi.Cluster

	// OutboundTrafficPolicy defines the outbound traffic policy for this sidecar.
	// If OutboundTrafficPolicy is ALLOW_ANY traffic to unknown destinations will
	// be forwarded.
	OutboundTrafficPolicy *networking.OutboundTrafficPolicy

	// Set of all namespaces this sidecar depends on. This is determined from the egress config
	namespaceDependencies map[string]struct{}
}
```

### 初始化sidecarScope

见 《istio-impl-item-PushContext_yonka.md》



## Sidecar 资源

该资源用于sidecar proxy的配置。

* `proxy` 作为出入流量通信的中介

* 换句话说，该资源描述了 一组出入流量通信配置相同的sidecar proxies

* 主要包括：

  * `WorkloadSelector`

    决定其作用于哪些（namespace内的）实例，默认是namespace内所有

  * `Ingress`

    * `Port *Port` listener bind到的端口（unix socket的话number为0）

      number+name+protocol 三元组

    * `Bind string` listener bind到的地址，可以是ip、unix socket

    * `CaptureMode enum CaptureMode`

      描述当前方向上的流量（中匹配该流 （bind+port描述）的部分）是如何到proxy的。

      > 考虑到实际上istio没做精细的流量劫持（比如到port级别），所以这个字段内容主要**影响对应的listener是否bindToPort**（而不是是否劫持） --- node的interceptionMode + listener的captureMode 共同决定

      * `DEFAULT`  好像没啥特别的意义，留了个默认值好兼容未设置该值的低版本，表示 `!= NONE`

      * `NONE`  不做流量劫持，那么需要bindToPort

        因为unix socket不支持劫持，所以只能是none

      * `IPTABLES` 使用iptables方式劫持（到proxy）

    * `DefaultEndpoint string` 表示流量该转发给（local的）谁，可以是network endpoint（ip+port）或者unix socket endpoint

      不支持指定特定的ip，只能是`127.0.0.1:PORT` 或者 `0.0.0.0:PORT`（会被替换为instance IP（估计是proxy IP的意思））

      > 可能防止使用者做出再转发给外部的蠢事吧

  * `Egress`

    * `Port`

    * `Bind`

    * `CaptureMode`

    * `Hosts []string` 格式`namespace/dnsName`，描述在这个listener/方向+流上提供的服务

      * `namespace` 

        > 注意： 这里的ns选择是在export to current ns的服务里再做筛选。 比如 
        > > service1的某实例（`*model.Service`）属性为ns1 + export to public（毕竟目前只支持private和public），那么 
        > > *ns2的sidecar可以感知到该服务，但这里可以再次通过sidecarScope筛选，比如`./*`，那么就看不到这个service1实例了*

        * `~` no namespace，如 `~/*`

        * `.` current ns

        * `*` any ns，如 `*/*` 

      * `dnsName`
        
        FQDN格式，允许左通配，比如`foo.example.com`， `*.example.com` 或 `*`

  * `OutboundTrafficPolicy` 设置proxy处理outbound流量时的默认行为

    默认行为apply to **unknown** traffic，实际可能在不同的层面（应用、网络）生效，具体见 《istio-impl-item-SidecarScope_yonka.md》

    目前只定义了：

    * `OutboundTrafficPolicy_REGISTRY_ONLY` 对应known traffic
    * `OutboundTrafficPolicy_ALLOW_ANY` 对应unknown traffic 

* 会convert to `SidecarScope`


```go
// Sidecar describes the configuration of the sidecar proxy that mediates
// inbound and outbound communication of the workload instance to which it is
// attached.
//
// <!-- go code generation tags
// +kubetype-gen
// +kubetype-gen:groupVersion=networking.istio.io/v1alpha3
// +genclient
// +k8s:deepcopy-gen=true
// -->
type Sidecar struct {
	// Criteria used to select the specific set of pods/VMs on which this
	// sidecar configuration should be applied. If omitted, the sidecar
	// configuration will be applied to all workload instances in the same namespace.
	WorkloadSelector *WorkloadSelector `protobuf:"bytes,1,opt,name=workload_selector,json=workloadSelector,proto3" json:"workload_selector,omitempty"`
	// Ingress specifies the configuration of the sidecar for processing
	// inbound traffic to the attached workload instance. If omitted, Istio will
	// automatically configure the sidecar based on the information about the workload
	// obtained from the orchestration platform (e.g., exposed ports, services,
	// etc.). If specified, inbound ports are configured if and only if the
	// workload instance is associated with a service.
	Ingress []*IstioIngressListener `protobuf:"bytes,2,rep,name=ingress,proto3" json:"ingress,omitempty"`
	// Egress specifies the configuration of the sidecar for processing
	// outbound traffic from the attached workload instance to other services in the
	// mesh.
	Egress []*IstioEgressListener `protobuf:"bytes,3,rep,name=egress,proto3" json:"egress,omitempty"`
	// This allows to configure the outbound traffic policy.
	// If your application uses one or more external
	// services that are not known apriori, setting the policy to ALLOW_ANY
	// will cause the sidecars to route any unknown traffic originating from
	// the application to its requested destination.
	OutboundTrafficPolicy *OutboundTrafficPolicy `protobuf:"bytes,4,opt,name=outbound_traffic_policy,json=outboundTrafficPolicy,proto3" json:"outbound_traffic_policy,omitempty"`
	XXX_NoUnkeyedLiteral  struct{}               `json:"-"`
	XXX_unrecognized      []byte                 `json:"-"`
	XXX_sizecache         int32                  `json:"-"`
}
```

```go
// IstioEgressListener specifies the properties of an outbound traffic
// listener on the sidecar proxy attached to a workload instance.
type IstioEgressListener struct {
	// The port associated with the listener. If using Unix domain socket,
	// use 0 as the port number, with a valid protocol. The port if
	// specified, will be used as the default destination port associated
	// with the imported hosts. If the port is omitted, Istio will infer the
	// listener ports based on the imported hosts. Note that when multiple
	// egress listeners are specified, where one or more listeners have
	// specific ports while others have no port, the hosts exposed on a
	// listener port will be based on the listener with the most specific
	// port.
	Port *Port `protobuf:"bytes,1,opt,name=port,proto3" json:"port,omitempty"`
	// The ip or the Unix domain socket to which the listener should be bound
	// to. Port MUST be specified if bind is not empty. Format: `x.x.x.x` or
	// `unix:///path/to/uds` or `unix://@foobar` (Linux abstract namespace). If
	// omitted, Istio will automatically configure the defaults based on imported
	// services, the workload instances to which this configuration is applied to and
	// the captureMode. If captureMode is NONE, bind will default to
	// 127.0.0.1.
	Bind string `protobuf:"bytes,2,opt,name=bind,proto3" json:"bind,omitempty"`
	// When the bind address is an IP, the captureMode option dictates
	// how traffic to the listener is expected to be captured (or not).
	// captureMode must be DEFAULT or NONE for Unix domain socket binds.
	CaptureMode CaptureMode `protobuf:"varint,3,opt,name=capture_mode,json=captureMode,proto3,enum=istio.networking.v1alpha3.CaptureMode" json:"capture_mode,omitempty"`
	// One or more service hosts exposed by the listener
	// in `namespace/dnsName` format. Services in the specified namespace
	// matching `dnsName` will be exposed.
	// The corresponding service can be a service in the service registry
	// (e.g., a Kubernetes or cloud foundry service) or a service specified
	// using a `ServiceEntry` or `VirtualService` configuration. Any
	// associated `DestinationRule` in the same namespace will also be used.
	//
	// The `dnsName` should be specified using FQDN format, optionally including
	// a wildcard character in the left-most component (e.g., `prod/*.example.com`).
	// Set the `dnsName` to `*` to select all services from the specified namespace
	// (e.g., `prod/*`).
	//
	// The `namespace` can be set to `*`, `.`, or `~`, representing any, the current,
	// or no namespace, respectively. For example, `*/foo.example.com` selects the
	// service from any available namespace while `./foo.example.com` only selects
	// the service from the namespace of the sidecar. If a host is set to `*/*`,
	// Istio will configure the sidecar to be able to reach every service in the
	// mesh that is exported to the sidecar's namespace. The value `~/*` can be used
	// to completely trim the configuration for sidecars that simply receive traffic
	// and respond, but make no outbound connections of their own.
	//
	// NOTE: Only services and configuration artifacts exported to the sidecar's
	// namespace (e.g., `exportTo` value of `*`) can be referenced.
	// Private configurations (e.g., `exportTo` set to `.`) will
	// not be available. Refer to the `exportTo` setting in `VirtualService`,
	// `DestinationRule`, and `ServiceEntry` configurations for details.
	//
	// **WARNING:** The list of egress hosts in a `Sidecar` must also include
	// the Mixer control plane services if they are enabled. Envoy will not
	// be able to reach them otherwise. For example, add host
	// `istio-system/istio-telemetry.istio-system.svc.cluster.local` if telemetry
	// is enabled, `istio-system/istio-policy.istio-system.svc.cluster.local` if
	// policy is enabled, or add `istio-system/*` to allow all services in the
	// `istio-system` namespace. This requirement is temporary and will be removed
	// in a future Istio release.
	Hosts                []string `protobuf:"bytes,4,rep,name=hosts,proto3" json:"hosts,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}
```

```go
// IstioIngressListener specifies the properties of an inbound
// traffic listener on the sidecar proxy attached to a workload instance.
type IstioIngressListener struct {
	// The port associated with the listener.
	Port *Port `protobuf:"bytes,1,opt,name=port,proto3" json:"port,omitempty"`
	// The ip to which the listener should be bound. Must be in the
	// format `x.x.x.x`. Unix domain socket addresses are not allowed in
	// the bind field for ingress listeners. If omitted, Istio will
	// automatically configure the defaults based on imported services
	// and the workload instances to which this configuration is applied
	// to.
	Bind string `protobuf:"bytes,2,opt,name=bind,proto3" json:"bind,omitempty"`
	// The captureMode option dictates how traffic to the listener is
	// expected to be captured (or not).
	CaptureMode CaptureMode `protobuf:"varint,3,opt,name=capture_mode,json=captureMode,proto3,enum=istio.networking.v1alpha3.CaptureMode" json:"capture_mode,omitempty"`
	// The loopback IP endpoint or Unix domain socket to which
	// traffic should be forwarded to. This configuration can be used to
	// redirect traffic arriving at the bind IP:Port on the sidecar to a localhost:port
	// or Unix domain socket where the application workload instance is listening for
	// connections. Format should be 127.0.0.1:PORT or `unix:///path/to/socket`
	DefaultEndpoint      string   `protobuf:"bytes,4,opt,name=default_endpoint,json=defaultEndpoint,proto3" json:"default_endpoint,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}
```

