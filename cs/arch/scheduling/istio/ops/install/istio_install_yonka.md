

# revision



## 新老版本并存： 支持和不支持rev的



### 注入

* 老的使用： ns级别 `istio-injection=enabled` + object（pod/dep）级别  `sidecar.istio.io/inject NotIn [false]`
* 新版本使用： `istio.io/rev In []` **并且**为了不和老版本冲突，加上了`istio-injection DoesNotExist`



# 卸载

* `./bin/istioctl x uninstall --set revision=1-11-4`
* `--purge` **会删除所有版本，注意！**



# 安装



## 下载

* `curl -L https://istio.io/downloadIstio | sh`

* `curl -L https://istio.io/downloadIstio | ISTIO_VERSION=1.6.8 TARGET_ARCH=x86_64 sh -`



## record/example



```sh
curl -L https://istio.io/downloadIstio | sh   # or download a specific version like: curl -L https://istio.io/downloadIstio | ISTIO_VERSION=1.6.8 TARGET_ARCH=x86_64 sh -
# cd into downloaded release, like 1.11.4
# istioctl is under bin/
./bin/istioctl x precheck
./bin/istioctl install --set revision=1-11-4
WARNING: Istio is being downgraded from 1.9.1 -> 1.11.4.This will install the Istio 1.11.4 default profile with ["Istio core" "Istiod" "Ingress gateways"] components into the cluster. Proceed? (y/N) y
# 资源request太多了： 500m + 2g。 起不来。。。 gg
# 看了下，control-c并没cleanup。 手动删吧
# ./bin/istioctl x uninstall --purge --set revision=1-11-4  # 我擦执行错了，这个会导致全部被清除
./bin/istioctl x uninstall --set revision=1-11-4  # 应该是这个
# 只能全部重新安装了

./bin/istioctl install --set revision=1-11-4 --set components.pilot.k8s.resources.requests.cpu=50m --set
components.pilot.k8s.resources.requests.memory=300Mi
This will install the Istio 1.11.4 default profile with ["Istio core" "Istiod" "Ingress gateways"] components into the cluster. Proceed? (y/N) y
✔ Istio core installed
✔ Istiod installed
✔ Ingress gateways installed
✔ Installation complete
Thank you for installing Istio 1.11.  Please take a few minutes to tell us about your install/upgrade experience!  https://forms.gle/kWULBRjUv7hHci7T6

# istio-samples里的deployment的注入label还是老版本的，所以不生效了。 delete pod重启后就没sidecar了


# 把1.9也装回去
curl -L https://istio.io/downloadIstio | ISTIO_VERSION=1.9.9 sh -
cd istio-1.9.9
./bin/istioctl install --set revision=1-9-9 --set components.pilot.k8s.resources.requests.cpu=50m --set components.pilot.k8s.resources.requests.memory=300Mi
! Istio is being downgraded from 1.11.4 -> 1.9.9.This will install the Istio 1.9.9  profile with ["Istio core" "Istiod" "Ingress gateways"] components into the cluster. Proceed? (y/N)
# 似乎gateway没必要重复，还是换成minimal profile只装core+istiod吧

./bin/istioctl install --set profile=minimal --set revision=1-9-9 --set components.pilot.k8s.resources.req
uests.cpu=50m --set components.pilot.k8s.resources.requests.memory=300Mi
! Istio is being downgraded from 1.11.4 -> 1.9.9.This will install the Istio 1.9.9 minimal profile with ["Istio core" "Istiod"] components into the cluster. Proceed? (y/N)


```



## istioctl/helm/release内容



**release结构**

```shtree -d -L 2
tree -L 2
.
├── bin
│   └── istioctl
├── LICENSE
├── manifests
│   ├── charts
│   ├── examples
│   └── profiles
├── manifest.yaml
├── README.md
├── samples
│   ├── addons
│   ├── bookinfo
│   ├── certs
│   ├── custom-bootstrap
│   ├── extauthz
│   ├── external
│   ├── grpc-echo
│   ├── health-check
│   ├── helloworld
│   ├── httpbin
│   ├── jwt-server
│   ├── kubernetes-blog
│   ├── multicluster
│   ├── operator
│   ├── ratelimit
│   ├── README.md
│   ├── security
│   ├── sleep
│   ├── tcp-echo
│   └── websockets
└── tools
    ├── certs
    ├── _istioctl
    └── istioctl.bash

27 directories, 7 files
```



