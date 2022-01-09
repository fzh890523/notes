

**documentation**

> ...
>
> [The Go Programming Language](https://golang.org/)
>
> [Documents](https://golang.org/doc/) [Packages](https://golang.org/pkg/) [The Project](https://golang.org/project/) [Help](https://golang.org/help/) [Blog](https://golang.org/blog/) [Play](http://play.golang.org/) 
>
> # Package debug
>
>
>
>
> ## Overview ▾
>
> Package debug contains facilities for programs to debug themselves while they are running.
>
> ## Index ▾
>
>
> #### Package files
>
> [garbage.go](https://golang.org/src/runtime/debug/garbage.go) [stack.go](https://golang.org/src/runtime/debug/stack.go) [stubs.go](https://golang.org/src/runtime/debug/stubs.go)
>
> ## func [FreeOSMemory](https://golang.org/src/runtime/debug/garbage.go?s=4066:4085#L97)
>
> ```
> func FreeOSMemory()
> ```
>
> FreeOSMemory forces a garbage collection followed by an attempt to return as much memory to the operating system as possible. (Even if this is not called, the runtime gradually returns memory to the operating system in a background task.)
>
> ## func [PrintStack](https://golang.org/src/runtime/debug/stack.go?s=516:533#L15)
>
> ```
> func PrintStack()
> ```
>
> PrintStack prints to standard error the stack trace returned by Stack.
>
> ## func [ReadGCStats](https://golang.org/src/runtime/debug/garbage.go?s=1207:1239#L21)
>
> ```
> func ReadGCStats(stats *GCStats)
> ```
>
> ReadGCStats reads statistics about garbage collection into stats. The number of entries in the pause history is system-dependent; stats.Pause slice will be reused if large enough, reallocated otherwise. ReadGCStats may use the full capacity of the stats.Pause slice. If stats.PauseQuantiles is non-empty, ReadGCStats fills it with quantiles summarizing the distribution of pause time. For example, if len(stats.PauseQuantiles) is 5, it will be filled with the minimum, 25%, 50%, 75%, and maximum pause times.
>
> ## func [SetGCPercent](https://golang.org/src/runtime/debug/garbage.go?s=3707:3741#L87)
>
> ```
> func SetGCPercent(percent int) int
> ```
>
> SetGCPercent sets the garbage collection target percentage: a collection is triggered when the ratio of freshly allocated data to live data remaining after the previous collection reaches this percentage. SetGCPercent returns the previous setting. The initial setting is the value of the GOGC environment variable at startup, or 100 if the variable is not set. A negative percentage disables garbage collection.
>
> ## func [SetMaxStack](https://golang.org/src/runtime/debug/garbage.go?s=4568:4599#L111)
>
> ```
> func SetMaxStack(bytes int) int
> ```
>
> SetMaxStack sets the maximum amount of memory that can be used by a single goroutine stack. If any goroutine exceeds this limit while growing its stack, the program crashes. SetMaxStack returns the previous setting. The initial setting is 1 GB on 64-bit systems, 250 MB on 32-bit systems.
>
> SetMaxStack is useful mainly for limiting the damage done by goroutines that enter an infinite recursion. It only limits future stack growth.
>
> ## func [SetMaxThreads](https://golang.org/src/runtime/debug/garbage.go?s=5412:5447#L129)
>
> ```
> func SetMaxThreads(threads int) int
> ```
>
> SetMaxThreads sets the maximum number of operating system threads that the Go program can use. If it attempts to use more than this many, the program crashes. SetMaxThreads returns the previous setting. The initial setting is 10,000 threads.
>
> The limit controls the number of operating system threads, not the number of goroutines. A Go program creates a new thread only when a goroutine is ready to run but all the existing threads are blocked in system calls, cgo calls, or are locked to other goroutines due to use of runtime.LockOSThread.
>
> SetMaxThreads is useful mainly for limiting the damage done by programs that create an unbounded number of threads. The idea is to take down the program before it takes down the operating system.
>
> ## func [SetPanicOnFault](https://golang.org/src/runtime/debug/garbage.go?s=6072:6111#L142)
>
> ```
> func SetPanicOnFault(enabled bool) bool
> ```
>
> SetPanicOnFault controls the runtime's behavior when a program faults at an unexpected (non-nil) address. Such faults are typically caused by bugs such as runtime memory corruption, so the default response is to crash the program. Programs working with memory-mapped files or unsafe manipulation of memory may cause faults at non-nil addresses in less dramatic situations; SetPanicOnFault allows such programs to request that the runtime trigger only a panic, not a crash. SetPanicOnFault applies only to the current goroutine. It returns the previous setting.
>
> ## func [Stack](https://golang.org/src/runtime/debug/stack.go?s=902:921#L25)
>
> ```
> func Stack() []byte
> ```
>
> Stack returns a formatted stack trace of the goroutine that calls it. For each routine, it includes the source line information and PC value, then attempts to discover, for Go functions, the calling function or method and the text of the line containing the invocation.
>
> Deprecated: Use package runtime's Stack instead.
>
> ## func [WriteHeapDump](https://golang.org/src/runtime/debug/garbage.go?s=6328:6358#L149)
>
> ```
> func WriteHeapDump(fd uintptr)
> ```
>
> WriteHeapDump writes a description of the heap and the objects in it to the given file descriptor. The heap dump format is defined at <https://golang.org/s/go13heapdump>.
>
> ## type [GCStats](https://golang.org/src/runtime/debug/garbage.go?s=280:672#L4)
>
> ```
> type GCStats struct {
>         LastGC         time.Time       // time of last collection
>         NumGC          int64           // number of garbage collections
>         PauseTotal     time.Duration   // total pause for all collections
>         Pause          []time.Duration // pause history, most recent first
>         PauseEnd       []time.Time     // pause end times history, most recent first
>         PauseQuantiles []time.Duration
> }
> ```
>
> GCStats collect information about recent garbage collections.
>
> Build version go1.5.1.
> Except as [noted](https://developers.google.com/site-policies#restrictions), the content of this page is licensed under the Creative Commons Attribution 3.0 License, and code is licensed under a [BSD license](https://golang.org/LICENSE).
> [Terms of Service](https://golang.org/doc/tos.html) | [Privacy Policy](http://www.google.com/intl/en/policies/privacy/)
>
> 来源： <<https://golang.org/pkg/runtime/debug/#GCStats>>
>
>  



## GODEBUG env



https://pkg.go.dev/runtime#hdr-Environment_Variables

`env GODEBUG=schedtrace=1000 godoc -http=:8080 -index`

[A whirlwind tour of Go’s runtime environment variables](https://dave.cheney.net/tag/godebug)



