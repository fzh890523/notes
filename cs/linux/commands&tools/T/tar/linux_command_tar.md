



tar命令几个常用参数解读：

* `-c`或`--create`：建立新的备份文件； 
<<<<<<< HEAD
* `-x`或`--extract`或`--get`：从备份文件中还原文件；
* `-t`或`--list`：列出备份文件的内容； 
* `-f<备份文件>`或`--file=<备份文件>`：指定备份文件； 
* `-v`或`--verbose`：显示指令执行过程；
* `-z`或`--gzip`或`--ungzip`：通过gzip指令处理备份文件； 
=======

  `czf -` 输出到标准输出，类似 `tar czf - xx.txt > xx.tar.gz`

* `-x`或`--extract`或`--get`：从备份文件中还原文件；

* `-t`或`--list`：列出备份文件的内容； 

* `-f<备份文件>`或`--file=<备份文件>`：指定备份文件； 

* `-v`或`--verbose`：显示指令执行过程；

* `-z`或`--gzip`或`--ungzip`：通过gzip指令处理备份文件； 

>>>>>>> 1122cdc (re-init 20240121)
* `-j`：支持bzip2解压文件；



```sh
tar czf xxx.tar.gz xx_dir --exclude=xx_dir/logs/
```



```sh
tar czf hsf-guide-server.tar.gz -C /Users/xxx/target/ release
# 这样就不至于出现解压出来是 /Users/xxx/target/release 的问题
# 直接 tar czf hsf-guide-server.tar.gz /Users/xxx/target/release 会这样
# -C == new_cwd
```



```sh
tar tzf xx.tar.gz
# 查看原文件内容目录信息
```





* output to pipe： `-f -`

  如： `tar -cf - something > xx.tar`

  对于解压 `-O`/`--to-stdout`：

  `tar xzf xx.tar -O > something` or `tar -O -xzf xx.tar > something`

  也能串联：

  `tar -cf - something | tar xzf - -O > something1`



