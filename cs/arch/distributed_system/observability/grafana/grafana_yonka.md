



## 存储



```yaml
# 以istio grafana为例

    - mountPath: /var/lib/grafana
      name: storage
    - mountPath: /etc/grafana/provisioning/datasources/datasources.yaml
      name: config
      subPath: datasources.yaml
    - mountPath: /etc/grafana/provisioning/dashboards/dashboardproviders.yaml
      name: config
      subPath: dashboardproviders.yaml
    - mountPath: /etc/grafana/grafana.ini
      name: config
      subPath: grafana.ini

  - emptyDir: {}
    name: storage
  - configMap:
      defaultMode: 420
      name: grafana
    name: config   
    
# configmap grafana
apiVersion: v1
data:
  dashboardproviders.yaml: |
    # ...
```

> <del>如果有效配置都存cm的话，那算是持久化了</del> 然而并不是，新的数据... 写到这个目录下，就... gg
>
> /var/lib/grafana下不知道是不是都临时数据
>
> > 是。 所以...



