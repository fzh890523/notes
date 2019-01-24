





### kill指定信号的命令行语法



The [standard (POSIX) syntax](http://pubs.opengroup.org/onlinepubs/9699919799/utilities/kill.html) is:

```
kill -s STOP "$pid"
```

That is, without the `SIG` prefix. Some shell implementations, support `kill -s SIGSTOP` or `kill -SIGSTOP` as an extension but that's not standard nor portable.

The UNIX specification (POSIX+XSI) also allows:

```
kill -STOP "$pid"
```

And

```
kill -19 "$pid"
```

Though which signal number is `SIGSTOP` is not specified and may change between systems and even architectures for a same system, so should be avoided.



> 背景：
>
> command-shell里 kill -SIGTERM 可以 而 脚本里就不行