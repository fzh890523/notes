## 快捷键



### 提示



* completion
  * basic: `alt+s` - 自定义的
  * smart: `ctrl+alt+s` - 自定义的
* view
  * param info: `ctrl+p`
  * type info: `ctrl+shift+p`





## debug



### breakpoint



#### 条件断点



##### 根据执行次数。。。



没有原生支持，大约的思路：

1. 增加一个遍历bp
2. 每次执行+1
3. 条件断点判断值



问题： 侵入代码，比较麻烦。 而且有些lib修改起来更。。。



应对： `eval("exec('try:\\n    x += 1\\nexcept NameError:\\n    x = 1') or x == 10000")`

或者写的更不容易名字冲突一点： `eval("exec('try:\\n    CBP_counter += 1\\nexcept NameError:\\n    CBP_counter = 1') or CBP_counter == 10000")`

> from https://stackoverflow.com/questions/44980837/ignore-pycharm-breakpoint-for-first-n-hits
>
> What this does is execute a try statement which increments a variable or creates it if it doesn't exist. Then evaluates that along with a statement checking if the variable has been incremented enough times yet with that being your ending condition. Note, the `exec` is required to run the `try`, but the `eval` is needed to "return" the condition to PyCharm. This is absurdly hacky, but it works for your case!
>
> 
>
> yonka： 亲测可用



# 问题



## 细碎



* pycharm里project目录好像默认会加到python path

  同一个proj里多个project定义的话，可能会混乱

  遇到：

  Proj1/src/main/a/a/b/c

  把proj1和main都设置为proj了，导致main被默认加到py path，于是`import a.b`时，`a`理解为`main/a`这个，于是gg。

  = =



















