

https://istio.io/latest/docs/ops/diagnostic-tools/component-logging/

```sh
--log_output_level attributes:debug,cli:warn
```

似乎没办法一次性配置所有的scopes的日志级别，类似： `*:debug`

见： [Log levels for Istio are not easily configurable and missing documentation](https://github.com/istio/istio/issues/11847)

