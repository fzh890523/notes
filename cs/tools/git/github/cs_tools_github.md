

## auth



### ssh key



#### github添加ssh key时报错ssh key in use问题



> from stackoverflow

You can create one more key say `id_rsa_personal.pub` and add it to the github account. In order to access both the github accounts from same pc, edit `.ssh/config` file.

```
    # Default GitHub
    Host github.com
      HostName github.com
      User git
      IdentityFile ~/.ssh/id_rsa

    Host github-public
      HostName github.com
      User git
      IdentityFile ~/.ssh/id_rsa_public

    Host github-personal
      HostName github.com
      User git
      IdentityFile ~/.ssh/id_rsa_personal

```

The above file will help you parallely with two github accounts. Also u can refer [Multiple ssh keys](https://stackoverflow.com/questions/2419566/best-way-to-use-multiple-ssh-private-keys-on-one-client)

James Ferguson added this important bit of information:

> ... you [may] need to alter your .git/config remote url to point to `git@github-personal/<reponame>.git` etc., rather than the usual `git@github.com/<reponame>.git`



##### example



```sh
ssh-keygen  # specify filepath, notice to not to override the original one
```





```shell
vim ~/.ssh/config
```



```
# Default GitHub
Host github.com
  HostName github.com
  User git
  IdentityFile ~/.ssh/id_rsa

# user2  GitHub
Host github-user2
  HostName github.com
  User git
  IdentityFile ~/.ssh/id_rsa_1

# user1  GitHub  -- default user
Host github-user1
  HostName github.com
  User git
  IdentityFile ~/.ssh/id_rsa
```



```sh
# cd to repo-dir
vim .git/config
# change remote.url to ...
```

```
[remote "origin"]
	url = git@github-user2:user2/tcpproxy.git
```



```sh
add ~/.ssh/id_rsa_1.pub to github user2's ssh keys
```



## merge



### squash merge

* commit author算谁？ 

  算原来的，但执行merge的人会多一行msg `Co-authored-by: `

* 如果只有一个commit，算ff吗？ 还是commit id也会变？

  不算ff； 会变



### fast-forward： 不支持

github的rebase-merge，看描述是不增加merge commit，实际也确实不增加，但commit id变了，导致`pull`时还是会merge，导致变成了`change-commit1 - change-commit2 - merge-commit3` 吐血。。。

> The rebase and merge behavior on GitHub deviates slightly from `git rebase`. Rebase and merge on GitHub will always update the committer information and create new commit SHAs, whereas `git rebase` outside of GitHub does not change the committer information when the rebase happens on top of an ancestor commit. 
>
> https://docs.github.com/en/github/administering-a-repository/configuring-pull-request-merges/about-merge-methods-on-github



## pull request



### 联动issue



参考： https://docs.github.com/en/issues/tracking-your-work-with-issues/linking-a-pull-request-to-an-issue



github支持的关键字

- close
- closes
- closed
- fix
- fixes
- fixed
- resolve
- resolves
- resolved



使用场景：

* pr desc
* pr commit msg



搜索issue： 暂时没用到，用的是交互里的 链接-搜索 



样例：

* `close #10`
* `resolves #10, resolves #123, resolves octo-org/octo-repo#100`

