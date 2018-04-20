









# tutorial



## 小技巧



### 指定请求的host







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



















