

# http route



## 数据设计



* `name` 用端口，如 `8080`

  > yonka疑问： 如果启用`IsProtocolSniffingEnabledForOutbound`的话，可能route name的格式是`<xxx>:<port>`。 

* `virtual_hosts []`

  * `name` 服务名+端口，如 `istio-pilot.istio-system.svc.cluster.local:8080`
  * `domains []` 域名的重载形式，比如带端口的，不带的，等等
  *  `routes []`
    * `match`
    * `route`
    * `decorator`
    * `per_filter_config`

* `request_headers_to_add`

* `request_headers_to_remove`

* ...



## build 流程



* `pushRoute`

  > 这里就是指http route了吧

  * `generateRawRoutes`

    把`con.Routes`作为参数传入下面

    * `ConfigGenerator.BuildHTTPRoutes`

      * sidecar： `configgen.buildSidecarOutboundHTTPRouteConfig`

        迭代处理，传入`routeName`，传入了个`vHostCache`来避免重复生成

      * gw/router： `configgen.buildGatewayHTTPRouteConfig`

        迭代处理，传入`routeName`

      对于生成的路由，都会`envoyfilter.ApplyRouteConfigurationPatches`，只是（传入的）context不同： `EnvoyFilter_SIDECAR_OUTBOUND` vs `EnvoyFilter_GATEWAY`

      而如果没生成，会补一个空route（保证routeName对应的数据存在吧）

      

      

