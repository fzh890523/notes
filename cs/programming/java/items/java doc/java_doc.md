





# tutorial



## ref method

* `{@link package.class#member label}`

  如：

  ```java
  {@link #getComponentAt(int, int) getComponentAt}
  ```
  ​





### 以下来自[how-to-reference-a-method-in-javadoc](https://stackoverflow.com/questions/5915992/how-to-reference-a-method-in-javadoc)



The general format, from the [@link section of the javadoc documentation](http://docs.oracle.com/javase/8/docs/technotes/tools/windows/javadoc.html#CHDDIECH), is:

[![{@link package.class#member label}](https://i.stack.imgur.com/HGVDj.png)](https://i.stack.imgur.com/HGVDj.png)

#### Examples

**Method in the same class:**

```
/** See also {@link #myMethod(String)}. */
void foo() { ... }
```

**Method in a different class,** either in the same package or imported:

```
/** See also {@link MyOtherClass#myMethod(String)}. */
void foo() { ... }
```

**Method in a different package** and not imported:

```
/** See also {@link com.mypackage.YetAnotherClass#myMethod(String)}. */
void foo() { ... }
```

**Label linked to method, in plain text** rather than code font:

```
/** See also this {@linkplain #myMethod(String) implementation}. */
void foo() { ... }
```

**A chain of method calls,** as in your question. We have to specify labels for the links to methods outside this class, or we get `getFoo().Foo.getBar().Bar.getBaz()`. But these labels can be fragile; see "Labels" below.

```
/**
 * A convenience method, equivalent to 
 * {@link #getFoo()}.{@link Foo#getBar() getBar()}.{@link Bar#getBaz() getBaz()}.
 * @return baz
 */
public Baz fooBarBaz()
```

#### Labels

**Automated refactoring may not affect labels.** This includes renaming the method, class or package; and changing the method signature.

**Therefore, provide a label only if you want different text than the default.**

For example, you might link from human language to code:

```
/** You can also {@linkplain #getFoo() get the current foo}. */
void setFoo( Foo foo ) { ... }
```

Or you might link from a code sample with text different than the default, as shown above under "A chain of method calls." However, this can be fragile while APIs are evolving.

#### Type erasure and #member

If the method signature includes parameterized types, use the erasure of those types in the javadoc @link. For example:

```
int bar( Collection<Integer> receiver ) { ... }

/** See also {@link #bar(Collection)}. */
void foo() { ... }
```





## ref method parameter

* `<code>foo</code>`

* `{@code foo}`

* `{@code Iterator<String>}` or `<code>Iterator&lt;String&gt;</code>` 

  显然前者更好















