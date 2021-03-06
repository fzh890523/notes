

主要基于1.3代码，部分参考1.5代码（1.6开始就干掉了，晕）

* processing1的代码1.5里已经没了
* processing2的代码1.6里也没了。。。o(╯□╰)o



* source处理源头数据，转为内部事件
* processor里处理内部事件，对应的逻辑。 比如service、endpoint、pod事件都会触发serviceentry的重新build（之后可能触发推送给mcp client）等等





## 流程



### 启动

这里的管控指的是谁来启动各个组件、编排-组合各个组件的数据流

* `server.go::serverCmd.Run`

  * `server.New`

    有个组件管理的简单模型： `server.host.components` 来聚合各个组件。 以下会加到components里。

    * `NewProbe` 开启liveness、readiness

    * 根据参数`EnableValidation`等决定开启validation功能`NewValidation`

    * 根据参数`EnableServer`决定开启server功能

      根据`UseOldProcessor`决定开启哪个

      * `components.NewProcessing`

        用`snap.Cache`作为<u>distributor实现</u>来new `Processing` instance

      * `components.NewProcessing2`

        用`snap.Cache`作为<u>mcpCache</u>来new `Processing2` instance

    * `NewMonitoring`

    * 根据`EnableProfiling`决定开启`NewProfiling`

    * `NewCtrlz`

  * `s.Start` -> `s.host.Start` -> `foreach(s.host.components, Start)`

    * validation start -> `validation.RunValidation`, `validation.ReconcileWebhookConfiguration`

      具体业务逻辑还没看，好像是http server，对crd进行校验（？）

    * server start
      * processing start

        1. 加载meshConfig

        2. 用meshConfig来`createSource`，见 <u>source-processing1</u> 部分

        3. `runtime.NewProcessor`

           完成依赖注入（strategy、source、distributor等），详见processor部分

        4. new grpc server

        5. 根据`args.SinkAddress != ""` new callout

           > yonka: 这个好像是建连方向`s -> c`（请求方向还是`c -> s`）类型的mcp server，实际处理逻辑跟后面那个一样都是`Source.ProcessStream`。 看起来是个额外可选功能

        6. new mcp source

           > 这个source跟 file/kube/zk source概念不同。 这个source是sink、作为source的意思，而后者是 访问其他source 的意思

        6. listen到grpc server地址

        7. 把mcp resourceSourceServer注册到grpcServer

        8. 异步启动，等待启动完成

           * 启动processor，`p.grpcServer.Serve(l)`

             processor的启动就是启动source然后异步启动自己的处理任务

           * 启动callout

      * processing2 start

        1. 用`args.MeshConfigFile`来创建一个file source

           这里为mesh config resource专门写了一个file source实现，晕死，太重了

        2. `metadata.MustGet` 触发metadata的读取、解析

           > yonka: 这个metadata实际上是把部分逻辑配置化了，比如：
           >
           > * 哪些collection分配到哪个snapshot group，比如 synthetic se单独一个group
           >
           > * group对应的strategy是什么，比如 synthetic se为immediate，default为debounce
           >
           >   > 是因为sse上开增量所以不需要抗抖动吗？
           >
           > 等

        3. 初始化resource信息（该禁用的禁用），然后用它们来`createSource`

           见对应source部分描述。 2只支持kube source

        4. 用`snapshotter.NewMCPDistributor(p.mcpCache)`来作为distributor

        5. 用metadata数据来完成transform providers的初始化（`galley/pkg/config/processor/transforms/transforms.go::Providers`）

           这里加载了 `transforms/{serviceentry,ingress,direct,authpolicy}` 这几个类别/包下定义的transformer providers

        6. 用metadata、distributor和聚合的source（1 和 3中的）来初始化processor（`processorInitialize` -> `galley/pkg/config/processor/build.go::Initialize`），拿到runtime实例

           1. 根据metadata的snapshot配置，生成snapshotOptions，它会在ProcessorProvider里用于生成snapshotter

           2. 用ProcessorProvider、source来构建`RuntimeOptions`然后用来`NewRuntime`

              这里会通过`Source.Dispatch`来将source的事件回调设置为 Runtime.handle，其中会委托给 `r.session.handle`

        后面的步骤和processing1的new processor之后的步骤一样。 processor.Start替换为runtime.Start，详见processor部分

    

