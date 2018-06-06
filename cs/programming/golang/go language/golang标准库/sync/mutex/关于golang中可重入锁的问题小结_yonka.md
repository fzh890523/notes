I'm sorry to not answer your question directly:

IMHO, the best way how to implement recursive locks in Go is to not implement them, but rather redesign your code to not need them in the first place. It's probable, I think, that the desire for them indicates a wrong approach to some (unknown here) problem is being used.

As an indirect "proof" of the above claim: Would a recursive lock be a common/correct approach to the/some usual situations involving mutexes, it would be sooner or later included in the standard library.

And finally, last but not least: What Russ Cox from the Go development team wrote here<https://groups.google.com/d/msg/golang-nuts/XqW1qcuZgKg/Ui3nQkeLV80J>:

> Recursive (aka reentrant) mutexes are a bad idea. The fundamental reason to use a mutex is that mutexes protect invariants, perhaps internal invariants like "p.Prev.Next == p for all elements of the ring", or perhaps external invariants like "my local variable x is equal to p.Prev."
>
> Locking a mutex asserts "I need the invariants to hold" and perhaps "I will temporarily break those invariants." Releasing the mutex asserts "I no longer depend on those invariants" and "If I broke them, I have restored them."
>
> Understanding that mutexes protect invariants is essential to identifying where mutexes are needed and where they are not. For example, does a shared counter updated with atomic increment and decrement instructions need a mutex? It depends on the invariants. If the only invariant is that the counter has value i - d after i increments and d decrements, then the atmocity of the instructions ensures the invariants; no mutex is needed. But if the counter must be in sync with some other data structure (perhaps it counts the number of elements on a list), then the atomicity of the individual operations is not enough. Something else, often a mutex, must protect the higher-level invariant. This is the reason that operations on maps in Go are not guaranteed to be atomic: it would add expense without benefit in typical cases.
>
> Let's take a look at recursive mutexes. Suppose we have code like this:

```Go
     func F() {
             mu.Lock()... do some stuff ...
             G()... do some more stuff ...
             mu.Unlock()}

     func G() {
             mu.Lock()... do some stuff ...
             mu.Unlock()}
```

> Normally, when a call to mu.Lock returns, the calling code can now assume that the protected invariants hold, until it calls mu.Unlock.
>
> A recursive mutex implementation would make G's mu.Lock and mu.Unlock calls be no-ops when called from within F or any other context where the current thread already holds mu. If mu used such an implementation, then when mu.Lock returns inside G, the invariants may or may not hold. It depends on what F has done before calling G. Maybe F didn't even realize that G needed those invariants and has broken them (entirely possible, especially in complex code).
>
> Recursive mutexes do not protect invariants. Mutexes have only one job, and recursive mutexes don't do it.
>
> There are simpler problems with them, like if you wrote

```Go
     func F() {
             mu.Lock()... do some stuff
     }
```

> you'd never find the bug in single-threaded testing. But that's just a special case of the bigger problem, which is that they provide no guarantees at all about the invariants that the mutex is meant to protect.
>
> If you need to implement functionality that can be called with or without holding a mutex, the clearest thing to do is to write two versions. For example, instead of the above G, you could write:

```Go
     // To be called with mu already held.// Caller must be careful to ensure that ...
     func g() {... do some stuff ...}

     func G() {
             mu.Lock()
             g()
             mu.Unlock()}
```

> or if they're both unexported, g and gLocked.
>
> I am sure that we'll need TryLock eventually; feel free to send us a CL for that. Lock with timeout seems less essential but if there were a clean implementation (I don't know of one) then maybe it would be okay. Please don't send a CL that implements recursive mutexes.
>
> Recursive mutexes are just a mistake, nothing more than a comfortable home for bugs.
>
> Russ
>
> 
>
> 来源： <<http://stackoverflow.com/questions/14670979/recursive-locking-in-go>>
>
>  

You could quite easily make a recursive lock out of a [sync.Mutex](http://golang.org/pkg/sync/#Mutex) and a [sync.Cond](http://golang.org/pkg/sync/#Cond). See [Appendix A here](http://www.hdfgroup.org/HDF5/doc/TechNotes/ThreadSafeLibrary.html) for some ideas.

**Except** for the fact that the Go runtime doesn't expose any notion of goroutine Id. This is to stop people doing silly things with goroutine local storage, and probably indicates that the designers think that if you need a goroutine Id you are doing it wrong.

You can of course [dig the goroutine Id out of the runtime with a bit of C](https://groups.google.com/d/msg/golang-nuts/Nt0hVV_nqHE/9Qj8zRlZWZ4J) if you really want to. You might want to read that thread to see why the designers of Go think it is a bad idea.