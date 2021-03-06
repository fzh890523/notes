
* [istio doc/concepts/traffic-management#virtual-services](https://istio.io/docs/concepts/traffic-management/#virtual-services)


* istio的实质“服务治理”的概念模型

  没有VS/DR之前，几乎只有“服务发现”（如k8s原生的）： 

  * 全部instance做LB

  * 简单的权重支持

  * metadata

  当然了，没有VS，代码裸写route也是可以实现所有的“治理”功能，毕竟 VS/DR 模型也是要翻译为实际的route，只是裸写不具备生产力/抽象能力

* VirtualService

* DestinationRule



```yaml
apiVersion: networking.istio.io/v1alpha3
kind: VirtualService
metadata:
  name: reviews
spec:
  hosts:
  - reviews
  http:
```



* 可以route to同一个服务的不同subsets
  
  ```yaml
    - destination:
        host: reviews  
        subset: v2
  ```

* 也可以route to不同的服务

  ```yaml
  http:
  - match:
    - uri:
        prefix: /reviews
    route:
    - destination:
        host: reviews
  - match:
    - uri:
        prefix: /ratings
    route:
    - destination:
        host: ratings  
  ```
  
  看似这样的话，不同cluster可以做成不同的service。 但也有问题，比如如果要在所有clusters之间做LB怎么办？ - weighted？

* 单route多destination、带权重destination

  ```yaml
  - route:
    - destination:
        host: reviews
        subset: v1
      weight: 75
    - destination:
        host: reviews
        subset: v2
      weight: 25  
  ```

## 细节

### hosts: []string

注意，这个hosts **don’t actually have to be part of the Istio service registry, they are simply virtual destinations**

`can be an IP address, a DNS name, or, depending on the platform, a short name (such as a Kubernetes service short name) that resolves, implicitly or explicitly, to a fully qualified domain name (FQDN). You can also use wildcard (”*”) prefixes, letting you create a single set of routing rules for all matching services. Virtual service hosts don’t actually have to be part of the Istio service registry, they are simply virtual destinations. This lets you model traffic for virtual hosts that don’t have routable entries inside the mesh.`



* IP

* DNS name

* 平台自定义

* short name

  主要对于k8s，会在istio侧补充为完整FQDN，大约是 `${shortName}.${namespace}.svc.${domainSuffix}`

  > domainSuffix默认是`cluster.local`。 如：`a.default.svc.cluster.local`

* 通配pattern（prefix)

### gateways: []string

格式：

* `./<gw>` -> `<ns_from_configmeta>/<gw>`
* `<ns>/<gw>` 直接用，表示`ns_from_configmeta`中定义的gw可以自描述为作用于`ns`
* `<gw_shortname>` -> `<ns_from_configmeta>/<gw>` 老式，向下兼容
* `<gw_FQDN>`(`<gw>.<ns>`) -> `<ns>/<gw>` 老式，向下兼容

> yonka： 还好这个不支持通配。。。( ╯□╰ )



解释：

* 用于指示该VS apply到哪些gateways(没错，只是gateways)

* 用特殊的gateway name `mesh` 表示所有sidecars

  也即如果想apply到gateways ∪ sidecars的话，要将`mesh`作为其中一个“gateway”

* 为空的话会自动填充为`[]string{"mesh"}`

### exportTo: []string

export到哪些ns

* `.`
* `*`
* 比较新的版本里支持了特定ns



## route rules

### tls route rule

相比tcp route，只是多了 SNI hosts 这个路由条件。
至于ALPN等，估计还在开发中。

