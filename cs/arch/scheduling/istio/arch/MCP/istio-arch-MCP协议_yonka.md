
ref: https://github.com/istio/api/tree/master/mcp

* MCP： mesh config protocol

* 基于XDS，会和XDS保持概念对齐

  这个 “基于” 不同于一般的。。。

* 网络交互与角色

  C/S vs `sink`/`source`

  > 常见的交互和角色关系式 client（主动发起连接）为sink（请求资源者），也即 `C(sink) -> S(source)`
  > 但，MCP也支持翻转的模式，也即 `C(source) -> S(sink)`，简单的说就是： （server：）谁连我我就向谁请求资源（囧

* 协议交互

  详见文档

  * 数据

    * 请求的是`Collecton`而不是特定resource（names）

    * 并没有看到`InitialResourceVersions`能用于表示请求的内容。 而是限于第一次请求是限定版本（？）

      ```go
	  // When the RequestResources is the first in a stream, the initial_resource_versions must
	  // be populated. Otherwise, initial_resource_versions must be omitted. The keys are the
	  // resources names of the MCP resources known to the MCP client. The values in the map
	  // are the associated resource level version info.
	  InitialResourceVersions map[string]string
      ```
      
      感觉作用是： 可以用incremental response来避免重复发送已有（且版本够新）的数据，**而不是request-on-demand/限定发送的数据**。 也即即使只发了三个资源的版本，有四个资源也还是会发满四个

  * 流程

    * sink -> source 都叫 `RequestResources` 即使即使ack/nack
      
      * 分普通请求、ack和nack

    * source -> sink， `Resources`

      * 分incremental 和 non-incremental/full

      * source可以根据自己是否支持来选择发送 全量 还是 差量，也即 sink请求里的incremental字段**仅供参考** （囧

* **问题**

  * 前面提到的，只有全量推送，没有按需获取、推送机制

  * 基于前一条，既然没有“描述需求”的协议交互内容，更没办法在运行时进行更新了

