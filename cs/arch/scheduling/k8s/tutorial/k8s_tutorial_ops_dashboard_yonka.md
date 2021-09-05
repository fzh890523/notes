

ref：

* https://kubernetes.io/docs/tasks/access-application-cluster/web-ui-dashboard/



## 安装

见链接。

```sh
kubectl apply -f https://raw.githubusercontent.com/kubernetes/dashboard/v2.0.0/aio/deploy/recommended.yaml
```





## 访问



* `kubectl proxy`

  > 访问限制：默认监听127，且有remote addr限制
  >
  > 见 《k8s_tutorial_kubectl_cmd_proxy.md》

* http://localhost:8001/api/v1/namespaces/kubernetes-dashboard/services/https:kubernetes-dashboard:/proxy/.



### 权限

允许的健全方式：

* token

  参考： https://github.com/kubernetes/dashboard/blob/master/docs/user/access-control/creating-sample-user.md

  ```sh
  apiVersion: v1
  kind: ServiceAccount
  metadata:
    name: admin-user
    namespace: kubernetes-dashboard
  ---
  apiVersion: rbac.authorization.k8s.io/v1
  kind: ClusterRoleBinding
  metadata:
    name: admin-user
  roleRef:
    apiGroup: rbac.authorization.k8s.io
    kind: ClusterRole
    name: cluster-admin
  subjects:
  - kind: ServiceAccount
    name: admin-user
    namespace: kubernetes-dashboard
  ---
  # 创建sa，授权ClusterRole。 然后把sa的token拿出来
  # kubectl -n kubernetes-dashboard get secret $(kubectl -n kubernetes-dashboard get sa/admin-user -o jsonpath="{.secrets[0].name}") -o go-template="{{.data.token | base64decode}}"
  ```

  

* config

  > 可能报错： [Not enough data to create auth info structure.](https://stackoverflow.com/questions/48228534/kubernetes-dashboard-access-using-config-file-not-enough-data-to-create-auth-inf)
  >
  > 链接中给的解法：
  >
  > ```sh
  > #!/bin/bash
  > TOKEN=$(kubectl -n kube-system describe secret default| awk '$1=="token:"{print $2}')
  > 
  > kubectl config set-credentials kubernetes-admin --token="${TOKEN}"
  > ```
  >
  > 好像还是不行，可能版本老了？



## 使用

* 主要是查看
* 创建、修改的支持比较弱，简单的粘贴、提交





