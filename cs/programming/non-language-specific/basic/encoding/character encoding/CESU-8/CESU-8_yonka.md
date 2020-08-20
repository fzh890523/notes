

utf-8的变种，融合utf-8和utf-16。



1. BMP： utf-8

2. 辅助平面： 类似utf-16，拆成代理对，只是每个代理码元以utf-8编码，因此需要 6 bytes

    `11101101 1010yyyy 10xxxxxx 11101101 1011xxxx 10xxxxxx` (yyyy represents the top five bits of the character minus one)

   但前提是这部分utf-8跟BMP内utf-8不重叠，才能区分出彼此吧。。。

> 非标，不适合作为公开数据交互编码，内部使用为主



https://en.wikipedia.org/wiki/CESU-8



