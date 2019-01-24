



* https://github.com/golang/dep





## 安装



* mac
  * `brew install dep`
  * `brew upgrade dep`



`go get -u github.com/golang/dep/cmd/dep`



## 使用



`dep ensure`



`dep ensure -v -update`



`dep ensure -v -add "git@xx.com:${user_name}/n${repo_name}"`

> 好像不读配置，所以没法指定source为git协议

`dep ensure -v -update "git@xx.com:${user_name}/n${repo_name}"`

> add以后.lock里才有，才能update





```sh
dep --help                                                                                                 feature_hsf_over_http
Dep is a tool for managing dependencies for Go projects

Usage: "dep [command]"

Commands:

  init     Set up a new Go project, or migrate an existing one
  status   Report the status of the project's dependencies
  ensure   Ensure a dependency is safely vendored in the project
  version  Show the dep version information
  check    Check if imports, Gopkg.toml, and Gopkg.lock are in sync

Examples:
  dep init                               set up a new project
  dep ensure                             install the project's dependencies
  dep ensure -update                     update the locked versions of all dependencies
  dep ensure -add github.com/pkg/errors  add a dependency to the project

Use "dep help [command]" for more information about a command.
```



### 私有仓库



类似

`Gopkg.toml`里配置：

```Toml
[[constraint]]
  branch = "master"
  name = "gitlab.com/eddycjy/test"
  source = "git@gitlab.com:EDDYCJY/test.git"
```

or

```toml
[[constraint]]
  branch = "master"
  name = "gitlab.com/eddycjy/test"
  source = "https://{username}:{password}@gitlab.com"
```







```sh
Solving failure:
	(1) failed to list versions for http://${}/${wrong username}/${}.git: fatal: could not read Username for 'http://${}': terminal prompts disabled
: exit status 128

ls ../../../pkg/dep/sources/ssh---git@${}-${}-${}.git
有代码了。 revision也是需要的那个。
为什么还去拉versions呢？ 

这个${wrong username}是之前错误配置的。。。
估计哪里cache了。。。

因为实际的path是 ${wrong username} 所以被dep尝试纠错了 = =
```











