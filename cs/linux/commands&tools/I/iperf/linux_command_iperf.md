

> 注： 这个是跨平台的

```s
sudo iperf -s # 开启server端，默认5001端口
iperf -c 192.168.88.1

sudo iperf -s -p 8080
iperf -p 8080 -c 192.168.88.1 

sudo iperf -s -p 8080 -u
iperf -u -p 8080 -c 192.168.88.1  # --udp-histogram
```

