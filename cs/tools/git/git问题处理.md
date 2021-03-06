## git checkout



### 如何只切换文件不切换HEAD

ref： https://clubmate.fi/git-checkout-file-or-directories-from-another-branch/



* 指定文件： `git checkout feature-branch -- src/js/some-file.js`

  然后，该文件就被替换成`feature-branch`分支的了

* 指定目录： `git checkout feature-branch -- src/js/`

* 全部： `git checkout feature-branch -- ./`





### 把branch name认为是file name问题

```
git co feature/user_controlled_site_layouts 
error: pathspec 'feature/user_controlled_site_layouts' did not match any file(s) known to git.
```



或者 



```
git co 2.12.4
# 同样的报错
```



#### 处理

```shell
git remote update
git fetch 
git checkout --track origin/<BRANCH-NAME>
# origin可能也需要换成正确的remote
```



## git “撤销”



### git ”unstage“

常见的场景是**误把不想commit的文件add了**，处理办法是： `git reset HEAD ${file}`。



之后如果想撤销掉该文件的修改： `git checkout -- ${file}`



## git diff



### git difftool



```shell
# 一次性比较整个repo目录的差异(vs 某个commit)
git difftool --dir-diff  f0ab990c
```



不过默认情况下对于 `own` 是创建symlink到实际文件，这样导致beyondcompare比较具体文件时会显示内容为空，处理办法：

* git加上参数`--no-symlinks`
* bc4开始在比较页面的  `rules-handling（处理）-follow symblic links（跟随符号链接）` 选中即可



### git diff tool 配置工具



```shell
git config --global --list | grep tool

difftool.sourcetree.cmd=opendiff "$LOCAL" "$REMOTE"
difftool.sourcetree.path=
mergetool.sourcetree.cmd=/Applications/SourceTree.app/Contents/Resources/opendiff-w.sh "$LOCAL" "$REMOTE" -ancestor "$BASE" -merge "$MERGED"
mergetool.sourcetree.trustexitcode=true
```





配置方式

```shell
git config --global diff.tool opendiff  # 配置为opendiff
git config --global difftool.prompt false
```





```
[core]
    excludesfile = /Users/zhihengfang/.gitignore_global
    quotepath = false
    editor = /usr/bin/vim
[difftool "sourcetree"]
    cmd = opendiff \"$LOCAL\" \"$REMOTE\"
    path =
[difftool "bcomp"]
    trustExitCode = true
    cmd = "/usr/local/bin/bcomp" \"$LOCAL\" \"$REMOTE\"
[mergetool "sourcetree"]
    cmd = /Applications/SourceTree.app/Contents/Resources/opendiff-w.sh \"$LOCAL\" \"$REMOTE\" -ancestor \"$BASE\" -merge \"$MERGED\"
    trustExitCode = true
[mergetool "bcomp"]
    trustExitCode = true
    cmd = "/usr/local/bin/bcomp" \"$LOCAL\" \"$REMOTE\" \"$BASE\" \"$MERGED\"
[diff]
    tool = bc3
[difftool]
    prompt = false
```





```
Git 1.8 and newer
(Note:  Use bc3 on the command line for both BC version 3 and 4)

Diff
Confirm that "bcompare" can be launched from Terminal.  Then in a console window enter:
  $ git config --global diff.tool bc3
  $ git config --global difftool.bc3.trustExitCode true
To launch a diff using Beyond Compare, use the command "git difftool file.ext".

Merge Pro only
Confirm that "bcompare" can be launched from Terminal.  Then enter the following:
  git config --global merge.tool bc3
  git config --global mergetool.bc3.trustExitCode true
To launch a 3-way merge using Beyond Compare, use the command "git mergetool file.ext".


Git 1.7.x and older
(Instructions vary depending on Git version)

Diff
Create a shell script file "git-diff-wrapper.sh" with the following content:
#!/bin/sh
# diff is called by git with 7 parameters:
# path old-file old-hex old-mode new-file new-hex new-mode
"<path_to_bc3_executable>" "$2" "$5" | cat
In a console window enter the command:
$ git config --global diff.external <path_to_wrapper_script>
3-way Merge Pro only
In a console window enter the following three commands:
  $ git config --global merge.tool bc3
  $ git config --global mergetool.bc3.cmd "/usr/bin/bcompare \$LOCAL
    \$REMOTE \$BASE \$MERGED"
  $ git config --global mergetool.bc3.trustExitCode true
2-way Merge
In a console window enter the following three commands:
  $ git config --global merge.tool bc3
  $ git config --global mergetool.bc3.cmd "/usr/bin/bcompare \$LOCAL
    \$REMOTE -savetarget=\$MERGED"
  $ git config --global mergetool.bc3.trustExitCode true 
```

