



# common listener











# http listener



## 数据设计



```json
    {
     "version_info": "2021-01-18T12:20:23Z/2",
     "listener": {
      "name": "0.0.0.0_8080",
      "address": {
       "socket_address": {
        "address": "0.0.0.0",
        "port_value": 8080
       }
      },
      "filter_chains": [
       {
        "filter_chain_match": {
         "prefix_ranges": [
          {
           "address_prefix": "x.x.x.x",  // 某具体地址
           "prefix_len": 32
          }
         ]
        },
        "filters": [
         {
          "name": "envoy.tcp_proxy",
          "typed_config": {
           "@type": "type.googleapis.com/envoy.config.filter.network.tcp_proxy.v2.TcpProxy",
           "stat_prefix": "BlackHoleCluster",
           "cluster": "BlackHoleCluster"
          }
         }
        ]
       },
       {
        "filters": [
         {
          "name": "envoy.http_connection_manager",
          "typed_config": {
           "@type": "type.googleapis.com/envoy.config.filter.network.http_connection_manager.v2.HttpConnectionManager",
           "stat_prefix": "0.0.0.0_8080",
           "rds": {
            "config_source": {
             "ads": {},
             "initial_fetch_timeout": "0s"
            },
            "route_config_name": "8080"
           },
           "http_filters": [
            {
             "name": "mixer",
             "typed_config": {
              "@type": "type.googleapis.com/istio.mixer.v1.config.client.HttpClientConfig",
              // ...
             }
            },
            {
             "name": "envoy.router",
             "config": {
              "start_child_span": true
             }
            }
           ],       
           "tracing": { ... },
           "access_log": [ ... ],
           "use_remote_address": false,
           "generate_request_id": true,
           "upgrade_configs": [ ... ]
           ],
           "stream_idle_timeout": "0s",
           "normalize_path": true
          }  // end of typed_config HttpConnectionManager
         }  // end of filter http_connection_manager
        ]  // end of filters
       }  // end of current filter chain
      ],  // end of filter chains
      "deprecated_v1": {
       "bind_to_port": false
      },
      "listener_filters_timeout": "0.100s",
      "traffic_direction": "OUTBOUND",
      "continue_on_listener_filters_timeout": true
     },              
```



简化后的结构：

* `name`
* `address`
* `filter_chains []`
  * `filter_chain_match`
  * `filters []`



# build listener



> 基于1.3

* `pushLds`

  * `generateRawListeners`

    * `ConfigGenerator.BuildListeners`

      * sidecar: `configgen.buildSidecarListeners`

        > `mesh.ProxyListenPort` 这个配置决定proxy是否`listen for incoming connections`，`== 0`的话就不用listeners了

        1. inbound listeners `buildSidecarInboundListeners`

        2. outbound listeners `buildSidecarOutboundListeners`

        3. management listeners `buildManagementListeners`

        4. virtual outbound listeners `buildVirtualOutboundListener`

           `0.0.0.0:15001`那个，见下面

           代码里直接叫： `ipTablesListener`

        5. virtual inbound listeners `buildVirtualInboundListener`

           类似的，`0.0.0.0:15006`，见下面 **build inbound virtual listener**

      * gw/router: `configgen.buildGatewayListeners`

      `builder.patchListeners`、执行build得到listeners



# virtual listeners



## inbound 

类似的，`0.0.0.0:15006`，没有用`"use_original_dst": true` ，而是 `"listener_filters": [{"name": "envoy.listener.original_dst"}]`，然后在`filter_chains`里显式dispatch了

dispatch逻辑如下：

* 具体ip+port匹配

  * 15020 -> `"cluster": "inbound|15020|mgmt-15020|mgmtCluster"`
  * local services -> static route_config -> ... inbound cluster

  这些应该都是聚合来的

* 通配 `0.0.0.0/0` -> `"cluster": "InboundPassthroughClusterIpv4"`

  这个是显式生成的



### build inbound virtual listener

1. 拿到node的通配地址（只要有一个v4的就是`0.0.0.0`，否则是v6的通配）

