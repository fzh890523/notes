## out of socker memory



Ref:

* [The "Out of socket memory" error](http://blog.tsunanet.net/2011/03/out-of-socket-memory.html) or [local](resource/Tsuna's blog_ The _Out of socket memory_ error.html)




### 可能的原因



So **two conditions that can trigger this "Out of socket memory" error**:

* There are "too many" orphan sockets (most common).
* The socket already has the minimum amount of memory and we can't give it more because TCP is already using more than its limit.



### 相关源码



[`net/ipv4/tcp_timer.c`](http://git.kernel.org/?p=linux/kernel/git/torvalds/linux-2.6.git;a=blob;f=net/ipv4/tcp_timer.c;hb=v2.6.38-rc8#l82)

```c
static int tcp_out_of_resources(struct sock *sk, int do_reset)
{
        struct tcp_sock *tp = tcp_sk(sk);
        int shift = 0;
        /* If peer does not open window for long time, or did not transmit
         * anything for long time, penalize it. */
        if ((s32)(tcp_time_stamp - tp->lsndtime) > 2*TCP_RTO_MAX || !do_reset)
                shift++;
        /* If some dubious ICMP arrived, penalize even more. */
        if (sk->sk_err_soft)
                shift++;
        if (tcp_too_many_orphans(sk, shift)) {
                if (net_ratelimit())
                        printk(KERN_INFO "Out of socket memory\n");
```



[`include/net/tcp.h`](http://git.kernel.org/?p=linux/kernel/git/torvalds/linux-2.6.git;a=blob;f=include/net/tcp.h;hb=v2.6.38-rc8#l268)

```c
static inline bool tcp_too_many_orphans(struct sock *sk, int shift)
{
        struct percpu_counter *ocp = sk->sk_prot->orphan_count;
        int orphans = percpu_counter_read_positive(ocp);
        if (orphans << shift > sysctl_tcp_max_orphans) {
                orphans = percpu_counter_sum_positive(ocp);
                if (orphans << shift > sysctl_tcp_max_orphans)
                        return true;
        }
        if (sk->sk_wmem_queued > SOCK_MIN_SNDBUF &&
            atomic_long_read(&tcp_memory_allocated) > sysctl_tcp_mem[2])
                return true;
        return false;
}
```



### 排查



#### tcp mem配置和使用排查



* `cat /proc/sys/net/ipv4/tcp_mem`

  输出类似：

  ```shell
  3093984 4125312 6187968
  ```

* `sysctl -a | grep tcp_mem`

* `cat /proc/net/sockstat`

  输出类似：

  ```shell
  sockets: used 14565
  TCP: inuse 35938 orphan 21564 tw 70529 alloc 35942 mem 1894
  UDP: inuse 11 mem 3
  UDPLITE: inuse 0
  RAW: inuse 0
  FRAG: inuse 0 memory 0
  ```

  注意那个 **mem**

> 配置和当前值的单位都是页。



关于该参数，更多参考 《linux_io_socket_usage_option&arg&conf.md》



#### orphan sockets排查

* `cat /proc/sys/net/ipv4/tcp_max_orphans`

  输出类似：

  ```shell
  65536
  ```

* `cat /proc/net/sockstat | grep orphans`

  ...



注意：

```c
if (orphans << shift > sysctl_tcp_max_orphans)
```

不是直接拿 `当前orhans` 和 `sysctl_tcp_max_orphans` 比较，而是...

> 这个shift变量的值暂时还没查找怎么来的，但ref文里提到是 **0-2** ，所以从安全角度来看，实际orphans应小于max的四分之一





















