

# 子命令



## logs



* `tail` 效果： `--follow`



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



#### 获取所有ns数据（默认为当前ns）

`--all-namespaces`



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



















