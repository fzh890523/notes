

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

  



# commit



## date



* `git commit --date="10 day ago" -m "Your commit message" `

* ```shell
  git commit --date="`stat -c %y myfile`" myfile
  ```

* `git commit --date="$(date -v-1d)"`



## msg



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

## 类型

> git for-each-ref tells you what each ref is to by default, its id and its type. To restrict it to just tags, do git for-each-ref refs/tags.
>
> > [T]he output has three fields: The hash of an object, the type of the object, and the name in refs/tags that refers to the object. A so-called "lightweight" tag is a name in refs/tags that refers to a commit¹ object. An "annotated" tag is a name in refs/tags that refers to a tag object.
> >
> > - Solomon Slow (in the comments)
>
> Here is an example:
>
> ```sh
> $ git for-each-ref refs/tags                                           
> 902fa933e4a9d018574cbb7b5783a130338b47b8 commit refs/tags/v1.0-light
> 1f486472ccac3250c19235d843d196a3a7fbd78b tag    refs/tags/v1.1-annot
> fd3cf147ac6b0bb9da13ae2fb2b73122b919a036 commit refs/tags/v1.2-light
> ```
>
> To do this for just one ref, you can use git cat-file -t on the local ref, to continue the example:
>
> ```sh
> $ git cat-file -t v1.0-light
> commit
> $ git cat-file -t v1.1-annot
> tag
> ```
>
> 
>
> > ¹ tags can refer to any Git object, if you want a buddy to fetch just one file and your repo's got a git server, you can `git tag forsam :that.file` and Sam can fetch it and show it. Most of the convenience commands don't know what to do with tagged blobs or trees, but the core commands like update-index and such do



> The big difference is perfectly explained here.
>
> Basically, lightweight tags are just pointers to specific commits. No further information is saved; on the other hand, annotated tags are regular objects, which have an author and a date and can be referred because they have their own SHA key.
>
> If knowing who tagged what and when is relevant for you, then use annotated tags. If you just want to tag a specific point in your development, no matter who and when did that, then lightweight tags are good enough.
>
> Normally you'd go for annotated tags, but it is really up to the Git master of the project.







## search



### 根据commit hash查找

* `git tag --points-at HEAD`/`git tag --points-at <hash>`

* `git tag --contains <commit>`（也支持`HEAD`）

  但真的只是contains，比如用一个老的commit，可能输出多个tag



## list



* `git tag`

* `git tag -l`

  `git tag -l "0.1.*"`

* `git tag -a v1.0.0 -m "for test"`

*  `git push origin v1.0.0`/`git push origin --tags`



```sh
 git show-ref --tags
89cdb55df2d556bc63317630cce23c9768eb9734 refs/tags/0.1.0
5a25a62cf322518b196fc8fb44b7e9c104ddd07e refs/tags/0.1.1
...
```



显示日期：

```sh
 git log --tags --simplify-by-decoration  --pretty="format:%ai %d"
2021-01-15 15:35:10 -0700  (tag: 1.9.0-beta.0, origin/release-1.9)
2021-01-12 17:06:23 -0600  (tag: 1.8.2)
2020-12-07 13:31:30 -0800  (tag: 1.8.1)
...
```

> There is no date information attached to a lightweight tag. Therefore, this answer is incorrect. The dates in the output of `git log --tags --simplify-by-decoration --pretty="format:%ai %d"` are the dates of the *objects* to which the tags point, *not* the dates when the tags themselves were created.

```sh
git log --date-order --graph --tags --simplify-by-decoration --pretty=format:'%ai %h %d'

* 2021-01-15 15:35:10 -0700 5dd20445f2  (tag: 1.9.0-beta.0, origin/release-1.9)
| * 2021-01-12 17:06:23 -0600 bfa8bcbc11  (tag: 1.8.2)
| * 2020-12-07 13:31:30 -0800 806fb24bc1  (tag: 1.8.1)
| | * 2020-12-07 20:40:53 +0800 74a8d16a80  (tag: 1.7.6)
| | * 2020-12-04 15:53:29 -0800 35ffee31de  (release-1.7)
| | | * 2020-11-20 13:43:48 -0600 3ddc57b6d1  (tag: 1.6.14, origin/release-1.6)
| * | | 2020-11-18 14:44:33 -0800 c87a4c874d  (tag: 1.8.0-rc.1, tag: 1.8.0)
# graph样式
```