### 整体数据流



source -> processor -> distributor

* source：对接外部config，转为内部事件

  这一层在角色、功能上，1和2差别小，但实际上2只支持kube source了

* processor： 事件内容处理，对于一些类型的resource，需要做聚合等处理操作

  这一层是1和2的主要差别，1是裸写了两套处理流程，2则对两个流程共性进行抽象，允许自定义流程、资源类型的组合

* distributor

  这一层比较浅，基本上都是数据缓存+通知watcher

  1和2的接口有差异



### 功能实体

* mcp

  * sink `pkg/mcp/sink` 这个实际不在galley里启动

    类似的，也分为 clientMode sink和serverMode sink

  * source `pkg/mcp/source`

    * grpcServer-mcpServer-resourceServer/serverSource
    * grpcClient-clientSource

  * snapshot `pkg/mcp/snapshot`

    * Cache impl Watcher

* distribute

  * 1

    ```go
    // Distributor interface allows processor to distribute snapshots of configuration.
    type Distributor interface {
    	SetSnapshot(name string, snapshot sn.Snapshot)
    
    	ClearSnapshot(name string)
    }
    ```

    Cache impl 了这个

  * 2

    `galley/pkg/config/processing/snapshotter/distributor.go`

    ```go
    // Distributor interface abstracts the snapshotImpl distribution mechanism. Typically, this is implemented by the
    // MCP layer.
    type Distributor interface {
    	Distribute(name string, s *Snapshot)
    }
    
    // MCPDistributor distributes a snapshot to the MCP layer.
    type MCPDistributor struct {
    	mcpCache *snapshot.Cache
    }
    ```

    

* processor
  * 1
    * Processor
      * serviceEntryHandler
      * state、stateStrategy
      * handler
  * 2
    * Runtime
    * session
    * Snapshotter
      * Router(Handler)
      * accumulator(Handler)
      * snapshotGroups
      * xforms



## source



### source-processing1

#### source接口： `pkg/runtime/Source`

```go
// Source to be implemented by a source configuration provider.
type Source interface {
	// Start the source interface, provided the EventHandler. The initial state of the underlying
	// config store should be reflected as a series of Added events, followed by a FullSync event.
	Start(handler resource.EventHandler) error

	// Stop the source interface. Upon return from this method, the channel should not be accumulating any
	// more events.
	Stop()

```





#### 对source的选择 `Processing.createSource` 

可以看到支持多种source（已列出的全部）： `galley/pkg/source/{aggregate,eureka,fs,kube,zookeeper}`

* 根据是否支持聚合source来决定

  * 启用： 把多个source聚合成一个`aggregate.Source`

    根据配置，逐个创建source，支持的有：

    * zk

    * eureka

    * kube

      只能二选一，优先file

      * file
      * api-server

  * 不启用： 只能kube

    一样，二选一

    * file
    * api-server



### source-processing2



#### source接口： `pkg/config/event/Source`

功能基本和1一致，只是把 handler的传递抽到initialize阶段 （的做法更合适）



```go
// Dispatcher is an event source that can dispatch events to Handlers.
type Dispatcher interface {
	// Dispatch events to the given handler.
	Dispatch(handler Handler)
}

// Source is an event source for a single collection.
//
// - A Source can be started/stopped multiple times, idempotently.
// - Every time a Source is started, it is expected to send the full list of events, including a FullSync event for
// each collection.
// - It must halt its dispatch of events before the Stop() call returns. The callers will assume that
// once Stop() returns, none of the registered handlers will receive any new events from this source.
type Source interface {
	Dispatcher

	// Start sending events.
	Start()

	// Stop sending events.
	Stop()
}
```





#### 对source的选择 `Processing2.createSource` 

可以看到，只支持kube source（file、api-server）了，并且source的代码也不同： `galley/pkg/config/source`。

