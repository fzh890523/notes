并发执行命令，需要安装：

```shell
yum install -y parallel
```



类似：

```shell
(echo command1; echo command2) | parallel
# 可能有“广告”，可以加个 2>/dev/null

parallel -j 2 "sh a.sh" "sh b.sh";  
# 好像不是每个版本都支持

# zsh builtin支持的repeat，也可以 zsh -c "repeat 5 ..."
repeat 5 {echo sh a.sh} | parallel -j 5  # 并发度为5执行脚本
```