```sh
git tag -l --format='%(refname)   %(taggerdate)'
refs/tags/0.1.0
refs/tags/0.1.1   Wed May 10 22:49:45 2017 -0700
refs/tags/0.1.2   Sun May 14 22:51:04 2017 -0700
refs/tags/0.1.3   Mon May 15 21:18:57 2017 -0700
...

 git tag -l --sort=-creatordate --format='%(creatordate:short):  %(refname:short)'
2021-01-15:  1.9.0-beta.0
2021-01-14:  1.8.2
2020-12-10:  1.7.6
...
```





## show(detail)



* `git show <name>`



### 获取tag hash

> To get git tags with the SHA1 hash of the Tag object, you can run:
>
> ```
> git show-ref --tags
> ```
>
> The output will then look something like:
>
> ```
> 0e76920bea4381cfc676825f3143fdd5fcf8c21f refs/tags/1.0.0
> 5ce9639ead3a54bd1cc062963804e5bcfcfe1e83 refs/tags/1.1.0
> 591eceaf92f99f69ea402c4ca639605e60963ee6 refs/tags/1.2.0
> 40414f41d0fb89f7a0d2f17736a906943c05acc9 refs/tags/1.3.0
> ```
>
> Each line is the SHA1 hash of the tag, followed by the tag name prefixed with `refs/tags/`.
>
> If you want the SHA1 hash of the commit, instead of the tag object, you can run:
>
> ```
> git show-ref --tags -d
> ```
>
> This will produce output like:
>
> ```
> 0e76920bea4381cfc676825f3143fdd5fcf8c21f refs/tags/1.0.0
> 3e233dd8080617685992dc6346f739a6f6396aae refs/tags/1.0.0^{}
> 5ce9639ead3a54bd1cc062963804e5bcfcfe1e83 refs/tags/1.1.0
> 09173980152a7ed63d455829553448ece76c6fdc refs/tags/1.1.0^{}
> 591eceaf92f99f69ea402c4ca639605e60963ee6 refs/tags/1.2.0
> 56d803caaa8a93a040b7be0b8a36abdc4ce8c509 refs/tags/1.2.0^{}
> 40414f41d0fb89f7a0d2f17736a906943c05acc9 refs/tags/1.3.0
> 1bdf628a70fda7a0d840c52f3abce54b1c6b0130 refs/tags/1.3.0^{}
> ```
>
> The lines ending with `^{}` start with the SHA1 hash of the actual commit that the tag points to.



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





# log

`git log --help` for ref



## 好用的的模板

* `hs`

  ```sh
  git config --global alias.hs "log --pretty='%C(yellow)%h %C(cyan)%cd %Cblue%aN%C(auto)%d %Creset%s %C(bold normal)%S' --graph --date=short --date-order"
  # --date=short: only date in format YYYY-MM-DD
  # %cd use format from --date
  # %S like --source
  git hs
  
  # like: d45fdb06xxx 2021-09-03 author1 Add .cicd
  ```

  



## 支持直接指定多个目标

如： `git log master v1.0.0`



## 查看所有（分支）

* `--all`

  > Instead of `--all` you may want to use `--branches`, since `--all` also includes `refs/tags` and `refs/remotes`.

* `--branches`

  > 这个参数比较特殊，因为支持的是shell glob，所以如果没有配置任何通配，比如`--branches=master`，会处理为`--branches=master/*`
  >
  > ```sh
  > --branches[=<pattern>]
  >     Pretend as if all the refs in refs/heads are listed on
  >     the command line as <commit>. If <pattern> is given, 
  >     limit branches to ones matching given shell glob. If 
  >     pattern lacks ?, *, or [, /* at the end is implied.
  > ```
  >
  > 所以这个case，需要处理为 `--branches=maste[r]`。 比较丑。 其实一个分支的话，直接`git log master`即可

  * `=maste[r]`
  * `={master,release-7*}`
  * `--branches=maste[r] --branches=v7.0*` 可以这样指定多个，效果应该符合预期



## `--pretty`参数

详见： https://git-scm.com/docs/pretty-formats





* `--pretty=oneline`

  ```
  * 9dae7ff088531b39e1076xxxxx (tag: v7.0.1, 7.0.xx-yy, 7.0.xx) Fix: interface conversion error
  ```

  一行显示一个commit，默认log的话是分行显示各个信息的（commit msg等）
  
  据说约等于 `--pretty=format:'%Cred%h%Creset -%C(yellow)%d%Creset %s %Cgreen(%cr) %C(bold blue)<%an>%Creset' --abbrev-commit`



### 格式化参数

#### 颜色相关



*%Cred*

switch color to red

*%Cgreen*

switch color to green

*%Cblue*

