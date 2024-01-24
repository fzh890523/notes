



# storage



## link repo to

把wc中repo（dir）link到其他app，让其他app使用，这里有个前提是其他app暴露了存储（也即在`file` app中能点进去），创建好了<del>作为挂载点的</del>目录



* link了的repo上会有链接的图标

* link后，`file`中查看wc中本repo dir就成了一个100B左右的特殊文件，实际应该是个`link`

  把这个链接文件拷贝到es下，在es中能看到这个目录，并且在其中创建文件能在源目录中看到 -- **证明这是个link，而不是mount予以**

  

  **但这违反了认知： 原以为source dir在wc侧，但其实好像link操作把source dir拷贝到dest dir同时在本地保留了一个link**

  这就导致了以下现象： 

  * 在wc内做unlink后，wc内再也没这个repo了，而dest dir中的内容还在 （提示信息说会删除git history）

    > 测试了下，还真是。 dest dir中没有git内容了
    >
    > 可以再link-ext-repo回来查看，此时需要init-repo了

  * 一个repo重复link到多个target（如target1，target2），实际上的行为是：

    1. copy (content) to target1

    2. create link pointing to target1

    3. copy (content) to target2

    4. change link to point to target2

       自此target2 和 target1是独立的两份数据了...



## link external repo

ref: https://workingcopyapp.com/manual/external-repos



把其他位置（一般是其他app的存储空间中）的repo（dir）link到wc来使用，等于wc管理其他app中内容

这个一个典型场景是 icloud 等特殊app中内容

> 尝试link termius中remote host中内容，交互上dir能进去，但无法选择（“打开”）其中的git repo目录。 虽然这样意义也不大，太卡



交互上可以：

* `+` - `link external app` 然后在打开的交互中选择...

* `file` app里拖拽目录到wc dir

  有些目录如 `Swift Playgrounds and Codea projects that are not treated as folders by iOS.`，只能用这种方式。 估计前一种方式里无法选中

  

这个看起来实现也比较简单： 就是在wc中创建一个`link`指向dest dir，**而不是mount语义**



更多细节：

> A .git folder is created if the directory you pick isn't a repository and kept inside Working Copy to not confuse other apps. You can change the location of the .git directory from the repository configuration screen.
>
> 