# 概念



## dependency

### scope

* provided

  > 可以认为这个provided是目标容器已经provide这个artifact。换句话说，它只影响到编译，测试阶段。在编译测试阶段，我们需要这个artifact对应的jar包在classpath中，而在运行阶段，假定目标的容器（比如我们这里的liferay容器）已经提供了这个jar包，所以无需我们这个artifact对应的jar包了。

* optional

* compile

  > 默认scope，也就是说这个项目在编译，测试，运行阶段都需要这个artifact对应的jar包在classpath中。


