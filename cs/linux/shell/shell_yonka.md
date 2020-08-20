



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

  **读取的配置文件**：

  * bash： `BASH_ENV`指定的
  * zsh： `/etc/zshenv` 和 `~/.zshenv`



此外，对于**GUI terminal**不同系统有点差异：

* ubuntu是non-login shell

* macos是login shell

  





* 定义： `A login shell is the first process that executes under your user ID when you log in for an interactive session. `







**区分**：

* 按照惯例，在shell里`echo $0`，输出是类似`-$SHELL`如`-bash`则为login shell，`$SHELL`格式如`bash`则为non-login shell

  `w`命令的输出的`WHAT`列也可以看到

* `shopt -q login_shell`的返回值，如 

  ```sh
  shopt -q login_shell && echo login_shell || echo non_login_shell
  login_shell
  ```

  

