









### Resource temporarily unavailable



```sh
bash: fork: Resource temporarily unavailable
# and etc
```



```sh
# ref: https://gist.github.com/lucasdavila/f382f5f5eb4fc4e83cf5

# see the current limits
$ sysctl -a | grep maxproc

# increase it
$ sudo sysctl -w kern.maxproc=xxxx
$ sudo sysctl -w kern.maxprocperuid=xxx

# run at startup
$ sudo vim /etc/sysctl.conf

kern.maxproc=xxxx
kern.maxprocperuid=xxxx

$ sudo launchctl limit
$ sudo launchctl limit maxproc <maxprocperuid> <maxproc>
$ sudo launchctl limit maxfiles 512 unlimited

# run at startup
$ sudo vim /etc/launchd.conf

limit maxproc <maxprocperuid> <maxproc>
limit maxfiles 512 unlimited

# kern.maxproc: 1064 -> 2128
# kern.maxprocperuid: 709 -> 1418
```

