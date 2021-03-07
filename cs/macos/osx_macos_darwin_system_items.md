



# process management



## env



### 查看运行进程环境变量： ps eww ${pid}



### 查看运行进程cwd： lsof -d cwd



```sh
lsof -a -d cwd -p ${pid}

lsof -a -d cwd -c ${lsof -a -d cwd -c programname}
```









### 











