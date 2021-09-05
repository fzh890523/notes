
可以用于构造no-buf stdout，适用于一些没有相应参数的命令。 或者说更通用

```sh
xxx | stdbuf -o0 grep line | cut -c 9-
```

