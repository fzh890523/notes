

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







### 删除文件

* 删除copy的文件： `--remove-source-files`
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