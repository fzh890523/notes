

### 查看istio给某workload下发的配置： config_dump

* 1.3就支持，但`proxyID`参数要的是proxy id而此版本adsz返回的是con id
  这个proxy id是envoy的node id的`~`分割的第三个field
* 1.7（或者之前就）可以用`adsz`返回内容作为`proxyID`参数

