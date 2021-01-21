

## Golang Internals, Part 1: Main Concepts and Project Structure

Posted by [Sergey Matyukevich](http://blog.altoros.com/author/sergey-matyukevich), Cloud Engineer at Altoros, in [Research & Development](http://blog.altoros.com/category/research-and-development)

Tags: [Golang](http://blog.altoros.com/tag/golang)

02 Mar 2015

[2 Comments](http://blog.altoros.com/golang-part-1-main-concepts-and-project-structure.html#disqus_thread)

*All parts: \**Part 1** | Part 2 | Part 3 | Part 4 | Part 5 | Part 6*
![15-02-26-Golang-Internals](https://kshttps0.wiz.cn/wiz-resource/89077880-eff4-11e0-a402-00237def97cc/232751c6-04cf-47cb-aa34-4cc690561d2b/index_files/b2caae51-5d27-4378-9bbb-6787ab52b16f.png)This
 series of blog posts is intended for those who are already familiar 
with the basics of Go and would like to get a deeper insight into its 
internals. Today’s post is dedicated to the structure of Go source code 
and some internal details of the Go compiler. After reading this, you 
should be able to answer the following questions:1. What is the structure of Go source code?
\2. How does the Go compiler work?
\3. What is the basic structure of a node tree in Go?
Getting startedWhen
 you start learning a new programming language, you can usually find a 
lot of “hello-world” tutorials, beginner guides, and books with details 
on main language concepts, syntax, and even the standard library. 
However, getting information on such things as the layout of major data 
structures that the language runtime allocates or what assembly code is 
generated when you call built-in function is not that easy. Obviously, 
the answers lie inside the source code, but, from my own experience, you
 can spend hours wandering through it without making much progress.
I
 will not pretend to be an expert on the topic, nor will I attempt to 
describe every possible aspect. Instead, the goal is to demonstrate how 
you can decipher Go sources on your own.
Before we can begin, we certainly need our own copy of Go source files. There is nothing special in getting them. Just execute:`git clone https://github.com/golang/go` 
Please note that the code in the main branch is being constantly changed, so I use the*release-branch.go1.4* branch in this blog post.
 Understanding project structureIf you look at the */src* folder
 of the Go repository, you can see a lot of folders. Most of them 
contain source files of the standard Go library. The standard naming 
conventions are always applied here, so each package is inside a folder 
with a name that directly corresponds to the package name. Apart from 
the standard library, there is a lot of other stuff. In my opinion, the 
most important and useful folders are:FolderDescription[/src/cmd/](https://github.com/golang/go/tree/release-branch.go1.4/src/cmd)Contains different command line tools.[/src/cmd/go/](https://github.com/golang/go/tree/release-branch.go1.4/src/cmd/go)Contains
 source files of a Go tool that downloads and builds Go source files and
 installs packages. While doing this, it collects all source files and 
makes calls to the Go linker and Go compiler command line tools.[/src/cmd/dist/](https://github.com/golang/go/tree/release-branch.go1.4/src/cmd/dist)Contains
 a tool responsible for building all other command line tools and all 
the packages from the standard library. You may want to analyze its 
source code to understand what libraries are used in every particular 
tool or package.[/src/cmd/gc/](https://github.com/golang/go/tree/release-branch.go1.4/src/cmd/gc)This is the architecture-independent part of the Go compiler.[/src/cmd/ld/](https://github.com/golang/go/tree/release-branch.go1.4/src/cmd/ld)The
 architecture-independent part of the Go linker. Architecture-dependent 
parts are located in the folder with the “l” postfix that uses the same 
naming conventions as the compiler.[/src/cmd/5a/](https://github.com/golang/go/tree/release-branch.go1.4/src/cmd/5a), 6a, 8a, and 9aHere
 you can find Go assembler compilers for different architectures. The Go
 assembler is a form of assembly language that does not map precisely to
 the assembler of the underlying machine. Instead, there is a distinct 
compiler for each architecture that translates the Go assembler to the 
machine’s assembler. You can find more details [here](https://golang.org/doc/asm).[/src/lib9/](https://github.com/golang/go/tree/release-branch.go1.4/src/lib9), [/src/libbio](https://github.com/golang/go/tree/release-branch.go1.4/src/libbio),[/src/liblink](https://github.com/golang/go/tree/release-branch.go1.4/src/liblink)Different libraries that are used inside the compiler, linker, and runtime package.[/src/runtime/](https://github.com/golang/go/tree/release-branch.go1.4/src/runtime)The
 most important Go package that is indirectly included into all 
programs. It contains the entire runtime functionality, such as memory 
management, garbage collection, goroutines creation, etc. Inside the Go compilerAs I said above, the architecture-independent part of the Go compiler is located in the  [/src/cmd/gc/](https://github.com/golang/go/tree/release-branch.go1.4/src/cmd/gc) folder. The entry point is located in the [lex.c](https://github.com/golang/go/blob/release-branch.go1.4/src/cmd/gc/lex.c#L199) file. Apart from some common stuff, such as parsing command line arguments, the compiler does the following:Initializes some common data structures.Iterates
 through all of the provided Go files and calls the yyparse method for 
each file. This causes actual parsing to occur. The Go compiler uses [Bison](http://www.gnu.org/software/bison/) as the parser generator. The grammar for the language is fully described in the [go.y](https://github.com/golang/go/blob/release-branch.go1.4/src/cmd/gc/go.y) file
 (I will provide more details on it later). As a result, this step 
generates a complete parse tree where each node represents an element of
 the compiled program.Recursively
 iterates through the generated tree several times and applies some 
modifications, e.g., defines type information for the nodes that should 
be implicitly typed, rewrites some language elements—such as 
typecasting—into calls to some functions in the runtime package and does
 some other work.Performs the actual compilation after the parse tree is complete. Nodes are translated into assembler code.Creates
 the object file that contains generated assembly code with some 
additional data structures, such as the symbols table, which is 
generated and written to the disk. Diving into Go grammarNow lets take a closer look at the second step. The [go.y](https://github.com/golang/go/blob/release-branch.go1.4/src/cmd/gc/go.y) file
 that contains language grammar is a good starting point for 
investigating the Go compiler and the key to understanding the language 
syntax. The main part of this file consists of declarations, similar to 
the following:`xfndcl:
​     LFUNC fndcl fnbody
fndcl:
​     sym '(' oarg_type_list_ocomma ')' fnres
| '(' oarg_type_list_ocomma ')' sym '(' oarg_type_list_ocomma ')' fnres` 
In this declaration, the *xfndcl* and *fundcl* nodes are defined. The *fundcl* node can be in one of two forms. The first form corresponds to the following language construct:`somefunction(x int, y int) int` 
and the second one to this language construct:`(t *SomeType) somefunction(x int, y int) int.` 
The *xfndcl* node consists of the keyword *func* that is stored in the constant *LFUNC*, followed by the *fndcl* and *fnbody*nodes.An
 important feature of Bison (or Yacc) grammar is that it allows for 
placing arbitrary C code next to each node definition. The code is 
executed every time a match for this node definition is found in the 
source code. Here, you can refer to the result node as *$$* and to the child nodes as *$1*, *$2*, …It is easier to understand this through an example. Note that the following code is a shortcut version of the actual code.`fndcl:
​      sym '(' oarg_type_list_ocomma ')' fnres
​        {
​          t = nod(OTFUNC, N, N);
​          t->list = $3;
​          t->rlist = $5;
​          $$ = nod(ODCLFUNC, N, N);
​          $$->nname = newname($1);
​          $$->nname->ntype = t;
​          declare($$->nname, PFUNC);
​      }
| '(' oarg_type_list_ocomma ')' sym '(' oarg_type_list_ocomma ')' fnres` 
First, a new node is created, which contains type information for the function declaration. The *$3* argument list and the *$5* result list are referenced from this node. Then, the *$$*result
 node is created. It stores the function name and the type node. As you 
can see, there can be no direct correspondence between definitions in 
the *go.y* file and the node structure.
 Understanding nodesNow it is time to take a look at what a node actually is. First of all, a node is a struct (you can find a definition [here](https://github.com/golang/go/blob/release-branch.go1.4/src/cmd/gc/go.h#L245)).
 This struct contains a large number of properties, since it needs to 
support different kinds of nodes and different nodes have different 
attributes. Below is a description of several fields that I think are 
important to understand.Node struct fieldDescription[op](https://github.com/golang/go/blob/release-branch.go1.4/src/cmd/gc/go.h#L259)Node
 operation. Each node has this field. It distinguishes different kinds 
of nodes from each other. In our previous example, those were *OTFUNC* (operation type function) and *ODCLFUNC* (operation declaration function).[type](https://github.com/golang/go/blob/release-branch.go1.4/src/cmd/gc/go.h#L295)This
 is a reference to another struct with type information for nodes that 
have type information (there are no types for some nodes, e.g., control 
flow statements, such as *if*, *switch*, or *for*).[val](https://github.com/golang/go/blob/release-branch.go1.4/src/cmd/gc/go.h#L309)This field contains the actual values for nodes that represent literals.Now
 that you understand the basic structure of the node tree, you can put 
your knowledge into practice. In the next post, we will investigate what
 exactly the Go compiler generates, using a simple Go application as an 
example.
*Read all parts of the series: \**Part 1** | Part 2 | Part 3 | Part 4 | Part 5 | Part 6***\*About the author:** Sergey
 Matyukevich is a Cloud Engineer and Go Developer at Altoros. With 6+ 
years in software engineering, he is an expert in cloud automation and 
designing architectures for complex cloud-based systems. An active 
member of the Go community, Sergey is a frequent contributor to 
open-source projects, such as Ubuntu and Juju Charms.*Subscribe to our blog for the next parts of this series or follow [@altoros](https://twitter.com/altoros).
来源： <<http://blog.altoros.com/golang-part-1-main-concepts-and-project-structure.html>> 

------

## Golang Internals, Part 2: Diving Into the Go Compiler

Posted by [Sergey Matyukevich](http://blog.altoros.com/author/sergey-matyukevich), Cloud Engineer at Altoros, in [Research & Development](http://blog.altoros.com/category/research-and-development)

Tags: [Golang](http://blog.altoros.com/tag/golang)

05 Mar 2015

[0 Comments](http://blog.altoros.com/golang-internals-part-2-diving-into-the-go-compiler.html#disqus_thread)

![golang-internals-the-go-compiler]()*All parts: Part 1 | \**Part 2** | Part 3 | Part 4 | Part 5 | Part 6*
Do
 you know what exactly happens in the Go runtime, when you use a 
variable via interface reference? This is not a trivial question, 
because in Go a type that implements an interface does not contain any 
references to this interface whatsoever. Still, we can try answering it,
 using our knowledge of the Go compiler, which was discussed in the[previous](http://blog.altoros.com/golang-part-1-main-concepts-and-project-structure.html) blog post.So,
 let’s take a deep dive into the Go compiler: create a basic Go program 
and see the internal workings of the Go typecasting. Using it as an 
example, I’ll explain how a node tree is generated and utilized. So, you
 can further apply this knowledge to other Go compiler’s features.
Before you startTo
 perform the experiment, we will need to work directly with the Go 
compiler (not the Go tool). You can access it by using the command:`go tool 6g test.go` 
It will compile the *test.go* source file and create an object file. Here, *6g* is
 the name of the compiler on my machine that has an AMD64 architecture. 
Note that you should use different compilers for different 
architectures.When we work directly with the compiler, we can use some handy command line arguments (more details [here](https://golang.org/cmd/gc/#hdr-Command_Line)). For the purposes of this experiment, we’ll need the *-W* flag that will print the layout of the node tree.
 Creating a simple Go programFirst of all, we are going to create a sample Go program. My version is below:`  1  package main
  2 
  3  type I interface {
  4          DoSomeWork()
  5  }
  6 
  7  type T struct {
  8          a int
  9  }
 10 
 11  func (t *T) DoSomeWork() {
 12  }
 13 
 14  func main() {
 15          t := &T{}
 16          i := I(t)
 17          print(i)
 18  }` 
Really simple, isn’t it? The only thing that might seem unnecessary is the 17th line, where we print the *i* variable. Nevertheless, without it, *i* will remain unused and the program will not be compiled. The next step is to compile our program using the -W switch:`go tool 6g -W test.go` 
After
 doing this, you will see output that contains node trees for each 
method defined in the program. In our case, these are the main and *init* methods. The *init* method is here because it is implicitly defined for all programs, but we actually do not care about it right now.For
 each method, the compiler prints two versions of the node tree. The 
first one is the original node tree that we get after parsing the source
 file. The second one is the version that we get after type checking and
 applying all the necessary modifications.
 Understanding the node tree of the main methodLet’s
 take a closer look at the original version of the node tree from the 
main method and try to understand what exactly is going on.`DCL l(15)
.   NAME-main.t u(1) a(1) g(1) l(15) x(0+0) class(PAUTO) f(1) ld(1) tc(1) used(1) PTR64-*main.T
AS l(15) colas(1) tc(1)
.   NAME-main.t u(1) a(1) g(1) l(15) x(0+0) class(PAUTO) f(1) ld(1) tc(1) used(1) PTR64-*main.T
.   PTRLIT l(15) esc(no) ld(1) tc(1) PTR64-*main.T
.   .   STRUCTLIT l(15) tc(1) main.T
.   .   .   TYPE <S> l(15) tc(1) implicit(1) type=PTR64-*main.T PTR64-*main.T
DCL l(16)
.   NAME-main.i u(1) a(1) g(2) l(16) x(0+0) class(PAUTO) f(1) ld(1) tc(1) used(1) main.I
AS l(16) tc(1)
.   NAME-main.autotmp_0000 u(1) a(1) l(16) x(0+0) class(PAUTO) esc(N) tc(1) used(1) PTR64-*main.T
.   NAME-main.t u(1) a(1) g(1) l(15) x(0+0) class(PAUTO) f(1) ld(1) tc(1) used(1) PTR64-*main.T
AS l(16) colas(1) tc(1)
.   NAME-main.i u(1) a(1) g(2) l(16) x(0+0) class(PAUTO) f(1) ld(1) tc(1) used(1) main.I
.   CONVIFACE l(16) tc(1) main.I
.   .   NAME-main.autotmp_0000 u(1) a(1) l(16) x(0+0) class(PAUTO) esc(N) tc(1) used(1) PTR64-*main.T
VARKILL l(16) tc(1)
.   NAME-main.autotmp_0000 u(1) a(1) l(16) x(0+0) class(PAUTO) esc(N) tc(1) used(1) PTR64-*main.T
PRINT l(17) tc(1)
PRINT-list
.   NAME-main.i u(1) a(1) g(2) l(16) x(0+0) class(PAUTO) f(1) ld(1) tc(1) used(1) main.I` 
In the explanation below, I will use an abridged version, from which I removed all the unnecessary details.The first node is rather simple:`DCL l(15)
.   NAME-main.t l(15) PTR64-*main.T` 
The first node is a declaration node. *l(15)* tells us that this node is defined in line 15. The declaration node references the name node that represents the *main.t* variable. This variable is defined in the main package and is actually a 64-bit pointer to the *main.T* type. You can look at line 15 and easily understand what declaration is represented there.The next one is a bit trickier.`AS l(15) 
.   NAME-main.t l(15) PTR64-*main.T
.   PTRLIT l(15) PTR64-*main.T
.   .   STRUCTLIT l(15) main.T
.   .   .   TYPE l(15) type=PTR64-*main.T PTR64-*main.T` 
The root node is the assignment node. Its first child is the name node that represents the*main.t* variable. The second child is a node that we assign to *main.t*—a
 pointer literal node (&). It has a child struct literal, which, in 
its turn, points to the type node that represents the actual type (*main.T*).The next node is another declaration. This time, it is a declaration of the *main.i* variable that belongs to the *main.I* type.`DCL l(16)
.   NAME-main.i l(16) main.I` 
Then, the compiler creates another variable, *autotmp_0000*, and assigns the *main.t* variable to it.`AS l(16) tc(1)
.   NAME-main.autotmp_0000 l(16) PTR64-*main.T
.   NAME-main.t l(15) PTR64-*main.T` 
Finally, we came to the nodes that we are actually inetersted in.`AS l(16) 
.   NAME-main.i l(16)main.I
.   CONVIFACE l(16) main.I
.   .   NAME-main.autotmp_0000 PTR64-*main.T` 
Here, we can see that the compiler has assigned a special node called *CONVIFACE* to the*main.i* variable.
 But this does not give us much information about what’s happening under
 the hood. To find out what’s going on, we need to look into the node 
tree of the main method after all node tree modifications have been 
applied (you can find this information in the “after walk main” section 
of your output).
 How the compiler translates the assignment nodeBelow, you can see how the compiler translates our assignment node:`AS-init
.   AS l(16) 
.   .   NAME-main.autotmp_0003 l(16) PTR64-*uint8
.   .   NAME-go.itab.*"".T."".I l(16) PTR64-*uint8
.   IF l(16) 
.   IF-test
.   .   EQ l(16) bool
.   .   .   NAME-main.autotmp_0003 l(16) PTR64-*uint8
.   .   .   LITERAL-nil I(16) PTR64-*uint8
.   IF-body
.   .   AS l(16)
.   .   .   NAME-main.autotmp_0003 l(16) PTR64-*uint8
.   .   .   CALLFUNC l(16) PTR64-*byte
.   .   .   .   NAME-runtime.typ2Itab l(2) FUNC-funcSTRUCT-(FIELD-
.   .   .   .   .   NAME-runtime.typ·2 l(2) PTR64-*byte, FIELD-
.   .   .   .   .   NAME-runtime.typ2·3 l(2) PTR64-*byte PTR64-*byte, FIELD-
.   .   .   .   .   NAME-runtime.cache·4 l(2) PTR64-*PTR64-*byte PTR64-*PTR64-*byte) PTR64-*byte
.   .   .   CALLFUNC-list
.   .   .   .   AS l(16) 
.   .   .   .   .   INDREG-SP l(16) runtime.typ·2 G0 PTR64-*byte
.   .   .   .   .   ADDR l(16) PTR64-*uint8
.   .   .   .   .   .   NAME-type.*"".T l(11) uint8
.   .   .   .   AS l(16)
.   .   .   .   .   INDREG-SP l(16) runtime.typ2·3 G0 PTR64-*byte
.   .   .   .   .   ADDR l(16) PTR64-*uint8
.   .   .   .   .   .   NAME-type."".I l(16) uint8
.   .   .   .   AS l(16) 
.   .   .   .   .   INDREG-SP l(16) runtime.cache·4 G0 PTR64-*PTR64-*byte
.   .   .   .   .   ADDR l(16) PTR64-*PTR64-*uint8
.   .   .   .   .   .   NAME-go.itab.*"".T."".I l(16) PTR64-*uint8
AS l(16) 
.   NAME-main.i l(16) main.I
.   EFACE l(16) main.I
.   .   NAME-main.autotmp_0003 l(16) PTR64-*uint8
.   .   NAME-main.autotmp_0000 l(16) PTR64-*main.T` 
As you can see from the output, the compiler first adds an initialization node list (*AS-init*) to the assignment node. Inside the *AS-init* node, it creates a new variable, *main.autotmp_0003*, and assigns the value of the *go.itab.\*””.T.””.I* variable to it. After that, it checks whether this variable is nil. If the variable is nil, the compiler calls the *runtime.typ2Itab* function and passes the following to it:a pointer to the *main.T* type ,
a pointer to the *main.I* interface type,
and a pointer to the *go.itab.\*””.T.””.I* variable.From this code, it is quite evident that this variable is for caching the result of type conversion from *main.T* to *main.I*.
 Inside the *getitab* methodThe next logical step is to find *runtime.typ2Itab*. Below is the listing of this function:`func typ2Itab(t *_type, inter *interfacetype, cache **itab) *itab {
​	tab := getitab(inter, t, false)
​	atomicstorep(unsafe.Pointer(cache), unsafe.Pointer(tab))
​	return tab
}` 
It is quite evident that the actual work is done inside the *getitab* method, because the second line simply stores the created *tab* variable in the cache. So, let’s look inside *getitab*. Since it is rather big, I only copied the most valuable part.`m = 
​    (*itab)(persistentalloc(unsafe.Sizeof(itab{})+uintptr(len(inter.mhdr)-1)*ptrSize, 0,
​    &memstats.other_sys))
​    m.inter = interm._type = typ
ni := len(inter.mhdr)
nt := len(x.mhdr)
j := 0
for k := 0; k < ni; k++ {
​	i := &inter.mhdr[k]
​	iname := i.name
​	ipkgpath := i.pkgpath
​	itype := i._type
​	for ; j < nt; j++ {
​		t := &x.mhdr[j]
​		if t.mtyp == itype && t.name == iname && t.pkgpath == ipkgpath {
​			if m != nil {
​				*(*unsafe.Pointer)(add(unsafe.Pointer(&m.fun[0]), uintptr(k)*ptrSize)) = t.ifn
​			}
​		}
​	}
}` 
First, we allocate memory for the result:`(*itab)(persistentalloc(unsafe.Sizeof(itab{})+uintptr(len(inter.mhdr)-1)*ptrSize, 0, &memstats.other_sys))` 
Why should we allocate memory in Go and why is this done in such a strange way? To answer this question, we need to look at the *itab* struct definition.`type itab struct {
​	inter  *interfacetype
​	_type  *_type
​	link   *itab
​	bad    int32
​	unused int32
​	fun    [1]uintptr // variable sized
}` 
The last property, *fun*,
 is defined as an array of one element, but it is actually 
variable-sized. Later, we’ll see that this property contains an array of
 pointers to methods defined in a particular type. These methods 
correspond to the methods in the interface type. The authors of Go use 
dynamic memory allocation for this property (yes, such things are 
possible, when you use an unsafe package). The amount of memory to be 
allocated is calculated by adding the size of the struct itself to the 
number of methods in the interface multiplied by a pointer size.`unsafe.Sizeof(itab{})+uintptr(len(inter.mhdr)-1)*ptrSize` 
Next,
 you can see two nested loops. First, we iterate through all interface 
methods. For each method in the interface, we try to find a 
corresponding method in a particular type (the methods are stored in 
the *mhdr* collection). The process of checking whether two methods are equal is quite self-explanatory.`if t.mtyp == itype && t.name == iname && t.pkgpath == ipkgpath` 
If we find a match, we store a pointer to the method in the *fun* property of the result:`*(*unsafe.Pointer)(add(unsafe.Pointer(&m.fun[0]), uintptr(k)*ptrSize)) = t.ifn` 
A
 small note on performance: since methods are sorted alphabetically for 
interface and pre-set type definitions, this nested loop can repeat *O(n + m)* times instead of *O(n \* m)*times, where *n* and *m* correspond to the number of methods.Finally, do you remember the last part of the assignment?`AS l(16) 
.   NAME-main.i l(16) main.I
.   EFACE l(16) main.I
.   .   NAME-main.autotmp_0003 l(16) PTR64-*uint8
.   .   NAME-main.autotmp_0000 l(16) PTR64-*main.T` 
Here, we assign the *EFACE* node to the main.i variable. This node (*EFACE*) contains references to the *main.autotmp_0003* variable—a pointer to the itab struct that was returned by the *runtime.typ2Itab* method—and to the *autotmp_0000* variable that contains the same value as the *main.t* variable. This is all we need to call methods by interface references.So, the *main.i* variable contains an instance of the *iface* struct defined in the runtime package:`type iface struct {
​	tab  *itab
​	data unsafe.Pointer
}` What’s next?I
 understand that I’ve only covered a very small part of the Go compiler 
and the Go runtime so far. There are still plenty of interesting things 
to talk about, such as object files, the linker, relocations, etc.—they 
will be overviewed in the upcoming blog posts.
*Read all parts of the series: Part 1 | \**Part 2** | Part 3 | Part 4 | Part 5 | Part 6***\*About the author:** Sergey
 Matyukevich is a Cloud Engineer and Go Developer at Altoros. With 6+ 
years in software engineering, he is an expert in cloud automation and 
designing architectures for complex cloud-based systems. An active 
member of the Go community, Sergey is a frequent contributor to 
open-source projects, such as Ubuntu and Juju Charms.*Subscribe to our blog to read the upcoming parts of the series or follow [@altoros](https://twitter.com/Altoros).
来源： <<http://blog.altoros.com/golang-internals-part-2-diving-into-the-go-compiler.html>> 

------

## Golang Internals, Part 3: The Linker, Object Files, and Relocations

Posted by [Sergey Matyukevich](http://blog.altoros.com/author/sergey-matyukevich), Cloud Engineer at Altoros, in [Research & Development](http://blog.altoros.com/category/research-and-development)

Tags: [Golang](http://blog.altoros.com/tag/golang)

11 Mar 2015

[0 Comments](http://blog.altoros.com/golang-internals-part-3-the-linker-and-object-files.html#disqus_thread)

*All parts: Part 1 | Part 2 | \**Part 3** | Part 4 | Part 5 | Part 6*![15-03-11-Golang_Internals_the_Linker_and_Object_files]()Today, I will speak about the Go linker, Go object files, and relocations.Why
 should we care about these things? Well, if you want to learn the 
internals of any large project, the first thing you need to do is split 
it into components or modules. Second, you need to understand what 
interface these modules provide to each other. In Go, these high-level 
modules are the compiler, linker, and runtime. The interface that the 
compiler provides and the linker consumes is an object file and that’s 
where we will start our investigation today.
Generating a Go object fileLet’s
 do a practical experiment—write a super simple program, compile it, and
 see what object file will be produced. In my case, the program was as 
follows:`package main
func main() {
​	print(1)
}` 
Really straightforward, isn’t it? Now we need to compile it:`go tool 6g test.go` 
This command produces the *test.6* object file. To investigate its internal structure, we are going to use the [goobj](https://github.com/golang/go/tree/master/src/cmd/internal/goobj) library.
 It is employed internally in Go source code, mainly for implementing a 
set of unit tests that verifies whether object files are generated 
correctly in different situations. For this blog post, I wrote a very 
simple program that prints the output generated from the *googj* library to the console. You can take a look at the sources of this program [here](https://github.com/s-matyukevich/goobj_explorer).First of all, you need to download and install my program:`go get github.com/s-matyukevich/goobj_explorer` 
Then execute the following command:`goobj_explorer -o test.6` 
Now you should be able to see the *goob.Package* structure in your console.
 Investigating the object fileThe most interesting part of our object file is the *Syms* array.
 This is actually a symbol table. Everything that you define in your 
program—functions, global variables, types, constants, etc.—is written 
to this table. Let’s look at the entry that corresponds to the *main* function. (Note that I have cut the *Reloc* and *Func* fields from the output for now. We will discuss them later.)`&goobj.Sym{
​            SymID: goobj.SymID{Name:"main.main", Version:0},
​            Kind:  1,
​            DupOK: false,
​            Size:  48,
​            Type:  goobj.SymID{},
​            Data:  goobj.Data{Offset:137, Size:44},
​            Reloc: ...,
​            Func:  ...,
}` 
The names of the fields in the *goobj.Sum* structure are pretty self-explanatory:FieldDescription**SumID**The
 unique symbol ID that consists of the symbol’s name and version. 
Versions help to differentiate symbols with identical names.**Kind**Indicates to what kind the symbol belongs (more details later).**DupOK**This field indicates whether duplicates (symbols with the same name) are allowed.**Size**The size of symbol data.**Type**A reference to another symbol that represents a symbol type, if any.**Data**Contains
 binary data. This field has different meanings for symbols of different
 kinds, e.g., assembly code for functions, raw string content for string
 symbols, etc.**Reloc**The list of relocations (more details will be provided later)**Func**Contains special function metadata for function symbols (see more details below).Now, let’s look at different kinds of symbols. All possible kinds of symbols are defined as constants in the *goobj* package (you can find them [here](https://github.com/golang/go/blob/master/src/cmd/internal/goobj/read.go#L30)). Below, I copied the first part of these constants:`const (
​	_ SymKind = iota
​	// readonly, executable
​	STEXT
​	SELFRXSECT
​	// readonly, non-executable
​	STYPE
​	SSTRING
​	SGOSTRING
​	SGOFUNC
​	SRODATA
​	SFUNCTAB
​	STYPELINK
​	SSYMTAB // TODO: move to unmapped section
​	SPCLNTAB
​	SELFROSECT
​	...` 
As we can see, the *main.main* symbol belongs to kind 1 that corresponds to the *STEXT*constant. *STEXT* is a symbol that contains executable code. Now, let’s look at the *Reloc*array. It consists of the following structs:`type Reloc struct {
​	Offset int
​	Size   int
​	Sym    SymID
​	Add    int
​	Type int
}` 
Each relocation implies that the bytes situated at the *[Offset, Offset+Size]* interval should be replaced with a specified address. This address is calculated by summing up the location of the *Sym* symbol with the *Add* number of bytes.
 Understanding relocationsNow let’s use an example and see how relocations work. To do this, we need to compile our program using the *-S* switch that will print the generated assembly code:`go tool 6g -S test.go` 
Let’s look through the assembler and try to find the main function.`"".main t=1 size=48 value=0 args=0x0 locals=0x8
​	0x0000 00000 (test.go:3)	TEXT	"".main+0(SB),$8-0
​	0x0000 00000 (test.go:3)	MOVQ	(TLS),CX
​	0x0009 00009 (test.go:3)	CMPQ	SP,16(CX)
​	0x000d 00013 (test.go:3)	JHI	,22
​	0x000f 00015 (test.go:3)	CALL	,runtime.morestack_noctxt(SB)
​	0x0014 00020 (test.go:3)	JMP	,0
​	0x0016 00022 (test.go:3)	SUBQ	$8,SP
​	0x001a 00026 (test.go:3)	FUNCDATA	$0,gclocals·3280bececceccd33cb74587feedb1f9f+0(SB)
​	0x001a 00026 (test.go:3)	FUNCDATA	$1,gclocals·3280bececceccd33cb74587feedb1f9f+0(SB)
​	0x001a 00026 (test.go:4)	MOVQ	$1,(SP)
​	0x0022 00034 (test.go:4)	PCDATA	$0,$0
​	0x0022 00034 (test.go:4)	CALL	,runtime.printint(SB)
​	0x0027 00039 (test.go:5)	ADDQ	$8,SP
​	0x002b 00043 (test.go:5)	RET	,` 
In
 later blog posts, we’ll have a closer look at this code and try to 
understand how the Go runtime works. For now, we are interested in the 
following line:`0x0022 00034 (test.go:4)	CALL	,runtime.printint(SB)` 
This
 command is located at an offset of 0x0022 (in hex) or 00034 (decimal) 
within the function data. This line is actually responsible for calling 
the *runtime.printint* function. The issue is that the compiler does not know the exact address of the *runtime.printint* function
 during compilation. This function is located in a different object file
 the compiler knows nothing about. In such cases, it uses relocations. 
Below is the exact relocation that corresponds to this method call (I 
copied it from the first output of the *goobj_explorer*utility):`{
​                    Offset: 35,
​                    Size:   4,
​                    Sym:    goobj.SymID{Name:"runtime.printint", Version:0},
​                    Add:    0,
​                    Type:   3,
​                },` 
This
 relocation tells the linker that, starting from an offset of 35 bytes, 
it needs to replace 4 bytes of data with the address of the starting 
point of the *runtime.printint* symbol.
 But an offset of 35 bytes from the main function data is actually an 
argument of the call instruction that we have previously seen. (The 
instruction starts from an offset of 34 bytes. One byte corresponds to 
call instruction code and four bytes—to the address of this 
instruction.)
 How the linker operatesNow
 that we understand this, we can figure out how the linker works. The 
following schema is very simplified, but it reflects the main idea:The
 linker gathers all the symbols from all the packages that are 
referenced from the main package and loads them into one big byte array 
(or a binary image).For each symbol, the linker calculates an address in this image.Then
 it applies the relocations defined for every symbol. It is easy now, 
since the linker knows the exact addresses of all other symbols 
referenced from those relocations.The linker prepares all the headers necessary for the [Executable and Linkable (ELF)](http://en.wikipedia.org/wiki/Executable_and_Linkable_Format)format (on Linux) or the [Portable Executable (PE)](http://en.wikipedia.org/wiki/Portable_Executable) format (on Windows). Then, it generates an executable file with the results. Understanding TLSA careful reader will notice a strange relocation in the output of the *goobj_explorer utility*for the main method. It doesn’t correspond to any method call and even points to an empty symbol:`{
​                    Offset: 5,
​                    Size:   4,
​                    Sym:    goobj.SymID{},
​                    Add:    0,
​                    Type:   9,
​                },` 
So,
 what does this relocation do? We can see that it has an offset of 5 
bytes and its size is 4 bytes. At this offset, there is a command:`0x0000 00000 (test.go:3)	MOVQ	(TLS),CX` 
It
 starts at an offset of 0 and occupies 9 bytes (since the next command 
starts at an offset of 9 bytes). We can guess that this relocation 
replaces the strange *(TLS)* statement with some address, but what is TLS and what address does it use?
TLS is an abbreviation for Thread Local Storage. This technology is used in many programming languages (more details [here](http://en.wikipedia.org/wiki/Thread-local_storage)). In short, it enables us to have a variable that points to different memory locations when used by different threads.
In
 Go, TLS is used to store a pointer to the G structure that contains 
internal details of a particular Go routine (more details on this in 
later blog posts). So, there is a variable that—when accessed from 
different Go routines—always points to a structure with internal details
 of this Go routine. The location of this variable is known to the 
linker and this variable is exactly what was moved to the CX register in
 the previous command. TLS can be implemented differently for different 
architectures. For AMD64, TLS is implemented via the*FS* register, so our previous command is translated into *MOVQ FS*, *CX*.
To
 end our discussion on relocations, I am going to show you the 
enumerated type (enum) that contains all the different types of 
relocations:`// Reloc.type
enum
{
​	R_ADDR = 1,
​	R_SIZE,
​	R_CALL, // relocation for direct PC-relative call
​	R_CALLARM, // relocation for ARM direct call
​	R_CALLIND, // marker for indirect call (no actual relocating necessary)
​	R_CONST,
​	R_PCREL,
​	R_TLS,
​	R_TLS_LE, // TLS local exec offset from TLS segment register
​	R_TLS_IE, // TLS initial exec offset from TLS base pointer
​	R_GOTOFF,
​	R_PLT0,
​	R_PLT1,
​	R_PLT2,
​	R_USEFIELD,
};` 
As you can see from this enum, relocation type 3 is *R_CALL* and relocation type 9 is *R_TLS*. These enum names perfectly explain the behaviour that we discussed previously.
 More on Go object filesIn
 the next post, we’ll continue our discussion on object files. I will 
also provide more information necessary for you to move forward and 
understand how the Go runtime works. If you have any questions, feel 
free to ask them in the comments.
*Read all parts of the series: Part 1 | Part 2 | \**Part 3** | Part 4 | Part 5 | Part 6***\*About the author:** Sergey
 Matyukevich is a Cloud Engineer and Go Developer at Altoros. With 6+ 
years in software engineering, he is an expert in cloud automation and 
designing architectures for complex cloud-based systems. An active 
member of the Go community, Sergey is a frequent contributor to 
open-source projects, such as Ubuntu and Juju Charms.*Subscribe to our blog for the next parts of this series or follow [@altoros](https://twitter.com/altoros).
来源： <<http://blog.altoros.com/golang-internals-part-3-the-linker-and-object-files.html>> 

------

## Golang Internals, Part 4: Object Files and Function Metadata

Posted by [Sergey Matyukevich](http://blog.altoros.com/author/sergey-matyukevich), Cloud Engineer at Altoros, in [Research & Development](http://blog.altoros.com/category/research-and-development)

Tags: [Golang](http://blog.altoros.com/tag/golang)

18 Mar 2015

[0 Comments](http://blog.altoros.com/golang-part-4-object-files-and-function-metadata.html#disqus_thread)

![Golang-Part-4-Object-Files-and-Function-Metadata]()*All parts: Part 1 | Part 2 | Part 3 | \**Part 4** | Part 5 | Part 6*Today, we’ll take a closer look at the *Func* structure and discuss a few details on how garbage collection works in Go.This post is a continuation of [“Golang Internals, Part 3: The Linker and Go Object Files”](http://blog.altoros.com/golang-internals-part-3-the-linker-and-object-files.html) and uses the same sample program. So, if you haven’t read it, I strongly advise that you do this before moving forward.
The structure of function metadataThe main idea behind relocations should be clear from Part 3. Now let’s take a look at the*Func* structure of the *main* method:`01``Func: &goobj.Func{``02``    ``Args:    0,``03``    ``Frame:   8,``04``    ``Leaf:    false,``05``    ``NoSplit: false,``06``    ``Var:     {``07``    ``},``08``    ``PCSP:   goobj.Data{Offset:255, Size:7},``09``    ``PCFile: goobj.Data{Offset:263, Size:3},``10``    ``PCLine: goobj.Data{Offset:267, Size:7},``11``    ``PCData: {``12``        ``{Offset:276, Size:5},``13``    ``},``14``    ``FuncData: {``15``        ``{``16``            ``Sym:    goobj.SymID{Name:"gclocals·3280bececceccd33cb74587feedb1f9f", Version:0},``17``         ``Offset: 0,``18``     ``},``19``     ``{``20``         ``Sym:    goobj.SymID{Name:"gclocals·3280bececceccd33cb74587feedb1f9f", Version:0},``21``               ``Offset: 0,``22``           ``},``23``       ``},``24``       ``File: {"/home/adminone/temp/test.go"},``25``   ``},`You can think of this structure as function metadata emitted by the compiler in the object file and used by the Go runtime. [This](https://docs.google.com/document/d/1lyPIbmsYbXnpNj57a261hgOYVpNRcgydurVQIyZOz_o/pub) article
 explains the exact format and meaning of the different fields in Func. 
Now, I will try to show you how this metadata is used in the runtime.Inside the runtime package, this metadata is mapped on the following struct:`01``type _func struct {``02``    ``entry   uintptr // start pc``03``    ``nameoff int32   // function name``04` `05``    ``args  int32 // in/out args size``06``    ``frame int32 // legacy frame size; use pcsp if possible``07` `08``    ``pcsp      int32``09``    ``pcfile    int32``10``    ``pcln      int32``11``    ``npcdata   int32``12``    ``nfuncdata int32``13``}`You
 can see that not all the information that was in the object file has 
been mapped directly. Some of the fields are only used by the linker. 
Still, the most interesting here are the *pcsp*, *pcfile*, and *pcln* fields,which are used when a [program counter](http://en.wikipedia.org/wiki/Program_counter) is translated into a stack pointer, filename, and line accordingly.This is required, for example, when *panic* occurs.
 At that exact moment, the runtime only knows about the program counter 
of the current assembly instruction that has triggered the *panic*.
 So, the runtime uses that counter to obtain the current file, line 
number, and full stack trace. The file and line number are resolved 
directly, using the *pcfile* and *pcln* fields. The stack trace is resolved recursively, using *pcsp*.Now
 that I have a program counter, the question is, how do I get a 
corresponding line number? To answer it, you need to look through 
assembly code and understand how line numbers are stored in the object 
file:`1``0x001a 00026 (test.go:4)    MOVQ    $1,(SP)``2``    ``0x0022 00034 (test.go:4)    PCDATA  $0,$0``3``    ``0x0022 00034 (test.go:4)    CALL    ,runtime.printint(SB)``4``    ``0x0027 00039 (test.go:5)    ADDQ    $8,SP``5``    ``0x002b 00043 (test.go:5)    RET ,`We can see that program counters from 26 to 38 inclusive correspond to line number 4 and counters from 39 to *next_function_program_counter – 1* correspond to line number 5. For space efficiency, it is enough to store the following map:`1``26 - 4``2``39 - 5``3``…`This is almost exactly what the compiler does. The *pcln* field
 points to a particular offset in a map that corresponds to the first 
program counter of the current function. Knowing this offset and also 
the offset of the first program counter of the next function, the 
runtime can use binary search to find the line number that corresponds 
to the given program counter.In
 Go, this idea is generalized. Not only a line number or stack pointer 
can be mapped to a program counter, but also any integer value. This is 
done via the *PCDATA* instruction. Each time, the linker finds the following instruction:`1``0x0022 00034 (test.go:4)    PCDATA  $0,$0`It
 doesn’t generate any actual assembler instructions. Instead, it stores 
the second argument of this instruction in a map with the current 
program counter, while the first argument indicates what map is used. 
With this first argument, we can easily add new maps, which meaning is 
known to the compiler and runtime but is opaque to the linker.
 How the garbage collector uses function metadataThe last thing that still needs to be clarified in function metadata is the *FuncData* array. It contains information necessary for garbage collection. Go uses a [mark-and-sweep](http://www.brpreiss.com/books/opus5/html/page424.html) garbage
 collector (GC) that operates in two stages. During the first stage 
(mark), it traverses through all objects that are still in use and marks
 them as reachable. All the unmarked objects are removed during the 
second (sweep) stage.So,
 the garbage collector starts by looking for a reachable object in 
several known locations, such as global variables, processor registers, 
stack frames, and pointers in objects that have already been reached. 
However, if you think about it carefully, looking for pointers in stack 
frames is far from a trivial task. So, when the runtime is performing 
garbage collection, how does it distinguish whether a variable in the 
stack is a pointer or belongs to a non-pointer type? This is where *FuncData* comes into play.For
 each function, the compiler creates two variables. One contains a 
bitmap vector for the arguments area of the stack frame. The other one 
contains a bitmap for the rest of the frame that includes all the local 
variables of pointer types defined in the function. Each of these 
variables tells the garbage collector, where exactly in the stack frame 
the pointers are located, and that information is enough for it to do 
its job.It is also worth mentioning that, like *PCDATA*, *FUNCDATA* is also generated by a pseudo-Go assembly instruction:`1``0x001a 00026 (test.go:3)    FUNCDATA    $0,gclocals·3280bececceccd33cb74587feedb1f9f+0(SB)`The
 first argument of this instruction indicates, whether this is function 
data for arguments or a local variables area. The second one is actually
 a reference to a hidden variable that contains a GC mask.
 More on GolangIn
 the upcoming posts, I will tell you about the Go bootstrap process, 
which is the key to understanding how the Go runtime works. See you in a
 week.
*Read all parts of the series: Part 1 | Part 2 | Part 3 | \**Part 4** | Part 5 | Part 6***\*About the author:** Sergey
 Matyukevich is a Cloud Engineer and Go Developer at Altoros. With 6+ 
years in software engineering, he is an expert in cloud automation and 
designing architectures for complex cloud-based systems. An active 
member of the Go community, Sergey is a frequent contributor to 
open-source projects, such as Ubuntu and Juju Charms.*Subscribe to our blog for the next parts of this series or follow [@altoros](https://twitter.com/altoros).
来源： <<http://blog.altoros.com/golang-part-4-object-files-and-function-metadata.html>> 

------

## Golang Internals, Part 5: the Runtime Bootstrap Process

Posted by [Sergey Matyukevich](http://blog.altoros.com/author/sergey-matyukevich), Cloud Engineer at Altoros, in [Research & Development](http://blog.altoros.com/category/research-and-development)

Tags: [Golang](http://blog.altoros.com/tag/golang)

02 Apr 2015

[0 Comments](http://blog.altoros.com/golang-internals-part-5-runtime-bootstrap-process.html#disqus_thread)

![Golang Internals Go Runtime and Bootstrapping]()*All parts: Part 1 | Part 2 | Part 3 | Part 4 | \**Part 5** | Part 6*
The
 bootstrapping process is the key to understanding how the Go runtime 
works. Learning it is essential, if you want to move forward with Go. So
 the fifth installment in our Golang Internals series is dedicated to 
the Go runtime and, specifically, the Go bootstrap process. This time 
you will learn about:Go bootstrappingresizable stacks implementationinternal TLS implementationNote
 that this post contains a lot of assembler code and you will need at 
least some basic knowledge of it to proceed (here is a quick [guide to Go’s assembler](https://golang.org/doc/asm)). So let’s get going! 
Finding an entry pointFirst,
 we need to find what function is executed immediately after we start a 
Go program. To do this, we will write a simple Go app:`1``package main``2` `3``func main() {``4``    ``print(123)``5``}`Then we need to compile and link it:`1``go tool 6g test.go``2``go tool 6l test.6`This will create an executable file called *6.out* in your current directory. The next step involves the [objdump](https://sourceware.org/binutils/docs/binutils/objdump.html) tool,
 which is specific to Linux. Windows and Mac users can find analogs or 
skip this step altogether. Now run the following command:`1``objdump -f 6.out`You should get output that will contain the start address:`1``6.out:     file format elf64-x86-64``2``architecture: i386:x86-64, flags 0x00000112:``3``EXEC_P, HAS_SYMS, D_PAGED``4``start address 0x000000000042f160`Next, we need to disassemble our executable and find what function is located at this address:`1``objdump -d 6.out > disassemble.txt`Then we need to open the *disassemble.txt* file and search for “*42f160*.” Here is what I got:`1``000000000042f160 <_rt0_amd64_linux>:``2``  ``42f160:   48 8d 74 24 08              lea    0x8(%rsp),%rsi``3``  ``42f165:   48 8b 3c 24                 mov    (%rsp),%rdi``4``  ``42f169:   48 8d 05 10 00 00 00    lea    0x10(%rip),%rax        # 42f180 <main>``5``  ``42f170:   ff e0                           jmpq   *%rax`Nice, we have found it! The entry point for my OS and architecture is a function called*_rt0_amd64_linux*.
 The starting sequenceNow we need to find this function in Go runtime sources. It is located in the*rt0_linux_arm64.s* file.
 If you look inside the Go runtime package, you can find many filenames 
with postfixes related to OS and architecture names. When a runtime 
package is built, only the files that correspond to the current OS and 
architecture are selected. The rest are skipped. Let’s take a closer 
look at *rt0_linux_arm64.s*:`1``TEXT _rt0_amd64_linux(SB),NOSPLIT,$-8``2``    ``LEAQ    8(SP), SI // argv``3``    ``MOVQ    0(SP), DI // argc``4``    ``MOVQ    $main(SB), AX``5``    ``JMP AX``6` `7``TEXT main(SB),NOSPLIT,$-8``8``    ``MOVQ    $runtime·rt0_go(SB), AX``9``    ``JMP AX`The *_rt0_amd64_linux* function is very simple. It calls the main function and saves arguments (*argc* and *argv*) in registers (*DI* and *SI*). The arguments are located in the stack and can be accessed via the *SP* (stack pointer) register. The main function is also very simple. It calls *runtime.rt0_go*. The *runtime.rt0_go* function is longer and more complicated, so I will break it into small parts and describe each one separately.The first section goes like this:`1``MOVQ    DI, AX      // argc``2``MOVQ    SI, BX      // argv``3``SUBQ    $(4*8+7), SP        // 2args 2auto``4``ANDQ    $~15, SP``5``MOVQ    AX, 16(SP)``6``MOVQ    BX, 24(SP)`Here, we put some previously saved command line argument values inside the *AX* and *BX*decrease
 stack pointers. We also add space for two more four-byte variables and 
adjust it to be 16-bit aligned. Finally, we move the arguments back to 
the stack.`1``// create istack out of the given (operating system) stack.``2``// _cgo_init may update stackguard.``3``MOVQ    $runtime·g0(SB), DI``4``LEAQ    (-64*1024+104)(SP), BX``5``MOVQ    BX, g_stackguard0(DI)``6``MOVQ    BX, g_stackguard1(DI)``7``MOVQ    BX, (g_stack+stack_lo)(DI)``8``MOVQ    SP, (g_stack+stack_hi)(DI)`The second part is a bit more tricky. First, we load the address of the global *runtime.g0*variable into the DI register. This variable is defined in the *proc1.go* file and belongs to the*runtime,g* type. Variables of this type are created for each goroutine in the system. As you can guess, *runtime.g0 *describes a root goroutine. Then we initialize the fields that describe the stack of the root goroutine. The meaning of *stack.lo* and *stack.hi* should be clear. These are pointers to the beginning and the end of the stack for the current goroutine, but what are the *stackguard0* and *stackguard1* fields? To understand this, we need to set aside the investigation of the *runtime.rt0_go* function and take a closer look at stack growth in Go.
 Resizable stack implementation in GoThe
 Go language uses resizable stacks. Each goroutine starts with a small 
stack and its size changes each time a certain threshold is reached. 
Obviously, there is a way to check whether we have reached this 
threshold or not. In fact, the check is performed at the beginning of 
each function. To see how it works, let’s compile our sample program one
 more time with the *-S* flag (this will show the generated assembler code). The beginning of the main function looks like this:`1``"".main t=1 size=48 value=0 args=0x0 locals=0x8``2``    ``0x0000 00000 (test.go:3)    TEXT    "".main+0(SB),$8-0``3``    ``0x0000 00000 (test.go:3)    MOVQ    (TLS),CX``4``    ``0x0009 00009 (test.go:3)    CMPQ    SP,16(CX)``5``    ``0x000d 00013 (test.go:3)    JHI ,22``6``    ``0x000f 00015 (test.go:3)    CALL    ,runtime.morestack_noctxt(SB)``7``    ``0x0014 00020 (test.go:3)    JMP ,0``8``    ``0x0016 00022 (test.go:3)    SUBQ    $8,SP`First, we load a value from thread local storage (TLS) to the *CX* register (I have already explained what TLS is in one of my [previous posts](http://blog.altoros.com/golang-internals-part-3-the-linker-and-object-files.html)). This value always contains a pointer to the *runtime.g* structure
 that corresponds to the current goroutine. Then we compare the stack 
pointer to the value located at an offset of 16 bytes in the *runtime.g* structure. We can easily calculate that this corresponds to the *stackguard0* field.So,
 this is how we check if we have reached the stack threshold. If we 
haven’t reached it yet, the check fails. In this case, we call the *runtime.morestack_noctxt* function repeatedly until enough memory has been allocated for the stack. The stackguard1 field works very similarly to *stackguard0*, but it is used inside the C stack growth prologue instead of Go. The inner workings of *runtime.morestack_noctxt* is also a very interesting topic, but we will discuss it later. For now, let’s return to the bootstrap process.
 Continuing the investigation of Go bootstrappingWe will proceed with the starting sequence by looking at the next portion of code inside the*runtime.rt0_go* function:`01``    ``// find out information about the processor we're on``02``    ``MOVQ    $0, AX``03``    ``CPUID``04``    ``CMPQ    AX, $0``05``    ``JE  nocpuinfo``06` `07``    ``// Figure out how to serialize RDTSC.``08``    ``// On Intel processors LFENCE is enough. AMD requires MFENCE.``09``    ``// Don't know about the rest, so let's do MFENCE.``10``    ``CMPL    BX, $0x756E6547  // "Genu"``11``    ``JNE notintel``12``    ``CMPL    DX, $0x49656E69  // "ineI"``13``    ``JNE notintel``14``    ``CMPL    CX, $0x6C65746E  // "ntel"``15``    ``JNE notintel``16``    ``MOVB    $1, runtime·lfenceBeforeRdtsc(SB)``17``notintel:``18` `19``    ``MOVQ    $1, AX``20``    ``CPUID``21``    ``MOVL    CX, runtime·cpuid_ecx(SB)``22``    ``MOVL    DX, runtime·cpuid_edx(SB)``23``nocpuinfo: `This
 part is not crucial for understanding major Go concepts, so we will 
look through it briefly. Here, we are trying to figure out what 
processor we are using. If it is Intel, we set the*runtime·lfenceBeforeRdtsc* variable. The *runtime·cputicks* method is the only place where this variable is used. This method utilizes a different assembler instruction to get *cpu ticks*depending on the value of *runtime·lfenceBeforeRdtsc*. Finally, we call the CPUID assembler instruction, execute it, and save the result in the *runtime·cpuid_ecx* and *runtime·cpuid_edx*variables. These are used in the *alg.go* file to select a proper hashing algorithm that is natively supported by your computer’s architecture.Ok, let’s move on and examine another portion of code:`01``// if there is an _cgo_init, call it.``02``MOVQ    _cgo_init(SB), AX``03``TESTQ   AX, AX``04``JZ  needtls``05``// g0 already in DI``06``MOVQ    DI, CX  // Win64 uses CX for first parameter``07``MOVQ    $setg_gcc<>(SB), SI``08``CALL    AX``09` `10``// update stackguard after _cgo_init``11``MOVQ    $runtime·g0(SB), CX``12``MOVQ    (g_stack+stack_lo)(CX), AX``13``ADDQ    $const__StackGuard, AX``14``MOVQ    AX, g_stackguard0(CX)``15``MOVQ    AX, g_stackguard1(CX)``16` `17``CMPL    runtime·iswindows(SB), $0``18``JEQ ok`This fragment is only executed when *cgo* is
 enabled. cgo is a topic for a separate discussion and we might talk 
about it in one of the upcoming posts. At this point, we only want to 
understand the basic bootstrap workflow, so we will skip it.The next code fragment is responsible for setting up TLS:`01``needtls:``02``    ``// skip TLS setup on Plan 9``03``    ``CMPL    runtime·isplan9(SB), $1``04``    ``JEQ ok``05``    ``// skip TLS setup on Solaris``06``    ``CMPL    runtime·issolaris(SB), $1``07``    ``JEQ ok``08` `09``    ``LEAQ    runtime·tls0(SB), DI``10``    ``CALL    runtime·settls(SB)``11` `12``    ``// store through it, to make sure it works``13``    ``get_tls(BX)``14``    ``MOVQ    $0x123, g(BX)``15``    ``MOVQ    runtime·tls0(SB), AX``16``    ``CMPQ    AX, $0x123``17``    ``JEQ 2(PC)``18``    ``MOVL    AX, 0   // abort`I have already mentioned TLS before. Now it is time to understand how it is implemented.
 Internal TLS implementationIf you look at the previous code fragment carefully, you can easily understand that the only lines that do actual work are:`1``LEAQ    runtime·tls0(SB), DI``2``    ``CALL    runtime·settls(SB)`All
 the other stuff is used to skip TLS setup when it is not supported on 
your OS and check that TLS works correctly. The two lines above store 
the address of the *runtime·tls0* variable in the DI register and call the *runtime·settls* function. The code of this function is shown below:`01``// set tls base to DI``02``TEXT runtime·settls(SB),NOSPLIT,$32``03``    ``ADDQ    $8, DI  // ELF wants to use -8(FS)``04` `05``    ``MOVQ    DI, SI``06``    ``MOVQ    $0x1002, DI // ARCH_SET_FS``07``    ``MOVQ    $158, AX    // arch_prctl``08``    ``SYSCALL``09``    ``CMPQ    AX, $0xfffffffffffff001``10``    ``JLS 2(PC)``11``    ``MOVL    $0xf1, 0xf1  // crash``12``    ``RET`From the comments, we can understand that this function makes an *arch_prctl* system call and passes *ARCH_SET_FS *as an argument. We can also see that this system call sets a base for the *FS* segment register. In our case, we set TLS to point to the *runtime·tls0* variable.Do you remember the instruction that we saw at the beginning of the assembler code for the main function?`1``0x0000 00000 (test.go:3)    MOVQ    (TLS),CX`I have previously explained that it loads the address of the *runtime.g* structure
 instance into the CX register. This structure describes the current 
goroutine and is stored in thread local storage. Now we can find out and
 understand how this instruction is translated into machine assembler. 
If you open the previously created *disassembly.txt* file and look for the*main.main* function, the first instruction inside it should look like this:`1``400c00:       64 48 8b 0c 25 f0 ff    mov    %fs:0xfffffffffffffff0,%rcx`The colon in this instruction (*%fs:0xfffffffffffffff0*) stands for segmentation addressing (you can read more on it [here](http://thestarman.pcministry.com/asm/debug/Segments.html)).
 Returning to the starting sequenceFinally, let’s look at the last two parts of the *runtime.rt0_go* function:`01``ok:``02``    ``// set the per-goroutine and per-mach "registers"``03``    ``get_tls(BX)``04``    ``LEAQ    runtime·g0(SB), CX``05``    ``MOVQ    CX, g(BX)``06``    ``LEAQ    runtime·m0(SB), AX``07` `08``    ``// save m->g0 = g0``09``    ``MOVQ    CX, m_g0(AX)``10``    ``// save m0 to g0->m``11``    ``MOVQ    AX, g_m(CX)`Here, we load the TLS address into the BX register and save the address of the *runtime·g0*variable in TLS. We also initialize the *runtime.m0* variable. If *runtime.g0* stands for root goroutine, then *runtime.m0* corresponds to the root operating system thread used to run this goroutine. We may take a closer look at *runtime.g0* and *runtime.m0* structures in upcoming blog posts.The
 final part of the starting sequence initializes arguments and calls 
different functions, but this is a topic for a separate discussion.
 More on GolangSo,
 we have learned the inner mechanisms of the bootstrap process and found
 out how stacks are implemented. To move forward, we need to analyze the
 last part of the starting sequence. That will be the subject of my next
 post. If you want to get notified as soon as it comes out, hit the 
subscribe button below or follow [@altoros](http://www.twitter.com/altoros).
*Read all parts of the series: Part 1 | Part 2 | Part 3 | Part 4 | \**Part 5** | Part 6***\*About the author:** Sergey
 Matyukevich is a Cloud Engineer and Go Developer at Altoros. With 6+ 
years in software engineering, he is an expert in cloud automation and 
designing architectures for complex cloud-based systems. An active 
member of the Go community, Sergey is a frequent contributor to 
open-source projects, such as Ubuntu and Juju Charms.*Subscribe to our blog for the next parts of this series or follow [@altoros](https://twitter.com/altoros).
来源： <<http://blog.altoros.com/golang-internals-part-5-runtime-bootstrap-process.html>> 

------

## Golang Internals, Part 6: Bootstrapping and Memory Allocator Initialization

Posted by [Sergey Matyukevich](http://blog.altoros.com/author/sergey-matyukevich), Cloud Engineer at Altoros, in [Research & Development](http://blog.altoros.com/category/research-and-development)

Tags: [Golang](http://blog.altoros.com/tag/golang)

15 Oct 2015

[0 Comments](http://blog.altoros.com/golang-internals-part-6-bootstrapping-and-memory-allocator-initialization.html#disqus_thread)

![Go Gopher]()*All parts:* *Part 1* | *Part 2* | *Part 3* | *Part 4* | *Part 5* | **\*Part 6***This
 post is the continuation of our Golang Internals series. It explores 
the bootstrapping process, which is key to understanding the Go runtime,
 in more detail. In this part, we will run through the second portion of
 the starting sequence, learn how arguments are initialized, what 
functions are called, etc.
The starting sequenceI will pick up our exploration from where we left off last time—the *runtime.rt0_go* function. There is still a part of it that we haven’t looked at:`01``CLD                         // convention is D is always left cleared``02``CALL    runtime·check(SB)``03` `04``MOVL    16(SP), AX          // copy argc``05``MOVL    AX, 0(SP)``06``MOVQ    24(SP), AX          // copy argv``07``MOVQ    AX, 8(SP)``08``CALL    runtime·args(SB)``09``CALL    runtime·osinit(SB)``10``CALL    runtime·schedinit(SB)`The first instruction (CLD) clears the [direction](https://en.wikipedia.org/wiki/Direction_flag) flag of the *FLAGS* register. This flag affects the direction of string processing.The next function is a call to the *runtime.check* function,
 which is also not very valuable for our purposes. The runtime just 
tries to create instances of all built-in types, check their sizes and 
some other parameters, etc. and it *panics* if something goes wrong. You can easily explore this [function](https://github.com/golang/go/blob/go1.5.1/src/runtime/runtime1.go#L136) on your own. Analyzing argumentsThe next function, *runtime.Args*, is somewhat more interesting. Besides storing arguments (*argc* and *argv*) in static variables, on Linux systems, it is responsible for analyzing the ELF auxiliary vector and initializing *syscall* addresses.This
 requires some explanation. When the operating system loads a program 
into memory, it initializes the initial stack for this program with some
 data in a pre-defined format. At the top of the stack, lay the 
arguments—pointers to environment variables. At the bottom, we can find 
the “ELF auxiliary vector,” which is actually an array of records that 
contains some other useful information, for example, the number and size
 of program headers. See this[article](http://articles.manugarg.com/aboutelfauxiliaryvectors) for more on the ELF auxiliary vector format.The *runtime.Args* function is responsible for parsing this vector. Out of all the information that it contains, the runtime only uses *startupRandomData*,
 which mainly serves for initializing hashing functions and pointers to 
locations for some syscalls. The following variables are initialized 
here:`1``__vdso_time_sym``2``__vdso_gettimeofday_sym``3``__vdso_clock_gettime_sym`They
 are used for obtaining the current time in different functions. All 
these variables have default values. This allows Golang to use the *vsyscall* mechanism to call the corresponding functions.
 Inside the *runtime.osinit* functionThe next function called during the startup sequence is *runtime.osinit*. On Linux systems, the only thing it does is initialize the *ncpu* variable that holds the number of CPUs in the system. This is done via a syscall. Inside the *runtime.schedinit* function*runtime.schedinit*—the
 next function in the startup sequence—is more interesting. It begins by
 obtaining the current goroutine, which is, in fact, a pointer to the *g* structure. We have talked about how this pointer is stored when discussing the TLS implementation. Next, it calls *runtime.raceinit*. We will skip the discussion of *runtime.raceinit*,
 because this function is normally not called when checking for race 
conditions is not enabled. After that, some other initialization 
functions are called.Let’s explore them one at a time. Initializing tracebackThe *runtime.tracebackinit* function
 is responsible for initializing traceback. Traceback is a stack of 
functions that were called before we got to the current point of 
execution. For example, we can see it each time a panic occurs. 
Traceback is generated by a given program counter by calling a function 
called *runtime.gentraceback*.
 For this function to work, we need to know the addresses of some 
built-in functions (e.g., because we don’t want them to be included into
 the traceback). *runtime.tracebackinit* is responsible for initializing these addresses. Verifying linker symbolsLinker
 symbols are data emitted by the linker to the executable and the object
 file. Most of these symbols’ contents have been discussed in [Golang Internals, Part 3: The Linker, Object Files, and Relocations](http://blog.altoros.com/golang-internals-part-3-the-linker-and-object-files.html). In the runtime package, linker symbols are mapped to the*moduledata* struct. The *runtime.moduledataverify* function
 is responsible for performing some checks against this data and 
verifying that it has the correct structure and is not corrupted. Initializing the stack poolTo
 understand the next initialization step, you need a bit of knowledge 
about how stack growth is implemented in Go. When a new goroutine is 
created, a small fixed-size stack is allocated for it. When the stack 
reaches some threshold, its size is doubled and the stack is copied to 
another location.There
 is still a lot of detail on how reaching this threshold is determined 
and how Go adjusts pointers in the stack. I have already touched on some
 of these things in my previous blog posts, when talking about the *stackguard0* field and function metadata. You can also find a lot of useful information on this subject in [this document](https://docs.google.com/document/d/1wAaf1rYoM4S4gtnPh0zOlGzWtrZFQ5suE8qr2sD8uWQ/pub).Go uses a stack pool to cache currently unused stacks. The stack pool is an array initialized in the *runtime.stackinit* function. Each item in this array contains a linked list of stacks of the same size.Another variable initialized at this stage is *runtime.stackFreeQueue*.
 It also contains a linked list of stacks, but these are added to the 
list during garbage collection and are cleared after it is finished. 
Note that only 2 KB, 4 KB, 8 KB, and 16 KB stacks are cached. Larger 
ones are allocated directly. Initializing the memory allocatorThe process of memory allocation is described in this [source code comment](https://github.com/golang/go/blob/go1.5.1/src/runtime/malloc.go#L5).
 I strongly encourage you to read it, if you want to understand how 
memory allocation works. This topic will be covered in more detail in 
one of the upcoming posts. Initialization of the memory allocator is 
located in the *runtime.mallocinit* function, so let’s take a closer look at it. Initializing size classesThe first thing we can see here is that *runtime.mallocinit* is calling another function—*initSizes*,
 which is responsible for calculating size classes. But what size does a
 class have? When allocating a small object (less than 32 KB), the Go 
runtime first rounds its size up to a pre-defined class size. So the 
allocated block of memory can only have one of the predefined sizes that
 is usually larger than what is required for the object itself. This 
leads to a small memory wastage, but it enables you to easily re-use 
allocated memory blocks for different objects.The *initSizes* function is responsible for calculating these classes. At the top of this function, we can see the following code:`01``    ``align := 8``02``    ``for size := align; size <= _MaxSmallSize; size += align {``03``        ``if size&(size-1) == 0 {``04``            ``if size >= 2048 {``05``                ``align = 256``06``            ``} else if size >= 128 {``07``                ``align = size / 8``08``            ``} else if size >= 16 {``09``                ``align = 16``10``…``11``            ``}``12``        ``}`As
 we can see, the smallest two size classes are 8 and 16 bytes. 
Subsequent classes are located in every 16 bytes up to 128 bytes. From 
128 to 2,048 bytes, classes are located in every size/8 bytes. After 
2,048 bytes, size classes are located in every 256 bytes.The *initSizes* method initializes the *class_to_size* array, which converts a class (here, by class we mean its index in the class list) to its size. It also initializes the *class_to_allocnpages* array
 that stores data on how many memory pages should be obtained from the 
OS to fill one object of a given class, and two more arrays—*size_to_class8* and *size_to_class128*.
 These serve for conversion from object size to a corresponding class 
index. The first one converts object sizes smaller than 1 KB, and the 
second one is for object sizes of 1–32 KB. Virtual memory reservationThe next thing the *mallocinit* function
 does is reserve virtual memory for future allocations. Let’s see how 
this is done on x64 architectures. First of all, we need to initialize 
the following variables:`1``arenaSize := round(_MaxMem, _PageSize)``2``bitmapSize = arenaSize / (ptrSize * 8 / 4)``3``spansSize = arenaSize / _PageSize * ptrSize``4``spansSize = round(spansSize, _PageSize)`*arenaSize* is
 the maximum amount of virtual memory that can be reserved for object 
allocations. On 64-bit architectures, it is equal to 512 GB.*bitmapSize* corresponds
 to the amount of memory reserved for the garbage collector (GC) bitmap.
 The GC bitmap is a special memory type used to show where exactly 
pointers are located in memory and whether the object, which is pointed 
to, is marked by GC. For each allocated byte, we need two bits to store 
this information. That’s why bitmap size is calculated like so: *arenaSize / (ptrSize \* 8 / 4)**spansSize* is
 the amount of memory reserved for storing an array of pointers to all 
memory spans. A memory span is a structure that wraps a block of memory 
used for object allocations.Once all these variables have been calculated, the actual reservation is done:`1``pSize = bitmapSize + spansSize + arenaSize + _PageSize ``2``p = uintptr(sysReserve(unsafe.Pointer(p), pSize, &reserved))`Finally, we can initialize the *mheap* global variable that is used as central storage for all memory-related objects.`1``p1 := round(p, _PageSize)``2` `3``mheap_.spans = (**mspan)(unsafe.Pointer(p1))``4``mheap_.bitmap = p1 + spansSize``5``mheap_.arena_start = p1 + (spansSize + bitmapSize)``6``mheap_.arena_used = mheap_.arena_start``7``mheap_.arena_end = p + pSize``8``mheap_.arena_reserved = reserved`Note that, from the beginning, *mheap_.arena_used* is initialized with the same address as*mheap_.arena_start*, because nothing has been allocated yet. Initializing the heapNext, the *mHeap_Init* function is called. The first thing that is done here is allocator initialization.`1``fixAlloc_Init(&h.spanalloc, unsafe.Sizeof(mspan{}), recordspan, unsafe.Pointer(h), &memstats.mspan_sys)``2``fixAlloc_Init(&h.cachealloc, unsafe.Sizeof(mcache{}), nil, nil, &memstats.mcache_sys)``3``fixAlloc_Init(&h.specialfinalizeralloc, unsafe.Sizeof(specialfinalizer{}), nil, nil, &memstats.other_sys)``4``fixAlloc_Init(&h.specialprofilealloc, unsafe.Sizeof(specialprofile{}), nil, nil, &memstats.other_sys)`To better understand what an allocator is, let’s see how it is utilized. All allocators operate in the *fixAlloc_Alloc* function, called each time we want to allocate new *mspan*, *mcache*,*specialfinalizer*, and *specialprofile* structs. The main part of this function is:`1``if uintptr(f.nchunk) < f.size {``2``    ``f.chunk = (*uint8)(persistentalloc(_FixAllocChunk, 0, f.stat))``3``    ``f.nchunk = _FixAllocChunk``4``}`It allocates memory, but instead of allocating the actual size of the structure—*f.size* bytes—we set aside *_FixAllocChunk* bytes
 (currently equal to 16 KB). The rest of the available space is stored 
in the allocator. Next time we need to allocate a structure of the same 
type, it will not require calling *persistentalloc*, which can be time consuming.The *persistentalloc* function is responsible for allocating memory that should not be garbage collected. Its workflow is as follows:If the allocated block is larger than 64 KB, it is allocated directly from OS memoryOtherwise, we first need to find a persistent allocator:A
 persistent allocator is attached to each processor. This is done to 
avoid using locks when working with a persistent allocator. So, we try 
to use a persistent allocator from the current processor.If we cannot obtain information about the current processor, a global system allocator is used.If the allocator does not have enough free memory in its cache, we set aside more memory from the OS.The required amount of memory is returned from the allocator’s cacheThe *persistentalloc* and *fixAlloc_Alloc* functions
 work in similar ways. It is possible to say that those functions 
implement two levels of caching. You should also be aware that*persistentalloc* is used not only in *fixAlloc_Alloc*, but also in many other places where we need to allocate persistent memory.Let’s return to the *mHeap_Init* function.
 One more important question to answer here is how the four structures, 
for which allocators were initialized at the beginning of this function,
 are used:*mspan* is a wrapper for a memory block that should be garbage collected. We have talked about it when discussing size classes. A new *mspan* is created when we need to allocate a new object of a particular size class.*mcache* is
 a struct attached to each processor. It is responsible for caching 
spans. The reason for having a separate cache for each processor is to 
avoid locking.*specialfinalizeralloc* is a struct that is allocated when the *runtime.SetFinalizer* function
 is called. This can be done if we want the system to execute some 
cleanup code when an object is cleared. A good example is the *os.NewFile* function that associates a finalizer with each new file. This finalizer should close the OS file descriptor.*specialprofilealloc* is a struct employed in the memory profiler.After initializing memory allocators, the *mHeap_Init* function initializes lists by calling*mSpanList_Init*, which is very simple. All it does is initialize the first entry for the linked list. The *mheap* struct contains a few such linked lists.*mheap.free* and *mheap.busy* are arrays that contain *free* and *busy* lists
 with spans for large objects (larger than 32 KB, but smaller than 1 
MB). Each of these arrays contains one item per each possible size. 
Here, sizes are measured in pages. One page is equal to 32 KB. The first
 item contains a list with 32 KB spans, the second one contains a list 
with 64 KB spans, and so on.*mheap.freelarge* and *mheap.busylarge* are free and busy lists for objects larger than 1 MBThe next step is to initialize *mheap.central*, which stores spans for small objects (less than 32 KB). In *mheap.central*,
 lists are grouped accordingly to their size classes. Initialization is 
very similar to what we have seen previously. It is simply 
initialization of linked lists for each free list. Initializing the cacheNow, we are almost done with memory allocator initialization. The last thing that is left in the *mallocinit* function is *mcache* initialization:`1``_g_ := getg()``2``_g_.m.mcache = allocmcache()`Here, we first obtain the current coroutine. Each goroutine contains a link to the *m* struct. This struct is a wrapper around the operating system thread. Inside this struct, there is a field called *mcache* that is initialized in these lines. The *allocmcache* function calls*fixAlloc_Alloc* to initialize a new *mcache* struct. We have already discussed how allocation is done and the meaning of this struct (see above).A careful reader may notice that I have previously said *mcache* is attached to each processor, but now we see that it is attached to the *m* struct, which corresponds to an OS process, not a processor. And that is correct—*mcache* is
 initialized only for those threads that are currently executed and it 
is re-located to another thread whenever a process switch occurs. More about Go bootstrapping soonIn
 the next post, we will continue discussing the bootstrap process by 
looking at how the garbage collector is initialized and how the main 
goroutine is started. Meanwhile, don’t hesitate to share your thoughts 
and suggestions in the comments below.*Read all parts of the series:* *Part 1* | *Part 2* | *Part 3* | *Part 4* | *Part 5* | **\*Part 6*****\*About the author:** Sergey
 Matyukevich is a Cloud Engineer and Go Developer at Altoros. With 6+ 
years in software engineering, he is an expert in cloud automation and 
designing architectures for complex cloud-based systems. An active 
member of the Go community, Sergey is a frequent contributor to 
open-source projects, such as Ubuntu and Juju Charms.*Subscribe to our blog for the next parts of this series or follow [@altoros](https://twitter.com/altoros).
来源： <<http://blog.altoros.com/golang-internals-part-6-bootstrapping-and-memory-allocator-initialization.html>> 

