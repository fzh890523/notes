# 来源



```go
	// MeshExternal (if true) indicates that the service is external to the mesh.
	// These services are defined using Istio's ServiceEntry spec.
	MeshExternal bool
```





## k8s svc



`ExternalName`类型的服务（且`ExternalName != ""`，不被认为是internal，resolution也会被设为`DNSLB`



## serviceentry



```go
// Location specifies whether the service is part of Istio mesh or
// outside the mesh.  Location determines the behavior of several
// features, such as service-to-service mTLS authentication, policy
// enforcement, etc. When communicating with services outside the mesh,
// Istio's mTLS authentication is disabled, and policy enforcement is
// performed on the client-side as opposed to server-side.
type ServiceEntry_Location int32

const (
	// Signifies that the service is external to the mesh. Typically used
	// to indicate external services consumed through APIs.
	ServiceEntry_MESH_EXTERNAL ServiceEntry_Location = 0
	// Signifies that the service is part of the mesh. Typically used to
	// indicate services added explicitly as part of expanding the service
	// mesh to include unmanaged infrastructure (e.g., VMs added to a
	// Kubernetes based service mesh).
	ServiceEntry_MESH_INTERNAL ServiceEntry_Location = 1
)
```





# 影响



## mtls mode决策



```go
	if service.MeshExternal {
		// Only need the authentication mTLS mode when service is not external.
		return MTLSUnknown
	}
```

> 然后对于mesh internal services，才有后续的一些决策流程，如 resolution、authpolicies 等



## SniDnatCluster生成（auto_passthrough gw）

只对internal services才生成...

