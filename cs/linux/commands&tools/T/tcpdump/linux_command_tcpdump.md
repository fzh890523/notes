



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



* `tcpdump net 130.190.0.0/17`

* `... net 192.168.1` 前缀匹配，类似的`... net 10`
* `,,, src net 10`



## ip/host





```sh
# like
tcpdump -i eth0 host 192.168.1.3 and port 5060 -n -s 0 -vvv -w /usr/src/dump
```



* 两个host之间的流量： `tcpdump host host1 and host2`

  > 似乎用简单的`∩`来理解即可，本来`host host1` 应该等价于 `src host1 or dst host1`

* 指定src/dst： `dst host1`/`src host1`



## port



* `src port ftp`
* `tcp port 80`
* `port 53`



## proto

* `proto \\icmp` 因为常见协议名都是保留字，所以通过proto指定时需要转义一下



### udp



```shell
tcpdump -i any udp port 53 -vv -X
```



## 特殊包： 如 tcp-syn ack fin rst



```sh
nohup tcpdump -i any tcp port 15010 and 'not(host 192.168.1.3 or host 192.168.1.4)' and "tcp[tcpflags] & (tcp-syn|tcp-fin|tcp-rst) != 0"
```





## `-X` 打印



### ip、端口转换



```sh
-n     Don't convert host addresses to names.   This  can  be  used  to
              avoid DNS lookups.

-nn    Don't convert protocol and port numbers etc. to names either.
```





