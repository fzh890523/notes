

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