2. 生成passthrough的filterchains： `newInboundPassthroughFilterChains`

   > 会根据node对ipv4、v6的支持决定生成哪些，对应的cluster name是固定的： `InboundPassthroughClusterIpv4/6`
   >
   > 这些cluster会在build clusters时加上，它们是`ORIGINAL_DST`（注意不是`STATIC`）
   >
   > 详见： [istio-impl-item-xDS-CDS_yonka.md](istio-impl-item-xDS-CDS_yonka.md)

   > 下面以v4为例

   * filterchainmatch是 `0.0.0.0/0` ，应该是优先级最低的match，也即“兜底”

   * filter是指向以上clutername的`TCPProxy`

3. 如果启用ProtocolSniffing并且node支持（版本>=13），添加`HTTPPassThroughFilterChain`

   > yonka疑问：这个为什么要加上呢？ 防止sniff失败？

4. 用filterchains构建`virtualInboundListener`

5. `if builder.useInboundFilterChain` （默认true），`builder.aggregateVirtualInboundListener`： 把inbound方向non bind-to-port的listeners的filterchains聚合到这个listener（**但原来的仍然保留**）

   > 加了match，因为原来的listener通过listener地址端口信息“天然match”，而聚合到virtual后，需要额外match： 把原地址端口信息以精准匹配形式加进去

   一些细节：

   * 把该listener的`UseOriginalDst`置空，换成ListenerFilters方式：

     ```go
     ListenerFilters = append(builder.virtualInboundListener.ListenerFilters,
     		&listener.ListenerFilter{
     			Name: xdsutil.OriginalDestination,
     		},
     	)
     ```

   * 如果聚合的filterchains里有需要tls的，加上ListenerFilter `Name: xdsutil.TlsInspector`
   * 如果`IsProtocolSniffingEnabledForInbound`，加上ListenerFilter `Name: envoyListenerHTTPInspector`

   



## outbound

`0.0.0.0:15001`那个，`"use_original_dst": true` 以及 没有 `"bind_to_port": false`表示bindToPort。 会dispatch到真实listener之后重新过filters吧。

> 需要注意的是，这个filter里**use_original_dst才是重点**，自己的filterchains会在`original_dst`没有命中的情况下做尝试，所以兜底逻辑可以放在里面，或者说这里面的是兜底逻辑



### build outbound virtual listener



1. `newTCPProxyOutboundListenerFilter` 这里定义的是outbound的兜底逻辑

   根据`node.SidecarScope.OutboundTrafficPolicy.Mode == networking.OutboundTrafficPolicy_ALLOW_ANY`（默认不成立）决定是允许（passthroughcluster）还是拒绝（blackholecluster）

   > 这是全局的，也即所有（未匹配）流量

2. 如果`features.RestrictPodIPTrafficLoops` （默认true），在filterchains最前面加上一个blackhole filterhain，对到`node.IPAddresses`所有地址的流量做丢弃处理

   > 注释里提到，这个只是为了向下兼容，之后可能会删掉这个参数（也就是这个逻辑一定存在？）
   >
   > yonka疑问： 注释里说这个是为了避免无限请求。 那怎么样无限请求呢？
   >
   > **master上这个逻辑删掉了**，不过加上了一个禁止直接请求proxy listen port的filterchain 😂

3. 対生成的`ipTablesListener`做后处理： `configgen.onVirtualOutboundListener`



# management listener



## build sidecar management listeners

1. 如果sidecar（scope）定义了ingress listener，那么跳过该listener生成

   > 是自定义了...的话，则跳过所有的inbound listeners生成（virtual除外了）
   >
   > 不过management也不要，有点神奇

2. 如果该proxy是不拦截模式（`node.GetInterceptionMode() == model.InterceptionNone`），那么是认为该业务本身是提供了management ports，让它自己访问就行了，所以也跳过生成

   > 这个主要针对的是拦截方式吧，非精准（port）拦截的话，把所有流量都捞过来了，“就得对人家负责”

3. 遍历所有proxy ip，从registry拿到这个ip上的所有 management ports

4. ip + ports构建对应的listeners

   简单的说，就是为每个 ip+port 生成一个serviceinstance，再用这个instance来 `buildInboundNetworkFilters` -> 构造`filterChainOpts` -> 构造`buildListenerOpts` -> `buildListener` -> `buildCompleteFilterChain` -> append...

   > build数据的逻辑太复杂，定义了很多函数做部分事情，但又没有很好的描述，导致看起来比较混乱。
   >
   > 对复杂逻辑的结构化还是很重要的，oop是一种思路

