

# impl





## newInstance



```java
    @CallerSensitive
    public static Object newProxyInstance(ClassLoader loader,
                                          Class<?>[] interfaces,
                                          InvocationHandler h)
        throws IllegalArgumentException
```



```java
    {
        Objects.requireNonNull(h);

        final Class<?>[] intfs = interfaces.clone();
        final SecurityManager sm = System.getSecurityManager();
        if (sm != null) {
            checkProxyAccess(Reflection.getCallerClass(), loader, intfs);
        }

        /*
         * Look up or generate the designated proxy class.
         */
        Class<?> cl = getProxyClass0(loader, intfs);

        /*
         * Invoke its constructor with the designated invocation handler.
         */
        try {
            if (sm != null) {
                checkNewProxyPermission(Reflection.getCallerClass(), cl);
            }

            final Constructor<?> cons = cl.getConstructor(constructorParams);
            final InvocationHandler ih = h;
            if (!Modifier.isPublic(cl.getModifiers())) {
                AccessController.doPrivileged(new PrivilegedAction<Void>() {
                    public Void run() {
                        cons.setAccessible(true);
                        return null;
                    }
                });
            }
            return cons.newInstance(new Object[]{h});
        } catch (IllegalAccessException|InstantiationException e) {
            throw new InternalError(e.toString(), e);
        } catch (InvocationTargetException e) {
            Throwable t = e.getCause();
            if (t instanceof RuntimeException) {
                throw (RuntimeException) t;
            } else {
                throw new InternalError(t.toString(), t);
            }
        } catch (NoSuchMethodException e) {
            throw new InternalError(e.toString(), e);
        }
    }
```



实际上是：

> 实时生成类，满足以下：
>
> 1. 实现指定的（多个）接口
> 2. 有一个参数为`InvocationHandler`的构造器
> 3. 对接口的所有声明方法，实现为调该`InvocationHandler`的`invoke`方法



可知，重要的是生成类的过程。



### 生成类的过程



#### 对于生成类的cache



#### 实际生成类 - `Proxy.ProxyClassFactory`





##### code

```java
    /**
     * A factory function that generates, defines and returns the proxy class given
     * the ClassLoader and array of interfaces.
     */
    private static final class ProxyClassFactory
        implements BiFunction<ClassLoader, Class<?>[], Class<?>>
    {
        // prefix for all proxy class names
        private static final String proxyClassNamePrefix = "$Proxy";

        // next number to use for generation of unique proxy class names
        private static final AtomicLong nextUniqueNumber = new AtomicLong();

        @Override
        public Class<?> apply(ClassLoader loader, Class<?>[] interfaces) {

            Map<Class<?>, Boolean> interfaceSet = new IdentityHashMap<>(interfaces.length);
            for (Class<?> intf : interfaces) {
                /*
                 * Verify that the class loader resolves the name of this
                 * interface to the same Class object.
                 */
                Class<?> interfaceClass = null;
                try {
                    interfaceClass = Class.forName(intf.getName(), false, loader);
                } catch (ClassNotFoundException e) {
                }
                if (interfaceClass != intf) {
                    throw new IllegalArgumentException(
                        intf + " is not visible from class loader");
                }
                /*
                 * Verify that the Class object actually represents an
                 * interface.
                 */
                if (!interfaceClass.isInterface()) {
                    throw new IllegalArgumentException(
                        interfaceClass.getName() + " is not an interface");
                }
                /*
                 * Verify that this interface is not a duplicate.
                 */
                if (interfaceSet.put(interfaceClass, Boolean.TRUE) != null) {
                    throw new IllegalArgumentException(
                        "repeated interface: " + interfaceClass.getName());
                }
            }

            String proxyPkg = null;     // package to define proxy class in
            int accessFlags = Modifier.PUBLIC | Modifier.FINAL;

            /*
             * Record the package of a non-public proxy interface so that the
             * proxy class will be defined in the same package.  Verify that
             * all non-public proxy interfaces are in the same package.
             */
            for (Class<?> intf : interfaces) {
                int flags = intf.getModifiers();
                if (!Modifier.isPublic(flags)) {
                    accessFlags = Modifier.FINAL;
                    String name = intf.getName();
                    int n = name.lastIndexOf('.');
                    String pkg = ((n == -1) ? "" : name.substring(0, n + 1));
                    if (proxyPkg == null) {
                        proxyPkg = pkg;
                    } else if (!pkg.equals(proxyPkg)) {
                        throw new IllegalArgumentException(
                            "non-public interfaces from different packages");
                    }
                }
            }

            if (proxyPkg == null) {
                // if no non-public proxy interfaces, use com.sun.proxy package
                proxyPkg = ReflectUtil.PROXY_PACKAGE + ".";
            }

            /*
             * Choose a name for the proxy class to generate.
             */
            long num = nextUniqueNumber.getAndIncrement();
            String proxyName = proxyPkg + proxyClassNamePrefix + num;

            /*
             * Generate the specified proxy class.
             */
            byte[] proxyClassFile = ProxyGenerator.generateProxyClass(
                proxyName, interfaces, accessFlags);
            try {
                return defineClass0(loader, proxyName,
                                    proxyClassFile, 0, proxyClassFile.length);
            } catch (ClassFormatError e) {
                /*
                 * A ClassFormatError here means that (barring bugs in the
                 * proxy class generation code) there was some other
                 * invalid aspect of the arguments supplied to the proxy
                 * class creation (such as virtual machine limitations
                 * exceeded).
                 */
                throw new IllegalArgumentException(e.toString());
            }
        }
    }
```



