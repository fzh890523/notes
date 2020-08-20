
# [HotSpotVM JNI实现浅析](http://blog.csdn.net/kisimple/article/details/44204201) or [local](resource/HotSpotVM JNI实现浅析 - KISimple的专栏 - 博客频道 - CSDN.NET.html)



# 性能



ref：

* [what-makes-jni-calls-slow - stackoverflow](https://stackoverflow.com/questions/7699020/what-makes-jni-calls-slow)

  First, it's worth noting that by "slow," we're talking about something that can take tens of nanoseconds. For trivial native methods, in 2010 I measured calls at an average 40 ns on my Windows desktop, and 11 ns on my Mac desktop. Unless you're making *many* calls, you're not going to notice.

  That said, calling a native method can be *slower* than making a normal Java method call. Causes include:

  - Native methods will not be inlined by the JVM. Nor will they be just-in-time compiled for this specific machine -- they're already compiled.
  - A Java array may be copied for access in native code, and later copied back. The cost can be linear in the size of the array. I measured JNI *copying* of a 100,000 array to average about 75 microseconds on my Windows desktop, and 82 microseconds on Mac. Fortunately, direct access may be obtained via [GetPrimitiveArrayCritical](http://docs.oracle.com/javase/8/docs/technotes/guides/jni/spec/functions.html#GetPrimitiveArrayCritical_ReleasePrimitiveArrayCritical) or [NewDirectByteBuffer](http://docs.oracle.com/javase/8/docs/technotes/guides/jni/spec/functions.html#NewDirectByteBuffer).
  - If the method is passed an object, or needs to make a callback, then the native method will likely be making its own calls to the JVM. Accessing Java fields, methods and types from the native code requires something similar to reflection. Signatures are specified in strings and queried from the JVM. This is both slow *and* error-prone.
  - Java Strings are objects, have length and are encoded. Accessing or creating a string may require an O(n) copy.

  Some additional discussion, possibly dated, can be found in "Java¿ Platform Performance: Strategies and Tactics", 2000, by Steve Wilson and Jeff Kesselman, in section "9.2: Examining JNI costs". It's about a third of the way down [this page](http://www.mastercorp.free.fr/Ing1/Cours/Java/java_lesson1/doc/Tutorial/performance/JPNativeCode_fm.htm), provided in the comment by @Philip below.

  The 2009 IBM developerWorks paper ["Best practices for using the Java Native Interface"](http://www.ibm.com/developerworks/java/library/j-jni/) provides some suggestions on avoiding performance pitfalls with JNI.





















# 