5. 处理一种情况： management listener 的端口和服务的端口一样，这样会出问题

   处理方式： 去掉与已有（inbound + outbound）listener addr（addr+port）相同的management listener



# inbound listener



## build sidecar inbound listener

* `ListenerBuilder.buildSidecarInboundListeners`

  * `configgen.buildSidecarInboundListeners`

    一样的，分两种情况： 

    * 有自定义 ingress listeners

    * 没有...

      > 正常的根据匹配数据（proxy instances）生成...

      

### 根据proxyinstances生成

> 这里做了个考虑： **如果不是拦截流量模式**的话（影响bind_to_port配置吧），直接listen service port会导致业务应用（workload）无法监听，这肯定是不行的。 所以**这里会直接退出，不生成对应listeners**

1. 如上，不拦截就返回（不生成）

2. 构建了一个`listenerMap map[string]*inboundListenerEntry`，传入实际的build函数，避免重复

   key是`<bind>:<port>`，分别是endpoint的address和port

3. 遍历serviceInstances（proxy instances）

   1. 用instance相关数据构建`buildListenerOpts`

   2. 用相关信息构建`InputParams`

   3. `configgen.buildSidecarInboundListenerForPortOrUDS`

      



### `buildSidecarInboundListenerForPortOrUDS`

1. 先对`listenerMap `做判重，如果已有则返回（记录冲突）

   理论上，对于一个endpoint不应该在同一个`addr:port`上提供多个服务（以及对应的serviceInstance）

   > yonka： 但这对于hsf/dubbo等服务模型显然不成立，不能简单的认为是错误冲突情况并记录。 所以要直接复用的话，这里的逻辑要做一些处理

2. 提供plugin hook，调用各个插件的： `OnInboundFilterChains` 来提供（pre的）filterchains

3. 如果plugin没有生成fc，则构建一个默认的添加进去

4. 如果当前协议为auto，double一下filter chains（前面一半http、后面一半tcp）

5. 遍历每个fc，为其生成filters等

   这里是根据listener协议来处理的：

   * http： `configgen.buildSidecarInboundHTTPListenerOptsForPortOrUDS`

   * tcp： `buildInboundNetworkFilters`

   * auto： 

     * 如果是前一半的fc，作为http处理

       一些额外的逻辑： 

       1. 在filterchainmatch的`ApplicationProtocols`里加上`{http/1.1, http/1.0}` 
       2. 如果第一个ap是`istio`，则把`TransportProtocol`设为`tls`

     * 后一半，作为tcp处理



### `buildSidecarInboundHTTPListenerOptsForPortOrUDS`

1. 构建clusterName

   `inbound, endpoint.servicePort.name, service.Hostname, servicePort.port`

   允许传入参数里指定clusterName，特殊的场景下的clusterName不是按这个规则生成的

   > yonka： 同样的，这里其实inbound方向一个 port 只会有一个服务。 这太耦合k8s服务了

2. `buildSidecarInboundHTTPRouteConfig` 生成通配的默认路由

   vhost：name `<clusterName>`， domains `[*]`

   `BuildDefaultHTTPInboundRoute`生成具体route内容： 

   ```json
              "route_config": {
               "name": "<clusterName>",
               "virtual_hosts": [
                {
                 "name": "inbound|http|<svcPort>",  // hardcoded + strformat
                 "domains": [  // hardcodes
                  "*"
                 ],
                 "routes": [
                  {  // from BuildDefaultHTTPInboundRoute
                   "match": {  // from translateRouteMatch(nil)
                    "prefix": "/"
                   },
                   "route": {  // hardcoded
                    "cluster": "inbound|<svcPort>|<svcPortName>|<svc>.<ns>.svc.cluster.local",  // from <clusterName>
                    "timeout": "0s",
                    "max_grpc_timeout": "0s"
                   },
                   "decorator": {
                    "operation": "<svc>.<ns>.svc.cluster.local:80/*"
                   },
                   "typed_per_filter_config": {
                    "mixer": {  // from mixerplugin.OnInboundRouteConfiguration
                     "@type": "type.googleapis.com/istio.mixer.v1.config.client.ServiceConfig",
                     "disable_check_calls": true,
                     "mixer_attributes": {
                      "attributes": {
                       "destination.service.host": {
                        "string_value": "<svc>.<ns>.svc.cluster.local"
                       },
                       "destination.service.name": {
                        "string_value": "<svc>"
                       },
                       "destination.service.namespace": {
                        "string_value": "<ns>"
                       },
                       "destination.service.uid": {
                        "string_value": "istio://<ns>/services/a"
                       }
                      }
                     }
                    }
                   },
                   "name": "default"
                  }
                 ]
                }
               ],
               "validate_clusters": false
              },
   ```

   

