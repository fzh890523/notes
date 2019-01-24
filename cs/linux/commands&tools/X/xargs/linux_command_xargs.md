



```sh
ps aux | grep java | awk '{print $2}' | xargs -I{} bash -c "kill -9 "{}" || true"
```







## ignore error



```sh
xargs sh -c "somecommand || true"
```







# mac版本



没找到mac的gnu版本xargs，☹️。



```shell
find ./ -name "*.orig" | xargs -I yonka rm yonka
```













