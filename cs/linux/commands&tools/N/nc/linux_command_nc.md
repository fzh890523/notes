

* `-p <port>` 指定端口

  `nc -l -p 8080`

  > 有些版本支持 `nc -l <port>` 直接listen，有些不行，把后面的`<port>`理解成host还是啥

* `-s <ip>` 指定ip

* `-l` listen

* `-v`/`-vv` verbose模式

  比如会输出：

  ```sh
  listening on [192.168.1.5] 18888 ...
  connect to [192.168.1.5] from xxx [192.168.1.5] 42230  # incoming conn
  ```

   





* 默认（bsd/Mac？）对EOF仍然继续等待

  如： `nc {ip} {port} < xxx.txt` 表现为发完还hang。

  需要`-c`参数显式。。，如`nc -c {ip} {port} < xxx.txt`

  不过这样有新的问题，就是没发完就RESET了连接，特么。。。

  https://superuser.com/questions/115553/netcat-on-mac-os-x

  ```
  nc on MacOS has too many bugs, and Apple did none patch for years. the netcat from homebrew is a very low version. use ncat from nmap instead
  ```

  ```sh
  rm -f /usr/local/bin/nc && ln -s /usr/local/bin/../Cellar/nmap/7.70/bin/ncat /usr/local/bin/nc
  # 然后就好了， -c参数也不需要了
  ```

  





```sh
nc -l 1234 > filename.out

nc host.example.com 1234 < filename.in
```



