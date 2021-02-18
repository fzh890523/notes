# client



* <del>native client</del>
* web client



`There is no vSphere Client for 6.7. The old legacy C# client is long gone and not compatible with 6.7. The HTML5 embedded host client (https://FQDN/ui) is what's now available.`



## web client



### misc



#### idle timeout

`host - manage - system - advanced settings`

* key: UserVars.HostClientSessionTimeout
* name:  Default timeout for Host Client sessions in seconds
* 限制： 0 ≤ x ≤ 7200
* default: 60



最大只能设为2h



# network

## ssh



### add authorized_keys

ref： [Allowing SSH access to ESXi/ESX hosts with public/private key authentication (1002866)](https://kb.vmware.com/s/article/1002866)

不同版本或有差异。`For ESXi 5.x, 6.0, 6.5 and 6.7, the authorized_keys is located at: /etc/ssh/keys-<username>/authorized_keys`

> 默认只有 keys-root 目录，其他用户的要自己创建（有权限）



# storage



## data store



### data store 访问



* 可以通过scp： `scp <user>@<esxi_host>:/vmfs/volumes/<datastore>/path/to/files/* ./`

  > vm可以以这种方式访问