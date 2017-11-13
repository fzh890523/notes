

# tutorial



## 参数



* 指定输出的列/格式 `-o`

  如： `ps -o pcpu,pid,spid`

* 显示线程 `-T`

* 指定显示某个进程 `-p`

  如： `ps -p 1234`

* 显示所有进程 `-e`/`-A`

* 指定排序字段 `--sort`

  如： `--sort=+pid,-pcpu` 表示 按pid排序，同pid的按pcpu倒序排序



## 字段



### pcpu / %cpu

cpu占比，但注意是根据 `cputime / uptime` 计算而来，所以得出的是某进程启动以来的cpu消耗情况，而不能反应该进程当前的cpu消耗情况。

























