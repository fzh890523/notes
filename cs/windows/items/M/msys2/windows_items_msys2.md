
https://www.msys2.org/wiki/MSYS2-installation/



## 运行环境改造（至少对其git bash的mingw）



* `HOME` 使用windows user home

  `/etc/nsswitch.conf`下 `db_home: cygwin desc` 改为 `db_home: windows`

* `PATH` 使用windows path

  * `<INSTALL_PATH>/mingw64.ini or mingw32.ini or msys2.int`中的`#MSYS2_PATH_TYPE=inherit`改为`MSYS2_PATH_TYPE=inherit`也即去掉注释 - **直接执行*.exe才有效**

  * 用`msys2_shell.cmd` 启动的（比如自带的各个link），在link启动参数里加上`-full-path`

    > 这个似乎不读那些ini

* en环境： `~/.bashrc` 加入 `export LANG='en.UTF-8'`



## terminal



### windows terminal集成

参考： https://www.msys2.org/docs/terminals/

