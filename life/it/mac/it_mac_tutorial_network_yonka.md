## hostname

* s

* 在`设置-共享`里

* 在`scutil --set HostName`

  这个不清楚什么关系，跟前者。

  ```sh
  sudo scutil --set HostName <new host name>
  sudo scutil --set LocalHostName <new host name>
  sudo scutil --set ComputerName <new name>
  dscacheutil -flushcache
  ```

  ref: https://apple.stackexchange.com/questions/287760/set-the-hostname-computer-name-for-macos



## route



### default route



#### 默认路由优先级

> macos似乎没（默认）路由优先级的概念； 但有接口优先级概念；
>
> 所以同一个接口上的多个默认路由，似乎无法明确其优先级
>
> 

```sh
$ route delete default
$ route add default 192.168.0.1

# or

$ route change default -interface $INTF
$ route change 192.168.0.0/16 -interface $INTF
```



```sh
networksetup -listnetworkserviceorder  # 查看接口优先级
networksetup -ordernetworkservices  # 修改接口优先级

Usage: networksetup -ordernetworkservices <service1> <service2> <service3> <...>
        Order the services in order specified. Use "-listnetworkserviceorder" to view service order.
        Note: use quotes around service names which contain spaces (ie. "Built-in Ethernet").
```







# network svc



## sshd



### 修改port



* `ssh.plist` 好像要关sip

  > svc name `ssh` -> 具体port

  ```xml
      <key>Sockets</key>
      <dict>
              <key>Listeners</key>
              <dict>
                      <key>SockServiceName</key>
                      <string>ssh</string>
                      <key>Bonjour</key>
                      <array>
                              <string>ssh</string>
                              <string>sftp-ssh</string>
                      </array>
              </dict>
      </dict>
  ```

* `/etc/services` 但同时影响client

> 两个都要：
>
> sudo launchctl unload <plist文件> 然后 load 才能生效



> from https://gist.github.com/mrublev/3149842
>
> > 这个link里还提到新增一个 ssh-alt svc的方式（services+plist）
>
> For the purpose of changing the port that sshd listens on (rather than running an additional sshd on another port), I found that editing /etc/services to replace 22 with my alternate port worked. Then you don't need to use a custom launchctl (just enable the standard "Sharing -> Remote Login" preference), but you should also edit /etc/ssh/ssh_config to add " Port 22" so that outbound ssh still tries port 22.
>
> > <del>但。。。还是不行，outbound还是被影响了</del>
> >
> > `-v`看了下，发现`debug1: Reading configuration data /opt/homebrew/etc/ssh/ssh_config`，原来用的不是默认ssh，所以才没生效。 把homebrew这个client配置也改了就ok了

