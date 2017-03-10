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

