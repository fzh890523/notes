 

# topics



## 网络、多网络的支持



* 控制面（默认）network的确定

  如下，通过label指定。 istio有启动时sync获取和后面持续watch-更新的机制来保持信息同步。

  在**ns network发生变化**时会 `onNetworkChanged`：

  * `syncPods` 重新获取所有pods，触发`Add`的pod event

    > ？ 意义需要确认一下

  * `syncEndpoints` 类似的... endpoints，触发`Add`的endpoint event

  * `reloadNetworkGateways`  网络都变了，那gw信息自然要更新。 遍历所有服务，更新相关网关信息

    > 不是所有的服务都是gw svc。 极端的情况下，也许一个都无
    >
    > 如果有服务的网关真的更新了（可能是内容发生变化了），会触发full-push

    具体更新逻辑 `extractGatewaysInner`：

    >  数据存在`networkGateways map[host.Name]map[network.ID]gatewaySet`，一级key为服务hostname

    * 取该服务相关的网关信息，有label和network配置两个途径。 没有则跳过

      * label

        gwport来自 `networking.istio.io/gatewayPort` 或者default（目前是15443）

        network信息来自 `topology.istio.io/network`

      * network（从计算的结果缓存`registryServiceNameGateways`里取）

    * 遍历`ClusterExternalAddresses`（不为空才有数据）

    * 对于`ClusterExternalPorts`（nodeport类型），可能要覆盖gwport

    * 构建网关信息加入结果集

      ```go
      			newGateways.add(model.NetworkGateway{
      				Cluster: c.Cluster(),
      				Network: nw,
      				Addr:    ip,
      				Port:    gwPort,
      			}
      ```

    

* network信息获取（本集群、其他集群）： `MeshNetworks` cr

  一样的，istio也会启动时获取+持续watch来保持network信息同步

  `reloadMeshNetworks` （per-controller的，也即per-cluster）

  * 获取当前meshnetworks

  * 遍历处理

    * endpoint处理： cidr提取、加入 `ranger cidranger.Ranger`； 有registry的话，如果和当前cluster相同，则表示当前cluster属于当前network

      > 如果多个network的registry都match当前cluster，则有错

    * gateway处理： 如果是该cluster的network，那么提取registryservicename保存到 `registryServiceNameGateways map[host.Name]uint32` （svcname:gwport）

  之后同样也会`onNetworkChanged`，逻辑如上



* 网关数据的使用： `networkGateways map[host.Name]map[network.ID]gatewaySet`  -> `PushContext.networkMgr *NetworkManager`

  会在 `EndpointBuilder`中用到



###  `MeshNetworks` cr



* 描述L3网络信息

* 描述了 每个network

  * 那些ep属于该network

    * 根据ip段来确定： ip段（cidr）是什么
    * 根据registry（cluster）来确定： registry是什么

  * network该如何（从network外）访问

    * 从（该registry中的）（gw）服务
    * 从某个特定地址

    





