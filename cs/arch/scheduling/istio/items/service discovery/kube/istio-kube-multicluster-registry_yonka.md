


## MiltiCluster的逻辑



* kube serviceDiscovery/controller

  `initServiceControllers`的kube registry case里`createK8sServiceControllers` -> `pilot/pkg/serviceregistry/kube/controller/controller.go::NewController`

  区别于multicluster，这个叫做`primary cluster`，clusterID是固定的 **Kubernetes**

* kube multicluter “registry”

  `initClusterRegistries`里 -> `pilot/pkg/config/clusterregistry/multicluster.go::NewMulticluster`

  > 这部分代码明明是serviceregistry的，放在config里不合适。
  >
  > 1.5里移到`serviceregistry/kube/controller`里了

  这个multicluter registry实际上并不是一个registry，而是在`SecretController`的add/remove回调中新增、删除特定cluster的Controller（同上）

  所以这个registry甚至没有`Run`/`Start`，而是new时就`StartSecretController`



### AddMemberCluster

1. 用回调里的client、clusterID来构建kube controller，并初始化 `kubectl.InitNetworkLookup(m.meshNetworks)`

2. `m.serviceController.AddRegistry`

3. 缓存该controller

4. 为controller注册svc、inst回调： 都是full push...

   aggre原先下面的（静态）registries在aggre注册回调时已经遍历注册了，动态的要额外注册

5. start 该controller



### DeleteMemberCluster

1. 从aggre serviceController删除clusterID对应的（kube）registry

   但这里没有registry type/source的概念，意味着如果其他registry用了同样的clusterID的话

   > 当然了，这里实现上强耦合kube，所以clusterID不为空就是指kube，所以... o(╯□╰)o

2. 删除本地缓存的这个kube controller
3. 触发full-push（如果XDSUpdate不为空）



## SecretController



1. 用主k8s client（也就是访问当前k8s集群了？）去访问secret namespace拿secret resource，筛选`istio/multicluster=true`的，list+watch方式

2. watch回调为key入queue（add、de）

3. worker一直取queue内容进行处理，根据这个key是否还存在（获取key对应的secret内容）来判断是del还是add事件（o(╯□╰)o），分别调：

   > secret.data的格式为： {clusterID: kubeConfig}

   * `addMemberCluster` 遍历所有，如果缓存的clusters中有相同clusterID的则跳过，没有的话则做初始化： load config、校验、构建cluster、加入缓存、构建client、**回调**addCallback
   * `deleteMemberCluster` 遍历所有，对于缓存的clusters的secretName与这个相同的，都执行删除逻辑： **回调**removeCallback、删除缓存

   

## 生成的服务

从以上流程可以看到，member cluster对应的就是一个个普通kube controller，那么... 从这里看不出区别。

部分内容聚合的逻辑在aggre层做，见： 《istio-pilot-service_discovery_yonka.md》的对应章节。



## 如何调用？



1. 要开启了多网络

2. 有对应网络的网关配置

   从网关配置拿到地址，或者拿到网关关联的服务的external地址

3. 把特定网络的endpoints替换成对应的gw endpoints



