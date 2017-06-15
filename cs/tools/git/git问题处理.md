## checkout 把branch name认为是file name问题

```
git co feature/user_controlled_site_layouts 
error: pathspec 'feature/user_controlled_site_layouts' did not match any file(s) known to git.
```



或者 



```
git co 2.12.4
# 同样的报错
```



### 处理

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



