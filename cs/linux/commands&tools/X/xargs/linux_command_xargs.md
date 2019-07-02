



```sh
ps aux | grep java | awk '{print $2}' | xargs -I{} bash -c "kill -9 "{}" || true"
```







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


# mac版本



没找到mac的gnu版本xargs，☹️。



```shell
find ./ -name "*.orig" | xargs -I yonka rm yonka
```













