



### 查看进程环境变量

`ps eww ${pid}`

因为没有 `/proc/${pid}/environ`





### 查看运行进程cwd： lsof -d cwd



```sh
lsof -a -d cwd -p ${pid}

lsof -a -d cwd -c ${lsof -a -d cwd -c programname}
```



* `lsof -d cwd`

  查看所有进程的cwd

* `lsof -a -d cwd -p ${pid}`

  查看指定进程的cwd

> 需要具有要查看进程的权限，同user或者root



