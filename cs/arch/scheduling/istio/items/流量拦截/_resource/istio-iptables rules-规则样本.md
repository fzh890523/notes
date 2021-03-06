

## 一些概念理解



### istio的流量







## 不拦截入流量的




```sh
sudo iptables -L -t nat
Chain PREROUTING (policy ACCEPT)
target     prot opt source               destination

Chain INPUT (policy ACCEPT)
target     prot opt source               destination

Chain OUTPUT (policy ACCEPT)
target     prot opt source               destination
ISTIO_OUTPUT  tcp  --  anywhere             anywhere

Chain POSTROUTING (policy ACCEPT)
target     prot opt source               destination

Chain ISTIO_IN_REDIRECT (1 references)
target     prot opt source               destination
REDIRECT   tcp  --  anywhere             anywhere             redir ports 15001

Chain ISTIO_OUTPUT (1 references)
target     prot opt source               destination
RETURN     all  --  127.0.0.6            anywhere
ISTIO_IN_REDIRECT  all  --  anywhere            !localhost
RETURN     all  --  anywhere             anywhere             owner UID match istio-proxy
RETURN     all  --  anywhere             anywhere             owner GID match istio-proxy
RETURN     all  --  anywhere             localhost
ISTIO_REDIRECT  all  --  anywhere             anywhere

Chain ISTIO_REDIRECT (1 references)
target     prot opt source               destination
REDIRECT   tcp  --  anywhere             anywhere             redir ports 15001

# OUTPUT -> ISTIO_OUTPUT -> ISTIO_IN_REDIRECT/ISTIO_REDIRECT

Chain ISTIO_OUTPUT (1 references)
 pkts bytes target     prot opt in     out     source               destination
    0     0 RETURN     all  --  any    lo      127.0.0.6            anywhere
28233 1694K ISTIO_IN_REDIRECT  all  --  any    lo      anywhere            !localhost
  557 33420 RETURN     all  --  any    any     anywhere             anywhere             owner UID match istio-proxy
    0     0 RETURN     all  --  any    any     anywhere             anywhere             owner GID match istio-proxy
    0     0 RETURN     all  --  any    any     anywhere             localhost
    0     0 ISTIO_REDIRECT  all  --  any    any     anywhere             anywhere
```



### 排除出站端口



```sh
Chain ISTIO_OUTPUT (1 references)
 pkts bytes target     prot opt in     out     source               destination
    2   120 RETURN     tcp  --  any    any     anywhere             anywhere             tcp dpt:2181  # 这条， 对应参数： -o 2181
    1    60 RETURN     all  --  any    lo      127.0.0.6            anywhere
    0     0 ISTIO_IN_REDIRECT  all  --  any    lo      anywhere            !localhost
    6   360 RETURN     all  --  any    any     anywhere             anywhere             owner UID match istio-proxy
    0     0 RETURN     all  --  any    any     anywhere             anywhere             owner GID match istio-proxy
    0     0 RETURN     all  --  any    any     anywhere             localhost
    0     0 ISTIO_REDIRECT  all  --  any    any     anywhere             anywhere
```







## 标准配置



### 1.5



from: https://jimmysong.io/en/blog/sidecar-injection-iptables-and-traffic-routing/



```sh
# View the details of the rule configuration in the NAT table.
$ iptables -t nat -L -v
# PREROUTING chain: Used for Destination Address Translation (DNAT) to jump all incoming TCP traffic to the ISTIO_INBOUND chain.
Chain PREROUTING (policy ACCEPT 2701 packets, 162K bytes)
 pkts bytes target     prot opt in     out     source               destination
 2701  162K ISTIO_INBOUND  tcp  --  any    any     anywhere             anywhere

# INPUT chain: Processes incoming packets and non-TCP traffic will continue on the OUTPUT chain.
Chain INPUT (policy ACCEPT 2701 packets, 162K bytes)
 pkts bytes target     prot opt in     out     source               destination

# OUTPUT chain: jumps all outbound packets to the ISTIO_OUTPUT chain.
Chain OUTPUT (policy ACCEPT 79 packets, 6761 bytes)
 pkts bytes target     prot opt in     out     source               destination
   15   900 ISTIO_OUTPUT  tcp  --  any    any     anywhere             anywhere

# POSTROUTING CHAIN: All packets must first enter the POSTROUTING chain when they leave the network card, and the kernel determines whether they need to be forwarded out according to the packet destination.
Chain POSTROUTING (policy ACCEPT 79 packets, 6761 bytes)
 pkts bytes target     prot opt in     out     source               destination

# ISTIO_INBOUND CHAIN: Redirects all inbound traffic to the ISTIO_IN_REDIRECT chain, except for traffic destined for ports 15090 (used by mixer) and 15020 (used by Ingress gateway for Pilot health checks), and traffic sent to these two ports will return to the call point of the iptables rule chain, the successor POSTROUTING to the PREROUTING chain.
Chain ISTIO_INBOUND (1 references)
 pkts bytes target     prot opt in     out     source               destination
    0     0 RETURN     tcp  --  any    any     anywhere             anywhere             tcp dpt:ssh
    2   120 RETURN     tcp  --  any    any     anywhere             anywhere             tcp dpt:15090
 2699  162K RETURN     tcp  --  any    any     anywhere             anywhere             tcp dpt:15020
    0     0 ISTIO_IN_REDIRECT  tcp  --  any    any     anywhere             anywhere

# ISTIO_IN_REDIRECT chain: jumps all inbound traffic to the local 15006 port, thus successfully blocking traffic to the sidecar.
Chain ISTIO_IN_REDIRECT (3 references)
 pkts bytes target     prot opt in     out     source               destination
    0     0 REDIRECT   tcp  --  any    any     anywhere             anywhere             redir ports 15006

# ISTIO_OUTPUT chain: select the outbound traffic that needs to be redirected to Envoy (i.e., local), and all non-localhost traffic is forwarded to ISTIO_REDIRECT. to avoid infinite loops in the pod, all traffic to the istio-proxy userspace is returned to the next rule in its call point, which in this case is the OUTPUT chain, because jumping out of the ISTIO_OUTPUT rule leads to the next chain POSTROUTING. if the destination is non-localhost, jump to ISTIO_REDIRECT; if the traffic is from the istio-proxy userspace, jump out of the chain, return its call chain to continue the next rule (the next rule of OUTPUT, without processing the traffic); all non-istio-proxy userspace traffic whose destination is localhost, jump to ISTIO_REDIRECT.
Chain ISTIO_OUTPUT (1 references)
 pkts bytes target     prot opt in     out     source               destination
    0     0 RETURN     all  --  any    lo      127.0.0.6            anywhere
    0     0 ISTIO_IN_REDIRECT  all  --  any    lo      anywhere            !localhost            owner UID match 1337  # redir了 lo & !->localhost & istio 的。 还剩： !lo | ->localhost | !istio
    0     0 RETURN     all  --  any    lo      anywhere             anywhere             ! owner UID match 1337  # ret了 lo & !istio。 还剩： !lo | istio & ->localhost
   15   900 RETURN     all  --  any    any     anywhere             anywhere             owner UID match 1337  # ret了 istio。 还剩： !lo & !istio
    0     0 ISTIO_IN_REDIRECT  all  --  any    lo      anywhere            !localhost            owner GID match 1337
    0     0 RETURN     all  --  any    lo      anywhere             anywhere             ! owner GID match 1337
    0     0 RETURN     all  --  any    any     anywhere             anywhere             owner GID match 1337
    0     0 RETURN     all  --  any    any     anywhere             localhost
    0     0 ISTIO_REDIRECT  all  --  any    any     anywhere             anywhere

# ISTIO_REDIRECT chain: redirects all traffic to Sidecar (i.e. local) port 15001.
Chain ISTIO_REDIRECT (1 references)
 pkts bytes target     prot opt in     out     source               destination
    0     0 REDIRECT   tcp  --  any    any     anywhere             anywhere             redir ports 15001
```

