



Ref: <https://www.howtogeek.com/howto/16226/complete-guide-to-symbolic-links-symlinks-on-windows-or-linux/>



需要管理员权限。



Without any extra options, `mklink` creates a symbolic link to a file. The below command creates a symbolic, or “soft”, link at `Link` pointing to the file `Target` :

```
mklink Link Target
```

Use /D when you want to create a soft link pointing to a directory. like so:

```
mklink /D Link Target
```

Use /H when you want to create a hard link pointing to a file:

```
mklink /H Link Target
```

Use /J to create a hard link pointing to a directory, also known as a directory junction:

```
mklink /J Link Target
```



So, for example, if you wanted to create a hard link at C:\LinkToFolder that pointed to C:\Users\Name\OriginalFolder, you’d run the following command:

```
mklink /J C:\LinkToFolder C:\Users\Name\OriginalFolder
```

You’ll need to put quotation marks around paths with spaces. For example, if the folders are instead named C:\Link To Folder and C:\Users\Name\Original Folder, you’d use the following command instead:

```
mklink /J "C:\Link To Folder" "C:\Users\Name\Original Folder"
```



GUI支持的话，可以安装  [Link Shell Extension](http://schinagl.priv.at/nt/hardlinkshellext/linkshellextension.html) 。



