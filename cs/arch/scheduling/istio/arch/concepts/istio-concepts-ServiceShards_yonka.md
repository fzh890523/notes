
```go
	// EndpointShards for a service. This is a global (per-server) list, built from
	// incremental updates. This is keyed by service and namespace
    EndpointShardsByService map[string]map[string]*EndpointShards
    // service:name:shareds
    // shards: clusterId:[]*IstioEndpoint
```

* namespace作用： 限制影响（push）的范围

  `EDSUpdate`传入的namespace会一路传下去。
  最后在`ProxyNeedsPush`里会将merge得到的`namespacesUpdated`逐个尝试 `proxy.SidecarScope.DependsOnNamespace(ns)`，实际就是判断了 ns是否在`namespaceDependencies`中

