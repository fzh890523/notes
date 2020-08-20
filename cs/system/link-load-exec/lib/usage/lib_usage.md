
# cases

##python load ssl 失败问题



排查：

1. `ldd /usr/lib64/python2.6/lib-dynload/_ssl.so  | grep libssl`

   ```
   libssl.so.10 => /usr/lib64/libssl.so.10 (0x00007f759f8d3000)
   ```

2. `ldconfig -v 2>/dev/null  | egrep "^/|ssl"`

   ```
   /usr/lib64/mysql:
   /lib:
   /lib64:
   /usr/lib:
   /usr/lib64:
   	libssl.so.10 -> libssl.so.1.0.1e
   	libssl3.so -> libssl3.so
   /lib64/tls: (hwcap: 0x8000000000000000)
   /usr/lib64/sse2: (hwcap: 0x0000000004000000)
   /usr/lib64/tls: (hwcap: 0x8000000000000000)
   ```

3. `ll /usr/lib64/ | grep ssl`

   ```
   -rwxr-xr-x   1 root root   314528 5月  30 23:46 libssl3.so
   lrwxrwxrwx   1 root root       16 4月  20 11:56 libssl.so -> libssl.so.1.0.1e
   lrwxrwxrwx   1 root root       16 4月  20 11:54 libssl.so.10 -> libssl.so.1.0.1e
   -rwxr-xr-x   1 root root   446344 3月  23 05:46 libssl.so.1.0.1e
   drwxr-xr-x.  3 root root     4096 3月  23 05:45 openssl
   ```

   ​

以上是正确的环境。

有个同学环境不正确，py的ssl link到了`libssl.so.6`而本地并没有这个lib，同时强行`ln -s`的话，会`segment fault`。

最后建议是装一个6版本的openssl，或者重装个py。







