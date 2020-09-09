

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



## 删除分支



* git branch -d ${branchName}

  删除本地

* git push origin --delete ${branchName}

  删除远端



## checkout



### checkout到指定的remote分支



* 如果是默认的remote: `git checkout ${remoteBranchName}`
* 如果不是默认的remote： `git checkout -b ${localBranchName} ${remoteName}/${remoteBranchName}`



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







# commit



## date



* `git commit --date="10 day ago" -m "Your commit message" `

* ```shell
  git commit --date="`stat -c %y myfile`" myfile
  ```

* `git commit --date="$(date -v-1d)"`






# rebase



默认merge是按提交时间。。。，rebase则等于人为的把部分commit（不按时间）排在另一部分之后。







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
  ```



# fetch





# tag



## list



* `git tag`

* `git tag -l`

  `git tag -l "0.1.*"`

* `git tag -a v1.0.0 -m "for test"`

*  `git push origin v1.0.0`/`git push origin --tags`

## show(detail)



* `git show <name>`



## verify



* `git -v <name>`



## add



* annotation

  * normal `git tag -a <name> -m <comment>`

    打在某个commit上`git -a <name> <commit>`

    > commit号允许是前几位

  * signed `git tag -s <name> -m <comment>`

* lightweight

  * `git tag <name>`



## push



* 推送某个 `git push <remote> <tagname>`
* 推送全部`git push <remote> --tags`




## delete



local

* `git tag -d <tagname>`

  `git tag -d 0.1.0.2`




remote

* `git push origin :tagname` 

  > push an 'empty' reference to the remote tag name

* `git push --delete origin tagname`

  > or `-d` for old git versions





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



























