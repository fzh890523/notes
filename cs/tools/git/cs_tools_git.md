# history



## file commit history



### gitk ${filepath}方式





# commit



## date



* `git commit --date="10 day ago" -m "Your commit message" `

* ```shell
  git commit --date="`stat -c %y myfile`" myfile
  ```

* `git commit --date="$(date -v-1d)"`







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



* `git tag -d <tagname>`

  `git tag -d 0.1.0.2`






# merge



## cherry-pick



```shell
# develop上有个hotfix commit（commit号为abcd...），但develop本身还来不及merge到master(比如commit太多)

git checkout master
git cherry-pick -x abcd  # -x的话生成标准commit message，减少重复merge
```



























