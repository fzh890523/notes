
## service实例的获取、筛选

* registry： 给出自己的全部services

  不过对于每个service，namespace只能是某个；

  没有clusterId字段，但实际上，现在没限制多个registry都提供（假设同ns的）同一个服务（的实例）；

  从 `Services()` 和 `GetService(hostname)` 接口的一致性来说，要求对于一个服务，只有一个实例（属于某一个ns）；

  > 不过考虑到 `GetService` deprecated 了，且描述为只适合用于测试。
  > 那是不是意味着，这个约束不需要成立了

* 聚合registry

  * 对于ServiceInstances： 简单聚合

  * 对于Services： 除k8s registry，也是简单聚合
  
    k8s会实际聚合为一个。 但这里有点问题： **不同ns的也聚合为1个了**
    除非说k8s没有这个情况。 但，如果`namespace`映射到`tenant`的话，总不能说用k8s registry就不用多tenant啊

    > 注释里说本意是为了聚合多cluster的。。。

  * GetService： 取第一个（但既然deprecated，忽略）  

* pushContext级别

  取所有服务，按照namespace、exportTo来分组为 privateServicesByNamespace （`map[string][]*Service`） 和 publicServices （`[]*Service`）；

  但从 `ServiceByHostnameAndNamespace map[host.Name]map[string]*Service` 来看，是**没有给同ns不同cluster（registry）的同一service留下位置的**；

* sidecarScope级别

  根据configNamespace从前面的private/public服务中选择对应的服务，再根据import条件来匹配；

  最后如果一个hostname匹配到多个ns的Service的话，只取第一个ns并记录ns依赖关系；

  > 这时理论上不排除选择了一个ns内的多个cluster的Service实例，前提是允许... 

## 同hostname的不同Service实例

* 多ns的service实例

  除了k8s registry比较奇怪以外，其他应该ok

  ```
  Also I notice that aggregate.Controller.Service()  will group all Services of k8s multicluster registries to a single one which means the namespace info is lost.
  As I understand namespace in istio as a concept similar to tenant . We could not use tenant for k8s multicluster registries case?


  Only for ServiceEntry it has multiple
  if you have a k8s registry how do you have the same hostname in different namespace though
  namespace is part of the hostname
  ```

* 多cluster的service实例

  以上，应该是不支持的

  ```
  Hey, could anyone help explain if multiple cluster(registry)s can provide model.Service instances of the same hostname (in a single namespace).
  
  I see `ServiceByHostnameAndNamespace map[host.Name]map[string]*Service` does not leave position for that. But obviously multiple clusters can provide model.ServiceInstances of same service.

  So cluster  concept is just a hack for k8s registry and not a formal concept of istio ?
  ```