```go
// Describes match conditions and actions for routing unterminated TLS
// traffic (TLS/HTTPS) The following routing rule forwards unterminated TLS
// traffic arriving at port 443 of gateway called "mygateway" to internal
// services in the mesh based on the SNI value.
//
// ```yaml
// apiVersion: networking.istio.io/v1alpha3
// kind: VirtualService
// metadata:
//   name: bookinfo-sni
// spec:
//   hosts:
//   - "*.bookinfo.com"
//   gateways:
//   - mygateway
//   tls:
//   - match:
//     - port: 443
//       sniHosts:
//       - login.bookinfo.com
//     route:
//     - destination:
//         host: login.prod.svc.cluster.local
//   - match:
//     - port: 443
//       sniHosts:
//       - reviews.bookinfo.com
//     route:
//     - destination:
//         host: reviews.prod.svc.cluster.local
// ```
type TLSRoute struct {
	// Match conditions to be satisfied for the rule to be
	// activated. All conditions inside a single match block have AND
	// semantics, while the list of match blocks have OR semantics. The rule
	// is matched if any one of the match blocks succeed.
	Match []*TLSMatchAttributes `protobuf:"bytes,1,rep,name=match,proto3" json:"match,omitempty"`
	// The destination to which the connection should be forwarded to.
	Route                []*RouteDestination `protobuf:"bytes,2,rep,name=route,proto3" json:"route,omitempty"`
	XXX_NoUnkeyedLiteral struct{}            `json:"-"`
	XXX_unrecognized     []byte              `json:"-"`
	XXX_sizecache        int32               `json:"-"`
}
```

```go
// TLS connection match attributes.
type TLSMatchAttributes struct {
	// SNI (server name indicator) to match on. Wildcard prefixes
	// can be used in the SNI value, e.g., *.com will match foo.example.com
	// as well as example.com. An SNI value must be a subset (i.e., fall
	// within the domain) of the corresponding virtual serivce's hosts.
	SniHosts []string `protobuf:"bytes,1,rep,name=sni_hosts,json=sniHosts,proto3" json:"sni_hosts,omitempty"`
	// IPv4 or IPv6 ip addresses of destination with optional subnet.  E.g.,
	// a.b.c.d/xx form or just a.b.c.d.
	DestinationSubnets []string `protobuf:"bytes,2,rep,name=destination_subnets,json=destinationSubnets,proto3" json:"destination_subnets,omitempty"`
	// Specifies the port on the host that is being addressed. Many services
	// only expose a single port or label ports with the protocols they
	// support, in these cases it is not required to explicitly select the
	// port.
	Port uint32 `protobuf:"varint,3,opt,name=port,proto3" json:"port,omitempty"`
	// IPv4 or IPv6 ip address of source with optional subnet. E.g., a.b.c.d/xx
	// form or just a.b.c.d
	// $hide_from_docs
	SourceSubnet string `protobuf:"bytes,4,opt,name=source_subnet,json=sourceSubnet,proto3" json:"source_subnet,omitempty"`
	// One or more labels that constrain the applicability of a rule to
	// workloads with the given labels. If the VirtualService has a list of
	// gateways specified at the top, it should include the reserved gateway
	// `mesh` in order for this field to be applicable.
	SourceLabels map[string]string `protobuf:"bytes,5,rep,name=source_labels,json=sourceLabels,proto3" json:"source_labels,omitempty" protobuf_key:"bytes,1,opt,name=key,proto3" protobuf_val:"bytes,2,opt,name=value,proto3"`
	// Names of gateways where the rule should be applied to. Gateway names
	// at the top of the VirtualService (if any) are overridden. The gateway
	// match is independent of sourceLabels.
	Gateways             []string `protobuf:"bytes,6,rep,name=gateways,proto3" json:"gateways,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}
```

### tcp route rule

既然“四层”路由，决定的条件 和 目标 基本都是四层的：

* 条件

  * (dest) port

  * 目标地址、网段 (可以多个)

  * 来源地址、网段

  * 来源workload的labels

    可能有某种机制可以拿到请求来源的labels(根据ip查找还是？)

* 目标

  类似其他， host、subset、port、weight

```go
// Describes match conditions and actions for routing TCP traffic. The
// following routing rule forwards traffic arriving at port 27017 for
// mongo.prod.svc.cluster.local to another Mongo server on port 5555.
//
// ```yaml
// apiVersion: networking.istio.io/v1alpha3
// kind: VirtualService
// metadata:
//   name: bookinfo-Mongo
// spec:
//   hosts:
//   - mongo.prod.svc.cluster.local
//   tcp:
//   - match:
//     - port: 27017
//     route:
//     - destination:
//         host: mongo.backup.svc.cluster.local
//         port:
//           number: 5555
// ```
type TCPRoute struct {
	// Match conditions to be satisfied for the rule to be
	// activated. All conditions inside a single match block have AND
	// semantics, while the list of match blocks have OR semantics. The rule
	// is matched if any one of the match blocks succeed.
	Match []*L4MatchAttributes `protobuf:"bytes,1,rep,name=match,proto3" json:"match,omitempty"`
	// The destination to which the connection should be forwarded to.
	Route                []*RouteDestination `protobuf:"bytes,2,rep,name=route,proto3" json:"route,omitempty"`
	XXX_NoUnkeyedLiteral struct{}            `json:"-"`
	XXX_unrecognized     []byte              `json:"-"`
	XXX_sizecache        int32               `json:"-"`
}
```

```go
// L4 connection match attributes. Note that L4 connection matching support
// is incomplete.
type L4MatchAttributes struct {
	// IPv4 or IPv6 ip addresses of destination with optional subnet.  E.g.,
	// a.b.c.d/xx form or just a.b.c.d.
	DestinationSubnets []string `protobuf:"bytes,1,rep,name=destination_subnets,json=destinationSubnets,proto3" json:"destination_subnets,omitempty"`
	// Specifies the port on the host that is being addressed. Many services
	// only expose a single port or label ports with the protocols they support,
	// in these cases it is not required to explicitly select the port.
	Port uint32 `protobuf:"varint,2,opt,name=port,proto3" json:"port,omitempty"`
	// IPv4 or IPv6 ip address of source with optional subnet. E.g., a.b.c.d/xx
	// form or just a.b.c.d
	// $hide_from_docs
	SourceSubnet string `protobuf:"bytes,3,opt,name=source_subnet,json=sourceSubnet,proto3" json:"source_subnet,omitempty"`
	// One or more labels that constrain the applicability of a rule to
	// workloads with the given labels. If the VirtualService has a list of
	// gateways specified at the top, it should include the reserved gateway
	// `mesh` in order for this field to be applicable.
	SourceLabels map[string]string `protobuf:"bytes,4,rep,name=source_labels,json=sourceLabels,proto3" json:"source_labels,omitempty" protobuf_key:"bytes,1,opt,name=key,proto3" protobuf_val:"bytes,2,opt,name=value,proto3"`
	// Names of gateways where the rule should be applied to. Gateway names
	// at the top of the VirtualService (if any) are overridden. The gateway
	// match is independent of sourceLabels.
	Gateways             []string `protobuf:"bytes,5,rep,name=gateways,proto3" json:"gateways,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}
```

### http route rule

```yaml
  http:
  - match:
    - headers:
        end-user:
          exact: jason
    route:
    - destination:
        host: reviews  # 这里的host需要是istio host
        subset: v2
  - route:
    - destination:
        host: reviews
        subset: v3
```



**Unlike the virtual service’s host(s), the destination’s host must be a real destination that exists in Istio’s service registry or Envoy won’t know where to send traffic to it. This can be a mesh service with proxies or a non-mesh service added using a service entry**





