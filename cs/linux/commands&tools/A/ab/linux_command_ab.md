



一般是`httpd-tools`提供







# usage



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











