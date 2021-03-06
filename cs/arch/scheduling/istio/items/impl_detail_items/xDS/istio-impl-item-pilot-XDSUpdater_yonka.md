
# 概述

* 感觉就是为了在 `ConfigUpdate` 之外给`ServiceEntry`的变更开个洞

* 从注释里来看，社区也预先考虑到了 *endpoints可能规模很大，合适的group是有必要的*

  这里叫shard而不是cluster也许有这层含义，虽然目前实现上这里传入的shard基本是clusterId

* `Workload` 概念

  搜了下，应该是数据面负载的意思，比如 `workload instance` 大约指 sidecar/proxy/envoy

  目前只给k8s registry用了，实际效果是 full-push，感觉很奇怪

* 新引入的 `SvcUpdate` - WIP

  目前也只有k8s registry用了。  为 “服务信息发生变化” 的场景，而且似乎特指 `PostList` 发生变化，这简直。。。

  目前实现的效果是： 只对k8s registry（并且非 multicluster。 这里指除了是 `primary cluster`），更新了 `pc.ServicePort2Name` 


```go
// XDSUpdater is used for direct updates of the xDS model and incremental push.
// Pilot uses multiple registries - for example each K8S cluster is a registry instance,
// as well as consul and future EDS or MCP sources. Each registry is responsible for
// tracking a set of endpoints associated with mesh services, and calling the EDSUpdate
// on changes. A registry may group endpoints for a service in smaller subsets - for
// example by deployment, or to deal with very large number of endpoints for a service.
// We want to avoid passing around large objects - like full list of endpoints for a registry,
// or the full list of endpoints for a service across registries, since it limits scalability.
//
// Future optimizations will include grouping the endpoints by labels, gateway or region to
// reduce the time when subsetting or split-horizon is used. This design assumes pilot
// tracks all endpoints in the mesh and they fit in RAM - so limit is few M endpoints.
// It is possible to split the endpoint tracking in future.
type XDSUpdater interface {

	// EDSUpdate is called when the list of endpoints or labels in a ServiceEntry is
	// changed. For each cluster and hostname, the full list of active endpoints (including empty list)
	// must be sent. The shard name is used as a key - current implementation is using the registry
	// name.
	EDSUpdate(shard, hostname string, entry []*IstioEndpoint) error

	// SvcUpdate is called when a service port mapping definition is updated.
	// This interface is WIP - labels, annotations and other changes to service may be
	// updated to force a EDS and CDS recomputation and incremental push, as it doesn't affect
	// LDS/RDS.
	SvcUpdate(shard, hostname string, ports map[string]uint32, rports map[uint32]string)

	// WorkloadUpdate is called by a registry when the labels or annotations on a workload have changed.
	// The 'id' is the IP address of the pod for k8s if the pod is in the main/default network.
	// In future it will include the 'network id' for pods in a different network, behind a zvpn gate.
	// The IP is used because K8S Endpoints object associated with a Service only include the IP.
	// We use Endpoints to track the membership to a service and readiness.
	WorkloadUpdate(id string, labels map[string]string, annotations map[string]string)

	// ConfigUpdate is called to notify the XDS server of config updates and request a push.
	// The requests may be collapsed and throttled.
	// This replaces the 'cache invalidation' model.
	ConfigUpdate(full bool)
}
```

