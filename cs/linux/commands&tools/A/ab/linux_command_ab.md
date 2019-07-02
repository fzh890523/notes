



一般是`httpd-tools`提供



# install



```sh
apt-file find /usr/bin/ab  # ubuntu某版本下没找到 httpd-tools
aptitude install -y apache2-utils
```



# 问题



## http 1.1支持



没办法显式指定发1.1的包，而当server不支持1.0(返回426告知升级)时，会无法继续。。

```sh
HTTP/1.1 426 Upgrade Required
date: Fri, 17 May 2019 03:11:12 GMT
server: envoy
connection: close
content-length: 0
```





# usage



```sh
       ab  [  -A auth-username:password ] [ -b windowsize ] [ -B
       local-address ] [ -c concurrency ] [ -C cookie-name=value
       ]  [  -d  ] [ -e csv-file ] [ -f protocol ] [ -g gnuplot-
       file ] [ -h ] [ -H custom-header ] [ -i ] [ -k ] [ -l ] [
       -m  HTTP-method  ]  [ -n requests ] [ -p POST-file ] [ -P
       proxy-auth-username:password ] [ -q ] [ -r ] [ -s timeout
       ]  [  -S ] [ -t timelimit ] [ -T content-type ] [ -u PUT-
       file ] [  -v  verbosity]  [  -V  ]  [  -w  ]  [  -x  <ta‐
       ble>-attributes    ]   [   -X   proxy[:port]   ]   [   -y
       <tr>-attributes ] [ -z <td>-attributes ] [ -Z ciphersuite
       ] [http[s]://]hostname[:port]/path
```



```sh
ab -v 2 -n 10 -p /tmp/ab_body.json -H "Host:xx.com" http://127.0.0.1:8080/xx
```



## 参数



### `-v` 显示详细信息，参数值为int



有时比如timeout，可以打开该选项。

如：

```sh
ab -n 2 -c 1 "http://192.168.0.88/"
This is ApacheBench, Version 2.3 <$Revision: 1430300 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking 192.168.0.88 (be patient)...apr_pollset_poll: The timeout specified has expired (70007)


ab -v 2 -n 2 -c 1 "http://192.168.0.88/"
This is ApacheBench, Version 2.3 <$Revision: 1430300 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking 192.168.0.88 (be patient)...INFO: GET header ==
---
GET / HTTP/1.0
Host: 192.168.0.88
User-Agent: ApacheBench/2.3
Accept: */*


---
LOG: header received:
HTTP/1.1 426 Upgrade Required
date: Thu, 26 Jul 2018 07:05:08 GMT
server: envoy
content-length: 0


WARNING: Response code not 2xx (426)
^C

Server Software:
Server Hostname:        192.168.0.88
Server Port:            80

Document Path:          /
Document Length:        0 bytes

Concurrency Level:      1
Time taken for tests:   16.722 seconds
Complete requests:      0
Failed requests:        0
Write errors:           0
Non-2xx responses:      1
Total transferred:      104 bytes
HTML transferred:       0 bytes
```











