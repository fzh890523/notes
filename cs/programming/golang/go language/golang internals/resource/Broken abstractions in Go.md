# Posted on Monday, March 29, 2010. 

One of my favorite pieces of Go is the implementation of the `go` and `defer` statements, because it seems to cheat. It reuses the function call code in such a way that adding `go` is just a couple lines in the compiler back end, far shorter than it would have been if implemented from scratch. But reusing the function call code requires breaking an abstraction layer in a somewhat non-obvious way. `Defer` cheats in a few more interesting ways too. (I'm not claiming responsibility for any of this clever code. I believe it's all due to Ken Thompson.)

### Go

In Go, the statement `go f(x,y,z)` starts a new goroutine running the function call `f(x,y,z)`. The values of `f`, `x`, `y`, and `z` are all computed in the original goroutine: it is only the execution of `f` that happens in the new goroutine.

Before we go on, stop for a minute and think about how you'd implement this if you were generating C code. I would probably create a struct with fields for `f`, `x`, `y`, and `z`, and then I'd write a function that took a pointer to that struct as input and called `f(x,y,z)`, and then I'd compile the `go` statement by generating code to allocate and fill in the struct and then call a function `go(helper, structptr)` where `helper` is the other function that does the call. It's a fair amount of work, and it would have to happen for each call. The Go implementation of `go`avoids all of that.

Let's defer `go` for a minute and look at a normal function call. Here's the assembly for calling `f(1, 2, 3)`:

```
 MOVL    $1, 0(SP)
 MOVL    $2, 4(SP)
 MOVL    $3, 8(SP)
 CALL    f(SB)

```

It stores `1`, `2`, and `3` on the stack in the places where `f` expects them, and then it executes a `CALL` instruction. Simple enough.

Here's the code for `go f(1, 2, 3)`:

```
 MOVL    $1, 0(SP)
 MOVL    $2, 4(SP)
 MOVL    $3, 8(SP)
 PUSHQ   $f(SB)
 PUSHQ   $12
 CALL    runtime.newproc(SB)
 POPQ    AX
 POPQ    AX

```

This is not something you could have written in C. It starts out by setting up for an ordinary function call, storing the arguments in the usual places. But then at the last minute it swerves: instead of calling `f` directly, it pushes two more arguments onto the stack: `f` and the number `12`, and then it calls a different function `runtime.newproc`.

`Newproc` expects its arguments to be a byte count *n*, the function to call, and then *n* bytes of parameters to the function, already laid out exactly how the function needs to receive them. It copies those *n* bytes to a new stack and starts `f` running with its stack pointer pointing at those arguments. `Newproc` is essentially the helper, but there's just one instance of it.

In the gc compilers (6g, etc), the implementation of `go` calls the same code generator as for an ordinary function call, but it ends with that 5-instruction sequence instead of a simple `CALL` instruction.

This trick eliminates all the special code that I would have generated, requiring only a handful of lines in the runtime and only a handful of lines in the compiler.

### Defer

The statement `defer f(x,y,z)` is like `go f(x,y,z)`, but instead of running the call in a new goroutine, it saves the call for later, running it when the current function eventually returns. The implementation of the statement itself is almost identical to the implementation for `go`, except that the generated code calls `deferproc` instead of `newproc`.

The interesting new broken abstractions for `defer` are in how the deferred functions get called. If a function contains a defer statement, instead of ending with the usual epilogue

```
 ADDL $48, SP // or whatever the frame size is
 RET

```

the function ends with

```
 CALL runtime.deferreturn(SB)
 ADDL $48, SP
 RET

```

The runtime function `deferreturn` arranges for the deferred calls, if any, to run. How? Each goroutine has associated with it a linked list of deferred calls, `Defer` structures:

```
struct Defer
{
 int32 siz;
 byte* sp;
 byte* fn;
 Defer* link;
 byte args[8]; // padded to actual size
};

```

The struct represents a deferred call to `fn` with the `siz` bytes of arguments stored in `args`. (The struct is allocated with enough space on the end even if `siz > 8`.) The `link` field is for the linked list. What about `sp`? That's how `calldefer` knows whether it's time to run a particular call. The `sp` field records the address of the first argument (after `fn`) in the call to `deferproc`. If the first argument to `calldefer` is at the same address, then the `Defer` is for this call frame. If not, it is for a different call frame, and since defers run when a function exits, it must be for a frame higher up the stack:

```
void
deferreturn(uintptr arg0)
{
 byte *sp, *fn;
 Defer *d;

 sp = (byte*)&arg0;
 d = g->defer;
 if(d == nil || d->sp != sp)
  return;
 mcpy(d->sp, d->args, d->siz);
 g->defer = d->link;
 fn = d->fn;
 free(d);
 jmpdefer(fn, sp);
}

```

If you were writing this in pure C, you'd probably have a separate stack for each call frame, or maybe on entry to a function with a `defer` statement you'd push a special marker onto the defer stack and then, at the end, run deferred calls until you found the marker. But if you work at the lower-level world of machine instructions, the stack pointer is a perfectly good unique identifier of a particular call frame; using it avoids any work on the way into the function.

