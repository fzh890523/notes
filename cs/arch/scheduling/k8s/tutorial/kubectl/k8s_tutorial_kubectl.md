

ref：

* [kubectl-cheatsheet](https://kubernetes.io/docs/reference/kubectl/cheatsheet/)

* [kubectl commands](https://kubernetes.io/docs/reference/generated/kubectl/kubectl-commands)

* [kubectl conventions](https://kubernetes.io/docs/reference/kubectl/conventions/)

* [kubectl overview](https://kubernetes.io/docs/reference/kubectl/overview/)



# 子命令



## "info"

* `kubectl version` 输出client/server的版本信息



## config



```sh
kubectl config --help
Modify kubeconfig files using subcommands like "kubectl config set current-context my-context"

 The loading order follows these rules:

  1.  If the --kubeconfig flag is set, then only that file is loaded. The flag may only be set once
and no merging takes place.
  2.  If $KUBECONFIG environment variable is set, then it is used as a list of paths (normal path
delimiting rules for your system). These paths are merged. When a value is modified, it is modified
in the file that defines the stanza. When a value is created, it is created in the first file that
exists. If no files in the chain exist, then it creates the last file in the list.
  3.  Otherwise, ${HOME}/.kube/config is used and no merging takes place.

Available Commands:
  current-context Displays the current-context
  delete-cluster  Delete the specified cluster from the kubeconfig
  delete-context  Delete the specified context from the kubeconfig
  delete-user     Delete the specified user from the kubeconfig
  get-clusters    Display clusters defined in the kubeconfig
  get-contexts    Describe one or many contexts
  get-users       Display users defined in the kubeconfig
  rename-context  Renames a context from the kubeconfig file.
  set             Sets an individual value in a kubeconfig file
  set-cluster     Sets a cluster entry in kubeconfig
  set-context     Sets a context entry in kubeconfig
  set-credentials Sets a user entry in kubeconfig
  unset           Unsets an individual value in a kubeconfig file
  use-context     Sets the current-context in a kubeconfig file
  view            Display merged kubeconfig settings or a specified kubeconfig file

Usage:
  kubectl config SUBCOMMAND [options]

Use "kubectl <command> --help" for more information about a given command.
Use "kubectl options" for a list of global command-line options (applies to all commands).
```



```sh
kubectl config view # Show Merged kubeconfig settings.

# use multiple kubeconfig files at the same time and view merged config
KUBECONFIG=~/.kube/config:~/.kube/kubconfig2 

kubectl config view

# get the password for the e2e user
kubectl config view -o jsonpath='{.users[?(@.name == "e2e")].user.password}'

kubectl config view -o jsonpath='{.users[].name}'    # display the first user
kubectl config view -o jsonpath='{.users[*].name}'   # get a list of users
kubectl config get-contexts                          # display list of contexts 
kubectl config current-context                       # display the current-context
kubectl config use-context my-cluster-name           # set the default context to my-cluster-name

# add a new user to your kubeconf that supports basic auth
kubectl config set-credentials kubeuser/foo.kubernetes.com --username=kubeuser --password=kubepassword

# permanently save the namespace for all subsequent kubectl commands in that context.
kubectl config set-context --current --namespace=ggckad-s2

# set a context utilizing a specific username and namespace.
kubectl config set-context gce --user=cluster-admin --namespace=foo \
  && kubectl config use-context gce

kubectl config unset users.foo                       # delete user foo
```





### `set-context`

`kubectl config set-context <ctx> <设置内容>`

`kubectl config set-context [NAME | --current] [--cluster=cluster_nickname] [--user=user_nickname]
[--namespace=namespace] [options]`

* `<ctx>` 可以是具体某个context name，也可以是`--current`表示设置当前context的内容
* `<设置内容>`可以是：
  * `--user=<user>` 设置用户
  * `--cluster=<cluster>` 设置context指向的cluster
  * `--namespace=<ns>` 设置该context的当前ns 



```sh
kubectl config set-context --help
Sets a context entry in kubeconfig

 Specifying a name that already exists will merge new fields on top of existing values for those
fields.

Examples:
  # Set the user field on the gce context entry without touching other values
  kubectl config set-context gce --user=cluster-admin

Options:
      --current=false: Modify the current context

Usage:
  kubectl config set-context [NAME | --current] [--cluster=cluster_nickname] [--user=user_nickname]
[--namespace=namespace] [options]

Use "kubectl options" for a list of global command-line options (applies to all commands).
```





### 切换（当前）context

`kubectl config set-context <ctx>`



## logs

` kubectl logs [-f] [-p] (POD | TYPE/NAME) [-c CONTAINER] [options]`



* `--all-containers=false` ： 输出所有容器的，默认只输出一个

  > yonka: 哪个？ 好像是第一个

* `tail -f` 效果： `--follow`/`-f`

* `tail -b`效果： `--tail=10`
* `--limit-bytes=0`： 限制输出的长度，默认0表示无限制

* 额外打印时间戳： `--timestamps=false`

* 输出podname等：`--prefix`
* 筛出需要的容器的输出： `-l`/`--selector`，如： `-l app=example`

* 筛出指定容器的输出： `-c`/`--container`

* 也输出container之前的实例的日志内容： `-p`/`--previous`

* `--since=0s`： 限制输出的时间（多久之内的）

  格式可以是 `5s, 2m, or 3h` 这种

* `--since-time=`： 输出指定时刻之后的

  `RFC3339`格式



## apply

* 目录：
  *  `-f <folder>`
  * `-k <folder>` (parameterized manifest files )
  * `--recursive`/`-R` 则递归生效i



## 网络



### port-forward

> 注意和`kubectl proxy`区分：
>
> * proxy 是提供一个可以以http访问访问的 api-server 的代理
> * pf 则是通用的暴露内部服务到集群外的方式，只做4层转发



在执行机器本地起一个proxy，做端口转发： req -> localProxy -> remote(pod-port)

* `kubectl port-forward redis-master-765d459796-258hz 7000:6379`

  端口格式为： `{localPort}:{targetPort}` ，下同

  > localPort不指定的话，会自行选择一个可用端口（在output里打印出来）

  等于 `kubectl port-forward pods/redis-master-765d459796-258hz 7000:6379`

  ```sh
  Forwarding from 127.0.0.1:7000 -> 6379
  Forwarding from [::1]:7000 -> 6379
  ```

* `kubectl port-forward deployment/redis-master 7000:6379`
* `kubectl port-forward replicaset/redis-master 7000:6379`
* `kubectl port-forward service/redis-master 7000:redis`







> 依赖socat，可能报错：`uid : unable to do port forwarding: socat not found.`，那么就要在操作机上：
>
> `apt-get -y install socat`



## 读取资源



### get



```sh
Examples:
  # List all pods in ps output format.
  kubectl get pods

  # List all pods in ps output format with more information (such as node name).
  kubectl get pods -o wide

  # List a single replication controller with specified NAME in ps output format.
  kubectl get replicationcontroller web

  # List deployments in JSON output format, in the "v1" version of the "apps" API group:
  kubectl get deployments.v1.apps -o json

  # List a single pod in JSON output format.
  kubectl get -o json pod web-pod-13je7

  # List a pod identified by type and name specified in "pod.yaml" in JSON output format.
  kubectl get -f pod.yaml -o json

  # Return only the phase value of the specified pod.
  kubectl get -o template pod/web-pod-13je7 --template={{.status.phase}}

  # List all replication controllers and services together in ps output format.
  kubectl get rc,services

  # List one or more resources by their type and names.
  kubectl get rc/web service/frontend pods/web-pod-13je7

Options:
      --all-namespaces=false: If present, list the requested object(s) across all namespaces. Namespace in current context is ignored even if specified with --namespace.
      --allow-missing-template-keys=true: If true, ignore any errors in templates when a field or map key is missing in the template. Only applies to golang and jsonpath output formats.
      --chunk-size=500: Return large lists in chunks rather than all at once. Pass 0 to disable. This flag is beta and may change in the future.
      --export=false: If true, use 'export' for the resources.  Exported resources are stripped of cluster-specific information.
      --field-selector='': Selector (field query) to filter on, supports '=', '==', and '!='.(e.g. --field-selector key1=value1,key2=value2). The server only supports a limited number of field queries per type.
  -f, --filename=[]: Filename, directory, or URL to files identifying the resource to get from a server.
      --ignore-not-found=false: If the requested object does not exist the command will return exit code 0.
      --include-uninitialized=false: If true, the kubectl command applies to uninitialized objects. If explicitly set to false, this flag overrides other flags that make the kubectl commands apply to uninitialized objects, e.g., "--all". Objects with empty metadata.initializers are regarded as initialized.
  -L, --label-columns=[]: Accepts a comma separated list of labels that are going to be presented as columns. Names are case-sensitive. You can also use multiple flag options like -L label1 -L label2...
      --no-headers=false: When using the default or custom-column output format, don't print headers (default print headers).
  -o, --output='': Output format. One of: json|yaml|wide|name|custom-columns=...|custom-columns-file=...|go-template=...|go-template-file=...|jsonpath=...|jsonpath-file=... See custom columns [http://kubernetes.io/docs/user-guide/kubectl-overview/#custom-columns], golang template [http://golang.org/pkg/text/template/#pkg-overview] and jsonpath template [http://kubernetes.io/docs/user-guide/jsonpath].
      --raw='': Raw URI to request from the server.  Uses the transport specified by the kubeconfig file.
  -R, --recursive=false: Process the directory used in -f, --filename recursively. Useful when you want to manage related manifests organized within the same directory.
  -l, --selector='': Selector (label query) to filter on, supports '=', '==', and '!='.(e.g. -l key1=value1,key2=value2)
      --server-print=true: If true, have the server return the appropriate table output. Supports extension APIs and CRDs.
      --show-kind=false: If present, list the resource type for the requested object(s).
      --show-labels=false: When printing, show all labels as the last column (default hide labels column)
      --sort-by='': If non-empty, sort list types using this field specification.  The field specification is expressed as a JSONPath expression (e.g. '{.metadata.name}'). The field in the API resource specified by this JSONPath expression must be an integer or a string.
      --template='': Template string or path to template file to use when -o=go-template, -o=go-template-file. The template format is golang templates [http://golang.org/pkg/text/template/#pkg-overview].
  -w, --watch=false: After listing/getting the requested object, watch for changes. Uninitialized objects are excluded if no object name is provided.
      --watch-only=false: Watch for changes to the requested object(s), without listing/getting first.

Usage:
  kubectl get [(-o|--output=)json|yaml|wide|custom-columns=...|custom-columns-file=...|go-template=...|go-template-file=...|jsonpath=...|jsonpath-file=...] (TYPE[.VERSION][.GROUP] [NAME | -l label] | TYPE[.VERSION][.GROUP]/NAME ...) [flags] [options]

Use "kubectl options" for a list of global command-line options (applies to all commands).
```





#### output（格式）

* `-o yaml`，如： `kubectl get pods -oyaml`

  会wrap成list，如：

  ```yaml
  apiVersion: v1
  items:
  - 
  -
  ```

  可以通过简单的shell命令转为分隔符分割的多个item：

  ```sh
  cat xx.yaml | egrep -v "^[^ -]" | sed -e 's/^- /---\n/g' -e 's/^  //g' | k apply -n istio-test1214 -f -
  ```

  

* `-o json`

  **复杂的内容筛选、展示可以 -o json然后用jq来做**

  ```sh
  kubectl get pods -a --all-namespaces -o json  | jq -r '.items[] | select(.status.phase != "Running" or ([ .status.conditions[] | select(.type == "Ready" and .status == "False") ] | length ) == 1 ) | .metadata.namespace + "/" + .metadata.name'
  # 详见 linux_command_jq.md
  ```

  

* `-o jsonpath=<jsonpath_expression>`

  如： `kubectl get svc -o jsonpath='{.items[0].metadata.name}'`

  > jsonpath模板使用一对花括号(`{}`)把jsonpath表达式包含在里面(`go-template`是双花括号).除了标准jsonpath语法外,kubernetes jsonpath模板还额外支持以下语法:
  >
  > - 用""双引号来引用JSONPath表达式中的文本
  > - 使用**range**和**end**来遍历集合(这点和go-template类似)
  > - 使用负数来从尾部索引集合
  >
  > 详见： https://kubernetes.io/docs/reference/kubectl/jsonpath/ 和 jsonpath_yonka.md

  如：

  ```sh
  kubectl get pods -o json
  kubectl get pods -o=jsonpath='{@}'
  kubectl get pods -o=jsonpath='{.items[0]}'
  kubectl get pods -o=jsonpath='{.items[0].metadata.name}'
  kubectl get pods -o=jsonpath='{range .items[*]}{.metadata.name}{"\t"}{.status.startTime}{"\n"}{end}'
  kubectl get pods -o=jsonpath='{range .items[*]}{.metadata.name}{" "}{.status.podIP}{"\n"}{end}'
  
  kubectl -n istio-system get secret istio-ca-secret -ojsonpath={.data.ca-cert\.pem}
  # -ojsonpath="{.data['ca-cert.pem']}" 这样拿不到
  # -ojsonpath='{.data["ca-cert.pem"]}' 这样还报错，晕
  # -ojsonpath={.data['ca-cert\.pem']} 这个也可以
  # kubectl -n istio-system get secret cacerts "-ojsonpath={.data['root-cert\.pem']}"
  ```
```
  
  低版本好像不支持range
  
  
  
  




#### 筛选条目

* 按label（selector）筛选： `-l`/`--selector`

  支持 `=`/`!=`/`==`，可以逗号分隔指定多个同时满足的条件。如： `-l key1=val2,key2=val2`

  > Kubernetes is using 2 types of labels selectors - Set-based and Equality-based.
  >
  > Because the Service object is old and was created before Set-based requirements were released, you cannot use rules like if label 'myapp' exists, you should use equals like if label 'myapp' = 'abc'.
  >
  > * `-l mypod`
  > * `-l mypod=xx`

* 按字段（内容）进行筛选： `--field-selector`

  每种类型有对应支持的几种筛选条件。 使用方式类似`-l`

  如： 

  ```sh
  kubectl get pods --field-selector status.phase=Running
  kubectl get pods --field-selector=status.phase!=Running,spec.restartPolicy=Always
  kubectl get statefulsets,services --all-namespaces --field-selector metadata.namespace!=default
  
  k get pods --all-namespace --field-selector spec.nodeName=ubuntu-20041-3
```

* 获取所有ns数据（默认为当前ns）： `--all-namespaces`



#### 显示字段

* 显示所有labels： `--show-labels`

* 显示指定labels： `-L`/`--label-columns=[]`

  逗号分隔多个； 也可以重复指定。 如： `-L label1 -L label2`

* `kubectl get pods --no-headers -o custom-columns=":metadata.name"`
* `kubectl get --no-headers=true pods -o name | awk -F "/" '{print $2}'`



#### 单复数差别

比如`Service`是单数，`Endpoints`本身是单数形式的复数内容。

* `k get svc <svc` 是获取单个svc，`-oyaml`时为单个对象
* `k get svc` 是获取（当前ns）所有svcs， `-oyaml`时为`List`对象

* `k get ep <svc>`/`k get endpoints <svc>` 是获取单个svc的eps，`-oyaml`时为单个对象

* `k get ep` 是获取（当前ns）所有svcs的eps， `-oyaml`时为`List`对象



## 删除资源



### delete



#### tools



##### 批量删除`Evicted` pods

* 指定ns的

  `kubectl -n <ns> get pod | grep Evicted | awk '{print $2}' | xargs kubectl delete -n <ns>`

* 全部ns的

  `kubectl get pod --all-namespaces | grep Evicted | awk '{printf("kubectl delete -n %s pod %s\n", $1, $2)}' | xargs -I'{}' bash -c '{}'`





## label

### 读取

* describe里能看到
* get `-L`能输出

### 修改

`k labels <type> <name> label1=value1 labels2=value2`

* `--overwrite` 覆盖已有（否则不允许覆盖）
* 

### 删除： `<label>-`

末尾带上减号来表示移除指定label，如： `k label ns testns testlabel-`



## 文件



### cp



* ns的指定：

  * 参数指定： `-n <ns>`
  * 路径里给出： `<ns>/<pod>:<pod_path>`

* container的指定： `-c <container>`

  否则会选择第一个

* 本地dest path不能是dir，也即需要是 `k cp -c xxcontainer xxpod:/var/xxfile /tmp/xxfile` 而不能是 `/tmp/`



```sh
Copy files and directories to and from containers.

Examples:
  # !!!Important Note!!!
  # Requires that the 'tar' binary is present in your container
  # image.  If 'tar' is not present, 'kubectl cp' will fail.

  # Copy /tmp/foo_dir local directory to /tmp/bar_dir in a remote pod in the default namespace
  kubectl cp /tmp/foo_dir <some-pod>:/tmp/bar_dir

  # Copy /tmp/foo local file to /tmp/bar in a remote pod in a specific container
  kubectl cp /tmp/foo <some-pod>:/tmp/bar -c <specific-container>

  # Copy /tmp/foo local file to /tmp/bar in a remote pod in namespace <some-namespace>
  kubectl cp /tmp/foo <some-namespace>/<some-pod>:/tmp/bar

  # Copy /tmp/foo from a remote pod to /tmp/bar locally
  kubectl cp <some-namespace>/<some-pod>:/tmp/foo /tmp/bar

Options:
  -c, --container='': Container name. If omitted, the first container in the pod will be chosen
      --no-preserve=false: The copied file/directory's ownership and permissions will not be preserved in the container

Usage:
  kubectl cp <file-spec-src> <file-spec-dest> [options]

Use "kubectl options" for a list of global command-line options (applies to all commands).
```



## 执行



### run

https://kubernetes.io/docs/reference/generated/kubectl/kubectl-commands#run

https://jamesdefabia.github.io/docs/user-guide/kubectl/kubectl_run/ 有更多样例

* 运行指定镜像
* 通过创建 deployment或者job



```sh
kubectl run NAME --image=image [--env="key=value"] [--port=port] [--replicas=replicas] [--dry-run=bool] [--overrides=inline-json] [--command] -- [COMMAND] [args...]
```



* `docker run test --image=busybox -- /bin/sh`



### exec



#### 进入pod

* `k exec -it <pod> -- bash`

* 以root用户进入pod

  * docker方式

    ```sh
    kubectl describe <pod>  # 查看node和container id 如 docker://<id>
    ssh <node>
    docker exec -it -u root <container_id> /bin/bash
    
    # 如：
    docker exec -it --privileged -u root e819cc6f51cde029620
    6e3ceb754ed1b1ba28d27c526aafe281b993ae5ca7bf6 /bin/bash
    ```

  * 其他工具

    * https://github.com/jordanwilson230/kubectl-plugins： `kubectl ssh -u root -p nginx-0`

      ```sh
      # https://krew.sigs.k8s.io/docs/user-guide/setup/install/
      # 安装krew
      
      krew install exec-as
      kubectl exec-as ...
      ```

      

    * 

      



# 资源操作 per type



## 列出所有资源

* crd： `get crd`

* 所有（包含native）： `api-resources`

  > 不支持也不需要 --all-namespaces

  ```sh
  kubectl api-resources -o name
  
  bindings
  componentstatuses
  configmaps
  ...
  ```

  ```sh
  # 保存各个资源的配置
  kubectl api-resources -o name | awk '{print $1}' | xargs -I '{}' bash -c 'kubectl get {} --all-namespaces -oyaml > /tmp/k8s-all-resources/{}.yaml'
  
  # 去掉不必要的太大的
  exc="secrets packagemanifests.packages.operators.coreos.com"
  cur_env=xx
  cur_dir="/tmp/k8s-all-resources_${cur_env}"
  mkdir -p "${cur_dir}"
  kubectl api-resources  -o name | awk '{print $1}' | xargs -I '{}' bash -c 'exc="'"$exc"'"; for item in $exc; do if [[ "{}" = "$exc" ]]; then exit; fi; done; kubectl get {} --all-namespaces -oyaml > '"${cur_dir}"'/{}.yaml'
  ```

  



## crd



## pod



### 通过条件筛选pod

* 通过podIP： `k get pod --all-namespaces --field-selector='status.podIP=<ip>'`



### 获取node上的pods



```sh
kubectl get pods --all-namespaces -o wide --field-selector spec.nodeName=<node>
```



# config



## merge config

比如要同时管理多个集群的话会有多个`config`，从数据结构上可以merge为一个（多个context管理不同的cluster）。操作如下：

```sh
scp <user>@<clusterA_ip>:~/.kube/config /tmp/kube_config_<clusterA>

scp <user>@<clusterB_ip>:~/.kube/config /tmp/kube_config_<clusterB>

KUBECONFIG=/tmp/kube_config_<clusterA>:/tmp/kube_config_<clusterB> kubectl config view --flatten >> /tmp/kube_config_merged

copy /tmp/kube_config_merged ~/.kube/config
```

> 实际做的是merge，所以对于标准配置，需要自行改一下里面各种名字，比如集群名、用户名等。 做了区分之后才不会merge覆盖



# logs

* 查看kubelet日志： `journalctl -u kubelet`

* 设置日志级别

  有多种方式，比如可以通过node的configmap

  * 改node本地kubectl env文件： `/var/lib/kubelet/kubeadm-flags.env`

    ```sh
    sudo sed -i "/^KUBELET_KUBEADM_ARGS/s/\"$/ --v=4\"/" /var/lib/kubelet/kubeadm-flags.env
    # 末尾追加 --v=4
    sudo systemctl restart kubelet
    ```

    



# tools



```sh
function kubeconf() {
  local cluster=$1
  
  if [[ "$cluster" == "/l" ]]; then
  	ls ${HOME}/.kube/config.* | awk -F '.' '{print $NF}'
  	return
  fi
  
  local f=${HOME}/.kube/config.${cluster}
  if [ ! -f "$f" ]; then
  	echo "can not find config file: $f"
  	return 1
  fi
  
  shift
  
  kubectl --kubeconfig=$f $@
}
```


```sh
kubectl -n nmspc exec "$POD" -- env curIP=123 script01

runScript() {
  kubectl -n nmspc exec "$POD" -- bash -c 'export curIP=123 && script01 "$@"' _ "$@"
}

runScript --command "do stuff"
```



## 导出资源内容以作导入

* 老版本里有`--export`，类似 `kubectl get MYOBJECT --export -o yaml > my.yaml`

  **然而deprecated了**

* 替代方式： **官方没有说法** 

  > There is no consistent way to do this since there is **no overall guidelines about defaulting and other live data clean up**. That is why it was deprecated. You should **keep your source files in git or similar**.

  https://stackoverflow.com/questions/61392206/kubectl-export-is-deprecated-any-alternative

  网友的一些实践做法：

  * yaml: `yq`

    ```sh
    kubectl get secret "my_secret" -n "my_namespace" --context "my_context" -o yaml \
        | yq d - 'metadata.resourceVersion' \
        | yq d - 'metadata.uid' \
        | yq d - 'metadata.annotations' \
        | yq d - 'metadata.creationTimestamp' \
        | yq d - 'metadata.selfLink' \
        | yq d - 'metadata.managedFields'
    ```

  * json: `jq`

    ```sh
    kubectl get secret <secretname> -ojson | jq 'del(.metadata.namespace,.metadata.resourceVersion,.metadata.uid) | .metadata.creationTimestamp=null'
    ```

  * 从`kubectl.kubernetes.io/last-applied-configuration`里取

    ```sh
    kubectl get <resource kind> <resource name> -o yaml | \
    yq r - 'metadata.annotations."kubectl.kubernetes.io/last-applied-configuration"'
    ```

    



## [easy_kubectl](https://github.com/zhranklin/easy_kubectl)

















