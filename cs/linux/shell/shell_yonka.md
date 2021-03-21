



### login shell vs non-login shell

根据是否interactive还可以细分：

* `interactive login shell`

  **怎么“达到”**：

  * log in 

    * on a text console
    * though SSH
    * with `su -`

    注意，` When you log in in graphical mode (on an [X display manager](http://en.wikipedia.org/wiki/X_display_manager_(program_type))), you don't get a login shell, instead you get a session manager or a window manager`

  **读取的配置文件**：

  * `/etc/profile`
  * `~/.profile`
  * `~/.bash_profile` （对bash）
  * `~/.zsh_profile` 和 `/etc/profile` （对于zsh）
  * `~/.login` 和 `/etc/csh.login`  （对于csh）

* `non-interactive login shell`

  很少见

  **怎么“达到”**：

  * ` some X settings do that when you log in with a display manager`

  * ` log in remotely with a command passed through standard input which is not a terminal`

    如： `ssh example.com <my-script-which-is-stored-locally`。 **注意** 区别于 `ssh example.com my-script-which-is-on-the-remote-machine` （启动的是non-non...）

  **读取的配置文件**： (根据发行版。。。，可能读，或者不读)

  * `/etc/profile`
  * `~/.profile`

* `interactive non-login shell`

  **怎么“达到”**：

  * 在已有session里再启动shell（比如执行`sh`）

  **读取的配置文件**：

  * **rc文件**（`~/.${shellname}rc`如`~/.bashrc`） （bash/zsh/csh/mksh）
  * 对于`POSIX/XSI-compliant shells such as dash, ksh, and bash`：  the file indicated by the `ENV` variable

* `non-interactive non-login shell`

  **怎么“达到”**：

  * 在shell里执行脚本（所以是很常见的case了）

  > `wsl xx.sh` 或者 `wsl zsh xx.sh`  或者 `wsl echo 123` 也都是这个
>
  > 似乎wsl对于命令的执行做了一点封装，但看 `$SHLVL` 又是1

  **读取的配置文件**：

  * bash： `BASH_ENV`指定的
  * zsh： `/etc/zshenv` 和 `~/.zshenv`



此外，对于**GUI terminal**不同系统有点差异：

* ubuntu是non-login shell

* macos是login shell

  





* 定义： `A login shell is the first process that executes under your user ID when you log in for an interactive session. `







#### **区分**

* login shell
  * 按照惯例，在shell里`echo $0`，输出是类似`-$SHELL`如`-bash`则为login shell，`$SHELL`格式如`bash`则为non-login shell

    `w`命令的输出的`WHAT`列也可以看到

  * `shopt -q login_shell`的返回值，如 

    ```sh
    shopt -q login_shell && echo login_shell || echo non_login_shell
    login_shell
    # bash ok; zsh的话，non-login shell里都没有shopt这个name，会报错... 虽然可以推断出...
```
    
  
  
* interactive

  ```sh
  # bash
  [[ $- == *i* ]] && echo 'Interactive' || echo 'Not interactive'
  # or
  case $- in
    *i*) echo "This shell is interactive";;
    *) echo "This is a script";;
  esac
  
  
  # zsh
  [[ -o interactive ]] && echo interactive || echo non-interactive
  ```

  



### shell config



* `~/.profile`

  read by login shell，但是：

  * This file is not read by bash(1), if ~/.bash_profile or ~/.bash_login exists

  * Zsh runs `~/.zprofile`, not `~/.profile`, when it is invoked as a login shell. The reason is that zsh has enough incompatibilities with standard shells to break scripts.

    要使得zsh运行该文件的最简单方式：

    ```sh
    # 在 ~/.zprofile 中
    emulate sh
    . ~/.profile
    emulate zsh
    ```

    还有一种更迂回的方式：

    1. 设置`/bin/sh` 为login shell（这样会加载`~/.profile`）

    2. 在该文件中加入 `export SHELL=/bin/zsh` （效果类似在其中 `exec zsh`）

       有些终端不遵守`SHELL`，可能行为不一致

  



![/images/BashStartupFiles1.png](_pics/shell_yonka/BashStartupFiles1.png)



https://medium.com/@rajsek/zsh-bash-startup-files-loading-order-bashrc-zshrc-etc-e30045652f2e



```
/bin/bash
       The bash executable
/etc/profile
       The systemwide initialization file, executed for login shells
~/.bash_profile
       The personal initialization file, executed for login shells
~/.bashrc
       The individual per-interactive-shell startup file
~/.bash_logout
       The individual login shell cleanup file, executed when a login shell exits
~/.inputrc
       Individual readline initialization file
       
       
+----------------+-----------+-----------+------+
|                |Interactive|Interactive|Script|
|                |login      |non-login  |      |
+----------------+-----------+-----------+------+
|/etc/zshenv     |    A      |    A      |  A   |
+----------------+-----------+-----------+------+
|~/.zshenv       |    B      |    B      |  B   |
+----------------+-----------+-----------+------+
|/etc/zprofile   |    C      |           |      |
+----------------+-----------+-----------+------+
|~/.zprofile     |    D      |           |      |
+----------------+-----------+-----------+------+
|/etc/zshrc      |    E      |    C      |      |
+----------------+-----------+-----------+------+
|~/.zshrc        |    F      |    D      |      |
+----------------+-----------+-----------+------+
|/etc/zlogin     |    G      |           |      |
+----------------+-----------+-----------+------+
|~/.zlogin       |    H      |           |      |
+----------------+-----------+-----------+------+
|                |           |           |      |
+----------------+-----------+-----------+------+
|                |           |           |      |
+----------------+-----------+-----------+------+
|~/.zlogout      |    I      |           |      |
+----------------+-----------+-----------+------+
|/etc/zlogout    |    J      |           |      |
+----------------+-----------+-----------+------+       
```





### change shell

```bash
chsh /bin/zsh

sudo chsh /bin/zsh xxuser
```

> * 需要用shell path而不是name
> * shell path需要在 /etc/shells 里 （一般装好shell就在了）





