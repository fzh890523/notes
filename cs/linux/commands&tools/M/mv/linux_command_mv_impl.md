

## mv命令syscall



```sh
...
stat("/tmp", {st_mode=S_IFDIR|S_ISVTX|0777, st_size=12288, ...}) = 0
lstat("/mnt/disk1/test", {st_mode=S_IFREG|0644, st_size=4, ...}) = 0
lstat("/tmp/test", 0x7ffe928bf1f0)      = -1 ENOENT (No such file or directory)
rename("/mnt/disk1/test", "/tmp/test")  = -1 EXDEV (Invalid cross-device link)
unlink("/tmp/test")                     = -1 ENOENT (No such file or directory)
open("/mnt/disk1/test", O_RDONLY|O_NOFOLLOW) = 3
fstat(3, {st_mode=S_IFREG|0644, st_size=4, ...}) = 0
open("/tmp/test", O_WRONLY|O_CREAT|O_EXCL, 0600) = 4
fstat(4, {st_mode=S_IFREG|0600, st_size=0, ...}) = 0
ioctl(4, BTRFS_IOC_CLONE, 0x3)          = -1 EXDEV (Invalid cross-device link)
fadvise64(3, 0, 0, POSIX_FADV_SEQUENTIAL) = 0
...
read(3, "123\n", 131072)                = 4
write(4, "123\n", 4)                    = 4
read(3, "", 131072)
...
newfstatat(AT_FDCWD, "/mnt/disk1/test", {st_mode=S_IFREG|0644, st_size=4, ...}, AT_SYMLINK_NOFOLLOW) = 0
unlinkat(AT_FDCWD, "/mnt/disk1/test", 0) = 0
...
```



## "mv"操作原子性问题



### 结论



* 同一个`mount point`内，`rename`是原子

  所以`mv`命令实现时会首先尝试`rename`，失败则倒回到。。

* 不同`mount point`之间，普通的`open` and `write`

  而不是下面ref中比喻的

  ```sh
  cp /src/filename /dst/filename.tmp &&  # 没有给出这种比较低层操作的syscall api： 写文件内容再写文件元信息到dir文件
  mv /dst/filename.tmp /dst/filename &&
  rm /src/filename
  ```

* 其实很多时候需要的不是完全原子性： 同时查看到source dir下文件消失和target dir下文件出现，而是文件copy/mv到target dir这个操作的"可并发性"

  也即：

  ```sh
  cp /src1/filename1 /dst/filename
  mv /src2/filename2 /dst/filename
  # 这两个操作，不关心最终/dst下是哪个文件，只要求是某一个完整文件
  # 理论上，只要求写dir操作是原子的。 但没有合用的api吧，还是要用到rename，限制在同一个 mount point 下
  ```

  



### ref





<https://unix.stackexchange.com/questions/452084/is-there-a-way-to-atomically-move-file-and-directory-from-tempfs-to-ext4-partiti>



> Is moving file from `tmpfs` to `ext4` atomic?

No. Renames as such only work within a filesystem. The manual page for `rename(2)` explicitly mentions the error that is returned if trying to rename across mount points:

> `EXDEV` *oldpath* and *newpath* are not on the same mounted filesystem.

Moves across file systems need to be done as a combination of a copy and a delete. `mv` will do this for you if the `rename()` doesn't work, but it will not be atomic in that case.

The simple way to work around that would indeed be to first copy the file to a temporary location on the same filesystem. In general, it's simplest to place the temporary file in the same directory as the final destination, since that's the only place that's guaranteed to be on the same filesystem. Of course that requires that any process working on the files there will have some logic to ignore the temporary based on its name.

Roughly, something like this should work for one file:

```
cp /src/filename /dst/filename.tmp &&
mv /dst/filename.tmp /dst/filename &&
rm /src/filename
```

Note that the process you describe for a *directory* is essentially this:

```
cp -r /src/dir /dst/dir.tmp && 
mv /dst/dir /dst/dir.bak    &&
mv /dst/dir.tmp /dst/dir    &&
rm -r /dst/dir.bak
```

Which is not bad, but is *not* atomic. There's a moment of time between the two runs of `mv` (or calls to `rename()`), when `/dst/dir` does not exist. That could be worked around by accessing the directory through a symlink, since the link can be atomically replaced with a rename.