##### 实际生成类bytes - `ProxyGenerator.generateProxyClass` & `ProxyGenerator.generateClassFile` & `ProxyGenerator.ProxyMethod` ...



步骤：

1. 加入method

   1. proxy methods

      遍历要proxy的各接口的各方法。

      先构造`ProxyMethod`，然后（`ProxyMethod.generateMethod`）转为`MethodInfo`

      > generateMethod方法里写死了method内容： 调`InvocationHandler` field（field name写死为`h`）的`invoke`，把方法名、实参等传给...

   2. fields

      把proxy的methods以`Method` field形式添加到生成的class

   3. constructor

      ```java
              MethodInfo minfo = new MethodInfo(
                  "<init>", "(Ljava/lang/reflect/InvocationHandler;)V",
                  ACC_PUBLIC);
      		// ...
              out.writeByte(opc_invokespecial);
              out.writeShort(cp.getMethodRef(
                  superclassName,
                  "<init>", "(Ljava/lang/reflect/InvocationHandler;)V"));
      ```

      可见是写死的... 也写死了会调parent的同...构造器

2. 生成常量池内容

   * 父类为`Proxy`
   * 类名runtime确定（前一阶段确定）

3. 生成bytes

   按照class格式依次写入各部分内容，如下：

   ```java
               /*
                * Write all the items of the "ClassFile" structure.
                * See JVMS section 4.1.
                */
                                           // u4 magic;
               dout.writeInt(0xCAFEBABE);
                                           // u2 minor_version;
               dout.writeShort(CLASSFILE_MINOR_VERSION);
                                           // u2 major_version;
               dout.writeShort(CLASSFILE_MAJOR_VERSION);

               cp.write(dout);             // (write constant pool)

                                           // u2 access_flags;
               dout.writeShort(accessFlags);
                                           // u2 this_class;
               dout.writeShort(cp.getClass(dotToSlash(className)));
                                           // u2 super_class;
               dout.writeShort(cp.getClass(superclassName));

                                           // u2 interfaces_count;
               dout.writeShort(interfaces.length);
                                           // u2 interfaces[interfaces_count];
               for (Class<?> intf : interfaces) {
                   dout.writeShort(cp.getClass(
                       dotToSlash(intf.getName())));
               }

                                           // u2 fields_count;
               dout.writeShort(fields.size());
                                           // field_info fields[fields_count];
               for (FieldInfo f : fields) {
                   f.write(dout);
               }

                                           // u2 methods_count;
               dout.writeShort(methods.size());
                                           // method_info methods[methods_count];
               for (MethodInfo m : methods) {
                   m.write(dout);
               }

                                            // u2 attributes_count;
               dout.writeShort(0); // (no ClassFile attributes for proxy classes)
   ```

   ​



