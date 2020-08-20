

# Python控制台调试模式IPDB



# IPDB

前一段时间刚接触到这个, 之前 `DEBUG` 还停留在 `IDE` 层面… 原来还可以这么玩…

`IPDB` 是什么？`IPDB`（Ipython Debugger），和 `GDB` 类似，是一款集成了 `Ipython` 的 `Python` 代码命令行调试工具，可以看做 `PDB` 的升级版

那么 `PDB` 是什么? - `Python Debugger`

推荐直接使用: `IPDB` 毕竟是增强版: 支持Tab补全; 支持语法高亮; 支持 `iPython`

# 基础用法

有2种方式启用 `pdb` 一种是代码里导入 `pdb` 模块; 一种是命令行启用;

懒人当然选第二种(不用改代码): `python3 -m ipdb myscript.py`

默认是按行执行的, 按 `回车` 或者 `n` 执行下一行

**常用的命令**:

-  `h` 快捷的帮助文档
-  `l` 展示更多的源码
-  `b 10` 给第10行设置断点
-  `c` 进入下一个设置的断点
-  `p` 打印变量值
-  `s` 进入函数
-  `r` 跳出函数
-  `q` 退出



```bash
ipdb> h

Documented commands (type help <topic>):
========================================
EOF    cl         disable  interact  next    psource  rv         unt
a      clear      display  j         p       q        s          until
alias  commands   down     jump      pdef    quit     source     up
args   condition  enable   l         pdoc    r        step       w
b      cont       exit     list      pfile   restart  tbreak     whatis
break  continue   h        ll        pinfo   return   u          where
bt     d          help     longlist  pinfo2  retval   unalias
c      debug      ignore   n         pp      run      undisplay

Miscellaneous help topics:
==========================
exec  pdb

ipdb> h c
c(ont(inue))
        Continue execution, only stop when a breakpoint is encountered.
```



# PDB 命令表

常用的命令一般都是一个字母的简写, 使用完成的命令也是可以的

**整理的命令表(Python3)**: [参考:pdb-document](https://links.jianshu.com/go?to=https%3A%2F%2Fdocs.python.org%2F3%2Flibrary%2Fpdb.html%23pdbcommand-help)

1.  `h(elp)` - 默认展示全部命令列表!!! `h b` 设置断点的文档!!! 会这个, 其他的就简单了

2.  `w(here)` - 打印堆栈信息

3.  `d(own)` - Move the current frame count (default one) levels down in the stack trace (to a newer frame)

4.  `u(p)` - Move the current frame count (default one) levels up in the stack trace (to an older frame)

5. ```
   b(reak) [([filename:]lineno | function) [, condition]]
   ```

    \- 断点 

   - 不带参数时列出所有断点信息
   - 行号 - 给指定的行设置断点
   - 函数名 - 给指定函数设置断点
   - 文件名:行号 - 给指定文件名的某行设置断点 (此文件可能还没引人)
   - 如果有第二个参数, 则条件结果为 `True` 时设置断点

6. ```
   tbreak [([filename:]lineno | function) [, condition]]
   ```

    \- 临时断点 

   - 参数和 `break` 一样; 第一次命中断点后自动删除

7. ```
   cl(ear) [filename:lineno | bpnumber [bpnumber ...]]
   ```

    \- 清除断点 

   - 不带参数时, 清除所有断点(会先提示先确认)
   - 文件:行号 - 清理指定文件对应行号的断点
   - 断点号1 断点号2 … - 清理指定编号的断点; 断点编号列表可以使用 `b` 查看

8.  `disable [bpnumber [bpnumber ...]]` - 禁用断点, 不是删除; 后面可以重新开启

9.  `enable [bpnumber [bpnumber ...]]` - 启用禁用的断点

10.  `ignore bpnumber [count]` - 断点忽略断点次数, 直到次数为0时, 断点才会生效; 功效应该和几次`disable`后再`enable`一样

11.  `condition bpnumber [condition]` - 给指定编号的断点设置条件

    ```go
    *   没有条件参数时 - 去掉断点上面已有的条件
    *   有条件参数是 - 设置新的条件, 当条件值为 `True` 时, 断点才会生效
    ```

12. `commands [bpnumber]` - 给断点设置命令列表

    ```go
    *   删除断点所有命令, 只需要输入 `commands` 后立即输入 `end` 即可
    *   没有 断点编号 参数时, 默认为最后一个断点
    ```

13. `s(tep)` - 进入当前行对应的函数里

14. `n(ext)` - 下一步

15. `unt(il) [lineno]` - 正常执行直到某一行为止; 有点像临时断点, 有什么区别么?

16. `r(eturn)` - 跳出当前方法函数

17. `c(ont(inue))` - 继续执行, 知道碰到断点

18. `j(ump) lineno` - 直接跳转到某行执行, 中间跳过的代码不会执行!!!

19. `l(ist) [first[, last]]` - 展示当前行邻近的源代码; `l 10,20` 展示第10到20行代码

20. `ll | longlist` - 展示本文件所有代码

21. `a(rgs)` - 打印当前函数的参数列表, 以及对于参数的值

22. `p expression` - 打印当前 表达式 的值; 比如变量等

23. `pp expression` - 带格式化的打印 表达式 的值

24. `whatis expression` - 打印 表达式 的类型

25. `source expression` - 尝试获取 表达式 的源码

26. `display [expression]` - 显示 表达式 的值; 如表达式的值有变动, 那么就会实时打印结果, 动态跟踪

27. `undisplay [expression]` - 取消 表达式 的显示

28. `interact` - 启动一个交互式解释器

29. `alias [name [command]]` - 给命令起别名, 具体还是看文档吧

30. `unalias name` - 取消别名

31. `run | restart [args ...]` - 重启启动调试

    ```go
    *   之前的操作, 断点, 设置选项都将保留
    *   `restart` 是 `run` 的别名;
    ```

32. `q(uit)` - 退出调试模式



如果不全的话, 请直接参考: `h pinfo` 形式

