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



























