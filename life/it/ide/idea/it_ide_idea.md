

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























