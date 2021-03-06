

```go
type VirtualService struct {
	// REQUIRED. The destination hosts to which traffic is being sent. Could
	// be a DNS name with wildcard prefix or an IP address.  Depending on the
	// platform, short-names can also be used instead of a FQDN (i.e. has no
	// dots in the name). In such a scenario, the FQDN of the host would be
	// derived based on the underlying platform.
	//
	// A single VirtualService can be used to describe all the traffic
	// properties of the corresponding hosts, including those for multiple
	// HTTP and TCP ports. Alternatively, the traffic properties of a host
	// can be defined using more than one VirtualService, with certain
	// caveats. Refer to the
	// [Operations Guide](/help/ops/traffic-management/deploy-guidelines/#multiple-virtual-services-and-destination-rules-for-the-same-host)
	// for details.
	//
	// *Note for Kubernetes users*: When short names are used (e.g. "reviews"
	// instead of "reviews.default.svc.cluster.local"), Istio will interpret
	// the short name based on the namespace of the rule, not the service. A
	// rule in the "default" namespace containing a host "reviews will be
	// interpreted as "reviews.default.svc.cluster.local", irrespective of
	// the actual namespace associated with the reviews service. _To avoid
	// potential misconfigurations, it is recommended to always use fully
	// qualified domain names over short names._
	//
	// The hosts field applies to both HTTP and TCP services. Service inside
	// the mesh, i.e., those found in the service registry, must always be
	// referred to using their alphanumeric names. IP addresses are allowed
	// only for services defined via the Gateway.
	Hosts []string `protobuf:"bytes,1,rep,name=hosts,proto3" json:"hosts,omitempty"`
	// The names of gateways that should apply these rules. If omitted, the
	// rules are only applied to sidecars inside the mesh. If a list of gateway
	// names is provided, the rules will not be applied to sidecars inside the
	// mesh unless the reserved gateway name `mesh` is included in the list.
	// Gateways defined in a different namespace can be selected by prefixing
	// the gateway name with `namespace/`.
	//
	// The selection condition imposed by this field can be overridden using
	// the source field in the match conditions of protocol-specific routes.
	Gateways []string `protobuf:"bytes,2,rep,name=gateways,proto3" json:"gateways,omitempty"`
	// An ordered list of route rules for HTTP traffic. HTTP routes will be
	// applied to platform service ports named 'http-*'/'http2-*'/'grpc-*', gateway
	// ports with protocol HTTP/HTTP2/GRPC/ TLS-terminated-HTTPS and service
	// entry ports using HTTP/HTTP2/GRPC protocols.  The first rule matching
	// an incoming request is used.
	Http []*HTTPRoute `protobuf:"bytes,3,rep,name=http,proto3" json:"http,omitempty"`
	// An ordered list of route rule for non-terminated TLS & HTTPS
	// traffic. Routing is typically performed using the SNI value presented
	// by the ClientHello message. TLS routes will be applied to platform
	// service ports named 'https-*', 'tls-*', unterminated gateway ports using
	// HTTPS/TLS protocols (i.e. with "passthrough" TLS mode) and service
	// entry ports using HTTPS/TLS protocols.  The first rule matching an
	// incoming request is used.  NOTE: Traffic 'https-*' or 'tls-*' ports
	// without associated virtual service will be treated as opaque TCP
	// traffic.
	Tls []*TLSRoute `protobuf:"bytes,5,rep,name=tls,proto3" json:"tls,omitempty"`
	// An ordered list of route rules for opaque TCP traffic. TCP routes will
	// be applied to any port that is not a HTTP or TLS port. The first rule
	// matching an incoming request is used.
	Tcp []*TCPRoute `protobuf:"bytes,4,rep,name=tcp,proto3" json:"tcp,omitempty"`
	// A list of namespaces to which this virtual service is exported. Exporting a
	// virtual service allows it to be used by sidecars and gateways defined in
	// other namespaces. This feature provides a mechanism for service owners
	// and mesh administrators to control the visibility of virtual services
	// across namespace boundaries.
	//
	// If no namespaces are specified then the virtual service is exported to all
	// namespaces by default.
	//
	// The value "." is reserved and defines an export to the same namespace that
	// the virtual service is declared in. Similarly the value "*" is reserved and
	// defines an export to all namespaces.
	//
	// NOTE: in the current release, the `exportTo` value is restricted to
	// "." or "*" (i.e., the current namespace or all namespaces).
	ExportTo []string `protobuf:"bytes,6,rep,name=export_to,json=exportTo,proto3" json:"export_to,omitempty"`
	XXX_NoUnkeyedLiteral struct{}      `json:"-"`
	XXX_unrecognized     []byte        `json:"-"`
	XXX_sizecache        int32         `json:"-"`
}
```

## common

