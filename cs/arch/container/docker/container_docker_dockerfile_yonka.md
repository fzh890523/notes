



### ENTRYPOINT vs CMD

二者都有两种格式：

- **Shell form** 
  - `CMD echo "Hello World"`
  - `ENTRYPOINT echo "Hello World"`
- **Exec form**
  - `CMD ["echo", "Hello World"]`
  - `ENTRYPOINT ["echo", "Hello World"]`

> shell form其实都会被翻译为exec form
>
> exec form更准确，不容易出错



* 1

  ```
  CMD ["ls", "-l"]
  ```

  ```
              "Entrypoint": null,
              
              "Cmd": [
                  "/bin/sh",
                  "-c",
                  "#(nop) ",
                  "CMD [\"ls\" \"-l\"]"
              ],
  ```

  正常的`-l`输出。 估计此时会特殊处理把cmd内容拼接给... 因为docker知道是`-c`执行

* 2

  ```
  ENTRYPOINT ["/bin/bash", "-c"]
  CMD ["ls", "-l"]
  ```

  ```
              "Entrypoint": [
                  "/bin/bash",
                  "-c"
              ],
              
              "Cmd": [
                  "/bin/sh",
                  "-c",
                  "#(nop) ",
                  "CMD [\"ls\" \"-l\"]"
              ],            
  ```

  ls的短输出，也即`-l`丢失了，`/bin/bash -c`只读了后面的第一个参数，行为符合预期



### 文件路径

* 只能用rel path

  `COPY /etc/xx.conf /etc/` 会报错找不到，需要：

  ```sh
  cp /etc/xx.conf ./
  
  # dockerfile里
  COPY ./xx.conf /etc/
  # 没有./也行
  ```

  

* 不能超出当前目录，也即`../../xx.txt`不行





