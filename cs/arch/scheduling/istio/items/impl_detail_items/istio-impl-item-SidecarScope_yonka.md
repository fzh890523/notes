

## 初始化sidecarScope

见 《istio-impl-item-PushContext_yonka.md》

### 理解

* 因为convert过程需要获取、处理、缓存资源，所以如果资源发生变化，是要重新计算的

  主要是服务发生变化

## proxy所用sidecarScope的确定

* `SetSidecarScope`

  时机

  * `initConnectionNode` 时会

  * `pushConnection` 时如果是full-push会进行

  细节

  * 对于proxy： 根据`node.WorkloadLabels`选择 （`PushContext.getSidecarScope`）

    * 取该proxy所在ns的全部sc，进行遍历

      * 如果该sc有workloadSelector且能匹配该proxy，则选定

      * 否则作为fallback的sc

    * 如果有作为fallback的sc，则选定

    * 否则使用该ns的默认sc

  * 对于其他（gateway）： 取该ns的默认sidecarScope



## items



### OutboundTrafficPolicy







#### 处理

在pushContext初始化时从`Sidecar` crd里解析出，赋给对应的`SidecarScope`，而最后match某个sidecar的只有一个ss，so也只有一个policy。

详见： 《istio-impl-item-PushContext_yonka.md》 中sidecarScope的init过程



#### 生效



* mixer plugin的`OnOutboundListener`： 为fallthrough标记的filterchain append上mixer的filter

  其中，会根据该policy来选择不同的destination（保持与之前构建fallthrough的逻辑的一致性）。 实际是选择不同的svc来作为destination信息提供者（blackhole or passthrough）

  这个fallthrough标记，是在`buildSidecarOutboundListenerForPortOrUDS`中`appendListenerFallthroughRoute`这一步，`if features.EnableFallthroughRoute.Get() `则会生成兜底的filterchain，同样的，根据policy来决定tcpproxy filter

* `ConfigGeneratorImpl.onVirtualOutboundListener` <- `ListenerBuilder.buildVirtualOutboundListener` 其实是为了调plugin，但要准备好plugin参数，其中的svc，就是根据policy来选择，blackhole或者passthough

  这个svc在plugin中可能用到，比如mixer插件会从svc里拿到服务信息（以添加到report数据里）

* `ConfigGeneratorImpl.buildSidecarOutboundHTTPRouteConfig` 中在build的最后（apply plugin之前），根据 `if features.EnableFallthroughRoute.Get() && !useSniffing` 来生成兜底路由： 对所有域名`*`的根path `/`，同样的，根据policy的不同，生成的路由指向blackhole或者passthrough

* `ListenerBuilder.buildVirtualOutboundListener` 根据policy为virtualOutboundListener添加兜底的filterchain（的filter）（不需要match），`newTCPProxyOutboundListenerFilter`里根据policy来选择blackhole或者passthrough（tcpProxy类型）



以上一大通，感觉是code path没管理好，其实就两种：

* listener添加兜底filterchain，virtual outbound是一定会添加，普通outbound要根据feature开启情况
* http route添加兜底route





## 对外接口



### `Services() []*Service`





### `DestinationRule(hostname host.Name) *Config`





### `GetEgressListenerForRDS(port int, bind string) *IstioEgressListenerWrapper`



### `DependsOnNamespace(namespace string) bool`





### `IstioEgressListenerWrapper.Services() []*Service`





### `IstioEgressListenerWrapper.VirtualServices() []Config`