###### code

```java
    /**
     * Generate a proxy class given a name and a list of proxy interfaces.
     *
     * @param name        the class name of the proxy class
     * @param interfaces  proxy interfaces
     * @param accessFlags access flags of the proxy class
    */
    public static byte[] generateProxyClass(final String name,
                                            Class<?>[] interfaces,
                                            int accessFlags)
    {
        ProxyGenerator gen = new ProxyGenerator(name, interfaces, accessFlags);
        final byte[] classFile = gen.generateClassFile();
      // ...
    }
```



```java
    /**
     * Generate a class file for the proxy class.  This method drives the
     * class file generation process.
     */
    private byte[] generateClassFile() {

        /* ============================================================
         * Step 1: Assemble ProxyMethod objects for all methods to
         * generate proxy dispatching code for.
         */

        /*
         * Record that proxy methods are needed for the hashCode, equals,
         * and toString methods of java.lang.Object.  This is done before
         * the methods from the proxy interfaces so that the methods from
         * java.lang.Object take precedence over duplicate methods in the
         * proxy interfaces.
         */
        addProxyMethod(hashCodeMethod, Object.class);
        addProxyMethod(equalsMethod, Object.class);
        addProxyMethod(toStringMethod, Object.class);

        /*
         * Now record all of the methods from the proxy interfaces, giving
         * earlier interfaces precedence over later ones with duplicate
         * methods.
         */
        for (Class<?> intf : interfaces) {
            for (Method m : intf.getMethods()) {
                addProxyMethod(m, intf);
            }
        }

        /*
         * For each set of proxy methods with the same signature,
         * verify that the methods' return types are compatible.
         */
        for (List<ProxyMethod> sigmethods : proxyMethods.values()) {
            checkReturnTypes(sigmethods);
        }

        /* ============================================================
         * Step 2: Assemble FieldInfo and MethodInfo structs for all of
         * fields and methods in the class we are generating.
         */
        try {
            methods.add(generateConstructor());

            for (List<ProxyMethod> sigmethods : proxyMethods.values()) {
                for (ProxyMethod pm : sigmethods) {

                    // add static field for method's Method object
                    fields.add(new FieldInfo(pm.methodFieldName,
                        "Ljava/lang/reflect/Method;",
                         ACC_PRIVATE | ACC_STATIC));

                    // generate code for proxy method and add it
                    methods.add(pm.generateMethod());
                }
            }

            methods.add(generateStaticInitializer());

        } catch (IOException e) {
            throw new InternalError("unexpected I/O Exception", e);
        }

        if (methods.size() > 65535) {
            throw new IllegalArgumentException("method limit exceeded");
        }
        if (fields.size() > 65535) {
            throw new IllegalArgumentException("field limit exceeded");
        }

        /* ============================================================
         * Step 3: Write the final class file.
         */

        /*
         * Make sure that constant pool indexes are reserved for the
         * following items before starting to write the final class file.
         */
        cp.getClass(dotToSlash(className));
        cp.getClass(superclassName);
        for (Class<?> intf: interfaces) {
            cp.getClass(dotToSlash(intf.getName()));
        }

        /*
         * Disallow new constant pool additions beyond this point, since
         * we are about to write the final constant pool table.
         */
        cp.setReadOnly();

        ByteArrayOutputStream bout = new ByteArrayOutputStream();
        DataOutputStream dout = new DataOutputStream(bout);

        try {
            /*
             * Write all the items of the "ClassFile" structure.
             * See JVMS section 4.1.
             */
                                        // u4 magic;
            dout.writeInt(0xCAFEBABE);
                                        // u2 minor_version;
            dout.writeShort(CLASSFILE_MINOR_VERSION);
                                        // u2 major_version;
            dout.writeShort(CLASSFILE_MAJOR_VERSION);

            cp.write(dout);             // (write constant pool)

                                        // u2 access_flags;
            dout.writeShort(accessFlags);
                                        // u2 this_class;
            dout.writeShort(cp.getClass(dotToSlash(className)));
                                        // u2 super_class;
            dout.writeShort(cp.getClass(superclassName));

                                        // u2 interfaces_count;
            dout.writeShort(interfaces.length);
                                        // u2 interfaces[interfaces_count];
            for (Class<?> intf : interfaces) {
                dout.writeShort(cp.getClass(
                    dotToSlash(intf.getName())));
            }

                                        // u2 fields_count;
            dout.writeShort(fields.size());
                                        // field_info fields[fields_count];
            for (FieldInfo f : fields) {
                f.write(dout);
            }

                                        // u2 methods_count;
            dout.writeShort(methods.size());
                                        // method_info methods[methods_count];
            for (MethodInfo m : methods) {
                m.write(dout);
            }

                                         // u2 attributes_count;
            dout.writeShort(0); // (no ClassFile attributes for proxy classes)

        } catch (IOException e) {
            throw new InternalError("unexpected I/O Exception", e);
        }

        return bout.toByteArray();
    }
```



