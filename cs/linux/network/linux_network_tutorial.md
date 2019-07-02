



## dhcp

### renew dhcp addr

```sh
dhclient -r
# or 
dhclient -r ${interface_name}  # dhclient -r eth0
```



### dhcp broadcast hostname

```sh
cat /etc/dhcp/dhclient.conf | grep send
# 确保其中有： send host-name = gethostname();
# 如果希望写死的话，也可以 send host-name = "xxx";
```





##hostname



### set hostname



* `sudo hostname ${new_hostname}` 

  当前生效，重启丢失

*  `sudo vim /etc/hostname` 修改其中内容

  持久有效(但当前要生效的话，按前一条操作或者`sudo service hostname restart`)

* 在`/etc/network/interfaces`里针对特定接口进行配置







