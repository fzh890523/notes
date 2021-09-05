

# tutorial



https://blog.golang.org/using-go-modules



This post is part 1 in a series.

- **Part 1 — Using Go Modules** (this post)
- Part 2 — [Migrating To Go Modules](https://blog.golang.org/migrating-to-go-modules)
- Part 3 — [Publishing Go Modules](https://blog.golang.org/publishing-go-modules)
- Part 4 — [Go Modules: v2 and Beyond](https://blog.golang.org/v2-go-modules)



## 新手指南



### 从dep迁移

ref： 

* https://blog.callr.tech/migrating-from-dep-to-go-1.11-modules/

* https://blog.golang.org/migrating-to-go-modules



- move your code outside of `GOPATH`
- `go mod init [module path]`: this will import dependencies from `Gopkg.lock`.
- `go mod tidy`: this will remove unnecessary imports, and add indirect ones.
- `rm -fr vendor/`
- `go build`: is everthing ok?
- `rm -f Gopkg.lock Gopkg.toml`
- `git commit -m 'chore(dep): migrated from dep to Go 1.11 modules'`



```sh
# Migration
go mod init gitlab.callr.tech/platform/asterisk-pbx-agi
rm -fr Gopkg.* vendor/

# Updating your CI
## 主要是不用关心项目与GOPATH的关系、以及显式dep ensure了。。 直接 go build就行，"go build will handle the dependencies automatically"
```



### 如果在goland里使用vendor编译

也即这时要禁用 go mod。



1. 项目结构要满足GOPATH

   如: `${base}/src/ xx.com/xx`

2. 加环境变量 GOPATH把`${base}`加入

3. 加环境变量 `GO111MODULE=off`



即可



### 问题处理

#### symbol无法识别



当你引入了一个包

第一种方式就是go build时会自动在这个包下载安装后再运行
 第二种方式就是把鼠标放在这个包上 alt+shift+enter 会下载安装

mac上option+enter可以sync package。



#### gopath关系

go modules就不依赖项目在gopath下了，就不需要别扭的`projectBase/src/xxx.com/projectName`然后把`projectBase`加到gopath了。

相反，如果加了的话，go mod的包会存到这个path下，反而重复。 使用默认的全局gopath即可。



#### invalid: unknown revision 问题



```
x.x.x.x/yy d98c69345dc
```

如果rev不在任何一个分支上，那么是不行的。 一般是因为remote上该分支被删除了。
可以打个tag，然后用tag代替rev来解。。。


#### verifying ... reading ... 410 Gone 问题

类似如下报错

```sh
verifying github.com/rasteric/minidb@v0.0.7: github.com/rasteric/minidb@v0.0.7: reading https://sum.golang.org/lookup/github.com/rasteric/minidb@v0.0.7: 410 Gone
```

是因为go mod会有个校验的过程，而对于私有repo，golang.org没有该信息也无法访问，所以需要skip，方式是`GOPRIVATE`环境变量，如下：
```sh
GOPRIVATE=*.xx.com,yy.com go build
```

ref: https://golang.org/cmd/go/#hdr-Module_configuration_for_non_public_modules


#### go get ...: no matching versions for query "latest" 问题

ref： https://github.com/golang/go/issues/27215

I had the same (or very similar) problem building a module and using GO111MODULE=off wasn't an option for me since I wanted to use modules.
I solved this running `go clean -modcache`.

还是出现的话，可以查一下是不是import的包不对，遇到过因为版本更迭合并代码导致老的错误import包没有改完于是出现该报错的情况（因为差别细微肉眼一直没看出来）。


#### 如果 -> A -> B，A和B都不支持go mod时，依赖如何处理？

好像，自动取了最新的commit，而没有去解析dep的。。。 我。。。
直接依赖好像要写明。。。


## command usage

* `go mod [command]`
* `go list -m all`



```sh
go mod                                                                                                                  feature-mg-istio-1.4 
Go mod provides access to operations on modules.

Note that support for modules is built into all the go commands,
not just 'go mod'. For example, day-to-day adding, removing, upgrading,
and downgrading of dependencies should be done using 'go get'.
See 'go help modules' for an overview of module functionality.

Usage:

        go mod <command> [arguments]

The commands are:

        download    download modules to local cache
        edit        edit go.mod from tools or scripts
        graph       print module requirement graph
        init        initialize new module in current directory
        tidy        add missing and remove unused modules
        vendor      make vendored copy of dependencies
        verify      verify dependencies have expected content
        why         explain why packages or modules are needed

Use "go help mod <command>" for more information about a command.
```



#### go mod download

`go mod download github.com/Microsoft/hcsshim@v0.8.8-0.20200421182805-c3e488f0d815`

> 不带版本号的话似乎会去download 依赖分析的那个版本，具体参见 go mod help download



#### go mod init



```sh
go help mod init

usage: go mod init [module]

Init initializes and writes a new go.mod to the current directory,
in effect creating a new module rooted at the current directory.
The file go.mod must not already exist.
If possible, init will guess the module path from import comments
(see 'go help importpath') or from version control configuration.
To override this guess, supply the module path as an argument.
```



> 据说还会 this will import dependencies from `Gopkg.lock`.



#### go mod vendor



```sh
go mod vendor
go -mod=readonly mod vendor  # 据说只collect-to-vendor而不会做update，但不知道生效不。 有说法go mod会忽略 `-mod` 选项
```







# background



## vs vgo



vgo -> go (mod)



新的功能在go上面，backport到vgo。



golang的`vgo executable` 里就是：

* Project SDK

  如果是不支持go modules的低版本，则这个不可用

* vgo binary（如 `${GOPATH}/bin/vgo`）

# intro

### mod路径： `${GOPATH}/pkg/mod`

GOPATH多个的话，取第一个。



## GO111MODULE环境变量



ref：

* [cmd/go: default to GO111MODULE=auto (with changes) for Go 1.13 #31857](https://github.com/golang/go/issues/31857)
* [go wiki Modules](https://github.com/golang/go/wiki/Modules)

* [Why is GO111MODULE everywhere, and everything about Go Modules](https://dev.to/maelvls/why-is-go111module-everywhere-and-everything-about-go-modules-24k#-raw-go111module-endraw-with-go-113)



`GO111MODULE` with Go 1.11 and 1.12

- `GO111MODULE=on` will force using Go modules even if the project is in your GOPATH. Requires `go.mod` to work.
- `GO111MODULE=off` forces Go to behave the GOPATH way, even outside of GOPATH.
- `GO111MODULE=auto` is the default mode. In this mode, Go will behave
  - similarly to `GO111MODULE=on` when you are outside of `GOPATH`,
  - similarly to `GO111MODULE=off` when you are inside the `GOPATH` even if a `go.mod` is present.



Using Go 1.13, `GO111MODULE`'s default (`auto`) changes:

- behaves like `GO111MODULE=on` anywhere there is a `go.mod` OR anywhere outside the GOPATH even if there is no `go.mod`. So you can keep all your repositories in your GOPATH with Go 1.13.
- behaves like `GO111MODULE=off` in the GOPATH with no `go.mod`.



## go mod使用vendor



```sh
go build -mod=vendor 
```

> 似乎从某个版本开始需要这样还是？



> go help mod
>
> 
>
>         -mod mode
>                 module download mode to use: readonly, vendor, or mod.
>                 By default, if a vendor directory is present and the go version in go.mod
>                 is 1.14 or higher, the go command acts as if -mod=vendor were set.
>                 Otherwise, the go command acts as if -mod=readonly were set.
>                 See https://golang.org/ref/mod#build-commands for details.



* go mod版本
  * `>=1.14` + 有vendor目录： 等于 `-mod=vendor`
  * else： 等于 `-mod=readonly`
* 老版本： 需要项目在gopath下vendor目录才能生效



## go.mod



https://stackoverflow.com/questions/65921916/why-does-go-module-ssh-custom-private-repo-non-github-config-still-request-htt



In short, the answer is to use `.git` suffix in all places. Without `.git` suffix, `go mod tidy` and `go get` will use `https` instead of `ssh` (git).

**At Client:**

The file `~/.gitconfig` (at linux) if you use `/repopath/foo.git` path at server:

```golang
[url "ssh://user@private.com"]
    insteadOf = https://private.com
```

The file `~/.gitconfig` (at linux) if you use `~/repopath/foo.git` path at server:

```golang
[url "user@private.com:"]
    insteadOf = https://private.com/
```

Execute the following to update `~/.config/go/env` at linux:

```golang
go env -w GOPRIVATE=private.com
```

In `go.mod`, it should use

```golang
require private.com/repopath/foo.git v0.1.0
```

In `file.go`, it should be

```golang
import private.com/repopath/foo.git
```

**At SSH Server**

in `foo.git/go.mod` at private server should have:

```golang
module private.com/repopath/foo.git
```

And make sure the git repo at server has tag version `v0.1.0`. Don't forget to use `git push --tags` at client to update the tag version to the server. Without `--tags`, tag version will not be pushed.

After adding `.git` suffix to all the required places, `go mod tidy` and `go get` will no longer send https request.











