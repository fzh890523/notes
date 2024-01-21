# question



Comparable<? super T> e = (Comparable<? super T>)elemet用extends会报错相关问题：[Java 泛型  中 super 怎么 理解？与 extends 有何不同？](https://www.zhihu.com/question/20400700)



> TODO
>
> yonka:
>
> 没懂为什么不可以，实际测试可以声明啊。。。 = =
>
> ```java
>     static class A {}
>
>     static class F implements Comparable<A> {
>         @Override
>         public int compareTo(A o) {
>             return 0;
>         }
>     }
>
>     public static void main(String[] args) {
>         Comparable<? super A> e = new F();
>         Comparable<? extends A> e1 = new F();
>     }
> ```



# answer: R大



请使用PECS原则来判断这种问题。

producer-extends，consumer-super。意思是说如果泛型参数只出现在返回值位置（produce值），那么这个泛型参数可以声明为协变（covariant）的（extends）；如果泛型参数只出现在参数位置（consume值），那么这个泛型参数可以声明为逆变（contravariant）的（super）。如果一个泛型参数同时出现在了返回值和参数位置上，则它不可以参与协变，于是是不可变（invariant）的，此时extends和super都不可以用。

`Comparable<T>`接口上唯一的方法是：

```
int compareTo(T o)
```

这里泛型参数T只出现在了参数位置上，所以如果我们想声明一个 Comparable<? super Foo> 这样的泛型实例是OK的。

至于PECS原则背后的原理，通俗来说就是八字箴言：宽于律人，严于律己。

“人”就是参数，“己”就是返回值。



# Ref: [What is PECS (Producer Extends Consumer Super)?](https://stackoverflow.com/questions/2723397/what-is-pecs-producer-extends-consumer-super)



**tl;dr:** "PECS" is from the collection's point of view. If you are *only* pulling items from a generic collection, it is a producer and you should use `extends`; if you are *only* stuffing items in, it is a consumer and you should use `super`. If you do both with the same collection, you shouldn't use either `extends` or `super`.

------

Suppose you have a method that takes as its parameter a collection of things, but you want it to be more flexible than just accepting a `Collection<Thing>`.

**Case 1: You want to go through the collection and do things with each item.**
Then the list is a **producer**, so you should use a `Collection<? extends Thing>`.

The reasoning is that a `Collection<? extends Thing>` could hold any subtype of `Thing`, and thus each element will behave as a `Thing` when you perform your operation. (You actually cannot add anything to a `Collection<? extends Thing>`, because you cannot know at runtime which *specific*subtype of `Thing` the collection holds.)

**Case 2: You want to add things to the collection.**
Then the list is a **consumer**, so you should use a `Collection<? super Thing>`.

The reasoning here is that unlike `Collection<? extends Thing>`, `Collection<? super Thing>` can always hold a `Thing` no matter what the actual parameterized type is. Here you don't care what is already in the list as long as it will allow a `Thing` to be added; this is what `? super Thing`guarantees.











