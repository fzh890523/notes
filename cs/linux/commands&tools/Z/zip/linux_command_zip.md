
* 压缩

  * `zip squash.zip file1 file2 file3` 压缩几个文件

  * `zip -r squash.zip dir1` 压缩目录

    默认好像只包含一级文件。 需要`-r`递归加入才是预期行为。

  * `-e` 加密，prompt输入密码
  * `-P <password>`加密，需要显式带上密码

* 解压，详见unzip命令

  * `unzip squash.zip`

