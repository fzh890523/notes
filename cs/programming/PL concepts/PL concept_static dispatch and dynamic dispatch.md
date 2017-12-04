

## static dispatch

wikipedia：

>  the implementation of a polymorphic operation is selected at [compile-time](https://en.wikipedia.org/wiki/Compile-time).
>
> > \#yonka\# 
> >
> > 1. 首先，重载也是一种多态，所以编译时确定重载方法中的一个（实现） -> 静态分派
> > 2. 也许，`Son son = new Son();  son.doIt();` 这时也明确了方法是 `Son.doIt` 而非 `Parent.doIt`，也许也算是编译时确定实现（= =)





## dynamic dispatch



Wikipedia:

> **dynamic dispatch** is the process of selecting which implementation of a [polymorphic](https://en.wikipedia.org/wiki/Subtyping) operation ([method](https://en.wikipedia.org/wiki/Method_(computer_programming)) or function) to call at [run time](https://en.wikipedia.org/wiki/Run_time_(program_lifecycle_phase)).
>
> The purpose of dynamic dispatch is to support cases where the appropriate implementation of a polymorphic operation cannot be determined at compile time because it depends on the runtime type of one or more actual parameters to the operation.
>
> > 需要依赖运行时类型确定实现的多态 -> 多态中的一种



### 动态多分派



#### 动态多分派的意义



```java
class Fruit {}
class Apple extends Fruit {}

interface FruitStore {
  void buy(Fruit fruit);
}

class AppleStore implements FruitStore {
  void buy(Fruit fruit) {
    // ...
  }
  void buy(Apple apple) {
    // ...
  }
}

void test {
  FruitStore store = new AppleStore();
  Apple apple = new Apple();
  store.buy(apple);  // FruitStore.buy(LFruit;)V -> AppleStore.buy(LFruit;)V (and if supports multiple dynamic dispatch/polymorphism) -> AppleStore.buy(LApple;)V
}
```







## value dispatching



```java
enum Color {
  RED,
  BLACK,
  WHITE;
}

void useColor(Color@RED) { // or void useColor(Color@RED color) {
  // ...
}

void useColor(Color@BLACK) {
  // ...
}

void useColor(Color defaultColor) {
  // ...
}
```



## 概念： vs binding



### 探索



```
Dynamic binding is another name for Late Binding. That's where the language ties a data element or method to an object after compilation time.

Wikipedia equates dynamic & late binding:

Dynamic binding (computing), also known as late binding
links to

http://en.wikipedia.org/wiki/Dynamic_binding_(computing)

Javascript was my first exposure to that, because you can just drop functions into objects willy nilly and do cool things with them.

For example (untested, not guaranteed to work exactly):

var a = Object();
var do = function() { do something };
a.do = do;
a.do();
// neato!
BTW, on a side note, there is some question whether Java is Object-oriented, because one of the original ideas in OO was late binding. Unfortunately, this particular discussion seems to devolve into attempts to define "Object Oriented".

http://c2.com/cgi/wiki?IsJavaObjectOriented

lol!

Under the covers, Dynamic Dispatch and Dynamic Binding may work out the same. But the idea in dynamic dispatch is following some function pointer to see which method to actually invoke, or object to invoke it on. "Binding" is the idea that the method is "bound" to a particular instance (or class of instances) & that's how you identify it.

So they could work together -- a method that's bound to an object with dynamic binding might use dynamic dispatch when you call it.

...

Also dynamic dispatch has more of an OO flavor to it... it's the mechanism behind polymorphism, in which a reference to an object might point to one of multiple implementations. Dynamic dispatch decides at runtime which one to actually run. By contrast, late binding would be dropping in whole new methods that weren't there at compile time.
```

一家之见，参考一下，核心观点：

* dynamic dispatch is following some function pointer to see which method to actually invoke, or object to invoke it on
* "Binding" is the idea that the method is "bound" to a particular instance (or class of instances) & that's how you identify it



评论里有人回复：

```
How would you describe the situation (assume Java or .NET) where 
* a base type includes Foo(Animal), 
* derived type overrides Foo(Animal) but also defines Foo(Cat), 
* baseTypeReference identifies a derived-type instance, 
* client code calls baseTypeReference(someCat)? 
I would describe that as using static binding but dynamic dispatch. 
Would you agree with such terminology?
```

对应代码：

```java
public class TestDispatchAndBind {
	static class Parent {
		public void foo(Number number) {
			System.out.println("parent.Foo(Number)");
		}
	}

	static class Son extends Parent {
	    public void foo(Number number) {
			System.out.println("Son.Foo(number)");
		}
		public void foo(Integer integer) {
			System.out.println("Son.Foo(Integer)");
		}
	}

	public static void main(String[] args) {
		Parent p = new Son();
		Integer i = 1;
         p.foo(i);  // 明显是输出： Son.Foo(number)
         // 字节码： INVOKEVIRTUAL TestDispatchAndBind$Parent.foo (Ljava/lang/Number;)V
         // 根据静态类型 “Parent” 确定调用的是 Parent.foo (Ljava/lang/Number;)V
         // 运行时根据receiver动态dispatch到 Son.foo (Ljava/lang/Number;)V
	}
}
```

**周志明的书**里写到： `所有依赖静态类型来定位方法执行版本的分派执行的分派动作 称为 静态分派`。

<del>

当时就觉得哪里不对，经这里一说，感觉以下认识更为合理：

* 根据静态类型（Parent）确定（也是前面提到的`identify`）方法的执行版本： 静态绑定

  所以是根据Parent而不是实例的真正类型Son

* 根据invokevirtual指示，根据动态类型（receiver类型）来定位方法执行版本： 动态分派

</del>



不过问题又来了，这么理解的话，那 `动态绑定` 和 `静态分派` 是啥？



```
A fairly decent answer to this is actually incorporated into a question on late vs. early binding on [programmers.stackexchange.com](https://softwareengineering.stackexchange.com/a/200123).  # 就是下面JustinC的回答

In short, late binding refers to the object-side of an eval, dynamic dispatch refers to the functional-side. In late binding the type of a variable is the variant at runtime. In dynamic-dispatch, the function or subroutine being executed is the variant.

In C++, we don't really have late binding because the type is known (not necessarily the end of the inheritance hierarchy, but at least a formal base class or interface). But we do have dynamic dispatch via virtual methods and polymorphism.

The best example I can offer for late-binding is the untyped "object" in Visual Basic. The runtime environment does all the late-binding heavy lifting for you.

Dim obj

- initialize object then..
obj.DoSomething()
The compiler will actually code the appropriate execution context for the runtime-engine to perform a named lookup of the method called DoSomething, and if discovered with the properly matching parameters, actually execute the underlying call. In reality, something about the type of the object is known (it inherits from IDispatch and supports GetIDsOfNames(), etc). but as far as the language is concerned the type of the variable is utterly unknown at compile time, and it has no idea if DoSomething is even a method for whatever obj actually is until runtime reaches the point of execution.

I won't bother dumping a C++ virtual interface et'al, as I'm confident you already know what they look like. I hope it is obvious that the C++ language simply can't do this. It is strongly-typed. It can (and does, obviously) do dynamic dispatch via the polymorphic virtual method feature.

-- from https://stackoverflow.com/questions/20187587/what-is-the-difference-between-dynamic-dispatch-and-late-binding-in-c
-- by WhozCraig
```





```
There are two major concepts in confusion: binding and loading. It is conflated by the concept of DataBinding, which is somewhere in the middle often doing both. After considering it, I am going to add one more concept, to complete the trifecta, dispatch.

Types

Late Binding: type is unknown until the variable is exercised during run-time; usually through assignment but there are other means to coerce a type; dynamically typed languages call this an underlying feature, but many statically typed languages have some method of achieving late binding

Implemented often using [special] dynamic types, introspection/reflection, flags and compiler options, or through virtual methods by borrowing and extending dynamic dispatch

Early Binding: type is known before the variable is exercised during run-time, usually through a static, declarative means

Implemented often using standard primitive types

Functions

Static Dispatch: known, specific function or subroutine at compile time; it is unambiguous and matched by the signature

Implemented as static functions; no method can have the same signature

Dynamic Dispatch: not a specific function or subroutine at compile time; determined by the context during execution. There are two different approaches to "dynamic dispatch," distinguished by what contextual information is used to select the appropriate function implementation.

In single [dynamic] dispatch, only the type of the instance is used to determine the appropriate function implementation. In statically-typed languages, what this means in practice is that the instance type decides which method implementation is used irrespective of the reference type indicated when the variable is declared/assigned. Because only a single type -- the type of the object instance -- is used to infer the appropriate implementation, this approach is called "single dispatch".

There is also multiple [dynamic] dispatch, where input parameter types also help determine which function implementation to call. Because multiple types -- both the type of the instance and the type(s) of the parameter(s) -- influence which method implementation is selected, this approach is dubbed "multiple dispatch".

Implemented as virtual or abstract functions; other clues include overridden, hidden, or shadowed methods.

NB: Whether or not method overloading involves dynamic dispatch is language-specific. For example, in Java, overloaded methods are statically dispatched.

Values

Lazy Loading: object initialization strategy that defers value assignment until needed; allows an object to be in an essentially valid but knowingly incomplete state and waiting until the data is needed before loading it; often found particularly useful for loading large datasets or waiting on external resources

Implemented often by purposefully not loading a collection or list into a composite object during the constructor or initialization calls until some downstream caller asks to see the contents of that collection (eg. get_value_at, get_all_as, etc). Variations include loading meta information about the collection (like size or keys), but omitting the actual data; also provides a mechanism to some runtimes to provide developers with a fairly safe and efficient singleton implementation scheme

Eager Loading: object initialization strategy that immediately performs all value assignments in order to have all the data needed to be complete before considering itself to be in a valid state.

Implemented often by providing a composite objects with all their known data as soon as possible, like during a constructor call or initialization

Data Binding: often involves creating an active link or map between two compatible information streams so that changes to one are reflected back into the other and vice versa; in order to be compatible they often have to have a common base type, or interface

Implemented often as an attempt to provide cleaner, consistent synchronization between different application aspects (eg view-model to view, model to controller, etc.) and talks about concepts like source and target, endpoints, bind/unbind, update, and events like on_bind, on_property_change, on_explicit, on_out_of_scope

EDIT NOTE: Last major edit to provide description of examples of how these often occur. Particular code examples depend entirely on the implementation/runtime/platform

-- from https://softwareengineering.stackexchange.com/questions/200115/what-is-early-and-late-binding/200123#200123
-- by JustinC
```

这回答里解释了三个概念：

* binding - type

  确定类型

  * late

    *type is unknown* until the variable is exercised during run-time

    一般是通过*赋值*，也有其他方式

    常见于动态语言语言，静态语言也可以通过一些方式做到。

    实现上：

    * 动态类型
    * 自省/反射
    * 标志位、编译选项
    * 虚方法（借用/扩展动态分派）

  * early

    *type is known* before the variable is exercised during run-time

    一般是通过 *声明*

    实现上： 常使用标准的基本类型

* dispatch - function

  * static

    *known, specific function* or subroutine at compile time

    明确的、通过函数签名确定

    实现上：

    * 静态函数
    * 签名唯一性

  * dynamic

    *not a specific function* or subroutine at compile time; determined by the context during execution

    实现上：

    * 虚函数、抽象函数
    * 复写（overriden）、hidden、shadowed

    > 重载是否涉及动态分派要看具体实现，具体java里重载就是静态分派的

    根据使用什么样的上下文信息（来分派 - 选择对应的函数实现），分为 单（动态）分派 和 多（动态）分派

  此外，还有：

  * single

    只根据单一条件来dispatch，一般为*类型*

  * multiple

    根据多条件来dispatch，一般为 *类型*、*参数* （返回值...）

* loading - value

  * lazy
  * eager





```
The [link](http://en.wikipedia.org/wiki/Dynamic_dispatch) itself explained the difference:

Dynamic dispatch is different from late binding (also known as dynamic binding). In the context of selecting an operation, binding refers to the process of associating a name with an operation. Dispatching refers to choosing an implementation for the operation after you have decided which operation a name refers to.

and

With dynamic dispatch, the name may be bound to a polymorphic operation at compile time, but the implementation not be chosen until runtime (this is how dynamic dispatch works in C++). However, late binding does imply dynamic dispatching since you cannot choose which implementation of a polymorphic operation to select until you have selected the operation that the name refers to.
But they're mostly equal in C++ you can do a dynamic dispatch by virtual functions and vtables.

C++ uses early binding and offers both dynamic and static dispatch. The default form of dispatch is static. To get dynamic dispatch you must declare a method as virtual.

-- from https://stackoverflow.com/questions/20187587/what-is-the-difference-between-dynamic-dispatch-and-late-binding-in-c
-- by deepmax
```

* binding

   refers to the process of associating a name with an operation

  做什么？

  * late binding

    late binding does imply dynamic dispatching since you cannot choose which implementation of a polymorphic operation to select until you have selected the operation that the name refers to.

* dispatching

  refers to choosing an implementation for the operation after you have decided which operation a name refers to

  怎么做？ （哪种做法/实现？）

  * dynamic dispatch

    the name may be bound to a polymorphic operation at compile time, but the implementation not be chosen until runtime (this is how dynamic dispatch works in C++)

  ​



C++

* binding
  * early binding

* dispatch

  * dynamic  dispatch

    virtual

  * static dispatch

    default



```
On the other hand, my understanding of the term late binding is that the function pointer is looked up by name at runtime, from a hash table or something similar. This is the way things are done in Python, JavaScript and (if memory serves) Objective-C. This makes it possible to add new methods to a class at run-time, which cannot directly be done in C++. This is particularly useful for implementing things like mixins. However, the downside is that the run-time lookup is generally considerably slower than even a virtual call in C++, and the compiler is not able to perform any compile-time type checking for the newly-added methods.
```

观点：

* late binding: the function pointer is looked up by name at runtime, from a hash table or something similar



```
In C++, both dynamic dispatch and late binding is the same. Basically, the value of a single object determines the piece of code invoked at runtime. In languages like C++ and java dynamic dispatch is more specifically dynamic single dispatch which works as mentioned above. In this case, since the binding occurs at runtime, it is also called late binding. Languages like smalltalk allow dynamic multiple dispatch in which the runtime method is chosen at runtime based on the identities or values of more than one object.

In C++ we dont really have late binding, because the type information is known. Thus in the C++ or Java context, dynamic dispatch and late binding are the same. Actual/fully late binding, I think is in languages like python which is a method-based lookup rather than type based.
```



```cpp
#include <iostream>

struct Foo {
   virtual ~Foo() {}
};

struct FooOne : public Foo {};

struct Bar {
   virtual ~Bar() {}
   virtual void dispatch (const Foo &) {
      std::cout << "Bar::Dispatch(const Foo &)\n";
   }
};

struct BarOne : public Bar {
   using Bar::dispatch;
   virtual void dispatch (const Foo &) {
      std::cout << "BarOne::Dispatch(const Foo &)\n";
   }
   virtual void dispatch (const FooOne &) {
      std::cout << "BarOne::Dispatch(const FooOne &)\n";
   }
};

void process (Bar & bar, const Foo & foo) {
   bar.dispatch (foo);
}

int main () {
   Foo foo;
   Bar bar;
   FooOne foo_one;
   BarOne bar_one;

   process (bar, foo);  // Bar::Dispatch(const Foo &)
   process (bar, foo_one);  // Bar::Dispatch(const Foo &)

   process (bar_one, foo);  // BarOne::Dispatch(const Foo &)
   process (bar_one, foo_one);  // BarOne::Dispatch(const Foo &)

   bar_one.dispatch (foo_one);  // BarOne::Dispatch(const FooOne &)
   // static dispatch

   return 0;
}
```





### 小结



* binding指name -> target(data/operation)的过程，接近于java的resolve，确定引用/指向的目标

* dispatch指根据实现确定实际执行的内容的过程，主要针对函数，也主要应用于多态场景

  显然，对于resolved的method，不是都需要再做dispatch的



* `In most [dynamically-typed](https://en.wikipedia.org/wiki/Type_system#Dynamic_type_checking_and_runtime_type_information) languages, the list of methods on an object can be altered at runtime. This requires late binding.`


* `late binding does imply dynamic dispatching since you cannot choose which implementation of a polymorphic operation to select until you have selected the operation that the name refers to`

* `While dynamic dispatch does not imply late binding, late binding does imply dynamic dispatching since the binding is what determines the set of available dispatches.`

  比如动态语言如py，runtime才明确类型，同时方法也依赖runtime的lookup...

* `In the context of selecting an operation, [binding](https://en.wikipedia.org/wiki/Name_binding)associates a name to an operation. Dispatching chooses an implementation for the operation after you have decided which operation a name refers to. `













#### 各语言的 binding & dispatch 小结



* java
  * early binding

  * static (multiple) dispatch - overloading / dynamic (single) dispatch - overriding/polymorphism

  * 迂回实现动态多分派

    ```java
    interface I {
      void do(I i);
      void do(A a);
      void do(B b);
    }

    class A implements I {
      void do(I i) {
        i.do(this);
      }  
      void do(A a) {  
        // ...
      }
      void do(B b) {
        // ...
      }
    }
    class B implements I {
      void do(I i) {
        i.do(this);
      } 
      void do(A a) {  
        // ...
      }
      void do(B b) {
        // ...
      }  
    }

    void test() {
      I a = new A();
      I b = new B();
      a.do(b);  // I.do(LI;)V -> A.do(LI;)V -> I.do(LA;)V -> B.do(LA;)V
      // 但这样就反过来啊，囧
    }
    ```

    ​

    ```java
    	interface I {
    		default void doIt(I i) {
    			if (i instanceof A) {
    				doIt((A) i);
    			} else if (i instanceof B) {
    				doIt((B) i);
    			}
    		}

    		void doIt(A a);

    		void doIt(B b);
    	}

    	static class A implements I {
    		public void doIt(A a) {
    			System.out.println("A.doIt(A)");
    		}

    		public void doIt(B b) {
    			System.out.println("A.doIt(B)");
    		}
    	}

    	static class B implements I {
    		@Override
    		public void doIt(A a) {
    			System.out.println("B.doIt(A)");
    		}

    		@Override
    		public void doIt(B b) {
    			System.out.println("B.doIt(B)");
    		}
    	}

    	public static void main(String[] args) {
    		I a = new A();
    		I b = new B();
    		a.doIt(b);
        }
    ```

* c++

  * early binding
  * static/dynamic dispatch

* python

  * late binding
  * dynamic (single) dispatch

* c

  * early binding

  * static single dispatch

    有dispatch概念？ = =