> yonka： 感觉这里的ISTIO_OUTPUT太复杂了。 如下这样不行吗？
>
> 1. istio流量（uid、gid）: return
> 2. -> localhost 流量: return





### 1.7



```sh
Chain PREROUTING (policy ACCEPT)
target     prot opt source               destination         
ISTIO_INBOUND  tcp  --  anywhere             anywhere            

Chain INPUT (policy ACCEPT)
target     prot opt source               destination         

Chain OUTPUT (policy ACCEPT)
target     prot opt source               destination         
ISTIO_OUTPUT  tcp  --  anywhere             anywhere            

Chain POSTROUTING (policy ACCEPT)
target     prot opt source               destination         

Chain ISTIO_INBOUND (1 references)
target     prot opt source               destination         
RETURN     tcp  --  anywhere             anywhere             tcp dpt:15008
RETURN     tcp  --  anywhere             anywhere             tcp dpt:ssh
RETURN     tcp  --  anywhere             anywhere             tcp dpt:15090
RETURN     tcp  --  anywhere             anywhere             tcp dpt:15021
RETURN     tcp  --  anywhere             anywhere             tcp dpt:15020
ISTIO_IN_REDIRECT  tcp  --  anywhere             anywhere            

Chain ISTIO_IN_REDIRECT (3 references)
target     prot opt source               destination         
REDIRECT   tcp  --  anywhere             anywhere             redir ports 15006

Chain ISTIO_OUTPUT (1 references)
target     prot opt source               destination         
RETURN     all  --  127.0.0.6            anywhere            
ISTIO_IN_REDIRECT  all  --  anywhere            !localhost            owner UID match 1337  # 
RETURN     all  --  anywhere             anywhere             ! owner UID match 1337
RETURN     all  --  anywhere             anywhere             owner UID match 1337
ISTIO_IN_REDIRECT  all  --  anywhere            !localhost            owner GID match 1337
RETURN     all  --  anywhere             anywhere             ! owner GID match 1337
RETURN     all  --  anywhere             anywhere             owner GID match 1337
RETURN     all  --  anywhere             localhost           
ISTIO_REDIRECT  all  --  anywhere             anywhere            

Chain ISTIO_REDIRECT (1 references)
target     prot opt source               destination         
REDIRECT   tcp  --  anywhere             anywhere             redir ports 15001
```

加上verbose后：

```sh
Chain ISTIO_OUTPUT (1 references)
 pkts bytes target     prot opt in     out     source               destination
    0     0 RETURN     all  --  any    lo      127.0.0.6            anywhere
    0     0 ISTIO_IN_REDIRECT  all  --  any    lo      anywhere            !localhost            owner UID match 1337
    0     0 RETURN     all  --  any    lo      anywhere             anywhere             ! owner UID match 1337
 6669  400K RETURN     all  --  any    any     anywhere             anywhere             owner UID match 1337
    0     0 ISTIO_IN_REDIRECT  all  --  any    lo      anywhere            !localhost            owner GID match 1337
    0     0 RETURN     all  --  any    lo      anywhere             anywhere             ! owner GID match 1337
    0     0 RETURN     all  --  any    any     anywhere             anywhere             owner GID match 1337
    0     0 RETURN     all  --  any    any     anywhere             localhost
    6   360 ISTIO_REDIRECT  all  --  any    any     anywhere             anywhere
```







