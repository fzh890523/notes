

* `127.0.0.0/8` 下任何一个ip都可以作为： 

  * dest ip
  * src ip
  * listen ip

  如：

  ```sh
  nc -l -s 127.0.0.87 -p 18888
  telnet -b 127.0.0.88 127.0.0.87 18888
  
  
  netstat -atnp | grep 188
  (Not all processes could be identified, non-owned process info
   will not be shown, you would have to be root to see it all.)
  tcp        0      0 127.0.0.87:18888        0.0.0.0:*               LISTEN      8339/nc
  tcp        0      0 127.0.0.88:40669        127.0.0.87:18888        ESTABLISHED 8521/telnet
  tcp        0      0 127.0.0.87:18888        127.0.0.88:40669        ESTABLISHED 8339/nc
  ```

  





ref： [What is the 127.0.0.2 IP address for?](https://superuser.com/questions/393700/what-is-the-127-0-0-2-ip-address-for)



> - http://en.wikipedia.org/wiki/Loopback
> - https://en.wikipedia.org/wiki/Localhost
>
> > IPv4 network standards reserve the entire 127.0.0.0/8 address block for loopback purposes. That means any packet sent to one of those 16,777,214 addresses (127.0.0.1 through 127.255.255.254) is looped back. IPv6 has just a single address, ::1.
> >
> > Various Internet Engineering Task Force (IETF) standards reserve the IPv4 address block 127.0.0.0/8, in CIDR notation and the IPv6 address ::1 for this purpose. The most common IPv4 address used is 127.0.0.1. Commonly these loopback addresses are mapped to the hostnames, localhost or loopback.
>
> or from the RFC itself:
>
> - [RFC 3330 - Special-Use IPv4 Addresses](http://tools.ietf.org/html/rfc3330)
>
> > 127.0.0.0/8 - This block is assigned for use as the Internet host loopback address. A datagram sent by a higher level protocol to an address anywhere within this block should loop back inside the host. This is ordinarily implemented using only 127.0.0.1/32 for loopback, but no addresses within this block should ever appear on any network anywhere [RFC1700, page 5].
>
> For fun, try by pinging:
>
> ```
> $ ping 127.127.127.127
> PING 127.127.127.127 (127.127.127.127) 56(84) bytes of data.
> 64 bytes from 127.127.127.127: icmp_req=1 ttl=64 time=0.110 ms
> 64 bytes from 127.127.127.127: icmp_req=2 ttl=64 time=0.065 ms
> ^C
> --- 127.127.127.127 ping statistics ---
> 2 packets transmitted, 2 received, 0% packet loss, time 999ms
> rtt min/avg/max/mdev = 0.065/0.087/0.110/0.024 ms
> ```



