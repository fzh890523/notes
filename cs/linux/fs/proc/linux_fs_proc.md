ref： [man5-proc.5](http://man7.org/linux/man-pages/man5/proc.5.html)



# 介绍



* 伪FS
* 提供访问内核数据的接口
* 大部分只读，少数可写
* 一般挂载在`/proc/`
* 系统自动挂载，也可以手动挂载 `mount -t proc proc /proc1`



## 挂载选项

* `hidepid=${n}` (since 3.3)

  控制对`/proc/[pid]`的访问，取值如下：

  * `0` 每个用户都可以访问所有的`/proc/[pid]`。 这个是一贯行为，也是默认值

  * `1` 只能访问自己（所属）的`/proc/[pid]`（不过还是都可见的）

    一些敏感文件如 `/proc/[pid]/cmdline`、`/proc/[pid]/status`等对其他用户不可见，于是其他用户无法通过这些来获知是否在执行某程序...

  * `2` 和1类似，不过连目录本身都对其他用户不可见

    也即无法通过这种方式来查看哪些进程（pid）存在

* `gid=${gid)` (since 3.3)

  指定豁免于`hidepid`参数的用户组的id，对于该组用户，等同于`hidepid=0`。



## 文件、目录



### /proc/[pid]

* 一般属主为进程的 euid、egid；如果进程的`dumpable`属性不为1的话，为`root:root`

* `dumpable`属性可以通过以下方式改变：

  * prctl的`PR_SET_DUMPABLE`操作
  * `/proc/sys/fs/suid_dumpable`

  把该属性重设为1会使得`/proc/[pid]/*`这些文件的属主变为进程的真实uid和真实gid



#### /proc/[pid]/attr

* 该目录下文件提供访问安全模块的接口，对这些文件（按序）读写可以设置安全相关的属性
* 用于支持SELinux，设计的也可以支持其他安全模块
* 只有当内核配置有`CONFIG_SECURITY`时才存在该目录



```shell
ls -l /proc/20315/ | grep attr
dr-xr-xr-x 2 root root 0 11月 18 14:41 attr

ls -l /proc/20315/attr/
总用量 0
-rw-rw-rw- 1 root root 0 11月 18 14:41 current
-rw-rw-rw- 1 root root 0 11月 18 14:41 exec
-rw-rw-rw- 1 root root 0 11月 18 14:41 fscreate
-rw-rw-rw- 1 root root 0 11月 18 14:41 keycreate
-r--r--r-- 1 root root 0 11月 18 14:41 prev
-rw-rw-rw- 1 root root 0 11月 18 14:41 sockcreate

# 看起来不是直接读
cat /proc/20315/attr/*
cat: /proc/20315/attr/current: 无效的参数
cat: /proc/20315/attr/exec: 无效的参数
cat: /proc/20315/attr/fscreate: 无效的参数
cat: /proc/20315/attr/keycreate: 无效的参数
cat: /proc/20315/attr/prev: 无效的参数
cat: /proc/20315/attr/sockcreate: 无效的参数
```







##### /proc/[pid]/attr/current (since Linux 2.6.0)

* 该文件内容表示： 当前进程的安全属性
* 在2.6.11之前不能用来`set`（拒绝写操作），从2.6.11开始去除此限制开始支持通过（写）该文件来做`set`操作（前提是安全策略允许）
* 在2.6.28之前不允许多线程进程的线程通过该文件来`set`，因为可能出现各线程安全上下文不一致的情况， 从2.6.28开始有条件去除此限制： 新的策略的许可需要是老策略的子集





##### /proc/[pid]/attr/exec (since Linux 2.6.0)

表示通过`execve`来为进程设置的属性

* 对于SELinux，进程只能设置自己的该文件...



##### /proc/[pid]/attr/fscreate (since Linux 2.6.0)

* 表示通过`open`、`mkdir`、`symlink`、`mknod`来为文件设置的属性
* SELinux通过该文件来支持*创建安全状态的文件*，避免在创建和设置属性之间的gap带来的安全风险（说白了就是原子性）
* 对于SELinux，该属性会在`execve`时重置
* 对于SELinux，进程只能设置自己的该文件...



##### /proc/[pid]/attr/keycreate (since Linux 2.6.18)

* 如果进程写入安全上下文到该文件的话，之后创建的key文件都会带上该上下文



##### /proc/[pid]/attr/prev (since Linux 2.6.0)

* 记录了进程最近一次`execve`之前的安全上下文，也即 `/proc/[pid]/attr/current`的前一个值



##### /proc/[pid]/attr/socketcreate (since Linux 2.6.18)

* 如果进程写入安全上下文到该文件的话，之后创建的socket都会带上该上下文



#### /proc/[pid]/autogroup (since Linux 2.6.38)

* 参见 `sched` （= =）



#### /proc/[pid]/auxv (since 2.6.0-test7)

* 包含在exec时传递给进程的ELF解释器信息


* 内容的格式是：
  * 每个条目： 一个无符号long的ID + 一个无符号long的value
  * 最后一个条目两个均为0
* 对该文件的访问权限由 ptrace access mode `PTRACE_MODE_READ_FSCREDS` check （的结果）决定



#### /proc/[pid]/cgroup (since Linux 2.6.24)

* 参见 `cgroups` （= =）



#### /proc/[pid]/clear_refs (since Linux 2.6.22)



#### /proc/[pid]/cmdline



#### /proc/[pid]/comm (since Linux 2.6.33)



#### /proc/[pid]/coredump_filter (since Linux 2.6.23)



#### /proc/[pid]/cpuset (since Linux 2.6.12)



#### /proc/[pid]/cwd



#### /proc/[pid]/environ



#### /proc/[pid]/exe



#### /proc/[pid]/fd/



#### /proc/[pid]/fdinfo/ (since Linux 2.6.22)





#### /proc/[pid]/gid_map (since Linux 3.5)



#### /proc/[pid]/io (since kernel 2.6.20)



#### /proc/[pid]/limits (since Linux 2.6.24)





#### /proc/[pid]/map_files/ (since kernel 3.3)



#### /proc/[pid]/maps



#### /proc/[pid]/mem



#### /proc/[pid]/mountinfo (since Linux 2.6.26)



#### /proc/[pid]/mounts (since Linux 2.4.19)



#### /proc/[pid]/mountstats (since Linux 2.6.17)



#### /proc/[pid]/net (since Linux 2.6.25)



#### proc/[pid]/ns/ (since Linux 3.0)



#### /proc/[pid]/numa_maps (since Linux 2.6.14)



#### /proc/[pid]/oom_adj (since Linux 2.6.11)



#### /proc/[pid]/oom_score (since Linux 2.6.11)



#### /proc/[pid]/oom_score_adj (since Linux 2.6.36)



#### /proc/[pid]/pagemap (since Linux 2.6.25)



#### /proc/[pid]/personality (since Linux 2.6.28)



#### /proc/[pid]/root



#### /proc/[pid]/seccomp (Linux 2.6.12 to 2.6.22)



#### /proc/[pid]/setgroups (since Linux 3.19)



#### /proc/[pid]/smaps (since Linux 2.6.14)



#### /proc/[pid]/stack (since Linux 2.6.29)



#### /proc/[pid]/stat



#### /proc/[pid]/status



#### /proc/[pid]/syscall (since Linux 2.6.27)



#### /proc/[pid]/task (since Linux 2.6.0-test6)



##### /proc/[pid]/task/[tid]/children (since Linux 3.5)



#### /proc/[pid]/timers (since Linux 3.10)



#### /proc/[pid]/timerslack_ns (since Linux 4.6)



#### /proc/[pid]/uid_map, /proc/[pid]/gid_map (since Linux 3.5)



#### /proc/[pid]/wchan (since Linux 2.6.0)









### /proc/apm



### /proc/buddyinfo



### /proc/bus



#### /proc/bus/pccard



##### /proc/bus/pccard/drivers



#### /proc/bus/pci



##### /proc/bus/pci/devices





### /proc/cgroups (since Linux 2.6.24)



### /proc/cmdline



### /proc/config.gz (since Linux 2.6)



### /proc/crypto



### /proc/cpuinfo



### /proc/devices



### /proc/diskstats (since Linux 2.5.69)



### /proc/dma



### /proc/driver



### /proc/execdomains



### /proc/fb



### /proc/filesystems



### /proc/fs



### /proc/ide



### /proc/interrupts



### /proc/iomem



### /proc/ioports



### /proc/kallsyms (since Linux 2.5.71)



### /proc/kcore



### /proc/keys (since Linux 2.6.10)



### /proc/key-users (since Linux 2.6.10)



### /proc/kmsg



### /proc/kpagecount (since Linux 2.6.25)



### /proc/kpageflags (since Linux 2.6.25)



### /proc/ksyms (Linux 1.1.23–2.5.47)



### /proc/loadavg



### /proc/locks



### /proc/malloc (only up to and including Linux 2.2)



### /proc/meminfo



### /proc/modules



### /proc/mounts



### /proc/mtrr









### /proc/net



#### /proc/net/arp



#### /proc/net/dev



#### /proc/net/dev_mcast



#### /proc/net/igmp



#### /proc/net/rarp



#### /proc/net/raw



#### /proc/net/snmp



#### /proc/net/tcp



#### /proc/net/udp



#### /proc/net/unix





### /proc/partitions



### /proc/pci



### /proc/profile (since Linux 2.4)



### /proc/scsi

#### /proc/scsi/scsi



#### /proc/scsi/[drivername]



### /proc/self



### /proc/slabinfo

### /proc/stat

### /proc/swaps



### /proc/sys



#### /proc/sys/abi (since Linux 2.4.10)



#### /proc/sys/debug



#### /proc/sys/dev



#### /proc/sys/fs



##### /proc/sys/fs/binfmt_misc



##### /proc/sys/fs/dentry-state (since Linux 2.2)



##### /proc/sys/fs/dir-notify-enable



##### /proc/sys/fs/dquot-max



##### /proc/sys/fs/dquot-nr



##### /proc/sys/fs/epoll (since Linux 2.6.28)



##### /proc/sys/fs/file-max



##### /proc/sys/fs/file-nr



##### /proc/sys/fs/inode-max (only present until Linux 2.2)



##### /proc/sys/fs/inode-nr



##### /proc/sys/fs/inode-state



##### /proc/sys/fs/inotify (since Linux 2.6.13)



##### /proc/sys/fs/lease-break-time



##### /proc/sys/fs/leases-enable



##### /proc/sys/fs/mount-max (since Linux 4.9)



##### /proc/sys/fs/mqueue (since Linux 2.6.6)



##### /proc/sys/fs/nr_open (since Linux 2.6.25)



##### /proc/sys/fs/overflowgid and /proc/sys/fs/overflowuid



##### /proc/sys/fs/pipe-max-size (since Linux 2.6.35)



##### /proc/sys/fs/pipe-user-pages-hard (since Linux 4.5)



##### /proc/sys/fs/pipe-user-pages-soft (since Linux 4.5)



##### /proc/sys/fs/protected_hardlinks (since Linux 3.6)



##### /proc/sys/fs/protected_symlinks (since Linux 3.6)



##### /proc/sys/fs/suid_dumpable (since Linux 2.6.13)



##### /proc/sys/fs/super-max



##### /proc/sys/fs/super-nr



#### /proc/sys/kernel



##### /proc/sys/kernel/acct



##### /proc/sys/kernel/auto_msgmni (Linux 2.6.27 to 3.18)



##### /proc/sys/kernel/cap_last_cap (since Linux 3.2)



##### /proc/sys/kernel/cap-bound (from Linux 2.2 to 2.6.24)



##### /proc/sys/kernel/core_pattern



##### /proc/sys/kernel/core_pipe_limit



##### /proc/sys/kernel/core_uses_pid



##### /proc/sys/kernel/ctrl-alt-del



##### /proc/sys/kernel/dmesg_restrict (since Linux 2.6.37)



##### /proc/sys/kernel/domainname and /proc/sys/kernel/hostname



##### /proc/sys/kernel/hotplug



##### /proc/sys/kernel/htab-reclaim (before Linux 2.4.9.2)



##### /proc/sys/kernel/keys/*



##### /proc/sys/kernel/kptr_restrict (since Linux 2.6.38)



##### /proc/sys/kernel/l2cr



##### /proc/sys/kernel/modprobe



##### /proc/sys/kernel/modules_disabled (since Linux 2.6.31)



##### /proc/sys/kernel/msgmax (since Linux 2.2)



##### /proc/sys/kernel/msgmni (since Linux 2.4)



##### /proc/sys/kernel/msgmnb (since Linux 2.2)



##### /proc/sys/kernel/ngroups_max (since Linux 2.6.4)



##### /proc/sys/kernel/ostype and /proc/sys/kernel/osrelease



##### /proc/sys/kernel/overflowgid and /proc/sys/kernel/overflowuid



##### /proc/sys/kernel/panic



##### /proc/sys/kernel/panic_on_oops (since Linux 2.5.68)



##### /proc/sys/kernel/pid_max (since Linux 2.5.34)



##### /proc/sys/kernel/powersave-nap (PowerPC only)



##### /proc/sys/kernel/printk



##### /proc/sys/kernel/pty (since Linux 2.6.4)



###### /proc/sys/kernel/pty/max

###### /proc/sys/kernel/pty/nr



##### /proc/sys/kernel/random



###### /proc/sys/kernel/random/uuid (since Linux 2.4)

##### /proc/sys/kernel/randomize_va_space (since Linux 2.6.12)

##### /proc/sys/kernel/real-root-dev



##### /proc/sys/kernel/reboot-cmd (Sparc only)



##### /proc/sys/kernel/rtsig-max



##### /proc/sys/kernel/rtsig-nr



##### /proc/[pid]/sched_autogroup_enabled (since Linux 2.6.38)



##### /proc/sys/kernel/sched_child_runs_first (since Linux 2.6.23)



##### /proc/sys/kernel/sched_rr_timeslice_ms (since Linux 3.9)



##### /proc/sys/kernel/sched_rt_period_us (since Linux 2.6.25)



##### /proc/sys/kernel/sched_rt_runtime_us (since Linux 2.6.25)



##### /proc/sys/kernel/sem (since Linux 2.4)



##### /proc/sys/kernel/sg-big-buff



##### /proc/sys/kernel/shm_rmid_forced (since Linux 3.1)



##### /proc/sys/kernel/shmall (since Linux 2.2)



##### /proc/sys/kernel/shmmax (since Linux 2.2)





##### /proc/sys/kernel/shmmni (since Linux 2.4)



##### /proc/sys/kernel/sysctl_writes_strict (since Linux 3.16)



##### /proc/sys/kernel/sysrq



##### /proc/sys/kernel/version



##### /proc/sys/kernel/threads-max (since Linux 2.3.11)



##### /proc/sys/kernel/yama/ptrace_scope (since Linux 3.5)



##### /proc/sys/kernel/zero-paged (PowerPC only)







#### /proc/sys/net



##### /proc/sys/net/core/bpf_jit_enable



##### /proc/sys/net/core/somaxconn







#### /proc/sys/proc



#### /proc/sys/sunrpc



#### /proc/sys/user (since Linux 4.9)



#### /proc/sys/vm



##### /proc/sys/vm/admin_reserve_kbytes (since Linux 3.10)



##### /proc/sys/vm/compact_memory (since Linux 2.6.35)



##### /proc/sys/vm/drop_caches (since Linux 2.6.16)



##### /proc/sys/vm/legacy_va_layout (since Linux 2.6.9)



##### /proc/sys/vm/memory_failure_early_kill (since Linux 2.6.32)



##### /proc/sys/vm/memory_failure_recovery (since Linux 2.6.32)



##### /proc/sys/vm/oom_dump_tasks (since Linux 2.6.25)

##### /proc/sys/vm/oom_kill_allocating_task (since Linux 2.6.24)



##### /proc/sys/vm/overcommit_kbytes (since Linux 3.14)



##### /proc/sys/vm/overcommit_memory



##### /proc/sys/vm/overcommit_ratio (since Linux 2.6.0)



##### /proc/sys/vm/panic_on_oom (since Linux 2.6.18)



##### /proc/sys/vm/swappiness



##### /proc/sys/vm/user_reserve_kbytes (since Linux 3.10)







### /proc/sysrq-trigger (since Linux 2.4.21)





### /proc/sysvipc









### /proc/thread-self (since Linux 3.17)





### /proc/timer_list (since Linux 2.6.21)



### /proc/timer_stats (from  Linux 2.6.21 until Linux 4.10)



### /proc/tty



### /proc/uptime



### /proc/version





### /proc/vmstat (since Linux 2.6.0)







### /proc/zoneinfo (since Linux 2.6.13)























































