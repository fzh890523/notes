



# detail



## load agent的classloader： Sys/App classLoader

从源码里可以看到： 

使用的是 System/App classLoader。



## how to unload an agent



思路：

* 像sandbox一样，agent类 和 sandbox core解耦，这时候虽然没有unload agent（class），但可以unload sandbox

* 真正unload agent（class），实际上 -> unload appCL 加载的一个类

  而 unload class 的前提是 其CL被GC（等等），而appCL显然是不会被GC的，所以... 没辙 = =





# source



openJDK中：

```java
/**
 * The Java side of the JPLIS implementation. Works in concert with a native JVMTI agent
 * to implement the JPLIS API set. Provides both the Java API implementation of
 * the Instrumentation interface and utility Java routines to support the native code.
 * Keeps a pointer to the native data structure in a scalar field to allow native
 * processing behind native methods.
 */
public class InstrumentationImpl implements Instrumentation {
  // ...
}
```



## loadAgent



```java
// InstrumentationImpl

    // WARNING: the native code knows the name & signature of this method
    private void
    loadClassAndCallPremain(    String  classname,
                                String  optionsString)
            throws Throwable {

        loadClassAndStartAgent( classname, "premain", optionsString );
    }

    // WARNING: the native code knows the name & signature of this method
    private void
    loadClassAndCallAgentmain(  String  classname,
                                String  optionsString)
            throws Throwable {

        loadClassAndStartAgent( classname, "agentmain", optionsString );
    }

    // Attempt to load and start an agent
    private void
    loadClassAndStartAgent( String  classname,
                            String  methodname,
                            String  optionsString)
            throws Throwable {

        ClassLoader mainAppLoader   = ClassLoader.getSystemClassLoader();
        Class<?>    javaAgentClass  = mainAppLoader.loadClass(classname);
      // ...
    }
```









