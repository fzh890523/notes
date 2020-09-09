
### link

#### show

```sh
 ip link show
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN mode DEFAULT group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
2: ens33: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc fq_codel state UP mode DEFAULT group default qlen 1000
    link/ether 00:xx brd ff:ff:ff:ff:ff:ff
    alias nat
```

#### set alias

```sh
ip link set lo0 alias test
```





## route



### 修改路由权重



```sh
ip route
default via 192.168.0.1 dev ens38 proto dhcp src 192.168.154.165 metric 100
default via 192.168.64.2 dev ens33 proto dhcp src 192.168.64.128 metric 100
# ...

sudo ip route change default via 192.168.64.2 dev ens33 proto dhcp metric 99
RTNETLINK answers: No such file or directory  # 报错

sudo ip route del default via 192.168.64.2 dev ens33 proto dhcp
sudo ip route add default via 192.168.64.2 dev ens33 proto dhcp  src 192.168.64.128 metric 99
# ok
```



但这样似乎不能持久化。。。

