# maven



## maven repo 错误问题：默认的local repo里的`~`

默认配置为： `/Applications/IntelliJ IDEA.app/Contents/bin/~/.m2/repository`，然后这个`~`就被翻译为raw string，于是就gg了。

处理方式为： 覆盖为对应的目录（用户目录下的。..）

= = 不支持环境变量，不能设为类似 `${HOME}/.m2/repository`

**多个窗口的话，要分别改配置**



# 快捷键

## 智能提示： alt/option+回车

## 格式化： alt+cmd+l（L）

## 优化import： alt+cmd+o

## 开启终端： fn+alt+F12

## 查看javaDoc： ctrl+j（还可以设置为on mouse on）



## 查看右侧views： 双击cmd，第二下要按住不放

win中默认展示，mac下默认隐藏而且好像不能设置为一直展示，需要如此操作，第二下松开后就隐藏... = =



# 编译

## 编译class

class reload是基于class文件变化而不是java文件变化的，而**idea不会在java文件修改后自动重编译**所以就比较少看到*class文件变化是否reload的提示*。

而很多时候是希望做到**hot reload**的，这时候可以用：**cmd+shift+F9**来手动编译。



此外：

* cmd+shift+F10 运行
* cmd+shift+F8 查看断点


