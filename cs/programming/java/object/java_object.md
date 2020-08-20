

Ref:

* [HotSpot和OpenJDK入门](http://www.infoq.com/cn/articles/Introduction-to-HotSpot/)





## 

# 对象结构



对象：

- 对象头

  - mark
  - klass

- 对象体

  字段...









## oop



```
堆中的任何Java对象都是由一个普通的对象指针（OOP）表示的。在C/C++中一个OOP是一个真正的指针——一个指向Java堆里面某个内存位置的机器字。在JVM进程的虚拟地址空间中，会为Java堆分配一个单独的连续的地址范围，然后用户空间中的这块内存就会完全由JVM进程自己管理，直到JVM因为某些原因需要调整堆大小为止。

这意味着Java对象的创建和收集并不会牵扯到分配和释放内存的系统调用。

一个OOP由两个机器字头组成，它们被称为Mark和Klass字，之后是这个实例的成员字段。对于数组而言，在成员字段之前还有一个额外的字头——数组的长度。
```

这段描述的太混乱。

目前的理解是：

> <del>oop -> object，在Hotspot上实现为指针而不是引用（再包含一个指针）。</del>
>
> `*oop ` -> oop



### oop in Hotspot

HotSpot源中相关的OOP类型层次非常复杂。这些类型被保存在hotspot/src/share/vm/oops中，包括：

- oop (抽象基础)
- instanceOop (实例对象)
- methodOop (方法表示)
- arrayOop (数组抽象基础)
- symbolOop (内部符号/字符串类)
- klassOop
- markOop



参考：

```cpp
// oopDesc is the top baseclass for objects classes.  The {name}Desc classes describe
// the format of Java objects so the fields can be accessed from C++.
// oopDesc is abstract.
// (see oopHierarchy for complete oop class hierarchy)
//
// no virtual functions allowed

class oopDesc {
  friend class VMStructs;
 private:
  volatile markOop  _mark;
  union _metadata {
    Klass*      _klass;
    narrowKlass _compressed_klass;
  } _metadata;
```



```cpp
typedef class oopDesc*                            oop;
typedef class   instanceOopDesc*            instanceOop;
typedef class   arrayOopDesc*                    arrayOop;
typedef class     objArrayOopDesc*            objArrayOop;
typedef class     typeArrayOopDesc*            typeArrayOop;


// The metadata hierarchy is separate from the oop hierarchy

//      class MetaspaceObj
class   ConstMethod;
class   ConstantPoolCache;
class   MethodData;
//      class Metadata
class   Method;
class   ConstantPool;
//      class CHeapObj
class   CompiledICHolder;


// The klass hierarchy is separate from the oop hierarchy.

class Klass;
class   InstanceKlass;
class     InstanceMirrorKlass;
class     InstanceClassLoaderKlass;
class     InstanceRefKlass;
class   ArrayKlass;
class     ObjArrayKlass;
class     TypeArrayKlass;
```



## klass



Klass字是OOP头中最重要的部分之一。它是指向这个类元数据的指针（它由一个称为KlassOOP的C++类型表示）。在这些元数据当中最重要的是这个类的方法，它们被表示为一个C++虚拟方法表（一个“vtable”）。

我们并不想让所有的实例都携带着方法的所有细节，因为这样做效率会非常低，所以使用了一个vtable在实例之间共享这些信息。

需要注意的是，KlassOOP和类加载操作所产生的类对象是不同的。这两者之间的区别可以概括为下面两个方面：

- Class对象（例如String.class）仅仅是普通的Java对象——它们和任何其他的Java对象（实例OOP）一样都是OOP，和所有其他的对象那样拥有同样的行为，同时它们也能够被放入Java变量中。
- KlassOOP是类元数据的JVM表示——它们通过一个vtable结构携带类的方法信息。我们不能直接从Java代码中获得到KlassOOP的引用——它们存在于堆的Permgen区域。

记住这个区别最容易的方式是，将KlassOOP当作是类对象的JVM级别的“镜像”。



```cpp
//
// A Klass provides:
//  1: language level class object (method dictionary etc.)
//  2: provide vm dispatch behavior for the object
// Both functions are combined into one C++ class.

// One reason for the oop/klass dichotomy in the implementation is
// that we don't want a C++ vtbl pointer in every object.  Thus,
// normal oops don't have any virtual functions.  Instead, they
// forward all "virtual" functions to their klass, which does have
// a vtbl and does the C++ dispatch depending on the object's
// actual type.  (See oop.inline.hpp for some of the forwarding code.)
// ALL FUNCTIONS IMPLEMENTING THIS DISPATCH ARE PREFIXED WITH "oop_"!

//  Klass layout:
//    [C++ vtbl ptr  ] (contained in Metadata)
//    [layout_helper ]
//    [super_check_offset   ] for fast subtype checks
//    [name          ]
//    [secondary_super_cache] for fast subtype checks
//    [secondary_supers     ] array of 2ndary supertypes
//    [primary_supers 0]
//    [primary_supers 1]
//    [primary_supers 2]
//    ...
//    [primary_supers 7]
//    [java_mirror   ]
//    [super         ]
//    [subklass      ] first subclass
//    [next_sibling  ] link to chain additional subklasses
//    [next_link     ]
//    [class_loader_data]
//    [modifier_flags]
//    [access_flags  ]
//    [last_biased_lock_bulk_revocation_time] (64 bits)
//    [prototype_header]
//    [biased_lock_revocation_count]
//    [_modified_oops]
//    [_accumulated_modified_oops]
//    [trace_id]

class Klass : public Metadata {
  friend class VMStructs;
```





## mark













# 对象状态



对象的状态可能会是：

- 未锁定
- 偏向的
- 轻量级锁定
- 重量级锁定
- 标记的（仅在垃圾收集期间有效）

























