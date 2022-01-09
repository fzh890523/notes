

ref:

* https://kubernetes.io/docs/concepts/scheduling-eviction/taint-and-toleration/
* https://kubernetes.io/docs/reference/generated/kubectl/kubectl-commands#taint



### 查看： 从node spec里看



## tutorial

* add taint

  ```sh
  # kubectl taint nodes node-name key=value:effect
  kubectl taint nodes node-main taint=test:PrefereNoSchedule
  ```

* add toleration

  ```yaml
  spec:
    containers:
    - name: nginx
      image: nginx
      imagePullPolicy: IfNotPresent
    tolerations:
    - key: "taint"
      value: "test"
      effect: "PrefereNoSchedule"
  ```

  



### taint





```sh
k taint --help
kubectl -n kube-system taint --help
Update the taints on one or more nodes.

  * A taint consists of a key, value, and effect. As an argument here, it is expressed as key=value:effect.
  * The key must begin with a letter or number, and may contain letters, numbers, hyphens, dots, and underscores, up to
253 characters.
  * Optionally, the key can begin with a DNS subdomain prefix and a single '/', like example.com/my-app
  * The value must begin with a letter or number, and may contain letters, numbers, hyphens, dots, and underscores, up
to  63 characters.
  * The effect must be NoSchedule, PreferNoSchedule or NoExecute.
  * Currently taint can only apply to node.

Examples:
  # Update node 'foo' with a taint with key 'dedicated' and value 'special-user' and effect 'NoSchedule'.
  # If a taint with that key and effect already exists, its value is replaced as specified.
  kubectl taint nodes foo dedicated=special-user:NoSchedule

  # Remove from node 'foo' the taint with key 'dedicated' and effect 'NoSchedule' if one exists.
  kubectl taint nodes foo dedicated:NoSchedule-

  # Remove from node 'foo' all the taints with key 'dedicated'
  kubectl taint nodes foo dedicated-

  # Add a taint with key 'dedicated' on nodes having label mylabel=X
  kubectl taint node -l myLabel=X  dedicated=foo:PreferNoSchedule

Options:
      --all=false: Select all nodes in the cluster
      --allow-missing-template-keys=true: If true, ignore any errors in templates when a field or map key is missing in
the template. Only applies to golang and jsonpath output formats.
  -o, --output='': Output format. One of:
json|yaml|name|go-template|go-template-file|template|templatefile|jsonpath|jsonpath-file.
      --overwrite=false: If true, allow taints to be overwritten, otherwise reject taint updates that overwrite existing
taints.
  -l, --selector='': Selector (label query) to filter on, supports '=', '==', and '!='.(e.g. -l key1=value1,key2=value2)
      --template='': Template string or path to template file to use when -o=go-template, -o=go-template-file. The
template format is golang templates [http://golang.org/pkg/text/template/#pkg-overview].
      --validate=true: If true, use a schema to validate the input before sending it

Usage:
  kubectl taint NODE NAME KEY_1=VAL_1:TAINT_EFFECT_1 ... KEY_N=VAL_N:TAINT_EFFECT_N [options]

Use "kubectl options" for a list of global command-line options (applies to all commands).
```



* effect好像没默认值，必须带

  > - **NoSchedule**: if this taint is applied to a node which contains already some pod that doesn’t tolerate this taint, they are not excluded from this node. But no more pods are scheduled on this node if it doesn’t match all the taints of this node. This is a strong constraint.
  > - **PreferNoSchedule:** Like the previous one, this taint may not allow pods to be scheduled on the node. But this time, if the pod tolerates one taint, it can be scheduled. This is a soft constraint.
  > - **NoExecute**: This taint applies to a node excluding all actual running pods on it and doesn’t allow scheduling if new pods don’t tolerate all taint. This is a strong constraint.

* taint存在于node属性，无法单独get

  ```sh
  kubectl get nodes -o json | jq ".items[]|{name:.metadata.name, taints:.spec.taints}"
  ```

* NoSchedule, NoExecute区别在于后者会驱逐已经在该node上运行的不满足tolerance条件的pod（前者应该不会）

* 除了单独给node打以外，还可以node（kubelet参数）自己注册taint

  ```sh
  --register-with-taints []api.Taint
      Register the node with the given list of taints (comma separated "=:").
      No-op if register-node is false.
  ```

  如： `--register-with-taints=key=value:NoSchedule` （可以是逗号分隔的多个，`<key1>=<value1>:<effect1>,<key2>=<value2>:<effect2>`）



### toleration

```yaml
tolerations:
- key: "key1"
  operator: "Equal"
  value: "value1"
  effect: "NoSchedule"
```



> The default value for `operator` is `Equal`.
>
> A toleration "matches" a taint if the keys are the same and the effects are the same, and:
>
> - the `operator` is `Exists` (in which case no `value` should be specified), or
>
>   **这个可以用于实现通配**，如`kube-proxy`自带的两个toleration：
>
>   * 以下表示容忍所有key为`CriticalAddonsOnly` 的taint
>
>     ```yaml
>           - key: CriticalAddonsOnly
>             operator: Exists
>     ```
>
>   * 而以下则表示...
>
>     ```yaml
>           - operator: Exists
>     ```
>
>   * 类似的，以下表示容忍key为`yonka` effect为`NoSchedule`的taint
>
>     ```yaml
>     - key: "yonka"
>       operator: "Exists"
>       effect: "NoSchedule"
>     ```
>
>     
>
> - the `operator` is `Equal` and the `value`s are equal.







