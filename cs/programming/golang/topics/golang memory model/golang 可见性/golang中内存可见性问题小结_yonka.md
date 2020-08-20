golang中内存可见性问题小结_yonka















**为什么没有volatile**

> **TL;DR**: Go does not have a keyword to make a variable safe for multiple goroutines to write/read it. Use the `sync/atomic` package for that. Or better yet [Do not communicate by sharing memory; instead, share memory by communicating](http://blog.golang.org/share-memory-by-communicating).
>
> ------
>
> Two answers for the [two meanings of `volatile`](http://www.drdobbs.com/parallel/volatile-vs-volatile/212701484) ![volatile Ven diagram](https://kshttps0.wiz.cn/ks/note/view/89077880-eff4-11e0-a402-00237def97cc/07931014-80c3-4fd2-b38c-077138c733c0/index_files/311b9446-bc63-44cb-ad8a-98019cade16c.gif)
>
> # .NET/Java *concurrency*
>
> Some excerpts from the [Go Memory Model](http://golang.org/ref/mem).
>
> > If the effects of a goroutine must be observed by another goroutine, use a synchronization mechanism such as a lock or channel communication to establish a relative ordering.
>
> One of the examples from the [Incorrect Synchronization](http://golang.org/ref/mem#tmp_9) section is an example of busy waiting on value.
>
> > Worse, there is no guarantee that the write to done will ever be observed by main, since there are no synchronization events between the two threads. The loop in main is not guaranteed to finish.
>
> Indeed, this code([play.golang.org/p/K8ndH7DUzq](http://play.golang.org/p/K8ndH7DUzq)) never exits.
>
> # C/C++ *non-standard memory*
>
> Go's memory model does not provide a way to address non-standard memory. If you have raw access to a device's I/O bus you'll need to use assembly or C to safely write values to the memory locations. I have only ever needed to do this in a device driver which generally precludes use of Go.
>
> 来源： <<http://stackoverflow.com/questions/18491032/does-go-support-volatile-non-volatile-variables>>
>
>  
>
> 
>
> ------
>
> The simple answer is that volatile is not supported by the current Go specification, period.
>
> If you do have one of the use cases where volatile is necessary, such as low-level atomic memory access that is unsupported by [existing](http://golang.org/pkg/sync/) [packages](http://golang.org/pkg/sync/atomic/) in the standard library, or unbuffered access to hardware mapped memory, you'll need to link in a C or assembly file.
>
> Note that if you do use C or assembly as understood by the GC compiler suite, you don't even need cgo for that, since the [568]c C/asm compilers are also able to handle it.
>
> You can find examples of that in Go's source code. For example:
>
> - <http://golang.org/src/pkg/runtime/sema.goc>
> - <http://golang.org/src/pkg/runtime/atomic_arm.c>
>
> Grep for many other instances.
>
> For how memory access in Go does work, check out [The Go Memory Model](http://golang.org/pkg/sync/).









