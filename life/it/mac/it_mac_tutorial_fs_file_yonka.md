



### 部分目录无法访问问题： `Operation not permitted`

比如ls无法显示内容等等。是因为macos的`System Integrity Protection (the SIP)`。
应对方式：

* **不建议** 关掉SIP
  命令行执行`csrutil disable` 然后重启（反向操作为`csrutil enable`

* Go to System Preferences > Security & Privacy and give Full Disk Access to Terminal

  ** Apple menu** -> **System Preferences** -> **Security & Privacy** -> **Privacy** -> **Full Disk Access ->** +

  Choose:

  **Applications** -> **Utilities** -> **folder** -> **Terminal** -> **open/grant terminal full disk access privileges.** -> **Relaunch terminal**

  > 其他app同理



