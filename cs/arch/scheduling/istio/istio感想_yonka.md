





## thoughts





### istio and k8s



### 



## read



### 读 [大规模微服务架构下的Service Mesh探索之路](https://skyao.io/publication/service-mesh-explore/)



我们做技术选型的时候，有两大方向：

- 一个选择是在开源产品上做
- 另外一种思路全新打造



两条路线的终点，从图上看，我们有两个目标：

1. 第一个目标是内部落地
2. 第二个目标是技术输出



友商路线：

* 华为
  * golang
  * go chassis -> ...
  * 部分对接istio
* 微博
  * golang
* tencent
  * Envoy + istio定制扩展
  * 解耦k8s
* ucloud
  * envoy + pilot定制
  * pilot做服务发现，over ETCD，去掉其他adapters，可以脱离k8s
* 蚂蚁🐜
  * envoy -go-> 
  * istio -> 解决几个问题
    * 服务框架互通
    * k8s解耦
    * 性能
  * 最终
    * cp: pilot增强 + mixer + auth增强
    * dp: (new) golang sidecar(部分mixer下沉) in golang
  * 细节
    * golang sidecar -XDS API-pilot
    * Mixer的check/policy、quota -下沉> golang sidecar，report保留
    * pilot增强
      * 支持/打通多个服务注册中心，支持其之间的同步
      * 支持跨注册中心的服务调用
      * 支持异构（跨框架。。。）



对mixer的反思

* 解决mixer性能问题的方式mixer cache不可行

  笔者提到的笛卡尔积问题前面提了，不认同

* 隔离和抽象的层次

  * 抽象的成本，多走mixer中间层成本太高，考虑sidecard直连基础设施，在sidecar层面完成抽象和隔离
  * check和quota，即使直连基础设施，也嫌开销大

* report的集中式流量



### 读 [Mixer Cache: Istio的阿克琉斯之踵?](https://skyao.io/post/201804-istio-achilles-heel/)



#### “笛卡尔积”问题

![](https://skyao.io/post/201804-istio-achilles-heel/images/cache-account2.jpg)

笔者提到： mixer cache在mixer侧和在envoy侧，数据量是 `笛卡尔积 vs 和`。 

对于这点不认同，或者说也许是实现上的误区。

* 规则只有涉及的内容，并不是涉及的属性的笛卡尔积全集
* 规则的cache在mixer还是envoy并不影响规则本身的量级













