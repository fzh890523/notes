

# 问题



## 细碎



* pycharm里project目录好像默认会加到python path

  同一个proj里多个project定义的话，可能会混乱

  遇到：

  Proj1/src/main/a/a/b/c

  把proj1和main都设置为proj了，导致main被默认加到py path，于是`import a.b`时，`a`理解为`main/a`这个，于是gg。

  = =



















