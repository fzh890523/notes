

ref：

* https://kubernetes.io/docs/concepts/workloads/pods/ephemeral-containers/



## 启用



To enable the Ephemeral Containers feature, edit the following configuration files on your Kubernetes master nodes and restart each master:

Enable the EphemeralContainers alpha feature gate in the following places

* /etc/kubernetes/manifests/kube-apiserver.yaml

* /etc/kubernetes/manifests/kube-scheduler.yaml

by adding the following line inside the command section:

`--feature-gates=EphemeralContainers=true`

> yonka: 还有 controller-manager 也要操作，**kublet也要操作**
>
> 此外，起码在1.20版本里，如果运行时修改master的话，已有的<u>worker node的kubelet</u>的配置不会同步，需要手动完成：
>
> > 否则会hang住，能看到container信息，但不会实际运行
>
> ```yaml
> # /var/lib/kubelet/config.yaml 
> featureGates:
>   EphemeralContainers: true
> ```
>
> `sudo systemctl restart kubelet.service`
>
> ref:
>
> * https://kubernetes.io/docs/reference/config-api/kubelet-config.v1beta1/
> * https://stackoverflow.com/questions/65246520/ephemeral-containers-in-kubernetes
> * https://stackoverflow.com/questions/65306212/kubectl-debug-hangs-on-1-20-with-feature-gate-enabled



## 使用



* `kubectl debug`

  ```sh
  kubectl debug -n kube-system -it coredns-74ff55c5b-s8sn6 --image=busybox --target=coredns
  ```

* `kubectl replace`

  ```sh
  kubectl replace --raw /api/v1/namespaces/default/pods/test-nfs-prov/ephemeralcontainers     -f /tmp/ec.json
  ```

  ```json
  {
      "apiVersion": "v1",
      "kind": "EphemeralContainers",
      "metadata": {
              "name": "test-nsf-prov"
      },
      "ephemeralContainers": [{
          "command": [
              "sh"
          ],
          "image": "busybox",
          "imagePullPolicy": "IfNotPresent",
          "name": "debugger",
          "stdin": true,
          "tty": true,
          "terminationMessagePolicy": "File",
          "volumeMounts": [{
            "mountPath": "/var/data",
            "name": "test-nfs-prov"
          }]
      }]
  }
  ```

  > 然后可以attach/exec到`debugger`这个容器了
  >
  > replace加进去的ec好像无法删除，尬... 





