



# [![img](https://wiki.freebsdchina.org/lib/tpl/dokuwiki/images/logo.png)FreeBSDChina Wiki](https://wiki.freebsdchina.org/start)



您在这里: [欢迎访问 FreeBSDChina Wiki !](https://wiki.freebsdchina.org/start) » [用 pkg_cutleaves 来管理软件包](https://wiki.freebsdchina.org/doc/start) » [s](https://wiki.freebsdchina.org/doc/s/start) » [内核 sysctl 含义详解](https://wiki.freebsdchina.org/doc/s/sysctl) » [security子树：内核安全特性行为控制](https://wiki.freebsdchina.org/doc/s/sysctl/security)

您的足迹: • [net.inet](https://wiki.freebsdchina.org/doc/s/sysctl/net.inet) • [通过 sysctl 进行调优](https://wiki.freebsdchina.org/doc/s/sysctl/tuning) • [compat子树：内核二进制兼容相关的控制](https://wiki.freebsdchina.org/doc/s/sysctl/compat) • [net子树：网络子系统](https://wiki.freebsdchina.org/doc/s/sysctl/net) • [内核 sysctl 含义详解](https://wiki.freebsdchina.org/doc/s/sysctl) • [security子树：内核安全特性行为控制](https://wiki.freebsdchina.org/doc/s/sysctl/security)

------

doc:s:sysctl:security

# security子树：内核安全特性行为控制

security.jail 子树主要控制或查询 jail 子系统的状态和行为

| security.jail.jailed                  | 当前进程是否是 jail 中的？                        |
| ------------------------------------- | --------------------------------------- |
| security.jail.list                    | 当前 jail 列表（用户不应直接操作）                    |
| security.jail.mount_allowed           | jail 中的进程是否可以挂接或卸载对 jail 友好的文件系统        |
| security.jail.chflags_allowed         | jail 中的进程是否可以改变系统文件标志                   |
| security.jail.allow_raw_sockets       | jail 中的 root 是否可以创建原生 socket(例如ping)    |
| security.jail.enforce_statfs          | jail 中的进程可以看到系统中多少文件系统                  |
| security.jail.sysvipc_allowed         | jail 中的进程是否可以使用 System V IPC 原语         |
| security.jail.socket_unixiproute_only | jail 中的进程是否只能创建 UNIX/IPv4/route sockets |
| security.jail.set_hostname_allowed    | jail 中的进程是否可以设置 jail 的主机名               |

security.bsd 子树

| security.bsd.suser_enabled            | uid 0拥有特权                      |
| ------------------------------------- | ------------------------------ |
| security.bsd.unprivileged_proc_debug  | 非特权进程可以使用调试机制                  |
| security.bsd.conservative_signals     | 非特权进程是否不能向已经改变信任状的进程发信号        |
| security.bsd.see_other_gids           | 非特权进程是否能看到隶属其他 gid 的主体/对象      |
| security.bsd.see_other_uids           | 非特权进程是否能看到隶属其他 uid 的主体/对象      |
| security.bsd.unprivileged_read_msgbuf | 非特权进程是否能读取内核消息缓冲               |
| security.bsd.hardlink_check_gid       | 非特权进程是否不能创建隶属其他 gid 的文件的硬连接    |
| security.bsd.hardlink_check_uid       | 非特权进程是否不能创建隶属其他 uid 的文件的硬连接    |
| security.bsd.unprivileged_get_quota   | 非特权进程是否能读取其他 uid 和 gid 的空间配额信息 |

/data/vhosts/wiki-data/pages/doc/s/sysctl/security.txt · 最后更改: 2007/12/16 17:59 由 delphij

------

### 页面工具

除额外注明的地方外，本维基上的内容按下列许可协议发布： [CC Attribution-Share Alike 4.0 International](http://creativecommons.org/licenses/by-sa/4.0/)

[![CC Attribution-Share Alike 4.0 International](https://wiki.freebsdchina.org/lib/images/license/button/cc-by-sa.png)](http://creativecommons.org/licenses/by-sa/4.0/) [![Donate](https://wiki.freebsdchina.org/lib/tpl/dokuwiki/images/button-donate.gif)](https://www.dokuwiki.org/donate) [![Powered by PHP](https://wiki.freebsdchina.org/lib/tpl/dokuwiki/images/button-php.gif)](https://php.net/) [![Valid HTML5](https://wiki.freebsdchina.org/lib/tpl/dokuwiki/images/button-html5.png)](https://validator.w3.org/check/referer) [![Valid CSS](https://wiki.freebsdchina.org/lib/tpl/dokuwiki/images/button-css.png)](https://jigsaw.w3.org/css-validator/check/referer?profile=css3) [![Driven by DokuWiki](https://wiki.freebsdchina.org/lib/tpl/dokuwiki/images/button-dw.png)](https://dokuwiki.org/)

![img](https://wiki.freebsdchina.org/lib/exe/indexer.php?id=doc%3As%3Asysctl%3Asecurity&1530016989)



