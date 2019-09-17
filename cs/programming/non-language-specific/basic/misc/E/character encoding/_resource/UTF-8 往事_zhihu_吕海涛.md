

https://zhuanlan.zhihu.com/p/70264909



很久以前，我偶然发现了一个[链接](https://link.zhihu.com/?target=https%3A//www.cl.cam.ac.uk/~mgk25/ucs/UTF-8-history.txt)，内容是 [Rob Pike](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Rob_Pike) 在 2003 发的邮件，讨论的是 UTF-8 编码诞生之初的故事。Rob 和 [Ken](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Ken_Thompson) 是 UTF-8 的共同发明人。读罢不仅深化了对 UTF-8 编码的理解，更为大师们的智慧所折服。现在整理成文分享给大家。

故事是从 [Plan 9](https://link.zhihu.com/?target=https%3A//9p.io/plan9/) 操作系统开始的。为了让 Plan 9 支持 Unicode (ISO 10646)，Rob 和 Ken 选用了 UCS 编码（标准制定的 16 位编码方案，后来扩展成了 UTF-16）。虽然使用了 UCS 编码，但对 USC 编码并不满意，原文是 **but we hated it**。当整个工作几近完成的时候（大约在 1992 年的九月份），[X/Open](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/X/Open)  组织有人给他们打电话，让 Rob 和 Ken 投票支持所谓的 FSS/UTF 编码方案。Rob 和 Ken  提出要根据自己的经验设计一个更好的编码方案。X/Open 的人接受了这个提议，但要求尽快提交方案。吃晚饭的时候，Ken  在餐桌上就完成了编码规则的设计。回到实验室，他们给 X/Open 发邮件说明了新编码的设计大纲。X/Open 的人则回复说 Rob 和 Ken  的方案比他们自己方案更好，并且询问什么时候能实现这一编码。当天是周三，Rob 以为 X/Open  会在下周一开始投票，所以就保证说下周一给出完整实现。

Rob 和 Ken **当晚就开始编码**，Ken 负责实现 UTF-8  编解码逻辑，Rob 负责改造现有的 c 库和图形库。到了第二天，编码工作就已完成，俩个人开始使用新编码对 Plan 9  上的文本文件进行转码。到周五，Plan 9 系统完全基于新编码运行了。Rob 和 Ken 称这种新的编码为 UTF-8。然后，UTF-8  改变了历史。

根据 [Google 2012 年的统计](https://link.zhihu.com/?target=https%3A//googleblog.blogspot.com/2012/02/unicode-over-60-percent-of-web.html)，当年 web 领域 UTF-8 编码的占比就已经超过了 60%。

可是，Rob 和 Ken 为什么没有采用 X/Open 的编码方案呢？Rob 指出 X/Open 的编码方案和当时的好多编码一样，没有**自同步**这一特性，所以他们提出了 UTF-8 方案。我们会在下文解释这个**自同步**特性。

为了把事情说清楚，Rob 联系 Russ Cox 查询当年的来往邮件。然后 Russ 真的找到了相关邮件，甚至还给出了 1992 年的邮件发送记录！邮件记录了 UTF-8 最早的设计方案。

FSS/UTF 编码全称是 **File System Safe** Universal Character Set Transformation Format。为什么要考虑这个文件系统安全呢？因为在 unicode 出现之前，计算机普遍使用 ASCII 编码。UNIX 的文件系统使用 `/`，也就是 `0x2f`，作为路径分隔标志。另一方面，c 语言使用 `0x00` 表示字符串的结尾。而 ISO/IEC 10646 (Unicode) 制定 UCS-2 编码使用双字节编码，最多支持表示 65535 个字符（code point）。UCS-2 编码一定会出现某个字符编码包含 `0x2f` 或 `0x00` 情况。例如，「⼀」的 UCS-2 编码是 `0x2f00`，同时包含了 `0x2f` 和 `0x00`。UNIX 系统和 c 语言基本没法处理使用 UCS-2 编码的数据。如果非要使用 UCS-2 编码，那就只有一个办法——将老数据使用 UCS-2 转码。这显然不现实。

所以 Rob 和 Ken 给新编码制定了几条指导原则：

1. 兼容历史文件系统，文件名不能包含 `0x2f` 和 `0x00`
2. 兼容现有程序，非 ASCII 字符编码不能部分包含 ASCII 编码
3. 与 UCS 编码转换要简单
4. 首字节需要指明后续字节长度
5. 编码格式不要浪费空间
6. 自同步

前两条讲得是一个事情。ASCII 编码范围是 `0x00-0x7f`，新编码方案中非 ASCII 字符的编码序列不能包含 `0x00-0x7f` 范围的内容，不然现有的系统和程序会把这部分内容当成 ASCII 处理而导致混乱。

第六条说的是错误恢复。简单来说，程序从文件的任意部分开始读取，可能只读到一个字符的部分编码字节，从而无法实别这一字符。但没关系，编码方案需要支持程序快速跳过有问题的字节，然后正常解码。

这六条原则一言一蔽之，**多快好省**。

最终的编码方案使用**变长字节编码**，不同范围的字符使用不同长度的字节编码，最多使用 6 个字节，可表示范围为 `[0,0x7fffffff]`。

其中，ASCII 字符 `[0x00-0x7f]` 的编码方式与现有 ASCII 编码保持一致，已有的 ASCII 编码无需做任何改动。其他字符使用多字节编码。

为了实现第一条和第二条原则，多字节编码的每个字节的最高位永远是 `1`，而 ASCII 字符编码的最高位是 `0`，所以从根本上杜绝了编码冲突。

为了实现第四条原则，多字节编码以 `11{1,5}0` 开头。`1` 和 `0` 之间 `1` 的数量表示后续字节的长度（这里借用了正则的表示方式）。

为了实现第五条原则，编码规定，如果一个字符的编码可以有多种表示方式，则选用最短的表示。

为了实现第六条原则，编码序列的后续字节都是以 `10` 开头的。如果程序读到了受损的文件，只能有三种情况：1、当前字节最高位是 `0`，则是合法 ASCII 字符；2、当前最高两位是 `11`，则是合法的多字节编码；3、当前字节最高两位是 `10`，则是其他字符编码的一部分，跳过，直到读到最高位为 `0` 或最高两位为 `11` 为止。

举个例子，汉字「吕」的 Unicde 编码是 `U+5415`，对应二进制为 `0b0101010000010101`，需要 15 bit，所以使用三字节编码，对应二进制拆成（从低位到高位）三部分，分别是 `0b0101`, `0b010000`, `0b010101`，再拼上编码前缀得到 `0b11100101`, `0b10010000`, `0b10010101`，对应十六进制为 `0xe5`, `0x90`, `0x95`。所以汉字「吕」的 UTF-8 编码是 `0xe59095`。

完整的编码规则如下表：

```text
Bits  Hex Min  Hex Max  Byte Sequence in Binary
1    7  00000000 0000007f 0vvvvvvv
2   11  00000080 000007FF 110vvvvv 10vvvvvv
3   16  00000800 0000FFFF 1110vvvv 10vvvvvv 10vvvvvv
4   21  00010000 001FFFFF 11110vvv 10vvvvvv 10vvvvvv 10vvvvvv
5   26  00200000 03FFFFFF 111110vv 10vvvvvv 10vvvvvv 10vvvvvv 10vvvvvv
6   31  04000000 7FFFFFFF 1111110v 10vvvvvv 10vvvvvv 10vvvvvv 10vvvvvv 10vvvvvv
```

最终 ISO 标准化的 FSS/UTF 编码方案可以从[这里](https://link.zhihu.com/?target=https%3A//www.unicode.org/versions/Unicode1.1.0/appF.pdf)获取。

IETF 也制定了 [RFC3629](https://link.zhihu.com/?target=https%3A//tools.ietf.org/html/rfc3629) 对 UTF-8 做了进一步标准化。RFC3629 制定的版本将 UTF-8 的表示范围限制在了 `[0-10FFFF]`，所以只需要 4 个字节就够了。标准原文

>  Restricted the range of characters to 0000-10FFFF (the UTF-16 accessible range)

如果大家对这个问题感兴趣，可以参考 [Stijn de Witt 的这篇文章](https://link.zhihu.com/?target=https%3A//stijndewitt.com/2014/08/09/max-bytes-in-a-utf-8-char/)。此处就不展开讨论了。对于 Ken 在一夜之间实现的编解码算法，我另写了一篇文章，也不展开讨论了。

吕海涛：史上首个 UTF-8 编解码实现赏析

zhuanlan.zhihu.com![图标](https://zhstatic.zhihu.com/assets/zhihu/editor/zhihu-card-default.svg)

好了，UTF-8 的故事讲完了，但使用 UTF-8 编码记录的故事永远都讲不完。

凡人随波逐流，大师改变世界，向大师致敬。





