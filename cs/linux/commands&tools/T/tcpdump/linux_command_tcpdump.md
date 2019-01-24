



## interface



### capture on multiple interfaces

`tcpdump -i any`

好像没法多次指定 = = （比如 `-i lo0 -i eth0`）

有一种很麻烦的方式：

```shell
tcpdump -i wan0 -nn -s 0 -w wan0.dump
tcpdump -i wan1 -nn -s 0 -w wan1.dump
tcpdump -i lan0 -nn -s 0 -w lan0.dump

mergecap -w merged.dump wan0.dump wan1.dump lan0.dump
# 噗。。。
```



## net/ip range



`tcpdump net 130.190.0.0/17`





## ip





```sh
# like
tcpdump -i eth0 host 192.168.1.3 and port 5060 -n -s 0 -vvv -w /usr/src/dump
```





## proto



### udp



```shell
tcpdump -i any udp port 53 -vv -X
```









