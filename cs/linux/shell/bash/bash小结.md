ref:

* [Bash Shortcuts For Maximum Productivity](http://www.skorks.com/2009/09/bash-shortcuts-for-maximum-productivity/) or [local](resource/Bash Shortcuts For Maximum Productivity.htm)




# 语法



## 控制语句



### 条件



#### 多条件



```shell
if [ \( "$g" -eq 1 -a "$c" = "123" \) -o \( "$g" -eq 2 -a "$c" = "456" \) ]
then echo abc
else echo efg
fi
```



或者



```shell
check_permission()
{
    [[ ( -d "${S_HOME}" && -w "${S_HOME}" ) || ( ! -e "${S_HOME}" && -d "${S_PARENT}" && -w "${S_PARENT}" ) ]] \
        || exit_on_err 1 "permission denied, ${HOME} is not writeable."
}
```



































