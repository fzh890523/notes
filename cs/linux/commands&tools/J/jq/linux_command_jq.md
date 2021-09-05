

https://stedolan.github.io/jq/manual/



# 格式转换



## 转为yaml

https://stackoverflow.com/questions/53315791/how-to-convert-a-json-response-into-yaml-in-bash



# 数据类型

## 特殊处理

### 空/null处理

* `try E` is equivalent to `try E catch empty`
* `try E` is equivalent to `(E)?`

## 数组

* `arr[]` 展开
  
  ```sh
  jq '.a[] as $i | $i+1 ' <<< '{"a": [1,2,3,null]}'
  2
  3
  4
  1  # null as 0 -> +1 = 1
  jq '.b[] as $i | $i+1 ' <<< '{"a": [1,2,3,null]}'
  jq: error (at <stdin>:1): Cannot iterate over null (null)
  jq '.b[]? as $i | $i+1 ' <<< '{"a": [1,2,3,null]}'  # empty output
  ```
  
  
  
* `[ <expr> ]` 收集

* `[ arr[] | select(<expr>) ]` 过滤筛选数组元素然后收集为数组

* **slice**： `.configs[0:2]`

* **length**： `.configs[0:2] | length`

  > arr如果不存在/null， `.items | length` 可能报错，可以：
  >
  > * `.items | .[]? | length`
  > * `try .items[]` 等于 `(.items[])?` 不够不等于 `.items[]?`



# interactive jq



## [jiq](https://github.com/fiatjaf/jiq)



```s
[jq]> .configs[] | ."@type"
                                                                                       "type.googleapis.com/envoy.admin.v3.BootstrapConfigDump"
"type.googleapis.com/envoy.admin.v3.ClustersConfigDump"
"type.googleapis.com/envoy.admin.v3.DubboRoutesConfigDump"
"type.googleapis.com/envoy.admin.v3.ListenersConfigDump"
"type.googleapis.com/envoy.admin.v3.ScopedRoutesConfigDump"
"type.googleapis.com/envoy.admin.v3.RoutesConfigDump"
"type.googleapis.com/envoy.admin.v3.SecretsConfigDump"
"type.googleapis.com/envoy.admin.v3.ThriftRoutesConfigDump"


[jq]> .configs[0].bootstrap.
 admin dynamic_	resources layered_runtime node static_resources stats_config tracing 
 # 只是访问的话可
```



有个很重要的功能没有： enter/dive-into

比如以上，如果支持箭头上下，选中-enter进去 以 选中条目为条件搜索的结果



```s
[jq]> [ .configs[] | ."@type" ][0]
```



