

## items

### `HTTP health checking filter`

envoy可以对在（主要是inbound，看起来都支持。但istio只用了inbound场景）listener上安装一个health check filter，来对hc相关的请求做对应的处理，提供几种策略支持：

* `No pass through` 根据server状态自行返回200/503等

* `No pass through, computed from upstream cluster health` 根据upstream cluster端点的（可设置）可用比例来返回200/503等

* `Pass through` 转给cluster upstream

* `Pass through with caching` 顾名思义...