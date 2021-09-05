

* `netstat -b` 查看在使用网络的程序

  ```sh
   netstat -b
  
  活动连接
  
    协议  本地地址          外部地址        状态
    TCP    127.0.0.1:51710        kubernetes:51711       ESTABLISHED
   [goland64.exe]
    TCP    127.0.0.1:51711        kubernetes:51710       ESTABLISHED
   [goland64.exe]
   ...
    [chrome.exe]
    TCP    192.168.1.xx:54954     113.96.202.105:http    ESTABLISHED
    ...
  ```

* `netstat -n` 查看连接

  ```sh
   netstat -n
  
  活动连接
  
    协议  本地地址          外部地址        状态
    TCP    127.0.0.1:51710        127.0.0.1:51711        ESTABLISHED
    TCP    127.0.0.1:51711        127.0.0.1:51710        ESTABLISHED
    ...
  ```

  



