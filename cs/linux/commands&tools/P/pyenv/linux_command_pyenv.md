

使用 `pyenv` 工具可以方便地管理多个 Python 版本。以下是在 Ubuntu 上安装和使用 `pyenv` 的步骤：

1. 安装依赖项：首先，确保你的系统已安装以下依赖项。运行以下命令安装它们：

   

   ~~~sh
   sudo apt update
   sudo apt install curl git
   ```
   ~~~

2. 安装 `pyenv`：运行以下命令来下载和安装 `pyenv`：

   

   ~~~sh
   curl -L https://github.com/pyenv/pyenv-installer/raw/master/bin/pyenv-installer | bash
   ```
   
   这将下载 `pyenv` 安装脚本并执行安装。
   ~~~

3. 配置环境变量：安装完成后，需要将 `pyenv` 添加到你的 shell 配置文件（例如 `~/.bashrc` 或 `~/.bash_profile`）中。运行以下命令将其添加到配置文件中：

   

   ~~~sh
   echo 'export PATH="$HOME/.pyenv/bin:$PATH"' >> ~/.bashrc
   echo 'eval "$(pyenv init --path)"' >> ~/.bashrc
   echo 'eval "$(pyenv virtualenv-init -)"' >> ~/.bashrc
   ```
   
   运行以下命令以使配置变更生效：
   
   ````bash
   source ~/.bashrc
   ```
   ~~~

4. 安装 Python 版本：现在，可以使用 `pyenv` 安装所需的 Python 版本。运行以下命令列出可用的 Python 版本：

   

   ~~~sh
   pyenv install --list
   ```
   
   从列表中选择一个版本并安装它。例如，要安装 Python 3.9.7，运行以下命令：
   
   ````bash
   # 依赖 libssl-dev, sqlite3 等
   sudo apt-get install build-essential libssl-dev zlib1g-dev libbz2-dev \
      libreadline-dev libsqlite3-dev wget curl llvm libncurses5-dev \
      libncursesw5-dev xz-utils tk-dev libffi-dev liblzma-dev
   
   pyenv install 3.9.7
   # 这个不支持指定安装包，如果网络问题下载太慢。 只能类似： all_proxy=socks5://x.x.x.x:1080 pyenv install 3.9.7
   ```
   
   安装过程可能需要一些时间，因为它会从源代码构建 Python。
   ~~~

5. 使用特定版本的 Python：安装完成后，你可以使用 `pyenv` 来切换和使用特定版本的 Python。运行以下命令切换到特定版本：

   

   ~~~sh
   pyenv global 3.9.7
   ```
   
   这将将全局 Python 版本设置为 3.9.7。你还可以使用 `pyenv local` 命令将特定版本绑定到项目的目录。
   ~~~

6. 验证安装：运行以下命令验证新版本的 Python 是否已成功安装：

   

   ~~~sh
   python --version
   ```
   
   确保显示的是配置的 Python 版本号。
   ~~~

现在，你可以使用 `pyenv` 来管理多个 Python 版本，并根据需要在不同项目中切换版本。

