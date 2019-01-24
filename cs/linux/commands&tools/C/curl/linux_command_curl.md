









# tutorial





## method



```sh
curl -X POST
```



## body



```sh
curl -data "xxx"
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



### 把“请求失败”体现在exitCode

Ref: [Can I make cURL fail with an exitCode different than 0 if the HTTP status code is not 200?](https://superuser.com/questions/590099/can-i-make-curl-fail-with-an-exitcode-different-than-0-if-the-http-status-code-i)





* `-f, --fail`

  可以实现，但会干掉output

* `--show-error`

  和`-f`组合使用，可以打印些许信息：

  ```shell
  curl: (22) The requested URL returned error: 404 Not Found
  ```

  ​



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



























