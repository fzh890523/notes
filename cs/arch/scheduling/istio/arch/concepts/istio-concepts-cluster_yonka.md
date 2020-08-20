

感觉主要是应对：

* 多registry协同

  可能提供同一个服务。 这是clusterId为registry名

* k8s的multicluster支持

  这是为k8s的clusterId


注意：

registry提供 不同namespace的服务 跟 不同cluster的服务 是不一样的，可见性不同。
前者只有对应namespace的才可见； 后者只是不同的组织/部署形式，可见性一样，会做合并。

具体可以参见 《istio_concepts_namespace_yonka.md》 和 《istio_concepts_ServiceShards_yonka.md》

