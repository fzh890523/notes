# basic

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

* `git push origin v1.0.0`/`git push origin --tags`



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





### sort by date



> Lightweight (non-annotated) tags do only point to another object (like a commit, which has a date). See the one of the other answers to print these (creatordate).
>
> Annotated tags do carry a date, an author and a message. The one of the other answers to print these (taggerdate).



* annotated tag可以

  ` git tag -l --sort=-creatordate --format='%(creatordate:short):  %(refname:short)'`

* lightweight tag不行





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



# 场景