If there is a deferred call to run, then `deferreturn` copies the arguments to the stack—there's definitely room, because that's the same address `deferproc` copied them from—frees the defer stub, and then calls the assembly function `jmpdefer` to transfer control to `fn` as though the original function had called `fn` directly instead of `deferreturn`.

But wait! That only takes care of a single deferred call, yet a function can defer many calls during the course of its execution. How can that work?

Well, the assembly trampoline `jmpdefer` has one more abstraction breaker up its sleeve. It subtracts five—the size of `CALL` instruction that invoked `deferreturn`—from the return address on the stack before jumping to `fn`, so the deferred function returns not to the instruction *after*the `CALL`, as it normally would, but back to the `CALL` itself. That is, subtracting five turns the `CALL` instruction into a loop. The only way out of the loop is for `deferreturn` to find no work left for this call frame and return normally, without calling `jmpdefer`. This subterfuge avoids the need to write a loop at the end of every function with a defer statement.

### On abstraction

In today's programming world, there seems to be a lot of emphasis on the power of abstraction. I think there's not enough emphasis on the power of breaking abstractions. All three of these places where the Go implementation breaks the abstraction are more efficient than if it had colored within the abstraction boundaries.

All three are also the kinds of tricks that were commonplace in the early days of Unix, since it had been written in assembly. For example, the original `fork` system call handler distinguished parent from child by changing the return address just as `jmpdefer` does. In modern Unix, the`fork` system call returns the new process id in the parent but returns zero in the child. In the early versions, including Sixth Edition, `fork` returns the new process id in both, but the child returned normally while the parent returned to one instruction past the usual return address. Thus the instruction after invoking the fork system call needed to be an unconditional jump to the child-specific code.

There are other, more fundamental abstractions broken in Go. The implementation of segmented stacks in Go breaks the simple abstraction of a stack that most C compilers assume (more on that in another post). The idea that an object can implement an interface without explicitly declaring that fact is foreign to Java: it is impossible to compile Go to standard Java byte codes, because Go's interfaces break the JVM's abstraction.

Ultimately, I think the reason I like all these broken abstractions is that they help you get to a better understanding of the system as a whole. Where before you only saw two different layers, you now begin to see how the layers are related and how they can interact. And every broken abstraction is a chance to see or create a new concept that may not have even been expressible before.

(Comments originally posted via Blogger.)

- [Barry Kelly](http://www.blogger.com/profile/10559947643606684495) (March 29, 2010 9:49 AM) It's
   expressly the job of a compiler to remove abstractions from code. 
  Ultimately it needs to remove all abstractions, until the remaining 
  bitstream is able to signal the hardware to generate the desired effects
   in the physical world.
  So yes, when you're implementing a 
  compiler, you need to be aware of the fact that it's your job to destroy
   abstraction, to map it into appropriate layers with as much efficiency 
  as correctness will allow. But that's not the same thing as saying that 
  breaking abstractions is a good thing.


- [Autodidactic Asphyxiation](http://www.blogger.com/profile/09787472194187459747) (March 29, 2010 9:57 AM) Are parameters passed on the stack in 64-bit, as well?
  How does the call/ret-5 subterfuge effect the call/ret prediction on modern processors?


- [Russ Cox](http://swtch.com/~rsc/) (March 29, 2010 10:21 AM) @barry: Not a good thing if done all the time, but certainly a good thing once in a while.
  @aa:
   The fact that f called deferreturn, which called jmpdefer, which is 
  returning straight to f, already broke the RET prediction. Rewinding the
   CALL instruction doesn't make it any worse. ;-)


- [Peter](http://www.blogger.com/profile/03688076015831464616) (May 19, 2010 9:21 PM) I am pretty sure you can compile Go to JVM by messing around with Java interfaces sufficiently. It wouldn't be pretty, that's for sure. :-D


- [IDisposable](http://www.blogger.com/profile/02275315449689041289) (January 4, 2011 5:30 PM) Why
   doesn't the go helper runtime.newproc pop the known-to-be function 
  address and arglist byte-size automatically instead of relying on the 
  call-site to pop them? That would turn this:
  MOVL $1, 0(SP)
  MOVL $2, 4(SP)
  MOVL $3, 8(SP)
  PUSHQ $f(SB)
  PUSHQ $12
  CALL runtime.newproc(SB)
  POPQ AX
  POPQ AX
  into
  MOVL $1, 0(SP)
  MOVL $2, 4(SP)
  MOVL $3, 8(SP)
  PUSHQ $f(SB)
  PUSHQ $12
  CALL runtime.newproc(SB)
  which would be much better on pipeline sizes?


- [Carsten Milkau](http://www.blogger.com/profile/10773876428711731299) (May 27, 2011 5:08 AM) I wonder why
  defer <FunctionBody>
  go <FunctionBody>
  haven't been turned into (lexical) synonyms for
  defer func() <FunctionBody> ()
  go func() <FunctionBody> ()
  following the general design pattern of go to avoid boilerplate. 
  IMHO
   this is fairly straightforward and unambiguous, in mind that anonymous 
  functions are closures. The defer and go keywords are prominent enough 
  to remind this is an anonymous function call and not an ordinary block. 
  Still, in many cases that difference doesn't matter, and in those cases 
  the resemblance is even a plus.

来源： <<http://research.swtch.com/goabstract>>

 



