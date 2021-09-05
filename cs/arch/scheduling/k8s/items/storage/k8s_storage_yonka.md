

* v(volume)

  * 把存储和容器解耦

  * 但<u>自身生命周期与pod耦合</u>

    所以”角色“也不独立，比如无法创建一个单独的volume

  * 允许（pod内）容器间共享存储，以及容器重启后保持访问

* pv

  > PersistentVolume（PV）是集群中由管理员配置的一块存储。它是集群中的资源，就和节点是集群资源一样。PV是卷插件比如Volumes，但是它的生命周期独立于使用PV的任何pod个体。该API对象捕获实现存储的详细信息，包括NFS、iSCSI或着是云服务商特定的存储系统。

  * 如上，pv的特点在于 （角色、生命周期）独立于pod，可以认为是 ”first-class obj“

  * 描述了”存储“**本身**，在pvc中引用

  * 可以手动创建，也可以根据需要自动创建： `dynamic provisioning`，通过`StorageClass`

    当pvc需求的（静态）pv不存在时，会尝试动态供应： 根据sc，而显式设置sc为`""`则表示禁止（为自己）动态供应pv

  * binding概念： 无论是静态还是动态供应，最终（如果能）达成的的是`pv -binding- pvc`

    如果因为资源不足（比如请求量太大）无法完成bind，等满足条件时会重新完成bind

* pvc

  > PersistentVolumeClaim（PVC）是用户关于存储的请求。它类似于一个pod，pod消耗节点资源，而PVC消耗PV资源。Pods可以请求特定级别的资源（CPU和内容），而Claim可以请求特定的大小和访问模式（例如，可以一次读/写或者多次只读）。

  * 描述了对存储的 使用、申请、额度分配等

    pv画出一个饼，pvc切走一块

  * 可以手动创建，也可以用pvcTemplate来动态创建

    如：

    ```yaml
    apiVersion: v1
    kind: PersistentVolumeClaim
    metadata:
      name: claim1
    spec:
      accessModes:
        - ReadWriteOnce
      storageClassName: fast
      resources:
        requests:
          storage: 30Gi
    ```

    

  * 在container中引用

* StorageClass

  * 可以提供dynamic provisioning的能力

  * 可以设置某个sc为默认sc（通过annotation `storageclass.kubernetes.io/is-default-class`）

    如果（全局）有默认sc的话，那么会自动填充给没指定`storageClassName`的pvc（by `DefaultStorageClass`这个admission controller）

    > 显然这个默认sc最多只能有一个，否则无法创建不带sc name的pvc
    >
    > > 也就是说如果没有的话也不能创建吧

  * 样例：

    ```yaml
    apiVersion: storage.k8s.io/v1
    kind: StorageClass
    metadata:
      name: slow
    provisioner: kubernetes.io/gce-pd
    parameters:
      type: pd-standard
    ```

    



# volume types



## nsf支持

* static provision： 支持

* dynamic provision： internal（官方）不支持，[external（第三方）](https://github.com/kubernetes-sigs/nfs-subdir-external-provisioner)支持

  https://github.com/kubernetes-sigs/sig-storage-lib-external-provisioner 列了几个



## smb/cifs支持

* 官方volume type： [azure_file](https://github.com/kubernetes/examples/blob/master/staging/volumes/azure_file/README.md)

  这个实际是云上azure的smb支持... 对其他场景无意义

* https://k8scifsvol.juliohm.com.br/