```java
    /**
     * Add another method to be proxied, either by creating a new
     * ProxyMethod object or augmenting an old one for a duplicate
     * method.
     *
     * "fromClass" indicates the proxy interface that the method was
     * found through, which may be different from (a subinterface of)
     * the method's "declaring class".  Note that the first Method
     * object passed for a given name and descriptor identifies the
     * Method object (and thus the declaring class) that will be
     * passed to the invocation handler's "invoke" method for a given
     * set of duplicate methods.
     */
    private void addProxyMethod(Method m, Class<?> fromClass) {
        String name = m.getName();
        Class<?>[] parameterTypes = m.getParameterTypes();
        Class<?> returnType = m.getReturnType();
        Class<?>[] exceptionTypes = m.getExceptionTypes();

        String sig = name + getParameterDescriptors(parameterTypes);
        List<ProxyMethod> sigmethods = proxyMethods.get(sig);
        if (sigmethods != null) {
            for (ProxyMethod pm : sigmethods) {
                if (returnType == pm.returnType) {
                    /*
                     * Found a match: reduce exception types to the
                     * greatest set of exceptions that can thrown
                     * compatibly with the throws clauses of both
                     * overridden methods.
                     */
                    List<Class<?>> legalExceptions = new ArrayList<>();
                    collectCompatibleTypes(
                        exceptionTypes, pm.exceptionTypes, legalExceptions);
                    collectCompatibleTypes(
                        pm.exceptionTypes, exceptionTypes, legalExceptions);
                    pm.exceptionTypes = new Class<?>[legalExceptions.size()];
                    pm.exceptionTypes =
                        legalExceptions.toArray(pm.exceptionTypes);
                    return;
                }
            }
        } else {
            sigmethods = new ArrayList<>(3);
            proxyMethods.put(sig, sigmethods);
        }
        sigmethods.add(new ProxyMethod(name, parameterTypes, returnType,
                                       exceptionTypes, fromClass));
    }
```





```java
    /**
     * Generate the constructor method for the proxy class.
     */
    private MethodInfo generateConstructor() throws IOException {
        MethodInfo minfo = new MethodInfo(
            "<init>", "(Ljava/lang/reflect/InvocationHandler;)V",
            ACC_PUBLIC);

        DataOutputStream out = new DataOutputStream(minfo.code);

        code_aload(0, out);

        code_aload(1, out);

        out.writeByte(opc_invokespecial);
        out.writeShort(cp.getMethodRef(
            superclassName,
            "<init>", "(Ljava/lang/reflect/InvocationHandler;)V"));

        out.writeByte(opc_return);

        minfo.maxStack = 10;
        minfo.maxLocals = 2;
        minfo.declaredExceptions = new short[0];

        return minfo;
    }
```