3. 把default route、vhosts等构建`RouteConfiguration`
4. 构建`InputParams`，调各个plugin的`OnInboundRouteConfiguration`

5. `envoyfilter.ApplyRouteConfigurationPatches`



### `buildInboundNetworkFilters`

1. build cluster name，一样的`<diretion:inbound>|<svcPort>|<svcPortName>|<hostname>`

2. 构建`TcpProxy`，`setAccessLogAndBuildTCPFilter`加入access log配置

3. `buildNetworkFiltersStack` 尝试生成实际的协议filter

   会根据`svcPort.Protocol`来dispatch，目前支持的：

   * `Mongo`：在前面插入一个`MongoProxy` filter，后面还是tcpproxy filter收尾

     看起来也啥都没看，可能和mysql一样，做了些业务功能无关的支持

   * `Redis`： 这个不一样，在`RedisProxy`做了路由，**属于terminating filter**，所以后面就不用加tcpproxy了

     虽然是catch all -> clusterName，但做了op级别的timeout设置，显然解析了redis协议，so...

   * `MySQL`： 根据`EnableMysqlFilter`决定是否在前面插入一个`MySQLProxy` filter，后面还是tcpproxy filter收尾

     > 但这个MySQLProxy看起来啥都没做，在业务功能上。 可能是一些周边功能支持，比如要做mysql的metric等，就需要解析协议，可能在这个filter里做了支持

     > 但k8s里的mysql服务信息，经常`protocol: TCP`，那就只能命中default了

   * default 啥都没，只有那个tcpproxy，也即"behave just as a simple tcp proxy"

   



### 根据ingress listener生成

> 来自 `Sidecar` crd -> `SidecarScope`







# outbound listener



# gateway listeners







# items



## listener protocol



`ModelProtocolToListenerProtocol`： `servicePort.Protocol` -> `ListenerProtocol`

逻辑是：

* HTTP,HTTP2,GRPC,GRPCWeb -> HTTP

* TCP,HTTPS,TLS,Mongo,Redis,MySQL -> TCP

  额外的，如果<u>当前trafficDirection上不支持protocolSniffing</u>的话，Unsupported -> TCP

* UDP -> Unknown

* default(Unsupported吧) -> auto

  如上面提到的，如果当前trafficDirection上支持protocolSniffing的话，会走到这里 -> auto



### auto

> `enables auto protocol detection`



对应的业务逻辑：

* 生成listener
  * inbound： 会把已有的



## ProtocolSniffing



大约理解一下，好像允许不指定协议



## 流量走向



### inbound



**remote发出的请求/流量**： `<srcIp:srcPort>(remote caller) -> <dstIp:dstPort>(local endpoint)`

local endpoint一般是 podIP+应用真实listen port

> 也有少数情况可以不使用标准istio方式，而是：
>
> * istio agent/proxy代为注册（在不同端口）
>
> * （可能是过渡并存阶段）应用同样也注册（在原端口）
>
> * 可以对原端口流量不拦截直接打到到应用（也可以拦截）
>
> * 拦截istio注册的端口的流量到istio proxy
>
>   此时local endpoint就是 podIP+代理发布port
>
> > 但这个确实复杂了点，不如agent做个判断，根据应用的能力（thin/fat 或者说 是否能感知mesh存在而不发布）来决定是否代为注册 --- 这个关联部署的方式比较好，能在镜像注入阶段而不用等待运行时确认应用能力

remote可以通过以下几种方式得到这个ep然后进行调用：

* dns解析
* k8s cluster ip(dispatch)
* 第三方服务发现机制（eureka、dubbo-zk等）



**流量进入provider机器（pod）后**：

* 不拦截：
  * 直接打到应用
  * 直接打到bind-to-port的istio proxy的listen端口（对应listener）
* 拦截
  * 打到bind-to-port的istio proxy的virtual inbound listener端口
    * 根据orig_dest来dispatch到对应的listener
      * ...
    * 走到virtual listener自己的（兜底）filter chains





### outbound

















