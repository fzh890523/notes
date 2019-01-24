



```sh
tar czf xxx.tar.gz xx_dir --exclude=xx_dir/logs/
```



```sh
tar czf hsf-guide-server.tar.gz -C /Users/xxx/target/ release
# 这样就不至于出现解压出来是 /Users/xxx/target/release 的问题
# 直接 tar czf hsf-guide-server.tar.gz /Users/xxx/target/release 会这样
# -C == new_cwd
```

