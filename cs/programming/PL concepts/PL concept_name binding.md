

# intro



binding

**定义**： the association of entities (data and/or code) with [identifiers](https://en.wikipedia.org/wiki/Identifier).

> An identifier bound to an object is said to [reference](https://en.wikipedia.org/wiki/Reference_(computer_science)) that object



**对于机器语言**，本身是没有（不需要）标识符的概念，不过需要为程序员实现 name-object 的binding关系（便于开发）。



**binding和scoping的关系密切**，在很多场景下作用域会决定name bind到那个object。

> \#yonka\# 不过感觉此bind跟彼bind还是不一样啊：
>
> * 局部变量的name只有**助记**作用，并没有实际的bind的过程，只是对开发者的语义层面的bind
> * 换句话说，可能感觉里，只有 resolve-and-bind 这个过程的才算严格的bind吧



## 概念关系

wikipedia里是把 eager/early 和 static、 late 和 dynamic 是完全等同的。



## 时机

```
The binding of names before the program is run is called static (also "early"); bindings performed as the program runs are dynamic (also "late" or "virtual").

--- wikipedia
```

维基里直接以**“run”**作为early/late的分割点，之前为early/static，之后则为...

这个思路显然是针对C这种机器语言的。





## late/dynamic binding



wikipedia：

> in which the method being called upon an object or the function being called with arguments is looked up by name at [runtime](https://en.wikipedia.org/wiki/Run_time_(program_lifecycle_phase)).
>
> With [early binding](https://en.wikipedia.org/wiki/Early_binding), or [static binding](https://en.wikipedia.org/wiki/Static_binding), in an object-oriented language, the compilation phase fixes all types of variables and expressions. This is usually stored in the compiled program as an offset in a [virtual method table](https://en.wikipedia.org/wiki/Virtual_method_table) ("v-table") and is very efficient. With late binding the compiler does not have enough information to verify the method even exists, let alone bind to its particular slot on the v-table. Instead the method is looked up by name at runtime.
>
> > 这么说，java倒是late binding了... 而c++不是



stackexchange上看到的一个定义： `Dynamic binding is another name for Late Binding. That's where the language ties a data element or method to an object after compilation time.`





### java的late binding



几种理解：

* 运行时resolve ref，而不像c等有静态链接。 有运行时的 name -> target 的过程，符合binding定义，也符合late定义 = =
* 作为 dynamic dispatch 的同义词
* 反射





#### php的static field binding



```php
class A {
    static $word = "hello";
    static function hello() { print self::$word; }
}

class B extends A {
    static $word = "bye";
}

B::hello();  # hello
```



```php
class A {
    static $word = "hello";
    static function hello() { print static::$word; }  # late binding for static field
    # 感觉这里的设计有点混乱，倒不如 self::$word 是 early binding 而 static::$word 是late binding
}

class B extends A {
    static $word = "bye";
}

B::hello();  # bye
```







## rebinding VS mutation

```
Rebinding should not be confused with mutation – "rebinding" is a change to the referencing identifier; "mutation" is a change to the referenced value. 

-- wikipedia
```



```java
void do() {
  A a = new A();
  a.i = 1;  // mutation
  a = null;  // rebinding
}

// 但是

class A {
  void do() {
    this.b = null;  // 对于 b 来说是rebinding； 对a来说是mutation 
    this.i = 0;  // 对于primitive来说，不知道有没有rebind的说法
  }
}
```



## binding VS dispatch



参见 [《PL concept_static dispatch and dynamic dispatch》](./PL concept_static dispatch and dynamic dispatch.md)





