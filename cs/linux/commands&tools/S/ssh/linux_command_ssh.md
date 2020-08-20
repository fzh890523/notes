

# cases



## 设置了authorized_keys仍然要输入密码的问题





```
debug1: rekey after 134217728 blocks
debug1: SSH2_MSG_NEWKEYS received
debug1: SSH2_MSG_EXT_INFO received
debug1: kex_input_ext_info: server-sig-algs=<rsa-sha2-256,rsa-sha2-512>
debug1: SSH2_MSG_SERVICE_ACCEPT received
debug1: Authentications that can continue: publickey,gssapi-keyex,gssapi-with-mic,password
debug1: Next authentication method: publickey
debug1: Offering RSA public key: /Users/${user}/.ssh/id_rsa
debug1: Authentications that can continue: publickey,gssapi-keyex,gssapi-with-mic,password
debug1: Trying private key: /Users/${user}/.ssh/id_dsa
debug1: Trying private key: /Users/${user}/.ssh/id_ecdsa
debug1: Trying private key: /Users/${user}/.ssh/id_ed25519
debug1: Next authentication method: password
admin@${ip}'s password:
```







```
tail -f /var/log/secure
Apr 24 13:34:55 ${host} su: pam_unix(su-l:session): session opened for user admin by root(uid=0)
Apr 24 13:35:33 ${host} sshd[12029]: Authentication refused: bad ownership or modes for file /home/admin/.ssh/authorized_keys
Apr 24 13:35:36 ${host} sshd[12029]: Connection closed by ${ip} port 32818 [preauth]
Apr 24 13:35:47 ${host} su: pam_unix(su-l:session): session closed for user admin
Apr 24 13:36:52 ${host} sshd[12040]: Authentication refused: bad ownership or modes for file /home/admin/.ssh/authorized_keys
Apr 24 13:38:54 ${host} su: pam_unix(su-l:session): session opened for user admin by root(uid=0)
Apr 24 13:40:18 ${host} sshd[12092]: Authentication refused: bad ownership or modes for directory /home/admin/.ssh
Apr 24 13:40:32 ${host} su: pam_unix(su-l:session): session closed for user admin
Apr 24 13:41:03 ${host} sshd[12092]: Connection closed by ${ip} port 53524 [preauth]
Apr 24 13:41:44 ${host} sshd[12101]: Connection closed by ${ip} port 44473 [preauth]
Apr 24 13:42:50 ${host} sshd[12107]: Connection closed by ${ip} port 57466 [preauth]
Apr 24 13:42:55 ${host} sshd[12123]: Accepted publickey for admin from ${ip} port 34185 ssh2: RSA SHA256:xxx
Apr 24 13:42:55 ${host} sshd[12123]: pam_unix(sshd:session): session opened for user admin by (uid=0)
Apr 24 13:42:55 ${host} sshd[12123]: pam_unix(sshd:session): session opened for user admin by (uid=0)
Apr 24 13:42:58 ${host} sshd[12125]: Received disconnect from ${ip} port 34185:11: disconnected by user
Apr 24 13:42:58 ${host} sshd[12125]: Disconnected from ${ip} port 34185
Apr 24 13:42:58 ${host} sshd[12123]: pam_unix(sshd:session): session closed for user admin
```



`~/.ssh`和`~/.ssh/authorized_keys`需要时`700`（for dir）和`600`（for file）



## 想不重复登录输入密码等验证： 连接复用



```sh
Host xxx
  HostName xxxx
  ControlMaster auto
  ControlPersist 3600  # 使得第一个连接断开后也能保持一段时间
  ControlPath /tmp/ssh_connection_%h_%p_%r.sock
  ServerAliveInterval 80  # 避免被idle timeout踢掉
  ServerAliveCountMax 6
  User xxx
```





## 代理ssh

Ref:

* https://stackoverflow.com/questions/19161960/connect-with-ssh-through-a-proxy
* https://www.techrepublic.com/article/how-to-use-ssh-to-proxy-through-a-linux-jump-host/



一个场景是： 指定的机器无法直接访问，需要通过某跳板机。



* 跳板机开的是nc之类的直接连接转发

  ```sh
  ssh USER@FINAL_DEST -o "ProxyCommand=nc -X connect -x PROXYHOST:PROXYPORT %h %p"
  ```

* 跳板机开的是socks代理

  ```sh
  ssh -o "ProxyCommand=./ncat --proxy-type socks4 --proxy 127.0.0.1:9150 %h %p" USERNAME@REMOTESERVER
  ```

* 跳板机开的是ssh

  ```sh
   ssh -v -o "ProxyCommand=ssh -q -W %h:%p ${user}@${proxyId}" ${user}@${destIp}
  ```

  

```
If you get this in OS X:

 nc: invalid option -- X
 Try `nc --help' for more information.
it may be that you're accidentally using the homebrew version of netcat (you can see by doing a which -a nc command--/usr/bin/nc should be listed first). If there are two then one workaround is to specify the full path to the nc you want, like ProxyCommand=/usr/bin/nc ...

For CentOS nc has the same problem of invalid option --X. connect-proxy is an alternative, easy to install using yum and works --

ssh -o ProxyCommand="connect-proxy -S PROXYHOST:PROXYPORT %h %p" USER@FINAL_DEST
```



每次手动输入不方便的话，可以考虑加到配置（`~/.ssh/config`）里：

```
Host host-a
        User USERNAME
        Hostname 192.168.1.38

Host host_b
        User USERNAME
        Hostname 192.168.1.221
        Port 22
        ProxyCommand ssh -q -W %h:%p host-a
```



```
The options in the above config file are:

