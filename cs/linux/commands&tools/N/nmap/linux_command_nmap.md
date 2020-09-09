

```sh
nmap -p 8080 192.168.1.2-250
# 输出类似如下：
...
Nmap scan report for (192.168.1.16)
Host is up (0.0011s latency).

PORT     STATE SERVICE
8080/tcp open  http-proxy

Nmap scan report for 192.168.1.18
Host is up (0.0021s latency).

PORT     STATE  SERVICE
8080/tcp closed http-proxy
...
```