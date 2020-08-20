# 使用



## Method



### 判断是否为static method

```java
int m = c.getModifiers();
if (Modifier.isPublic(m))
  System.out.println ("public");
if (Modifier.isAbstract(m))
 System.out.println ("abstract");
if (Modifier.isFinal(m))
   System.out.println ("final");
if(Modifier.isStatic(m){
   System.out.println("static");
}
```





### 判断是否为public method

见上。



### 判断是否为abstract method

见上。

### 判断是否为final method

见上。

## Field



### 判断是否为static field

```java
// Field field = ...;
java.lang.reflect.Modifier.isStatic(field.getModifiers())
```









# 实现细节



## inflation



Ref:

* [java-what-is-jitcs-reflection-inflation](https://stackoverflow.com/questions/10082523/java-what-is-jitcs-reflection-inflation)

  Did some source code digging and coding myself to figure this out, and here's what I've found out:

  Java's 'Method' class has a member variable 'methodAccessor' of type 'MethodAccessor' which is an interface with a method 'invoke', similar to Method's invoke. Methods's invoke delegates to methodAccessor's invoke.

  If inflation is enabled (noInflation is false) this accessor points to an implementation which uses JNI to run this Java method (I think using api's like GetObjectClass, GetMethodID and Call*Method). This is like duel dispatching, and execution with JNI is slow due to this and other reasons. ( [What makes JNI calls slow?](https://stackoverflow.com/questions/7699020/what-makes-jni-calls-slow) )

  After 15 executions of a method through reflection ('15' is default and can be changed) and with noInflation false, the JNI based accessor creates a class on the fly (the name is dynamically generated, e.g. say 'GeneratedMethodAccessor1') which also has the invoke method. Now, within this 'invoke' method, it casts the first 'obj' argument to its corresponding class, and then calls the target method on it. It then creates an instance of this class, and changes the methodAccessor settings such that every execution of the method henceforth is delegated to this instance instead of JNI accessor. This is called inflation.

  Because this instance is of a Java class which delegates to a Java object, the delegation henceforth is a normal Java delegation. It never goes to JNI and hence saves that overhead, plus JITC can perform other optimization on it due to which it becomes efficient.

  The downside is, if a lot of methods are inflated in this manner, their classes occupy permgen space and can possibly cause out of memory error.

  For details, see:

  [http://hg.openjdk.java.net/jdk8/jdk8/jdk/file/tip/src/share/classes/sun/reflect/ReflectionFactory.java](http://hg.openjdk.java.net/jdk8/jdk8/jdk/file/tip/src/share/classes/sun/reflect/ReflectionFactory.java)

  [http://java.sun.com/docs/books/jni/html/fldmeth.html](http://java.sun.com/docs/books/jni/html/fldmeth.html)

  [http://anshuiitk.blogspot.com/2010/11/excessive-full-garbage-collection.html](http://anshuiitk.blogspot.com/2010/11/excessive-full-garbage-collection.html)






# 踩坑



## final (String为例) field被优化导致通过反射修改该field无效果

## 

```java
	private static final String PROC_DIR_PATH = "/proc";

	static {
		init();
	}

	private static void init() {
		File procDir = new File(PROC_DIR_PATH);
```



在testcase里反射修改`PROC_DIR_PATH`然后重新init。 = = 没生效，原因如下：



```assembly
// access flags 0xA
  private static init()V
    TRYCATCHBLOCK L0 L1 L2 java/lang/ClassNotFoundException
    TRYCATCHBLOCK L0 L1 L2 java/lang/NoSuchMethodException
    TRYCATCHBLOCK L0 L1 L2 java/lang/reflect/InvocationTargetException
    TRYCATCHBLOCK L0 L1 L2 java/lang/IllegalAccessException
   L3
    LINENUMBER 40 L3
    NEW java/io/File
    DUP
    LDC "/proc"
    INVOKESPECIAL java/io/File.<init> (Ljava/lang/String;)V
    ASTORE 0
```

编译器对final field的优化，直接ref到字符串常量了。



解决办法： 去掉final。

```assembly
 private static init()V
    TRYCATCHBLOCK L0 L1 L2 java/lang/ClassNotFoundException
    TRYCATCHBLOCK L0 L1 L2 java/lang/NoSuchMethodException
    TRYCATCHBLOCK L0 L1 L2 java/lang/reflect/InvocationTargetException
    TRYCATCHBLOCK L0 L1 L2 java/lang/IllegalAccessException
   L3
    LINENUMBER 40 L3
    NEW java/io/File
    DUP
    GETSTATIC com/axx/edxx/saxx/module/common/utils/ThreadUtils.PROC_DIR_PATH : Ljava/lang/String;
    INVOKESPECIAL java/io/File.<init> (Ljava/lang/String;)V
    ASTORE 0
```







