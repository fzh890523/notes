

ref： [李嘉鹏-JVM源码分析之javaagent原理完全解读](http://www.infoq.com/cn/articles/javaagent-illustrated/)



# JVMTI agent

JVMTIAgent其实就是一个动态库，利用JVMTI暴露出来的一些接口来干一些我们想做、但是正常情况下又做不到的事情，不过为了和普通的动态库进行区分，它一般会实现如下的一个或者多个函数：

```c++
JNIEXPORT jint JNICALL
Agent_OnLoad(JavaVM *vm, char *options, void *reserved);

JNIEXPORT jint JNICALL
Agent_OnAttach(JavaVM* vm, char* options, void* reserved);

JNIEXPORT void JNICALL
Agent_OnUnload(JavaVM *vm); 
```

- Agent_OnLoad函数，如果agent是在启动时加载的，也就是在vm参数里通过-agentlib来指定的，那在启动过程中就会去执行这个agent里的Agent_OnLoad函数。
- Agent_OnAttach函数，如果agent不是在启动时加载的，而是我们先attach到目标进程上，然后给对应的目标进程发送load命令来加载，则在加载过程中会调用Agent_OnAttach函数。
- Agent_OnUnload函数，在agent卸载时调用，不过貌似基本上很少实现它。

## 示例



### IDE调试java代码： JVMTI agent - jdwp

比如我们经常使用Eclipse等工具调试Java代码，其实就是利用JRE自带的jdwp agent实现的，只是Eclipse等工具在没让你察觉的情况下将相关参数(类似`-agentlib:jdwp=transport=dt_socket,suspend=y,address=localhost:61349`)自动加到程序启动参数列表里了，其中agentlib参数就用来跟要加载的agent的名字，比如这里的jdwp(不过这不是动态库的名字，JVM会做一些名称上的扩展，比如在Linux下会去找libjdwp.so的动态库进行加载，也就是在名字的基础上加前缀lib，再加后缀.so)，接下来会跟一堆相关的参数，将这些参数传给Agent_OnLoad或者Agent_OnAttach函数里对应的options。

- agent名字：jdwp

  `-agentlib:${agent_name}`

- 动态库： libjdwp.so

  `lib${agent_name}.so`

  > 命名风格应该是per platform的

### javaagent： JVMTI agent 之 JPLISAgent



JPLISAgent(Java Programming Language Instrumentation Services Agent)



instrument agent实现了Agent_OnLoad和Agent_OnAttach两方法，也就是说在使用时，agent既可以在启动时加载，也可以在运行时动态加载。其中启动时加载还可以通过类似-javaagent:myagent.jar的方式来间接加载instrument agent，运行时动态加载依赖的是JVM的attach机制（[JVM Attach机制实现](http://lovestblog.cn/blog/2014/06/18/jvm-attach/)），通过发送load命令来加载agent。

#### instrument agent的核心数据结构

```
struct _JPLISAgent {
    JavaVM *                mJVM;                   /* handle to the JVM */
    JPLISEnvironment        mNormalEnvironment;     /* for every thing but retransform stuff */
    JPLISEnvironment        mRetransformEnvironment;/* for retransform stuff only */
    jobject                 mInstrumentationImpl;   /* handle to the Instrumentation instance */
    jmethodID               mPremainCaller;         /* method on the InstrumentationImpl that does the premain stuff (cached to save lots of lookups) */
    jmethodID               mAgentmainCaller;       /* method on the InstrumentationImpl for agents loaded via attach mechanism */
    jmethodID               mTransform;             /* method on the InstrumentationImpl that does the class file transform */
    jboolean                mRedefineAvailable;     /* cached answer to "does this agent support redefine" */
    jboolean                mRedefineAdded;         /* indicates if can_redefine_classes capability has been added */
    jboolean                mNativeMethodPrefixAvailable; /* cached answer to "does this agent support prefixing" */
    jboolean                mNativeMethodPrefixAdded;     /* indicates if can_set_native_method_prefix capability has been added */
    char const *            mAgentClassName;        /* agent class name */
    char const *            mOptionsString;         /* -javaagent options string */
};

struct _JPLISEnvironment {
    jvmtiEnv *              mJVMTIEnv;              /* the JVM TI environment */
    JPLISAgent *            mAgent;                 /* corresponding agent */
    jboolean                mIsRetransformer;       /* indicates if special environment */
};
```

这里解释一下几个重要项：

- mNormalEnvironment：主要提供正常的类transform及redefine功能。
- mRetransformEnvironment：主要提供类retransform功能。
- mInstrumentationImpl：这个对象非常重要，也是我们Java agent和JVM进行交互的入口，或许写过javaagent的人在写`premain`以及`agentmain`方法的时候注意到了有个Instrumentation参数，该参数其实就是这里的对象。
- mPremainCaller：指向`sun.instrument.InstrumentationImpl.loadClassAndCallPremain`方法，如果agent是在启动时加载的，则该方法会被调用。
- mAgentmainCaller：指向`sun.instrument.InstrumentationImpl.loadClassAndCallAgentmain`方法，该方法在通过attach的方式动态加载agent的时候调用。
- mTransform：指向`sun.instrument.InstrumentationImpl.transform`方法。
- mAgentClassName：在我们javaagent的MANIFEST.MF里指定的`Agent-Class`。
- mOptionsString：传给agent的一些参数。
- mRedefineAvailable：是否开启了redefine功能，在javaagent的MANIFEST.MF里设置`Can-Redefine-Classes:true`。
- mNativeMethodPrefixAvailable：是否支持native方法前缀设置，同样在javaagent的MANIFEST.MF里设置`Can-Set-Native-Method-Prefix:true`。
- mIsRetransformer：如果在javaagent的MANIFEST.MF文件里定义了`Can-Retransform-Classes:true`，将会设置mRetransformEnvironment的mIsRetransformer为true。

#### 在启动时加载instrument agent

正如前面“概述”里提到的方式，就是启动时加载instrument agent，具体过程都在`InvocationAdapter.c`的`Agent_OnLoad`方法里，这里简单描述下过程：

- 创建并初始化JPLISAgent
  - 监听VMInit事件，在vm初始化完成之后做下面的事情：		创建InstrumentationImpl对象		监听ClassFileLoadHook事件		调用InstrumentationImpl的`loadClassAndCallPremain`方法，在这个方法里会调用javaagent里MANIFEST.MF里指定的`Premain-Class`类的premain方法		
- 解析javaagent里MANIFEST.MF里的参数，并根据这些参数来设置JPLISAgent里的一些内容

#### 在运行时加载instrument agent

在运行时加载的方式，大致按照下面的方式来操作：

```
VirtualMachine vm = VirtualMachine.attach(pid); 
vm.loadAgent(agentPath, agentArgs); 
```

上面会通过JVM的attach机制来请求目标JVM加载对应的agent，过程大致如下：

- 创建并初始化JPLISAgent
- 解析javaagent里MANIFEST.MF里的参数
- 创建InstrumentationImpl对象
- 监听ClassFileLoadHook事件
- 调用InstrumentationImpl的loadClassAndCallAgentmain方法，在这个方法里会调用javaagent里MANIFEST.MF里指定的Agent-Class类的agentmain方法

#### **instrument agent的ClassFileLoadHook回调实现**

不管是启动时还是运行时加载的instrument agent，都关注着同一个jvmti事件——ClassFileLoadHook，这个事件是在读取字节码文件之后回调时用的，这样可以对原来的字节码做修改，那这里面究竟是怎样实现的呢？

```
void JNICALL

eventHandlerClassFileLoadHook(  jvmtiEnv *              jvmtienv,
                                JNIEnv *                jnienv,
                                jclass                  class_being_redefined,
                                jobject                 loader,
                                const char*             name,
                                jobject                 protectionDomain,
                                jint                    class_data_len,
                                const unsigned char*    class_data,
                                jint*                   new_class_data_len,
                                unsigned char**         new_class_data) {

    JPLISEnvironment * environment  = NULL;

    environment = getJPLISEnvironment(jvmtienv);

    /* if something is internally inconsistent (no agent), just silently return without touching the buffer */

    if ( environment != NULL ) {

        jthrowable outstandingException = preserveThrowable(jnienv);
        transformClassFile( environment->mAgent,
                            jnienv,
                            loader,
                            name,
                            class_being_redefined,
                            protectionDomain,
                            class_data_len,
                            class_data,
                            new_class_data_len,
                            new_class_data,
                            environment->mIsRetransformer);

        restoreThrowable(jnienv, outstandingException);
    }

}
```

先根据jvmtiEnv取得对应的JPLISEnvironment，因为上面我已经说到其实有两个JPLISEnvironment（并且有两个jvmtiEnv），其中一个是专门做retransform的，而另外一个用来做其他事情，根据不同的用途，在注册具体的ClassFileTransformer时也是分开的，对于作为retransform用的ClassFileTransformer，我们会注册到一个单独的TransformerManager里。

接着调用transformClassFile方法，由于函数实现比较长，这里就不贴代码了，大致意思就是调用InstrumentationImpl对象的transform方法，根据最后那个参数来决定选哪个TransformerManager里的ClassFileTransformer对象们做transform操作。

```
private byte[]
    transform(  ClassLoader         loader,
                String              classname,
                Class               classBeingRedefined,
                ProtectionDomain    protectionDomain,
                byte[]              classfileBuffer,
                boolean             isRetransformer) {

        TransformerManager mgr = isRetransformer?

                                        mRetransfomableTransformerManager :
                                        mTransformerManager;

        if (mgr == null) {

            return null; // no manager, no transform

        } else {

            return mgr.transform(   loader,
                                    classname,
                                    classBeingRedefined,
                                    protectionDomain,
                                    classfileBuffer);

        }

    }


  public byte[]

    transform(  ClassLoader         loader,
                String              classname,
                Class               classBeingRedefined,
                ProtectionDomain    protectionDomain,
                byte[]              classfileBuffer) {

        boolean someoneTouchedTheBytecode = false;
        TransformerInfo[]  transformerList = getSnapshotTransformerList();
        byte[]  bufferToUse = classfileBuffer;

        // order matters, gotta run 'em in the order they were added

        for ( int x = 0; x < transformerList.length; x++ ) {

            TransformerInfo         transformerInfo = transformerList[x];
            ClassFileTransformer    transformer = transformerInfo.transformer();
            byte[]                  transformedBytes = null;

            try {

                transformedBytes = transformer.transform(   loader,
                                                            classname,
                                                            classBeingRedefined,
                                                            protectionDomain,
                                                            bufferToUse);

            }

            catch (Throwable t) {

                // don't let any one transformer mess it up for the others.
                // This is where we need to put some logging. What should go here? FIXME

            }


            if ( transformedBytes != null ) {
                someoneTouchedTheBytecode = true;
                bufferToUse = transformedBytes;
            }

        }


        // if someone modified it, return the modified buffer.
        // otherwise return null to mean "no transforms occurred"

        byte [] result;

        if ( someoneTouchedTheBytecode ) {
            result = bufferToUse;
        }
        else {
            result = null;
        }

        return result;

    }   
```

以上是最终调到的java代码，可以看到已经调用到我们自己编写的javaagent代码里了，我们一般是实现一个ClassFileTransformer类，然后创建一个对象注册到对应的TransformerManager里。

#### **Class Transform的实现**

这里说的class transform其实是狭义的，主要是针对第一次类文件加载时就要求被transform的场景，在加载类文件的时候发出ClassFileLoad事件，然后交给instrumenat agent来调用javaagent里注册的ClassFileTransformer实现字节码的修改。

#### **Class Redefine的实现**

类重新定义，这是Instrumentation提供的基础功能之一，主要用在已经被加载过的类上，想对其进行修改，要做这件事，我们必须要知道两个东西，一个是要修改哪个类，另外一个是想将那个类修改成怎样的结构，有了这两个信息之后就可以通过InstrumentationImpl下面的redefineClasses方法操作了：

```
public void redefineClasses(ClassDefinition[]   definitions) throws  ClassNotFoundException {

        if (!isRedefineClassesSupported()) {

            throw new UnsupportedOperationException("redefineClasses is not supported in this environment");

        }

        if (definitions == null) {

            throw new NullPointerException("null passed as 'definitions' in redefineClasses");

        }

        for (int i = 0; i < definitions.length; ++i) {

            if (definitions[i] == null) {

                throw new NullPointerException("element of 'definitions' is null in redefineClasses");

            }

        }

        if (definitions.length == 0) {

            return; // short-circuit if there are no changes requested

        }


        redefineClasses0(mNativeAgent, definitions);

    }
```

在JVM里对应的实现是创建一个VM_RedefineClasses的VM_Operation，注意执行它的时候会stop-the-world：

```
jvmtiError

JvmtiEnv::RedefineClasses(jint class_count, const jvmtiClassDefinition* class_definitions) {

//TODO: add locking

  VM_RedefineClasses op(class_count, class_definitions, jvmti_class_load_kind_redefine);

  VMThread::execute(&op);

  return (op.check_error());

} /* end RedefineClasses */
```

这个过程我尽量用语言来描述清楚，不详细贴代码了，因为代码量实在有点大：

- 挨个遍历要批量重定义的jvmtiClassDefinition
- 然后读取新的字节码，如果有关注ClassFileLoadHook事件的，还会走对应的transform来对新的字节码再做修改
- 字节码解析好，创建一个klassOop对象
  - 对比新老类，并要求如下：		父类是同一个		实现的接口数也要相同，并且是相同的接口		类访问符必须一致		字段数和字段名要一致		新增的方法必须是private static/final的		可以删除修改方法		
- 对新类做字节码校验
- 合并新老类的常量池
- 如果老类上有断点，那都清除掉
- 对老类做JIT去优化
- 对新老方法匹配的方法的jmethodId做更新，将老的jmethodId更新到新的method上
- 新类的常量池的holer指向老的类
- 将新类和老类的一些属性做交换，比如常量池，methods，内部类
- 初始化新的vtable和itable
- 交换annotation的method、field、paramenter
- 遍历所有当前类的子类，修改他们的vtable及itable

上面是基本的过程，总的来说就是只更新了类里的内容，相当于只更新了指针指向的内容，并没有更新指针，避免了遍历大量已有类对象对它们进行更新所带来的开销。

#### **Class Retransform的实现**

retransform class可以简单理解为回滚操作，具体回滚到哪个版本，这个需要看情况而定，下面不管那种情况都有一个前提，那就是javaagent已经要求要有retransform的能力了：

- 如果类是在第一次加载的的时候就做了transform，那么做retransform的时候会将代码回滚到transform之后的代码
- 如果类是在第一次加载的的时候没有任何变化，那么做retransform的时候会将代码回滚到最原始的类文件里的字节码
- 如果类已经加载了，期间类可能做过多次redefine(比如被另外一个agent做过)，但是接下来加载一个新的agent要求有retransform的能力了，然后对类做redefine的动作，那么retransform的时候会将代码回滚到上一个agent最后一次做redefine后的字节码

我们从InstrumentationImpl的retransformClasses方法参数看猜到应该是做回滚操作，因为我们只指定了class：

```
    public void retransformClasses(Class<?>[] classes) {

        if (!isRetransformClassesSupported()) {

            throw new UnsupportedOperationException( "retransformClasses is not supported in this environment");

        }

        retransformClasses0(mNativeAgent, classes);

    }
```

不过retransform的实现其实也是通过redefine的功能来实现，在类加载的时候有比较小的差别，主要体现在究竟会走哪些transform上，如果当前是做retransform的话，那将忽略那些注册到正常的TransformerManager里的ClassFileTransformer，而只会走专门为retransform而准备的TransformerManager的ClassFileTransformer，不然想象一下字节码又被无声无息改成某个中间态了。

```
private:

  void post_all_envs() {

    if (_load_kind != jvmti_class_load_kind_retransform) {

      // for class load and redefine,

      // call the non-retransformable agents

      JvmtiEnvIterator it;

      for (JvmtiEnv* env = it.first(); env != NULL; env = it.next(env)) {

        if (!env->is_retransformable() && env->is_enabled(JVMTI_EVENT_CLASS_FILE_LOAD_HOOK)) {

          // non-retransformable agents cannot retransform back,

          // so no need to cache the original class file bytes

          post_to_env(env, false);

        }

      }

    }

    JvmtiEnvIterator it;

    for (JvmtiEnv* env = it.first(); env != NULL; env = it.next(env)) {

      // retransformable agents get all events

      if (env->is_retransformable() && env->is_enabled(JVMTI_EVENT_CLASS_FILE_LOAD_HOOK)) {

        // retransformable agents need to cache the original class file

        // bytes if changes are made via the ClassFileLoadHook

        post_to_env(env, true);

      }

    }

  }
```

#### **javaagent的其他小众功能**

javaagent除了做字节码上面的修改之外，其实还有一些小功能，有时候还是挺有用的

- 获取所有已经被加载的类：Class[] getAllLoadedClasses(); 
- 获取所有已经初始化了的类： Class[] getInitiatedClasses(ClassLoader loader); 
- 获取某个对象的大小： long getObjectSize(Object objectToSize); 
- 将某个jar加入到bootstrap classpath里优先其他jar被加载： void appendToBootstrapClassLoaderSearch(JarFile jarfile); 
- 将某个jar加入到classpath里供appclassloard去加载：void appendToSystemClassLoaderSearch(JarFile jarfile); 
- 设置某些native方法的前缀，主要在找native方法的时候做规则匹配： void setNativeMethodPrefix(ClassFileTransformer transformer, String prefix)。



## agentlib vs javaagent

* javaagent 在jvm范围内，产生jvm对象，持有引用，被gc影响也影响gc



## 使用

ref： [JavaSe：-javaagent,-agentlib,-agentpath](http://www.cnblogs.com/f1194361820/p/5059367.html)

**-javaagent、-agentlib、-agentpath**

**-agentlib:libname[=options]** 

 用于装载本地lib包；
 其中libname为本地代理库文件名，默认搜索路径为环境变量PATH中的路径，options为传给本地库启动时的参数，多个参数之间用逗号分隔。在Windows平台上jvm搜索本地库名为libname.dll的文件，在linux上jvm搜索本地库名为libname.so的文件，搜索路径环境变量在不同系统上有所不同，比如Solaries上就默认搜索LD_LIBRARY_PATH。
 比如：-agentlib:hprof
 用来获取jvm的运行情况，包括CPU、内存、线程等的运行数据，并可输出到指定文件中；windows中搜索路径为JRE_HOME/bin/hprof.dll。

**-agentpath:pathname[=options]** 
 按全路径装载本地库，不再搜索PATH中的路径；其他功能和agentlib相同；更多的信息待续，在后续的JVMTI部分会详述。

**-javaagent:jarpath[=options]** 

 指定jvm启动时装入java语言基础设施代理。jarpath文件中的mainfest文件必须有Premain-Class（启动前捆绑时需要）， Agent-Class（运行时捆绑时需要）属性。代理类也必须实现公共的静态public static void premain(String agentArgs, Instrumentation inst)方法（和main方法类似）。当jvm初始化时，将按代理类的说明顺序调用premain方法；具体参见java.lang.instrument软件包的描述。



```java
package com.fjn.jdk.jvm.options.javaagent;

import java.io.IOException;
import java.lang.instrument.Instrumentation;

import com.sun.tools.attach.AttachNotSupportedException;

public class CustomAgent {
    /**
     * 如果Agent是通过JVM选项的方式捆绑到程序中，则在JVM初化完毕后，会执行premain方法，premain执行之后才是程序的main方法。
     * 清单文件中需要指定Premain-Class 
     * <p>
     * premain有两种形式，默认会执行1), 如果没有1)则会执行2), 1)和2)只会执行一个<br>
     * <code>
     * 1) public static void premain(String agentArgs, Instrumentation instrumentation)<br/>
     * 2) public static void premain(String agentArgs)
     * </code></p>
     * 
     * @param agentArgs
     * @param instrumentation
     */
    public static void premain(String agentArgs, Instrumentation instrumentation) {
        System.out
                .println("CustomAgent#premain(String agentArgs, Instrumentation instrumentation)");
        parseAgentArgs(agentArgs);
    }

    public static void premain(String agentArgs) {
        System.out.println("CustomAgent#premain(String agentArgs)");
        parseAgentArgs(agentArgs);
    }
    
    
    /**
     * 如果Agent是在程序运行过程中，动态的捆绑到程序中，则是执行agentmain方法。
     * 清单文件中要指定 Agent-Class
     * <p>
     * agentmain有两种形式，默认会执行1), 如果没有1)则会执行2), 1)和2)只会执行一个<br>
     * <code>
     * 1) public static void agentmain(String agentArgs, Instrumentation instrumentation)<br/>
     * 2) public static void agentmain(String agentArgs)
     * </code></p>
     * 
     * 通过程序捆绑的代码：<br/>
     * <code>
     *     VirtualMachine vm=VirtualMachine.attach("PID"); //给指定的进程捆绑agent<br/>
     *  在得到目标进程的vm后，就可以通过
     *    vm.loadAgent("agentjar"),vm.loadAgentLibrary(dll), and loadAgentPath(dllPath) 进行捆绑操作了 <br/>
     *  其中:<br>
     *        loadAgent是捆绑一个jar文件，
     *        loadAgentLibrary,loadAgentPath则是捆绑本地方法库（动态连接库）
     * </code>
     * 
     * @param agentArgs
     * @param inst
     */
    public static void agentmain(String agentArgs, Instrumentation inst) {
        System.out
                .println("CustomAgent#agentmain(String agentArgs, Instrumentation instrumentation)");
        parseAgentArgs(agentArgs);
    }

    public static void agentmain(String agentArgs) {
        System.out.println("CustomAgent#agentmain(String agentArgs)");
        parseAgentArgs(agentArgs);
    }

    /**
     * 不论是premain,还在agentmain,都可以指定参数，参数是一个字符串，具体怎么解析，是程序自己的事
     * @param agentArgs
     * @return
     * @throws IOException 
     * @throws AttachNotSupportedException 
     */
    private static boolean parseAgentArgs(String agentArgs) {
        boolean hasArgs = false;
        if (agentArgs != null && !agentArgs.isEmpty()) {
            System.out.println("agentArgs is : " + agentArgs);
            hasArgs = true;
        } else {
            System.out.println("has no agentArgs .");
        }

        return hasArgs;
    }
}
```



```
Manifest-Version: 1.0
Premain-Class: com.fjn.jdk.jvm.options.javaagent.CustomAgent
Sealed: true
```



### 启动加载多个agent

```shell
java -javaagent:agentA.jar -javaagent:agentB.jar MyJavaProgram
```



### 混用javaagent和agentlib

一起用自然可以，但是没办法做交互。

ref：[java agent with jvmti load at runtime, unload from within](https://stackoverflow.com/questions/17377130/java-agent-with-jvmti-load-at-runtime-unload-from-within)

1. You can only inject (deploy) an agent either at [start time](http://docs.oracle.com/javase/7/docs/platform/jvmti/jvmti.html#onload) of the JVM by passing the argument `-agentlib:<agent-lib-name>=<options>` or `-agentpath:<path-to-agent>=<options>`.

   The other way is through Java itself. This depends heavily on the JVM, for that reason it is out of JVMTI specification scope. For instance if there is the [`VirtualMachine`](http://docs.oracle.com/javase/6/docs/jdk/api/attach/spec/com/sun/tools/attach/VirtualMachine.html) class through the method [`loadAgentPath(agentPath, options)`](http://docs.oracle.com/javase/6/docs/jdk/api/attach/spec/com/sun/tools/attach/VirtualMachine.html#loadAgentPath%28java.lang.String,%20java.lang.String%29). If you want to execute this from within the native JVMTI code you would need to do Bytecode instrumentation.

2. I am not sure, similar to 1. you could run the Java `unload` method through Bytecode instrumentation. If `dlclose()` works I do not see a problem in doing that.

As you see, you can pass data to the Java agent by using the options. Alternatively, if you want to pass data continuously between both instances you can open two sockets and write/read between them. I used [Protobuf](https://code.google.com/p/protobuf/)



## 实现：how to load JVMTI agent



### load

```cpp
jint JvmtiExport::load_agent_library(AttachOperation* op, outputStream* st) {
  char ebuf[1024];
  char buffer[JVM_MAXPATHLEN];
  void* library = NULL;
  jint result = JNI_ERR;
  const char *on_attach_symbols[] = AGENT_ONATTACH_SYMBOLS;
  size_t num_symbol_entries = ARRAY_SIZE(on_attach_symbols);

  // get agent name and options
  const char* agent = op->arg(0);
  const char* absParam = op->arg(1);
  const char* options = op->arg(2);

  // The abs paramter should be "true" or "false"
  bool is_absolute_path = (absParam != NULL) && (strcmp(absParam,"true")==0);

  // Initially marked as invalid. It will be set to valid if we can find the agent
  AgentLibrary *agent_lib = new AgentLibrary(agent, options, is_absolute_path, NULL);

  // Check for statically linked in agent. If not found then if the path is
  // absolute we attempt to load the library. Otherwise we try to load it
  // from the standard dll directory.

  if (!os::find_builtin_agent(agent_lib, on_attach_symbols, num_symbol_entries)) {
    if (is_absolute_path) {
      library = os::dll_load(agent, ebuf, sizeof ebuf);
    } else {
      // Try to load the agent from the standard dll directory
      if (os::dll_build_name(buffer, sizeof(buffer), Arguments::get_dll_dir(),
                             agent)) {
        library = os::dll_load(buffer, ebuf, sizeof ebuf);
      }
      if (library == NULL) {
        // not found - try local path
        char ns[1] = {0};
        if (os::dll_build_name(buffer, sizeof(buffer), ns, agent)) {
          library = os::dll_load(buffer, ebuf, sizeof ebuf);
        }
      }
    }
    if (library != NULL) {
      agent_lib->set_os_lib(library);
      agent_lib->set_valid();
    }
  }
  // If the library was loaded then we attempt to invoke the Agent_OnAttach
  // function
  if (agent_lib->valid()) {
    // Lookup the Agent_OnAttach function
    OnAttachEntry_t on_attach_entry = NULL;
    on_attach_entry = CAST_TO_FN_PTR(OnAttachEntry_t,
       os::find_agent_function(agent_lib, false, on_attach_symbols, num_symbol_entries));
    if (on_attach_entry == NULL) {
      // Agent_OnAttach missing - unload library
      if (!agent_lib->is_static_lib()) {
        os::dll_unload(library);
      }
      delete agent_lib;
    } else {
      // Invoke the Agent_OnAttach function
      JavaThread* THREAD = JavaThread::current();
      {
        extern struct JavaVM_ main_vm;
        JvmtiThreadEventMark jem(THREAD);
        JvmtiJavaThreadEventTransition jet(THREAD);

        result = (*on_attach_entry)(&main_vm, (char*)options, NULL);
      }

      // Agent_OnAttach may have used JNI
      if (HAS_PENDING_EXCEPTION) {
        CLEAR_PENDING_EXCEPTION;
      }

      // If OnAttach returns JNI_OK then we add it to the list of
      // agent libraries so that we can call Agent_OnUnload later.
      if (result == JNI_OK) {
        Arguments::add_loaded_agent(agent_lib);
      } else {
        delete agent_lib;
      }

      // Agent_OnAttach executed so completion status is JNI_OK
      st->print_cr("%d", result);
      result = JNI_OK;
    }
  }
  return result;
}
```





### find entry

```cpp
#define AGENT_ONLOAD_SYMBOLS    {"Agent_OnLoad"}


// Find the Agent_OnLoad entry point
static OnLoadEntry_t lookup_agent_on_load(AgentLibrary* agent) {
  const char *on_load_symbols[] = AGENT_ONLOAD_SYMBOLS;
  return lookup_on_load(agent, on_load_symbols, sizeof(on_load_symbols) / sizeof(char*));
}


// Find a command line agent library and return its entry point for
//         -agentlib:  -agentpath:   -Xrun
// num_symbol_entries must be passed-in since only the caller knows the number of symbols in the array.
static OnLoadEntry_t lookup_on_load(AgentLibrary* agent, const char *on_load_symbols[], size_t num_symbol_entries) {
  OnLoadEntry_t on_load_entry = NULL;
  void *library = NULL;

  if (!agent->valid()) {
    char buffer[JVM_MAXPATHLEN];
    char ebuf[1024];
    const char *name = agent->name();
    const char *msg = "Could not find agent library ";

    // First check to see if agent is statically linked into executable
    if (os::find_builtin_agent(agent, on_load_symbols, num_symbol_entries)) {
      library = agent->os_lib();
    } else if (agent->is_absolute_path()) {
      library = os::dll_load(name, ebuf, sizeof ebuf);
      if (library == NULL) {
        const char *sub_msg = " in absolute path, with error: ";
        size_t len = strlen(msg) + strlen(name) + strlen(sub_msg) + strlen(ebuf) + 1;
        char *buf = NEW_C_HEAP_ARRAY(char, len, mtThread);
        jio_snprintf(buf, len, "%s%s%s%s", msg, name, sub_msg, ebuf);
        // If we can't find the agent, exit.
        vm_exit_during_initialization(buf, NULL);
        FREE_C_HEAP_ARRAY(char, buf, mtThread);
      }
    } else {
      // Try to load the agent from the standard dll directory
      if (os::dll_build_name(buffer, sizeof(buffer), Arguments::get_dll_dir(),
                             name)) {
        library = os::dll_load(buffer, ebuf, sizeof ebuf);
      }
      if (library == NULL) { // Try the local directory
        char ns[1] = {0};
        if (os::dll_build_name(buffer, sizeof(buffer), ns, name)) {
          library = os::dll_load(buffer, ebuf, sizeof ebuf);
        }
        if (library == NULL) {
          const char *sub_msg = " on the library path, with error: ";
          size_t len = strlen(msg) + strlen(name) + strlen(sub_msg) + strlen(ebuf) + 1;
          char *buf = NEW_C_HEAP_ARRAY(char, len, mtThread);
          jio_snprintf(buf, len, "%s%s%s%s", msg, name, sub_msg, ebuf);
          // If we can't find the agent, exit.
          vm_exit_during_initialization(buf, NULL);
          FREE_C_HEAP_ARRAY(char, buf, mtThread);
        }
      }
    }
    agent->set_os_lib(library);
    agent->set_valid();
  }

  // Find the OnLoad function.
  on_load_entry =
    CAST_TO_FN_PTR(OnLoadEntry_t, os::find_agent_function(agent,
                                                          false,
                                                          on_load_symbols,
                                                          num_symbol_entries));
  return on_load_entry;
}


/*
 * Support for finding Agent_On(Un)Load/Attach<_lib_name> if it exists.
 * If check_lib == true then we are looking for an
 * Agent_OnLoad_lib_name or Agent_OnAttach_lib_name function to determine if
 * this library is statically linked into the image.
 * If check_lib == false then we will look for the appropriate symbol in the
 * executable if agent_lib->is_static_lib() == true or in the shared library
 * referenced by 'handle'.
 */
void* os::find_agent_function(AgentLibrary *agent_lib, bool check_lib,
                              const char *syms[], size_t syms_len) {
  assert(agent_lib != NULL, "sanity check");
  const char *lib_name;
  void *handle = agent_lib->os_lib();
  void *entryName = NULL;
  char *agent_function_name;
  size_t i;

  // If checking then use the agent name otherwise test is_static_lib() to
  // see how to process this lookup
  lib_name = ((check_lib || agent_lib->is_static_lib()) ? agent_lib->name() : NULL);
  for (i = 0; i < syms_len; i++) {
    agent_function_name = build_agent_function_name(syms[i], lib_name, agent_lib->is_absolute_path());
    if (agent_function_name == NULL) {
      break;
    }
    entryName = dll_lookup(handle, agent_function_name);
    FREE_C_HEAP_ARRAY(char, agent_function_name, mtThread);
    if (entryName != NULL) {
      break;
    }
  }
  return entryName;
}


/*
 * glibc-2.0 libdl is not MT safe.  If you are building with any glibc,
 * chances are you might want to run the generated bits against glibc-2.0
 * libdl.so, so always use locking for any version of glibc.
 */
void* os::dll_lookup(void* handle, const char* name) {
  pthread_mutex_lock(&dl_mutex);
  void* res = dlsym(handle, name);
  pthread_mutex_unlock(&dl_mutex);
  return res;
}
```









# VM实现





## 产生JVMTI事件

```cpp

#ifdef VM_JVMTI
      _jvmti_interp_events = JvmtiExport::can_post_interpreter_events();
#endif


#define JVMTI_SUPPORT_FLAG(key)                                           \
  private:                                                                \
  static bool  _##key;                                                    \
  public:                                                                 \
  inline static void set_##key(bool on) {                                 \
    JVMTI_ONLY(_##key = (on != 0));                                       \
    NOT_JVMTI(report_unsupported(on));                                    \
  }                                                                       \
  inline static bool key() {                                              \
    JVMTI_ONLY(return _##key);                                            \
    NOT_JVMTI(return false);                                              \
  }


  bool interp_events =
    avail.can_generate_field_access_events ||
    avail.can_generate_field_modification_events ||
    avail.can_generate_single_step_events ||
    avail.can_generate_frame_pop_events ||
    avail.can_generate_method_entry_events ||
    avail.can_generate_method_exit_events;

JvmtiExport::set_can_post_interpreter_events(interp_events);
```





```cpp

      // Notify jvmti
#ifdef VM_JVMTI
      if (_jvmti_interp_events) {
        // Whenever JVMTI puts a thread in interp_only_mode, method
        // entry/exit events are sent for that thread to track stack depth.
        if (THREAD->is_interp_only_mode()) {
          CALL_VM(InterpreterRuntime::post_method_entry(THREAD),
                  handle_exception);
        }
      }
#endif /* VM_JVMTI */
```



```cpp
void JvmtiExport::post_method_entry(JavaThread *thread, Method* method, frame current_frame) {
  HandleMark hm(thread);
  methodHandle mh(thread, method);

  EVT_TRIG_TRACE(JVMTI_EVENT_METHOD_ENTRY, ("JVMTI [%s] Trg Method Entry triggered %s.%s",
                     JvmtiTrace::safe_get_thread_name(thread),
                     (mh() == NULL) ? "NULL" : mh()->klass_name()->as_C_string(),
                     (mh() == NULL) ? "NULL" : mh()->name()->as_C_string() ));

  JvmtiThreadState* state = thread->jvmti_thread_state();
  if (state == NULL || !state->is_interp_only_mode()) {
    // for any thread that actually wants method entry, interp_only_mode is set
    return;
  }

  state->incr_cur_stack_depth();

  if (state->is_enabled(JVMTI_EVENT_METHOD_ENTRY)) {
    JvmtiEnvThreadStateIterator it(state);
    for (JvmtiEnvThreadState* ets = it.first(); ets != NULL; ets = it.next(ets)) {
      if (ets->is_enabled(JVMTI_EVENT_METHOD_ENTRY)) {
        EVT_TRACE(JVMTI_EVENT_METHOD_ENTRY, ("JVMTI [%s] Evt Method Entry sent %s.%s",
                                             JvmtiTrace::safe_get_thread_name(thread),
                                             (mh() == NULL) ? "NULL" : mh()->klass_name()->as_C_string(),
                                             (mh() == NULL) ? "NULL" : mh()->name()->as_C_string() ));

        JvmtiEnv *env = ets->get_env();
        JvmtiMethodEventMark jem(thread, mh);
        JvmtiJavaThreadEventTransition jet(thread);
        jvmtiEventMethodEntry callback = env->callbacks()->MethodEntry;
        if (callback != NULL) {
          (*callback)(env->jvmti_external(), jem.jni_env(), jem.jni_thread(), jem.jni_methodID());
        }
      }
    }
  }
}
```











# API



## java API？

ref： [java-api-on-top-of-jvmti - SO](https://stackoverflow.com/questions/4819099/java-api-on-top-of-jvmti)

ref里提到了JVM TI的定义就是：

```
The JVM tool interface (JVM TI) is a standard native API that allows for native libraries to capture events and control a Java Virtual Machine (JVM) for the Java platform.
```

so… 不会有目前也没有java binding。



不过参考[github bytecode-examples](https://github.com/jon-bell/bytecode-examples)可以看到，可以通过JNI的方式（把agent作为native method）来和java代码交互。

但这依赖target process加载该java代码，而不是作为单独javaagent（无侵入性）。



## 事件（回调）



ref： [深入 Java 调试体系，第 2 部分 JVMTI 和 Agent 实现](https://www.ibm.com/developerworks/cn/java/j-lo-jpda2/)



设置回调函数的时候，开发者需要注意以下几点：

- 如同 Java 异常机制一样，如果在回调函数中自己抛出一个异常（Exception），或者在调用 JNI 函数的时候制造了一些麻烦，让 JNI 丢出了一个异常，那么任何在回调之前发生的异常就会丢失，这就要求开发人员要在处理错误的时候需要当心。
- 虚拟机不保证回调函数会被同步，换句话说，程序有可能同时运行同一个回调函数（比如，好几个线程同时开始运行了，这个 HandleThreadStart 就会被同时调用几次），那么开发人员在开发回调函数时需要处理同步的问题。











