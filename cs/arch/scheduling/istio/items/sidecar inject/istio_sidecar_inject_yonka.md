

* 老的使用： ns级别 `istio-injection=enabled` + object（pod/dep）级别  `sidecar.istio.io/inject NotIn [false]`
* 新版本使用： `istio.io/rev In []` **并且**为了不和老版本冲突，加上了`istio-injection DoesNotExist`

