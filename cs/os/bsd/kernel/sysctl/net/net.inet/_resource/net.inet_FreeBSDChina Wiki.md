

# [![img](https://wiki.freebsdchina.org/lib/tpl/dokuwiki/images/logo.png)FreeBSDChina Wiki](https://wiki.freebsdchina.org/start)



您在这里: [欢迎访问 FreeBSDChina Wiki !](https://wiki.freebsdchina.org/start) » [用 pkg_cutleaves 来管理软件包](https://wiki.freebsdchina.org/doc/start) » [s](https://wiki.freebsdchina.org/doc/s/start) » [内核 sysctl 含义详解](https://wiki.freebsdchina.org/doc/s/sysctl) » [net.inet](https://wiki.freebsdchina.org/doc/s/sysctl/net.inet)

您的足迹: • [net.inet](https://wiki.freebsdchina.org/doc/s/sysctl/net.inet)

------

doc:s:sysctl:net.inet

### 目录

- [net.inet](https://wiki.freebsdchina.org/doc/s/sysctl/net.inet#netinet)
  - - [net.inet.ip.portrange.*](https://wiki.freebsdchina.org/doc/s/sysctl/net.inet#netinetipportrange)
    - [net.inet.tcp.delayed_ack](https://wiki.freebsdchina.org/doc/s/sysctl/net.inet#netinettcpdelayed_ack)
    - [net.inet.tcp.always_keepalive](https://wiki.freebsdchina.org/doc/s/sysctl/net.inet#netinettcpalways_keepalive)

# net.inet

| net.inet.ip.forwarding                   | 启用网卡之间的 IP 转发（例如作为网关）                    |
| ---------------------------------------- | ---------------------------------------- |
| net.inet.ip.redirect                     | 允许发出 IP 转移报文                             |
| net.inet.ip.ttl                          | IP包的最大TTL值                               |
| net.inet.ip.rtexpire                     | 动态学习的路由默认过期时间                            |
| net.inet.ip.rtminexpire                  | 动态学习路由的最短过期时间                            |
| net.inet.ip.rtmaxcache                   | 系统可缓存的动态学习路由的数量上限                        |
| net.inet.ip.sourceroute                  | 允许转发来源指定 IP 包的路由（启用可能会引入安全隐患）            |
| net.inet.ip.intr_queue_maxlen            | IP输入队列的最大长度                              |
| net.inet.ip.intr_queue_drops             | 从IP输入队列直接丢弃的包计数                          |
| net.inet.ip.stats                        | IP 统计数据 (struct ipstat, netinet/ip_var.h) 主要由 netstat 使用 |
| net.inet.ip.accept_sourceroute           | 允许接受来源指定 IP 包的路由（启用可能会引入安全隐患）            |
| net.inet.ip.keepfaith                    | 允许 FAITH IPv4→IPv6 翻译服务截包                |
| net.inet.ip.gifttl                       | gif接口上的ttl最大值                            |
| net.inet.ip.same_prefix_carp_only        | 禁止在不同网络接口上指定前缀相同的地址                      |
| net.inet.ip.subnets_are_local            | 认定所有子网均为直接连接                             |
| net.inet.ip.random_id_total              | Count of IP IDs created                  |
| net.inet.ip.random_id_collisions         | Count of IP ID collisions                |
| net.inet.ip.random_id_period             | IP ID Array size                         |
| net.inet.ip.mcast.filters                | Per-interface stack-wide source filters  |
| net.inet.ip.mcast.loop                   | Loopback multicast datagrams by default  |
| net.inet.ip.mcast.maxsocksrc             | Max source filters per socket            |
| net.inet.ip.mcast.maxgrpsrc              | Max source filters per group             |
| net.inet.ip.fastforwarding               | Enable fast IP forwarding                |
| net.inet.ip.maxfragpackets               | Maximum number of IPv4 fragment reassembly queue entries |
| net.inet.ip.output_flowtable_size        | number of entries in the per-cpu output flow caches |
| net.inet.ip.maxfragsperpacket            | Maximum number of IPv4 fragments allowed per packet |
| net.inet.ip.fragpackets                  | Current number of IPv4 fragment reassembly queue entries |
| net.inet.ip.check_interface              | Verify packet arrives on correct interface |
| net.inet.ip.random_id                    | Assign random ip_id values               |
| net.inet.ip.sendsourcequench             | Enable the transmission of source quench packets |
| net.inet.ip.process_options              | Enable IP options processing ([LS]SRR, RR, TS) |
| net.inet.icmp.maskrepl                   | Reply to ICMP Address Mask Request packets. |
| net.inet.icmp.stats                      | ICMP统计数据                                 |
| net.inet.icmp.icmplim                    | Maximum number of ICMP responses per second |
| net.inet.icmp.bmcastecho                 |                                          |
| net.inet.icmp.quotelen                   | Number of bytes from original packet to quote in ICMP reply |
| net.inet.icmp.reply_from_interface       | ICMP reply from incoming interface for non-local packets |
| net.inet.icmp.reply_src                  | icmp reply source for non-local packets. |
| net.inet.icmp.icmplim_output             | Enable rate limiting of ICMP responses   |
| net.inet.icmp.log_redirect               | Log ICMP redirects to the console        |
| net.inet.icmp.drop_redirect              | Ignore ICMP redirects                    |
| net.inet.icmp.maskfake                   | Fake reply to ICMP Address Mask Request packets. |
| net.inet.igmp.stats                      |                                          |
| net.inet.igmp.ifinfo                     | Per-interface IGMPv3 state               |
| net.inet.igmp.gsrdelay                   | Rate limit for IGMPv3 Group-and-Source queries in seconds |
| net.inet.igmp.default_version            | Default version of IGMP to run on each interface |
| net.inet.igmp.legacysupp                 | Allow v1/v2 reports to suppress v3 group responses |
| net.inet.igmp.v2enable                   | Enable backwards compatibility with IGMPv2 |
| net.inet.igmp.v1enable                   | Enable backwards compatibility with IGMPv1 |
| net.inet.igmp.sendlocal                  | Send IGMP membership reports for 224.0.0.0/24 groups |
| net.inet.igmp.sendra                     | Send IP Router Alert option in IGMPv2/v3 messages |
| net.inet.igmp.recvifkludge               | Rewrite IGMPv1/v2 reports from 0.0.0.0 to contain subnet address |
| net.inet.tcp.rfc1323                     | Enable rfc1323 (high performance TCP) extensions |
| net.inet.tcp.mssdflt                     | Default TCP Maximum Segment Size         |
| net.inet.tcp.stats                       | TCP statistics (struct tcpstat, netinet/tcp_var.h) |
| net.inet.tcp.keepidle                    | time before keepalive probes begin       |
| net.inet.tcp.keepintvl                   | time between keepalive probes            |
| net.inet.tcp.sendspace                   | Maximum outgoing TCP datagram size       |
| net.inet.tcp.recvspace                   | Maximum incoming TCP datagram size       |
| net.inet.tcp.keepinit                    | time to establish connection             |
| net.inet.tcp.pcblist                     | List of active TCP connections           |
| net.inet.tcp.delacktime                  | Time before a delayed ACK is sent        |
| net.inet.tcp.v6mssdflt                   | Default TCP Maximum Segment Size for IPv6 |
| net.inet.tcp.hostcache.purge             | Expire all entires on next purge run     |
| net.inet.tcp.hostcache.prune             | Time between purge runs                  |
| net.inet.tcp.hostcache.expire            | Expire time of TCP hostcache entries     |
| net.inet.tcp.hostcache.count             | Current number of entries in hostcache   |
| net.inet.tcp.hostcache.bucketlimit       | Per-bucket hash limit for hostcache      |
| net.inet.tcp.hostcache.hashsize          | Size of TCP hostcache hashtable          |
| net.inet.tcp.hostcache.cachelimit        | Overall entry limit for hostcache        |
| net.inet.tcp.read_locking                | Enable read locking strategy             |
| net.inet.tcp.recvbuf_max                 | Max size of automatic receive buffer     |
| net.inet.tcp.recvbuf_inc                 | Incrementor step size of automatic receive buffer |
| net.inet.tcp.recvbuf_auto                | Enable automatic receive buffer sizing   |
| net.inet.tcp.insecure_rst                | Follow the old (insecure) criteria for accepting RST packets |
| net.inet.tcp.ecn.maxretries              | Max retries before giving up on ECN      |
| net.inet.tcp.ecn.enable                  | TCP ECN support                          |
| net.inet.tcp.abc_l_var                   | Cap the max cwnd increment during slow-start to this number of segments |
| net.inet.tcp.rfc3465                     | Enable RFC 3465 (Appropriate Byte Counting) |
| net.inet.tcp.rfc3390                     | Enable RFC 3390 (Increasing TCP's Initial Congestion Window) |
| net.inet.tcp.rfc3042                     | Enable RFC 3042 (Limited Transmit)       |
| net.inet.tcp.drop_synfin                 | Drop TCP packets with SYN+FIN set        |
| net.inet.tcp.delayed_ack                 | Delay ACK to try and piggyback it onto a data packet |
| net.inet.tcp.blackhole                   | Do not send RST on segments to closed ports |
| net.inet.tcp.log_in_vain                 | Log all incoming TCP segments to closed ports |
| net.inet.tcp.sendbuf_max                 | Max size of automatic send buffer        |
| net.inet.tcp.sendbuf_inc                 | Incrementor step size of automatic send buffer |
| net.inet.tcp.sendbuf_auto                | Enable automatic send buffer sizing      |
| net.inet.tcp.tso                         | Enable TCP Segmentation Offload          |
| net.inet.tcp.newreno                     | Enable NewReno Algorithms                |
| net.inet.tcp.local_slowstart_flightsize  | Slow start flight size for local networks |
| net.inet.tcp.slowstart_flightsize        | Slow start flight size                   |
| net.inet.tcp.path_mtu_discovery          | Enable Path MTU Discovery                |
| net.inet.tcp.reass.overflows             | Global number of TCP Segment Reassembly Queue Overflows |
| net.inet.tcp.reass.maxqlen               | Maximum number of TCP Segments per individual Reassembly Queue |
| net.inet.tcp.reass.cursegments           | Global number of TCP Segments currently in Reassembly Queue |
| net.inet.tcp.reass.maxsegments           | Global maximum number of TCP Segments in Reassembly Queue |
| net.inet.tcp.sack.globalholes            | Global number of TCP SACK holes currently allocated |
| net.inet.tcp.sack.globalmaxholes         | Global maximum number of TCP SACK holes  |
| net.inet.tcp.sack.maxholes               | Maximum number of TCP SACK holes allowed per connection |
| net.inet.tcp.sack.enable                 | Enable/Disable TCP SACK support          |
| net.inet.tcp.getcred                     | Get the xucred of a TCP connection       |
| net.inet.tcp.inflight.stab               | Inflight Algorithm Stabilization 20 = 2 packets |
| net.inet.tcp.inflight.max                | Upper-bound for TCP inflight window      |
| net.inet.tcp.inflight.min                | Lower-bound for TCP inflight window      |
| net.inet.tcp.inflight.rttthresh          | RTT threshold below which inflight will deactivate itself |
| net.inet.tcp.inflight.debug              | Debug TCP inflight calculations          |
| net.inet.tcp.inflight.enable             | Enable automatic TCP inflight data limiting |
| net.inet.tcp.isn_reseed_interval         | Seconds between reseeding of ISN secret  |
| net.inet.tcp.icmp_may_rst                | Certain ICMP unreachable messages may abort connections in SYN_SENT |
| net.inet.tcp.pcbcount                    | Number of active PCBs                    |
| net.inet.tcp.do_tcpdrain                 | Enable tcp_drain routine for extra help when low on mbufs |
| net.inet.tcp.tcbhashsize                 | Size of TCP control-block hashtable      |
| net.inet.tcp.log_debug                   | Log errors caused by incoming TCP segments |
| net.inet.tcp.minmss                      | Minmum TCP Maximum Segment Size          |
| net.inet.tcp.syncache.rst_on_sock_fail   | Send reset on socket allocation failure  |
| net.inet.tcp.syncache.rexmtlimit         | Limit on SYN/ACK retransmissions         |
| net.inet.tcp.syncache.hashsize           | Size of TCP syncache hashtable           |
| net.inet.tcp.syncache.count              | Current number of entries in syncache    |
| net.inet.tcp.syncache.cachelimit         | Overall entry limit for syncache         |
| net.inet.tcp.syncache.bucketlimit        | Per-bucket hash limit for syncache       |
| net.inet.tcp.syncookies_only             | Use only TCP SYN cookies                 |
| net.inet.tcp.syncookies                  | Use TCP SYN cookies if the syncache overflows |
| net.inet.tcp.timer_race                  | Count of t_inpcb races on tcp_discardcb  |
| net.inet.tcp.per_cpu_timers              | run tcp timers on all cpus               |
| net.inet.tcp.finwait2_timeout            | FIN-WAIT2 timeout                        |
| net.inet.tcp.fast_finwait2_recycle       | Recycle closed FIN_WAIT_2 connections faster |
| net.inet.tcp.always_keepalive            | Assume SO_KEEPALIVE on all TCP connections |
| net.inet.tcp.rexmit_slop                 | Retransmission Timer Slop                |
| net.inet.tcp.rexmit_min                  | Minimum Retransmission Timeout           |
| net.inet.tcp.msl                         | Maximum segment lifetime                 |
| net.inet.tcp.nolocaltimewait             | Do not create compressed TCP TIME_WAIT entries for local connections |
| net.inet.tcp.maxtcptw                    | Maximum number of compressed TCP TIME_WAIT entries |
| net.inet.udp.checksum                    | compute udp checksum                     |
| net.inet.udp.stats                       | UDP statistics (struct udpstat, netinet/udp_var.h) |
| net.inet.udp.maxdgram                    | Maximum outgoing UDP datagram size       |
| net.inet.udp.recvspace                   | Maximum space for incoming UDP datagrams |
| net.inet.udp.pcblist                     | List of active UDP sockets               |
| net.inet.udp.getcred                     | Get the xucred of a UDP connection       |
| net.inet.udp.blackhole                   | Do not send port unreachables for refused connects |
| net.inet.udp.log_in_vain                 | Log all incoming UDP packets             |
| net.inet.sctp.assoclist                  | List of active SCTP associations         |
| net.inet.sctp.stats                      | SCTP statistics (struct sctp_stat)       |
| net.inet.sctp.vtag_time_wait             | Vtag time wait time 0 disables.          |
| net.inet.sctp.nat_friendly_init          | Enable sending of the nat-friendly SCTP option on INITs. |
| net.inet.sctp.enable_sack_immediately    | Enable sending of the SACK-IMMEDIATELY-bit. |
| net.inet.sctp.udp_tunneling_port         | Set the SCTP/UDP tunneling port          |
| net.inet.sctp.udp_tunneling_for_client_enable | Enable SCTP/UDP tunneling for client     |
| net.inet.sctp.mobility_fasthandoff       | Enable SCTP fast handoff                 |
| net.inet.sctp.mobility_base              | Enable SCTP base mobility                |
| net.inet.sctp.default_frag_interleave    | Default fragment interleave level        |
| net.inet.sctp.default_cc_module          | Default congestion control module        |
| net.inet.sctp.log_level                  | Ltrace/KTR trace logging level           |
| net.inet.sctp.max_retran_chunk           | Maximum times an unlucky chunk can be retran'd before assoc abort |
| net.inet.sctp.min_residual               | Minimum residual data chunk in second part of split |
| net.inet.sctp.strict_data_order          | Enforce strict data ordering, abort if control inside data |
| net.inet.sctp.abort_at_limit             | When one-2-one hits qlimit abort         |
| net.inet.sctp.hb_max_burst               | Confirmation Heartbeat max burst         |
| net.inet.sctp.do_sctp_drain              | Should SCTP respond to the drain calls   |
| net.inet.sctp.max_chained_mbufs          | Default max number of small mbufs on a chain |
| net.inet.sctp.abc_l_var                  | SCTP ABC max increase per SACK (L)       |
| net.inet.sctp.nat_friendly               | SCTP NAT friendly operation              |
| net.inet.sctp.auth_disable               | Disable SCTP AUTH function               |
| net.inet.sctp.asconf_auth_nochk          | Disable SCTP ASCONF AUTH requirement     |
| net.inet.sctp.early_fast_retran_msec     | Early Fast Retransmit minimum timer value |
| net.inet.sctp.early_fast_retran          | Early Fast Retransmit with timer         |
| net.inet.sctp.cwnd_maxburst              | Use a CWND adjusting maxburst            |
| net.inet.sctp.cmt_pf                     | CMT PF type flag                         |
| net.inet.sctp.cmt_use_dac                | CMT DAC on/off flag                      |
| net.inet.sctp.nr_sack_on_off             | NR_SACK on/off flag                      |
| net.inet.sctp.cmt_on_off                 | CMT on/off flag                          |
| net.inet.sctp.outgoing_streams           | Default number of outgoing streams       |
| net.inet.sctp.add_more_on_output         | When space wise is it worthwhile to try to add more to a socket send buffer |
| net.inet.sctp.path_rtx_max               | Default maximum of retransmissions per path |
| net.inet.sctp.assoc_rtx_max              | Default maximum number of retransmissions per association |
| net.inet.sctp.init_rtx_max               | Default maximum number of retransmission for INIT chunks |
| net.inet.sctp.valid_cookie_life          | Default cookie lifetime in sec           |
| net.inet.sctp.init_rto_max               | Default maximum retransmission timeout during association setup in msec |
| net.inet.sctp.rto_initial                | Default initial retransmission timeout in msec |
| net.inet.sctp.rto_min                    | Default minimum retransmission timeout in msec |
| net.inet.sctp.rto_max                    | Default maximum retransmission timeout in msec |
| net.inet.sctp.secret_lifetime            | Default secret lifetime in sec           |
| net.inet.sctp.shutdown_guard_time        | Default shutdown guard timer in sec      |
| net.inet.sctp.pmtu_raise_time            | Default PMTU raise timer in sec          |
| net.inet.sctp.heartbeat_interval         | Default heartbeat interval in msec       |
| net.inet.sctp.asoc_resource              | Max number of cached resources in an asoc |
| net.inet.sctp.sys_resource               | Max number of cached resources in the system |
| net.inet.sctp.sack_freq                  | Default SACK frequency                   |
| net.inet.sctp.delayed_sack_time          | Default delayed SACK timer in msec       |
| net.inet.sctp.chunkscale                 | Tuneable for Scaling of number of chunks and messages |
| net.inet.sctp.min_split_point            | Minimum size when splitting a chunk      |
| net.inet.sctp.pcbhashsize                | Tunable for PCB hash table sizes         |
| net.inet.sctp.tcbhashsize                | Tunable for TCB hash table sizes         |
| net.inet.sctp.maxchunks                  | Default max chunks on queue per asoc     |
| net.inet.sctp.maxburst                   | Default max burst for sctp endpoints     |
| net.inet.sctp.peer_chkoh                 | Amount to debit peers rwnd per chunk sent |
| net.inet.sctp.strict_init                | Enable strict INIT/INIT-ACK singleton enforcement |
| net.inet.sctp.loopback_nocsum            | Enable NO Csum on packets sent on loopback |
| net.inet.sctp.strict_sacks               | Enable SCTP Strict SACK checking         |
| net.inet.sctp.ecn_nonce                  | Enable SCTP ECN Nonce                    |
| net.inet.sctp.ecn_enable                 | Enable SCTP ECN                          |
| net.inet.sctp.auto_asconf                | Enable SCTP Auto-ASCONF                  |
| net.inet.sctp.recvspace                  | Maximum incoming SCTP buffer size        |
| net.inet.sctp.sendspace                  | Maximum outgoing SCTP buffer size        |
| net.inet.sctp.getcred                    | Get the ucred of a SCTP connection       |
| net.inet.raw.pcblist                     | List of active raw IP sockets            |
| net.inet.raw.recvspace                   | Maximum space for incoming raw IP datagrams |
| net.inet.raw.maxdgram                    | Maximum outgoing raw IP datagram size    |
| net.inet.accf.unloadable                 | Allow unload of accept filters (not recommended) |
| net.inet.flowtable.stats                 | flowtable statistics                     |
| net.inet.flowtable.nmbflows              | Maximum number of flows allowed          |
| net.inet.flowtable.tcp_expire            | seconds after which to remove flow allocated to a TCP connection. |
| net.inet.flowtable.fin_wait_expire       | seconds after which to remove a flow in FIN_WAIT. |
| net.inet.flowtable.udp_expire            | seconds after which to remove flow allocated to UDP. |
| net.inet.flowtable.syn_expire            | seconds after which to remove syn allocated flow. |
| net.inet.flowtable.enable                | enable flowtable caching.                |
| net.inet.flowtable.debug                 | print debug info.                        |

### net.inet.ip.portrange.*

net.inet.ip.portrange.* 是用来控制 IP 协议中所使用的 port 范围。

传统上，Unix 系统中的前 1024 个端口 【0 - 1023】 是保留给 root 用户使用的。FreeBSD默认遵循这一约定，但也允许拥有 PRIV_NETINET_RESERVEDPORT 特权的进程使用这些端口。这些端口通常称作特权端口或保留端口。余下的端口中，【10000 - 65535】范围的端口是系统预设可以分配给任何进程使用的，其中，【49152 - 65535】这个范围的端口称作高范围端口，应用程序在协议没有明确指出端口时，可要求操作系统在这三种范围中分配。

出于安全考虑，一般情况下会希望在没有明确指定端口时，操作系统提供一个外界无法预测的端口。对于大量访问网络上不同节点的系统来说，由于其端口的使用和回收不是单一的外界节点能够控制的，因此不一定需要使用随机的端口分配。

我们将这些 sysctl 变量分成几个部分来进行讨论：

#### 随机分配端口相关

| net.inet.ip.portrange.randomtime | 在切换为随机分配端口之前顺序分配端口的最短时间 （默认为45） |
| -------------------------------- | ------------------------------- |
| net.inet.ip.portrange.randomcps  | 切换为顺序分配端口之前随机分配最多端口数量 （默认为10）   |
| net.inet.ip.portrange.randomized | 启用端口的随机分配 （默认为1）                |

#### 保留端口

| net.inet.ip.portrange.reservedlow  | 特权端口最小值 （默认为0）    |
| ---------------------------------- | ----------------- |
| net.inet.ip.portrange.reservedhigh | 特权端口最大值 （默认为1023） |

#### 临时分配的端口

| net.inet.ip.portrange.hifirst  | 非特权高范围端口最小值 （默认为49152）  |
| ------------------------------ | ----------------------- |
| net.inet.ip.portrange.hilast   | 非特权高范围端口最大值 （默认为65535）  |
| net.inet.ip.portrange.first    | 自动分配的临时端口最小值 （默认为10000） |
| net.inet.ip.portrange.last     | 自动分配的临时端口最大值 （默认为65536） |
| net.inet.ip.portrange.lowfirst | 自动分配的特权端口最小值 （默认为600）   |
| net.inet.ip.portrange.lowlast  | 自动分配的特权端口最大值 （默认为1023）  |

对于繁忙的 FTP 或 代理服务器 而言，可以根据需要将 net.inet.ip.portrange.first 适当减少以避免端口不够用的情况。

### net.inet.tcp.delayed_ack

TCP 协议有一个特性，就是当收到客户端的数据时，会传回一个 ACK (acknowledgement) 的封包，以确认已收到数据。然而，我们也可以将 ACK 封包和所要回传的资料一起送出。例如，当我使用 telnet 进入系统时，在输入指定时，当我们在键盘上敲打一个字符，系统会送回一个表示已接收到该字符的 ACK 封包，并传回一个含有该字符的封包以在终端机上显示。当 net.inet.tcp.delayed_ack 打开时，系统会将 ACK 和显示该字符的封包一传送，而不需分成二个封包。所以这个选项打开时，可以将封包数量减少一半，以加速网络传输。其它的网络服务，例如，WWW、SMTP、POP3 等也都具有这种特性。

### net.inet.tcp.always_keepalive

当这个选项打开时，系统会定期送出「keepalives」以检查一个 TCP 联机是否中断。在打开的状况下，所有运作的网络程序都会有定时检查联机是否中断的功能，否则只有当应用程序本身支持时才有此功能。这个选项打开的好处是让系统更便于管理网络联机，尤其是当我们系统中常有一些莫名其妙就中断联机的使用者时。例如，当一个使用者利用拨接连到系统时，很可能在完成一个完整的 TCP 联机之前，就因为拨接中断而造成联机异常中断。当然，在某些情况下，也有可能会造成系统误判网络联机已中断而结束这个 TCP 联机。

/data/vhosts/wiki-data/pages/doc/s/sysctl/net.inet.txt · 最后更改: 2010/09/04 09:10 由 delphij

------

### 页面工具

除额外注明的地方外，本维基上的内容按下列许可协议发布： [CC Attribution-Share Alike 4.0 International](http://creativecommons.org/licenses/by-sa/4.0/)

[![CC Attribution-Share Alike 4.0 International](https://wiki.freebsdchina.org/lib/images/license/button/cc-by-sa.png)](http://creativecommons.org/licenses/by-sa/4.0/) [![Donate](https://wiki.freebsdchina.org/lib/tpl/dokuwiki/images/button-donate.gif)](https://www.dokuwiki.org/donate) [![Powered by PHP](https://wiki.freebsdchina.org/lib/tpl/dokuwiki/images/button-php.gif)](https://php.net/) [![Valid HTML5](https://wiki.freebsdchina.org/lib/tpl/dokuwiki/images/button-html5.png)](https://validator.w3.org/check/referer) [![Valid CSS](https://wiki.freebsdchina.org/lib/tpl/dokuwiki/images/button-css.png)](https://jigsaw.w3.org/css-validator/check/referer?profile=css3) [![Driven by DokuWiki](https://wiki.freebsdchina.org/lib/tpl/dokuwiki/images/button-dw.png)](https://dokuwiki.org/)

![img](https://wiki.freebsdchina.org/lib/exe/indexer.php?id=doc%3As%3Asysctl%3Anet.inet&1530016575)

