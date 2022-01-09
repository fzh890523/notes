

* https://kubernetes.io/docs/reference/access-authn-authz/extensible-admission-controllers/

  



## 作用范围





* namespaceSelector
* objectSelector

> 基于label的筛选



从istio的用法来看，这两个selector似乎是同时生效（`&&`关系）：

```yaml
  namespaceSelector:
    matchExpressions:
    - key: istio.io/rev
      operator: In
      values:
      - 1-11-4
    - key: istio-injection
      operator: DoesNotExist
  objectSelector:
    matchExpressions:
    - key: sidecar.istio.io/inject
      operator: NotIn
      values:
      - "false"
```

并且似乎`matchExpressions`的数组也是与关系





