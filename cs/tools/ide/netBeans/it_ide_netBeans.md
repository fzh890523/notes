

# 配置选项



## general



### 设置“语言”



 Control-click the NetBeans application icon and choose Show Package Contents from the contextual menu. In the Finder window that appears, navigate to the `Contents/Resources/NetBeans/etc/` directory, Control-click `netbeans.conf`



```shell
netbeans_default_options="-J-client -J-Xss2m -J-Xms32m -J-Dapple.laf.useScreenMenuBar=true -J-Dapple.awt.graphics.UseQuartz=true -J-Dsun.java2d.noddraw=true -J-Dsun.java2d.dpiaware=true -J-Dsun.zip.disableMemoryMapping=true"
```



在后面追加：

```shell
-J-Duser.language=en -J-Duser.region=US
```



# 语言



## c/c++





### 杂项



#### symbol因为宏定义没法解析的问题

如：

```cpp
#ifdef CC_INTERP
// ...
#endif
```

这样整段代码甚至整个文件的代码都在宏定义if里的，缺少宏定义就gg了。



可以手动添加：

`Project Properties/Code Assistance/C Compiler/Preprocessor Definitions` （对于

c++的话是 `C++ Compiler`）里加入宏定义。

比如这里，在后面追加` CC_INTERP`就行了。



#### 因为宏定义只能走部分“分支”问题

比如openJDK里根据宏`CC_INTERP`控制选择使用`CppInterpreter`还是`TemplateInterpreter`，于是阅读代码时只能看到一部分（= = ）。

处理办法是：

1. 代码copy一份（新的）
2. 打开代码（项目）
3. 在新项目里设置宏，在老项目里不设置
4. 因为netBeans在一个窗口里管理多个项目，于是两个分支的symbol都可以查到了 = =



### 操作



#### 跳转



##### 符号跳转































