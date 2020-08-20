

ref: https://unix.stackexchange.com/questions/239528/dynamic-zsh-autocomplete-for-custom-commands


```sh
#compdef eb_instances

_eb_instances() {
  local state

  _arguments \
    '1: :->aws_profile'\
    '*: :->eb_name'

  case $state in
    (aws_profile) _arguments '1:profiles:(cuonglm test)' ;;
              (*) compadd "$@" prod staging dev
  esac
}

_eb_instances "$@"

# 链接里的文件路径有问题，应该是 fpath 中的路径表示的“目录”下创建 _${func} 文件，然后加入如上内容。 如 ~/.zsh/completion/_eb_instances 文件
```

ref： https://github.com/zsh-users/zsh-completions/blob/master/zsh-completions-howto.org

<https://askql.wordpress.com/2011/01/11/zsh-writing-own-completion/>



```sh
# COMPLETION SETTINGS
# add custom completion scripts
fpath=(~/.zsh/completion $fpath) 
 
# compsys initialization
autoload -U compinit
compinit
 
# show completion menu when number of options is at least 2
zstyle ':completion:*' menu select=2
```



# examples



## diskutil



```sh
#compdef diskutil

_diskutil() {
  local state

  _arguments \
    '1: :->sub_cmd'

  case $state in
    (sub_cmd) _arguments "1:sub_cmds:(`diskutil | grep \"^     \" | awk '{print $1}'`)" ;;
  esac
}

_diskutil "$@"
```













