

# config



## file changes



### ignore file mode changes

https://stackoverflow.com/questions/1580596/how-do-i-make-git-ignore-file-mode-chmod-changes



```
git config core.fileMode false
```

From [git-config(1)](https://www.kernel.org/pub/software/scm/git/docs/git-config.html):

> ```
> core.fileMode
>     Tells Git if the executable bit of files in the working tree
>     is to be honored.
> 
>     Some filesystems lose the executable bit when a file that is
>     marked as executable is checked out, or checks out a
>     non-executable file with executable bit on. git-clone(1)
>     or git-init(1) probe the filesystem to see if it handles the 
>     executable bit correctly and this variable is automatically
>     set as necessary.
> 
>     A repository, however, may be on a filesystem that handles
>     the filemode correctly, and this variable is set to true when
>     created, but later may be made accessible from another
>     environment that loses the filemode (e.g. exporting ext4
>     via CIFS mount, visiting a Cygwin created repository with Git
>     for Windows or Eclipse). In such a case it may be necessary
>     to set this variable to false. See git-update-index(1).
> 
>     The default is true (when core.filemode is not specified
>     in the config file).
> ```

The `-c` flag can be used to set this option for one-off commands:

```
git -c core.fileMode=false diff
```

And the `--global` flag will make it be the default behavior for the logged in user.

```
git config --global core.fileMode false
```

Changes of the global setting won't be applied to existing repositories. Additionally, `git clone` and `git init` explicitly set `core.fileMode` to `true` in the repo config as discussed in [Git global core.fileMode false overridden locally on clone](https://stackoverflow.com/questions/30392318/git-global-core-filemode-false-overridden-locally-on-clone)

**Warning**

`core.fileMode` is not the best practice and should be used carefully. This setting only covers the executable bit of mode and never the read/write bits. In many cases you think you need this setting because you did something like `chmod -R 777`, making all your files executable. But in most projects **most files don't need and should not be executable for security reasons**.

The proper way to solve this kind of situation is to handle folder and file permission separately, with something like:

```sh
find . -type d -exec chmod a+rwx {} \; # Make folders traversable and read/write
find . -type f -exec chmod a+rw {} \;  # Make files read/write
```

If you do that, you'll never need to use `core.fileMode`, except in very rare environment.





# branch



## 创建分支



### 从指定commit创建分支



* `git branch branchname <sha1-of-commit>`
* `git branch branchname HEAD~3`
* `git checkout -b branchname <sha1-of-commit or HEAD~3>`



## track branch

https://git-scm.com/book/en/v2/Git-Branching-Remote-Branches



* `git pull`时，根据track关系获得remote信息，进行fetch。 然后同样根据track关系来决定把那个branch merge到当前branch



查看：

* `git branch -v`
* `git branch -vv`



**建立track关系**

* clone时（实际checkout）
* checkout一个remote branch时会 create-and-track
  * `git checkout serverfix`
  * `git checkout --track origin/serverfix`
  * `git checkout -b sf origin/serverfix`

* 已有分支设置为track另一个
  * `git branch --set-upstream-to other-origin/serverfix` (`-u`同样)
  * `git branch -u other-origin/foo bar` （指定本地分支bar来track foo）

* push时同时建立track（一般多见于checkout一个新的分支然后push）
  * `git push -u origin dev`



## view branch

### 详细信息如upstream： `-vv`

```sh
$ git branch -vv
* master                        b249caad03 [origin/master: behind 35] Connect external control plane to remote cluster after install (#34854)
  release-1.3.8-patch           3c137f4381 [origin/release-1.3.8-patch] Update proxy SHA to HEAD of release-1.3
  release-1.5                   25bea56c0e [origin/release-1.5] Update Proxy SHA (#26298)

```



### 查看包含某commit的branch

* `git branch -a --contains <SHA1sum-of-commit>`

> 注意有点可能不在branch上，这时要`git log -all`来搜了



## 删除分支



* git branch -d ${branchName}

  删除本地

* git push origin --delete ${branchName}

  删除远端



## checkout



### checkout到指定的remote分支



* 如果是默认的remote: `git checkout ${remoteBranchName}`
* 如果不是默认的remote： `git checkout -b ${localBranchName} ${remoteName}/${remoteBranchName}`



## 丢弃local changes/commits与remote保持一致



`git fetch github && git reset --hard github/master`



# history



## file commit history



### gitk ${filepath}方式



# remote

ref: https://stackoverflow.com/questions/42830557/git-remote-add-origin-vs-remote-set-url-origin



```sh
git remote add origin git@github.com:User/UserRepo.git
```

is used to a add a new remote

> git remote remove origin后可以通过这个加回来



```sh
git remote set-url origin git@github.com:User/UserRepo.git
```

is used to change the url of an existing remote repository

```sh
git push -u origin master
```

will push your code to the master branch of the remote repository defined with `origin` and `-u` let you point your current local branch to the remote master branch





## url



### 默认https而希望用ssh： insteadof

```properties
# Enforce SSH
[url "ssh://git@github.com/"]
  insteadOf = https://github.com/
[url "ssh://git@gitlab.com/"]
  insteadOf = https://gitlab.com/
[url "ssh://git@bitbucket.org/"]
  insteadOf = https://bitbucket.org/
```

> `url "git@github.com:"` ?
>
> **注意**： 在go mod/get场景会有额外的考虑因素，详见go mod



cmd方式： `git config --global url.ssh://git@github.com/.insteadOf https://github.com/`





# diff

ref： https://cloud.tencent.com/developer/ask/26560





* `git diff --shortstat`

  显示文件、行数变化。

  ```
   1 file changed, 23 deletions(-)
  ```

* `git diff --stat`

  更可读一些

  ```
   codecov.skip | 23 -----------------------
   1 file changed, 23 deletions(-)
  ```

  `git diff --stat <commit-ish> <commit-ish>`

* `git diff --numstat`

  表格式输出

  ```
  4       4       Gopkg.toml
  0       23      codecov.skip
  ```



**git log与diff**

git log内部调用diff机器以打印请求的信息，所以可以给它任何差异统计选项 - 不只是--shortstat。你可能想要使用的是：

```js
git log --author="Your name" --stat <commit1>..<commit2>
```

但你可以使用--numstat或--shortstat为好。git  log也可以用各种其他方式选择提交 -  查看文档。你可能会感兴趣的东西--since（而不是指定提交范围，只是从上周选择提交）和--no-merges（合并提交实际上不引入更改），以及漂亮的输出选项（--pretty=oneline,  short, medium, full...）。

```sh
git log --numstat --pretty="%H" --author="Your Name" commit1..commit2 | awk 'NF==3 {plus+=$1; minus+=$2} END {printf("+%d, -%d\n", plus, minus)}'
```



<<<<<<< HEAD
=======
### 只显示变更的文件列表



```sh
git diff --name-only <commit1> <commit2>
```





>>>>>>> 1122cdc (re-init 20240121)
### diff指定文件内容

> git diff master~20:pom.xml master:pom.xml
>
> git diff master~20:pom.xml pom.xml

但感觉这些似乎不太好用： `git diff <branch> -- ./pkg`





### 两个commit/tag/branch之间的差异

> 非内容



* `git log --oneline --graph v3.3.6...v3.3.7`

  > Assuming that you just want to see what commits are in your feature branch that don't exist in master (i.e. which commits in feature are "ahead" of master), then you can do

  output:

  ```sh
  *   52f3b79d3a (tag: v3.3.7) Merge branch 'xxx' into 'yyy'
  |\
  | * 742b13bfde (origin/3.3.xxx) xxx
  | * cf1c218ad8 fix xxx
  |/
  * 5e0cf6b936 Fix: xxxx
  * 63d6eae300 Fix: xxx
  ```

  

* `git log --oneline --graph --decorate --left-right v3.3.5...v3.3.6`

  > If you want to see how both master and feature have diverged, you can use this command:

  output：

  ```
  >   52f3b79d3a (tag: v3.3.7) Merge branch 'xxx' into 'yyy'
  |\
  | > 742b13bfde (origin/3.3.xxx) xxx
  | > cf1c218ad8 fix xxx
  |/
  > 5e0cf6b936 Fix: xxx
  > 63d6eae300 Fix: xxx
  ```

  

### 不进入“interactive mode”

`git --no-pager diff`

> 注意参数不能在diff后，似乎意为这是git的参数而不是子命令diff的参数



# commit



## date



* `git commit --date="10 day ago" -m "Your commit message" `

* ```shell
  git commit --date="`stat -c %y myfile`" myfile
  ```

* `git commit --date="$(date -v-1d)"`



## msg



### 修改之前的某commit

1. `rebase -i <commit>^`，注意最后这个`^`表示前一个，这里似乎语义是左开

2. 要修改的那个，`pick <commit>`改为`edit <commit>` 然后save、quit，进入修改状态

   修改； add； `commit --amend`； `rebase --continue`

   > 会有提示

> 会影响那个commit以及之后所有commit号，所以push要带`--force`，酌情使用



### 修改commit msg

* 最近的： `git commit --amend -m "new msg"`

* 之前的： 

  * rebase方式

    * `git rebase -i HEAD^^^` / `git rebase -i <commit_id>` 指向要修改的commit前面的那个

      > or `git rebase -i HEAD~5` 方式

      输出一般是如下格式：
    
      ```s
      pick 43f8707f9 fix: update dependency json5 to ^2.1.1
      pick cea1fb88a fix: update dependency verdaccio to ^4.3.3
      pick aa540c364 fix: update dependency webpack-dev-server to ^3.8.2
      pick c5e078656 chore: update dependency flow-bin to ^0.109.0
      pick 11ce0ab34 fix: Fix spelling.
      Rebase 7e59e8ead..11ce0ab34 onto 7e59e8ead (5 commands)
      ```
      
      > 按时间顺序从前到后，和 git log相反
      >
      > 所以本case要修改的是 11ce0ab34 这个最后commit
      
        * 把要修改的这个commit，`pick <commit> <msg>` 改为 `reword <commit> <new_msg>`然后保存退出
      
        * 会继续弹窗做个commit确认，保存退出即可
      
        * done...
      



## 变更



### 插入commit

> 常用于修复历史bug，比如重构前的bug，保证重构前的版本功能ok
>
> 该修改似乎会导致后面的commit号都变化，慎用



https://stackoverflow.com/questions/32315156/how-to-inject-a-commit-between-some-two-arbitrary-commits-in-the-past

It's even easier than in OP's answer.

1. `git rebase -i <any earlier commit>`. This displays a list of commits in your configured text editor.
2. Find the commit you want to insert after (let's assume it's `a1b2c3d`). In your editor, for that line, change `pick` to `edit`.
3. Begin the rebase by closing your text editor (save your changes). This leaves you at a command prompt with the commit you chose earlier (`a1b2c3d`) *as if it has just been committed*.
4. Make your changes and `git commit` (**NOT** amending, unlike most `edit`s). This creates new a commit *after* the one you chose.
5. `git rebase --continue`. This replays the successive commits, leaving your new commit inserted in the correct place.

Beware that this will rewrite history, and break anyone else who tries to pull.





<<<<<<< HEAD
=======
## file

### 拷贝指定commit的文件



```sh
git show <commit>:<file_path> > <new_file_path>
```



>>>>>>> 1122cdc (re-init 20240121)



# rebase



默认merge是按提交时间。。。，rebase则等于人为的把部分commit（不按时间）排在另一部分之后。



`rebase -i <commit>`，可以调整`<commit>`之后的commit，也即`左开`，需要要修改第一个commit的话，`rebase -i --root`

> 一开始设计为`左闭`不就ok了？



<<<<<<< HEAD
### rebase + squash



1. 找到要squash的最早一个commit，如commit1

2. `git rebase -i commit1^`

3. 内容改为

   ```sh
   reword commit1
   squash commit2
   ...
   ```

4. 按照提示把commit1 msg改为squash后期望的内容
5. squash默认会把commit2的msg追加在第一个后面，如果有需要删除、修改，记得操作
6. ...





=======
>>>>>>> 1122cdc (re-init 20240121)
# merge



## cherry-pick



```shell
# develop上有个hotfix commit（commit号为abcd...），但develop本身还来不及merge到master(比如commit太多)

git checkout master
git cherry-pick -x abcd  # -x的话生成标准commit message，减少重复merge
```



## resolve conflicts



### 选用某一方的结果

```sh
git checkout --ours -- <paths>
# or
git checkout --theirs -- <paths>
```

**注意： merge 和 rebase时，ours和theirs的意义是相反的**

* merge
  `ours`指当前分支
  
  ```sh
  git branch  # branchA
  git merge -X ours branchB  # prefer files from branchA
  ```
  
  
  
* rebase
  `theirs`指当前分支
  
  ```sh
  git branch  # branchA
  git rebase -X theirs branchB  # prefer files from branchA
  ```



<<<<<<< HEAD
=======


## merge tool



* sublime merge





>>>>>>> 1122cdc (re-init 20240121)
# clone

* 指定branch： `-b`

  `git clone -b branchA --single-branch <url>`






# pull



# push



## push到remote不同分支(尤其是不存在时)



一种方式：

* local new -> remote new

  ```sh
  git checkout new
  git push origin new
  ```

* local old -> remote new 临时

  ```sh
  git push origin old:new
  # 注意: git push origin :new 会导致remote new被删除
  ```

* local old -> remote new 持久

  ```sh
  git push --set-upstream new old
  # 或者先set再push吧
  
  # 似乎这样也行
  git push -u origin my_branch:remote_branch
  ```



# fetch

https://git-scm.com/docs/git-fetch



**目标**

* 可指定server/remote

* 默认fetch origin
* `-all` fetch all remotes
* `--multiple` 来指定多个



**内容**

* 默认会把下载内容的关联tags（指向内容）一起下载，`-n`/`--no-tags` 则不下载tags

*  `-t`/`--tags` 下载所有tags

  > 和不带的区别是？
  >
  > 估计是因为只拉取“感兴趣”分支的内容，所以其他分支内容不下载的同时也意味着其他分支上的tags不会下载。 而此参数会导致“不感兴趣”的分支上的tag也下载（？）

  



# tag

见 [git_tag_yonka.md](git_tag_yonka.md)



# log

见 [git_log_yonka.md](git_log_yonka.md)



# describe

ref: https://blog.csdn.net/ustccw/article/details/79068354s



获取当前仓库当前的 commit 对应的最近的 tag`



**参数选项**

* `--dirty` 意味着如果源码如果被修改了(git status)，则会在版本后面加上 -dirty (默认)，如版本为 v2.1 ， 如果你修改了源码，则git describe 结果会成为 v2.1-dirty ,你也可以通过 --dirty= 来赋值新的字符串。
* `--tags` Instead of using only the annotated tags, use any tag found in refs/tags namespace. This option enables matching a lightweight (non-annotated) tag.
* `--always` Show uniquely abbreviated commit object as fallback.



**示例**

```sh
git describe --always --tags --dirty
v3.1-dev-374-gb0f7ff5  # output
# v3.1-dev: 当前分支最近的 tag
# 374 代表在 v3.1-dev tag 后有 374 次提交(commit)
# -g 代表 git
# b0f7ff5 代表最近的 commitID
# git describe --always 会获取最近的 commitID
```







# merge



## cherry-pick



```shell
# develop上有个hotfix commit（commit号为abcd...），但develop本身还来不及merge到master(比如commit太多)

git checkout master
git cherry-pick -x abcd  # -x的话生成标准commit message，减少重复merge
```





# stash



* `git stash`
* `git stash pop`
* `git stash list`
* `git stash show`/`git stash show -P`
* `git stash clear`



# submodule

> * 好像没管理好 .git/modules 和 .gitmodules 的同步。 手动添加文件和内容然后 git submodule update 或者 init都不行
> * set-branch 



* `git submodule update --recursive --init`

* `git submodule add <url> <path>` 

  `git submodule add git@x.x.x:yyy/zzz.git dep/zzz`

* `git submodule set-branch --branch <branch> -- <path>`

  会修改.gitmodules，但似乎没更新？ 手动update好像也不行

<<<<<<< HEAD
=======
## 初始拉取

> 
> 使用 Git Submodule 进行初始化拉取的步骤如下：
>
> 1. 在主存储库中执行 `git submodule add <submodule_repository_url> <submodule_directory>` 命令来添加子模块。
> 2. 执行 `git submodule init` 命令来初始化本地配置文件并拉取子模块的代码。
> 3. 执行 `git submodule update` 命令来检出子模块代码的正确版本。
>
> 这些步骤将会初始化子模块并将其添加为主存储库的一部分。

>>>>>>> 1122cdc (re-init 20240121)


```sh
# 需要在git repo里
git submodule add git@github.com:istio/pkg.git istio-pkg
cd istio-pkg
git checkout d97bc429be20
# changes
git checkout -b release-1.7.4
git commit -m "xxx"

git submodule set-url git@github.com:YonkaFang/pkg.git istio-pkg/
cd istio-pkg
git remote set-url origin git@github.com:YonkaFang/pkg.git
git push origin release-1.7.4

cd ..
git submodule status
+f1c7fffabc49e30eacad234d12387593e33da01c istio-pkg (1.7.4-1-gf1c7fff)
```



## 嵌套的情况

`proj1/proj2/proj3` 这样的嵌套submodule。

```sh
# proj1当前版本里对submodule proj2的版本描述为 v2.1，其中对proj3的版本描述为 v3.1
cd proj2
git checkout v2.2  # 其中对proj3的版本描述为v3.2
git submodule update  # 这时还是会认为是...
# 正确的做法
cd ../
git add -A
git commit -m "change dep proj2 to v2.2"
cd proj2
git submodule update
```





## remove submodule

```sh
# Remove the submodule entry from .git/config
git submodule deinit -f path/to/submodule

# Remove the submodule directory from the superproject's .git/modules directory
rm -rf .git/modules/path/to/submodule

# Remove the entry in .gitmodules and remove the submodule directory located at path/to/submodule
git rm -f path/to/submodule

# ref: https://stackoverflow.com/questions/1260748/how-do-i-remove-a-submodule/36593218#36593218
```



<<<<<<< HEAD
=======
## 查看



> 要查看 Git Submodule 的信息，可以使用以下命令：
>
> 1. `git submodule status`：显示当前存储库中所有子模块的状态和提交 ID。
> 2. `git submodule foreach <subcommand>`：对每个子模块执行指定的 Git 命令。
> 3. `cat .gitmodules`：显示存储库中所有子模块的 URL 和路径信息。
> 4. `git config --list`：列出所有 Git 全局配置和本地存储库配置，包括子模块信息。
>
> 使用这些命令可以查询和管理子模块的信息。



## 修改



### 修改仓库



> 要将 Git Submodule 指向不同的存储库，可以按照以下步骤进行操作：
>
> 1. 在主存储库中执行 `git submodule sync` 命令以确保子模块 URL 已同步。
> 2. 执行 `git submodule deinit <submodule_directory>` 命令以取消初始化子模块。
> 3. 在主存储库中执行 `git submodule set-url <submodule_directory> <new_submodule_repository_url>` 命令以将子模块指向新的存储库。
> 4. 执行 `git submodule update --init` 命令来重新初始化并拉取子模块代码。
>
> 这些步骤将会修改子模块指向的存储库，使其指向新的 URL。注意，这些更改将不会自动提交到版本控制。需要将这些更改提交到主存储库中以便其他人可以访问它们。



>>>>>>> 1122cdc (re-init 20240121)
# reset



* `--hard` index（head自然也...）、working都退回到指定commit。 **等于这个commit的内容完全消失**

* `--soft` 只改head，index、working都不变。 这时候 退回stage后**待commit状态**

* `--mixed` **默认**。 改head和index，也即 退回unstage状态，**等于这个commit只剩下working内容的变化**

  需要重新add和commit



## "reset" init commit



`git update-ref -d HEAD`

> You just need to delete the branch you are on. You can't use `git branch -D` as this has a safety check against doing this. You can use update-ref to do this.
>
> `git update-ref -d HEAD`
> Do not use `rm -rf .git` or anything like this as this will completely wipe your entire repository including all other branches as well as the branch that you are trying to reset.



# revert





# "unstage"



git add的内容想回退到unstage状态的话：

* `git reset -- <filePath>`
* `git restore --staged <filePath>` （2.24版本开始）

> `git rm --cache <filePath>` 效果与预期不符，它只是 **stage一个删除操作**

output： 

> ```sh
> touch 1
> 
> git add 1
> git rm --cache 1
> rm '1'  # output
> # working tree有untracked 1
> 
> git add 1
> git reset -- 1
> # working tree有untracked 1
> 
> echo 123 > commited_file
> git add commited_file
> git rm --cache commited_file
> # working tree有untracked 1; staged里有： delete commited_file
> # 看起来像是用一个 delete commited_file 覆盖了stage里的 modified:   commited_file
> git add commited_file
> # 恢复： stage里 modified:   commited_file
> 
> git reset --staged commited_file
> # working tree里有 modified:   commited_file
> ```
>
> 



# "status"



* **能覆盖所有变更内容的，目前只知道** `[ -z $(git status -s --porcelain) ]`

* 覆盖除untracked files以外的：

  *  `git diff-files --quiet && git diff-index --quiet --cached HEAD --`

    一个看working tree、一个看index/staged

  * `git diff-index --quie HEAD --`  似乎和上面等效



<<<<<<< HEAD
=======
## 不能正确显示中文文件



> 如果您使用Git时，`git status`命令无法正确显示中文文件名，可能是由于终端或操作系统的字符编码设置问题导致的。以下是一些可能的解决方法：
>
> 1. 检查终端字符编码：确保您使用的终端支持并正确设置了UTF-8字符编码。您可以在终端的设置中查找字符编码选项，并将其设置为UTF-8。
>
> 2. 检查Git配置：使用`git config --get core.quotepath`命令检查您的Git配置。如果返回的结果是`false`，则Git应该可以正确显示中文文件名。如果返回的结果是`true`，可以使用`git config --global core.quotepath false`命令来禁用引号路径。
>
>    > y: 这个是主要
>
> 3. 更新Git版本：确保您使用的是最新版本的Git。有时，较旧的Git版本可能存在与中文文件名显示相关的问题。您可以通过访问Git的官方网站来获取最新版本。
>
> 4. 使用Unicode转义：如果您仍然遇到问题，您可以尝试使用Unicode转义序列来引用文件名。例如，对于文件名为中文的文件，可以使用`git status "\u4E2D\u6587"`来查看其状态。

>>>>>>> 1122cdc (re-init 20240121)










