# c/cpp dev env



默认好像就安装了`cpptool`（名字好像改了），所以默认支持c/cpp。



## search symbol



* `工作区中查找`： `cmd + T`
* `当前文件中查找`： `shift+cmd + O`



### 相关配置

### 



不清楚是否只有按如下改才生效，但



1. `shift+cmd+p` - `C/cpp: edit configuration`

   ```json
   {
       "configurations": [
           {
               "name": "Mac",
               "includePath": [
                   "/usr/include",
                   "/usr/local/include",
                   "/usr/include/c++",  // for c++ stdlib
                   "/Users/xx/git_root/external/c/glibc-2.25"  // 加入项目目录
               ],
               "browse": {
                   "limitSymbolsToIncludedHeaders": false,  // 改为false
                   "databaseFilename": "glibc.db"
               }
           },
           {
               "name": "Linux",
               "includePath": [
                   "/usr/include",
                   "/usr/local/include"
               ],
               "browse": {
                   "limitSymbolsToIncludedHeaders": true,
                   "databaseFilename": ""
               }
           },
           {
               "name": "Win32",
               "includePath": [
                   "C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/include/*"
               ],
               "browse": {
                   "limitSymbolsToIncludedHeaders": true,
                   "databaseFilename": ""
               }
           }
       ]
   }
   
   ```

   ​

2. `shift+cmd+p` - `settings`

   ```Json
   // 将设置放入此文件中以覆盖默认值和用户设置。
   {
       "[cpp]": {
           "editor.quickSuggestions": true
       },
       "[c]": {
           "editor.quickSuggestions": true
       }
   }
   ```




# settings



## workspace settings



`shift+cmd + P`然后输入`settings`然后选择`首选项：打开工作区设置`



文件在： `.vscode/settings.json`




# file



## search file: cmd+p



然后输入文件名



## file encoding设置



`.vscode/settings.json` 中加入类似： `"files.encoding": "gbk"`



ref: https://stackoverflow.com/questions/30082741/change-the-encoding-of-a-file-in-visual-studio-code

Apart from the settings explained in the answer by @DarkNeuron:

```
"files.encoding": "any encoding"
```

you can also specify settings for an specific language like so:

```
"[language id]": {
  "files.encoding": "any encoding"
}
```

For example, I use this when I need to edit PowerShell files previously created with ISE (which are created in ANSI format):

```
"[powershell]": {
    "files.encoding": "windows1252"
}
```

You can get a [list of identifiers of well-known languages here](https://code.visualstudio.com/docs/languages/identifiers).



# content



* 前进： `shift+cmd + -`
* 后退： `cmd + -`












