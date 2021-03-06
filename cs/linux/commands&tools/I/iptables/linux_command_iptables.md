

# tutorial

![img](http://www.linuxjournal.com/files/linuxjournal.com/ufiles/imagecache/large-550px-centered/u1002061/10822f2.png)



## general

* `-t`  指定表，默认filter表

## _view

* `iptables -t nat -L -n -v`

* `-L`/`--list` list chains and rules

* `-v` verbose

* `--line-numbers` 显示行号

  > 可以用于删除

  如：

  ```sh
  Chain OUTPUT (policy ACCEPT)
  num  target     prot opt source               destination
  1    KUBE-SERVICES  all  --  anywhere             anywhere             /* kubernetes service portals */
  2    ISTIO_OUTPUT  tcp  --  anywhere             anywhere
  ```

  然后：

  ```sh
  iptables -t nat -D OUTPUT 2
  ```

  

## _CRUD

* `-N`/`--new-chain` 添加新的chain（到指定table）

* `-X`/`--delete-chain` 删除（指定table的）指定chain

* `-D`/`--delete` 删除 rule，可以是rule （line）num也可以是详细rule内容

  * 内容： 把原语句`-A`换成`-D`即可
  * line num： 如`iptables -t nat -D OUTPUT 2` 表示从nat表的chain OUTPUT删除第二行

* `-F`/`--flush` 删除指定chain（没有则为该表中全部chain）中所有规则（逐个删除）

* `-A`/`--apend` 添加（到最后）

  ```Shell
  iptables -A INPUT -p tcp --dport 17500 -j REJECT --reject-with icmp-port-unreachable
  ```

* `-I`/`--insert` 插入（到最前）

  类似的规则可以通过在前面插入来“优先处理”，达到覆盖/旁路老规则的效果。

* `-R`/`--replace` 替换

  ```shell
  iptables -nvL --line-numbers  # 打出number，然后用这个number去替换

  Chain INPUT (policy ACCEPT 0 packets, 0 bytes)
  num   pkts bytes target     prot opt in     out     source               destination
  1        0     0 REJECT     tcp  --  *      *       0.0.0.0/0            0.0.0.0/0            tcp dpt:17500 reject-with icmp-port-unreachable

  Chain FORWARD (policy DROP 0 packets, 0 bytes)
  num   pkts bytes target     prot opt in     out     source               destination

  Chain OUTPUT (policy ACCEPT 0 packets, 0 bytes)
  num   pkts bytes target     prot opt in     out     source               destination

  iptables -R INPUT 1 -p tcp --dport 17500 ! -s 10.0.0.85 -j REJECT --reject-with icmp-port-unreachable  # number： 1
  ```

* `-Z`/`--zero` 清零（指定chain，默认全部chains）的包/byte计数

* `-P`/`--policy` 给chain指定策略（target），限于内建的chain

* `-E`/`—rename-chain` 重命名chain

## op

* `-d`/`--destination [!] address[/mask]` 指定目的（ip），支持mask格式

* `-s`/`--source [!] address[/mask]` 指定源（ip），支持mask格式

* `-p`/`--protocol [!] protocol` 指定协议，tcp、udp 。。。

* `-j`/`--jump` 指定规则target，也即需要对包做什么

  target参见下面targets部分

  默认（无该参数的话）表示该规则不影响该包的处理（除非`-g`参数存在）

* `-g`/`--goto chain` 跳至指定的chain继续执行

  Unlike the --jump option return will not continue processing in this chain but instead in the chain that called us via —jump ？？？

* `-i`/`--in-interface [!] name`  指定入包接口（只对 INPUT、FORWARD、PREROUTING（就是入方向的几个了）有效）

  `!`表示”其他接口； `+`结尾来表示前缀通配

* `-o`/`--out-interface [!] name` 指定出包接口（… FORWARD、OUTPUT、POSTROUTING ...）

* `[!] -f, --fragment` 只适用于分片包的非分片；`!`则表示只适用于首分片

* `-c, --set-counters PKTS BYTES` （在INSERT APPEND REPLACE时）指定规则的计数器初始值

* iptables-save

* iptables-restore



## modules



* list loaded modules

  ```sh
  $ cat /proc/net/ip_tables_matches
  owner
  addrtype
  conntrack
  conntrack
  conntrack
  addrtype
  icmp
  udplite
  udp
  tcp
  ```