q - Quiet mode (supresses all warning and diagnostic messages).
W - Requests that standard input and output on the client be forwarded to HOST on PORT over the secure channel.
%h - Host to connect to.
%p - Port to connect to on the remote host.
```



## ssh tunnel



**正向tunnel**

主要用于远程机器不能直接访问（需要经过一个ssh跳板机），又需要稳定访问的场景，比如测试http服务等。

可以通过`-L`来启动一个本地proxy，实际请求会： `caller -> localProxy ---ssh-tunnel---> jumper/proxy -> targetServer`

```sh
ssh -L local_port:remote_address:remote_port username@server.com
# local_port: 本地proxy端口
# remote_address: targetServer地址； remote_port: targetServer端口
# username/server.com: jumper/proxy的用户名和地址

# 如：
ssh -L 8888:192.168.1.111:1234 bob@ssh.youroffice.com
# 建立 本地 - ssh.youroffice.com 这个堡垒机 - 192.168.1.111:1234 的隧道
# 然后可以通过 localHost:8888 访问 192.168.1.111:1234 的服务了
```



**反向tunnel**

```sh
ssh -R remote_port:local_address:local_port username@server.com

# 如：
ssh -R 8888:localhost:1234 bob@ssh.youroffice.com
# 在 jumper/proxy ssh.youroffice.com上开一个8888端口上的服务，该服务实际会被转发到 localhost:1234，也即把 localhost:1234 这个服务暴露给 jumper后的环境

# <del>用了proxyCommand的好像不行，本端端口起不来</del>
# 注意： 并不是tunnel自己来起这个1234端口，而是转发给（已有的）1234
```



这里默认是bound到loopback接口，127.0.0.1。 可以如下方式来指定（[ref](https://superuser.com/questions/588591/how-to-make-ssh-tunnel-open-to-public)）： 

* `ssh -R 0.0.0.0:8888:localhost:1234 bob@ssh.youroffice.com`
* `ssh -R \*:8080:localhost:80 -N root@example.com`
* `ssh -R "[::]:8080:localhost:80" -N root@example.com`

不过，对于openSSH，需要配置启用才有效：

```
Note that if you use OpenSSH sshd server, the server's GatewayPorts option needs to be enabled (set to yes or clientspecified) for this to work (check file /etc/ssh/sshd_config on the server). Otherwise (default value for this option is no), the server will always force the port to be bound on the loopback interface only.

# 修改然后重启ssh服务
```





## 动态socks代理



可以在本地建立socks代理，请求会tunnel到remote ssh server然后转发出去。

```sh
ssh -D local_port username@server.com
# 如：
ssh -D 8888 bob@ssh.yourhome.com
# 然后可以愉快地访问 localhost:8888 这个socks代理了
```



> 早夭的科学。。方式之一





# tutorial



## exec command



## 执行多个命令



```sh
ssh aliyunzixun@xxx.com "pwd; cat hello.txt"
```



```sh
ssh aliyunzixun@xxx.com "pwd
> cat hello.txt
> ls
> pwd
> " 

# or

export name=nick
ssh aliyunzixun@xxx.com "
> echo $name
> " 
```



```sh
ssh aliyunzixun@xxx.com > /dev/null 2>&1 << eeooff
cd /home
tar xvf trove_rpm_code.tar
cd /home/trove_rpm_code/rpm
rpm -U *.rpm
exit
eeooff 
```



```sh
ssh user@host 'bash -s' < /path/script.sh
```



## 指定端口： -p ${port}





## 优先使用指定认证方式



* 密码

  `-o PreferredAuthentications=password`

* 密钥

  `-o PubkeyAuthentication=no` 禁用密钥方式



## 代理ssh

Ref:

* https://stackoverflow.com/questions/19161960/connect-with-ssh-through-a-proxy
* https://www.techrepublic.com/article/how-to-use-ssh-to-proxy-through-a-linux-jump-host/



一个场景是： 指定的机器无法直接访问，需要通过某跳板机。



* 跳板机开的是nc之类的直接连接转发

  ```sh
  ssh USER@FINAL_DEST -o "ProxyCommand=nc -X connect -x PROXYHOST:PROXYPORT %h %p"
  ```

* 跳板机开的是socks代理

  ```sh
  ssh -o "ProxyCommand=./ncat --proxy-type socks4 --proxy 127.0.0.1:9150 %h %p" USERNAME@REMOTESERVER
  ```

* 跳板机开的是ssh

  ```sh
   ssh -v -o "ProxyCommand=ssh -q -W %h:%p ${user}@${proxyId}" ${user}@${destIp}
  ```

  

```
If you get this in OS X:

 nc: invalid option -- X
 Try `nc --help' for more information.
it may be that you're accidentally using the homebrew version of netcat (you can see by doing a which -a nc command--/usr/bin/nc should be listed first). If there are two then one workaround is to specify the full path to the nc you want, like ProxyCommand=/usr/bin/nc ...

For CentOS nc has the same problem of invalid option --X. connect-proxy is an alternative, easy to install using yum and works --

ssh -o ProxyCommand="connect-proxy -S PROXYHOST:PROXYPORT %h %p" USER@FINAL_DEST
```



每次手动输入不方便的话，可以考虑加到配置（`~/.ssh/config`）里：

```
Host host-a
        User USERNAME
        Hostname 192.168.1.38

Host host_b
        User USERNAME
        Hostname 192.168.1.221
        Port 22
        ProxyCommand ssh -q -W %h:%p host-a
```



```
The options in the above config file are:

q - Quiet mode (supresses all warning and diagnostic messages).
W - Requests that standard input and output on the client be forwarded to HOST on PORT over the secure channel.
%h - Host to connect to.
%p - Port to connect to on the remote host.
```















