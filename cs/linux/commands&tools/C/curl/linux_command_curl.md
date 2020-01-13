









# tutorial





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



























