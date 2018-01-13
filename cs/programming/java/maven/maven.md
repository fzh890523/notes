# 概念



## dependency

### scope

* provided

  > 可以认为这个provided是目标容器已经provide这个artifact。换句话说，它只影响到编译，测试阶段。在编译测试阶段，我们需要这个artifact对应的jar包在classpath中，而在运行阶段，假定目标的容器（比如我们这里的liferay容器）已经提供了这个jar包，所以无需我们这个artifact对应的jar包了。

* optional

* compile

  > 默认scope，也就是说这个项目在编译，测试，运行阶段都需要这个artifact对应的jar包在classpath中。






# 问题



## jdk

### 默认jdk 1.5问题



处理：

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



## test



### how to skip tests

[from stackoverflow](https://stackoverflow.com/questions/2593588/maven-skip-building-test-classes):

According to the [documentation](http://maven.apache.org/plugins/maven-surefire-plugin/test-mojo.html#skip) on the Maven Surefire Plugin, `-Dmaven.test.skip` *should* skip both compilation and execution of the tests. By contrast, `-DskipTests` just skips the test *execution*: the tests are still compiled.



## java doc



### 命令行禁用maven-javadoc-plugin

```Shell
-Dmaven.javadoc.skip=true
```



## gpg



### 命令行禁用gpg



```shell
-Dgpg.skip
```





























