

# Posted on Tuesday, February 2, 2010. 

Go organizes programs into individual pieces called packages. A package gets to pick a short name for itself, like `vector`, even if the `import` statement must use a longer path like `"container/vector"` to name the file where the compiled code is installed. The early Go compilers used the package name as a unique identifier during linking, so that `vector`'s `New` function could be distinguished from `list`'s `New`. In the final binary, one was `vector.New` and the other `list.New`. As we started to fill out the standard library, it became clear that we needed to do something about managing the package name space: if multiple packages tried to be package `vector`, their symbols would collide in the linker. For a while we considered segmenting the name space, reserving lower-case names for standard packages and upper-case names for local packages. (Since package names and object file names are conventionally the same, one reason not to do this is that it would require a case-sensitive file system.)

Other languages simply use longer names. Both Java and Python tie the name to the directory in which the package is found, as in `com.java.google.WebServer` for the code in `com/java/google/WebServer.class`. In practice this leads to unnecessarily long identifiers, something Go tries to avoid. It also ties the name to a particular mechanism for finding code: a file system. One of the reasons that import paths are string constants in Go is so that it is easy to substitute other notations, like URLs.

Last spring, during a long discussion about how to divide up the package name space, Robert Griesemer cut the Gordian knot by suggesting that we allow multiple packages to choose a single name and fix the tool chain to cope. The [import statement](http://golang.org/doc/go_spec.html#Import_declarations) already allows introducing a local alias for the package during the import, so there's no linguistic reason package names have to be unique. We all agreed that this was the right approach, but we weren't sure how to implement it. Other considerations, like the open source release, took priority during most of 2009, but we recently returned to the problem.

Ultimately, the linker needs some unique name for each symbol in the program; the fundamental problem caused by deciding that package names won't be unique is to find another source of uniqueness that fits into the tool chain well.

The best approach* seems to be to use the package's import path as the unique identifier, since it must uniquely identify the package in the import statement already. Then `container/vector`'s `New` is `container/vector.New`. But! When you're compiling a package, how does the compiler know what the package's import path will be? The package statement just says `vector`, and while every compilation that imports `"container/vector"` knows the import path, the compilation of `vector` itself does not, because compilation is handled separately from installing the binary in its final, importable location.

Last week I changed the gc compiler suite to do this. My solution to the import path question was to introduce a special name syntax that refers to “this package's import path.” Because the import paths are string literals in the Go compiler metadata, I chose the empty string—`""`—as the self-reference name. Thus, in the object file for package `vector`, the local symbol `New` is written `"".New`. When the linker reads the object file, it knows what import path it used to find the file. It substitutes that path for the `""`, producing, in this case, the unique name`container/vector.New`.

Not embedding a package's final installed location in its object file makes the object files easy to move and duplicate. For example, consider this trivial package:

```
package seq
var n int
func Next() int {
    n++
    return n
}

```

It's valid for a Go program to import the same path multiple times using different local names, but all the names end up referring to the same package:

```
package main

import (
    "fmt"
    s "seq" // changed to "seq1" later
    t "seq"
)

func main() {
    fmt.Println(s.Next(), s.Next(), t.Next(), t.Next())
}

```

prints `1 2 3 4`, because it all four calls are to the same `Next` function:

```
$ 6g seq.go
$ 6g -I. main.go
$ 6l -L. main.6
$ 6.out
1 2 3 4
$ 

```

But if we change one of the imports to say `"seq1"` and then merely copy the `"seq"` binary to `"seq1"`, we've created a distinct package, using lowly `cp` instead of a compiler:

```
$ cp seq.6 seq1.6
$ ed main.go
120
/seq
 s "seq"
s/seq/seq1
 s "seq1"
wq
121
$ 6g -I. main.go
$ 6l -L. main.6
$ 6.out
1 2 1 2
$ 

```

Now the `s.Next` calls refer to `seq1.6`'s `Next`, while the `t.Next` calls refer to `seq.6`'s `Next`. Duplicating the object actually duplicated the code. This is very different from the behavior of a traditional C compiler and linker.

A digression: the explicit `"".` prefix is not strictly necessary. It would be cleaner if the linker treated every symbol as needing to be qualified by the import path, so that all the `"".` could be dropped. But occasionally it's important to be able to break the rules, for example to define a symbol that is logically in one package be implemented in another. For example, the implementation of [`unsafe.Reflect`](http://golang.org/pkg/unsafe/#Reflect) is actually in the binary for [package `runtime`](http://golang.org/pkg/runtime/), because that's where all the interface manipulation code lives:

```
$ 6nm pkg/darwin_amd64/runtime.a|grep Reflect
iface.6: T unsafe.Reflect
$

```

Another reason to use an explicit prefix is to admit names with no prefix at all, as would be generated by legacy C code. Otherwise, what should C's `printf` be in? If the linker enforced a strict boundary between packages, both of these examples would be impossible. Most of the time that would be a good thing, but systems languages do not have the luxury of stopping at “most of the time.” Last October, a few weeks before the public release of Go, I changed the linker to insert import path qualifiers on all names during linking, but it was too disruptive a change to commit before the release. Last week's implementation, which allows for semipermeable package boundaries, is a much better fit for Go.

This week Ian Lance Taylor is working on eliminating the global package name space assumption in gccgo. He'd like to avoid making changes to the linker, which rules out introducing a “this package” notation like `""`. Gccgo must be able to write objects that know their own import paths, which means gccgo must know the import path at compile time. But how? There will be a new gccgo command line option, and the build system will simply tell the compiler what the import path is.

In retrospect, I wonder if the effort of `""` in the gc tool chain was justified compared to adding an option. The gc implementation is easier to use, but it's not clear how important that will be. Time will tell.

\* An alternative approach would be to generate a [random identifier](http://en.wikipedia.org/wiki/Globally_Unique_Identifier) each time the compiler is invoked and to use it for the package compiled by that run. When other packages import the compiled package, they can read the identifier and use it to generate references to that package's symbols. The most glaring problem with this approach is that the symbol names you'd see while debugging would be ugly, like [mangled C++ names](http://en.wikipedia.org/wiki/Name_mangling) but worse. Another problem is that it would break aggressive incremental compilation: if `fmt` gets recompiled, all packages that import it would have to be recompiled to pick up the new identifier, even if the external interface hadn't changed. It would be nice to avoid those recompilations, especially in large programs.

(Comments originally posted via Blogger.)

- [Brian Slesinsky](http://www.blogger.com/profile/06578159790743176316) (February 3, 2010 9:04 AM) Thanks
   for the clear explanation! However, I have to make a few corrections 
  about Java: it isn't true that Java packages tie a name to "a particular
   mechanism for finding code: a file system."
  Java classes are 
  often loaded from jar files, which in the case of applets are loaded 
  over the network, so there's no reason why the machine running a JVM 
  need have a traditional filesystem at all. It can cache jars and .class 
  files using any convenient mechanism. In addition, Android and GWT take 
  Java source as input but don't generate .class files at all.
  I 
  think this shows the flexibility of the Java naming scheme. Java 
  transparently supports loading code over the network without changing 
  source code (in particular, import statements). It would be a mistake to
   put network identifiers into import statements because it would require
   editing source code in order to change where source code or object code
   are located. Instead, the strategy for finding source code is left to 
  the build system where it can be conveniently overridden using 
  developer-specific flags.
  Also, while there's a strong convention
   for locating Java source files in a particular directory hierarchy, 
  this isn't strictly necessary. I wrote a Java source code indexer that 
  scans a large directory tree for cross-references in Java source files, 
  and it works find regardless of where the source files are located. 
  Cross-references can be found simply by looking at the contents of Java 
  source files for package names and import statements. This isn't true in
   the scheme you describe here, which seems unfortunate.

来源： <<http://research.swtch.com/gopackage>>

 



