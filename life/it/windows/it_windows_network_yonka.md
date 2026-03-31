

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

  

### 多网卡时设置默认路由优先级

* 接口跃点
* 网关跃点： windows似乎对DHCP下发的自动设置为50，而手动设置的默认路由（常见于手动设置IP时填入）自动设置为0

最后`route print -4`查看到的默认路由的跃点数等于上述两个相加



案例：

* WIFI： DHCP
* 以太网： 手动设置IP（和网关）

期望： 默认路由走WIFI

开始设置WIFI接口跃点为50，以太网为80，结果查看到默认路由里WIFI的是100、以太网的是80

然后直接把以太网的接口跃点改成180了，效果就符合预期了（100 VS 180）

