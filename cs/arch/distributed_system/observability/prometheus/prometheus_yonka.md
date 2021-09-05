



## 存储



* tsdb（默认）

  ```yaml
  # 以istio Prometheus为例
  # 估计是demo profile的原因，竟然是emptyDir存储
  
      - --storage.tsdb.path=/data
      
      - mountPath: /data
        name: storage-volume
        
    - emptyDir: {}
      name: storage-volume      
  ```

  

  



