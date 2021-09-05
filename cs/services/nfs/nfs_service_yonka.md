

## protocol



NFS

Network File System (NFS) is used by UNIX clients for file access. NFS uses port 2049.

NFSv3 and NFSv2 use the portmapper service on TCP or UDP port 111. The portmapper service is consulted to get the port numbers for services used with NFSv3 or NFSv2 protocols such as mountd, statd, and nlm. NFSv4 does not require the portmapper service.

NFSv4 provides the delegation feature that enables your storage system to grant local file access to clients. To delegate, your storage system sets up a separate connection to the client and sends callbacks on it. To communicate with the client, your storage system uses one of the reserved ports (port numbers less than 1024). To initiate the connection, the client registers the callback program on a random port and informs the server about it.

With delegations enabled, NFSv4 is not firewall-friendly because several other ports need to be opened up as well.

Starting with Data ONTAP 7.3.1, IPv6 over NFS is supported.

You can disable the TCP and UDP ports by setting the nfs.tcp.enable and nfs.udp.enable options to off.

To disable NFS, you should use the nfs off command.





## 处理跨fs情况



如要export `/mnt`的话，`/mnt`下面基本都是单独mount的fs，那么需要特殊处理（分别指定fsid），如下：

```sh
/mnt    (rw,crossmnt)
/mnt/xxx    (rw,fsid=1)
/export/mnt     (rw,crossmnt)
/export (rw,fsid=0)
```



简单的生成脚本：

> fsid从1开始递增。 如果实际有占用，需要做对应调整

```sh
 i=0; for fs in `ls /mnt/`; do ((i++)); sudo printf '/mnt/%s    (rw,fsid=%d)\n' "$fs" "$i" | sudo tee -a /etc/exports; done
```