https://www.scootersoftware.com/support.php?zz=kb_vcs





然后可以： `git difftool origin/branch_xx`



#### opendiff

mac上新版本xcode默认自带。It is a command line tool which opens the GUI of [FileMerge](https://www.google.com/search?q=filemerge).



#### beyondcompare

… 

配置类似：

```
[difftool "bcomp"]
    trustExitCode = true
    cmd = "/usr/local/bin/bcomp" \"$LOCAL\" \"$REMOTE\"
[diff]
    tool = bcomp  # or bc or bc3
[difftool]
    prompt = false
```



使用：

```shell
git difftool origin/branch_xx
# 或者 
git difftool -t bcomp origin/branch_xx
```





可能需要在beyondcompare应用里 menu-安装命令行工具 来安装。

然后bcomp和bcompare的区别在于： 前者会等当前比较回话（tab）退出（关闭），在git difftool命令里变现为逐个…； 而bcompare则是（添加session后）立即返回，那么就会一下把所有diff session全部添加。



**然后**，beyond有个bug，当window已经打开时，命令行的 `becomp <srouce> <target>` 或  `becompare <srouce> <target>` 都会实际报错，然后打开一个错误的页面，然后就无法继续了。

表现为：

* 没打开window时：

  * becomp能比较第一个，然后第二个报错，第三个没有了
  * becompare添加一个正常比较和`n-1`个报错的比较

* 打开window时：

  和上面类似，只是第一个正常比较也没了



处理办法：

```shell
# /usr/local/bin/bc-wrapper.sh

#!/bin/bash
bc_pid=`pgrep BCompare`
[ ! -z "${bc_pid}" ] && kill ${bc_pid}
"$1" "$2" "$3"
```

然后：

```
[difftool "bcomp"]
    trustExitCode = false
    cmd = "/usr/local/bin/bc-wrapper.sh" "/usr/local/bin/bcomp" \"$LOCAL\" \"$REMOTE\"
[diff]
        tool = bcomp
[difftool]
        prompt = false
```

显然，这个只能用于bcomp而不能用于bcompare。





要同时支持mergetool的话，脚本要改为：

```shell
#!/bin/bash
bc_pid=`pgrep BCompare`
[ ! -z "${bc_pid}" ] && kill ${bc_pid}

if [ $# -eq 3 ]; then
    "$1" "$2" "$3"
elif [ $# -eq 5 ]; then
    "$1" "$2" "$3" "$4" "$5"
fi
```



```
[mergetool "bcomp"]
    trustExitCode = true
    cmd = "/usr/local/bin/bc-wrapper.sh" "/usr/local/bin/bcomp" \"$LOCAL\" \"$REMOTE\" \"$BASE\" \"$MERGED\"
    
[merge]
    tool = bcomp
```





## git merge



常见流程：

1. `git pull <remote> <branch>`

   用`git fetch <remote> <branch>` and `git merge --no-commit <remote> <branch>` 也许更好

2. `git mergetool`

3. `git difftool <remote> <branch>` / `git difftool HEAD`

   查看是否“自动merge”出错。



### 自动merge带来的问题

* 经常被忽略的问题
* 自动merge != 合乎预期，也即 != 正确，也即 没有冲突 != OK



所以，对要merge内容没什么信息的话，有时可能更倾向于： **不要自动merge而是每个都手动merge**。

但是： **似乎是没有标准做法**

搜到的：

```
What you are trying to do is bypass Git from getting involved in the merge process and to hand-pick each line of each modified file to be merged. This not the same as git cherry-pick. Neither will git merge --no-commit, etc. help. Here is what you will need to do:

$ git checkout master
$ git difftool -t kdiff3 local-branch HEAD
In the KDiff3 window, the left hand side (A) is your local-branch and the right hand side (B) is your current branch (master).
Select Merge | Merge Current File from the menu (or press the colorful diamond shaped icon with the same title).
You will then be shown a diff and conflicts (if any) for each file. And you will have the ability to pick left or right side (A or B), or both, and/or manually tweak the merged file.

On another note, something is telling me you have some bigger issues with your workflow.
```



### rebase冲突手动选择手动选择(而不是compare)

主要用于“显然以某方为准”的场景，尤其是binary文件不好diff-再选择。

```sh
git checkout --ours -- <paths>
# or
git checkout --theirs -- <paths>
```

还有就是rebase时直接指定：
```sh
# rebase preferring current branch changes during conflicts
$ git rebase -X theirs branch-b
```

对于merge,**ours, theirs的含义与rebase相反**，要`prefer current branch时`：
```sh
# assuming branch-a is our current version
$ git merge -X ours branch-b  # <- ours: branch-a, theirs: branch-b
```


### git merge tool



```shell
git config --global merge.tool opendiff
```



#### beyondcompare作为merge tool



背景：

1. `git pull <remote> <branch>`

2. `git mergetool`

   事先配置为beyond compare（用wrapper绕开前面说的bug）。

   实际执行： `"/usr/local/bin/bc-wrapper.sh" "/usr/local/bin/bcomp" \"$LOCAL\" \"$REMOTE\" \"$BASE\" \"$MERGED\"` （语法： `bcomp [options] [<Left> [<Right> [<Center> [<Output>]]]]`）

> 怎么理解： LOCAL、BASE、REMOTE、MERGED呢？
>
> * `LOCAL` 目前理解为 `working directory`
> * `BASE` 目前理解为 `HEAD`
> * `REMOTE` 目前理解为 `<remote> <branch>`
> * `MERGED` 目前理解为 `(git auto )merged result`（可能是冲突状态）
>
>







## git fetch



只是把远程的remote branch信息拉到本地，这样本地通过`<remote> <branch>`可以访问到最新的内容。

注意，不是“拉到当前分支”。 其实理解为： `git sync-to-latest remote branch`  比较合适。



## git pull



`git pull <remote> <branch>` == `git fetch <remote> <branch> && git merge <remote> <branch>`





# 场景



## win/linux file path不兼容问题

git里记录了信息，只要文件系统支持都可以提交/还原，相应的如果不支持、还原失败的话就会变成`files deleted`，会造成：

* `files deleted`自然很烦，即使不处理也会干扰正常的变更
* 这样的文件一多，会使得git、sourcetree等很卡、非常卡



应对方式： 在原来支持的平台上把file path修复成兼容各平台的格式（参照着出现问题平台上的信息），然后提交，在问题平台上拉取覆盖。



其他问题： 批量处理的话，包含中文、特殊字符（`: ' " : ?`等）的部分会给脚本编写带来不小的麻烦。



## 公司内commit/push pub的repo



* 修改user.name

  ```sh
  git config --replace-all user.name ${user_name}
  
  # 然后
  git config --local --list 查看
  ```

* 修改user.email

  ```sh
  git config --replace-all user.email ${user_email}
  ```



## auth



### 选择ssh key



ref： <https://superuser.com/questions/232373/how-to-tell-git-which-private-key-to-use>

There is **no direct way** to tell `git` which private key to use, because it relies on `ssh` for repository authentication. However, there are still a few ways to achieve your goal:



**Option 1**: `ssh-agent`

You can use `ssh-agent` to temporarily authorize your private key.

For example:

```
$ ssh-agent sh -c 'ssh-add ~/.ssh/id_rsa; git fetch user@host'
```



**Option 2**: `GIT_SSH_COMMAND`

Pass the ssh arguments by using the `GIT_SSH_COMMAND` environment variable (Git 2.3.0+).

For example:

```
$ GIT_SSH_COMMAND='ssh -i ~/.ssh/id_rsa -o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no' \
  git clone user@host
```

You can type this all on one line — ignore `$` and leave out the `\`.



**Option 3**: `GIT_SSH`

Pass the ssh arguments by using the `GIT_SSH` environment variable to specify alternate `ssh` binary.

For example:

```
$ echo 'ssh -i ~/.ssh/id_rsa -o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no $*' > ssh
$ chmod +x ssh
$ GIT_TRACE=1 GIT_SSH='./ssh' git clone user@host
```

Note: The above lines are shell (terminal) command lines which you should paste into your terminal. They will create a file named *ssh*, make it executable, and (indirectly) execute it.

Note: [`GIT_SSH` is available since v0.99.4](https://github.com/git/git/commit/4852f7232b7a83fbd1b745520181bd67bf95911b) (2005).



**Option 4**: `~/.ssh/config`

Use the `~/.ssh/config` file as suggested in other answers in order to specify the location of your private key, e.g.

```
Host github.com
  User git
  Hostname github.com
  IdentityFile ~/.ssh/id_rsa
```

example:

```
Host testHost
  User git
  Hostname www.testHost.com
  IdentityFile ~/.ssh/id_rsa.1  
```
然后：
```sh
git remote add remote1 git@testHost:user/repo  # 这个testHost与前面Host对应
```



### https -> ssh/git (insteadOf配置)

主要是有些场景下只能用https格式，比如go get。

`go get github.com/xx/yy`，而当private repo时，鉴权会比较麻烦，明文密码也不方便，这时候用ssh/git是必要的，但url格式又不能改，于是：

> 在 gitconfig配置（如`~/.gitconfig`）中加入：

```
[url "git@github.com:"]
    insteadOf = https://github.com/
```

其他repo同理。（gitlab好像也是git username）。

y










