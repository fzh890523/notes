

# [         阅读go语言源代码1                    ](http://blog.csdn.net/hopingwhite/article/details/5710084)

2010-07-02 21:484386人阅读[评论](http://blog.csdn.net/HopingWhite/article/details/5710084#comments)(0)[收藏](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/568102e4-a731-4b2a-9451-e29fe7f6100d/)[举报](http://blog.csdn.net/HopingWhite/article/details/5710084#report)

[go](http://www.csdn.net/tag/go)[语言](http://www.csdn.net/tag/%e8%af%ad%e8%a8%80)[websocket](http://www.csdn.net/tag/websocket)[archive](http://www.csdn.net/tag/archive)[encoding](http://www.csdn.net/tag/encoding)[testing](http://www.csdn.net/tag/testing)

前面大概说过bison与yacc的语法，那么其实go也是用bison来实现的，下面来说说怎么阅读go语言源代码。要看代码首先得找到代码的入口，怎么找呢？你可能首先想到grep main是的，一般来说这是一个没有争议的办法，但是对于go语言来说，这个办法对，也不对。怎么说呢？我们用gdb来进行debug就会看到所有的go语言程序的入口居然都在src/lib9/main.c里的main函数。

[cpp]

view plain

copy

1. \#include <u.h>  
2. \#define NOPLAN9DEFINES  
3. \#include <libc.h>  
4.   
5. extern void p9main(int, char**);  
6.   
7. int  
8. main(int argc, char **argv)  
9. {  
10. ​    p9main(argc, argv);  
11. ​    exits("main");  
12. ​    return 99;   
13. }  

从go/include/libc.h中里我们可以看到这么一句

[cpp]

view plain

copy

1. \#ifndef NOPLAN9DEFINES  
2. \#define main    p9main  
3. \#endif  

也就是说，在go语言源代码里main函数都重命名了，所有的函数入口都变成了src/lib9/mina.c，也实际的入口各自的代码里，因此说入口是各自的main这句话，对也不对。

 

那么先大概说一下go语言的代码结构

 

[php]

view plain

copy

1. .  
2. |-- bin//我自己定义的安装目录  
3. |-- doc//相关文档，godoc就是利用这个目录  
4. |   |-- codelab  
5. |   |   `-- wiki  
6. |   |-- codewalk  
7. |   |-- devel  
8. |   |-- gopher  
9. |   |-- progs  
10. |   `-- talks  
11. |       `-- io2010  
12. |-- include//一些全局的定义文件  
13. |-- lib//一些lib9相关的全局库  
14. |   |-- codereview  
15. |   `-- godoc  
16. |-- misc//一些工具  
17. |   |-- arm  
18. |   |-- bash  
19. |   |-- bbedit  
20. |   |-- cgo  
21. |   |   |-- gmp  
22. |   |   |-- life  
23. |   |   `-- stdio  
24. |   |-- dashboard  
25. |   |   `-- godashboard  
26. |   |-- emacs  
27. |   |-- kate  
28. |   |-- nacl  
29. |   |-- vim  
30. |   |   |-- ftdetect  
31. |   |   `-- syntax  
32. |   `-- xcode  
33. |-- pkg//编译好的go库  
34. |   `-- linux_amd64  
35. |       |-- archive  
36. |       |-- compress  
37. |       |-- container  
38. |       |-- crypto  
39. |       |-- debug  
40. |       |-- encoding  
41. |       |-- exp  
42. |       |-- go  
43. |       |-- hash  
44. |       |-- http  
45. |       |-- image  
46. |       |-- io  
47. |       |-- os  
48. |       |-- rpc  
49. |       |-- runtime  
50. |       `-- testing  
51. |-- src  
52. |   |-- cmd//go语言相关的程序  
53. |   |   |-- 5a  
54. |   |   |-- 5c  
55. |   |   |-- 5g  
56. |   |   |-- 5l  
57. |   |   |-- 6a//amd64的汇编器  
58. |   |   |-- 6c//amd64的c编译器  
59. |   |   |-- 6g//amd64的编译器  
60. |   |   |-- 6l//amd64的链接器  
61. |   |   |-- 8a  
62. |   |   |-- 8c  
63. |   |   |-- 8g  
64. |   |   |-- 8l  
65. |   |   |-- cc  
66. |   |   |-- cgo  
67. |   |   |-- cov  
68. |   |   |-- ebnflint  
69. |   |   |-- gc  
70. |   |   |-- godefs  
71. |   |   |-- godoc  
72. |   |   |-- gofmt  
73. |   |   |-- goinstall  
74. |   |   |-- gopack  
75. |   |   |-- gotest  
76. |   |   |-- goyacc  
77. |   |   |-- hgpatch  
78. |   |   |-- ld  
79. |   |   |-- nm  
80. |   |   `-- prof  
81. |   |-- lib9//lib9相关的库  
82. |   |   |-- fmt  
83. |   |   `-- utf  
84. |   |-- libbio//全局库  
85. |   |-- libcgo//cgo的库  
86. |   |-- libmach//库  
87. |   `-- pkg//go库  
88. |       |-- archive  
89. |       |-- asn1  
90. |       |-- big  
91. |       |-- bufio  
92. |       |-- bytes  
93. |       |-- cmath  
94. |       |-- compress  
95. |       |-- container  
96. |       |-- crypto  
97. |       |-- debug  
98. |       |-- ebnf  
99. |       |-- encoding  
100. |       |-- exec  
101. |       |-- exp  
102. |       |-- expvar  
103. |       |-- flag  
104. |       |-- fmt  
105. |       |-- go  
106. |       |-- gob  
107. |       |-- hash  
108. |       |-- html  
109. |       |-- http  
110. |       |-- image  
111. |       |-- io  
112. |       |-- json  
113. |       |-- log  
114. |       |-- math  
115. |       |-- mime  
116. |       |-- net  
117. |       |-- netchan  
118. |       |-- nntp  
119. |       |-- once  
120. |       |-- os  
121. |       |-- patch  
122. |       |-- path  
123. |       |-- rand  
124. |       |-- reflect  
125. |       |-- regexp  
126. |       |-- rpc  
127. |       |-- runtime  
128. |       |-- scanner  
129. |       |-- sort  
130. |       |-- strconv  
131. |       |-- strings  
132. |       |-- sync  
133. |       |-- syscall  
134. |       |-- syslog  
135. |       |-- tabwriter  
136. |       |-- template  
137. |       |-- testing  
138. |       |-- time  
139. |       |-- unicode  
140. |       |-- unsafe  
141. |       |-- utf16  
142. |       |-- utf8  
143. |       |-- websocket  
144. |       `-- xml  
145. `-- test//测试  
146. ​    |-- bench  
147. ​    |-- bugs  
148. ​    |-- chan  
149. ​    |-- fixedbugs  
150. ​    |   |-- bug083.dir  
151. ​    |   |-- bug088.dir  
152. ​    |   |-- bug106.dir  
153. ​    |   |-- bug133.dir  
154. ​    |   |-- bug160.dir  
155. ​    |   |-- bug191.dir  
156. ​    |   |-- bug222.dir  
157. ​    |   |-- bug226.dir  
158. ​    |   |-- bug248.dir  
159. ​    |   `-- bug282.dir  
160. ​    |-- garbage  
161. ​    |-- interface  
162. ​    |-- ken  
163. ​    |-- nilptr  
164. ​    `-- syntax  

 

我们先来看一个go语言的hello world文件hello.go

[c-sharp]

view plain

copy

1. package main  
2.   
3. func main(){  
4. ​    println("Hello World");  
5. }  

那么这个文件要被编译链接成一个可执行文件，那么要执行如下命令：

[c-sharp]

view plain

copy

1. 6g hello.o  
2. 6l hello.6 -o hello  

 很显然最关键的就是6g和6l两个程序，那么我们就从6g开始

首先gdb -tui打开tui界面的gdb程序

然后输入file 6g载入6g程序，就可以看到

 /home/hoping/go/src/lib9/main.c，然后输入start后，step进入了/home/hoping/go/src/cmd/gc/lex.c的main函数

因此我们只需要看这个文件就大概可以看到程序的执行流程了。接下来暂时不需要gdb了，我们换vi和ctags。

在go目录执行ctags -R *来生成tag，然后用vi打开lex.c进入main函数，其实即使你看了这个函数也不会有太多收获。

正如我们前面说过的那样，6g是用bison的，那么显然我们需要找到bison文件和yylex函数即可。既然入口在gc文件夹，那么猜想y文件也在gc文件夹，于是进入ls *.y之后可以看到go.y。而go.y的输入全是由yylex函数来提供的。所以首先我们来看看yylex函数，在go文件夹下执行vim -t yylex，然后选择gc文件夹那个。

来源： <<http://blog.csdn.net/HopingWhite/article/details/5710084>>

 

# [          阅读go语言源代码2                    ](http://blog.csdn.net/hopingwhite/article/details/5713064)

​        分类：            [golang](http://blog.csdn.net/HopingWhite/article/category/705187)2010-07-05 10:481389人阅读[评论](http://blog.csdn.net/hopingwhite/article/details/5713064#comments)(0)[收藏](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/568102e4-a731-4b2a-9451-e29fe7f6100d/)[举报](http://blog.csdn.net/hopingwhite/article/details/5713064#report)

[语言](http://www.csdn.net/tag/%e8%af%ad%e8%a8%80)[go](http://www.csdn.net/tag/go)[token](http://www.csdn.net/tag/token)[buffer](http://www.csdn.net/tag/buffer)[struct](http://www.csdn.net/tag/struct)[newline](http://www.csdn.net/tag/newline)

上回说到6g程序应该如何看，这回书接上文，从yylex这个函数看起。vim -t yylex仍然选gc那个。

[cpp]

view plain

copy

1. 1107 int32  
2. 1108 yylex(void)  
3. 1109 {  
4. 1110     int lx;  
5. 1111   
6. 1112     lx = _yylex();//主要处理函数  
7. 1113   
8. 1114     if(curio.nlsemi && lx == EOF) {  
9. 1115         // if the nlsemi bit is set, we'd be willing to  
10. 1116         // insert a ; if we saw a /n, but we didn't.  
11. 1117         // that means the final /n is missing.  
12. 1118         // complain here, because we can give a  
13. 1119         // good message.  the syntax error we'd get  
14. 1120         // otherwise is inscrutable.  
15. 1121         yyerror("missing newline at end of file");  
16. 1122         lx = ';';  
17. 1123     }  
18. 1124   
19. 1125     switch(lx) {  
20. 1126     case LNAME:  
21. 1127     case LLITERAL:  
22. 1128     case LBREAK:  
23. 1129     case LCONTINUE:  
24. 1130     case LFALL:  
25. 1131     case LRETURN:  
26. 1132     case LINC:  
27. 1133     case LDEC:  
28. 1134     case ')':  
29. 1135     case '}':  
30. 1136     case ']':  
31. 1137         curio.nlsemi = 1;  
32. 1138         break;  
33. 1139     default:  
34. 1140         curio.nlsemi = 0;  
35. 1141         break;  
36. 1142     }  
37. 1143     return lx;  
38. 1144 }  

其实这个函数做的事情比较简单，就是对于需要默认加;的代码后面加了分号而已。主要的处理逻辑还是在_yylex里完成了。

实际上这里需要关注的是两个全局变量curio和pushedio，这两个都是Io结构体类型，定义如下：

[cpp]

view plain

copy

1. 566 struct  Io  
2.  567 {  
3.  568     char*   infile;//本结构体所代表的内部文件  
4.  569     Biobuf* bin;//代表实际的文件以及buffer  
5.  570     int32   ilineno;//内部行号  
6.  571     int nlsemi;//是否为文件结尾，需要加分号  
7.  572     int peekc; //当前未读出的字符  
8.  573     int peekc1; // second peekc for ...  
9.  574     char*   cp; // used for content when bin==nil，主要用于内部package，比如builtin  
10.  575     int importsafe;  
11.  576 };  

而pushedio是相当于一个临时保存当前处理文件的对象，因为一些内部的参数定义也是以package形式存在的，因此在parse一个package之前，6g会自己先加载一些临时的package，目前看到就是runtime.builtin，这个时候curio会被压到pushedio里暂存，然后再恢复。

再来看一下Biobuf的结构：

[cpp]

view plain

copy

1. 56 struct  Biobuf  
2.  57 {  
3.  58     int icount;     /* neg num of bytes at eob */  
4.  59     int ocount;     /* num of bytes at bob */  
5.  60     int rdline;     /* num of bytes after rdline */  
6.  61     int runesize;   /* num of bytes of last getrune */  
7.  62     int state;      /* r/w/inactive */  
8.  63     int fid;        /* open file */  
9.  64     int flag;       /* magic if malloc'ed */  
10.  65     vlong   offset;     /* offset of buffer in file */  
11.  66     int bsize;      /* size of buffer */  
12.  67     unsigned char*  bbuf;       /* pointer to beginning of buffer */  
13.  68     unsigned char*  ebuf;       /* pointer to end of buffer */  
14.  69     unsigned char*  gbuf;       /* pointer to good data in buf */  
15.  70     unsigned char   b[Bungetsize+Bsize];  
16.  71 };  

这个结构体主要是一个缓存，因此我们知道yylex里是一个字符一个字符进行处理的，如果每次都从文件里实际读取效率显然是极低的，因此这里是先读出一部分字符，然后缓存到buffer里，这样回退前进都会比较容易。具体的逻辑可以参考_yylex。这里需要额外说明的数据结构有Sym，这个结构体代表了一些关键字的语法，比如if, for等等。事实上从go.y里的关于%union的定义我们可以看出来：

[cpp]

view plain

copy

1. 24 %union  {  
2.   25     Node*       node;  
3.   26     NodeList*       list;  
4.   27     Type*       type;  
5.   28     Sym*        sym;  
6.   29     struct  Val val;  
7.   30     int     lint;  
8.   31 }  
9.   32   
10.   33 // |sed 's/.*   //' |9 fmt -l1 |sort |9 fmt -l50 | sed 's/^/%xxx        /'  
11.   34   
12.   35 %token  <val>   LLITERAL  
13.   36 %token  <lint>  LASOP  
14.   37 %token  <sym>   LBREAK LCASE LCHAN LCOLAS LCONST LCONTINUE LDDD  
15.   38 %token  <sym>   LDEFAULT LDEFER LELSE LFALL LFOR LFUNC LGO LGOTO  
16.   39 %token  <sym>   LIF LIMPORT LINTERFACE LMAP LNAME  
17.   40 %token  <sym>   LPACKAGE LRANGE LRETURN LSELECT LSTRUCT LSWITCH  
18.   41 %token  <sym>   LTYPE LVAR  
19.   42   
20.   43 %token      LANDAND LANDNOT LBODY LCOMM LDEC LEQ LGE LGT  
21.   44 %token      LIGNORE LINC LLE LLSH LLT LNE LOROR LRSH  

从这里我们可以看到一些词法的定义。接下来我们主要关注一下Sym和Val这个结构体

[cpp]

view plain

copy

1. 286 struct  Sym  
2.  287 {  
3.  288     ushort  lexical;  
4.  289     uchar   flags;  
5.  290     uchar   sym;        // huffman encoding in object file  
6.  291     Sym*    link;  
7.  292   
8.  293     // saved and restored by dcopy  
9.  294     Pkg*    pkg;  
10.  295     char*   name;       // variable name  
11.  296     Node*   def;        // definition: ONAME OTYPE OPACK or OLITERAL  
12.  297     int32   block;      // blocknumber to catch redeclaration  
13.  298     int32   lastlineno; // last declaration for diagnostic  
14.  299 };  
15. 119 struct  Val  
16.  120 {  
17.  121     short   ctype;  
18.  122     union  
19.  123     {  
20.  124         short   reg;        // OREGISTER  
21.  125         short   bval;       // bool value CTBOOL  
22.  126         Mpint*  xval;       // int CTINT  
23.  127         Mpflt*  fval;       // float CTFLT  
24.  128         Mpcplx* cval;       // float CTCPLX  
25.  129         Strlit* sval;       // string CTSTR  
26.  130     } u;  
27.  131 };  

 

版权声明：本文为博主原创文章，未经博主允许不得转载。

来源： <<http://blog.csdn.net/hopingwhite/article/details/5713064>>

