

# tutorial



## display



### 排序



#### size: `-S`

#### 逆序： `-r`





### 额外字段



#### 完整时间： `--time-style=full-iso`







# 其他



## extended attributes



```shell
   The Long Format
     If the -l option is given, the following information is displayed for
     each file: file mode, number of links, owner name, group name, number of
     bytes in the file, abbreviated month, day-of-month file was last modi-
     fied, hour file last modified, minute file last modified, and the path-
     name.  In addition, for each directory whose contents are displayed, the
     total number of 512-byte blocks used by the files in the directory is
     displayed on a line by itself, immediately before the information for the
     files in the directory.  If the file or directory has extended
     attributes, the permissions field printed by the -l option is followed by
     a '@' character.  Otherwise, if the file or directory has extended secu-
     rity information (such as an access control list), the permissions field
     printed by the -l option is followed by a '+' character.

-@      Display extended attribute keys and sizes in long (-l) output.
```



```shell
ls -lh
total 1408
-rw-r--r--@  1 xxx  staff    34K 11 23  2011 COPYING
```



```shell
ls -lh@
total 1408
-rw-r--r--@  1 xxx  staff    34K 11 23  2011 COPYING
	com.apple.quarantine	  71B
```



可以通过如下方式删除

```shell
sudo xattr -d -r com.apple.quarantine ./
# -d就表示删除扩展属性的意思，-r 表示遍历文件夹中的文件，若权限控制符中有@也会去掉
```







