

# jdk版本

## 新module jdk版本1.5问题

### 背景

* proj jdk版本17
* 新module jdk版本1.5（source -> lauguage level; target -> bytecode version）



### 原因

是： maven默认是1.5，而对于maven项目，idea会尊重maven版本，so... 



### 处理

在maven配置（如 `~/.m2/settings.xml`）里加入类似：

```xml
<profile>
  <id>jdk-1.8</id>
  <activation>
      <activeByDefault>true</activeByDefault>
      <jdk>1.8</jdk>
  </activation>
  <properties>
    <maven.compiler.source>1.8</maven.compiler.source>
    <maven.compiler.target>1.8</maven.compiler.target>
    <maven.compiler.compilerVersion>1.8</maven.compiler.compilerVersion>
  </properties>
</profile>
```





# 日常



## 编译



### 反编译



#### 反编译代码行号对不上问题



I found that on OSX you can change this setting in the following file:

> ~/Library/Preferences/IntelliJIdea2016.1/options/options.xml

Add the following properties:

```
    <property name="decompiler.use.line.mapping" value="true" />
    <property name="decompiler.dump.original.lines" value="true" />
```



or through Cmd + Shift + A -> Registry in Intellij



效果如下：

```java
    private void run() {
        int b = this.a;// 17
        ++b;// 18
        T.T1 t = new T.T1();// 20
        ++t.a;// 21
        this.a = b;// 23
        ++this.a;// 24
        sa = b;// 26
        sa = this.a;// 27
    }// 28
```

并没有像想象的做align，只是标注了一下。 = =



## project



### recent projects



显示的是`project name`，而该名字默认是目录名，表意性不好。

如果只是修改`module name`的话是无法让`recent projects`中的显式也同步生效的，要修改`project name`。































