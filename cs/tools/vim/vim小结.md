# 设置



## 显示



### 行号 `set number`







## 特殊设置

### [Vim中显示不可见字符](http://blog.csdn.net/wusecaiyun/article/details/50435416)                     

```


在Linux中，cat -A file可以把文件中的所有可见的和不可见的字符都显示出来，在Vim中，如何将不可见字符也显示出来呢？当然，如果只是想在Vim中查看的话，可以这样:%!cat -A在Vim中调用cat转换显示。这样的做法不便于编辑，其实Vim本身是可以设置显示不可见字符的。

只需要:set invlist即可以将不可见的字符显示出来，例如，会以^I表示一个tab符，$表示一个回车符等。

或者，你还可以自己定义不可见字符的显示方式：

set listchars=eol:$,tab:>-,trail:~,extends:>,precedes:<
set list

最后，:set nolist可以回到正常的模式。
```



# cmd



## 打开文件： `:e`





# text操作



## 复制粘贴



### 复制到系统剪贴板/从系统剪切板粘贴



亲试可用

1. 命令模式选中
2. 输入 `"*y`即可，会提示已经copy到 `"*`
3. 在需要粘贴的地方输入`"*p`即可



其他方式：

On Mac **OSX**

- copy selected part: visually select text(type `v` or `V` in normal mode) and type `:w !pbcopy`

  试了下，好像全部copy了而不是选中 = =

- copy the whole file `:%w !pbcopy`

- paste from the clipboard `:r !pbpaste`

On *most* **Linux** Distros, you can substitute:

- `pbcopy` above with `xclip -i -sel c` or `xsel -i -b`
- `pbpaste` using `xclip -o -sel -c` or `xsel -o -b`
  -- **Note:** *In case neither of these tools (xsel and xclip) are preinstalled on your distro, you can probably find them in the repos*




## 查找



### 跨行查找： `abc.*\n.*def`

似乎简单的`\n`即可



### 不区分大小写 /\c${pattern}



## 替换



### 换行处理



简而言之，要把 行首的`foo`换成

```
@bar
foo
```

的话： `:s/^foo/@bar\rfoo/g`

要所有行处理的话： `:%s ...`



ref: https://stackoverflow.com/questions/71323/how-to-replace-a-character-by-a-newline-in-vim



Here's the trick:

First, set your Vi(m) session to allow pattern matching with special characters (i.e.: newline). It's probably worth putting this line in your .vimrc or .exrc file:

```
:set magic
```

Next, do:

```
:s/,/,^M/g
```

To get the `^M` character, type Ctrl + V and hit Enter. Under Windows, do Ctrl + Q, Enter. The only way I can remember these is by remembering how little sense they make:

> A: *What would be the worst control-character to use to represent a newline?*
>
> B: *Either `q` (because it usually means "Quit") or `v` because it would be so easy to type Ctrl + C by mistake and kill the editor.*
>
> A: *Make it so.*





In the syntax `s/foo/bar`, `\r` and `\n` have different meanings, depending on context.

------

**Short:**

For `foo`:

`\r` == "carriage return" (`CR` / `^M`)
`\n` == matches "line feed" (`LF`) on Linux/Mac, and `CRLF` on Windows

For `bar`:

`\r` == produces `LF` on Linux/Mac, `CRLF` on Windows
`\n` == "null byte" (`NUL` / `^@`)

When editing files in linux (i.e. on a webserver) that were initially created in a windows environment and uploaded (i.e. FTP/SFTP) - all the `^M`'s you see in vim, are the `CR`'s which linux does not translate as it uses only `LF`'s to depict a line break.

------

**Longer (with ASCII numbers):**

`NUL` == 0x00 == 0 == Ctrl + @ == `^@` shown in vim
`LF` == 0x0A == 10 == Ctrl + J
`CR` == 0x0D == 13 == Ctrl + M == `^M` shown in vim

Here is a list of the [ASCII control characters](http://www.cs.tut.fi/~jkorpela/chars/c0.html). Insert them in Vim via Ctrl + V,Ctrl + ---key---.

In Bash or the other Unix/Linux shells, just type Ctrl + ---key---.

Try Ctrl + M in Bash. It's the same as hitting Enter, as the shell realizes what is meant, even though Linux systems use line feeds for line delimiting.

To insert literal's in bash, prepending them with Ctrl + V will also work.

Try in Bash:

```
echo ^[[33;1mcolored.^[[0mnot colored.
```

This uses [ANSI escape sequences](http://en.wikipedia.org/wiki/ANSI_escape_code). Insert the two `^[`'s via Ctrl + V, Esc.

You might also try Ctrl + V,Ctrl + M, Enter, which will give you this:

```
bash: $'\r': command not found
```

Remember the `\r` from above? :>

This [ASCII control characters](http://www.cs.tut.fi/~jkorpela/chars/c0.html) list is different from a complete [ASCII symbol table](http://ascii-code.com/), in that the control characters, which are inserted into a console/pseudoterminal/Vim via the Ctrl key (haha), can be found there.

Whereas in C and most other languages, you usually use the octal codes to represent these 'characters'.

If you really want to know where all this comes from: *[The TTY demystified](http://www.linusakesson.net/programming/tty/)*. This is the best link you will come across about this topic, but beware: There be dragons.

------

*TL;DR*

Usually `foo` = `\n`, and `bar` = `\r`.





# 跳转



## 通用跳转

### C-O 返回跳转前位置 

### C-T 同上 

### nC-T 返回跳转 n 次



## 行跳转

### 0 数字0,跳转至行首 

### ^ 跳转至行第一个非空字符 

### $ 跳转至行尾

​                    



## 句子跳转

### ( 移到这个句子的开头

### ) 移到下一个句子的开头



## 段落跳转

### { 跳到上一段的开头

### } 跳到下一段的的开头



## 括号跳转



### % 跳转到相配对的括号



## 函数跳转



### [[ 跳转至上一个函数(要求代码块中'{'必须单独占一行)

### ]] 跳转至下一个函数(要求代码块中'{'必须单独占一行)

### C-] 跳转至函数或变量定义处