```java
       /**
         * Return a MethodInfo object for this method, including generating
         * the code and exception table entry.
         */
        private MethodInfo generateMethod() throws IOException {
            String desc = getMethodDescriptor(parameterTypes, returnType);
            MethodInfo minfo = new MethodInfo(methodName, desc,
                ACC_PUBLIC | ACC_FINAL);

            int[] parameterSlot = new int[parameterTypes.length];
            int nextSlot = 1;
            for (int i = 0; i < parameterSlot.length; i++) {
                parameterSlot[i] = nextSlot;
                nextSlot += getWordsPerType(parameterTypes[i]);
            }
            int localSlot0 = nextSlot;
            short pc, tryBegin = 0, tryEnd;

            DataOutputStream out = new DataOutputStream(minfo.code);

            code_aload(0, out);

            out.writeByte(opc_getfield);
            out.writeShort(cp.getFieldRef(
                superclassName,
                handlerFieldName, "Ljava/lang/reflect/InvocationHandler;"));

            code_aload(0, out);

            out.writeByte(opc_getstatic);
            out.writeShort(cp.getFieldRef(
                dotToSlash(className),
                methodFieldName, "Ljava/lang/reflect/Method;"));

            if (parameterTypes.length > 0) {

                code_ipush(parameterTypes.length, out);

                out.writeByte(opc_anewarray);
                out.writeShort(cp.getClass("java/lang/Object"));

                for (int i = 0; i < parameterTypes.length; i++) {

                    out.writeByte(opc_dup);

                    code_ipush(i, out);

                    codeWrapArgument(parameterTypes[i], parameterSlot[i], out);

                    out.writeByte(opc_aastore);
                }
            } else {

                out.writeByte(opc_aconst_null);
            }

            out.writeByte(opc_invokeinterface);
            out.writeShort(cp.getInterfaceMethodRef(
                "java/lang/reflect/InvocationHandler",
                "invoke",
                "(Ljava/lang/Object;Ljava/lang/reflect/Method;" +
                    "[Ljava/lang/Object;)Ljava/lang/Object;"));
            out.writeByte(4);
            out.writeByte(0);

            if (returnType == void.class) {

                out.writeByte(opc_pop);

                out.writeByte(opc_return);

            } else {

                codeUnwrapReturnValue(returnType, out);
            }

            tryEnd = pc = (short) minfo.code.size();

            List<Class<?>> catchList = computeUniqueCatchList(exceptionTypes);
            if (catchList.size() > 0) {

                for (Class<?> ex : catchList) {
                    minfo.exceptionTable.add(new ExceptionTableEntry(
                        tryBegin, tryEnd, pc,
                        cp.getClass(dotToSlash(ex.getName()))));
                }

                out.writeByte(opc_athrow);

                pc = (short) minfo.code.size();

                minfo.exceptionTable.add(new ExceptionTableEntry(
                    tryBegin, tryEnd, pc, cp.getClass("java/lang/Throwable")));

                code_astore(localSlot0, out);

                out.writeByte(opc_new);
                out.writeShort(cp.getClass(
                    "java/lang/reflect/UndeclaredThrowableException"));

                out.writeByte(opc_dup);

                code_aload(localSlot0, out);

                out.writeByte(opc_invokespecial);

                out.writeShort(cp.getMethodRef(
                    "java/lang/reflect/UndeclaredThrowableException",
                    "<init>", "(Ljava/lang/Throwable;)V"));

                out.writeByte(opc_athrow);
            }

            if (minfo.code.size() > 65535) {
                throw new IllegalArgumentException("code size limit exceeded");
            }

            minfo.maxStack = 10;
            minfo.maxLocals = (short) (localSlot0 + 1);
            minfo.declaredExceptions = new short[exceptionTypes.length];
            for (int i = 0; i < exceptionTypes.length; i++) {
                minfo.declaredExceptions[i] = cp.getClass(
                    dotToSlash(exceptionTypes[i].getName()));
            }

            return minfo;
        }
```











