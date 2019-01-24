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



### 不区分大小写 /\c${pattern}





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











