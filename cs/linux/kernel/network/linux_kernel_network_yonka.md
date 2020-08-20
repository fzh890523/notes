

# TCP

## 问题处理

### 持续 `FIN_WAIT1` 状态连接怎么“清除”

背景：

* 对端（client）hang了
* 本端关闭不再listen，重启发现端口占用
* `netstat -atnp | grep xxx` 发现有`FIN_WAIT1`状态连接消不掉

处理办法：

ref https://serverfault.com/questions/7689/how-do-i-get-rid-of-sockets-in-fin-wait1-state

```sh
# record what tcp_max_orphans's current value
original_value=$(cat /proc/sys/net/ipv4/tcp_max_orphans)

#set the tcp_max_orphans to 0 temporarily
echo 0 > /proc/sys/net/ipv4/tcp_max_orphans

# watch /var/log/messages
# it will split out "kernel: TCP: too many of orphaned sockets"
# it won't take long for the connections to be killed

# restore the value of tcp_max_orphans whatever it was before. 
echo $original_value > /proc/sys/net/ipv4/tcp_max_orphans

# verify with 
netstat -an|grep FIN_WAIT1
```

