



```sh
docker tag --help

Usage:  docker tag SOURCE_IMAGE[:TAG] TARGET_IMAGE[:TAG]

Create a tag TARGET_IMAGE that refers to SOURCE_IMAGE
```

这里的source，可以是：

* `repository` （`name:tag`格式）
* image id



### 打tag



```sh
docker tag helloworld username/helloworld:v1
```



给镜像指定完整名，一般来说格式为： `<username>/<name>:<version>`

> yonka: 这里的helloworld算什么？ 如果多个叫这个的呢？
>
> 是原镜像名，如果不带tag部分的话则默认为`latest` 这个tag



### 删除tag： rmi

直接rmi即可，image的管理是在失去最后一个引用（tag）时删除

> 那没有tag的呢？