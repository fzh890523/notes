

简单的语法汇总

* 颜色： `%{$目标[颜色]}%着色内容%{$reset_color%}`

  目标：

  * `fd`/`fg_bold`

  颜色：

  * `red`
  * `yellow`
  * `green`
  * ...

  `reset_color` 顾名思义，会重置掉前面的颜色设置，恢复到默认色

* 判断exit code来决定显示内容： `%{?:为0显示内容:不为0显示内容}`

* 内容
  * `%n` username
  * `%m` hostname



```sh
PROMPT="%(?:%{$fg_bold[green]%}➜ :%{$fg_bold[red]%}➜ )%{$reset_color%}"
PROMPT+="%{$fg[yellow]%} %n%{$reset_color%}@%{$fg[yellow]%}%m %{$reset_color%}"
PROMPT+=' %{$fg[cyan]%}%c%{$reset_color%} $(git_prompt_info)'
```





