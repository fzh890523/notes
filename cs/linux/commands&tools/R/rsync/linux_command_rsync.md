

### 文件选择



#### 排除一些内容



```sh
rsync -r --exclude=".git" ${git_dir} root@10.211.55.3:/tmp/
# 得到： /tmp/`basename ${git_dir}`
# exclude pattern里不需要把 ${git_dir} 的路径加上，也即是相对（要同步内容的）路径
```



```sh
rsync -r --exclude="*.tmp" ${git_dir} root@10.211.55.3:/tmp/
# 排除指定后缀名的文件(支持递归，不只是顶层目录下的)
```



#### 选择一些内容





### 文件覆盖条件



<<<<<<< HEAD
=======
> `rsync` 在处理目标目录下已有的文件时，会根据文件的属性（如大小和修改时间）来确定是否需要重新拷贝覆盖。具体处理方式如下：
>
> 1. 如果源目录中的文件与目标目录中的文件具有相同的文件名、大小和修改时间，则 `rsync` 将跳过该文件，不进行拷贝操作。
> 2. 如果源目录中的文件与目标目录中的文件具有相同的文件名但是不同的大小或修改时间，则 `rsync` 将视为文件内容发生了变化，会重新拷贝源文件到目标目录，覆盖目标目录中的文件。
> 3. 如果源目录中的文件在目标目录中不存在，则 `rsync` 会将源目录中的文件拷贝到目标目录。
>
> 需要注意的是，`rsync` 默认会比较文件的大小和修改时间来判断文件是否需要重新拷贝。如果文件内容相同但是修改时间不同，`rsync` 仍会重新拷贝文件。如果你希望仅根据文件内容来确定是否重新拷贝文件，可以使用 `--checksum` 选项。
>
> 综上所述，`rsync` 会根据文件的属性判断是否需要重新拷贝文件，并确保源目录和目标目录保持一致。



>>>>>>> 1122cdc (re-init 20240121)
* 不覆盖： `--ignore-existing`

* 仅当source文件更大（大覆盖小）： `--append`/`--append-verify`（从名字可以知道，跟预期效果不是那么一致）

  

  ```sh
  --append
  
  This causes rsync to update a file by appending data onto the end of the file, which presumes that the data that already exists on the receiving side is identical with the start of the file on the sending side. If a file needs to be transferred and its size on the receiver is the same or longer than the size on the sender, the file is skipped.
  
  use --append-verify, which, as the man-page states, includes the entire file content in the post-transfer verification process, and resends it if there are discrepancies in the first parts of the file (which the option a priori would consider as identical). In my understanding, this should ensure that even in the case that at first only the "additional" part is actually transferred, the verification will consider the entire content of the file, notice that the parts that would have been the same had your file really only "grown by appending" also differ, and resend the entire file, thus creating a correct copy of it. This of course means unnecessary data transfer and is the reason for my warning about this being non-efficient.
  ```

  



### 删除文件

* 删除copy的文件： `--remove-source-files`

  仅限文件，不包括目录。。。 且没有直接参数可以指定同时删除源文件目录

  > 根据实现，可能不是逐个（传完就）删除，而是小批量删除。 所以看到某文件传完source没立即删除不用惊讶

* 删除target的source不存在的文件： `--delete`





### merge directories



常见的需求：

1. mv： 也即同步好的要删除源文件 (`--remove-source-files`)

2. target已有而source没有的，不影响(也即**不能用**`--delete`)



示例:

```sh
rsync -avh dir1 dir0 --remove-source-files
# dir1 merged to dir0/dir1
```



> 默认应该是会根据文件大小和修改时间来判断相同路径文件是否做update；
>
> 如果期望永远不update的话，加上: `--ignore-existing`
>
> `--update` does something slightly different, which is probably why you are getting unexpected results (see `man rsync`):
>
> ```
> This forces rsync to skip any files which exist on the destination and have a modified time that is newer than the source file. (If an existing destination file has a modification time equal to the source file's, it will be updated if the sizes are different.)
> ```



### 目录对应关系

* `rsync /a/b /backup/`

  会得到 `/backup/b`

* `rsync /a/b/ /backup/`

  会把b下面的内容拷到`/backup/`下



### 一些常用参数



* `-avP` or `-avzP`（多一个开启压缩）

* `--remove-source-files`



* `-a`

  ```sh
  equals -rlptgoD (no -H,-A,-X)
  
  It's all of these:
  
  -r, --recursive recurse into directories
  
  -l, --links copy symlinks as symlinks
  
  -p, --perms preserve permissions
  
  -t, --times preserve modification times
  
  -g, --group preserve group
  
  -o, --owner preserve owner (super-user only)
  
  -D same as --devices --specials
  
  --devices preserve device files (super-user only)
  
  --specials preserve special files
  
  It excludes:
  
  -H, --hard-links preserve hard links
  
  -A, --acls preserve ACLs (implies -p)
  
  -X, --xattrs preserve extended attributes
<<<<<<< HEAD
=======
  
  -h, --help
>>>>>>> 1122cdc (re-init 20240121)
  ```

* `-v` verbose
* `-P` 等同于`--partial --progress`  保留部分传输文件+显示进度

* `-remove-source-files` sync完（某文件）后删除源文件
* `--no-perms` 默认行为，但如果一些其他选项开启了保留（or 同步）perms的话（`-a`），可以通过该选项来额外关闭
* `--omit-dir-times` 一些target fs可能没有权限设置时间（mtime？），这个可以忽略（但仅限目录，还没看到对于普通文件的该功能对应选项）
* `-r` 递归，同步整个目录必须选项
* `--ignore-existing` 忽略（target侧）已有的文件
* `--update` 只同步 target侧没有 或者 source侧timestamp更新 的文件

<<<<<<< HEAD
=======
* `--delete`

  > `--delete` 选项的含义是在同步过程中删除目标目录中不存在于源目录的文件和目录。具体含义如下：
  >
  > 1. 如果源目录中存在某个文件或目录，而目标目录中没有对应的文件或目录，则使用 `--delete` 选项会将目标目录中的该文件或目录删除，以使目标目录与源目录保持一致。
  > 2. `--delete` 选项仅在同步过程中执行删除操作，而不会影响源目录中的文件和目录。
  > 3. 需要注意的是，`--delete` 选项会删除目标目录中与源目录不匹配的文件和目录，因此请谨慎使用，以免意外删除了重要的数据。



## 一些典型场景



### src/dst目录保持完全一致



`rsync -avP -r --delete src/ dst/`





### src合并到dst目录



>>>>>>> 1122cdc (re-init 20240121)


















