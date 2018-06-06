------

Plans for Go 1.4+ garbage collector:

- hybrid stop-the-world/concurrent collector
- stop-the-world part limited by a 10ms deadline
- CPU cores dedicated to running the concurrent collector
- tri-color mark-and-sweep algorithm
- non-generational
- non-compacting
- fully precise
- incurs a small cost if the program is moving pointers around
- lower latency, but most likely also lower throughput, than Go 1.3 GC

Go 1.3 garbage collector updates on top of Go 1.1:

- concurrent sweep (results in smaller pause times)
- fully precise

Go 1.1 garbage collector:

- mark-and-sweep (parallel implementation)
- non-generational
- non-compacting
- mostly precise (except stack frames)
- stop-the-world
- bitmap-based representation
- zero-cost when the program is not allocating memory (that is: shuffling pointers around is as fast as in C, although in practice this runs somewhat slower than C because the Go compiler is not as advanced as C compilers such as GCC)
- supports finalizers on objects
- there is no support for weak references

Go 1.0 garbage collector:

- same as Go 1.1, but instead of being mostly precise the garbage collector is conservative. The conservative GC is able to ignore objects such as []byte.

Replacing the GC with a different one is controversial, for example:

- except for very large heaps, it is unclear whether a generational GC would be faster overall

- package "unsafe" makes it hard to implement fully precise GC and compacting GC

  ​

  来源： <<http://stackoverflow.com/questions/7823725/what-kind-of-garbage-collection-does-go-use>>

   

------