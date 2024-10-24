









# tutorial



## encode



urlencode

`curl -v -s  --get --data-urlencode "cmd=ip addr" "httpbin:8000/cmd"`

> 但似乎得到了 `cmd=ip+addr`，跟实际escape的`cmd=ip%20addr`不同

> 不加`--get`则是post form方式



## proxy

`curl -x sock5://127.0.0.1:8888 www.baidu.com`



## retry

```sh
curl --connect-timeout 5 \
    --max-time 10 \
    --retry 5 \
    --retry-delay 0 \
    --retry-max-time 40 \
    'http://your_url'


--max-time 10     (how long each retry will wait)
--retry 5         (it will retry 5 times)
--retry-delay 0   (an exponential backoff algorithm)
--retry-max-time  (total time before it's considered failed)
```



> --retry
>
> If a transient error is returned when curl tries to perform a transfer, it will retry this number of times before giving up. Setting the number to 0 makes curl do no retries (which is the default). Transient error means either: a timeout, an FTP 4xx response code or an HTTP 5xx response code.
>
> When curl is about to retry a transfer, it will first wait one second and then for all forthcoming retries it will double the waiting time until it reaches 10 minutes which then will be the delay between the rest of the retries. By using --retry-delay you disable this exponential backoff algorithm. See also --retry-max-time to limit the total time allowed for retries.
>
> If this option is used several times, the last one will be used.
>
> Added in 7.12.3.s



## timeout

ref: https://unix.stackexchange.com/questions/94604/does-curl-have-a-timeout/94612

**Timeout parameters**

`curl` has two options: `--connect-timeout` and `--max-time`.

Quoting from the manpage:

```
--connect-timeout <seconds>
    Maximum  time  in  seconds  that you allow the connection to the
    server to take.  This only limits  the  connection  phase,  once
    curl has connected this option is of no more use.  Since 7.32.0,
    this option accepts decimal values, but the actual timeout  will
    decrease in accuracy as the specified timeout increases in deci‐
    mal precision. See also the -m, --max-time option.

    If this option is used several times, the last one will be used.
```

and:

```
-m, --max-time <seconds>
    Maximum  time  in  seconds that you allow the whole operation to
    take.  This is useful for preventing your batch jobs from  hang‐
    ing  for  hours due to slow networks or links going down.  Since
    7.32.0, this option accepts decimal values, but the actual time‐
    out will decrease in accuracy as the specified timeout increases
    in decimal precision.  See also the --connect-timeout option.

    If this option is used several times, the last one will be used.
```

**Defaults**