switch color to blue

*%Creset*

reset color

*%C(…)*

color specification, as described under Values in the "CONFIGURATION FILE" section of [git-config[1\]](https://git-scm.com/docs/git-config). By default, colors are shown only when enabled for log output (by `color.diff`, `color.ui`, or `--color`, and respecting the `auto` settings of the former if we are going to a terminal). `%C(auto,...)` is accepted as a historical synonym for the default (e.g., `%C(auto,red)`). Specifying `%C(always,...)` will show the colors even when color is not otherwise enabled (though consider just using `--color=always` to enable color for the whole output, including this format and anything else git might color). `auto` alone (i.e. `%C(auto)`) will turn on auto coloring on the next placeholders until the color is switched again.

> 详细的color见git config



#### 时间相关

*%ad*

author date (format respects --date= option)

*%aD*

author date, RFC2822 style

*%ar*

author date, relative

*%at*

author date, UNIX timestamp

*%ai*

author date, ISO 8601-like format

*%aI*

author date, strict ISO 8601 format

*%as*

author date, short format (`YYYY-MM-DD`)

*%ah*

author date, human style (like the `--date=human` option of [git-rev-list[1\]](https://git-scm.com/docs/git-rev-list))

*%cd*

committer date (format respects --date= option)

*%cD*

committer date, RFC2822 style

*%cr*

committer date, relative

*%ct*

committer date, UNIX timestamp

*%ci*

committer date, ISO 8601-like format

*%cI*

committer date, strict ISO 8601 format

*%cs*

committer date, short format (`YYYY-MM-DD`)

*%ch*

committer date, human style (like the `--date=human` option of [git-rev-list[1\]](https://git-scm.com/docs/git-rev-list))



## 树形显示

* `git log --pretty=oneline --graph`
* `git log --all --source --pretty=oneline --graph`



## 显示内容



### `--date`

--date=<format>

Only takes effect for dates shown in human-readable format, such as when using `--pretty`. `log.date` config variable sets a default value for the log command’s `--date` option. By default, dates are shown in the original time zone (either committer’s or author’s). If `-local` is appended to the format (e.g., `iso-local`), the user’s local time zone is used instead.

`--date=relative` shows dates relative to the current time, e.g. “2 hours ago”. The `-local` option has no effect for `--date=relative`.

`--date=local` is an alias for `--date=default-local`.

`--date=iso` (or `--date=iso8601`) shows timestamps in a ISO 8601-like format. The differences to the strict ISO 8601 format are:

- a space instead of the `T` date/time delimiter
- a space between time and time zone
- no colon between hours and minutes of the time zone

`--date=iso-strict` (or `--date=iso8601-strict`) shows timestamps in strict ISO 8601 format.

`--date=rfc` (or `--date=rfc2822`) shows timestamps in RFC 2822 format, often found in email messages.

`--date=short` shows only the date, but not the time, in `YYYY-MM-DD` format.

`--date=raw` shows the date as seconds since the epoch (1970-01-01 00:00:00 UTC), followed by a space, and then the timezone as an offset from UTC (a `+` or `-` with four digits; the first two are hours, and the second two are minutes). I.e., as if the timestamp were formatted with `strftime("%s %z")`). Note that the `-local` option does not affect the seconds-since-epoch value (which is always measured in UTC), but does switch the accompanying timezone value.

`--date=human` shows the timezone if the timezone does not match the current time-zone, and doesn’t print the whole date if that matches (ie skip printing year for dates that are "this year", but also skip the whole date itself if it’s in the last few days and we can just say what weekday it was). For older dates the hour and minute is also omitted.

`--date=unix` shows the date as a Unix epoch timestamp (seconds since 1970). As with `--raw`, this is always in UTC and therefore `-local` has no effect.

`--date=format:...` feeds the format `...` to your system `strftime`, except for %z and %Z, which are handled internally. Use `--date=format:%c` to show the date in your system locale’s preferred format. See the `strftime` manual for a complete list of format placeholders. When using `-local`, the correct syntax is `--date=format-local:...`.

`--date=default` is the default format, and is similar to `--date=rfc2822`, with a few exceptions:

- there is no comma after the day-of-week
- the time zone is omitted when the local time zone is used



### `--source`： display commit source

```sh
--source
Print out the ref name given on the command line by which each commit was reached.
```

大约意思是多一个字段用于显示 **从哪reach到该commit的**，一般就是该commit所在的分支名。

如： `* 27182f095943147060d7510886eexxx      7.0.xxx Remove Dockerfile`中的`7.0.xxx`就是



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













