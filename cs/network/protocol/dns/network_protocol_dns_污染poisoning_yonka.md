
ref:

* [dns poisoning and countering](https://www.yichya.dev/dns-poisoning-and-countering/)


* wall内dns

  ISP dns server直接返回错误/无效结果 - 正常dns流程

  * ISP内部

  * ISP没有的条目，向上流（直至国外dns）查询时，触发下面的流程

  > wall内除了ISP dns，还有一些公共dns、私有dns、小dns 等等

* wall外dns

  wall抢答返回错误/无效结果 - 异常dns流程

> 实际好像后者不多，前者多一些。 所以大多数情况下，改用wall外dns可以缓解，代价就是有点慢

> 还有就是使用wall内“干净”的（一般是小的）dns，不过可能不稳定