## istioctl cmd

* istioctl默认使用当前目录的manifests，可以`--manifests`参数来安装指定目录的，如 `istioctl install --manifests istio-1.11.4/manifests`

### manifest子命令

* generate： `istioctl manifest generate --manifests mycharts/ --set profile=custom1 -f path-to-user-overlay.yaml`

  **先generate再修改再apply是最灵活的安装方式了**

* diff: `istioctl manifest diff 1.yaml 2.yaml`

  先generate再diff

### profile子命令

* list

  ```sh
  ./bin/istioctl profile list
  Istio configuration profiles:
      default
      demo
      empty
      minimal
      openshift
      preview
      remote
  ```

* dump

  ```sh
  ./bin/istioctl profile dump minimal
  2021-11-06T14:34:32.274789Z     info    proto: tag has too few fields: "-"
  apiVersion: install.istio.io/v1alpha1
  kind: IstioOperator
  spec:
    components:
      base:
  ...    
  ```
  
  
  
   `istioctl profile dump --config-path components.ingressGateways`
  
  ```sh
  istioctl profile dump --config-path components.ingressGateways
  2021-11-06T14:11:14.870732Z     info    proto: tag has too few fields: "-"
  - enabled: true
    name: istio-ingressgateway
  
  istioctl profile dump --config-path components
  2021-11-06T14:11:20.043546Z     info    proto: tag has too few fields: "-"
base:
    enabled: true
  cni:
    enabled: false
  egressGateways:
  - enabled: false
    name: istio-egressgateway
  ingressGateways:
  - enabled: true
    name: istio-ingressgateway
  istiodRemote:
    enabled: false
  pilot:
    enabled: true
  ```

* diff

  ```sh
  istioctl profile diff default demo
   gateways:
     egressGateways:
  -  - enabled: false
  +  - enabled: true
  ...
       k8s:
          requests:
  -          cpu: 100m
  -          memory: 128Mi
  +          cpu: 10m
  +          memory: 40Mi
         strategy:
  ...
  ```

  

### 配置覆盖

* profile设置： `--set profile=`

* revision设置： `--set revision=1-11-4`

* 具体配置设置： `--set components.pilot.k8s.resources.requests.cpu=50m --set components.pilot.k8s.resources.requests.memory=300Mi`

  似乎把`IstioOperator` cr作为...的operator项目，所以用cr方式来描述也可以，或者更工整。 如下：

  ```yaml
  apiVersion: install.istio.io/v1alpha1
  kind: IstioOperator
  spec:
    components:
      pilot:
        k8s:
          resources:
            requests:
              cpu: 1000m # override from default 500m
              memory: 4096Mi # ... default 2048Mi
          hpaSpec:
            maxReplicas: 10 # ... default 5
            minReplicas: 2  # ... default 1
  ```

  `istioctl install -f samples/operator/pilot-k8s.yaml`



## profiles



|                        | default | demo | minimal | external | empty | preview |
| ---------------------- | ------- | ---- | ------- | -------- | ----- | ------- |
| Core components        |         |      |         |          |       |         |
| `istio-egressgateway`  |         | ✔    |         |          |       |         |
| `istio-ingressgateway` | ✔       | ✔    |         |          |       | ✔       |
| `istiod`               | ✔       | ✔    | ✔       |          |       | ✔       |





# 升级

* [in-place](https://istio.io/latest/docs/setup/upgrade/in-place/)

  > 链接里提到： [`istioctl upgrade`](https://istio.io/latest/docs/reference/commands/istioctl/#istioctl-upgrade)是in-place方式，但 **不支持** revision版本
  >
  > 要求跨度不大于一个minor版本（比如 1.6 -> 1.8 不行）。 估计是为了降低复杂度只维护了同/相邻minor版本内的升级支持

  ```sh
  kubectl config view  # confirm correct k8s cluster
  # download and cd into istio release dir
  istioctl x precheck
  istioctl upgrade  # -f <IstioOperator-custom-resource-definition-file> 与当初安装时保持一致
  kubectl rollout restart deployment  # 重启业务容器来重新注入新版本对应的sidecar
  ```

  可能的问题？

  * 老版本sidecar连到新版本控制面了？
  * 

* [canary](https://istio.io/latest/docs/setup/upgrade/canary/) 更建议的方式

  ```sh
  # download and cd into istio release dir
  
  ```

  