* `source/kube/fs` -> `source/kube/inmemory` -> `source/inmemory`

  fs这层剩下文件访问逻辑； 

  把yaml文件内容解析部分的逻辑移到`kube/inmemory`中便于复用（目前是sourceAnalyzer用了）

  > 这个抽出做的不太干净，inmemory里有太多file相关的东西

  解析完后的实际resource存在`source/inmemory`里

* `source/kube/apiserver`



### file source-1

#### 读取操作

* init
* SIGUSR1触发的reload



会读取配置目录下所有文件（walk方式）

#### 解析

按`\n---\n`分割读取所有内容，先全部读取、反序列化，再根据需要的资源类型进行过滤

> 这里允许（在`builtin.types.go`里）对特定类型自定义反序列化方式，对于这种优先使用。。。主要是反解为特定的对象（而不是通用对象），因为后面要用呢
>
> 目前这些主要还是serviceDiscovery相关的： Service、Node、Pod、Endpoints、Namespace



需要的资源类型（`Processing.createSource`里生成，传递给`fs.New`）： 

* 在全部的基础上
* 删掉exclude的，但是如果`EnableServiceDiscovery`则对应的类型不能删掉



最后解析出来的格式：

```go
// Entry is a single converted entry.
type Entry struct {
	Key      resource.FullName
	Metadata resource.Metadata
	Resource proto.Message
}

type fileResource struct {
	entry converter.Entry  // 实际内容
	spec  *schema.ResourceSpec
	sha   [sha1.Size]byte  // 对 yaml chunk bytes的sha1，用于后面判断是否变化
}
```



#### 对解析内容的处理

（`p.reload`里）会把内容和缓存内容做比较，确定要触发的事件类型。

> 如果是初始加载的话，会简化一些

然后`s.process`处理事件： 基本上就是做数据封装转换为processor层事件格式，然后`s.handler(event)`

而这个handler传自processor层，于是就到了...



### kube source-1



#### new/start



为每种（需要的）资源创建source，做了个优化，对于builtin（基本就是serviceDiscovery相关了）使用特化client，创建的是`kube.builtin.source`； 对于其他使用动态（泛化）的client，创建的是`kube.dynamic.source`。 

> 看了下，特化source，主要是包含了资源类型信息，这个“信息”里有提取、比较对象的逻辑，可以较为高效

两者都是多种资源（的source）共享一个client。

最后聚合成一个返回。

* builtin.source.new

  主要是记录了关注的资源类型

* dyn.source.new

  ... 通过`client.Resource(gv.WithResource(spec.Plural))`得到特定资源类型的resourceClient



start时，start了每个子source且传递了processor层给的handler。 不过异步做了个latchcountdown的操作： 

* 等待每个子source的第一个`FullSync`

  通过waitGroup实现

* 全部到达后触发一个`handler(resource.FullSyncEvent)`



* builtin.source.Start

  ...

* dyn.source.Start

  向kube库的informer注册了List、Watch的实际执行逻辑； 以及对事件的watch回调

  会异步`WaitForCacheSync`再`handler(resource.FullSyncEvent)`



#### 事件处理

* buildin

  除了调用`s.t`（资源类型信息）来做对象提取、比较以外，也只是简单的事件转换（为processor层event）

* dyn

* ...



### zk source-1

看了下，好像是直接生成了se而不是sse



### aggre source-1

* `galley/pkg/source/aggregate`下的“正牌”aggre

  代码十分简单，只是简单的透传Start和对应的handler。 没有做逻辑上的wrap（见下）

  > yonka: 这意味着，只要有一个source的fullSync到来，就会使得整个系统达到fullSync，**这应该是不符合预期的吧**

* `galley/pkg/source/kube`下的kube自用的aggre

  kube source实际上对每种schema/type建了个source实例，用aggre聚合成一个，对上层呈现为一个

  

  这个实现的相对完整，做了fullSync事件的“latchCountdown”，也即： 等待并拦截各子source的fullSync都到来之后才触发fullSync，其他类型的事件则透传 --- 这个逻辑封装为syncHandler作为handler传给子source们这个实现的相对完整，做了`fullSync`事件的“latchCountdown”，也即： 等待并拦截各子source的fullSync都到来之后才触发fullSync，其他类型的事件则透传 --- 这个逻辑封装为`syncHandler`作为handler传给子source们



