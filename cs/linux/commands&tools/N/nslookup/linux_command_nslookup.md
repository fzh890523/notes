<<<<<<< HEAD


### 指定端口和server

* cmd模式

  `nslookup -port=5003 baidu.com 127.0.0.1` 向127的5003端口发出baidu的dns解析请求。

* interactive模式

  ```s
  nslookup
  > set port=53
  > server 8.8.8.8
  Default server: 8.8.8.8
  Address: 8.8.8.8#53
  > baidu.com
  Server:		8.8.8.8
  Address:	8.8.8.8#53
  
  Name:	baidu.com
  Address: 220.181.38.251
  Name:	baidu.com
  Address: 220.181.38.148
  ```

  
=======




# 安装



`yum install -y bind-utils`

`apt-get install dns-utils` （有时也可能是 dnsutils）



# 使用

### 指定端口和server

* cmd模式

  `nslookup -port=5003 baidu.com 127.0.0.1` 向127的5003端口发出baidu的dns解析请求。

* interactive模式

  ```s
  nslookup
  > set port=53
  > server 8.8.8.8
  Default server: 8.8.8.8
  Address: 8.8.8.8#53
  > baidu.com
  Server:		8.8.8.8
  Address:	8.8.8.8#53
  
  Name:	baidu.com
  Address: 220.181.38.251
  Name:	baidu.com
  Address: 220.181.38.148
  ```

* ptr

  `nslookup 8.8.8.8`

>>>>>>> 1122cdc (re-init 20240121)
