

# 使用 Typora 画图（类图、流程图、时序图）

[![阿远](https://pic3.zhimg.com/v2-d2a92d6454542982244c88363de9d7dc_xs.jpg)](https://www.zhihu.com/people/zoumingyuan)

[阿远](https://www.zhihu.com/people/zoumingyuan)[](https://www.zhihu.com/question/48510028)



华中科技大学 软件工程硕士在读

今天继续分享一下使用 Typora 在画图方面的小技巧。是的，你没看错，Typora 也能”画“图。

说是画图，但并不像其他专业的绘图软件那样，通过拖拖拽拽一些元素，画出流程图、时序图等。Typora 的制图功能是由 Mermaid 强力驱动，如果你使用过 PlantUML，那么 Mermaid 也非常容易上手。

### **Mermaid**

**Mermaid[1]** 是一种简单的类似 Markdown  的脚本语言，通过 JavaScript 编程语言，将文本转换为图片。因此，真正实现画图功能的并不是 Typora 本身，它只是内置了对 Mermaid 的支持。

![img](https://picb.zhimg.com/80/v2-4e44a08fa37bdb6b03df9fcec8480ed9_1440w.jpg)

Mermaid 支持绘制非常多种类的图，常见的有时序图、流程图、类图、甘特图等等。下面分享一下如何绘制这些图，语法非常容易掌握。

先在 Typora 中，输入 **```mermaid** 然后敲击回车，即可初始化一张空白图。

### **流程图**

语法解释：`graph` 关键字就是声明一张流程图，`TD` 表示的是方向，这里的含义是 Top-Down 由上至下。

```mermaid
graph TD;
    A-->B;
    A-->C;
    B-->D;
```



```mermaid
graph TD
  A[Christmas] -->|Get money| B(Go shopping)
  B --> C{Let me think}
  C -->|One| D[Laptop]
  C -->|Two| E[iPhone]
  C -->|Three| F[fa:fa-car Car]
```





### 脑图（基于流程图，by yonka）

ref：

* ![](http://img.go0s.cn/summer/1586970265787.png)

```mermaid
graph LR
style e fill:orange,color:white

b(Java)
c(Spring)
f(Go)

%%java
a(Backend) --> b --> c
b --> d(Hibernate)
b --> e(Dubbo)

%%go
a --> f
f --> g(Beego)

```





### **时序图**

语法解释：`->>` 代表实线箭头，`-->>` 则代表虚线。

```mermaid
sequenceDiagram
    Alice->>John: Hello John, how are you?
    John-->>Alice: Great!
```

```mermaid
sequenceDiagram
    Alice->>+John: Hello John, how are you?
    Alice->>+John: John, can you hear me?
    John-->>-Alice: Hi Alice, I can hear you!
    John-->>-Alice: I feel great!
```



### **状态图**

语法解释：`[*]` 表示开始或者结束，如果在箭头右边则表示结束。

```mermaid
stateDiagram
    [*] --> s1
    s1 --> [*]
```

```mermaid
stateDiagram
    [*] --> Still
    Still --> [*]
    Still --> Moving
    Moving --> Still
    Moving --> Crash
    Crash --> [*]
            
```



### **类图**

语法解释：`<|--` 表示继承，`+` 表示 `public`，`-` 表示 `private`，学过 Java 的应该都知道。

```mermaid
classDiagram
      Animal <|-- Duck
      Animal <|-- Fish
      Animal <|-- Zebra
      Animal : +int age
      Animal : +String gender
      Animal: +isMammal()
      Animal: +mate()
      class Duck{
          +String beakColor
          +swim()
          +quack()
      }
      class Fish{
          -int sizeInFeet
          -canEat()
      }
      class Zebra{
          +bool is_wild
          +run()
      }
```



### **甘特图**

甘特图一般用来表示项目的计划排期，目前在工作中经常会用到。

语法也非常简单，从上到下依次是图片标题、日期格式、项目、项目细分的任务。

```mermaid
gantt
    title 工作计划
    dateFormat  YYYY-MM-DD
    section Section
    A task           :a1, 2020-01-01, 30d
    Another task     :after a1  , 20d
    section Another
    Task in sec      :2020-01-12  , 12d
    another task      : 24d
```



### **饼图**

饼图使用 `pie` 表示，标题下面分别是区域名称及其百分比。

```mermaid
pie
    title Key elements in Product X
    "Calcium" : 42.96
    "Potassium" : 50.05
    "Magnesium" : 10.01
    "Iron" :  5
```



### ER图

```mermaid
erDiagram
          CUSTOMER }|..|{ DELIVERY-ADDRESS : has
          CUSTOMER ||--o{ ORDER : places
          CUSTOMER ||--o{ INVOICE : "liable for"
          DELIVERY-ADDRESS ||--o{ ORDER : receives
          INVOICE ||--|{ ORDER : covers
          ORDER ||--|{ ORDER-ITEM : includes
          PRODUCT-CATEGORY ||--|{ PRODUCT : contains
          PRODUCT ||--o{ ORDER-ITEM : "ordered in"
            
```







### **导出**

绘制好的图片可以选择菜单/文件/导出，导出为图片或者网页格式。在网页中图片是以 SVG 格式渲染的，你可以复制 SVG 内容，导入到 SVG 的图片编辑器中进一步操作。

![img](https://picb.zhimg.com/80/v2-907aef8d603e125c85d2a14ed96a9e79_1440w.jpg)

Mermaid 官方有一个在线的工具，可以导出 SVG 和 PNG。

[Mermaid live editor](https://link.zhihu.com/?target=https%3A//mermaid-js.github.io/mermaid-live-editor)

[mermaid-js.github.io](https://link.zhihu.com/?target=https%3A//mermaid-js.github.io/mermaid-live-editor)

**小结**

可以看到，Mermaid 使用非常简洁优雅的语法，让使用者可以快速地画出常用的图形，并且图片的非常美观，配色和谐，富有现代感。这个 Typora 画图的功能，你学会了吗？

我还写过关于 Typora 的一些使用小技巧，欢迎阅读 

[https://zhuanlan.zhihu.com/p/163608242](https://zhuanlan.zhihu.com/p/163608242)[zhuanlan.zhihu.com![图标](https://pic2.zhimg.com/v2-dd83e053fd151d52053e2a73dcad956d_ipico.jpg)](https://zhuanlan.zhihu.com/p/163608242)

### **参考资料**

[1] 

Mermaid: *[https://mermaid-js.github.io/mermaid/#/](https://link.zhihu.com/?target=https%3A//mermaid-js.github.io/mermaid/%23/)*



