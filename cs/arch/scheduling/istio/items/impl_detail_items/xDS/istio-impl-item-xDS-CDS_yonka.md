

# inbound clusters



## default passthrough clusters



这些cluster会在build clusters时加上，它们是`ORIGINAL_DST`（注意不是`STATIC`）并且它们会加上额外的配置： `upstream_bind_config.source_address: 127.0.0.6:0`

> srcIP:srcPort->dstIP:dstPort 这个stream/conn bind到 127.0.0.6:newSrcPort->dstIP:dstPort
>
> yonka： 为什么要这么做呢？ 让协议栈自行确定src不行吗？
>
> > 不行，因为目前流量拦截策略，只有 to localhost 的不拦截，而 orig_dst 的话还是尽量保留源信息，所以 dest 肯定不是localhost了，所以这里用了个特殊source来加 iptables策略把这部分流量exclude了 







```json
    {
     "version_info": "2021-01-16T12:09:20Z/2",
     "cluster": {
      "name": "InboundPassthroughClusterIpv4",
      "type": "ORIGINAL_DST",
      "connect_timeout": "1s",
      "lb_policy": "CLUSTER_PROVIDED",
      "circuit_breakers": {
       "thresholds": [
        {
         "max_connections": 102400,
         "max_retries": 1024
        }
       ]
      },
      "upstream_bind_config": {
       "source_address": {
        "address": "127.0.0.6",
        "port_value": 0
       }
      }
     },
     "last_updated": "2021-01-18T11:20:44.814Z"
    },
```







