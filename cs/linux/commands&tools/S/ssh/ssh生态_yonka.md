


# 命令

## ssh-keygen

* 生成
  * `ssh-keygen`
  * `ssh-keygen -t rsa -b 2048`
  * `ssh-keygen -t rsa`
* `-f` 指定生成的密钥文件路径（私钥）
  
  * `-m pem` 指定类型为pem格式（而不是openssh格式）
  
* 查看

  * `ssh-keygen -lf ~/.ssh/id_rsa.pub` 查看footprint

    类似： `3072 SHA256:4c/dm/xt1F ... `

  * `ssh-keygen -E md5 -lf ~/.ssh/id_rsa.pub` 查看footprint的md5格式

    类似： `3072 MD5:95:ba:ae ... `



## ssh-add

`ssh-add ~/.ssh/id_rsa`

把指定私钥添加到agent，需要agent已运行

* `ssh-add -l` 显示agent已加载的

* `ssh-add -L` 显示已加载的，并且输出footprint等信息

* `ssd-add -K <path>` 添加（持久化）到keychain，不过需要已有keychain，如果没运行的话会报错
  默认是加到agent（的非持久化存储）

## ssh-copy-id

```sh
## [ warning this will overwrite existing file on the remote box ] ##
scp $HOME/.ssh/id_rsa.pub nixcraft@server1.cyberciti.biz:~/.ssh/authorized_keys
```

更好的做法是：
```sh
ssh-copy-id -i $HOME/.ssh/id_rsa.pub nixcraft@server1.cyberciti.biz
```


# 组件

## ssh-agent

常见启动方式 `ssh-agent $SHELL`，开子shell然后...

也可以 `eval "ssh-agent"` 这样会直接后台执行并且把必要的环境变量输出到stdout，通过eval可以生效，如：

```sh
ssh-agent

SSH_AUTH_SOCK=/tmp/ssh-bQ81ztcMJxmM/agent.46060; export SSH_AUTH_SOCK;
SSH_AGENT_PID=46061; export SSH_AGENT_PID;
echo Agent pid 46061;

eval "ssh"
```

不过，这个方式登陆（shell）退出后，会成为孤儿进程被init领养，所以会多出。。。一般来说需要在使用结束后：

```
kill "$SSH_AGENT_PID"
```



```sh
ssh-add.exe ~/.ssh/id_rsa
Could not open a connection to your authentication agent.

ssh-agent $SHELL

ssh-add ~/.ssh/id_rsa
Identity added: ...

# now we can ssh to ... and auto-pass ssh key
```






## keychain

因为ssh-agent是session级别的，退出会话或者重启等操作后会丢失管理的keys数据。
所以可以用keychain来固化。

常见用法是： 在`~/.bashrc`里加入（ref： https://www.cyberciti.biz/faq/ubuntu-debian-linux-server-install-keychain-apt-get-command/）：
```sh
#####################################################################################
### The --clear option make sure Intruder cannot use your existing SSH-Agents keys 
### i.e. Only allow cron jobs to use password less login 
#####################################################################################
/usr/bin/keychain --clear $HOME/.ssh/id_rsa
source $HOME/.keychain/$HOSTNAME-sh
```

OR

```sh
###########################################################################
# allow $USER to use keys. Only enter once and it will remain enabled till
# you delete it or reboot the server 
###########################################################################
/usr/bin/keychain $HOME/.ssh/id_rsa
source $HOME/.keychain/$HOSTNAME-sh
```

加入目录下所有： 

```sh
keychain `ls $HOME/.ssh/id_* | grep -v '\.pub$'`
```





* `keychain --list`  list signatures of all active SSH keys

* `keychain --list-fp` show fingerprints of all active SSH keys

`--clear` 先清空agent已加载的所有keys
```sh
/usr/bin/keychain --clear /root/.ssh/id_ed25519
source /root/.keychain/$HOSTNAME-sh
```

# 配置项



## ProxyCommand



* `ProxyCommand C:\\Windows\\System32\\OpenSSH\\ssh.exe -q -W %h:%p home`
* `ProxyCommand ssh -q -W %h:%p home`



## AgentForward

## UseKeychain

