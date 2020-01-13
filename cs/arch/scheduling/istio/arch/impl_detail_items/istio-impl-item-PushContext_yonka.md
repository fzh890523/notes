


pushContext理解： 全局的一个push相关的resource manager

## 内容



## 流程

### init/update时机

* 在`StreamAggregatedResources`开始时会对`globalPushContext`进行`InitContext`

  这里其实只是lazy init

* 在`Push`里会调`InitContext`

* `InitContext`

  根据 `if pushReq == nil || oldPushContext == nil || !oldPushContext.initDone || len(pushReq.ConfigTypesUpdated) == 0` 条件决定是哪种
  
  * `createNewContext`
  * `updateContext`

### init过程

* serviceRegistry

  取所有（registry）的服务，按照namespace和visibility来分类保存：

  * 各ns private的服务

  * public的服务

  * hostname:ns:services 

  > 如果没有exportTo的话，会用default行为（来自配置）

* virtualService

  取所有，做一些数据清理、转换，然后同样根据namespace和visibility来分类为：

  * 各ns private的VirtualService

  * public VirtualService

* destinationRule

   ...
   同上，export规则稍微复杂一些，具体再看

* 鉴权和授权规则...

* envoyFilters

  per-namespace

* gateway

  per-ns

* sideScope

  * 取所有`Sidecar`资源

  * 都转为`SidecarScope`资源，按ns进行分组

    这步转换的计算有一些，会获取、转换、存储一些资源。 参见 [istio-concepts_SidecarScope_yonka.md](../concepts/istio-concepts_SidecarScope_yonka.md)

  * 根据配置中的`RootNamespace`来确定rootNs以及对应的`Sidecar`资源

  * 为所有没有`sidecarsWithoutSelector`的ns，以rootNs和对应`Sidecar`资源创建一个`SidecarScope`资源

  