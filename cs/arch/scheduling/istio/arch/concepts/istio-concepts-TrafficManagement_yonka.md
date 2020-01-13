

Ref: 



* https://istio.io/docs/concepts/traffic-management/





* VirtualService

  `how you route your traffic **to** a given destination`

  * 由一组路由规则组成，按序evaluate

  * 大致结构

    ```yaml
    apiVersion: networking.istio.io/v1alpha3
    kind: VirtualService
    metadata:
      name: reviews
    spec:
      hosts:  # 作用于哪些"目标"
      - reviews
      http:   # http类型的一组理由
      # 可以增加其他类型的，比如： xxx: 
    ```
    
  * `hosts` 
  
    `can be an IP address, a DNS name, or, depending on the platform, a short name (such as a Kubernetes service short name) that resolves, implicitly or explicitly, to a fully qualified domain name (FQDN). You can also use wildcard (”*”) prefixes, letting you create a single set of routing rules for all matching services. Virtual service hosts don’t actually have to be part of the Istio service registry, they are simply virtual destinations. This lets you model traffic for virtual hosts that don’t have routable entries inside the mesh.`
    
    重点： **don’t actually have to be part of the Istio service registry, they are simply virtual destinations**
  
* DestinationRule

  `configure what happens to traffic **for** that destination`

  > 或者说  what exactly that destination is and how it can be reached

  一般包括：
  
  * apply给哪个(host、服务)
  
  * “分割”subset
  
  * 流量策略(LB等)
  
    rule级别、subset级别
  
  > pre-splitted subset，因为如果涉及多维切分的话，难免会有重复、重叠，在envoy侧的数据量会很大
  > 所以，似乎是不如envoy侧自行split，下发split方式即可： 比如meta - 这样的成本在于，instance meta可能会比较多，可以尽量用短的key/value来缓解
  
  ```yaml
  apiVersion: networking.istio.io/v1alpha3
  kind: DestinationRule
  metadata:
    name: my-destination-rule
  spec:
    host: my-svc
    trafficPolicy:
      loadBalancer:
        simple: RANDOM
    subsets:
    - name: v1
      labels:
        version: v1
    - name: v2
      labels:
        version: v2
      trafficPolicy:
        loadBalancer:
          simple: ROUND_ROBIN
    - name: v3
      labels:
        version: v3  
  ```

* Gateway

  apply to：

  * `√` proxies at the edge of mesh
  * `×` sidecars（along with workloads)

  优点、特点： 复用既有的istio模型、部署形式来实现gateway，也即
  
  * dataplane可以一样

  * controlplane的数据模型也类似，只是下发内容有差别

  in/out:
  
  * ingress为主

  * 也可以egress
  
* Service Entry

  用于描述istio角度的“服务”，现实意义上：
  
  * istio内可以通过添加该类型条目来增加服务

  * 外部产生的服务，一样可以以该类型的方式提供： MCP registry controller
  
  ```yaml
  apiVersion: networking.istio.io/v1alpha3
  kind: ServiceEntry
  metadata:
    name: svc-entry
  spec:
    hosts:
    - ext-svc.example.com
    ports:
    - number: 443
      name: https
      protocol: HTTPS
    location: MESH_EXTERNAL
    resolution: DNS
  ```

* [Sidecar](https://istio.io/docs/concepts/traffic-management/#sidecars)

  Sidecar配置：

  - Fine-tune the set of ports and protocols that an Envoy proxy accepts.
  - Limit the set of services that the Envoy proxy can reach.

  作用范围：

  * 特定NS（的所有workloads）
  * 特定workloads（由`workloadSelector`来描述）

  

  参见： [Sidecar reference](https://istio.io/docs/reference/config/networking/sidecar/)

* Network resilience and testing

  * timeouts

    可以在VS的路由的级别进行配置(如果该路由类型支持的话，比如http)

  * reties

    也是在VS的路由级别进行配置，如 重试次数、重试超时时间

  * circuit breakers

    在DR的subset的trafficPolicy中配置，如连接池配置最大连接数等

  * fault injection

    在VS的路由级别配置，如http路由的`fault`字段
    
    主要是业务层/应用层注入错误，支持两种： 延迟、abort
    
    最后在istio侧会翻译为envoy侧的fault： `networking.HTTPFaultInjection into Envoy's HTTPFault`
    
    ```yaml
    http:
    - fault:
        delay:
          percentage:
            value: 0.1
          fixedDelay: 5s    
    ```




