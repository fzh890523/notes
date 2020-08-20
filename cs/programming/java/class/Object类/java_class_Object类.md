

# impl



## clone



### ref



#### [zhihu问题-java clone浅拷贝 调用super.clone()为什么能够转型为当前类型？-R大回答](https://www.zhihu.com/question/63667745/answer/224396818)



因为 Object.clone() 是一个非常特殊的虚方法。请仔细阅读它的JavaDoc：[java.lang.Object.clone()](https://link.zhihu.com/?target=https%3A//docs.oracle.com/javase/8/docs/api/java/lang/Object.html%23clone--)

其中的这段：

> The method clone for class Object performs a specific cloning operation. First, if the class of this object does not implement the interface Cloneable, then a CloneNotSupportedException is thrown. Note that all arrays are considered to implement the interface Cloneable and that the return type of the clone method of an array type T[] is T[] where T is any reference or primitive type. Otherwise, **this method creates a new instance of the class of this object and initializes all its fields with exactly the contents of the corresponding fields of this object, as if by assignment; the contents of the fields are not themselves cloned.** Thus, this method performs a "shallow copy" of this object, not a "deep copy" operation.

也就是说JavaDoc指明了 Object.clone() 有特殊的语义，它就是能把当前对象的整个结构完全浅拷贝一份出来。

每层 clone() 都顺着 super.clone() 的链向上调用的话最终就会来到 Object.clone() ，于是根据上述的特殊语义就可以有 x.clone().getClass() == x.getClass() 。

> By convention, the returned object should be obtained by calling super.clone. If a class and all of its superclasses (except Object) obey this convention, it will be the case that x.clone().getClass() == x.getClass().

至于这是如何实现的，其实很简单。可以把JVM原生实现的 Object.clone() 的语义想像为拿到 this 引用之后通过反射去找到该对象实例的所有字段，然后逐一字段拷贝。

HotSpot VM中，Object.clone() 在不同的优化层级上有不同的实现。其中最不优化的版本是这样做的：拿到 this 引用，通过对象头里记录的Klass信息去找出这个对象有多大，然后直接分配一个新的同样大的空对象并且把Klass信息塞进对象头（这样就已经实现了 x.clone().getClass() == x.getClass() 这部分语义），然后直接把对象体的内容看作数组拷贝一样从源对象“盲”拷贝到目标对象，bitwise copy。然后就完事啦。























