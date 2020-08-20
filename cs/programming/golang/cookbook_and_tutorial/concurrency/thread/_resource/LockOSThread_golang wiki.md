

# LockOSThread

Jeremy Jackins edited this page on 27 Jan · [4 revisions](https://github.com/golang/go/wiki/LockOSThread/_history)

### 

###  Pages 97

# Contents

- [Home](https://github.com/golang/go/wiki/)
- [Getting started with Go](https://github.com/golang/go/wiki/#getting-started-with-go)
- [Working with Go](https://github.com/golang/go/wiki/#working-with-go)
- [Learning more about Go](https://github.com/golang/go/wiki/#learning-more-about-go)
- [The Go Community](https://github.com/golang/go/wiki/#the-go-community)
- [Using the go toolchain](https://github.com/golang/go/wiki/#using-the-go-toolchain)
- [Additional Go Programming Wikis](https://github.com/golang/go/wiki/#additional-go-programming-wikis)
- [Online Services that work with Go](https://github.com/golang/go/wiki/#online-services-that-work-with-go)
- [Troubleshooting Go Programs in Production](https://github.com/golang/go/wiki/#troubleshooting-go-programs-in-production)
- [Contributing to the Go Project](https://github.com/golang/go/wiki/#contributing-to-the-go-project)
- [Platform Specific Information](https://github.com/golang/go/wiki/#platform-specific-information)
- [Release Specific Information](https://github.com/golang/go/wiki/#release-specific-information)

##### Clone this wiki locally

# 

# Introduction

Some libraries, especially graphical frameworks/libraries like Cocoa, OpenGL, libSDL all require it's called from the main OS thread or called from the same OS thread due to its use of thread local data structures. Go's runtime provides `LockOSThread()` function for this, but it's notoriously difficult to use correctly.

# 

# Solutions

Russ Cox presented a good solution for this problem in this [thread](https://groups.google.com/d/msg/golang-nuts/IiWZ2hUuLDA/SNKYYZBelsYJ).

```
package sdl

// Arrange that main.main runs on main thread.func init() {
    runtime.LockOSThread()
}

// Main runs the main SDL service loop.// The binary's main.main must call sdl.Main() to run this loop.// Main does not return. If the binary needs to do other work, it// must do it in separate goroutines.func Main() {
    for f := range mainfunc {
        f()
    }
}

// queue of work to run in main thread.var mainfunc = make(chan func())

// do runs f on the main thread.func do(f func()) {
    done := make(chan bool, 1)
    mainfunc <- func() {
        f()
        done <- true
    }
    <-done
}
```

And then other functions you write in package sdl can be like

```
func Beep() {
    do(func() {
        // whatever must run in main thread
    })
}
```

来源： <<https://github.com/golang/go/wiki/LockOSThread>>

 

