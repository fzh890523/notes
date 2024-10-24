

[The Go Programming Language](https://golang.org/)

[Documents](https://golang.org/doc/) [Packages](https://golang.org/pkg/) [The Project](https://golang.org/project/) [Help](https://golang.org/help/) [Blog](https://blog.golang.org/) 

#### Next article

[Testable Examples in Go](https://blog.golang.org/examples)

#### Previous article

[Errors are values](https://blog.golang.org/errors-are-values)

#### Links

- [golang.org](https://golang.org/)
- [Install Go](https://golang.org/doc/install.html)
- [A Tour of Go](https://tour.golang.org/)
- [Go Documentation](https://golang.org/doc/)
- [Go Mailing List](https://groups.google.com/group/golang-nuts)
- [Go on Google+](https://plus.google.com/101406623878176903605)
- [Go+ Community](https://plus.google.com/112164155169467723645/posts)
- [Go on Twitter](https://twitter.com/golang)

[Blog index](https://blog.golang.org/index)

# [The Go Blog](https://blog.golang.org/)

### [Package names](https://blog.golang.org/package-names)

4 February 2015

#### Introduction

Go code is organized into packages. Within a package, code can refer to any identifier (name) defined within, while clients of the package may only reference the package's exported types, functions, constants, and variables. Such references always include the package name as a prefix: `foo.Bar` refers to the exported name `Bar` in the imported package named `foo`.

Good package names make code better. A package's name provides context for its contents, making it easier for clients to understand what the package is for and how to use it. The name also helps package maintainers determine what does and does not belong in the package as it evolves. Well-named packages make it easier to find the code you need.

Effective Go provides [guidelines](https://golang.org/doc/effective_go.html#names) for naming packages, types, functions, and variables. This article expands on that discussion and surveys names found in the standard library. It also discusses bad package names and how to fix them.

#### Package names

Good package names are short and clear. They are lower case, with no `under_scores` or `mixedCaps`. They are often simple nouns, such as:

- `time` (provides functionality for measuring and displaying time)
- `list` (implements a doubly linked list)
- `http` (provides HTTP client and server implementations)

The style of names typical of another language might not be idiomatic in a Go program. Here are two examples of names that might be good style in other languages but do not fit well in Go:

- `computeServiceClient`
- `priority_queue`

A Go package may export several types and functions. For example, a `compute` package could export a `Client`type with methods for using the service as well as functions for partitioning a compute task across several clients.

**Abbreviate judiciously.** Package names may be abbreviated when the abbreviation is familiar to the programmer. Widely-used packages often have compressed names:

- `strconv` (string conversion)
- `syscall` (system call)
- `fmt` (formatted I/O)

On the other hand, if abbreviating a package name makes it ambiguous or unclear, don't do it.

**Don't steal good names from the user.** Avoid giving a package a name that is commonly used in client code. For example, the buffered I/O package is called `bufio`, not `buf`, since `buf` is a good variable name for a buffer.

#### Naming package contents

A package name and its contents' names are coupled, since client code uses them together. When designing a package, take the client's point of view.

**Avoid stutter.** Since client code uses the package name as a prefix when referring to the package contents, the names for those contents need not repeat the package name. The HTTP server provided by the `http` package is called `Server`, not `HTTPServer`. Client code refers to this type as `http.Server`, so there is no ambiguity.

**Simplify function names.** When a function in package pkg returns a value of type `pkg.Pkg` (or `*pkg.Pkg`), the function name can often omit the type name without confusion:

```
start := time.Now()                                  // start is a time.Time
t, err := time.Parse(time.Kitchen, "6:06PM")         // t is a time.Time
```

```
ctx = context.WithTimeout(ctx, 10*time.Millisecond)  // ctx is a context.Context
ip, ok := userip.FromContext(ctx)                    // ip is a net.IP
```

A function named `New` in package `pkg` returns a value of type `pkg.Pkg`. This is a standard entry point for client code using that type:

```
q := list.New()  // q is a *list.List
```

When a function returns a value of type `pkg.T`, where `T` is not `Pkg`, the function name may include `T` to make client code easier to understand. A common situation is a package with multiple New-like functions:

```
d, err := time.ParseDuration("10s")  // d is a time.Duration
elapsed := time.Since(start)         // elapsed is a time.Duration
ticker := time.NewTicker(d)          // ticker is a *time.Ticker
timer := time.NewTimer(d)            // timer is a *time.Timer
```

Types in different packages can have the same name, because from the client's point of view such names are discriminated by the package name. For example, the standard library includes several types named `Reader`, including `jpeg.Reader`, `bufio.Reader`, and `csv.Reader`. Each package name fits with `Reader` to yield a good type name.

If you cannot come up with a package name that's a meaningful prefix for the package's contents, the package abstraction boundary may be wrong. Write code that uses your package as a client would, and restructure your packages if the result seems poor. This approach will yield packages that are easier for clients to understand and for the package developers to maintain.

#### Package paths

A Go package has both a name and a path. The package name is specified in the package statement of its source files; client code uses it as the prefix for the package's exported names. Client code uses the package path when importing the package. By convention, the last element of the package path is the package name:

```
import (
    "fmt"                       // package fmt
    "os/exec"                   // package exec
    "golang.org/x/net/context"  // package context
)
```

Build tools map package paths onto directories. The go tool uses the [GOPATH](https://golang.org/doc/code.html#GOPATH) environment variable to find the source files for path `"github.com/user/hello"` in directory `$GOPATH/src/github.com/user/hello`. (This situation should be familiar, of course, but it's important to be clear about the terminology and structure of packages.)

**Directories.** The standard library uses like directories `crypto`, `container`, `encoding`, and `image` to group packages for related protocols and algorithms. There is no actual relationship among the packages in one of these directories; a directory just provides a way to arrange the files. Any package can import any other package provided the import does not create a cycle.

Just as types in different packages can have the same name without ambiguity, packages in different directories can have the same name. For example, [runtime/pprof](https://golang.org/pkg/runtime/pprof) provides profiling data in the format expected by the [pprof](https://code.google.com/p/gperftools)profiling tool, while [net/http/pprof](https://golang.org/pkg/net/http/pprof) provides HTTP endpoints to present profiling data in this format. Client code uses the package path to import the package, so there is no confusion. If a source file needs to import both `pprof`packages, it can [rename](https://golang.org/ref/spec#Import_declarations) one or both locally. When renaming an imported package, the local name should follow the same guidelines as package names (lower case, no `under_scores` or `mixedCaps`).

#### Bad package names

Bad package names make code harder to navigate and maintain. Here are some guidelines for recognizing and fixing bad names.

**Avoid meaningless package names.** Packages named `util`, `common`, or `misc` provide clients with no sense of what the package contains. This makes it harder for clients to use the package and makes it harder for maintainers to keep the package focused. Over time, they accumulate dependencies that can make compilation significantly and unnecessarily slower, especially in large programs. And since such package names are generic, they are more likely to collide with other packages imported by client code, forcing clients to invent names to distinguish them.

**Break up generic packages.** To fix such packages, look for types and functions with common name elements and pull them into their own package. For example, if you have

```
package util
func NewStringSet(...string) map[string]bool {...}
func SortStringSet(map[string]bool) []string {...}
```

then client code looks like

```
set := util.NewStringSet("c", "a", "b")
fmt.Println(util.SortStringSet(set))
```

Pull these functions out of `util` into a new package, choosing a name that fits the contents:

```
package stringset
func New(...string) map[string]bool {...}
func Sort(map[string]bool) []string {...}
```

then the client code becomes

```
set := stringset.New("c", "a", "b")
fmt.Println(stringset.Sort(set))
```

Once you've made this change, its easier to see how to improve the new package:

```
package stringset
type Set map[string]bool
func New(...string) Set {...}
func (s Set) Sort() []string {...}
```

which yields even simpler client code:

```
set := stringset.New("c", "a", "b")
fmt.Println(set.Sort())
```

The name of the package is a critical piece of its design. Work to eliminate meaningless package names from your projects.

**Don't use a single package for all your APIs.** Many well-intentioned programmers put all the interfaces exposed by their program into a single package named `api`, `types`, or `interfaces`, thinking it makes it easier to find the entry points to their code base. This is a mistake. Such packages suffer from the same problems as those named `util` or `common`, growing without bound, providing no guidance to users, accumulating dependencies, and colliding with other imports. Break them up, perhaps using directories to separate public packages from implementation.

**Avoid unnecessary package name collisions.** While packages in different directories may have the same name, packages that are frequently used together should have distinct names. This reduces confusion and the need for local renaming in client code. For the same reason, avoid using the same name as popular standard packages like `io` or `http`.

#### Conclusion

Package names are central to good naming in Go programs. Take the time to choose good package names and organize your code well. This helps clients understand and use your packages and helps maintainers to grow them gracefully.

#### Further reading

- [Effective Go](https://golang.org/doc/effective_go.html)
- [How to Write Go Code](https://golang.org/doc/code.html)
- [Organizing Go Code (2012 blog post)](https://blog.golang.org/organizing-go-code)
- [Organizing Go Code (2014 Google I/O talk)](https://talks.golang.org/2014/organizeio.slide)

By Sameer Ajmani

Except as [noted](https://developers.google.com/site-policies#restrictions), the content of this page is licensed under the Creative Commons Attribution 3.0 License,
and code is licensed under a [BSD license](https://golang.org/LICENSE).
[Terms of Service](https://golang.org/doc/tos.html) | [Privacy Policy](https://www.google.com/intl/en/policies/privacy/)

来源： <<https://blog.golang.org/package-names>>

 

