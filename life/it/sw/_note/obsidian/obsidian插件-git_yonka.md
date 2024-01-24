

## 移动端支持

在ios端不支持ssh-auth，需要切换到https，credential估计要设成保存在当前目录下



（ios下）几个问题：

* 打包拷到ob目录的方式： 发现它很慢/卡

  https://github.com/denolehov/obsidian-git/issues/562#issuecomment-1732563081 提到需要用[官方推荐的方式（在vault中clone）](https://github.com/denolehov/obsidian-git/wiki/Installation#mobile)否则是会这样，但又遇到下面的问题

* 用[官方推荐的方式-在vault中clone](https://github.com/denolehov/obsidian-git/wiki/Installation#mobile) 遇到crash问题

  怀疑是这个过程中会删除local `.obsidian`目录，导致ob app本身逻辑错误，等于是ob bug，但没办法。。。

* 用 https://meganesulli.com/blog/sync-obsidian-vault-iphone-ipad/ 的 `Working Copy + OB` 方案，再尝试ob-git时，报错说不是有效的git仓库（`can't find a valid git repo`），可能哪里兼容性有问题 = =

  wc clone到ob link过来的空dir，有问题 如上（按原po的流程： 1. ob创建vault （dir）； 2. dir link到wc； 3. wc clone）

  > 另两种方式：
  >
  > * wc link 到ob
  >
  >   1. wc clone
  >   2. wc link 到 ob
  >   3. ob中打开，使用ob-git
  >
  >   此时ob-git先是提示选择remote，然后拉取时crash （`notes` crash，另一个小repo ok）
  >
  > * wc copy 到ob
  >
  >   1. wc clone
  >   2. wc copy 到 ob
  >   3. ob中打开，使用ob-git
  >
  >   此时ob-git先是提示输入用户名跟密码，然后拉取时crash
  >
  > > 以上问题估计是因为`notes`这个repo太大而ob-git用的git库有bug导致

  

