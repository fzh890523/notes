

## 设置

```cmd
PS C:\Users\xx> ubuntu /?                                                                                             Launches or configures a Linux distribution.

Usage:
    <no args>
        Launches the user's default shell in the user's home directory.

    install [--root]
        Install the distribuiton and do not launch the shell when complete.
          --root
              Do not create a user account and leave the default user set to root.

    run <command line>
        Run the provided command line in the current working directory. If no
        command line is provided, the default shell is launched.

    config [setting [value]]
        Configure settings for this distribution.
        Settings:
          --default-user <username>
              Sets the default user to <username>. This must be an existing user.

    help
        Print usage information.
```



### 默认用户

```sh
ubuntu.exe config --default-user ${user}
```



可以指定root，应对某些case，比如其他用户的sodu无法使用（破坏了 `/etc/sudoers`文件等等）。



## 图形界面



```sh
# sudo apt-get update
sudo apt-get install ubuntu-desktop
```



## 服务



### sshd



默认不开启，也没有systemd来管理，so...

hack的方法：

```sh
# ~/.profile里
pgrep sshd || sudo service ssh start
```



对于zsh

```sh
# ~/.zprofile里
emulate sh
. ~/.profile
emulate zsh
```



这样需要打开wsl ubuntu，如果想不打开的话：

```bat
# C:\ProgramData\Microsoft\Windows\Start Menu\Programs\StartUp 里加入 wsl_start_services.bat，内容如下
wsl sudo service ssh start

# 这个目录在运行里： shell:startup 或者 shell:commons startup 可以打开

# 其他windows启动执行/调度执行机制也可以
```



wsl2似乎就不用这么迂回了。。。 不过需要windows版本升上去才能升





