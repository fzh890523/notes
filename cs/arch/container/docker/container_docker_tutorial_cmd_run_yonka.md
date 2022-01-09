



https://docs.docker.com/engine/reference/commandline/run/



```sh
docker run -d <image>  # run in detach(from stdout/stdin) mode
```



### 参数



| Name, shorthand           | Default   | Description                                                  |
| ------------------------- | --------- | ------------------------------------------------------------ |
| `--add-host`              |           | Add a custom host-to-IP mapping (host:ip)                    |
| `--attach , -a`           |           | Attach to STDIN, STDOUT or STDERR                            |
| `--blkio-weight`          |           | Block IO (relative weight), between 10 and 1000, or 0 to disable (default 0) |
| `--blkio-weight-device`   |           | Block IO weight (relative device weight)                     |
| `--cap-add`               |           | Add Linux capabilities                                       |
| `--cap-drop`              |           | Drop Linux capabilities                                      |
| `--cgroup-parent`         |           | Optional parent cgroup for the container                     |
| `--cgroupns`              |           | [**API 1.41+**](https://docs.docker.com/engine/api/v1.41/) Cgroup namespace to use (host\|private) 'host': Run the container in the Docker host's cgroup namespace 'private': Run the container in its own private cgroup namespace '': Use the cgroup namespace as configured by the default-cgroupns-mode option on the daemon (default) |
| `--cidfile`               |           | Write the container ID to the file                           |
| `--cpu-count`             |           | CPU count (Windows only)                                     |
| `--cpu-percent`           |           | CPU percent (Windows only)                                   |
| `--cpu-period`            |           | Limit CPU CFS (Completely Fair Scheduler) period             |
| `--cpu-quota`             |           | Limit CPU CFS (Completely Fair Scheduler) quota              |
| `--cpu-rt-period`         |           | [**API 1.25+**](https://docs.docker.com/engine/api/v1.25/) Limit CPU real-time period in microseconds |
| `--cpu-rt-runtime`        |           | [**API 1.25+**](https://docs.docker.com/engine/api/v1.25/) Limit CPU real-time runtime in microseconds |
| `--cpu-shares , -c`       |           | CPU shares (relative weight)                                 |
| `--cpus`                  |           | [**API 1.25+**](https://docs.docker.com/engine/api/v1.25/) Number of CPUs |
| `--cpuset-cpus`           |           | CPUs in which to allow execution (0-3, 0,1)                  |
| `--cpuset-mems`           |           | MEMs in which to allow execution (0-3, 0,1)                  |
| `--detach , -d`           |           | Run container in background and print container ID           |
| `--detach-keys`           |           | Override the key sequence for detaching a container          |
| `--device`                |           | Add a host device to the container                           |
| `--device-cgroup-rule`    |           | Add a rule to the cgroup allowed devices list                |
| `--device-read-bps`       |           | Limit read rate (bytes per second) from a device             |
| `--device-read-iops`      |           | Limit read rate (IO per second) from a device                |
| `--device-write-bps`      |           | Limit write rate (bytes per second) to a device              |
| `--device-write-iops`     |           | Limit write rate (IO per second) to a device                 |
| `--disable-content-trust` | `true`    | Skip image verification                                      |
| `--dns`                   |           | Set custom DNS servers                                       |
| `--dns-opt`               |           | Set DNS options                                              |
| `--dns-option`            |           | Set DNS options                                              |
| `--dns-search`            |           | Set custom DNS search domains                                |
| `--domainname`            |           | Container NIS domain name                                    |
| `--entrypoint`            |           | Overwrite the default ENTRYPOINT of the image                |
| `--env , -e`              |           | Set environment variables                                    |
| `--env-file`              |           | Read in a file of environment variables                      |
| `--expose`                |           | Expose a port or a range of ports                            |
| `--gpus`                  |           | [**API 1.40+**](https://docs.docker.com/engine/api/v1.40/) GPU devices to add to the container ('all' to pass all GPUs) |
| `--group-add`             |           | Add additional groups to join                                |
| `--health-cmd`            |           | Command to run to check health                               |
| `--health-interval`       |           | Time between running the check (ms\|s\|m\|h) (default 0s)    |
| `--health-retries`        |           | Consecutive failures needed to report unhealthy              |
| `--health-start-period`   |           | [**API 1.29+**](https://docs.docker.com/engine/api/v1.29/) Start period for the container to initialize before starting health-retries countdown (ms\|s\|m\|h) (default 0s) |
| `--health-timeout`        |           | Maximum time to allow one check to run (ms\|s\|m\|h) (default 0s) |
| `--help`                  |           | Print usage                                                  |
| `--hostname , -h`         |           | Container host name                                          |
| `--init`                  |           | [**API 1.25+**](https://docs.docker.com/engine/api/v1.25/) Run an init inside the container that forwards signals and reaps processes |
| `--interactive , -i`      |           | Keep STDIN open even if not attached                         |
| `--io-maxbandwidth`       |           | Maximum IO bandwidth limit for the system drive (Windows only) |
| `--io-maxiops`            |           | Maximum IOps limit for the system drive (Windows only)       |
| `--ip`                    |           | IPv4 address (e.g., 172.30.100.104)                          |
| `--ip6`                   |           | IPv6 address (e.g., 2001:db8::33)                            |
| `--ipc`                   |           | IPC mode to use                                              |
| `--isolation`             |           | Container isolation technology                               |
| `--kernel-memory`         |           | Kernel memory limit                                          |
| `--label , -l`            |           | Set meta data on a container                                 |
| `--label-file`            |           | Read in a line delimited file of labels                      |
| `--link`                  |           | Add link to another container                                |
| `--link-local-ip`         |           | Container IPv4/IPv6 link-local addresses                     |
| `--log-driver`            |           | Logging driver for the container                             |
| `--log-opt`               |           | Log driver options                                           |
| `--mac-address`           |           | Container MAC address (e.g., 92:d0:c6:0a:29:33)              |
| `--memory , -m`           |           | Memory limit                                                 |
| `--memory-reservation`    |           | Memory soft limit                                            |
| `--memory-swap`           |           | Swap limit equal to memory plus swap: '-1' to enable unlimited swap |
| `--memory-swappiness`     | `-1`      | Tune container memory swappiness (0 to 100)                  |
| `--mount`                 |           | Attach a filesystem mount to the container                   |
| `--name`                  |           | Assign a name to the container                               |
| `--net`                   |           | Connect a container to a network                             |
| `--net-alias`             |           | Add network-scoped alias for the container                   |
| `--network`               |           | Connect a container to a network                             |
| `--network-alias`         |           | Add network-scoped alias for the container                   |
| `--no-healthcheck`        |           | Disable any container-specified HEALTHCHECK                  |
| `--oom-kill-disable`      |           | Disable OOM Killer                                           |
| `--oom-score-adj`         |           | Tune host's OOM preferences (-1000 to 1000)                  |
| `--pid`                   |           | PID namespace to use                                         |
| `--pids-limit`            |           | Tune container pids limit (set -1 for unlimited)             |
| `--platform`              |           | [**API 1.32+**](https://docs.docker.com/engine/api/v1.32/) Set platform if server is multi-platform capable |
| `--privileged`            |           | Give extended privileges to this container                   |
| `--publish , -p`          |           | Publish a container's port(s) to the host                    |
| `--publish-all , -P`      |           | Publish all exposed ports to random ports                    |
| `--pull`                  | `missing` | Pull image before running ("always"\|"missing"\|"never")     |
| `--read-only`             |           | Mount the container's root filesystem as read only           |
| `--restart`               | `no`      | Restart policy to apply when a container exits               |
| `--rm`                    |           | Automatically remove the container when it exits             |
| `--runtime`               |           | Runtime to use for this container                            |
| `--security-opt`          |           | Security Options                                             |
| `--shm-size`              |           | Size of /dev/shm                                             |
| `--sig-proxy`             | `true`    | Proxy received signals to the process                        |
| `--stop-signal`           | `SIGTERM` | Signal to stop a container                                   |
| `--stop-timeout`          |           | [**API 1.25+**](https://docs.docker.com/engine/api/v1.25/) Timeout (in seconds) to stop a container |
| `--storage-opt`           |           | Storage driver options for the container                     |
| `--sysctl`                |           | Sysctl options                                               |
| `--tmpfs`                 |           | Mount a tmpfs directory                                      |
| `--tty , -t`              |           | Allocate a pseudo-TTY                                        |
| `--ulimit`                |           | Ulimit options                                               |
| `--user , -u`             |           | Username or UID (format: <name\|uid>[:<group\|gid>])         |
| `--userns`                |           | User namespace to use                                        |
| `--uts`                   |           | UTS namespace to use                                         |
| `--volume , -v`           |           | Bind mount a volume                                          |
| `--volume-driver`         |           | Optional volume driver for the container                     |
| `--volumes-from`          |           | Mount volumes from the specified container(s)                |
| `--workdir , -w`          |           | Working directory inside the container                       |



* `--network` 把容器加入network

  `--network container:<container_id>`，这个可以用name或者id

  如果直接`--net host`则不使用ns直接使用宿主机网络栈

  ```sh
  docker network create test-net
  docker run -d --network test-net nginx
  docker run -d --network test-net xx  # 可以访问nginx了， 也可以 --network container:nginx
  ```

  

* `--expose` “暴露”端口，其实**只是个声明的描述作用**

  可以指定多个端口，也可以指定端口范围。 与dockerfile的`EXPOSE`指令等效

  **并不会真的“暴露”到host，那是publish干的事情**

  ```sh
  --export 2000-3000
  ```

  ```dockerfile
  EXPOSE 80 8080 8001
  ```

  

* `-it` interactive

* `--rm` 停止运行后，自动删除容器文件

* `-d` 容器启动后，在后台运行

* `--name <name>` 容器的名字叫

* `--env <name>=<value>`/`-e` 传入环境变量，可以多次指定

  ```sh
  -e REDIS_NAMESPACE='staging' \ 
  -e POSTGRES_ENV_POSTGRES_PASSWORD='foo' \
  -e POSTGRES_ENV_POSTGRES_USER='bar' \
  -e POSTGRES_ENV_DB_NAME='mysite_staging'
  ```

  

* `-v`/`--volume <local_path>:<container_path>` 将目录映射到容器的目录

  * 可以指定读写权限，如： `-v /tmp:/tmp:ro`，默认rw

  * 如果只指定一个path，如`-v /tmp`，则创建一个**匿名卷**... 而**不等于**`-v /tmp:/tmp`，等于 `-v :/tmp`。 指定读写权限时为了不产生歧义，需要： `-v :/tmp:ro`
  * 可以是文件，如： `-v /tmp/xx.txt:/tmp/a/xx.txt`  （自动创建`/tmp/a`目录）

  > 与`mount`命令行为不同，docker会额外创建不存在的挂载目录以供挂载

  > 似乎不能是relative path

* `--read-only`  把root（`/`）挂载为ro，此时只有额外挂载为rw的卷才能写入

* `--tmpfs` 挂载tmpfs

  `--tmpfs /run:rw,noexec,nosuid,size=65536k`

* `-p`/`--publish` 把指定容器端口expose到host，`-p <hostPort>:<containerPort>` 如 `-p 80:8080`

  > docker ps的输出 PORTS field：
  >
  > * `10000/tcp` 没expose
  > * `0.0.0.0:32768->10000/tcp` expose为32768

  * `-p 80` 这样会expose到随机端口

  * 还可以指定bind addr，如： `-p 192.168.1.100:80:8080 -p 127.0.0.1::8081`

* `-P`/`--publish-all` 所有容器端口都expose到host（随机端口）

* `--link <container>:<alias>` 当前容器连接到指定容器

  使得源容器（被链接的容器）和接收容器（主动去链接的容器）之间可以互相通信，并且接收容器可以获取源容器的一些数据，如源容器的环境变量。

  * 通信方式可以通过容器名或者这里的别名来dns解析到ip然后访问，实际是更新了hosts文件
  * 环境变量是以： `<alias>_NAME` 的名字继承过来

  > **deprecated**
  >
  > 官方建议create network来连接容器。 后续会删掉该支持。

* `--entrypoint` 覆盖原来entrypoint

  清空的话，`--entrypoint=''` 

* `--cap-add` 为容器增加能力支持

  如要能正常运行iptables需要`--cap-add=NET_ADMIN`，否则报错类似

  ```sh
  can't initialize iptables table `filter': Permission denied (you must be root)
  ```

* `--privileged` 完全容器能力

  > The --privileged flag gives all capabilities to the container, and it also lifts all the limitations enforced by the device cgroup controller. In other words, the container can then do almost everything that the host can do. This flag exists to allow special use-cases, like running Docker within Docker.

* `--user`/`-u` 指定user或uid（覆盖dockerfile里的）

  可用于debug，毕竟容器本身可能非root用户，有些操作不方便

* `-w` 设置working dir

  `-w /path/to/dir`

* `-restart <policy>` 设置重启的策略

  可选的有：

  | Policy                       | Result                                                       |
  | :--------------------------- | :----------------------------------------------------------- |
  | **no**                       | Do not automatically restart the container when it exits. This is the default. |
  | **on-failure**[:max-retries] | Restart only if the container exits with a non-zero exit status. Optionally, limit the number of restart retries the Docker daemon attempts. |
  | **always**                   | Always restart the container regardless of the exit status. When you specify always, the Docker daemon will try to restart the container indefinitely. The container will also always start on daemon startup, regardless of the current state of the container. |
  | **unless-stopped**           | Always restart the container regardless of the exit status, including on daemon startup, except if the container was put into a stopped state before the Docker daemon was stopped. |

  > 意思是daemon stop之后干掉container的话，daemon感知不到，所以不算，也即下次daemon还会启动。
  >
  > 只有“明确告知”daemon停掉容器，才会...
  >
  > **看起来unless这个比always这个好用**



examples：

* ```sh
  docker container run \
    --rm \
    --name wordpress \
    --volume "$PWD/":/var/www/html \
    --link wordpressdb:mysql \
    phpwithmysql
  ```

* ```sh
  docker container run \
    -d \
    --rm \
    --name wordpressdb \
    --env MYSQL_ROOT_PASSWORD=123456 \
    --env MYSQL_DATABASE=wordpress \
    mysql:5.7
  ```

* ```sh
  docker container run \
    --rm \
    --name wordpress \
    --volume "$PWD/":/var/www/html \
    php:5.6-apache
  ```

* 

* 



# tutorial



## 容器“auto-start”

用`--restart`策略来控制

