


pushContext理解： 全局的一个push相关的resource manager

## 内容



#### 一些exportTo相关配置

```go
// Instance defines whether a given config or service is exported to local namespace, all namespaces or none
type Instance string

const (
	// Private implies namespace local config
	Private Instance = "."
	// Public implies config is visible to all
	Public Instance = "*"
)
```





* `defaultServiceExportTo         map[visibility.Instance]bool` 

  逻辑和vs同，配置为： `Mesh.DefaultServiceExportTo`

* `defaultVirtualServiceExportTo  map[visibility.Instance]bool`

  ```go
  	if ps.Env.Mesh.DefaultVirtualServiceExportTo != nil {
  		for _, e := range ps.Env.Mesh.DefaultVirtualServiceExportTo {
  			ps.defaultVirtualServiceExportTo[visibility.Instance(e)] = true
  		}
  	} else {
  		ps.defaultVirtualServiceExportTo[visibility.Public] = true
  	}
  ```

  也即，默认没配这个的话，是public的

* `defaultDestinationRuleExportTo map[visibility.Instance]bool`

  逻辑和vs同，配置为： `DefaultDestinationRuleExportTo`

  



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

  

#### init virtual service



##### 1.3

> vs有 exportTo 和 gateways 两个字段，都用于“限定”其影响范围
>
> * exportTo -> ns
> * gateways -> gw(mesh作为一个特殊gw)
> * hosts --- 与service关联



1. 取所有ns的vs

2. 做deep copy

   因为后面有一些内容的填充、转换会塞回原数据结构

3. vs数据预处理、填充

   1. hosts处理： short -> FQDN

      > 允许：
      >
      > * `*`
      > * FQDN（包含`.`）
      >
      > 以上两种都直接返回
      >
      > * short name -> FQDN，详见如下

      1. ns不为空的话，-> `<host>.<ns>`

      2. domain不为空的话， -> `<...>.svc.<domain>`

         这个domain来自istio自己的`Config.ConfigMeta.Domain`，实际是k8s crd转换为istio config对象时用配置填充的（晕），来自命令行参数`domain`，默认为`cluster.local`

         > 估计是给k8s做填充，算是hack逻辑

         > 这个隐式包含了只有k8s服务才能使用short name... 代码里加了FIXME但一直到master也没改

   2. gateways处理

      > 允许： 
      >
      > * `IstioMeshGateway`也即`mesh`： 不处理
      >
      > * `./<gw>`： -> `<gw>`
      > * `<gw>.<ns>`：  -> `<ns>/<gw>` 好像向下兼容
      > * `<gw>`：  -> `<ns>/<gw>`，ns取的是vs的ns
      >
      > 也就是说这里认为gw本身只能是short name了

   3. http路由处理
      1. `Match[*].Gateways`字段处理，方式如上gw
      2. `Route[*].Desination.Host`字段处理，方式如上host
      3. `Mirror.Host`字段处理，方式如上host
   4. tcp路由、tls路由处理
      1. 一样的，`Match[*].Gateways`字段处理...
      2. `Route[*].Destination.Host` 。。。

4. vs数据分派

   > 分成 nsPrivateVSs 和 publicVSs。 根据相关配置决定export到哪里（其一）
   >
   > master上加了一级gw，也即： 
   >
   > * pub: {gw: []}
   > * nsPrivate: {ns: gw: []}
   >
   > > 把 IstioMeshGateway="mesh" 作为gw的特殊情况： all sidecars。 所以

   * 如果没有 exportTo 字段内容的话，则使用 ps的全局配置（`defaultVirtualServiceExportTo`，而这个配置来自meshConfig，默认是public的）来确定export到哪里

   * 否则： 

     > 1.3, 1.5都只处理了第一个exportTo元素。 master上全处理了，并且支持exportTo other ns了（不再限于 current 和 public

     根据exportTo来决定是到current ns 还是 public

     

     

#### init destination rule



##### 1.3



1. list all DRs of all-ns

2. 按createTime排序

