





## 概念： vs binding



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

当时就觉得哪里不对，经这里一说，感觉以下认识更为合理：

* 根据静态类型（Parent）确定（也是前面提到的`identify`）方法的执行版本： 静态绑定

  所以是根据Parent而不是实例的真正类型Son

* 根据invokevirtual指示，根据动态类型（receiver类型）来定位方法执行版本： 动态分派



不过问题又来了，这么理解的话，那 `动态绑定` 和 `静态分派` 是啥？



