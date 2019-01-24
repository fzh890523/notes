# intro



netfilter的cmd工具： iptables。

所以，iptables没有守护进程，严格的说不是个服务。即使支持 `service iptables start` 类似的操作。





## flow



**入方向**

* PREROUTING(nat)


* routing decision：目标是否本机
  * 是 -> INPUT(filter) -> userSpace(local process)
  * 否 -> 是否允许转发
    * 是 -> FORWARD(filter)
    * 否 -> DROP （吧？）



**出方向**

* routing decision
* OUTPUT(nat-OUTPUT, filter-OUTPUT)
* POSTROUTING(nat)







```
                               XXXXXXXXXXXXXXXXXX
                             XXX     Network    XXX
                               XXXXXXXXXXXXXXXXXX
                                       +
                                       |
                                       v
 +-------------+              +------------------+
 |table: filter| <---+        | table: nat       |
 |chain: INPUT |     |        | chain: PREROUTING|
 +-----+-------+     |        +--------+---------+
       |             |                 |
       v             |                 v
 [local process]     |           ****************          +--------------+
       |             +---------+ Routing decision +------> |table: filter |
       v                         ****************          |chain: FORWARD|
****************                                           +------+-------+
Routing decision                                                  |
****************                                                  |
       |                                                          |
       v                        ****************                  |
+-------------+       +------>  Routing decision  <---------------+
|table: nat   |       |         ****************
|chain: OUTPUT|       |               +
+-----+-------+       |               |
      |               |               v
      v               |      +-------------------+
+--------------+      |      | table: nat        |
|table: filter | +----+      | chain: POSTROUTING|
|chain: OUTPUT |             +--------+----------+
+--------------+                      |
                                      v
                               XXXXXXXXXXXXXXXXXX
                             XXX    Network     XXX
                               XXXXXXXXXXXXXXXXXX
```



![](_pics/iptables_process_flow.jpg)





![](_pics/iptables_process_flow_zsy.png)





![](_pics/iptables-ip-processing.jpg)

> 实测，发往 127.0.0.1 的流量不受 nat表PREROUTING链 影响； 而发往 eth0上ip的流量则受。。。





## tables



* FILTER

  主要用于“过滤”（而非“修改），iptables的主要table

* NAT

  顾名思义。

* MANGLE

  可用于修改包内容（特殊字段等）

* RAW

  主要用于添加 不追踪连接 的规则

* SECURITY



![](_pics/iptables-tables.png)





## chains

* **INPUT CHAIN**: It is used for rules which are applicable to the traffic/packets coming towards the server.
* **OUTPUT CHAIN:** It is used for rules which need to be applied on outgoing traffic/packets from our server.
* **FORWARD CHAIN: **It is used for adding rules related to forwarding an IP packet.
* **PRE-ROUTING CHAIN: **It is used to add rules which define actions that need to be **taken before a routing decision is made** by the kernel.
* **POST-ROUTING CHAIN: **It is used for adding rules which will define actions that need to be taken **after a routing decision which is taken** by the kernel.





|              | PREROUTING | INPUT      | OUTPUT | FORWARD | POSTROUTING |
| ------------ | ---------- | ---------- | ------ | ------- | ----------- |
| **filter**   |            | √          | √      | √       |             |
| **nat**      | √          | √（centos7） | √      |         | √           |
| **raw**      | √          |            | √      |         |             |
| **mangle**   | √          | √          | √      | √       | √           |
| **security** |            |            |        |         |             |







## rules



![](_pics/iptables-rules-chain.png)







## targets

*A firewall rule specifies criteria for a packet and a target. If the packet does not match, the next rule in the chain is the examined; if it does match, then the next rule is specified by the value of the target, which can be the name of a user-defined chain or one of the special values ACCEPT, DROP [, REJECT], QUEUE or RETURN.*



可以是

* 用户定义的chain（非当前rule所在的chain）

* 内建的特殊target

  * RETURN

    *means stop traversing this chain and resume at the next rule in the previous (calling) chain. If the end of a built-in chain is reached or a rule in a built-in chain with target RETURN is matched, the target specified by the chain policy determines the fate of the packet.*

    1. 从一个CHAIN里可以jump到另一个CHAIN, jump到的那个CHAIN是子CHAIN.
    2. 从子CHAIN return后，回到触发jump的那条规则，从那条规则的下一条继续匹配.
    3. 如果return不是在子CHAIN里，而是在main CHAIN，那么就以默认规则进行. 

  * `REJECT` *is used to send back an error packet in response to the matched packet: otherwise it is equivalent to DROP so it is a **terminating TARGET**, ending rule traversal.*

  * `ACCEPT` *means to let the packet through*

  * `DROP` *means to drop the packet on the floor, i.e. to discard it and not send any response*

  * `QUEUE` *means to pass the packet to userspace*

* 扩展



## extensions



### match extensions



* account
* addrtype
  * UNSPEC
  * UNICAST
  * LOCAL
  * BROADCAST
  * ANYCAST
  * MULTICAST
  * BLACKHOLE
  * UNREADHABLE
  * PROHIBIT
  * THROW
  * NAT
  * XRESOLVE
* childlevel
* comment
* connbytes
* connlimit
* connmark
* connrate
* conntrack
* dccp
* dscp
* dstlimit
* ecn
* esp
* fuzzy
* hashlimit
* helper
* icmp
* iprange
* ipv4options
* length
* limit
* mac
* mark
* mport
* multiport
* nth
* osf
* owner
* physdev
* pkttype
* policy
* psd
* quota
* random
* realm
* recent
* sctp
* set
* state
* string
* tcp
* tcpmss
* time
* tos
* ttl
* u32
* udp
* unclean



### target extensions

* BALANCE
* CLASSIFY
* CLUSTERIP
* CONNMARK
* DNAT
* DSCP
* ECN
* IPMARK
* LOG
* MARK
* MASQUERADE
* MIRROR
* NETMAP
* NFQUEUE
* NOTRACK
* REDIRECT
* REJECT
* SAME
* SET
* SNAT
* TARPIT
* TCPMSS
* TOS
* TRACE
* TTL
* ULOG
* XOR



# tutorial



* iptables

  * general

    * `-t`  指定表，默认filter表

  * _view

    * `iptables -t nat -L -n -v`
    * `-L`/`--list` list chains and rules
    * `-v` verbose

  * _CRUD

    * `-N`/`--new-chain` 添加新的chain（到指定table）

    * `-X`/`--delete-chain` 删除（指定table的）指定chain

    * `-D`/`--delete` 删除 rule，可以是rule num也可以是详细rule内容

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

  * op

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






# ref



* [zsythink-iptables详解](http://www.zsythink.net/archives/1199)



