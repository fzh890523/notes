

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





