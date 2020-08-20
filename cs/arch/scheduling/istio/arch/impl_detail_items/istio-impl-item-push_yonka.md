
## 各版本演进

### 1.1 -> 1.4

`startPush`差别：
差别比较大
* 老版本直接遍历conn来等待每个conn（前一个push完）接受这个push（带超时控制）
      这里有个问题是此时conn上的push才“刚开始”，当然这只是次要。 真正的问题是，这里会积压任务。 越慢越积压，要计算的频次蹭蹭的上去
* 新版本通过queue（merge） + semaphore + doneCallback的方式来实现并发控制和较为精确的“push完感知”

`pushConnection`差别：
差别主要在对 edsPush 的处理上： 
* 新版本允许根据scope信息（namespacesUpdated、configTypesUpdated）来判断该proxy是否需要pushEds
  只对 edsPush。 fullPush不生效。 = = 这个也是有问题的，按需加载咋生效？ -> 还是沿用老的吧。



## push的“丢失”怎么应对

如上，

* 老版本（1.1）里 - **可能丢失**

  * 最后都是通过`client.PushChannel`来做conn级别的限流，保证串行发送

  * 对于push任务的处理
  
    * 通过逐个goroutine来触发每个conn上的push任务

    * 每个conn上的push任务会带超时和重试，报错 或 总超时时间耗尽 后才失败

    * 所有conn的push任务都结束（成功或失败）才会结束当前push任务

    可以看到，问题：

    * push任务本身的限流机制有限，导致可能造成不必要的排队+重复推送
      
      设想，如果push任务连续到来，而client上推送速度有限，那么该client上的推送会排队：
      
      * 如果特别卡后面会超时失败

        看了下，这个分支没有错误处理。 也即只能等conn坏掉了，**而如果又没坏的话，就gg了，push丢失了**

        > 为了尽量不丢，重试操作本身加剧了client上可能的阻塞等待时间

      * 否则，后面的push会跟上推送，推送是推送完了，但**可能重复推送**
      
        比如连续的full-push； 同一个cluster上的连续EDS变化 等；
        如果是不同cluster上的连续EDS，本来也无法merge减少，所以倒差不多；
  

* 新版本（1.4）里 - **不会丢失**

  currentRequest/inProgressRequest + merge pushRequest + timeout-reconn

  * 最后都是通过`client.PushChannel`来做conn级别的限流，保证串行发送

  * 通过一个`支持merge PushRequest`的queue来做缓冲，同时还支持`inProgress PushRequest`

    意味着对于某个client可能有三种状态（来PushRequest时对应的处理`->`）：

    * 没有pushRequest

      -> add

    * queue中有pushRequest(inProgress)

      -> merge with inProgress PushRequest

      * 没有在处理/推送（也即dequeued）的PushRequest

      * 有在推送的PushRequest

    这种设计，可以保证不丢PushRequest，所以就不需要老版本的重试了

  * 对client的推送的控制： 通过semaphore来控制并发度

    每acquire成功，则dequeue一个`<client, pushRequest>`item，然后开goroutine来触发对client的push（`XdsEvent -> client.pushChannel`）

    > 这里用固定goroutine+chan应该更好


## push “范围” 的确定

主要是指push只推给“影响到”的proxies。

先不考虑额外实现的隔离机制：

* EDS： 触发不隔离，但最后执行pushEds的时候会做 `updateEds ∪ conn.clusters`，所以虽然有少许开销，但不做实际计算、推送的话，还好

  触发能隔离就更好了。 `UpdatedNamespaces` 可以改善这个。

* full-push

  目前入口主要是：

  * `ClearCache`: `s.ConfigUpdate(&model.PushRequest{Full: true}`

    这个一个限定都没有。。。 最彻底的。
    主逻辑里基本没有用到。 （老版本是直接调这个方法）

  * `ConfigUpdate`

    * service change

    * instance change

      不该。。。 看注释是有一些code path里instance change需要full-push，如果不希望的，自己去调 `XDSUpdater.EdsUpdate` 吧    

    * config change

      这块主要是哪些需要引起full-push不好界定，所以都。。
      估计后期精细化处理
  
然后，考虑隔离方式：

* `PushRequest.ConfigTypesUpdated`

  这个不好界定，好像目前比较少用这个来选择push范围

* `PushRequest.NamespacesUpdated`

  目前的主要方式。

  `ProxyNeedsPush` 里会考虑 `pushEv.namespacesUpdated`，如果有，并且该proxy的`SidecarScope`.`namespaceDependencies` 有这个ns的话，则需要push

  `edsUpdate`参数namespace等 -> `PushRequest.NamespacesUpdated` -> `XdsRequest.namespacesUpdated`

  > PushRequest在merge时，会对`NamespacesUpdated`进行merge

  但只适合强隔离语义（namespace的本意： tenant）。