> ```go
> // MeshNetworks (config map) provides information about the set of networks
> // inside a mesh and how to route to endpoints in each network. For example
> //
> // MeshNetworks(file/config map):
> //
> // ```yaml
> // networks:
> //   network1:
> //     endpoints:
> //     - fromRegistry: registry1 #must match kubeconfig name in Kubernetes secret
> //     - fromCidr: 192.168.100.0/22 #a VM network for example
> //     gateways:
> //     - registryServiceName: istio-ingressgateway.istio-system.svc.cluster.local
> //       port: 15443
> //       locality: us-east-1a
> //     - address: 192.168.100.1
> //       port: 15443
> //       locality: us-east-1a
> // ```
> //
> type MeshNetworks struct {
> 	// The set of networks inside this mesh. Each network should
> 	// have a unique name and information about how to infer the endpoints in
> 	// the network as well as the gateways associated with the network.
> 	Networks             map[string]*Network `protobuf:"bytes,1,rep,name=networks,proto3" json:"networks,omitempty" protobuf_key:"bytes,1,opt,name=key,proto3" protobuf_val:"bytes,2,opt,name=value,proto3"`
> }
> 
> // Network provides information about the endpoints in a routable L3
> // network. A single routable L3 network can have one or more service
> // registries. Note that the network has no relation to the locality of the
> // endpoint. The endpoint locality will be obtained from the service
> // registry.
> type Network struct {
> 	// The list of endpoints in the network (obtained through the
> 	// constituent service registries or from CIDR ranges). All endpoints in
> 	// the network are directly accessible to one another.
> 	Endpoints []*Network_NetworkEndpoints `protobuf:"bytes,2,rep,name=endpoints,proto3" json:"endpoints,omitempty"`
> 	// Set of gateways associated with the network.
> 	Gateways             []*Network_IstioNetworkGateway `protobuf:"bytes,3,rep,name=gateways,proto3" json:"gateways,omitempty"`
> }
> 
> // NetworkEndpoints describes how the network associated with an endpoint
> // should be inferred. An endpoint will be assigned to a network based on
> // the following rules:
> //
> // 1. Implicitly: If the registry explicitly provides information about
> // the network to which the endpoint belongs to. In some cases, its
> // possible to indicate the network associated with the endpoint by
> // adding the `ISTIO_META_NETWORK` environment variable to the sidecar.
> //
> // 2. Explicitly:
> //
> //    a. By matching the registry name with one of the "fromRegistry"
> //    in the mesh config. A "from_registry" can only be assigned to a
> //    single network.
> //
> //    b. By matching the IP against one of the CIDR ranges in a mesh
> //    config network. The CIDR ranges must not overlap and be assigned to
> //    a single network.
> //
> // (2) will override (1) if both are present.
> type Network_NetworkEndpoints struct {
> 	// Types that are valid to be assigned to Ne:
> 	//	*Network_NetworkEndpoints_FromCidr
> 	//	*Network_NetworkEndpoints_FromRegistry
> 	Ne                   isNetwork_NetworkEndpoints_Ne `protobuf_oneof:"ne"`
> }
> 
> // The gateway associated with this network. Traffic from remote networks
> // will arrive at the specified gateway:port. All incoming traffic must
> // use mTLS.
> type Network_IstioNetworkGateway struct {
> 	// Types that are valid to be assigned to Gw:
> 	//	*Network_IstioNetworkGateway_RegistryServiceName
> 	//	*Network_IstioNetworkGateway_Address
> 	Gw isNetwork_IstioNetworkGateway_Gw `protobuf_oneof:"gw"`
> 	// The port associated with the gateway.
> 	Port uint32 `protobuf:"varint,3,opt,name=port,proto3" json:"port,omitempty"`
> 	// The locality associated with an explicitly specified gateway (i.e. ip)
> 	Locality             string   `protobuf:"bytes,4,opt,name=locality,proto3" json:"locality,omitempty"`
> }
> 
> type Network_IstioNetworkGateway_RegistryServiceName struct {
> 	RegistryServiceName string `protobuf:"bytes,1,opt,name=registry_service_name,json=registryServiceName,proto3,oneof" json:"registryServiceName,omitempty"`
> }
> type Network_IstioNetworkGateway_Address struct {
> 	Address string `protobuf:"bytes,2,opt,name=address,proto3,oneof" json:"address,omitempty"`
> }
> 
> type Network_NetworkEndpoints_FromCidr struct {
> 	FromCidr string `protobuf:"bytes,1,opt,name=from_cidr,json=fromCidr,proto3,oneof" json:"fromCidr,omitempty"`
> }
> type Network_NetworkEndpoints_FromRegistry struct {
> 	FromRegistry string `protobuf:"bytes,2,opt,name=from_registry,json=fromRegistry,proto3,oneof" json:"fromRegistry,omitempty"`
> }
> ```
>
> 





### 标签： topology.istio.io/network

* ns上： 设置该ns下的istio控制面的默认network（会影响该控制面管理的所有pods（的默认network，如下，可以被覆盖））

* （gw）svc上： 指定该svc作为（value指定的）该network的gw svc

  那么，其他network的pod要访问该svc时会使用它的外部地址，一般可能来自：

  > 那么，为什么要访问该svc呢？ 
  >
  > * 显式访问
  > * （其他network的pods）做跨network服务访问时”借用/经由“gw svc

  * svc的`spec.externalIPs`
  * lb类型的话，`status.loadBalancer.ingress[].ip`
  * nodeport类型的话，会根据nodeselector得到node ips

  该network信息，需要和所在（被纳管到的）控制面的network（通过控制面所在ns的label来确定）一致； 或者 `the network of the targeted pods.`

  > ？ 这个没懂

* pod上： 覆盖它的默认network信息，默认信息一般来自于注入（时对pod的修改）









## 多集群的支持



### （控制面）如何访问多集群

* `serviceregistry/kube/controller/Multicluster.InitSecretController` 会`StartSecretController`，注册addCluster、updateCluster的回调

  会创建对应的kuberegistry，来读取对应cluster的服务、端点信息

  > 似乎只有`WorkloadEntryCrossCluster`时才读取该cluster的`IstioNetworkingV1Alpha3Workloadentries`配置（也即 **一般的配置不读取**）。 
  >
  > 此时，-> (workload) configStore再用来`NewServiceDiscovery`再加到registries中

* `pkg/kube/secretcontroller.Controller` 会watch `istio/multiCluster=true`的secret，对应的创建/更新member cluster，调对应的回调



### 如何把cluster、registry、network厘清、关联







## istio nodeport类型gateway svc的支持

* lb类型： 直接取addr，存在 `istioService.Attributes.ClusterExternalAddresses`

* nodeport类型： 取port+nodeselector，port存在 `istioService.Attributes.ClusterExternalPorts`，后者可以用来selector addr然后更新addr，存在 `ClusterExternalAddresses``

  > 所以node变化可能触发...变化

最后会综合存储到： `networkGateways map[host.Name]map[network.ID]gatewaySet`，`PushContext`的`networkMgr *NetworkManager`会用这个数据来计算得到。 而这个mgr会 endpointbuilder的逻辑里用来...







