## 参数处理



### 文件指定



#### 跳过已存在的同样文件： -n



```sh
cp -rf * /tmp/copy/
# 中断重试时
cp -rf -n * /tmp/copy/  # 避免重复已完成的内容
```





## symbol link

### `-H` 和 `-L` 区别

ref： https://unix.stackexchange.com/questions/94714/cp-l-vs-cp-h



The difference in behavior between `-L` and `-H` comes when `-r` is specified as well. `cp` won't create symlinks in subdirectories with `-L -r` but it will if you use `-H -r`.



`-H` If a command line argument specifies a symbolic link, then **copy the file it points to rather than the symbolic link** itself. However, **copy (preserving its nature) any symbolic link** that is encountered via recursive traversal.



`-L, --dereference` Follow symbolic links when copying from them. With this option, `cp` cannot create a symbolic link. For example, a symlink (to regular file) in the source tree will be copied to a regular file in the destination tree.



* `-H` 对于指定的单个文件，会follow，但在递归copy场景不会
* `-L` 则递归场景也会



```sh
mkdir a
touch a/b
cd a && ln -s b b.link && cd ..

cp a/b b1
cp -r a a1
cp -rH a a2
cp -rL a a3

ls -l | grep b1

-rw-r--r--   1 yonka   users            0 Jun 17 16:01 b1

ls a*

a:
total 0
-rw-r--r-- 1 yonka users 0 Jun 17 16:01 b
lrwxrwxrwx 1 yonka users 1 Jun 17 16:01 b.link -> b

a1:
total 0
-rw-r--r-- 1 yonka users 0 Jun 17 16:01 b
lrwxrwxrwx 1 yonka users 1 Jun 17 16:01 b.link -> b

a2:
total 0
-rw-r--r-- 1 yonka users 0 Jun 17 16:01 b
lrwxrwxrwx 1 yonka users 1 Jun 17 16:01 b.link -> b

a3:
total 0
-rw-r--r-- 1 yonka users 0 Jun 17 16:01 b
-rw-r--r-- 1 yonka users 0 Jun 17 16:01 b.link
```