```go
// Destination indicates the network addressable service to which the
// request/connection will be sent after processing a routing rule. The
// destination.host should unambiguously refer to a service in the service
// registry. Istio's service registry is composed of all the services found
// in the platform's service registry (e.g., Kubernetes services, Consul
// services), as well as services declared through the
// [ServiceEntry](/docs/reference/config/networking/v1alpha3/service-entry/#ServiceEntry) resource.
//
// *Note for Kubernetes users*: When short names are used (e.g. "reviews"
// instead of "reviews.default.svc.cluster.local"), Istio will interpret
// the short name based on the namespace of the rule, not the service. A
// rule in the "default" namespace containing a host "reviews will be
// interpreted as "reviews.default.svc.cluster.local", irrespective of the
// actual namespace associated with the reviews service. _To avoid potential
// misconfigurations, it is recommended to always use fully qualified
// domain names over short names._
//
// The following Kubernetes example routes all traffic by default to pods
// of the reviews service with label "version: v1" (i.e., subset v1), and
// some to subset v2, in a Kubernetes environment.
//
// ```yaml
// apiVersion: networking.istio.io/v1alpha3
// kind: VirtualService
// metadata:
//   name: reviews-route
//   namespace: foo
// spec:
//   hosts:
//   - reviews # interpreted as reviews.foo.svc.cluster.local
//   http:
//   - match:
//     - uri:
//         prefix: "/wpcatalog"
//     - uri:
//         prefix: "/consumercatalog"
//     rewrite:
//       uri: "/newcatalog"
//     route:
//     - destination:
//         host: reviews # interpreted as reviews.foo.svc.cluster.local
//         subset: v2
//   - route:
//     - destination:
//         host: reviews # interpreted as reviews.foo.svc.cluster.local
//         subset: v1
// ```
//
// And the associated DestinationRule
//
// ```yaml
// apiVersion: networking.istio.io/v1alpha3
// kind: DestinationRule
// metadata:
//   name: reviews-destination
//   namespace: foo
// spec:
//   host: reviews # interpreted as reviews.foo.svc.cluster.local
//   subsets:
//   - name: v1
//     labels:
//       version: v1
//   - name: v2
//     labels:
//       version: v2
// ```
//
// The following VirtualService sets a timeout of 5s for all calls to
// productpage.prod.svc.cluster.local service in Kubernetes. Notice that
// there are no subsets defined in this rule. Istio will fetch all
// instances of productpage.prod.svc.cluster.local service from the service
// registry and populate the sidecar's load balancing pool. Also, notice
// that this rule is set in the istio-system namespace but uses the fully
// qualified domain name of the productpage service,
// productpage.prod.svc.cluster.local. Therefore the rule's namespace does
// not have an impact in resolving the name of the productpage service.
//
// ```yaml
// apiVersion: networking.istio.io/v1alpha3
// kind: VirtualService
// metadata:
//   name: my-productpage-rule
//   namespace: istio-system
// spec:
//   hosts:
//   - productpage.prod.svc.cluster.local # ignores rule namespace
//   http:
//   - timeout: 5s
//     route:
//     - destination:
//         host: productpage.prod.svc.cluster.local
// ```
//
// To control routing for traffic bound to services outside the mesh, external
// services must first be added to Istio's internal service registry using the
// ServiceEntry resource. VirtualServices can then be defined to control traffic
// bound to these external services. For example, the following rules define a
// Service for wikipedia.org and set a timeout of 5s for http requests.
//
// ```yaml
// apiVersion: networking.istio.io/v1alpha3
// kind: ServiceEntry
// metadata:
//   name: external-svc-wikipedia
// spec:
//   hosts:
//   - wikipedia.org
//   location: MESH_EXTERNAL
//   ports:
//   - number: 80
//     name: example-http
//     protocol: HTTP
//   resolution: DNS
//
// apiVersion: networking.istio.io/v1alpha3
// kind: VirtualService
// metadata:
//   name: my-wiki-rule
// spec:
//   hosts:
//   - wikipedia.org
//   http:
//   - timeout: 5s
//     route:
//     - destination:
//         host: wikipedia.org
// ```
type Destination struct {
	// REQUIRED. The name of a service from the service registry. Service
	// names are looked up from the platform's service registry (e.g.,
	// Kubernetes services, Consul services, etc.) and from the hosts
	// declared by [ServiceEntry](/docs/reference/config/networking/v1alpha3/service-entry/#ServiceEntry). Traffic forwarded to
	// destinations that are not found in either of the two, will be dropped.
	//
	// *Note for Kubernetes users*: When short names are used (e.g. "reviews"
	// instead of "reviews.default.svc.cluster.local"), Istio will interpret
	// the short name based on the namespace of the rule, not the service. A
	// rule in the "default" namespace containing a host "reviews will be
	// interpreted as "reviews.default.svc.cluster.local", irrespective of
	// the actual namespace associated with the reviews service. _To avoid
	// potential misconfigurations, it is recommended to always use fully
	// qualified domain names over short names._
	Host string `protobuf:"bytes,1,opt,name=host,proto3" json:"host,omitempty"`
	// The name of a subset within the service. Applicable only to services
	// within the mesh. The subset must be defined in a corresponding
	// DestinationRule.
	Subset string `protobuf:"bytes,2,opt,name=subset,proto3" json:"subset,omitempty"`
	// Specifies the port on the host that is being addressed. If a service
	// exposes only a single port it is not required to explicitly select the
	// port.
	Port                 *PortSelector `protobuf:"bytes,3,opt,name=port,proto3" json:"port,omitempty"`
	XXX_NoUnkeyedLiteral struct{}      `json:"-"`
	XXX_unrecognized     []byte        `json:"-"`
	XXX_sizecache        int32         `json:"-"`
}
```

## HTTP route

### desc

```go
// Describes match conditions and actions for routing HTTP/1.1, HTTP2, and
// gRPC traffic. See VirtualService for usage examples.
type HTTPRoute struct {
	// Match conditions to be satisfied for the rule to be
	// activated. All conditions inside a single match block have AND
	// semantics, while the list of match blocks have OR semantics. The rule
	// is matched if any one of the match blocks succeed.
	Match []*HTTPMatchRequest `protobuf:"bytes,1,rep,name=match,proto3" json:"match,omitempty"`
	// A http rule can either redirect or forward (default) traffic. The
	// forwarding target can be one of several versions of a service (see
	// glossary in beginning of document). Weights associated with the
	// service version determine the proportion of traffic it receives.
	Route []*HTTPRouteDestination `protobuf:"bytes,2,rep,name=route,proto3" json:"route,omitempty"`
	// A http rule can either redirect or forward (default) traffic. If
	// traffic passthrough option is specified in the rule,
	// route/redirect will be ignored. The redirect primitive can be used to
	// send a HTTP 301 redirect to a different URI or Authority.
	Redirect *HTTPRedirect `protobuf:"bytes,3,opt,name=redirect,proto3" json:"redirect,omitempty"`
	// Rewrite HTTP URIs and Authority headers. Rewrite cannot be used with
	// Redirect primitive. Rewrite will be performed before forwarding.
	Rewrite *HTTPRewrite `protobuf:"bytes,4,opt,name=rewrite,proto3" json:"rewrite,omitempty"`
	// Deprecated. Websocket upgrades are done automatically starting from Istio 1.0.
	// $hide_from_docs
	WebsocketUpgrade bool `protobuf:"varint,5,opt,name=websocket_upgrade,json=websocketUpgrade,proto3" json:"websocket_upgrade,omitempty"`
	// Timeout for HTTP requests.
	Timeout *types.Duration `protobuf:"bytes,6,opt,name=timeout,proto3" json:"timeout,omitempty"`
	// Retry policy for HTTP requests.
	Retries *HTTPRetry `protobuf:"bytes,7,opt,name=retries,proto3" json:"retries,omitempty"`
	// Fault injection policy to apply on HTTP traffic at the client side.
	// Note that timeouts or retries will not be enabled when faults are
	// enabled on the client side.
	Fault *HTTPFaultInjection `protobuf:"bytes,8,opt,name=fault,proto3" json:"fault,omitempty"`
	// Mirror HTTP traffic to a another destination in addition to forwarding
	// the requests to the intended destination. Mirrored traffic is on a
	// best effort basis where the sidecar/gateway will not wait for the
	// mirrored cluster to respond before returning the response from the
	// original destination.  Statistics will be generated for the mirrored
	// destination.
	Mirror *Destination `protobuf:"bytes,9,opt,name=mirror,proto3" json:"mirror,omitempty"`
	// Cross-Origin Resource Sharing policy (CORS). Refer to
	// [CORS](https://developer.mozilla.org/en-US/docs/Web/HTTP/CORS)
	// for further details about cross origin resource sharing.
	CorsPolicy *CorsPolicy `protobuf:"bytes,10,opt,name=cors_policy,json=corsPolicy,proto3" json:"cors_policy,omitempty"`
	// Use of `append_headers` is deprecated. Use the `headers`
	// field instead.
	AppendHeaders map[string]string `protobuf:"bytes,11,rep,name=append_headers,json=appendHeaders,proto3" json:"append_headers,omitempty" protobuf_key:"bytes,1,opt,name=key,proto3" protobuf_val:"bytes,2,opt,name=value,proto3"` // Deprecated: Do not use.
	// Use of `remove_response_header` is deprecated. Use the `headers`
	// field instead.
	RemoveResponseHeaders []string `protobuf:"bytes,12,rep,name=remove_response_headers,json=removeResponseHeaders,proto3" json:"remove_response_headers,omitempty"` // Deprecated: Do not use.
	// Use of `append_response_headers` is deprecated. Use the `headers`
	// field instead.
	AppendResponseHeaders map[string]string `protobuf:"bytes,13,rep,name=append_response_headers,json=appendResponseHeaders,proto3" json:"append_response_headers,omitempty" protobuf_key:"bytes,1,opt,name=key,proto3" protobuf_val:"bytes,2,opt,name=value,proto3"` // Deprecated: Do not use.
	// Use of `remove_request_headers` is deprecated. Use the `headers`
	// field instead.
	RemoveRequestHeaders []string `protobuf:"bytes,14,rep,name=remove_request_headers,json=removeRequestHeaders,proto3" json:"remove_request_headers,omitempty"` // Deprecated: Do not use.
	// Use of `append_request_headers` is deprecated. Use the `headers`
	// field instead.
	AppendRequestHeaders map[string]string `protobuf:"bytes,15,rep,name=append_request_headers,json=appendRequestHeaders,proto3" json:"append_request_headers,omitempty" protobuf_key:"bytes,1,opt,name=key,proto3" protobuf_val:"bytes,2,opt,name=value,proto3"` // Deprecated: Do not use.
	// Header manipulation rules
	Headers              *Headers `protobuf:"bytes,16,opt,name=headers,proto3" json:"headers,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}
```

```go
// HttpMatchRequest specifies a set of criterion to be met in order for the
// rule to be applied to the HTTP request. For example, the following
// restricts the rule to match only requests where the URL path
// starts with /ratings/v2/ and the request contains a custom `end-user` header
// with value `jason`.
//
// ```yaml
// apiVersion: networking.istio.io/v1alpha3
// kind: VirtualService
// metadata:
//   name: ratings-route
// spec:
//   hosts:
//   - ratings.prod.svc.cluster.local
//   http:
//   - match:
//     - headers:
//         end-user:
//           exact: jason
//       uri:
//         prefix: "/ratings/v2/"
//     route:
//     - destination:
//         host: ratings.prod.svc.cluster.local
// ```
//
// HTTPMatchRequest CANNOT be empty.
type HTTPMatchRequest struct {
	// URI to match
	// values are case-sensitive and formatted as follows:
	//
	// - `exact: "value"` for exact string match
	//
	// - `prefix: "value"` for prefix-based match
	//
	// - `regex: "value"` for ECMAscript style regex-based match
	//
	Uri *StringMatch `protobuf:"bytes,1,opt,name=uri,proto3" json:"uri,omitempty"`
	// URI Scheme
	// values are case-sensitive and formatted as follows:
	//
	// - `exact: "value"` for exact string match
	//
	// - `prefix: "value"` for prefix-based match
	//
	// - `regex: "value"` for ECMAscript style regex-based match
	//
	Scheme *StringMatch `protobuf:"bytes,2,opt,name=scheme,proto3" json:"scheme,omitempty"`
	// HTTP Method
	// values are case-sensitive and formatted as follows:
	//
	// - `exact: "value"` for exact string match
	//
	// - `prefix: "value"` for prefix-based match
	//
	// - `regex: "value"` for ECMAscript style regex-based match
	//
	Method *StringMatch `protobuf:"bytes,3,opt,name=method,proto3" json:"method,omitempty"`
	// HTTP Authority
	// values are case-sensitive and formatted as follows:
	//
	// - `exact: "value"` for exact string match
	//
	// - `prefix: "value"` for prefix-based match
	//
	// - `regex: "value"` for ECMAscript style regex-based match
	//
	Authority *StringMatch `protobuf:"bytes,4,opt,name=authority,proto3" json:"authority,omitempty"`
	// The header keys must be lowercase and use hyphen as the separator,
	// e.g. _x-request-id_.
	//
	// Header values are case-sensitive and formatted as follows:
	//
	// - `exact: "value"` for exact string match
	//
	// - `prefix: "value"` for prefix-based match
	//
	// - `regex: "value"` for ECMAscript style regex-based match
	//
	// **Note:** The keys `uri`, `scheme`, `method`, and `authority` will be ignored.
	Headers map[string]*StringMatch `protobuf:"bytes,5,rep,name=headers,proto3" json:"headers,omitempty" protobuf_key:"bytes,1,opt,name=key,proto3" protobuf_val:"bytes,2,opt,name=value,proto3"`
	// Specifies the ports on the host that is being addressed. Many services
	// only expose a single port or label ports with the protocols they support,
	// in these cases it is not required to explicitly select the port.
	Port uint32 `protobuf:"varint,6,opt,name=port,proto3" json:"port,omitempty"`
	// One or more labels that constrain the applicability of a rule to
	// workloads with the given labels. If the VirtualService has a list of
	// gateways specified at the top, it should include the reserved gateway
	// `mesh` in order for this field to be applicable.
	SourceLabels map[string]string `protobuf:"bytes,7,rep,name=source_labels,json=sourceLabels,proto3" json:"source_labels,omitempty" protobuf_key:"bytes,1,opt,name=key,proto3" protobuf_val:"bytes,2,opt,name=value,proto3"`
	// Names of gateways where the rule should be applied to. Gateway names
	// at the top of the VirtualService (if any) are overridden. The gateway match is
	// independent of sourceLabels.
	Gateways             []string `protobuf:"bytes,8,rep,name=gateways,proto3" json:"gateways,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}
```

```go
// Each routing rule is associated with one or more service versions (see
// glossary in beginning of document). Weights associated with the version
// determine the proportion of traffic it receives. For example, the
// following rule will route 25% of traffic for the "reviews" service to
// instances with the "v2" tag and the remaining traffic (i.e., 75%) to
// "v1".
//
// ```yaml
// apiVersion: networking.istio.io/v1alpha3
// kind: VirtualService
// metadata:
//   name: reviews-route
// spec:
//   hosts:
//   - reviews.prod.svc.cluster.local
//   http:
//   - route:
//     - destination:
//         host: reviews.prod.svc.cluster.local
//         subset: v2
//       weight: 25
//     - destination:
//         host: reviews.prod.svc.cluster.local
//         subset: v1
//       weight: 75
// ```
//
// And the associated DestinationRule
//
// ```yaml
// apiVersion: networking.istio.io/v1alpha3
// kind: DestinationRule
// metadata:
//   name: reviews-destination
// spec:
//   host: reviews.prod.svc.cluster.local
//   subsets:
//   - name: v1
//     labels:
//       version: v1
//   - name: v2
//     labels:
//       version: v2
// ```
//
// Traffic can also be split across two entirely different services without
// having to define new subsets. For example, the following rule forwards 25% of
// traffic to reviews.com to dev.reviews.com
//
// ```yaml
// apiVersion: networking.istio.io/v1alpha3
// kind: VirtualService
// metadata:
//   name: reviews-route-two-domains
// spec:
//   hosts:
//   - reviews.com
//   http:
//   - route:
//     - destination:
//         host: dev.reviews.com
//       weight: 25
//     - destination:
//         host: reviews.com
//       weight: 75
// ```
type HTTPRouteDestination struct {
	// REQUIRED. Destination uniquely identifies the instances of a service
	// to which the request/connection should be forwarded to.
	Destination *Destination `protobuf:"bytes,1,opt,name=destination,proto3" json:"destination,omitempty"`
	// REQUIRED. The proportion of traffic to be forwarded to the service
	// version. (0-100). Sum of weights across destinations SHOULD BE == 100.
	// If there is only one destination in a rule, the weight value is assumed to
	// be 100.
	Weight int32 `protobuf:"varint,2,opt,name=weight,proto3" json:"weight,omitempty"`
	// Use of `remove_response_header` is deprecated. Use the `headers`
	// field instead.
	RemoveResponseHeaders []string `protobuf:"bytes,3,rep,name=remove_response_headers,json=removeResponseHeaders,proto3" json:"remove_response_headers,omitempty"` // Deprecated: Do not use.
	// Use of `append_response_headers` is deprecated. Use the `headers`
	// field instead.
	AppendResponseHeaders map[string]string `protobuf:"bytes,4,rep,name=append_response_headers,json=appendResponseHeaders,proto3" json:"append_response_headers,omitempty" protobuf_key:"bytes,1,opt,name=key,proto3" protobuf_val:"bytes,2,opt,name=value,proto3"` // Deprecated: Do not use.
	// Use of `remove_request_headers` is deprecated. Use the `headers`
	// field instead.
	RemoveRequestHeaders []string `protobuf:"bytes,5,rep,name=remove_request_headers,json=removeRequestHeaders,proto3" json:"remove_request_headers,omitempty"` // Deprecated: Do not use.
	// Use of `append_request_headers` is deprecated. Use the `headers`
	// field instead.
	AppendRequestHeaders map[string]string `protobuf:"bytes,6,rep,name=append_request_headers,json=appendRequestHeaders,proto3" json:"append_request_headers,omitempty" protobuf_key:"bytes,1,opt,name=key,proto3" protobuf_val:"bytes,2,opt,name=value,proto3"` // Deprecated: Do not use.
	// Header manipulation rules
	Headers              *Headers `protobuf:"bytes,7,opt,name=headers,proto3" json:"headers,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}
```

### impl



# pilot主逻辑对vs的处理



## 初始化push context中的vs

详见 [istio-impl-item-PushContext_yonka.md](istio-impl-item-PushContext_yonka.md)



## 确定要使用的vs（生成路由等）

* sm（sidecar）：
* gw： 



# envoy提供的能力

了解envoy提供的能力可以有助于理解istio服务治理的实现： 无论抽象层定义了多少概念，最终是要落到istio实现，然后落到envoy侧支持的。

## http routing

```go
// A route is both a specification of how to match a request as well as an indication of what to do
// next (e.g., redirect, forward, rewrite, etc.).
//
// .. attention::
//
//   Envoy supports routing on HTTP method via :ref:`header matching
//   <envoy_api_msg_route.HeaderMatcher>`.
// [#comment:next free field: 14]
type Route struct {
	// Route matching parameters.
	Match RouteMatch `protobuf:"bytes,1,opt,name=match,proto3" json:"match"`
	// Types that are valid to be assigned to Action:
	//	*Route_Route
	//	*Route_Redirect
	//	*Route_DirectResponse
	Action isRoute_Action `protobuf_oneof:"action"`
	// The Metadata field can be used to provide additional information
	// about the route. It can be used for configuration, stats, and logging.
	// The metadata should go under the filter namespace that will need it.
	// For instance, if the metadata is intended for the Router filter,
	// the filter name should be specified as *envoy.router*.
	Metadata *core.Metadata `protobuf:"bytes,4,opt,name=metadata,proto3" json:"metadata,omitempty"`
	// Decorator for the matched route.
	Decorator *Decorator `protobuf:"bytes,5,opt,name=decorator,proto3" json:"decorator,omitempty"`
	// The per_filter_config field can be used to provide route-specific
	// configurations for filters. The key should match the filter name, such as
	// *envoy.buffer* for the HTTP buffer filter. Use of this field is filter
	// specific; see the :ref:`HTTP filter documentation <config_http_filters>` for
	// if and how it is utilized.
	PerFilterConfig map[string]*types.Struct `protobuf:"bytes,8,rep,name=per_filter_config,json=perFilterConfig,proto3" json:"per_filter_config,omitempty" protobuf_key:"bytes,1,opt,name=key,proto3" protobuf_val:"bytes,2,opt,name=value,proto3"` // Deprecated: Do not use.
	// The per_filter_config field can be used to provide route-specific
	// configurations for filters. The key should match the filter name, such as
	// *envoy.buffer* for the HTTP buffer filter. Use of this field is filter
	// specific; see the :ref:`HTTP filter documentation <config_http_filters>` for
	// if and how it is utilized.
	TypedPerFilterConfig map[string]*types.Any `protobuf:"bytes,13,rep,name=typed_per_filter_config,json=typedPerFilterConfig,proto3" json:"typed_per_filter_config,omitempty" protobuf_key:"bytes,1,opt,name=key,proto3" protobuf_val:"bytes,2,opt,name=value,proto3"`
	// Specifies a set of headers that will be added to requests matching this
	// route. Headers specified at this level are applied before headers from the
	// enclosing :ref:`envoy_api_msg_route.VirtualHost` and
	// :ref:`envoy_api_msg_RouteConfiguration`. For more information, including details on
	// header value syntax, see the documentation on :ref:`custom request headers
	// <config_http_conn_man_headers_custom_request_headers>`.
	RequestHeadersToAdd []*core.HeaderValueOption `protobuf:"bytes,9,rep,name=request_headers_to_add,json=requestHeadersToAdd,proto3" json:"request_headers_to_add,omitempty"`
	// Specifies a list of HTTP headers that should be removed from each request
	// matching this route.
	RequestHeadersToRemove []string `protobuf:"bytes,12,rep,name=request_headers_to_remove,json=requestHeadersToRemove,proto3" json:"request_headers_to_remove,omitempty"`
	// Specifies a set of headers that will be added to responses to requests
	// matching this route. Headers specified at this level are applied before
	// headers from the enclosing :ref:`envoy_api_msg_route.VirtualHost` and
	// :ref:`envoy_api_msg_RouteConfiguration`. For more information, including
	// details on header value syntax, see the documentation on
	// :ref:`custom request headers <config_http_conn_man_headers_custom_request_headers>`.
	ResponseHeadersToAdd []*core.HeaderValueOption `protobuf:"bytes,10,rep,name=response_headers_to_add,json=responseHeadersToAdd,proto3" json:"response_headers_to_add,omitempty"`
	// Specifies a list of HTTP headers that should be removed from each response
	// to requests matching this route.
	ResponseHeadersToRemove []string `protobuf:"bytes,11,rep,name=response_headers_to_remove,json=responseHeadersToRemove,proto3" json:"response_headers_to_remove,omitempty"`
	XXX_NoUnkeyedLiteral    struct{} `json:"-"`
	XXX_unrecognized        []byte   `json:"-"`
	XXX_sizecache           int32    `json:"-"`
}
```

```go
type RouteMatch struct {
	// Types that are valid to be assigned to PathSpecifier:
	//	*RouteMatch_Prefix
	//	*RouteMatch_Path
	//	*RouteMatch_Regex
	PathSpecifier isRouteMatch_PathSpecifier `protobuf_oneof:"path_specifier"`
	// Indicates that prefix/path matching should be case insensitive. The default
	// is true.
	CaseSensitive *types.BoolValue `protobuf:"bytes,4,opt,name=case_sensitive,json=caseSensitive,proto3" json:"case_sensitive,omitempty"`
	// Indicates that the route should additionally match on a runtime key. Every time the route
	// is considered for a match, it must also fall under the percentage of matches indicated by
	// this field. For some fraction N/D, a random number in the range [0,D) is selected. If the
	// number is <= the value of the numerator N, or if the key is not present, the default
	// value, the router continues to evaluate the remaining match criteria. A runtime_fraction
	// route configuration can be used to roll out route changes in a gradual manner without full
	// code/config deploys. Refer to the :ref:`traffic shifting
	// <config_http_conn_man_route_table_traffic_splitting_shift>` docs for additional documentation.
	//
	// .. note::
	//
	//    Parsing this field is implemented such that the runtime key's data may be represented
	//    as a FractionalPercent proto represented as JSON/YAML and may also be represented as an
	//    integer with the assumption that the value is an integral percentage out of 100. For
	//    instance, a runtime key lookup returning the value "42" would parse as a FractionalPercent
	//    whose numerator is 42 and denominator is HUNDRED. This preserves legacy semantics.
	RuntimeFraction *core.RuntimeFractionalPercent `protobuf:"bytes,9,opt,name=runtime_fraction,json=runtimeFraction,proto3" json:"runtime_fraction,omitempty"`
	// Specifies a set of headers that the route should match on. The router will
	// check the request’s headers against all the specified headers in the route
	// config. A match will happen if all the headers in the route are present in
	// the request with the same values (or based on presence if the value field
	// is not in the config).
	Headers []*HeaderMatcher `protobuf:"bytes,6,rep,name=headers,proto3" json:"headers,omitempty"`
	// Specifies a set of URL query parameters on which the route should
	// match. The router will check the query string from the *path* header
	// against all the specified query parameters. If the number of specified
	// query parameters is nonzero, they all must match the *path* header's
	// query string for a match to occur.
	QueryParameters []*QueryParameterMatcher `protobuf:"bytes,7,rep,name=query_parameters,json=queryParameters,proto3" json:"query_parameters,omitempty"`
	// If specified, only gRPC requests will be matched. The router will check
	// that the content-type header has a application/grpc or one of the various
	// application/grpc+ values.
	Grpc                 *RouteMatch_GrpcRouteMatchOptions `protobuf:"bytes,8,opt,name=grpc,proto3" json:"grpc,omitempty"`
	XXX_NoUnkeyedLiteral struct{}                          `json:"-"`
	XXX_unrecognized     []byte                            `json:"-"`
	XXX_sizecache        int32                             `json:"-"`
}
```

```go
// .. attention::
//
//   Internally, Envoy always uses the HTTP/2 *:authority* header to represent the HTTP/1 *Host*
//   header. Thus, if attempting to match on *Host*, match on *:authority* instead.
//
// .. attention::
//
//   To route on HTTP method, use the special HTTP/2 *:method* header. This works for both
//   HTTP/1 and HTTP/2 as Envoy normalizes headers. E.g.,
//
//   .. code-block:: json
//
//     {
//       "name": ":method",
//       "exact_match": "POST"
//     }
//
// .. attention::
//   In the absence of any header match specifier, match will default to :ref:`present_match
//   <envoy_api_field_route.HeaderMatcher.present_match>`. i.e, a request that has the :ref:`name
//   <envoy_api_field_route.HeaderMatcher.name>` header will match, regardless of the header's
//   value.
//
type HeaderMatcher struct {
	// Specifies the name of the header in the request.
	Name string `protobuf:"bytes,1,opt,name=name,proto3" json:"name,omitempty"`
	// Specifies how the header match will be performed to route the request.
	//
	// Types that are valid to be assigned to HeaderMatchSpecifier:
	//	*HeaderMatcher_ExactMatch
	//	*HeaderMatcher_RegexMatch
	//	*HeaderMatcher_RangeMatch
	//	*HeaderMatcher_PresentMatch
	//	*HeaderMatcher_PrefixMatch
	//	*HeaderMatcher_SuffixMatch
	HeaderMatchSpecifier isHeaderMatcher_HeaderMatchSpecifier `protobuf_oneof:"header_match_specifier"`
	// If specified, the match result will be inverted before checking. Defaults to false.
	//
	// Examples:
	//
	// * The regex *\d{3}* does not match the value *1234*, so it will match when inverted.
	// * The range [-10,0) will match the value -1, so it will not match when inverted.
	InvertMatch          bool     `protobuf:"varint,8,opt,name=invert_match,json=invertMatch,proto3" json:"invert_match,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}
```

```go
// Query parameter matching treats the query string of a request's :path header
// as an ampersand-separated list of keys and/or key=value elements.
type QueryParameterMatcher struct {
	// Specifies the name of a key that must be present in the requested
	// *path*'s query string.
	Name string `protobuf:"bytes,1,opt,name=name,proto3" json:"name,omitempty"`
	// Specifies the value of the key. If the value is absent, a request
	// that contains the key in its query string will match, whether the
	// key appears with a value (e.g., "?debug=true") or not (e.g., "?debug")
	Value string `protobuf:"bytes,3,opt,name=value,proto3" json:"value,omitempty"`
	// Specifies whether the query parameter value is a regular expression.
	// Defaults to false. The entire query parameter value (i.e., the part to
	// the right of the equals sign in "key=value") must match the regex.
	// E.g., the regex "\d+$" will match "123" but not "a123" or "123a".
	Regex                *types.BoolValue `protobuf:"bytes,4,opt,name=regex,proto3" json:"regex,omitempty"`
	XXX_NoUnkeyedLiteral struct{}         `json:"-"`
	XXX_unrecognized     []byte           `json:"-"`
	XXX_sizecache        int32            `json:"-"`
}
```

```go
type Route_Route struct {
	Route *RouteAction `protobuf:"bytes,2,opt,name=route,proto3,oneof"`
}
type Route_Redirect struct {
	Redirect *RedirectAction `protobuf:"bytes,3,opt,name=redirect,proto3,oneof"`
}
type Route_DirectResponse struct {
	DirectResponse *DirectResponseAction `protobuf:"bytes,7,opt,name=direct_response,json=directResponse,proto3,oneof"`
}

// [#comment:next free field: 27]
type RouteAction struct {
	// Types that are valid to be assigned to ClusterSpecifier:
	//	*RouteAction_Cluster
	//	*RouteAction_ClusterHeader
	//	*RouteAction_WeightedClusters
	ClusterSpecifier isRouteAction_ClusterSpecifier `protobuf_oneof:"cluster_specifier"`
	// The HTTP status code to use when configured cluster is not found.
	// The default response code is 503 Service Unavailable.
	ClusterNotFoundResponseCode RouteAction_ClusterNotFoundResponseCode `protobuf:"varint,20,opt,name=cluster_not_found_response_code,json=clusterNotFoundResponseCode,proto3,enum=envoy.api.v2.route.RouteAction_ClusterNotFoundResponseCode" json:"cluster_not_found_response_code,omitempty"`
	// Optional endpoint metadata match criteria used by the subset load balancer. Only endpoints
	// in the upstream cluster with metadata matching what's set in this field will be considered
	// for load balancing. If using :ref:`weighted_clusters
	// <envoy_api_field_route.RouteAction.weighted_clusters>`, metadata will be merged, with values
	// provided there taking precedence. The filter name should be specified as *envoy.lb*.
	MetadataMatch *core.Metadata `protobuf:"bytes,4,opt,name=metadata_match,json=metadataMatch,proto3" json:"metadata_match,omitempty"`
	// Indicates that during forwarding, the matched prefix (or path) should be
	// swapped with this value. This option allows application URLs to be rooted
	// at a different path from those exposed at the reverse proxy layer. The router filter will
	// place the original path before rewrite into the :ref:`x-envoy-original-path
	// <config_http_filters_router_x-envoy-original-path>` header.
	//
	// .. attention::
	//
	//   Pay careful attention to the use of trailing slashes in the
	//   :ref:`route's match <envoy_api_field_route.Route.match>` prefix value.
	//   Stripping a prefix from a path requires multiple Routes to handle all cases. For example,
	//   rewriting */prefix* to */* and */prefix/etc* to */etc* cannot be done in a single
	//   :ref:`Route <envoy_api_msg_route.Route>`, as shown by the below config entries:
	//
	//   .. code-block:: yaml
	//
	//     - match:
	//         prefix: "/prefix/"
	//       route:
	//         prefix_rewrite: "/"
	//     - match:
	//         prefix: "/prefix"
	//       route:
	//         prefix_rewrite: "/"
	//
	//   Having above entries in the config, requests to */prefix* will be stripped to */*, while
	//   requests to */prefix/etc* will be stripped to */etc*.
	PrefixRewrite string `protobuf:"bytes,5,opt,name=prefix_rewrite,json=prefixRewrite,proto3" json:"prefix_rewrite,omitempty"`
	// Types that are valid to be assigned to HostRewriteSpecifier:
	//	*RouteAction_HostRewrite
	//	*RouteAction_AutoHostRewrite
	HostRewriteSpecifier isRouteAction_HostRewriteSpecifier `protobuf_oneof:"host_rewrite_specifier"`
	// Specifies the upstream timeout for the route. If not specified, the default is 15s. This
	// spans between the point at which the entire downstream request (i.e. end-of-stream) has been
	// processed and when the upstream response has been completely processed.
	//
	// .. note::
	//
	//   This timeout includes all retries. See also
	//   :ref:`config_http_filters_router_x-envoy-upstream-rq-timeout-ms`,
	//   :ref:`config_http_filters_router_x-envoy-upstream-rq-per-try-timeout-ms`, and the
	//   :ref:`retry overview <arch_overview_http_routing_retry>`.
	Timeout *time.Duration `protobuf:"bytes,8,opt,name=timeout,proto3,stdduration" json:"timeout,omitempty"`
	// Specifies the idle timeout for the route. If not specified, there is no per-route idle timeout,
	// although the connection manager wide :ref:`stream_idle_timeout
	// <envoy_api_field_config.filter.network.http_connection_manager.v2.HttpConnectionManager.stream_idle_timeout>`
	// will still apply. A value of 0 will completely disable the route's idle timeout, even if a
	// connection manager stream idle timeout is configured.
	//
	// The idle timeout is distinct to :ref:`timeout
	// <envoy_api_field_route.RouteAction.timeout>`, which provides an upper bound
	// on the upstream response time; :ref:`idle_timeout
	// <envoy_api_field_route.RouteAction.idle_timeout>` instead bounds the amount
	// of time the request's stream may be idle.
	//
	// After header decoding, the idle timeout will apply on downstream and
	// upstream request events. Each time an encode/decode event for headers or
	// data is processed for the stream, the timer will be reset. If the timeout
	// fires, the stream is terminated with a 408 Request Timeout error code if no
	// upstream response header has been received, otherwise a stream reset
	// occurs.
	IdleTimeout *time.Duration `protobuf:"bytes,24,opt,name=idle_timeout,json=idleTimeout,proto3,stdduration" json:"idle_timeout,omitempty"`
	// Indicates that the route has a retry policy. Note that if this is set,
	// it'll take precedence over the virtual host level retry policy entirely
	// (e.g.: policies are not merged, most internal one becomes the enforced policy).
	RetryPolicy *RetryPolicy `protobuf:"bytes,9,opt,name=retry_policy,json=retryPolicy,proto3" json:"retry_policy,omitempty"`
	// Indicates that the route has a request mirroring policy.
	RequestMirrorPolicy *RouteAction_RequestMirrorPolicy `protobuf:"bytes,10,opt,name=request_mirror_policy,json=requestMirrorPolicy,proto3" json:"request_mirror_policy,omitempty"`
	// Optionally specifies the :ref:`routing priority <arch_overview_http_routing_priority>`.
	// [#comment:TODO(htuch): add (validate.rules).enum.defined_only = true once
	// https://github.com/lyft/protoc-gen-validate/issues/42 is resolved.]
	Priority core.RoutingPriority `protobuf:"varint,11,opt,name=priority,proto3,enum=envoy.api.v2.core.RoutingPriority" json:"priority,omitempty"`
	// [#not-implemented-hide:]
	RequestHeadersToAdd []*core.HeaderValueOption `protobuf:"bytes,12,rep,name=request_headers_to_add,json=requestHeadersToAdd,proto3" json:"request_headers_to_add,omitempty"` // Deprecated: Do not use.
	// [#not-implemented-hide:]
	ResponseHeadersToAdd []*core.HeaderValueOption `protobuf:"bytes,18,rep,name=response_headers_to_add,json=responseHeadersToAdd,proto3" json:"response_headers_to_add,omitempty"` // Deprecated: Do not use.
	// [#not-implemented-hide:]
	ResponseHeadersToRemove []string `protobuf:"bytes,19,rep,name=response_headers_to_remove,json=responseHeadersToRemove,proto3" json:"response_headers_to_remove,omitempty"` // Deprecated: Do not use.
	// Specifies a set of rate limit configurations that could be applied to the
	// route.
	RateLimits []*RateLimit `protobuf:"bytes,13,rep,name=rate_limits,json=rateLimits,proto3" json:"rate_limits,omitempty"`
	// Specifies if the rate limit filter should include the virtual host rate
	// limits. By default, if the route configured rate limits, the virtual host
	// :ref:`rate_limits <envoy_api_field_route.VirtualHost.rate_limits>` are not applied to the
	// request.
	IncludeVhRateLimits *types.BoolValue `protobuf:"bytes,14,opt,name=include_vh_rate_limits,json=includeVhRateLimits,proto3" json:"include_vh_rate_limits,omitempty"`
	// Specifies a list of hash policies to use for ring hash load balancing. Each
	// hash policy is evaluated individually and the combined result is used to
	// route the request. The method of combination is deterministic such that
	// identical lists of hash policies will produce the same hash. Since a hash
	// policy examines specific parts of a request, it can fail to produce a hash
	// (i.e. if the hashed header is not present). If (and only if) all configured
	// hash policies fail to generate a hash, no hash will be produced for
	// the route. In this case, the behavior is the same as if no hash policies
	// were specified (i.e. the ring hash load balancer will choose a random
	// backend). If a hash policy has the "terminal" attribute set to true, and
	// there is already a hash generated, the hash is returned immediately,
	// ignoring the rest of the hash policy list.
	HashPolicy []*RouteAction_HashPolicy `protobuf:"bytes,15,rep,name=hash_policy,json=hashPolicy,proto3" json:"hash_policy,omitempty"`
	// Indicates that the route has a CORS policy.
	Cors *CorsPolicy `protobuf:"bytes,17,opt,name=cors,proto3" json:"cors,omitempty"`
	// If present, and the request is a gRPC request, use the
	// `grpc-timeout header <https://github.com/grpc/grpc/blob/master/doc/PROTOCOL-HTTP2.md>`_,
	// or its default value (infinity) instead of
	// :ref:`timeout <envoy_api_field_route.RouteAction.timeout>`, but limit the applied timeout
	// to the maximum value specified here. If configured as 0, the maximum allowed timeout for
	// gRPC requests is infinity. If not configured at all, the `grpc-timeout` header is not used
	// and gRPC requests time out like any other requests using
	// :ref:`timeout <envoy_api_field_route.RouteAction.timeout>` or its default.
	// This can be used to prevent unexpected upstream request timeouts due to potentially long
	// time gaps between gRPC request and response in gRPC streaming mode.
	MaxGrpcTimeout         *time.Duration                     `protobuf:"bytes,23,opt,name=max_grpc_timeout,json=maxGrpcTimeout,proto3,stdduration" json:"max_grpc_timeout,omitempty"`
	UpgradeConfigs         []*RouteAction_UpgradeConfig       `protobuf:"bytes,25,rep,name=upgrade_configs,json=upgradeConfigs,proto3" json:"upgrade_configs,omitempty"`
	InternalRedirectAction RouteAction_InternalRedirectAction `protobuf:"varint,26,opt,name=internal_redirect_action,json=internalRedirectAction,proto3,enum=envoy.api.v2.route.RouteAction_InternalRedirectAction" json:"internal_redirect_action,omitempty"`
	// Indicates that the route has a hedge policy. Note that if this is set,
	// it'll take precedence over the virtual host level hedge policy entirely
	// (e.g.: policies are not merged, most internal one becomes the enforced policy).
	// [#not-implemented-hide:]
	HedgePolicy          *HedgePolicy `protobuf:"bytes,27,opt,name=hedge_policy,json=hedgePolicy,proto3" json:"hedge_policy,omitempty"`
	XXX_NoUnkeyedLiteral struct{}     `json:"-"`
	XXX_unrecognized     []byte       `json:"-"`
	XXX_sizecache        int32        `json:"-"`
}
```

## WeightedCluster

### desc

```go
type WeightedCluster struct {
	Clusters             []*WeightedCluster_ClusterWeight `protobuf:"bytes,1,rep,name=clusters,proto3" json:"clusters,omitempty"`
	TotalWeight          *wrappers.UInt32Value            `protobuf:"bytes,3,opt,name=total_weight,json=totalWeight,proto3" json:"total_weight,omitempty"`
	RuntimeKeyPrefix     string                           `protobuf:"bytes,2,opt,name=runtime_key_prefix,json=runtimeKeyPrefix,proto3" json:"runtime_key_prefix,omitempty"`
	XXX_NoUnkeyedLiteral struct{}                         `json:"-"`
	XXX_unrecognized     []byte                           `json:"-"`
	XXX_sizecache        int32                            `json:"-"`
}
```

```pb
// Compared to the :ref:`cluster <envoy_api_field_api.v3alpha.route.RouteAction.cluster>` field that
// specifies a single upstream cluster as the target of a request, the :ref:`weighted_clusters
// <envoy_api_field_api.v3alpha.route.RouteAction.weighted_clusters>` option allows for
// specification of multiple upstream clusters along with weights that indicate the percentage of
// traffic to be forwarded to each cluster. The router selects an upstream cluster based on the
// weights.
message WeightedCluster {
  // [#next-free-field: 11]
  message ClusterWeight {
    reserved 7, 8;

    reserved "per_filter_config";

    // Name of the upstream cluster. The cluster must exist in the
    // :ref:`cluster manager configuration <config_cluster_manager>`.
    string name = 1 [(validate.rules).string = {min_bytes: 1}];

    // An integer between 0 and :ref:`total_weight
    // <envoy_api_field_api.v3alpha.route.WeightedCluster.total_weight>`. When a request matches the
    // route, the choice of an upstream cluster is determined by its weight. The sum of weights
    // across all entries in the clusters array must add up to the total_weight, which defaults to
    // 100.
    google.protobuf.UInt32Value weight = 2;

    // Optional endpoint metadata match criteria used by the subset load balancer. Only endpoints in
    // the upstream cluster with metadata matching what is set in this field will be considered for
    // load balancing. Note that this will be merged with what's provided in :ref:
    // `RouteAction.MetadataMatch <envoy_api_field_api.v3alpha.route.RouteAction.metadata_match>`,
    // with values here taking precedence. The filter name should be specified as *envoy.lb*.
    core.Metadata metadata_match = 3;

    // Specifies a list of headers to be added to requests when this cluster is selected
    // through the enclosing :ref:`envoy_api_msg_route.RouteAction`.
    // Headers specified at this level are applied before headers from the enclosing
    // :ref:`envoy_api_msg_route.Route`, :ref:`envoy_api_msg_route.VirtualHost`, and
    // :ref:`envoy_api_msg_RouteConfiguration`. For more information, including details on
    // header value syntax, see the documentation on :ref:`custom request headers
    // <config_http_conn_man_headers_custom_request_headers>`.
    repeated core.HeaderValueOption request_headers_to_add = 4
        [(validate.rules).repeated = {max_items: 1000}];

    // Specifies a list of HTTP headers that should be removed from each request when
    // this cluster is selected through the enclosing :ref:`envoy_api_msg_route.RouteAction`.
    repeated string request_headers_to_remove = 9;

    // Specifies a list of headers to be added to responses when this cluster is selected
    // through the enclosing :ref:`envoy_api_msg_route.RouteAction`.
    // Headers specified at this level are applied before headers from the enclosing
    // :ref:`envoy_api_msg_route.Route`, :ref:`envoy_api_msg_route.VirtualHost`, and
    // :ref:`envoy_api_msg_RouteConfiguration`. For more information, including details on
    // header value syntax, see the documentation on :ref:`custom request headers
    // <config_http_conn_man_headers_custom_request_headers>`.
    repeated core.HeaderValueOption response_headers_to_add = 5
        [(validate.rules).repeated = {max_items: 1000}];

    // Specifies a list of headers to be removed from responses when this cluster is selected
    // through the enclosing :ref:`envoy_api_msg_route.RouteAction`.
    repeated string response_headers_to_remove = 6;

    // The per_filter_config field can be used to provide weighted cluster-specific
    // configurations for filters. The key should match the filter name, such as
    // *envoy.buffer* for the HTTP buffer filter. Use of this field is filter
    // specific; see the :ref:`HTTP filter documentation <config_http_filters>`
    // for if and how it is utilized.
    map<string, google.protobuf.Any> typed_per_filter_config = 10;
  }

  // Specifies one or more upstream clusters associated with the route.
  repeated ClusterWeight clusters = 1 [(validate.rules).repeated = {min_items: 1}];

  // Specifies the total weight across all clusters. The sum of all cluster weights must equal this
  // value, which must be greater than 0. Defaults to 100.
  google.protobuf.UInt32Value total_weight = 3 [(validate.rules).uint32 = {gte: 1}];

  // Specifies the runtime key prefix that should be used to construct the
  // runtime keys associated with each cluster. When the *runtime_key_prefix* is
  // specified, the router will look for weights associated with each upstream
  // cluster under the key *runtime_key_prefix* + "." + *cluster[i].name* where
  // *cluster[i]* denotes an entry in the clusters array field. If the runtime
  // key for the cluster does not exist, the value specified in the
  // configuration file will be used as the default weight. See the :ref:`runtime documentation
  // <operations_runtime>` for how key names map to the underlying implementation.
  string runtime_key_prefix = 2;
}
```
