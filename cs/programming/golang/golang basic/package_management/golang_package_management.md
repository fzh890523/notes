

# resolve dependencies



## resolve project's all-dependencies



```shell
cd ${project_root}/src
go get ./...
```





# 包管理工具



* ...


* godep
* dep
* vgo
* ...





# dep



`go get -u -v github.com/golang/dep `



* `dep init`

  `export GOPATH=… && dep init .`

* `dep status`

  ```sh
  ## mac
  brew install graphviz
  dep status -dot | dot -T png | open -f -a /Applications/Preview.app
  
  ## linux
  sudo apt-get install graphviz
  dep status -dot | dot -T png | display
  
  ## windows
  choco install graphviz.portable
  dep status -dot | dot -T png -o status.png; start status.png
  
  # https://golang.github.io/dep/docs/daily-dep.html#visualizing-dependencies
  ```

* `dep ensure`

  ```sh
  dep ensure -update # is the preferred way to update dependencies, though it's less effective for projects that don't publish semver releases.
  dep ensure -add # is usually the easiest way to introduce new dependencies, though you can also just add new import statements then run dep ensure.
  # If you ever make a manual change in Gopkg.toml, it's best to run dep ensure to make sure everything's in sync.
  # dep ensure is almost never the wrong thing to run; if you're not sure what's going on, running it will bring you back to safety ("the nearest lilypad"), or fail informatively.
  
  # ref: https://golang.github.io/dep/docs/daily-dep.html#visualizing-dependencies
  ```

* `dep check`

  will quickly report any ways in which your project is out of [sync](https://golang.github.io/dep/docs/glossary.html#sync).