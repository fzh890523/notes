

# How does the go build command work ?

This post explains how the Go build process works using examples from Go’s standard library.

# The gc toolchain

This article focuses on the gc toolchain. The gc toolchain takes its name for the Go compiler frontend, `cmd/gc`, and is mainly used to distinguish it from the `gccgo` toolchain. When people talk about the Go compilers, they are likely referring to the gc toolchain. This article won’t be focusing on the `gccgo` toolchain.

The gc toolchain is a direct descendant of the Plan 9 toolchain. The toolchain consists of a Go compiler, a C compiler, an assembler and a linker. Each of these tools are found in the src/`cmd/`subdirectory of the Go source and consist of a frontend, which is shared across all implementations, and a backend which is specific to the processor architecture. The backends are distinguished by their letter, which is again a Plan 9 tradition. The commands are:

- `5g`, `6g`, and `8g` are the compilers for `.go` files for `arm`, `amd64` and `386`
- `5c`, `6c`, and `8c` are the compilers for `.c` files for `arm`, `amd64` and `386`
- `5a`, `6a`, and `8a` are the assemblers for `.s` files for `arm`, `and64` and `386`
- `5l`, `6l`, and `8l` are the linkers for files produced by the commands above, again for `arm`,`amd64` and `386`.

It should be noted that each of these commands can be compiled on any supported platform and this forms the basis of Go’s cross compilation abilities. You can read more about [cross compilation in this article](http://dave.cheney.net/2013/07/09/an-introduction-to-cross-compilation-with-go-1-1).

# Building packages

Building a Go package involves at least two steps, compiling the `.go` files then packing the results into an archive. In this example I’m going to use `crypto/hmac` as it is a small package, only one source and one test file. Using the `-x` option I’ve asked `go build` to print out every step as it executes them

```
% go build -x crypto/hmac
WORK=/tmp/go-build249279931
mkdir -p $WORK/crypto/hmac/_obj/
mkdir -p $WORK/crypto/
cd /home/dfc/go/src/pkg/crypto/hmac
/home/dfc/go/pkg/tool/linux_arm/5g -o $WORK/crypto/hmac/_obj/_go_.5 -p crypto/hmac -complete -D _/home/dfc/go/src/pkg/crypto/hmac -I $WORK ./hmac.go
/home/dfc/go/pkg/tool/linux_arm/pack grcP $WORK $WORK/crypto/hmac.a $WORK/crypto/hmac/_obj/_go_.5
```

Stepping through each of these steps

```
WORK=/tmp/go-build249279931
mkdir -p $WORK/crypto/hmac/_obj/
mkdir -p $WORK/crypto/
```

`go build` creates a temporary directory, `/tmp/go-build249279931` and populates it with some skeleton subdirectories to hold the results of the compilation. The second `mkdir` may be redundant, [issue 6538](https://code.google.com/p/go/issues/detail?id=6583) has been created to track this.

```
cd /home/dfc/go/src/pkg/crypto/hmac
/home/dfc/go/pkg/tool/linux_arm/5g -o $WORK/crypto/hmac/_obj/_go_.5 -p crypto/hmac -complete -D _/home/dfc/go/src/pkg/crypto/hmac -I $WORK ./hmac.go
```

The `go` tool switches the the source directory of `crypto/hmac` and invokes the go compiler for this architecture, in this case `5g`. In reality there is no `cd`, `/home/dfc/go/src/pkg/crypto/hmac` is the supplied as the `exec.Command.Dir` field when `5g` is executed. This means the `.go` source files can be relative to their source directory, making the command line shorter.

The compiler produces a single temporary output file in `$WORK/crypto/hmac/_obj/_go_.5` which will be used in the final step.

```
/home/dfc/go/pkg/tool/linux_arm/pack grcP $WORK $WORK/crypto/hmac.a $WORK/crypto/hmac/_obj/_go_.5
```

The final step is to pack the object file into an archive file, `.a`, which the linker and the compiler consume.

Because we invoked `go build` on a package, the result is discarded as `$WORK` is deleted after the build completes. If we invoke `go install -x` two additional lines appear in the output

```
mkdir -p /home/dfc/go/pkg/linux_arm/crypto/
cp $WORK/crypto/hmac.a /home/dfc/go/pkg/linux_arm/crypto/hmac.a
```

This demonstrates the difference between `go build` and `install`; `build` builds, `install` builds then installs the result to be used by other builds.

# Building more complex packages

You may be wondering what the pack step in the previous example does. As the compiler and linker only accept a single file representing the contents of the package, if a package contains multiple object files, they must be packed into a single `.a` archive before they can be used.

A common example of a package producing more than one intermediary object file is cgo, but that is too complicated for this article, instead a simpler example is a package that contains some `.s` assembly files, like `crypto/md5`.

```
% go build -x crypto/md5
WORK=/tmp/go-build870993883
mkdir -p $WORK/crypto/md5/_obj/
mkdir -p $WORK/crypto/
cd /home/dfc/go/src/pkg/crypto/md5
/home/dfc/go/pkg/tool/linux_amd64/6g -o $WORK/crypto/md5/_obj/_go_.6 -p crypto/md5 -D _/home/dfc/go/src/pkg/crypto/md5 -I $WORK ./md5.go ./md5block_decl.go
/home/dfc/go/pkg/tool/linux_amd64/6a -I $WORK/crypto/md5/_obj/ -o $WORK/crypto/md5/_obj/md5block_amd64.6 -D GOOS_linux -D GOARCH_amd64 ./md5block_amd64.s
/home/dfc/go/pkg/tool/linux_amd64/pack grcP $WORK $WORK/crypto/md5.a $WORK/crypto/md5/_obj/_go_.6 $WORK/crypto/md5/_obj/md5block_amd64.6
```

In this example, executed on a `linux/amd64` host, `6g` is invoked to compile two `.go` files, `md5.go` and`md5block_decl.go`. The latter contains the [forward declaration](http://dave.cheney.net/2013/09/07/how-to-include-c-code-in-your-go-package) for the functions implemented in assembly.

`6a` is then invoked to assemble `md5block_amd64.s`. The logic for choosing which `.s` to compile is described in my [previous article on conditional compilation](http://dave.cheney.net/2013/10/12/how-to-use-conditional-compilation-with-the-go-build-tool).

Finally `pack` is invoked to pack the Go object file, `_go_.6`, and the assembly object file,`md5block_amd64.6,` into a single archive.

# Building commands

A Go command is a package who’s name is `main`. Main packages, or commands, are compiled just like other packages, but then undergo several additional steps to be linked into final executable. Let’s investigate this process with `cmd/gofmt`

```
% go build -x cmd/gofmt
WORK=/tmp/go-build979246884
mkdir -p $WORK/cmd/gofmt/_obj/
mkdir -p $WORK/cmd/gofmt/_obj/exe/
cd /home/dfc/go/src/cmd/gofmt
/home/dfc/go/pkg/tool/linux_amd64/6g -o $WORK/cmd/gofmt/_obj/_go_.6 -p cmd/gofmt -complete -D _/home/dfc/go/src/cmd/gofmt -I $WORK ./doc.go ./gofmt.go ./rewrite.go ./simplify.go
/home/dfc/go/pkg/tool/linux_amd64/pack grcP $WORK $WORK/cmd/gofmt.a $WORK/cmd/gofmt/_obj/_go_.6
cd .
/home/dfc/go/pkg/tool/linux_amd64/6l -o $WORK/cmd/gofmt/_obj/exe/a.out -L $WORK $WORK/cmd/gofmt.a
cp $WORK/cmd/gofmt/_obj/exe/a.out gofmt
```

The first six lines should be familiar, `main` packages are compiled like any other Go package, they are even packed like any other package.

The difference is the penultimate line, which invokes the linker to produce a binary executable.

```
/home/dfc/go/pkg/tool/linux_amd64/6l -o $WORK/cmd/gofmt/_obj/exe/a.out -L $WORK $WORK/cmd/gofmt.a
```

The final line copies and renames the completed binary to its final name. If you had used `go install` the binary would be copied to `$GOPATH/bin` (or `$GOBIN` if set).

# A little history

If you go far enough back in time, back before the `go` tool, back to the time of `Makefile`s, you can still find the core of the Go compilation process. This example is taken from the [release.r60](http://code.google.com/p/go/source/browse/doc/install.html?name=release.r60#214) documentation

```
$ cat >hello.go <<EOF
package main

import "fmt"

func main() {
        fmt.Printf("hello, world\n")
}
EOF
$ 6g hello.go
$ 6l hello.6
$ ./6.out
hello, world
```

It’s all here, `6g` compiling a `.go` file into a `.6` object file, `6l` linking the object file against the `fmt`(and `runtime`) packages to produce a binary, `6.out.`

# Wrapping up

In this post we’ve talked about how `go build` works and touched on how `go install` differs in its treatment of the compilation result.

Now that you know how `go build` works, and how to investigate the build process with `-x`, try passing that flag to `go test` and observe the result.

Additionally, if you have `gccgo` installed on your system, you can pass `-compiler gccgo` to `go build`, and using `-x` investigate how Go code is built using this compiler.

### Related Posts:

1. [gb, a project based build tool for the Go programming language](http://dave.cheney.net/2015/06/09/gb-a-project-based-build-tool-for-the-go-programming-language)
2. [How Go uses Go to build itself](http://dave.cheney.net/2013/06/04/how-go-uses-go-to-build-itself)
3. [What does go build build ?](http://dave.cheney.net/2014/06/04/what-does-go-build-build)
4. [Using // +build to switch between debug and release builds](http://dave.cheney.net/2014/09/28/using-build-to-switch-between-debug-and-release)

This entry was posted in [Go](http://dave.cheney.net/category/golang), [Programming](http://dave.cheney.net/category/programming-2) and tagged [compiler](http://dave.cheney.net/tag/compiler), [toolchain](http://dave.cheney.net/tag/toolchain) on [October 15, 2013](http://dave.cheney.net/2013/10/15/how-does-the-go-build-command-work).

来源： <<http://dave.cheney.net/2013/10/15/how-does-the-go-build-command-work>>

