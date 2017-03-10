
#  OpenJDK类加载实现浅析

* [OpenJDK类加载实现浅析#1：整体流程](http://blog.csdn.net/kisimple/article/details/44727147) or [local](resource/OpenJDK类加载实现浅析#1：整体流程 - KISimple的专栏 - 博客频道 - CSDN.NET.html)
* [OpenJDK类加载实现浅析#2：安全检查](http://blog.csdn.net/kisimple/article/details/44859187) or [local](resource/OpenJDK类加载实现浅析#2：安全检查 - KISimple的专栏 - 博客频道 - CSDN.NET.html)
* [OpenJDK类加载实现浅析#3：并行加载](http://blog.csdn.net/kisimple/article/details/44890463) or [local](resource/OpenJDK类加载实现浅析#3：并行加载 - KISimple的专栏 - 博客频道 - CSDN.NET.html)

## 点 - 对ref内容的部分摘录

### 几个`xxxClass`方法

* `loadClass`用于实现类加载的代理机制；
* `findClass`用于找到类的二进制表示；
* `defineClass`用于将类的二进制表示转化成Class对象，这一步由虚拟机来完成；

### jar包的处理

* jar包的处理有一个特别的地方，就是jar包Manifest的Class-Path属性如果指定了其它jar包，需要将它们也加载 - 采用Stack实现深度优先遍历

### classpath

* 按classpath字符串的字面顺序来构建数组的，所以结论就是findClass也是按照classpath字符串的字面顺序来进行搜索的

* 通配符的展开是由虚拟机的launcher来完成的，这样在虚拟机运行时才能看到展开后的classpath

  > 通配符的展开并不是确定的，在不同平台可能会不同，即使在同一机器，不同时刻可能也不同，我们不应该依赖通配符展开后的顺序，如果需要依赖顺序就应该直接写明而不要使用通配符
  > 在Linux平台上将会使用readdir这个系统调用（参考man）来读取目录下的文件，那么通配符的展开就要取决于这个系统调用读取文件的顺序了
### 安全检查

![](resource/OpenJDK类加载实现浅析#2：安全检查 - KISimple的专栏 - 博客频道 - CSDN.NET_files/4.jpg)

SecurityManager -> AccessController -> Policy



总结一下就是：

1. 通过`System.getSecurityManager()`拿到`SecurityManager`；
2. `SecurityManager`直接交给`AccessController`处理；
3. `AccessController`通过调用`getStackAccessControlContext`取得`AccessControlContext`，并交给`AccessControlContext`处理；
4. `AccessControlContext`交给它所持有的一个`ProtectionDomain`数组处理；
5. `ProtectionDomain`交给`PermissionCollection`处理，有两种方式拿到`PermissionCollection`，一种是使用自身持有的`PermissionCollection`（构造函数传入），另一种是使用`Policy`来获得。使用哪种方式由`staticPermissions`决定。

####  SecurityManager

`System.getSecurityManager()`返回的...是在虚拟机启动的时候，由launcher来设置的，而launcher会根据`"java.security.manager"`（System.getProperty）来…。

#### Policy

* 读取{java.home}/lib/security/java.security配置文件policy.provider=sun.security.provider.PolicyFile

* "sun.security.provider.PolicyFile"

#### Protecion Domain

> A domain conceptually encloses a set of classes whose instances are granted the same set of permissions. Protection domains are determined by the policy currently in effect.

 有点像权限组的概念，多对多：

* domain可以有多个class
* domain可以有多个权限

![](resource/OpenJDK类加载实现浅析#2：安全检查 - KISimple的专栏 - 博客频道 - CSDN.NET_files/security-spec.doc.anc.gif)

示例：

```
ProtectionDomain  null
 null
 <no principals>
 java.security.Permissions@73c94b51 (
 ("java.security.AllPermission" "<all permissions>" "<all actions>")
)


ProtectionDomain  (file:/C:/Java/jdk1.7.0_51/jre/lib/ext/groovy-2.4.0.jar <no signer certificates>)
 sun.misc.Launcher$ExtClassLoader@d325aef
 <no principals>
 java.security.Permissions@3aeb3f66 (
 ("java.io.FilePermission" "\C:\Java\jdk1.7.0_51\jre\lib\ext\groovy-2.4.0.jar" "read")
)


ProtectionDomain  (file:/E:/Projects/just4fun/target/classes/ <no signer certificates>)
 sun.misc.Launcher$AppClassLoader@35f784d7
 <no principals>
 java.security.Permissions@2a8f5fc2 (
 ("java.lang.RuntimePermission" "exitVM")
 ("java.io.FilePermission" "\E:\Projects\just4fun\target\classes\-" "read")
)
```

#### 安全检查的意义

##### default访问权限被绕过的问题

**同包**这个是非强制的，也即其他代码也可以“强行”和特定代码“同包”。

应对方式：

可以使用`Manifest`的`Sealed`属性了，在打jar包的时候，我们可以指定`Manifest`并添加`Sealed`属性，就像下面这样，

```
Name: me/kisimple/just4fun/
Sealed: true12
```

这样会在`getAndVerifyPackage`时对URL进行检查，以确保“确实同包”。

### 并发加载

最后总结一下：

类加载库层面，

- 不支持并发：同步classloader，`synchronized`方法；
- 支持并发：同步classname对象，`synchronized`代码块；

虚拟机层面，

- 不支持并发：对classloader加锁，`ObjectLocker ol(lockObject, THREAD, DoObjectLock);`
- 支持并发：对systemdictionary加锁，`MutexLocker mu(SystemDictionary_lock, THREAD);`

