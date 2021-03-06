
ref: https://docs.microsoft.com/en-us/windows/wsl/wsl-config

* global级别： `~/.wslconfig`

  ```properties
  [wsl2]
  kernel=C:\\temp\\myCustomKernel
  memory=4GB # Limits VM memory in WSL 2 to 4 GB
  processors=2 # Makes the WSL 2 VM use two virtual processors
  ```

* per-distro launch级别： distro中文件，如linux为`/etc/wsl.conf`

  支持的配置(blocks)：
  * `automount`
  * `network`
  
  ```properties
  # Enable extra metadata options by default
  [automount]
  enabled = true
  root = /windir/
  options = "metadata,umask=22,fmask=11"
  mountFsTab = false
  
  # Enable DNS – even though these are turned on by default, we'll specify here just to be explicit.
  [network]
  generateHosts = true
  generateResolvConf = true
  ```
  
  > 目前好像distro级别的配置不支持资源配置
  >
  > 因为wsl2实际在一个vm内，所以这个资源配置估计是指wsl全局。 不知道后面能否做到namespace（distro）级别。

详细的配置项解释见上面链接



### 配置修改

...修改

要生效的话，需要重启：

* wsl命令重启： `wsl --shutdown` 然后各自的启动方式

* `Restart-Service LxssManager` 

  > 还没试过