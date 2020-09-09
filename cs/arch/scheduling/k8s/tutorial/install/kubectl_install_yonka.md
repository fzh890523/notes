



## 安装



安装kubectl: https://kubernetes.io/docs/tasks/tools/install-kubectl/

* 下载安装

  ```sh
  curl -LO https://storage.googleapis.com/kubernetes-release/release/v1.18.0/bin/windows/amd64/kubectl.exe
  # put to bin dir and ensure this dir in in env PATH
  ```

* 通过包管理系统安装

  * PSGallery
  * choco
  * scoop



## 配置



### 配置auto-completion

详见上面文档



bash需要安装插件 `apt-get install bash-completion`



```sh
# ~/.bashrc
echo 'source <(kubectl completion bash)' >>~/.bashrc
# or
kubectl completion bash >/etc/bash_completion.d/kubectl
```





zsh

```sh
# ~/.zshrc
source <(kubectl completion zsh)
```



```sh
echo 'alias k=kubectl' >>~/.zshrc
echo 'complete -F __start_kubectl k' >>~/.zshrc
```