# 操作流程






## 环境、方案、模型确认



参照 https://istio.io/latest/docs/ops/deployment/deployment-models/ 可以知道，home环境的情况是

* 多（k8s）集群（单控制面）

  ![img](https://istio.io/latest/docs/ops/deployment/deployment-models/multi-cluster.svg)

* 多网络

  ![A service mesh with multiple networks](https://istio.io/latest/docs/ops/deployment/deployment-models/multi-net.svg)

  > 此图只表意多网络。 实际控制面可以 共享

* primary+remote（控制面）模型

  ![A service mesh with a primary and a remote cluster](https://istio.io/latest/docs/ops/deployment/deployment-models/shared-control.svg)



* mesh内信任模型
* 单mesh模型（区别于多mesh/mesh联邦）



### 模型比较

基本上

* primary-remote

  > 目前方案，还是会在remote里部署一个控制面，用于sidecar注入和ca，后面会去掉

  > remote sidecar -xds-> local ingress -> local istiod。 所以这个场景要先部署东西向gw/ingress
  >
  > 等于**这个ingress同时走了 xds 和 payload**
  >
  > > 获取ingress地址：
  > >
  > > ```sh
  > > export DISCOVERY_ADDRESS=$(kubectl \
  > >  --context="${CTX_CLUSTER1}" \
  > >  -n istio-system get svc istio-eastwestgateway \
  > >  -o jsonpath='{.status.loadBalancer.ingress[0].ip}')
  > > ```
  > >
  > > 文中同网络也这么配了，<del>实则不必吧，本来可通。</del>
  > >
  > > 噢，**还是有必要**。 因为local cluster svc clusterIp 对于remote不可用，而pod ip又一直变，所以都不行。 依赖ingress的不变地址（比如lb类型等）

  * [same network](https://istio.io/latest/docs/setup/install/multicluster/primary-remote/)

    remote主要体现在 `.values.global.remotePilotAddress` 不为空（比如这里测试设置为`192.168.1.1:15010`（实际好像不要port））

    以下是primary模式和remote模式生成配置的差别，可以看到，实际是用的discoveryAddress的服务名不同，然后用一个**静态`Service` + `Endpoints` 来指向了外部**... （-> local ingress）

    ```diff
    7412c7412
    <       discoveryAddress: istiod.istio-system.svc:15012
    ---
    >       discoveryAddress: istiod-remote.istio-system.svc:15012
    7519c7519
    <         "remotePilotAddress": "",
    ---
    >         "remotePilotAddress": "192.168.1.1:15010",
    8551a8552,8564
    > apiVersion: v1
    > kind: Endpoints
    > metadata:
    >   name: istiod-remote
    >   namespace: istio-system
    > subsets:
    > - addresses:
    >   - ip: 192.168.1.1:15010
    >   ports:
    >   - port: 15012
    >     name: tcp-istiod
    >     protocol: TCP
    > ---
    9318a9332,9343
    > ---
    > apiVersion: v1
    > kind: Service
    > metadata:
    >   name: istiod-remote
    >   namespace: istio-system
    > spec:
    >   ports:
    >   - port: 15012
    >     name: tcp-istiod
    >     protocol: TCP
    >   clusterIP: None
    ```

    然后还是加上两边的ingress来互通流量了

  * [multi networks](https://istio.io/latest/docs/setup/install/multicluster/primary-remote_multi-network/)

* multi-primary

  * [same network](https://istio.io/latest/docs/setup/install/multicluster/multi-primary/)

    * 两个集群meshID和network值都一样

    * pod能互访，只需要能访问其他集群的api server拿到svc/eps等

    * istio多集群的支持是通过 watch `istio/multiCluster=true`的secrets来”加载配置“，也即有secret就够了

      ```sh
      istioctl x create-remote-secret \
        --context="${CTX_CLUSTER1}" \
        --name=cluster1 | \
        kubectl apply -f - --context="${CTX_CLUSTER2}"
      # 这个叫 Enable Endpoint Discovery 操作 😂 cluster2 -访问-> cluster1
      # 相应的也有
      istioctl x create-remote-secret \
        --context="${CTX_CLUSTER2}" \
        --name=cluster2 | \
        kubectl apply -f - --context="${CTX_CLUSTER1}"
      ```

      > secret的格式：
      >
      > ```yaml
      > data/stringData:
      > cluster1: ...  # string or []byte
      > # ...
      > ```

  * [multi networks](https://istio.io/latest/docs/setup/install/multicluster/multi-primary_multi-network/)

    基于same network的步骤，为了能互通，需要额外的（东西向）gateway，istio做了对应的（脚本便利）支持

    此外，此时两个集群的控制面的network值就不同了。 如果ns已存在的话，需要单独label，如： `label namespace istio-system topology.istio.io/network=network2`

    





## 安装多集群

https://istio.io/latest/docs/setup/install/multicluster/



### 证书准备

考虑到后面可能多mesh互访，文档里建议是使用中间证书作为istio根证书，这样来自同一个根证书的istio之间互访可以...

https://istio.io/latest/docs/setup/install/multicluster/before-you-begin/

https://istio.io/latest/docs/tasks/security/cert-management/plugin-ca-cert/

> 文里说要re-install istio，应该也不是。 按照istio流程，应该停掉istio然后删掉已有证书，加入证书，再启动istio即可。 不过workloads都需要重启。 生产环境是不能这么简单。

```sh
# cd to istio release dir
mkdir certs
pushd certs

# target名字有意义
make -f ../tools/certs/Makefile.selfsigned.mk root-ca
make -f ../tools/certs/Makefile.selfsigned.mk cluster1-cacerts  #  <clusterName>-cacerts。 这个实际如果root-ca不存在时会创建，也即等于 { if not exist(root-ca) then make root-ca; }; make cluster1-cacerts

tree
.
├── cluster1
│   ├── ca-cert.pem
│   ├── ca-key.pem
│   ├── cert-chain.pem
│   └── root-cert.pem
├── root-ca.conf  # root这些要保存好，可以放到k8s里单独secret
├── root-cert.csr
├── root-cert.pem
├── root-cert.srl
└── root-key.pem

1 directory, 9 files

k scale deployment istiod-1-11-4 --replicas 0
k scale deployment istiod-1-9-9 --replicas 0

k delete cm istio-ca-root-cert
k delete secrets istio-ca-secret

# 这个会被istio使用
kubectl create secret generic cacerts -n istio-system \
      --from-file=cluster1/ca-cert.pem \
      --from-file=cluster1/ca-key.pem \
      --from-file=cluster1/root-cert.pem \
      --from-file=cluster1/cert-chain.pem
      
# 这个只做保存      
kubectl create secret generic istio-root-cacerts -n istio-system \
      --from-file=root-ca.conf \
      --from-file=root-key.pem \
      --from-file=root-cert.pem \
      --from-file=root-cert.csr \
      --from-file=root-cert.srl     
      
      
      
k scale deployment istiod-1-11-4 --replicas 1

021-11-07T02:44:31.424251Z     info    creating CA and initializing public key
2021-11-07T02:44:31.424289Z     info    Use local CA certificate
2021-11-07T02:44:31.425600Z     info    initializing controllers
2021-11-07T02:44:31.425615Z     info    No certificates specified, skipping K8S DNS certificate controller
2021-11-07T02:44:31.505103Z     warn    kube    Skipping CRD networking.x-k8s.io/v1alpha1/BackendPolicy as it is not present
2021-11-07T02:44:31.505319Z     warn    kube    Skipping CRD networking.x-k8s.io/v1alpha1/GatewayClass as it is not present
2021-11-07T02:44:31.505410Z     warn    kube    Skipping CRD networking.x-k8s.io/v1alpha1/Gateway as it is not present
2021-11-07T02:44:31.505498Z     warn    kube    Skipping CRD networking.x-k8s.io/v1alpha1/HTTPRoute as it is not present
2021-11-07T02:44:31.505566Z     warn    kube    Skipping CRD networking.x-k8s.io/v1alpha1/TCPRoute as it is not present
2021-11-07T02:44:31.505634Z     warn    kube    Skipping CRD networking.x-k8s.io/v1alpha1/TLSRoute as it is not present
2021-11-07T02:44:31.507414Z     info    Adding Kubernetes registry adapter
2021-11-07T02:44:31.507460Z     info    initializing Istiod DNS certificates host: istiod-1-11-4.istio-system.svc, custom host:
2021-11-07T02:44:31.706930Z     info    Generating istiod-signed cert for [istiod-1-11-4.istio-system.svc istiod.istio-system.svc istiod-remote.istio-system.svc istio-pilot.istio-system.svc]:
# 这个是生成 dns cert。 这个证书用于istio自己的server，如grpc和webhook。
# 打印了证书链，分别是
# Subject: （空，Issuer: O=Istio, CN=Intermediate CA, L=cluster1。 就是这个cert） 
# Subject: O=Istio, CN=Intermediate CA, L=cluster1
# Subject: O=Istio, CN=Root CA
-----BEGIN CERTIFICATE-----
...
-----END CERTIFICATE-----
-----BEGIN CERTIFICATE-----
...
-----END CERTIFICATE-----
-----BEGIN CERTIFICATE-----
...
-----END CERTIFICATE-----

2021-11-07T02:44:31.707433Z     info    Use plugged-in cert at etc/cacerts/ca-key.pem
2021-11-07T02:44:31.707735Z     info    x509 cert - Issuer: "CN=Intermediate CA,O=Istio,L=cluster1", Subject: "", SN: 3ea79cbadc573426df9509f6ecf0589e, NotBefore: "2021-11-07T02:42:31Z", NotAfter: "2031-11-05T02:44:31Z"
2021-11-07T02:44:31.707891Z     info    x509 cert - Issuer: "CN=Root CA,O=Istio", Subject: "CN=Intermediate CA,O=Istio,L=cluster1", SN: 34347481a9e1ddbcaf47f2b543f747a7cc26693e, NotBefore: "2021-11-07T02:30:05Z", NotAfter: "2023-11-07T02:30:05Z"
2021-11-07T02:44:31.708031Z     info    x509 cert - Issuer: "CN=Root CA,O=Istio", Subject: "CN=Root CA,O=Istio", SN: 3bc301fbe639a69f165e640ebc6e3d3001b66dc7, NotBefore: "2021-11-07T02:16:16Z", NotAfter: "2031-11-05T02:16:16Z"
2021-11-07T02:44:31.708103Z     info    Istiod certificates are reloaded
2021-11-07T02:44:31.708251Z     info    spiffe  Added 1 certs to trust domain cluster.local in peer cert verifier

# ingress果然出问题了
# https://istio.io/latest/docs/setup/additional-setup/gateway/

# 覆盖安装（应该只会影响gateway的secret吧，只有这个有差异）
./bin/istioctl install --set revision=1-11-4 --set components.pilot.k8s.resources.requests.cpu=50m --set components.pilot.k8s.resources.requests.memory=300Mi


# 查看cm和secret，发现
k get secrets cacerts -o json | jq -r '.data["cert-chain.pem"]' | base64 -d | openssl x509 -noout -text
# Subject: O = Istio, CN = Intermediate CA, L = cluster1

k get cm istio-ca-root-cert -o json | jq -r '.data["root-cert.pem"]' | openssl x509 -noout -text
# Subject: O = Istio, CN = Root CA
# Issuer: O = Istio, CN = Root CA
# 好像还是生成了一份istio ca，但是没使用？ 被cacerts优先了？

k get secrets istio-ingressgateway-ca-certs  -o json | jq -r '.data["example.com.crt"]' | base64 -d | openssl x509 -noout -text
# Issuer: O = example Inc., CN = example.com
# Subject: O = example Inc., CN = example.com
# 好像是独立的
    - mountPath: /var/run/secrets/istio
      name: istiod-ca-cert
  - configMap:
      defaultMode: 420
      name: istio-ca-root-cert
    name: istiod-ca-cert      
# 看了下网关挂载的也是这个    

# istiod自己倒是
  - name: cacerts
    secret:
      defaultMode: 420
      optional: true
      secretName: cacerts
      
    - mountPath: /etc/cacerts
      name: cacerts      
      
# 后来发现，是openssl那个命令默认只输出第一个证书，也即intermediate那个
k get secrets cacerts -o json | jq -r '.data["cert-chain.pem"]' | base64 -d > /tmp/1 && openssl crl2pkcs7 -nocrl -certfile /tmp/1  | openssl pkcs7 -print_certs -text -noout  # 可以看到全部了，这个的真root就是cm里那个，也即挂载给网关、sidecar那个
# 那么问题来了，只给一个root cert而不给istio ca也即immediate那个，能验证istio server cert吗？ 我猜可能是tls协商时server端负责把chain上的都下发？ 这样的话client用root cert就可以...了
```



### 开始

```sh
k label namespace istio-system topology.istio.io/network=network1
kc c2
k label namespace istio-system topology.istio.io/network=network2
kc c1
```

```yaml
cat <<EOF > op_cluster1.yaml
apiVersion: install.istio.io/v1alpha1
kind: IstioOperator
spec:
  revision: 1-11-4
  components:
    pilot:
      k8s:
        resources:
          requests:
            cpu: 50m
            memory: 300Mi
  values:
    global:
      meshID: mesh1
      multiCluster:
        clusterName: cluster1
      network: network1
EOF
```

```sh
./bin/istioctl manifest generate --context="${CTX_CLUSTER1}" --revision=1-11-4 -f op_cluster1.yaml > cluster1.yaml
# check
kubectl --context="${CTX_CLUSTER1}" apply -f cluster1.yaml

samples/multicluster/gen-eastwest-gateway.sh \
    --mesh mesh1 --cluster cluster1 --network network1 --revision 1-11-4 | \
    ./bin/istioctl --context="${CTX_CLUSTER1}" install -y -f -
# 前面不知道怎么，生成的op里的profile=empty丢失了，导致覆盖安装了错误的... 后来又好了，晕。 确实有必要check再apply
# ./bin/istioctl --context="${CTX_CLUSTER1}" manifest generate  -f - | 
```

> 有个坑。 原来的istio-system的ns有个 `istio-injection: disabled`
>
> 但gw支持注入后这个就有问题了，于是... gw的pod就不被inject，auto镜像无法换为正确的... 就一直拉不到镜像

```sh
cat samples/multicluster/expose-services.yaml | yq -y '.spec.selector["istio.io/rev"]="1-11-4"' | yq -y '.metadata.labels["istio.io/rev"]="1-11-4"' | kubectl apply --context="${CTX_CLUSTER1}" -n istio-system -f-
gateway.networking.istio.io/cross-network-gateway created

cp samples/multicluster/expose-istiod.yaml /tmp
# 这个麻烦点，手动改算了。 加上rev
kubectl apply --context="${CTX_CLUSTER1}" -n istio-system -f /tmp/expose-istiod.yaml
gateway.networking.istio.io/istiod-1-11-4-gateway created
virtualservice.networking.istio.io/istiod-1-11-4-vs created
```



```sh
istioctl x create-remote-secret \
    --context="${CTX_CLUSTER2}" \
    --name=cluster2 | \
    kubectl apply -f - --context="${CTX_CLUSTER1}"
    
export DISCOVERY_ADDRESS=$(kubectl \
    --context="${CTX_CLUSTER1}" \
    -n istio-system get svc istio-eastwestgateway \
    -o jsonpath='{.status.loadBalancer.ingress[0].ip}')
    
echo $DISCOVERY_ADDRESS    
```



```yaml
cat <<EOF > op_cluster2.yaml
apiVersion: install.istio.io/v1alpha1
kind: IstioOperator
spec:
  revision: 1-11-4
  components:
    pilot:
      k8s:
        resources:
          requests:
            cpu: 50m
            memory: 300Mi
  values:
    global:
      meshID: mesh1
      multiCluster:
        clusterName: cluster2
      network: network2
      remotePilotAddress: ${DISCOVERY_ADDRESS}
EOF
```

```sh
./bin/istioctl manifest generate --context="${CTX_CLUSTER2}" --revision=1-11-4 -f op_cluster2.yaml > cluster2.yaml
# check
kubectl --context="${CTX_CLUSTER2}" apply -f cluster2.yaml
```

```
# 问题
2021-11-07T19:11:56.299720Z     info    CA Endpoint istiod-1-11-4.istio-system.svc:15012, provider Citadel
2021-11-07T19:11:56.299873Z     info    Using CA istiod-1-11-4.istio-system.svc:15012 cert with certs: var/run/secrets/istio/root-cert.pem
2021-11-07T19:11:56.300052Z     info    citadelclient   Citadel client using custom root cert: istiod-1-11-4.istio-system.svc:15012

2021-11-07T19:11:58.356231Z     warn    sds     failed to warm certificate: failed to generate workload certificate: create certificate: rpc error: code = Unavailble desc = connection error: desc = "transport: authentication handshake failed: x509: certificate is valid for istiod-remote.istio-system.svc, istiod.istio-system.svc, istio-pilot.istio-system.svc, not istiod-1-11-4.istio-system.svc"

2021-11-07T19:15:08.215512Z     warning envoy config    StreamSecrets gRPC config stream closed: 2, failed to generate secret for ROOTCA: failed to generate workload certificate: create certificate: rpc error: code = Unavailable desc = connection error: desc = "transport: authentication handshake failed: x509: certificate is valid for istiod-remote.istio-system.svc, istiod.istio-system.svc, istio-pilot.istio-system.svc, not istiod-1-11-4.istio-system.svc"

> found a ENV of istiod named ISTIOD_CUSTOM_HOST can solve the problem
https://github.com/istio/istio/issues/32702


2021-11-07T17:26:25.598694Z     info    initializing Istiod DNS certificates host: istiod-1-11-4.istio-system.svc, custom host:
2021-11-07T17:26:25.708285Z     info    Generating istiod-signed cert for [istiod-1-11-4.istio-system.svc istiod.istio-system.svc istiod-remote.istio-system.svc istio-pilot.istio-system.svc]:

后来看了下，xds是正常的，有问题的是sds。
因为xds是连到remote control-plane，而sds是要连local control-plane。 xds下发的sds配置里，sds-grpc指向的还是 ./etc/istio/proxy/SDS 然后pilot-agent sds连到的是（local的）istiod-1-11-4。
而这个istiod又为了注入等原因把自己的 `discoveryAddress: istiod-remote.istio-system.svc:15012` （configmap `data.mesh.defaultConfig` ...）

加上env就好了
        env:
        - name: ISTIOD_CUSTOM_HOST
          value: istiod-1-11-4.istio-system.svc

2021-11-08T03:47:54.136470Z     info    Adding Kubernetes registry adapter
2021-11-08T03:47:54.136612Z     info    initializing Istiod DNS certificates host: istiod-remote.istio-system.svc, custom host: istiod-1-11-4.istio-system.svc
2021-11-08T03:47:54.136636Z     info    Adding custom hostname istiod-1-11-4.istio-system.svc
2021-11-08T03:47:54.441015Z     info    Generating istiod-signed cert for [istiod-remote.istio-system.svc istiod-1-11-4.istio-system.svc istiod.istio-system.svc istio-pilot.istio-system.svc]:

但改动到chars却没生效，晕
https://github.com/istio/istio/issues/35939

```

> 期间还遇到deployment replica无法修改的问题，竟然有个`horizontalpodautoscaler/istiod-1-11-4`

```sh
samples/multicluster/gen-eastwest-gateway.sh \
    --mesh mesh1 --cluster cluster2 --network network2 --revision 1-11-4 | \
    ./bin/istioctl --context="${CTX_CLUSTER2}" install -y -f -
kubectl --context="${CTX_CLUSTER2}" get svc istio-eastwestgateway -n istio-system
# 不过这个集群没有metallb，所以。。。 把svc istio-eastwestgateway改为nodeport试试
```





> 比较一下配置差异
>
> ```sh
> ./bin/istioctl manifest generate --set profile=minimal --revision=1-11-4 --set revision=1-11-4 --set components.pilot.k8s.resources.requests.cpu=50m --set components.pilot.k8s.resources.requests.memory=300Mi > /tmp/base.yaml
> 
> ./bin/istioctl manifest generate --set profile=minimal --revision=1-11-4 --set revision=1-11-4 --set components.pilot.k8s.resources.requests.cpu=50m --set components.pilot.k8s.resources.requests.memory=300Mi --set values.global.meshID=mesh1 --set values.global.multiCluster.clusterName=cluster1 --set values.global.network=networks > /tmp/base_mc1.yaml
> 
> ./bin/istioctl manifest diff /tmp/base.yaml /tmp/base_mc1.yaml
> ```
>
> ```yaml
> Differences in manifests are:
> 
> 
> Object ConfigMap:istio-system:istio-1-11-4 has diffs:
> 
> data:
>   mesh:
>     defaultConfig:
>       meshId: <empty> -> mesh1 (ADDED)
> 
> 
> Object ConfigMap:istio-system:istio-sidecar-injector-1-11-4 has diffs:
> 
> data:
>   values:
>     global:
>       meshID: <empty> -> mesh1 (ADDED)
>       multiCluster:
>         clusterName: <empty> -> cluster1 (ADDED)
>       network: <empty> -> networks (ADDED)
> 
> 
> Object Deployment:istio-system:istiod-1-11-4 has diffs:
> 
> spec:
>   template:
>     spec:
>       containers:
>         '[#0]':
>           env:
>             '[#12]':
>               value: Kubernetes -> cluster1
> ```





### 网关如何通讯/验证

https://istio.io/latest/docs/setup/install/multicluster/verify/

基本上就按着链接的方式验证。 不过，本次操作里：

```sh
ctx=$CTX_CLUSTER1
kubectl exec --context="${ctx}" -n sample -c sleep \
    "$(kubectl get pod --context="${ctx}" -n sample -l \
    app=sleep -o jsonpath='{.items[0].metadata.name}')" \
    -- curl -sS helloworld.sample:5000/hello
# 这个只能访问到v1，也即local/cluster1的那个    
# 估计还是因为cluster2的东西gw没有external ip，虽然按道理node port也是可以的...    
kubectl -n istio-system get service istio-eastwestgateway -o jsonpath='{.spec.ports[?(@.name=="tls")].nodePort}'    
    
ctx=$CTX_CLUSTER2
kubectl exec --context="${ctx}" -n sample -c sleep \
    "$(kubectl get pod --context="${ctx}" -n sample -l \
    app=sleep -o jsonpath='{.items[0].metadata.name}')" \
    -- curl -sS helloworld.sample:5000/hello    
# 这个v1和v2都可以出现，也即相对它的local/cluster2可以访问，同时通过 cluster1的东西向网关能访问到cluster1的    
```

> Anyone help explain how the nodeport type gateway works?
> I see that the code requires the "[traffic.istio.io/nodeSelector](http://traffic.istio.io/nodeSelector)" annotation to filter and determine the address of the gateway, but I think the number of pod copies of a normal nodetype type service does not necessarily cover this filtering result. The former would be a superset of the latter, which means that not all the addresses we end up with are available.
> I think the correct approach should be to get the corresponding node set from service endpoints and then generate the final endpoints with the corresponding node port.
>
> 在slack上讨论



```json
   "dynamic_listeners": [
    {
     "name": "0.0.0.0_15443",
     "active_state": {
      "version_info": "2021-11-07T18:38:47Z/15",
      "listener": {
       "@type": "type.googleapis.com/envoy.config.listener.v3.Listener",
       "name": "0.0.0.0_15443",
       "address": {
        "socket_address": {
         "address": "0.0.0.0",
         "port_value": 15443
        }
       },
       "filter_chains": [
         ...

        {
         "filter_chain_match": {
          "application_protocols": [
           "istio",
           "istio-peer-exchange",
           "istio-http/1.0",
           "istio-http/1.1",
           "istio-h2"
          ],
          "server_names": [
           "outbound_.5000_._.helloworld.sample.svc.cluster.local"
          ]
         },
          ...
          },
          {
           "name": "envoy.filters.network.tcp_proxy",
           "typed_config": {
            "@type": "type.googleapis.com/envoy.extensions.filters.network.tcp_proxy.v3.TcpProxy",
            "stat_prefix": "outbound_.5000_._.helloworld.sample.svc.cluster.local",
            "cluster": "outbound_.5000_._.helloworld.sample.svc.cluster.local"
           }
          }
         ]
        },      
       ...
       ],
       "listener_filters": [
        {
         "name": "envoy.filters.listener.tls_inspector",
         "typed_config": {
          "@type": "type.googleapis.com/envoy.extensions.filters.listener.tls_inspector.v3.TlsInspector"
         }
        }
       ],
       "traffic_direction": "OUTBOUND"
      },
      "last_updated": "2021-11-08T05:29:04.226Z"
     }
    },       
```



**结论： 把服务信息放在了tls里，所以才依赖...**

但其实不必啊。。。



```json
       {
        "name": "helloworld.sample.svc.cluster.local:5000",
        "domains": [
         "helloworld.sample.svc.cluster.local",
         "helloworld.sample.svc.cluster.local:5000",
         "helloworld",
         "helloworld:5000",
         "helloworld.sample.svc",
         "helloworld.sample.svc:5000",
         "helloworld.sample",
         "helloworld.sample:5000",
         "10.96.81.179",
         "10.96.81.179:5000"
        ],
        "routes": [
         {
          "match": {
           "prefix": "/"
          },
          "route": {
           "cluster": "outbound|5000||helloworld.sample.svc.cluster.local",
           "timeout": "0s",
           "retry_policy": {
            "retry_on": "connect-failure,refused-stream,unavailable,cancelled,retriable-status-codes",
            "num_retries": 2,
            "retry_host_predicate": [
             {
              "name": "envoy.retry_host_predicates.previous_hosts"
             }
            ],
            "host_selection_retry_max_attempts": "5",
            "retriable_status_codes": [
             503
            ]
           },
           "max_stream_duration": {
            "max_stream_duration": "0s",
            "grpc_timeout_header_max": "0s"
           }
          },
          "decorator": {
           "operation": "helloworld.sample.svc.cluster.local:5000/*"
          },
          "name": "default"
         }
        ],
        "include_request_attempt_count": true
         

         
    {
     "version_info": "2021-11-08T05:29:41Z/43",
     "cluster": {
      "@type": "type.googleapis.com/envoy.config.cluster.v3.Cluster",
      "name": "outbound|5000||helloworld.sample.svc.cluster.local",
      "type": "EDS",
      "eds_cluster_config": {
       "eds_config": {
        "ads": {},
        "initial_fetch_timeout": "0s",
        "resource_api_version": "V3"
       },         
       "service_name": "outbound|5000||helloworld.sample.svc.cluster.local"
      },
      "connect_timeout": "10s",
      "circuit_breakers": {
       "thresholds": [
        {
         "max_connections": 4294967295,
         "max_pending_requests": 4294967295,
         "max_requests": 4294967295,
         "max_retries": 4294967295,
         "track_remaining": true
        }
       ]
      },
      "metadata": {
       "filter_metadata": {
        "istio": {
         "default_original_port": 5000,
         "services": [
          {
           "host": "helloworld.sample.svc.cluster.local",
           "name": "helloworld",
           "namespace": "sample"
          }
         ]
        }
       }
      },
      "filters": [
       {         
        "typed_config": {
         "@type": "type.googleapis.com/udpa.type.v1.TypedStruct",
         "type_url": "type.googleapis.com/envoy.tcp.metadataexchange.config.MetadataExchange",
         "value": {
          "protocol": "istio-peer-exchange"
         }
        }
       }
      ],
      "transport_socket_matches": [
       {
        "name": "tlsMode-istio",
        "match": {
         "tlsMode": "istio"
        },
        "transport_socket": {
         "name": "envoy.transport_sockets.tls",
         "typed_config": {
          "@type": "type.googleapis.com/envoy.extensions.transport_sockets.tls.v3.UpstreamTlsContext",
          "common_tls_context": {
           "alpn_protocols": [
            "istio-peer-exchange",
            "istio"
           ],
           "tls_certificate_sds_secret_configs": [
            {
             "name": "default",
             "sds_config": {
              "api_config_source": {
               "api_type": "GRPC",
               "grpc_services": [
                {
                 "envoy_grpc": {
                  "cluster_name": "sds-grpc"
                 }
                }
               ],
               "set_node_on_first_message_only": true,
               "transport_api_version": "V3"
              },
              "initial_fetch_timeout": "0s",
              "resource_api_version": "V3"
             }
            }
           ],
           "combined_validation_context": {
            "default_validation_context": {
             "match_subject_alt_names": [
              {
               "exact": "spiffe://cluster.local/ns/sample/sa/default"
              }
             ]
            },
            "validation_context_sds_secret_config": {
             "name": "ROOTCA",
             "sds_config": {
              "api_config_source": {
               "api_type": "GRPC",
               "grpc_services": [
                {
                 "envoy_grpc": {         
                  "cluster_name": "sds-grpc"
                 }
                }
               ],
               "set_node_on_first_message_only": true,
               "transport_api_version": "V3"
              },
              "initial_fetch_timeout": "0s",
              "resource_api_version": "V3"
             }
            }
           }
          },
          "sni": "outbound_.5000_._.helloworld.sample.svc.cluster.local"
         }
        }
       },
       {
        "name": "tlsMode-disabled",
        "match": {},
        "transport_socket": {
         "name": "envoy.transport_sockets.raw_buffer"
        }
       }
      ]
     },
     "last_updated": "2021-11-08T05:29:44.499Z"
    },         
```




