3. 遍历

   * 填充内容

     1. `.Host` short -> FQDN，和vs一样
     2. 

   * 分派

     > 1.3的exportTo还没完全支持，`.`不支持。 所以变成 **默认每个dr都export to local/current ns**
     >
     > 同样的，也只考虑exportTo的第一个元素
     >
     > 目前会export到这几个目标：
     >
     > * ns local： 每个ns自己（定义）的 （public or private）
     >
     > * ns exported： 每个ns的public的（这个public可能是自己没定义exportTo而由ps级别的exportTo确定的）
     >
     >   > ns的每个value是ns local对应的value的子集吧
     >
     > * all exported（public）： 所有public的dr
     >
     >   > ns public的（values的）并集

     1. export to local ns，就是按资源的ns进行聚合（`processedDestRules{hosts []host.Name, destRule map[host.Name]*combinedDestinationRule}`），聚合的逻辑下面单独说
     2. 根据exportTo field（如果没有的话根据`ps.defaultDestinationRuleExportTo`判断是否public）
     3. 如果是public，则export到 ns public 和 all public
     4. 对所有的聚合的内容的hosts做排序

     **merge的逻辑**：

     > **看代码，没有做通配host的merge，也即 short -> FQDN 之后的host即使是通配，也是一个完全独立的merge单元了，不会再合并不同的hosts**
     >
     > `combinedDestinationRule{subsets map[string]struct{}, config *Config}`
     >
     > 可以看到，实际只聚合了subsets...

     1. 聚合subsets，用了个map来去重，<u>对于同样的hosts，不允许（跳过后来的）重复的subset（name）</u>。 不重复的，append到第一个的末尾

        重复的，计数...

     2. 聚合TrafficPolicy： ... 只取第一个非nil的

        言下之意是<u>对于同一个host，只允许有一种tp了</u>

     

     

#### init sidecar/sidecarScope



##### 1.3

1. list all (ns) `Sidecar` crds

2. 按createTime排序

3. 做个排序，有selector的排到前面，其他相对顺序不变

4. 遍历，`ConvertToSidecarScope`，然后按ns做简单的分组为： `map[string][]*SidecarScope`

5. 如果`env.Mesh.RootNamespace != ""`，尝试找到`rootNSConfig` - <u>rootNS对应的第一个selector为nil</u>的`Sidecar` 

   如果为空，自然就没有了

6. 遍历所有ns（其实是所有service的ns并集），如果该ns没有对应的`SidecarScope`，则**用rootNSConfig**为其生成ns default sidecarScope - `ConvertToSidecarScope`

   从下面可以知道，没有rootNSConfig时，生成的是`DefaultSidecarScopeForNamespace`



###### `ConvertToSidecarScope`

1. 如果传入的config为空，则`DefaultSidecarScopeForNamespace`

2. 遍历`.Egress`，进行预处理： `convertIstioListenerToWrapper`

3. 把所有egressListener所导入的服务的信息记录为 `NamespaceForHostname map[host.Name]string` （hostname:ns） ，如有重复（多个ns导入同一个服务），只取第一个（ns）

4. 按同样的方式，正式导入服务实例到`.services []*Service`。 同时记录依赖（实际使用到）的ns `.namespaceDependencies map[string]struct{}`

5. 根据导入的服务实例，获取对应的destinationRule，保存到 `.destinationRules map[host.Name]*Config`

   `out.destinationRules[s.Hostname] = ps.DestinationRule(&dummyNode, s)`

6. apply `OutboundTrafficPolicy`，如果为空则从配置`ps.Env.Mesh.OutboundTrafficPolicy.Mode`里取值构建填充

7. apply `Ingress`

   这个数据没有预处理，在使用处直接用

   不为空则`HasCustomIngressListeners = true`



###### `convertIstioListenerToWrapper`

1. 解析`Hosts`：`ns/host` 格式，所以解析为 `listenerHosts map[ns][]host`

   也要注意，ns可以是特殊匹配内容，`~`,`.`,`*`； host可以是左通配 `*.xx.com`, `*`。

   其中`.` ns这里会翻译为传入的 `configNamespace`

