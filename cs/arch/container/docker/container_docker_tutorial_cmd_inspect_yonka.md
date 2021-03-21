

```sh
docker inspect --format '{{.State.Pid}}' c1b7f8c6f85c  # 输出docker容器的进程号
82973

sudo ls -l /proc/82973/ns/
total 0
lrwxrwxrwx 1 root root 0 Oct 24 19:06 cgroup -> 'cgroup:[4026531835]'
lrwxrwxrwx 1 root root 0 Oct 24 19:06 ipc -> 'ipc:[4026532783]'
lrwxrwxrwx 1 root root 0 Oct 24 19:06 mnt -> 'mnt:[4026532781]'
lrwxrwxrwx 1 root root 0 Oct 24 19:06 net -> 'net:[4026532786]'
lrwxrwxrwx 1 root root 0 Oct 24 19:06 pid -> 'pid:[4026532784]'
lrwxrwxrwx 1 root root 0 Oct 24 19:06 pid_for_children -> 'pid:[4026532784]'
lrwxrwxrwx 1 root root 0 Oct 24 19:06 user -> 'user:[4026531837]'
lrwxrwxrwx 1 root root 0 Oct 24 19:06 uts -> 'uts:[4026532782]'
```





## history



```sh
docker history a43f156372a7
IMAGE               CREATED             CREATED BY                                      SIZE                COMMENT
a43f156372a7        7 weeks ago         /bin/sh -c #(nop)  ENTRYPOINT ["/usr/local/bi   0 B
<missing>           7 weeks ago         /bin/sh -c useradd -m --uid 1337 istio-proxy    447.9 kB
<missing>           7 weeks ago         /bin/sh -c #(nop) COPY file:35665fb734764cccd   2.906 kB
```

