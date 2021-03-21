



```sh
docker build --help

Usage:  docker build [OPTIONS] PATH | URL | -

Build an image from a Dockerfile

Options:
      --add-host list           Add a custom host-to-IP mapping (host:ip)
      --build-arg list          Set build-time variables
      --cache-from strings      Images to consider as cache sources
      --cgroup-parent string    Optional parent cgroup for the container
      --compress                Compress the build context using gzip
      --cpu-period int          Limit the CPU CFS (Completely Fair Scheduler) period
      --cpu-quota int           Limit the CPU CFS (Completely Fair Scheduler) quota
  -c, --cpu-shares int          CPU shares (relative weight)
      --cpuset-cpus string      CPUs in which to allow execution (0-3, 0,1)
      --cpuset-mems string      MEMs in which to allow execution (0-3, 0,1)
      --disable-content-trust   Skip image verification (default true)
  -f, --file string             Name of the Dockerfile (Default is 'PATH/Dockerfile')
      --force-rm                Always remove intermediate containers
      --iidfile string          Write the image ID to the file
      --isolation string        Container isolation technology
      --label list              Set metadata for an image
  -m, --memory bytes            Memory limit
      --memory-swap bytes       Swap limit equal to memory plus swap: '-1' to enable unlimited swap
      --network string          Set the networking mode for the RUN instructions during build (default "default")
      --no-cache                Do not use cache when building the image
      --pull                    Always attempt to pull a newer version of the image
  -q, --quiet                   Suppress the build output and print image ID on success
      --rm                      Remove intermediate containers after a successful build (default true)
      --security-opt strings    Security options
      --shm-size bytes          Size of /dev/shm
  -t, --tag list                Name and optionally a tag in the 'name:tag' format
      --target string           Set the target build stage to build.
      --ulimit ulimit           Ulimit options (default [])
```

* 直接build出来的镜像，没有`name`/`repository`和`tag`，只能ls然后inspect image id来看了

  这个应该是不能push的

* `-t`可以指定名，`name[:tag]`格式，不允许`:tag`

* dockerfile里好像没有指定镜像名的地方



```sh
 docker build -t helloworld .
```



### build时使用proxy

ref： https://stackoverflow.com/questions/27749193/how-to-build-docker-images-with-dockerfile-behind-http-proxy-by-jenkins

> **实在太麻烦了**
>
> 意思是：
>
> * 拉取镜像用`--build-arg`
> * RUN命令里的网络访问用`ENV`
> * config里配置proxy，但应该是对daemon生效而不是client
>
> **RUN apk add时，proxy怎么设置都不生效，即使RUN echo都打出来了**



Docker has multiple ways to set proxies that take effect at different times.

------

If your `docker build` has to **retrieve a base image through a proxy**, you'll want to specify `build-arg`s:

```xml
docker build --build-arg HTTP_PROXY=$http_proxy \
--build-arg HTTPS_PROXY=$http_proxy --build-arg NO_PROXY="$no_proxy" \
--build-arg http_proxy=$http_proxy --build-arg https_proxy=$http_proxy \
--build-arg no_proxy="$no_proxy" -t myContainer /path/to/Dockerfile/directory
```

where `$http_proxy` and `$no_proxy` were set in my bashrc. I used both `HTTP_PROXY` and `http_proxy` because different utilities will check different variables (`curl` checks both, `wget` only checks the lowercase ones, etc).

------

If your `docker build` has a **`RUN curl/wget/etc` command that has to go through the proxy**, you'll need to specify an environment variable inside your docker image:

```xml
ENV https_proxy=http://proxy-us02.company.com:8080
ENV http_proxy=http://proxy-us02.company.com:8080
ENV HTTP_PROXY=http://proxy-us02.company.com:8080
ENV HTTPS_PROXY=http://proxy-us02.company.com:8080
ENV no_proxy="localhost,localdomain,127.0.0.1,etc"
ENV NO_PROXY="localhost,localdomain,127.0.0.1,etc"
```

If you don't want this environment variable inside your image at runtime, you can remove all these at the end:

```xml
RUN unset http_proxy https_proxy no_proxy HTTP_PROXY HTTPS_PROXY NO_PROXY
```





```json
{
 "proxies":
 {
   "default":
   {
     "httpProxy": "http://127.0.0.1:3001",
     "httpsProxy": "http://127.0.0.1:3001",
     "ftpProxy": "http://127.0.0.1:3001",
     "noProxy": "*.test.example.com,.example2.com"
   }
 }
}
```





