



# 命令使用



默认行为： 把所有内容都读取，一起塞到后面的命令末尾。

如： `cat a.txt | xargs rm -rf` 大约等于 

```sh
rm -rf `cat a.txt`

# 假设a.txt内容如下：
1.txt
2 3.txt
4.txt
5  6.txt
```

因为sh在展开`a.txt`内容作为参数时可能根据`IFS`做一些重组，所以结果可能跟预期不符，这时要：

```sh
cat a.txt | xargs -I '{}' rm -rf '{}' 
# 这样才会把每行的内容逐次传给...

echo -e "a\nb" | xargs echo "line: "
line:  a b
# 只执行了一次

echo -e "a\nb" | xargs -I'{}' echo 'line: {}'
line: a
line: b
# 执行了两次
```







```sh
ps aux | grep java | awk '{print $2}' | xargs -I{} bash -c "kill -9 "{}" || true"
```







# cases





## ignore error



```sh
xargs sh -c "somecommand || true"
```





## 长度限制



Turns out that the command was too long, so it was working with shorter file names and failing for longer ones. From `man xargs`

```
-I replstr              Execute utility for each input line, replacing one or more occurrences of replstr in up to replacements (or 5 if no -R flag is specified) arguments to utility with the              entire line of input.  The resulting arguments, after replacement is done, will not be allowed to grow beyond 255 bytes; this is implemented by concatenating as much              of the argument containing replstr as possible, to the constructed arguments to utility, up to 255 bytes.  The 255 byte limit does not apply to arguments to utility              which do not contain replstr, and furthermore, no replacement will be done on utility itself.  Implies -x.
```





## 空格问题 / 分隔符问题



ref： <https://stackoverflow.com/questions/16758525/make-xargs-handle-filenames-that-contain-spaces>



**背景**
```sh
find ./ -name "*.mp3" | xargs -I '{}' bash -c 'd=`dirname "{}"` && mkdir -p "$d"'
# 发现，对于部分包含(尤其是多个)空格的路径，实际拿到的路径不一样，连续空格都变成了单个空格
```

**原因**
`The xargs utility reads space, tab, newline and end-of-file delimited strings from the standard input and executes utility with the strings as arguments.`

**处理办法**

* `ls *.mp3 | xargs -d '\n' mplayer`
  指定只认`\n`
  BSD(如macos)不认这个参数，只有`-0`(也即只认`NULL`)，这时要配合其他的姿势使用，如：
  
  ```sh
  find . -name "*.mp3" -print0 | xargs -0 mplayer
  # or
  ls *mp3 | tr '\n' '\0' | xargs -0 mplayer
  ```
  
  对于macos，可以额外安装合适的版本：
  ```sh
  brew install findutils
  # which gives you the "gxargs" command that does have the -d switch
  ```



> yonka（20201221）： 这个理解不对吧，xargs应该是默认用newline作为； 而原链接实际的问题在于 `ls *mp3 | xargs mplayer` 这样在后面evaluate时候的问题而不是parse input时

# mac版本



没找到mac的gnu版本xargs，☹️。



```shell
find ./ -name "*.orig" | xargs -I yonka rm yonka
```













