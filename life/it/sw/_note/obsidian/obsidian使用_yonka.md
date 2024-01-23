



# 插件



见单独的



# 移动端使用



## ios



1. 相对而言ios端交互还是没那么方便，所以可以在pc端把环境准备（使用配置，插件安装等）搞定

2. `.gitignore`处理

   确保有：

   ```
   .git-credentials
   ```

   

3. 新拉取一份repo，只拉主分支，以使内容尽量少

   在此之前可以把repo处理一下让主分支只有一个commit（详见git文档`创建“孤儿”分支`章节）

4. git remote改为https，把用户名密码credential保存在在repo下

   详见： [obsidian插件-git_yonka.md#移动端支持](obsidian插件-git_yonka.md#移动端支持) 和 git文档

   ```sh
   # git remote set-url origin https://...
   git config credential.helper "store --file .git-credentials"
   git fetch
   # 提示输入账号密码。 访问成功的话会保存到...
   
   # .git-credentials格式
   https://<username>:<password>@<domain>
   ```

   

5. 部分配置改为repo级别

   考虑到ios端没global git配置，所以把必要配置提前在repo级别设置是有必要的（前面的credentials也算一种）。 虽然后续能在插件配置中设置部分，但终究没那么方便

   * user.name
   * user.email
   * core.quotepath false
   * core.autocrlf input    （其他平台根据情况设置，比如win用true比较好）
   * core.filemode false

   ```sh
   git config user.name yonka
   git config user.email yonka.fang@foxmail.com
   git config core.quotepath false
   git config core.autocrlf input  # 这一行不在后续还要继续使用的windows repo中执行
   git config core.filemode false
   ```

   

6. `tar`成`<vaultName>.tar.gz`文件，丢到nas上

7. 文件管理器从nas拉取，放到`obsidian`目录下，解压，此时目录为 `obsidian/<vaultName>`

   如果windows - linux/macos/ios 互传可能crlf问题，obsidian-git没法设置这个，所以需要自行解决

   * 打包前先用工具转换一遍
   * 在linux上clone/打包，再 -> ios
   * 在ios侧reset一下

8. 打开obsidian即可看到这个新vault，打开即可使用，插件等因为在文件中承载，也可以继续使用（包含git-sync等）

