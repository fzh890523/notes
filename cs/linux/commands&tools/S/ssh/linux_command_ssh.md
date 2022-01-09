

# cases

## 登录

### slient登录

* 免密： 见下面免密登录部分

* 免提示

  * host key check提示

    提示类似

    ```
    The authenticity of host 'xxx.com (x.x.x.x)' can't be established.
    ECDSA key fingerprint is SHA256:xxx.
    Are you sure you want to continue connecting (yes/no/[fingerprint])? yes
    ```

    处理： 加入`StrictHostKeyChecking=accept-new`选项（代替老的 `StrictHostKeyChecking no`）

    * ssh命令里加入： `-o "StrictHostKeyChecking accept-new"`

    * ssh config里加入（如`~/.ssh/confiog`）

      ```properties
      Host *
        StrictHostKeyChecking accept-new
      ```

      



### 免密登录



#### key登录+authorized_keys



##### 自动“设置”authorized_keys： ssh-copy-id

```sh
ssh-copy-id xxx.com
# 输出密码

ssh xxx.com  # 自动key登录
```





##### 设置了authorized_keys仍然要输入密码的问题





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



### 想不重复登录输入密码等验证： 连接复用



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



> windows还不支持



### 指定密码登录



### 只用密码登录

`-o PreferredAuthentications=password -o PubkeyAuthentication=no`



## 代理ssh（ProxyCommand）

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



### key传递

A -> proxyB -> C

`ssh -o "ProxyCommand=ssh -W %h:%p root@proxyB" C`

可能有几种情况：

* 让b用b上key去连c
* 让b用a的key去连c



ssh-agent forward-agent方案应该是可以，但命令行的方式好像不太行

外面或者里面`-i`加上`-o "ForwardAgent=yes"`都不行。

里外同时`-i`可以： `ssh -i ~/.ssh/xxx -o "ProxyCommand=ssh -i ~/.ssh/xxx -W %h:%p root@proxyB" C`





### win上的问题：posix_spawn: No such file or directory

见： [ProxyCommand incorrectly requires an absolute path #1185](https://github.com/PowerShell/Win32-OpenSSH/issues/1185) 

大概就是，一个bug导致win上ssh在执行`ProxyCommand`是search path不是`PATH`，导致搜不到`ssh`，而要用完整路径

```sh
where.exe ssh  # C:\Windows\System32\OpenSSH\ssh.exe
# then
ProxyCommand C:\Windows\System32\OpenSSH\ssh.exe -q -W %h:%p xxx
# 为了git bash兼容性，写成如下更好
ProxyCommand C:\\Windows\\System32\\OpenSSH\\ssh.exe -q -W %h:%p xxx
```





## ssh tunnel

### 后台执行

* `-N` 不触发登录动作

  > **-N** Do not execute a remote command. This is useful for just forwarding ports (protocol version 2 only).

* `-f` 后台执行

  > **-f** Requests ssh to go to background just before command execution. This is useful if ssh is going to ask for passwords or passphrases, but the user wants it in the background. This implies **-n**.
  >
  > **-n** Redirects stdin from `/dev/null` (actually, prevents reading from stdin). This must be used when `ssh` is run in the background.



#### 怎么关闭后台tunnel？

`netstat -ltnp`根据端口找到pid再kill肯定是可以的，但太麻烦。 



更好的方式是用master mode，类似： `-M -S /tmp/xxx` 启动， `-S /tmp/xxx -O exit <target>`关闭

如：

```sh
# 启动tunnel
ssh -f -N -M -S /tmp/ssh-socket-xx-com -D <ip>:<port> xxx.com

# 关闭tunnel
ssh -S /tmp/ssh-sock-xxx-com -O exit xxx.com  # sock文件有访问权限，要同user
```



### tunnel自动启动

可以用os的自启动机制，如：

* linux下
  * 做成service
  * supervisord管理
  * shell启动脚本
  * ...
* windows
  * 做成bat加到`C:\Users\<user>\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup`





### **正向tunnel**



#### 基于命令参数： `-L` （实际是一个端口转发）

> 这个实际是端口转发而不是tunnel



主要用于远程机器不能直接访问（需要经过一个ssh跳板机），又需要稳定访问的场景，比如测试http服务等。

可以通过`-L`来启动一个本地proxy，实际请求会： `caller -> localProxy ---ssh-tunnel---> jumper/proxy -> targetServer`

```sh
ssh -L local_port:remote_address:remote_port username@server.com
# local_port: 本地proxy端口
# remote_address: targetServer地址； remote_port: targetServer端口
# username/server.com: jumper/proxy的用户名和地址
# 也即：
ssh -L local_port:target_address:target_port username@proxy.com

# 如：
ssh -L 8888:192.168.1.111:1234 bob@ssh.youroffice.com
# 建立 本地 - ssh.youroffice.com 这个堡垒机 - 192.168.1.111:1234 的隧道
# 然后可以通过 localHost:8888 访问 192.168.1.111:1234 的服务了

ssh -L 192.168.1.100:8888:192.168.1.111:1234 bob@ssh.youroffice.com
# local addr可以省略，默认127； remote addr好像不能省略，即使留空也会在连接时打印报错
```





#### 基于命令参数： `-D` （socket proxy而不是tcp tunnel）



可以在本地建立socks代理，请求会tunnel到remote ssh server然后转发出去。

```sh
ssh -D local_port username@server.com
# 如：
ssh -D 8888 bob@ssh.yourhome.com
# 然后可以愉快地访问 localhost:8888 这个socks代理了
```



> 早夭的科学。。方式之一







#### 基于配置 `LocalForward`

如：

```properties
Host home
    Hostname xx.com
    port 1234
    LocalForward 12345 192.168.1.100:2345
```

这个配置可以把当前 本地的`12345`端口 通过`本地-home之间的tunnel` 转发到远端 `192.168.1.100:2345`

> 这个12345也可以写成`addr:port`格式来限定。
>
> 效果与`-D`类似。
>
> **这种方式需要ssh home来触发tunnel建立，生命周期也与这个ssh conn相同**，也显然了同时只能有一个tunnel（local port不同），所以第二个ssh conn会报错tunnel无法建立



### **反向tunnel**



#### 基于命令参数 `-R` （也是端口转发）



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



## ssh命令的补全

bash/zsh都提供了ssh的复杂的补全功能支持，这里不赘述。

分别见：

* `/usr/share/zsh/functions/Completion/Unix/_ssh`
* `/usr/share/bash-completion/completions/ssh`



### ssh命令补全known_hosts内容

这个本来都支持的（如果对应的sh安装了completion的话），但一些linux发行版，可能是为了安全考虑，对`~/.ssh/known_hosts`中内容做了hash处理，会导致无法读取hostname。

处理方式：

1. 把如下内容加到`~/.ssh/config`

   ```properties
   Host *
       HashKnownHosts no
   ```

2. `rm ~/.ssh/known_hosts` （这一步非必须？）

3. 做ssh操作，让`~/.ssh/known_hosts`内容被填充

   这一步不一定要现在做

4. 重启shell（重新登陆等方式）

   因为这个文件的读取似乎是shell init的时候一次性的，所以...

   也意味着如果运行时增加的host，不能及时生效



# 问题



## 有些ssh client只支持pem（rsa）格式私钥



如果私钥是openssh格式的话，需要转为pem格式或者重新创建pem格式的













