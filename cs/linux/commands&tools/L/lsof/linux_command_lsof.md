

# usage



## 通过lsof找到/恢复已删除文件



1. 找到

   ```shell
   lsof -nP | grep '(deleted)'

   # 在linux上还可以
   find /proc/*/fd -ls | grep  '(deleted)'
   ```

2. 恢复

   ```shell
   cp /proc/$pid/fd/$fd ${target_path}
   ```

   > Copying from `/proc/*PID*/fd/` only works if the process currently has the file open. It can't do anything about a file that the process had open at some point in the past but no longer does. A program has no need to keep its configuration file open: once it's read its configuration, it closes the file. You aren't going to recover anything that way. The file you're attempting to recover is an unrelated file; it's a network socket that the process is listening on, and you can't read data from a socket like this, not that this would do you any good.
   >
   > `gcore 30495` [as suggested by Mark Plotnick](https://unix.stackexchange.com/questions/268247/recover-files-if-still-being-used-by-a-process#comment464904_268247) would dump a memory image of the process in the file `core.30495`. You could try to sift through that memory image, but don't get your hopes up: there's a good chance that the process has parsed the configuration file and reused whatever memory it stored the file in for other purposes. It's even likely that the whole configuration file was never entirely in memory, only piece by piece and each piece overwrote the previous one.

   ​