### 事件的处理



## processor层-processing1(`galley/pkg/runtime/`)



### 前一层（source）到processor层的交互

从下面可知，是processor层告知source层事件处理方式为： 塞给`p.eventCh`

而source层会把本层处理的内容、事件转为processor层的event

### 事件处理流程



* processor.Start里启动`p.source`并且传入EventHandler回调为： `resource.Event`丢给`p.eventCh`

  

* processor.Start里启动的任务： for循环里从`p.eventCh`里读取消息然后`p.processEvent(e)`

* 对`FullSync`类型的事件做特殊处理，其他的`p.handler.Handle(e)`

* Handle内容见下面



### 对于事件的处理

`buildDispatcher` 先为所有资源添加默认的处理，然后为serviceDiscovery相关的资源添加特定的处理。



* 所有资源类型事件的默认处理： `p.state.Handle` （需要是galley已知的，其他的会丢弃）

  不关心资源，对事件类型（add、del、update）做简单的对应处理

  内容规整化，从`resource.Entry`到`mcp.Resource`

  更新版本号，这里是先全局版本号++，然后赋给当前资源类型，使得二者都在单调递增

  

  <u>通知下层部分</u>，做了一些流控，最后还是 buildSnapshot、setSnapShot to distributor。

  在snapshot那边，这部分数据用的是default group

* （如果enableServiceDiscovery）sd相关资源的处理： `serviceHandler.Handle`

  会根据资源类型dispatch，每种会有一些自己的逻辑：

  * `Endpoints` 要更新handler内部维护的两个映射表/缓存： name->entry; ip->names， 然后触发对应服务的更新

    这里的name似乎是service name

    <u>对服务的更新</u>（`doUpdate`）：用service和endpoints两个entry生成mcp resource（body部分是marshal的`ServiceEntry`），更新内部mcp resource缓存，然后（`updateVersion` -> `notifyChange`）更新版本号、通知下层（这部分与state同）

  * `Services` 更新内部的 name->Service 缓存，然后触发对对应服务的更新（同endpoint）

  * `Nodes` `pod.cacheImpl.Handle` -> `handleNode`

    从代码里看到，这里只关心node的locality（通过region、zone确定）变化，变化了则：  更新内部nodeName->locality的缓存； 更新这个node上的pods的locality，并且对更新成功（有变化）的pod触发下一步更新操作（`podUpdateds`）： 对pod id对应的服务（前面ip->names缓存）分别做`doUpdate`操作

  * `Pods`  `pod.cacheImpl.Handle` -> `handlePod`： 简单的说就是把pod状态变化翻译为增删， pending、running视为增其他视为删除； 增 == addOrUpdate，而update时比较内容变化； 最后执行的也是 `podUpdated`

  简而言之，除了各自的缓存更新、关联更新以外，最终触发的都是mcp resource更新

  


## processor层-processing2（`galley/pkg/config/{...}）



* `processing` 
* `processor`



### 几个概念

* `transformer` 完成input types到output types的转换，以及对output resource数据的缓存、中间处理、通知下游

* `(transformer) provider` 生成transformer的中间数据结构

* `selector`/`event.Router` 面向input type/collection resource event的处理器实体，类似bus等等，从名字可以看到，会根据类型来路由（到正确的处理器）

* `accumulator` 面向output type/collection resource event的处理器实体，实际上也是`Handler`实现类

  会被添加到每个transformer的末尾，将transformer的逻辑与后续逻辑串联起来

* `strategy` 控制snapshotter的publish的策略，目前支持 immediate， debounce

  拦截了一道，只控制状态通知不带数据（也即这个publish调度不带参数），所以实现简单一些

* `Snapshotter` 作为snapshot这一层的“总管”。 实现了`Processor`接口，作为这一层的代表

* `Runtime`/`session` 见下文，实现了processor层的状态可迁移



### vs processing1

整个看下来，主要是抽象了一套 transform 流程，允许 描述一些resource类型聚合/处理得到另一些resource ： 这在processing1里是hardcode两套流程代码的 - 对于需要合成的sse 和 其他透传的resource。



