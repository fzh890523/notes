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



# pull



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



























