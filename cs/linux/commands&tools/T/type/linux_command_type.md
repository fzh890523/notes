

ref： https://www.howtoforge.com/linux-type-command/



输出给出的 symbol ref的对象。

`type [-afptP] name [name ...]`

* `-t` 只输出类型

* `-a` 输出全部匹配的

* `-P` 强制查找到路径，即使是alias

  类似会做个跟踪查找，先查找到alias，再查找到alias用到的命令的path。

  而如果该symbol（假如只是function）无法被查找到为path（file）的话会查找失败

可以通过返回值判断是否查找成功