

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









## Directories used by the IDE to store settings, caches, plugins and logs [FOLLOW](https://intellij-support.jetbrains.com/hc/en-us/articles/206544519-Directories-used-by-the-IDE-to-store-settings-caches-plugins-and-logs/subscription)

![Avatar](https://secure.gravatar.com/avatar/e4f80d61b08c213fcd012ce0c8587bfa?default=https%3A%2F%2Fassets.zendesk.com%2Fhc%2Fassets%2Fdefault_avatar.png&r=g)

JetBrains Team

Updated Sunday at 23:53

Location of the IDE files depends on the operating system, product and version.

Should you need to change the defaults, [refer to this document](https://intellij-support.jetbrains.com/hc/articles/207240985).

 

**Windows**

All the files are located under this directory by default:

- **Windows Vista, 7, 8, 10:**

```
<SYSTEM DRIVE>\Users\<USER ACCOUNT NAME>\.<PRODUCT><VERSION>
```

- **Windows XP:**

```
<SYSTEM DRIVE>\Documents and Settings\<USER ACCOUNT NAME>\.<PRODUCT><VERSION>
```

**Sample paths for different products**

- **IntelliJ IDEA 14 Ultimate Edition:**

```
c:\Users\John\.IntelliJIdea14\
```

- **IntelliJ IDEA 14 Community Edition:**

```
c:\Users\John\.IdeaIC14\
```

- **PhpStorm 2016.1:**

```
c:\Users\John\.PhpStorm2016.1\
```

- **WebStorm 11.0**:

```
c:\Users\John\.WebStorm11\
```

- **RubyMine 7.1:**

```
c:\Users\John\.RubyMine71\
```

- **PyCharm 4.5:**

```
c:\Users\John\.PyCharm45\
```

**Sub-directories**

Under this directory you'll find the following sub-directories

- **config**: configuration (**idea.config.path**)
- **config\plugins**: plugins (**idea.plugins.path**)
- **system**: caches, local history, etc (**idea.system.path**)
- **system\log**: logs and thread dumps (**idea.log.path**)

You can find more details about what is stored in each folder in the [Project and IDE Settings](https://www.jetbrains.com/help/idea/15.0/project-and-ide-settings.html) documentation section.

 

 

**Linux and other Unix systems**

Product directory starting with dot can be found in your user home directory, the pattern is:

```
 ~/.<PRODUCT><VERSION>
```

~ is an alias for the home directory, for example **/home/john**.

###  

**Mac OS X**

- **Configuration (idea.config.path):**

```
~/Library/Preferences/<PRODUCT><VERSION>
```

- **Caches (idea.system.path):**

```
~/Library/Caches/<PRODUCT><VERSION>
```

- **Plugins (idea.plugins.path):**

```
~/Library/Application Support/<PRODUCT><VERSION>
```

- **Logs (idea.log.path):**

```
~/Library/Logs/<PRODUCT><VERSION>
```

 

**PRODUCT** would be one of the following:

- **IntelliJIdea** (IntelliJ IDEA Ultimate Edition)
- **IdeaIC** (IntelliJ IDEA Community Edition)
- **RubyMine**
- **WebIde** (PhpStorm versions before 2016.1 and WebStorm before 7.0 use this common directory)
- **PhpStorm** (PhpStorm starting from 2016.1 version)
- **WebStorm** (WebStorm starting from 7.0 version)
- **PyCharm**
- **AppCode**
- **CLion**
- **DataGrip**
- **AndroidStudio**
- **Rider**





















