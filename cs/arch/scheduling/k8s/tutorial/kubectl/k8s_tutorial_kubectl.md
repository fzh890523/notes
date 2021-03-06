

ref：

* [kubectl-cheatsheet](https://kubernetes.io/docs/reference/kubectl/cheatsheet/)

* [kubectl commands](https://kubernetes.io/docs/reference/generated/kubectl/kubectl-commands)

* [kubectl conventions](https://kubernetes.io/docs/reference/kubectl/conventions/)

* [kubectl overview](https://kubernetes.io/docs/reference/kubectl/overview/)



# 子命令



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



## 网络



### port-forward

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
  ```
  
  低版本好像不支持range
  
  

  




#### 筛选条目

* 按label（selector）筛选： `-l`/`--selector`

  支持 `=`/`!=`/`==`，可以逗号分隔指定多个同时满足的条件。如： `-l key1=val2,key2=val2`

* 按字段（内容）进行筛选： `--field-selector`

  每种类型有对应支持的几种筛选条件。 使用方式类似`-l`

  如： 

  ```sh
  kubectl get pods --field-selector status.phase=Running
  kubectl get pods --field-selector=status.phase!=Running,spec.restartPolicy=Always
  kubectl get statefulsets,services --all-namespaces --field-selector metadata.namespace!=default
  ```

* 获取所有ns数据（默认为当前ns）： `--all-namespaces`



#### 显示字段

* 显示所有labels： `--show-labels`

* 显示指定labels： `-L`/`--label-columns=[]`

  逗号分隔多个； 也可以重复指定。 如： `-L label1 -L label2`



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





## 文件



### cp



* ns的指定：

  * 参数指定： `-n <ns>`
  * 路径里给出： `<ns>/<pod>:<pod_path>`

* container的指定： `-c <container>`

  否则会选择第一个



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







### exec





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



## [easy_kubectl](https://github.com/zhranklin/easy_kubectl)

















