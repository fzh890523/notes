







# misc



```
关键词：
-XX:+DontCompileHugeMethods
-XX:HugeMethodLimit=8000

用以上关键词就可以搜到HotSpot VM的一个怪脾气了 ^_^

tl;dr：HotSpot VM默认不会JIT编译字节码大小超过8000字节的方法。要通过 -XX:-DontCompileHugeMethods 来允许大方法被JIT编译；8000这个阈值在产品版HotSpot里无法调整。

作者：RednaxelaFX
链接：https://www.zhihu.com/question/263322849/answer/268228465
来源：知乎
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
```





