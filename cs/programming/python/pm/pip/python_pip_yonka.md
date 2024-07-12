

# tutorial



## install pip



ref: https://pip.pypa.io/en/stable/installing/

* 用linux包管理方式

* `ensurepip` （>=3.4）

* `get-pip.py` 方式

  ```sh
  curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py
  python get-pip.py
  ```

  > yonka: 注意，这个脚本可能随着py演进用到了新的语法，所以低版本py不一定兼容，甚至3.2可能不支持
  >
  > 可以尝试从： `https://bootstrap.pypa.io/<version>/get-pip.py`  获取，如：
  >
  > * https://bootstrap.pypa.io/3.2/get-pip.py
  >
  >   会 `Collecting pip<8`
  >
  > * https://bootstrap.pypa.io/2.7/get-pip.py

  **没有root权限怎么办？**： `python get-pip.py --user` 装到`~/.local/bin`下

  > 显然之后用这个pip装包也只能都`--user`了



## search



```shell
pip search ${name}
```

可以搜到不同版本。



## install



### install指定版本



```shell
pip install ${name}==${version}
```

这里的`==`是条件匹配，有些场景下（比如当前py版本兼容性问题）可能需要指定特殊的版本条件，如：

```
IPython 7.10+ supports Python 3.6 and above.
When using Python 2.7, please install IPython 5.x LTS Long Term Support version.
Python 3.3 and 3.4 were supported up to IPython 6.x.
Python 3.5 was supported with IPython 7.0 to 7.9.
# 这个提示有问题，2.6 or 3.2的话只能用1.x，那么 pip install 'ipython<2'，注意3.2一样要求pexpect降到3.x
```

这里对于py 3.2就要：`pip install "ipython<6"`



## uninstall

如题



## items



### 查看依赖

* `pip show <package>`

  ```sh
  pip3 show pexpect
  ---
  Metadata-Version: 2.1
  Name: pexpect
  Version: 4.8.0
  Summary: Pexpect allows easy control of interactive console applications.
  Home-page: https://pexpect.readthedocs.io/
  Author: Noah Spurrier; Thomas Kluyver; Jeff Quast
  Author-email: noah@noah.org, thomas@kluyver.me.uk, contact@jeffquast.com
  License: ISC license
  Location: /home/xx/.local/lib/python3.2/site-packages
  Requires: ptyprocess
  ```



### 解决版本依赖错误、冲突的思路



1. 发现冲突

   从报错里看出当前py解释器对lib code语法不支持 -> 看库的版本要求 或者说 看支持该py版本的最高库版本

2. `pip show <package>`看库依赖、看库的pypi说明/releaseNotes来了解版本支持

   关键词： drop, support 等

   > 很多库本身不严谨，只写了依赖的库的名字，这样容易引入过新的依赖库版本



### 安装whl文件

要使用 `pip` 安装指定的 `.whl` 文件，可以使用以下命令：

```sh
pip install 路径/文件名.whl
```

请将 `路径/文件名.whl` 替换为你要安装的 `.whl` 文件的实际路径和文件名。

确保在运行命令之前已经安装了 Python 和 pip，并且已经将它们添加到系统的环境变量中。

请注意，`.whl` 文件必须与你的 Python 版本和操作系统相匹配。如果你的 Python 环境与 `.whl` 文件不兼容，可能会导致安装失败或出现错误。因此，确保选择与你的 Python 版本和操作系统匹配的正确 `.whl` 文件。

此外，还建议在虚拟环境中安装和管理 Python 包，以隔离不同项目之间的依赖关系。你可以使用 `venv` 或者 `conda` 等工具创建和管理虚拟环境。



### 使用代理



```sh
 ALL_PROXY=socks5://192.168.1.1:8888 pip3 install django
 ERROR: Could not install packages due to an OSError: Missing dependencies for SOCKS support.


 pip3 install pysocks
 # 安装成功
 
 # 然后继续
 ALL_PROXY=socks5://192.168.1.1:8888 pip3 install django
 # ok
```



# 一些好用的库



## 媒体



### you-get 下载网络视频等媒体











