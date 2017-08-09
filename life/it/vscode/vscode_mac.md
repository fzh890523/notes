# c/cpp dev env



默认好像就安装了`cpptool`（名字好像改了），所以默认支持c/cpp。



## search symbol



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

   ​













