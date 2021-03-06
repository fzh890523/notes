ref：

* [Introducing Windows Terminal](https://devblogs.microsoft.com/commandline/introducing-windows-terminal/)

* [github-Microsoft/Terminal](https://github.com/Microsoft/Terminal)
* [terminal-profile-settings](https://aka.ms/terminal-profile-settings) or [windows/terminal/customize-settings/profile-general](https://docs.microsoft.com/en-us/windows/terminal/customize-settings/profile-general)



### 设置default shell



设置 - 进入settings.json - 把`defaultProfile`后面的uuid设置为下面`profiles.list[需要的那个比如wsl].guid`即可



### 添加新shell

类似：

```sh
            {
                "name": "git bash",
                "hidden": false,
                "commandline": "C:\\Program Files\\Git\\bin\\bash.exe"
            }
```

* 不能用软链接，完整带后缀路径也不行
* 可能是search path问题，其他... 需要用完整路径



### 复制多行

默认行为是： 显式为多行的，copy出来就是多行，即使原来是单行只是因为宽度原因换行了。

要把显式多行实际单行的复制为单行的话，操作如下：

1. 选择开始区域（选中部分内容）
2. 按下shift
3. 点击结束区域（或者一直拉过去也可以）
4. 放开shift
5. 右键复制



或者按下shift然后拖动选择要复制的（多行显示内容）然后放开shift+右键。



