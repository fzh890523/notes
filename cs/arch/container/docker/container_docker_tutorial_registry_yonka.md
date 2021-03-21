

# tutorial



## 搭建registry

ref:

* https://docs.docker.com/registry/deploying/

* [基于Docker搭建私有镜像仓库](https://www.cnblogs.com/niceyoo/p/13058238.html)

样例：

```sh
docker run -d -p 5000:5000 --restart=always --name registry  -v /var/data/docker_registry:/var/lib/registry registry:2
# data目录可以按需设置
```



## 使用



1. `docker pull busybox`

2. `docker tag busybox <domain_or_ip>:5000/<username>/busybox`

   需要专门re-tag+push。 暂时没找到“让registry自己下载镜像”的方式，registry as mirror勉强算，但还是要有一方拉取来触发，数据还是多写到拉取方本地了

3. `docker push  <domain_or_ip>:5000/<username>/busybox`

4. （拉取处） `docker pull <domain_or_ip>:5000/<username>/busybox`

   这时镜像的name或者说repository已经是 `<domain_or_ip>:5000/<username>/busybox` 而不是 `busybox`了，要想一样只能再次`docker tag`



> 可以看到，private registry跟default用起来还是不小的差异的。 docker公司可能一直在强调/维持自身的特殊位置
>
> 要想达到一样的效果，可以参考后面的 **registry as pull-through cache**



## registry as pull-through cache（registry as mirror）

参考： [Registry as a pull through cache](https://docs.docker.com/registry/recipes/mirror/)

* registry端： 配置好proxy目标

  `/etc/docker/registry/config.yml`

  ```yaml
  proxy:
    remoteurl: https://registry-1.docker.io
    username: [username]
    password: [password]
  ```

  > 不配似乎也没问题，如果要访问私有镜像的话则需要配置

* 拉取端： mirror配置为该registry

  好像可以配多个，这样并发请求可能调度到不同mirror上



# api

## v2

详见： https://github.com/distribution/distribution/blob/main/docs/spec/api.md

> | title       | description                         | keywords                                                     | redirect_from                |
> | ----------- | ----------------------------------- | ------------------------------------------------------------ | ---------------------------- |
> | HTTP API V2 | Specification for the Registry API. | registry, on-prem, images, tags, repository, distribution, api, advanced | /reference/api/registry_api/ |



* `curl -X GET https://myregistry:5000/v2/_catalog`

  ```json
  {"repositories":["redis","ubuntu"]}
  ```

  参数：

  * `n=100` page size
  * `last=pro-octopus-ws` 语义是： a page since ...

* `curl -X GET https://myregistry:5000/v2/ubuntu/tags/list`

  ```json
  {"name":"ubuntu","tags":["14.04"]}
  ```

  

