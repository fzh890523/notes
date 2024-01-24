

## 概念/模型/理念/流程



**概念**

* vault（密库？）： 大约对应一个repo

  类似一个proj，ob可以配置打开vault视图，方便在某vault中打开另一个vault

* 





## 配置



目前了解到的，obsidian的配置都在当前目录下： `./.obsidian`，目录结构如下：

```sh
.obsidian/
├── appearance.json
├── app.json
├── community-plugins.json
├── core-plugins.json
├── core-plugins-migration.json
├── hotkeys.json
├── plugins
│   ├── calendar
│   │   ├── data.json
│   │   ├── main.js
│   │   └── manifest.json
│   ├── obsidian-community-plugins-proxy
│   │   ├── data.json
│   │   ├── main.js
│   │   └── manifest.json
# 其他插件省略
```



## 插件



* core plugins

* community plugins

  > 这个网址好像被q了，官方没解决办法，只能全局..
  >
  > https://github.com/PhoenixFEC/obsidian-community-plugins-proxy?ref=eleanorkonik.com 这个项目做了支持，大致是用一个插件来为`community plugins`增加代理功能
  >
  > 而显然这个插件本身是需要

  从前面配置目录结构可以看出：

  * 插件由js+json配置+css等布局渲染文件组成/编写
  * 插件有约定的“接口“，包含文件布局等

  安装方式：

  * UI交互安装

  * 下载对应文件到对应目录，例如

    > You should add **GProxy** to your local plugins floder,
    >
    > ```
    > Vault -> .obsidian -> plugins -> obsidian-community-plugins-proxy
    > ```
    >
    > Includes:
    >
    > - [main.js](https://github.com/PhoenixFEC/obsidian-community-plugins-proxy/releases/download/0.2.0/main.js)
    > - [manifest.json](https://github.com/PhoenixFEC/obsidian-community-plugins-proxy/releases/download/0.2.0/manifest.json)



## 多端支持



* ios： 有，看着还比较全功能

  详见使用文档



## 功能



### sync

详见使用文档