Here (on Debian) it stops trying to connect after 2 minutes, regardless of the time specified with `--connect-timeout` and although the default connect timeout value seems to be *5 minutes* according to the `DEFAULT_CONNECT_TIMEOUT` macro in [lib/connect.h](https://github.com/curl/curl/blob/master/lib/connect.h).

A default value for `--max-time` doesn't seem to exist, making `curl` wait forever for a response if the initial connect succeeds.

**What to use?**

You are probably interested in the latter option, `--max-time`. For your case set it to `900` (15 minutes).

Specifying option `--connect-timeout` to something like `60` (one minute) might also be a good idea. Otherwise `curl` will try to connect again and again, apparently using some backoff algorithm.







## method



```sh
curl -X POST
```



## body



```sh
curl -data "xxx"

curl -d @${filePath} "xxx"  # 从${filePath}指定的路径读取文件作为body
```



## proxy



Ref: https://www.cnblogs.com/zafu/p/10000188.html



* env

  ```sh
  export http_proxy=http://your-ip-address:port/
  export http_proxy=http://user:password@your-proxy-ip-address:port/
  
  
  export https_proxy=https://your-ip-address:port/
  export https_proxy=https://user:password@your-proxy-ip-address:port/
  ```

* 命令行选项

  ```sh
  curl -x <[protocol://][user:password@]proxyhost[:port]> url
  --proxy <[protocol://][user:password@]proxyhost[:port]> url
  --proxy http://user:password@Your-Ip-Hereort url
  -x http://user:password@Your-Ip-Hereort url
  
  # 可以alias：
  alias curl = "curl -x server1.cyberciti.biz:3128"
  ```

* curl配置

  ```sh
  vi ~/.curlrc
  # add:
  proxy = server1.cyberciti.biz:3128
  proxy-user = "foo:bar"
  ```

  





## http2



### upgrade模式： --http2



### 直接发http2： -http2-prior-knowledge





## 小技巧



### 指定请求的host: -H （`-H "Host: xxx"`）



`-H`

可以多次出现以指定多个header。如： `-H "header1: xxx" -H "header2: yyy"`



```sh
       -H, --header <header>
              (HTTP) Extra header  to  include  in  the
              request  when  sending  HTTP to a server.
              You may specify any number of extra head‐
              ers. Note that if you should add a custom
              header that has the same name as  one  of
              the  internal  ones  curl would use, your
              externally  set  header  will   be   used
              instead  of the internal one. This allows
              you to make even trickier stuff than curl
              would normally do. You should not replace
              internally set  headers  without  knowing
              perfectly  well what you're doing. Remove
              an internal header by giving  a  replace‐
              ment without content on the right side of
              the colon, as in: -H "Host:". If you send
              the  custom header with no-value then its
              header must be terminated  with  a  semi‐
              colon,  such  as -H "X-Custom-Header;" to
              send "X-Custom-Header:".

              curl will make sure that each header  you
              add/replace  is sent with the proper end-
              of-line marker, you should thus  not  add
              that  as a part of the header content: do
              not add  newlines  or  carriage  returns,
              they will only mess things up for you.

              See  also  the  -A,  --user-agent and -e,
              --referer options.

              Starting in  7.37.0,  you  need  --proxy-
              header  to  send  custom headers intended
              for a proxy.

              Example:

              #    curl    -H    "X-First-Name:    Joe"
              http://192.168.0.1/

              WARNING:  headers  set  with  this option
              will be set in all requests - even  after
              redirects  are  followed,  like when told
              with -L, --location. This can lead to the
              header being sent to other hosts than the
              original  host,  so   sensitive   headers
              should be used with caution combined with
              following redirects.

              This option can be used multiple times to
              add/replace/remove multiple headers.              
```





### 不输出进度条: -s



```sh
       -s, --silent
              Silent or quiet mode. Don't show progress
              meter  or  error  messages.   Makes  Curl
              mute. It will still output the  data  you
              ask  for,  potentially even to the termi‐
              nal/stdout unless you redirect it.
```





### 指定发出流量的接口： --interface

如： `--interface eth0`



## 场景



### ssl证书问题

类似的问题：

```
*   Trying x.x.x.x...
* TCP_NODELAY set
* Connected to xx.xx.xx (x.x.x.x) port 443 (#0)
* ALPN, offering h2
* ALPN, offering http/1.1
* Cipher selection: ALL:!EXPORT:!EXPORT40:!EXPORT56:!aNULL:!LOW:!RC4:@STRENGTH
* successfully set certificate verify locations:
*   CAfile: /etc/ssl/certs/ca-certificates.crt
  CApath: none
* TLSv1.2 (OUT), TLS header, Certificate Status (22):
* TLSv1.2 (OUT), TLS handshake, Client hello (1):
* TLSv1.2 (IN), TLS handshake, Server hello (2):
* TLSv1.2 (IN), TLS handshake, Certificate (11):
* TLSv1.2 (OUT), TLS alert, Server hello (2):
* SSL certificate problem: unable to get local issuer certificate
* stopped the pause stream!
* Closing connection 0
```





```
-k, --insecure

(TLS) By default, every SSL connection curl makes is verified to be secure. This option allows curl to proceed and operate even for server connections otherwise considered insecure.

The server connection is verified by making sure the server's certificate contains the right name and verifies successfully using the cert store.

See this online resource for further details: https://curl.haxx.se/docs/sslcerts.html

See also --proxy-insecure and --cacert.
```



#### 指定ca： `--cacert`

**用于对server的验证：额外信任签发server cert的ca**

使用场景还比较多，因为内部通讯一般不会权威ca签发

该指定file里可以有多个certs



curl的cacert顺序（优先级递减）：

* `--cacert`
* env `CURL_CA_BUNDLE`
* 默认build带的



#### 指定cert/key： `--cert`/`--key`

**用于client侧应对server的验证**



### follow redirect

```
-L, --location

(HTTP) If the server reports that the requested page has moved to a different location (indicated with a Location: header and a 3XX response code), this option will make curl redo the request on the new place. If used together with -i, --include or -I, --head, headers from all requested pages will be shown. When authentication is used, curl only sends its credentials to the initial host. If a redirect takes curl to a different host, it won't be able to intercept the user+password. See also --location-trusted on how to change this. You can limit the amount of redirects to follow by using the --max-redirs option.

When curl follows a redirect and the request is not a plain GET (for example POST or PUT), it will do the following request with a GET if the HTTP response was 301, 302, or 303. If the response code was any other 3xx code, curl will re-send the following request using the same unmodified method.

You can tell curl to not change the non-GET request method to GET after a 30x response by using the dedicated options for that: --post301, --post302 and --post303.
```





* `-L` 开启follow
* `--max-redirs` 指定最大redirect次数
* `--post301, --post302 --post303` 指定对于这几个code不会转为GET(否则会重定向为GET)



example： `-L --max-redirs 8 --post3-1 --post302 --post303`





### 把“请求失败”体现在exitCode

Ref: [Can I make cURL fail with an exitCode different than 0 if the HTTP status code is not 200?](https://superuser.com/questions/590099/can-i-make-curl-fail-with-an-exitcode-different-than-0-if-the-http-status-code-i)





* `-f, --fail`

  可以实现，但会干掉output

* `--show-error`

  和`-f`组合使用，可以打印些许信息：

  ```shell
  curl: (22) The requested URL returned error: 404 Not Found
  ```

  



```shell

```



### curl Expect:100-continue 问题

ref: https://blog.csdn.net/fdipzone/article/details/42463727

使用curl POST数据时，如果POST的数据大于1024字节，curl并不会直接就发起POST请求。而是会分两步。

1.发送一个请求，header中包含一个Expect:100-continue，询问Server是否愿意接受数据。

2.接受到Server返回的100-continue回应后，才把数据POST到Server。



这个是libcurl定义的，具体可以查看相关描述：http://www.w3.org/Protocols/rfc2616/rfc2616-sec8.html#sec8.2.3



于是这样就会出现一个问题。并不是所有的Server都会回应100-continue的。例如lighttpd，会返回"417 Expectation Fail"，会造成逻辑错误。



解决方法如下，就是发送请求时，header中包含一个空的Expect。

```php
curl_setopt($ch, CURLOPT_HTTPHEADER, array("Expect:"));
```

也即:
```sh
curl -H "Expect:"
```



## features



### http2 support



#### mac上增加支持



Ref: https://simonecarletti.com/blog/2016/01/http2-curl-macosx/



```sh
# install cURL with nghttp2 support
➜  brew install curl --with-nghttp2

# link the formula to replace the system cURL
➜  brew link curl --force

# now reload the shell

# test an HTTP/2 request passing the --http2 flag
➜  curl -I --http2 https://www.cloudflare.com/
```



#### ubuntu上增加支持



```sh
# https://askubuntu.com/questions/884899/how-do-i-install-curl-with-http2-support

sudo apt-get install build-essential nghttp2 libnghttp2-dev libssl-dev
wget https://curl.haxx.se/download/curl-7.58.0.tar.gz
tar -xvf curl-7.58.0.tar.gz
cd curl-7.58.0
./configure --with-nghttp2 --prefix=/usr/local --with-ssl=/usr/local/ssl
make
sudo make install
sudo ldconfig
```



