2. `selectServices` 

   1. 拿configNamespace应该有的所有的服务（private+public） - `ps.Services(&dummyNode)`

      这里体现的是 *service的“意愿”* - 给不给

   2. 遍历服务，apply本scope的“意愿” - `Host`规则 - 要不要

      会依次看当前ns和通配ns（`*`）下的hosts是否能匹配该服务

   3. 对实际选择的服务实例做去重： 如果从多个ns都导入了同一个服务实例，只选用第一个

3. `selectVirtualServices`

   1. 拿configNamespace对应的所有的 **apply到mesh的** vs（pub+private） - `ps.VirtualServices(&dummyNode, meshGateway)`

   2. 从解析`Hosts`阶段得到的`listenerHosts`拿ns对应的service hosts，对其遍历，同时也遍历 `vs.Hosts`，（二层循环中）看二者是否match，match则select

      注意，这里两个host都支持（前缀）通配，所以`Match`的实现稍微有点复杂，见 `host.Name.Matches`

      > 这里有个bug，**一直到master（1.8）**都没修： vs可能apply to多个host，而这里是根据egressListener的host来match vs，可能只match了vs 的部分hosts，但加入之后再通过vs拿hosts会更多，于是... 
      >
      > 如： [a,b,c] -match-> vs[c,d,e]，然后从该vs拿到了d和e...
      >
      > 
      >
      > 不过看了部分使用处的代码，实际是先从el拿services，然后把这些services和vs一起比对得到 servicesInVirtualServices，感觉逻辑上没问题。 所以实际是不是真的遗留bug还待确认

   3. 从`listenerHosts`拿通配ns`*`对应的service hosts

      同样做如上处理





###### **DefaultSidecarScopeForNamespace**

> 特点： 处理ns是传入的指定ns外，其他都是通配
>
> yonka： 不明白为什么不构建一个包含通配数据的sidecar然后走同样的convert逻辑？ 那样要优雅一些
>
> 数据： 效果上来说，会拿到以下数据
>
> * service： 能拿到export到该ns的所有ns private service + public service
> * vs： 能拿到export到该ns的apply to mesh gateway的所有ns private vs + public vs
> * dr： 能拿到

1. 用`listenerHosts: {wildcardNamespace: {wildcardService}}` 构建 defaultEgressListener
2. `ps.VirtualServices(&dummyNode, meshGateway)` 填充el的`virtualServices`
3. 用el来构建`SidecarScope`，services就用el的services

3. 用`{defaultEgressListener}`来计算填充 `NamespaceForHostname`
4. 遍历services，获取dr来填充
5. 填充`OutboundTrafficPolicy`



## 对外接口



### `Services(*Proxy) []*Service`



1. 优先尝试从proxy的sidecarScope获取： `proxy.SidecarScope.Services`

   前提： `proxy != nil && proxy.SidecarScope != nil && proxy.Type == SidecarProxy`

否则才走下面自己的逻辑

2. 取proxy所在ns的private services

   前提是 `proxy != nil`，否则只能取所有ns的private services

3. 取public services



### `VirtualServices(*Proxy, gateways map[string]bool) []Config`

1. 取ns private vs
   * `proxy == nil`： 取所有ns的
   * 否则： 取proxy的ns的

2. 取public vs

3. 把取得的vs进行遍历筛选： 能apply传入的gateways的

   判断的依据 `vs.Gateways` 和 传入的gateways 有交集

   > 如果`vs.Gateways`为空，认为其（只）apply to 默认的mesh gateway

   > yonka：这里对`vs.Gateways`的内容又做了`resolveGatewayName`处理，感觉重复了，因为这个vs来自ps内部存储的数据，而这个数据在init阶段应该已经做过该处理了





### `DestinationRule(proxy *Proxy, service *Service) *Config`

1. `proxy == nil`： 因为没有ns信息，只能从 `allExportedDestRules` 里找

   遍历`allExportedDestRules.hosts`尝试匹配svc Hostname，匹配则返回其`config`，否则返回nil ↗

2. 如果proxy是sidecar，并且有sidecarscope，则从sc取 ↗
3. 





