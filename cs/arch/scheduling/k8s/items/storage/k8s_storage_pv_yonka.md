

ref： https://kubernetes.io/docs/concepts/storage/persistent-volumes/



## 对象/数据解构

```yaml
apiVersion: v1
kind: PersistentVolume
metadata:
  name: pv0003  # 需要是 dns subdomain name
spec:
  capacity:  # 单位见： https://git.k8s.io/community/contributors/design-proposals/scheduling/resources.md
    storage: 5Gi
    # 以后可能包含 iops、吞吐量等
  volumeMode: Filesystem  # 目前支持： fs（默认）、Block
  accessModes:
    - ReadWriteOnce
  persistentVolumeReclaimPolicy: Recycle
  storageClassName: slow
  mountOptions:
    - hard
    - nfsvers=4.1
  nfs:
    path: /tmp
    server: 172.17.0.2
```





## 支持的种类

插件形式。。

- [`awsElasticBlockStore`](https://kubernetes.io/docs/concepts/storage/volumes/#awselasticblockstore) - AWS Elastic Block Store (EBS)
- [`azureDisk`](https://kubernetes.io/docs/concepts/storage/volumes/#azuredisk) - Azure Disk
- [`azureFile`](https://kubernetes.io/docs/concepts/storage/volumes/#azurefile) - Azure File
- [`cephfs`](https://kubernetes.io/docs/concepts/storage/volumes/#cephfs) - CephFS volume
- [`cinder`](https://kubernetes.io/docs/concepts/storage/volumes/#cinder) - Cinder (OpenStack block storage) (**deprecated**)
- [`csi`](https://kubernetes.io/docs/concepts/storage/volumes/#csi) - Container Storage Interface (CSI)
- [`fc`](https://kubernetes.io/docs/concepts/storage/volumes/#fc) - Fibre Channel (FC) storage
- [`flexVolume`](https://kubernetes.io/docs/concepts/storage/volumes/#flexVolume) - FlexVolume
- [`flocker`](https://kubernetes.io/docs/concepts/storage/volumes/#flocker) - Flocker storage
- [`gcePersistentDisk`](https://kubernetes.io/docs/concepts/storage/volumes/#gcepersistentdisk) - GCE Persistent Disk
- [`glusterfs`](https://kubernetes.io/docs/concepts/storage/volumes/#glusterfs) - Glusterfs volume
- [`hostPath`](https://kubernetes.io/docs/concepts/storage/volumes/#hostpath) - HostPath volume (for single node testing only; WILL NOT WORK in a multi-node cluster; consider using `local` volume instead)
- [`iscsi`](https://kubernetes.io/docs/concepts/storage/volumes/#iscsi) - iSCSI (SCSI over IP) storage
- [`local`](https://kubernetes.io/docs/concepts/storage/volumes/#local) - local storage devices mounted on nodes.
- [`nfs`](https://kubernetes.io/docs/concepts/storage/volumes/#nfs) - Network File System (NFS) storage
- `photonPersistentDisk` - Photon controller persistent disk. (This volume type no longer works since the removal of the corresponding cloud provider.)
- [`portworxVolume`](https://kubernetes.io/docs/concepts/storage/volumes/#portworxvolume) - Portworx volume
- [`quobyte`](https://kubernetes.io/docs/concepts/storage/volumes/#quobyte) - Quobyte volume
- [`rbd`](https://kubernetes.io/docs/concepts/storage/volumes/#rbd) - Rados Block Device (RBD) volume
- [`scaleIO`](https://kubernetes.io/docs/concepts/storage/volumes/#scaleio) - ScaleIO volume (**deprecated**)
- [`storageos`](https://kubernetes.io/docs/concepts/storage/volumes/#storageos) - StorageOS volume
- [`vsphereVolume`](https://kubernetes.io/docs/concepts/storage/volumes/#vspherevolume) - vSphere VMDK volume





## items

### reclaiming 回收

删掉pvc后，对对应的pv（的使用内容）的处理，通过`persistentVolumeReclaimPolicy`字段设置：

* `Retain`保留： 不做额外操作

  那么如果要清理等，要人工操作

* `Delete`删除： 会把pv对象一起删掉，而对于背后的存储来说，这个操作可能（或者说应该会）触发对应的内容清理操作

* `Recycle`回收： 在流程里做一些（资源）清理操作以使得volume能以干净的状态再被使用

  > **文档里说这种方式不建议使用了，对应的需求场景建议用 动态供应**

  > If supported by the underlying volume plugin, the `Recycle` reclaim policy performs a basic scrub (`rm -rf /thevolume/*`) on the volume and makes it available again for a new claim.
  >
  > yonka: 这个没看明白，为什么要v插件支持？ 

  > 文档里也提到，可以自定义用于pv-recycle的pod template
  >
  > > 看起来，这个机制是通过controller注入一个类似post-process在pod销毁阶段执行的容器来实现的，默认容器、命令是如上
  >
  > 自定义的命令参数：
  >
  > * `--pv-recycler-pod-template-filepath-hostpath string`
  > * `--pv-recycler-pod-template-filepath-nfs string`



> Currently, only NFS and HostPath support recycling. AWS EBS, GCE PD, Azure Disk, and Cinder volumes support deletion



### 预留 reserve/预bind

需求场景： 某些pv本身就是为了特定用途，并不想被其他使用方使用，或者说不想被其他（乱入，误）匹配的pvc bind

思路： 自身表明只能被哪个pvc bind



```yaml
apiVersion: v1
kind: PersistentVolume
metadata:
  name: foo-pv
spec:
  storageClassName: ""
  claimRef:
    name: foo-pvc  # pvc name
    namespace: foo
  ...
```



对应的pvc：

```yaml
apiVersion: v1
kind: PersistentVolumeClaim
metadata:
  name: foo-pvc
  namespace: foo
spec:
  storageClassName: "" # Empty string must be explicitly set otherwise default StorageClass will be set
  volumeName: foo-pv
  ...
```



### pvc扩容 expand

操作： 直接改pvc里的内容

条件： 对应的sc能支持

> yonka: 这是里是声明的sc还是match的sc？ 因为有无sc而apply default sc 和 禁用sc而直接用ref的pv的sc的情况

> 文里说，实际操作不是创建新的pv，而是对应的pv本身resize
>
> yonka: 那有没有可能重新re-match/re-bind？ 可能不行，因为这样会导致数据丢失吧，这个bind可能只能人工解绑？



### access mode

- ReadWriteOnce/RWO -- the volume can be mounted as read-write by a single node
- ReadOnlyMany/ROX -- the volume can be mounted read-only by many nodes
- ReadWriteMany/RWX -- the volume can be mounted as read-write by many nodes

> 支持哪些modes取决于底层的v插件，有一些中间层插件类型比如CSI，还取决于底层的驱动（实际的实现）

> 一个pv只能一一种模式被挂载，比如即使支持rox的，也不能同时被（不同node）以rox和rwo挂载



### mount options

* 不是所有的v插件都支持mount option
* 如果指定了不支持的option，会失败



### node affinity





### dynamic proisioning



https://kubernetes.io/docs/concepts/storage/dynamic-provisioning/



```yaml
apiVersion: storage.k8s.io/v1
kind: StorageClass
metadata:
  name: slow
provisioner: kubernetes.io/gce-pd
parameters:
  type: pd-standard
---
apiVersion: storage.k8s.io/v1
kind: StorageClass
metadata:
  name: fast
provisioner: kubernetes.io/gce-pd
parameters:
  type: pd-ssd
  
---
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





### storage classes

https://kubernetes.io/docs/concepts/storage/storage-classes/





# examples



## block volume使用样例



```yaml
apiVersion: v1
kind: PersistentVolume
metadata:
  name: block-pv
spec:
  capacity:
    storage: 10Gi
  accessModes:
    - ReadWriteOnce
  volumeMode: Block
  persistentVolumeReclaimPolicy: Retain
  fc:
    targetWWNs: ["50060e801049cfd1"]
    lun: 0
    readOnly: false
```



```yaml
apiVersion: v1
kind: PersistentVolumeClaim
metadata:
  name: block-pvc
spec:
  accessModes:
    - ReadWriteOnce
  volumeMode: Block
  resources:
    requests:
      storage: 10Gi
```



```yaml
apiVersion: v1
kind: Pod
metadata:
  name: pod-with-block-volume
spec:
  containers:
    - name: fc-container
      image: fedora:26
      command: ["/bin/sh", "-c"]
      args: [ "tail -f /dev/null" ]
      volumeDevices:
        - name: data
          devicePath: /dev/xvda
  volumes:
    - name: data
      persistentVolumeClaim:
        claimName: block-pvc
```







