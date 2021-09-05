

* 单文件多对象：

  * `List`对象来wrap： 导出的格式，但不能apply（晕）

  * `---`分隔： 可以apply

    ```yaml
    kind: PersistentVolume
    apiVersion: v1
    # ...
    ---
    kind: PersistentVolumeClaim
    apiVersion: v1
    # ...
    ```

    