### 事件处理流程

* `Source` -> 

* `Runtime.handle` -> 

  new runtime把`r.handle`注册给source

* `session.handle` -> 

* `s.buffer.Handle` -> 

* 入`b.queue` -> 

* queue 的消费worker（`Buffer.Process`）调`b.handler.Handler`，这个`handler`实际就是`s.processor`也即调到了`Snapshotter.Handle` -> 

* `s.selector.Handle` ，这个selector实际是Router，根据type（collection）调对应的Handler（可能多个，按组合方式顺序调用。 实际是各个transformer）  -> 

  这里是in type了

* transformer处理完自己的逻辑后，调用对应的`accumulator.Handle` -> 

  这里是out type了

* 依次调用关联的snapshotGroups的`onSync` -> 

* `sg.strategy.OnChange` -> 

* 经过strategy内部逻辑消化后最后还是会调`Start(fn OnSnapshotFn)` 注册过去的回调，实际就是`Snapshotter.publish`，这里生成Snapshot，然后 ->

* `Distributor.Distribute(o.Group, sn)`，这里的Distributor是注入进来的，实现是 `MCPDistributor`，也即 ->

  其实没比cache多干啥，就是抽象了个`Distributor`接口，processing1里cache做distribute的事情，但直接外部调的`SetSnapshot`

* `d.mcpCache.SetSnapshot(name, s)`

  替换cache中snapshots内容

  找出`c.status`中对应的watch，把watch的版本和新snap的版本进行比较，决定是否触发推送 ->

  如果有推送的话，会删除原来的watch，估计协议约定依赖caller/client重新发起请求/watch

  > **其实这里（含）之后就跟processing1一样了**

* `watch.pushResponse(response)`

  这个`pushResponse source.PushResponseFunc`由`Cache.Watch`时注册，这个的调用层级是：

  -> `connection.processClientRequest` -> `Source.ProcessStream`



### processing部分



* `runtime.go` 作为processor层的实体，主要逻辑是管理session的创建、运行

  runtime将自己作为Handler注册给source，其处理逻辑其实委托给内部的session；

  `run`里for循环： 生成session id、创建session、启动session、等session结束后重新循环...

* `session.go` session的意义看起来主要是支持processor层的状态可迁移/可变

  大致理解了一下，可能是source层可能出现不可恢复的错误，这时候要求processor层能终止、重启流程，于是在这一层引入session来管理状态和状态迁移

  session会处理特殊控制消息（其实就是Reset消息了，在FullSync之外又定义的一个），根据消息指示完成状态迁移，干净的退出； 此外还处理来自`meshcfg.IstioMeshconfig`这个source的mesh event

* `transform/` 定义了`Provider`以及从provider到transformer、从providers到transformers的转换
* `snapshotter/`
  
  * `strategy`



### processor部分



* `transform/` 主要内容，包括各类providers、对应的transformer的定义
  * `serviceentry/`
  * `direct/`
  * `ingress/`
  * `authpolicy/`
  * `transform.go` 将各类providers汇总
* `metadata/` 元数据加载、初始化、提供
* `build.go` `Initialize`初始化runtime过程



## 资源、配置类型

* `metadata.Types`  从metadata.yaml生成的go，估计是个全量，包括`Service`等

* `args.ExcludedResourceKinds` 一些从启动参数传入的排序资源类型，不过后面知道enableServiceDiscovery的话，相关资源不能被排除（好像是先unreg再重新reg？）

  




## 其他问题





- [x] `--enableServiceDiscovery`的作用？ 目前看默认false，而启动参数又没有
  
  > 确实默认不启用
  
  * 影响`sourceSchema`： 如果开启的话，那builtin（也就是servicediscovery相关的，Service、Pod、Node等）类型不能被exclude --- 如果本来就没exclude那就无所谓了
  * 影响（等于）`SynthesizeServiceEntries`： 在build processor.handler时，把servicediscovery相关类型的handler指派为 `serviceEntryHandler`，该handler的handle主逻辑就是 buildSnapshot然后set给distributor（mcp层的cache），以此传播给mcp层，继而触发推送等

