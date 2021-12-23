

https://istio.io/latest/docs/ops/diagnostic-tools/component-logging/

```sh
--log_output_level attributes:debug,cli:warn
```

似乎没办法一次性配置所有的scopes的日志级别，类似： `*:debug`

见： [Log levels for Istio are not easily configurable and missing documentation](https://github.com/istio/istio/issues/11847)



 ```sh
 --log_stacktrace_level=default:debug
 ```

可以打出行号等，如：

```s
2021-12-10T09:21:02.184569Z     error   Could not decode object: unsupported type :&TypeMeta{Kind:Pod,APIVersion:v1,}
istio.io/pkg/log.emit
        /data/istio/src/istio.io/istio/vendor/istio.io/pkg/log/zapcore_handler.go:162
istio.io/pkg/log.(*Scope).emit
        /data/istio/src/istio.io/istio/vendor/istio.io/pkg/log/zapcore_handler.go:176
istio.io/pkg/log.Errorf
        /data/istio/src/istio.io/istio/vendor/istio.io/pkg/log/default.go:76
istio.io/istio/pkg/kube/inject.handleError
        /data/istio/src/istio.io/istio/pkg/kube/inject/webhook.go:970
istio.io/istio/pkg/kube/inject.(*Webhook).serveInject
        /data/istio/src/istio.io/istio/pkg/kube/inject/webhook.go:939
net/http.HandlerFunc.ServeHTTP
        /usr/local/go/src/net/http/server.go:2069
net/http.(*ServeMux).ServeHTTP
        /usr/local/go/src/net/http/server.go:2448
net/http.serverHandler.ServeHTTP
        /usr/local/go/src/net/http/server.go:2887
net/http.initALPNRequest.ServeHTTP
        /usr/local/go/src/net/http/server.go:3459
net/http.(*http2serverConn).runHandler
        /usr/local/go/src/net/http/h2_bundle.go:5723
```





