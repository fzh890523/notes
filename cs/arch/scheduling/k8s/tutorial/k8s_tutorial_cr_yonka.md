

### cr无法删除的问题

大多都是hang在fin阶段。

```sh
kubectl patch <crd>/<crname> -p '{"metadata":{"finalizers":[]}}' --type=merge
```



