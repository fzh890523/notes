

## dns

### 手动添加/自定义 dns entry

目前uck（controller）里尚不支持，只能在usg里设置（估计可能丢失）。

```sh
# ssh to usg

configure  # enter edit mode
set system static-host-mapping host-name host1.xx.com inet 1.1.1.1
# 还可以加上： set system static-host-mapping host-name host1.xx.com alias host1
## 这样的话可以 nslookup host1 解析得到 1.1.1.1 了
commit  # 可能需要一小会儿
save
exit  # exit edit mode
```

这个操作会在`/etc/hosts`中加入一条 `1.1.1.1 host1.xx.com       #vyatta entry` 的条目。 不过自行添加不行，不reload对应的service的话不生效。



## 抓包

https://help.ui.com/hc/en-us/articles/227129127-UniFi-Methods-for-Capturing-Useful-Debug-Information



`sudo tcpdump`  否则找不到命令

`tcpdump -np` 否则抓不到包（这两个选项有空看下啥意思）





