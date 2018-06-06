

**documentation**

> ...
>
> [The Go Programming Language](https://golang.org/)
>
> [Documents](https://golang.org/doc/) [Packages](https://golang.org/pkg/) [The Project](https://golang.org/project/) [Help](https://golang.org/help/) [Blog](https://golang.org/blog/) [Play](http://play.golang.org/) 
>
> # Package pprof
>
>
>
>
> ## Overview ▾
>
> Package pprof writes runtime profiling data in the format expected by the pprof visualization tool. For more information about pprof, see <http://code.google.com/p/google-perftools/>.
>
> ## Index ▾
>
>
> #### Package files
>
> [pprof.go](https://golang.org/src/runtime/pprof/pprof.go)
>
> ## func [Profiles](https://golang.org/src/runtime/pprof/pprof.go?s=3863:3889#L129)
>
> ```
> func Profiles() []*Profile
> ```
>
> Profiles returns a slice of all the known profiles, sorted by name.
>
> ## func [StartCPUProfile](https://golang.org/src/runtime/pprof/pprof.go?s=16488:16527#L560)
>
> ```
> func StartCPUProfile(w io.Writer) error
> ```
>
> StartCPUProfile enables CPU profiling for the current process. While profiling, the profile will be buffered and written to w. StartCPUProfile returns an error if profiling is already enabled.
>
> ## func [StopCPUProfile](https://golang.org/src/runtime/pprof/pprof.go?s=17628:17649#L601)
>
> ```
> func StopCPUProfile()
> ```
>
> StopCPUProfile stops the current CPU profile, if any. StopCPUProfile only returns after all the writes for the profile have completed.
>
> ## func [WriteHeapProfile](https://golang.org/src/runtime/pprof/pprof.go?s=10558:10598#L366)
>
> ```
> func WriteHeapProfile(w io.Writer) error
> ```
>
> WriteHeapProfile is shorthand for Lookup("heap").WriteTo(w, 0). It is preserved for backwards compatibility.
>
> ## type [Profile](https://golang.org/src/runtime/pprof/pprof.go?s=2053:2193#L45)
>
> ```
> type Profile struct {
>         // contains filtered or unexported fields
> }
> ```
>
> A Profile is a collection of stack traces showing the call sequences that led to instances of a particular event, such as allocation. Packages can create and maintain their own profiles; the most common use is for tracking resources that must be explicitly closed, such as files or network connections.
>
> A Profile's methods can be called from multiple goroutines simultaneously.
>
> Each Profile has a unique name. A few profiles are predefined:
>
> ```
> goroutine    - stack traces of all current goroutines
> heap         - a sampling of all heap allocations
> threadcreate - stack traces that led to the creation of new OS threads
> block        - stack traces that led to blocking on synchronization primitives
>
> ```
>
> These predefined profiles maintain themselves and panic on an explicit Add or Remove method call.
>
> The heap profile reports statistics as of the most recently completed garbage collection; it elides more recent allocation to avoid skewing the profile away from live data and toward garbage. If there has been no garbage collection at all, the heap profile reports all known allocations. This exception helps mainly in programs running without garbage collection enabled, usually for debugging purposes.
>
> The CPU profile is not available as a Profile. It has a special API, the StartCPUProfile and StopCPUProfile functions, because it streams output to a writer during profiling.
>
> ### func [Lookup](https://golang.org/src/runtime/pprof/pprof.go?s=3688:3721#L122)
>
> ```
> func Lookup(name string) *Profile
> ```
>
> Lookup returns the profile with the given name, or nil if no such profile exists.
>
> ### func [NewProfile](https://golang.org/src/runtime/pprof/pprof.go?s=3262:3299#L104)
>
> ```
> func NewProfile(name string) *Profile
> ```
>
> NewProfile creates a new profile with the given name. If a profile with that name already exists, NewProfile panics. The convention is to use a 'import/path.' prefix to create separate name spaces for each package.
>
> ### func (*Profile) [Add](https://golang.org/src/runtime/pprof/pprof.go?s=5407:5457#L181)
>
> ```
> func (p *Profile) Add(value interface{}, skip int)
> ```
>
> Add adds the current execution stack to the profile, associated with value. Add stores value in an internal map, so value must be suitable for use as a map key and will not be garbage collected until the corresponding call to Remove. Add panics if the profile already contains a stack for value.
>
> The skip parameter has the same meaning as runtime.Caller's skip and controls where the stack trace begins. Passing skip=0 begins the trace in the function calling Add. For example, given this execution stack:
>
> ```
> Add
> called from rpc.NewClient
> called from mypkg.Run
> called from main.main
>
> ```
>
> Passing skip=0 begins the stack trace at the call to Add inside rpc.NewClient. Passing skip=1 begins the stack trace at the call to NewClient inside mypkg.Run.
>
> ### func (*Profile) [Count](https://golang.org/src/runtime/pprof/pprof.go?s=4481:4510#L154)
>
> ```
> func (p *Profile) Count() int
> ```
>
> Count returns the number of execution stacks currently in the profile.
>
> ### func (*Profile) [Name](https://golang.org/src/runtime/pprof/pprof.go?s=4355:4386#L149)
>
> ```
> func (p *Profile) Name() string
> ```
>
> Name returns this profile's name, which can be passed to Lookup to reobtain the profile.
>
> ### func (*Profile) [Remove](https://golang.org/src/runtime/pprof/pprof.go?s=5946:5989#L202)
>
> ```
> func (p *Profile) Remove(value interface{})
> ```
>
> Remove removes the execution stack associated with value from the profile. It is a no-op if the value is not in the profile.
>
> ### func (*Profile) [WriteTo](https://golang.org/src/runtime/pprof/pprof.go?s=6751:6806#L221)
>
> ```
> func (p *Profile) WriteTo(w io.Writer, debug int) error
> ```
>
> WriteTo writes a pprof-formatted snapshot of the profile to w. If a write to w returns an error, WriteTo returns that error. Otherwise, WriteTo returns nil.
>
> The debug parameter enables additional output. Passing debug=0 prints only the hexadecimal addresses that pprof needs. Passing debug=1 adds comments translating addresses to function names and line numbers, so that a programmer can read the profile without tools.
>
> The predefined profiles may assign meaning to other debug values; for example, when printing the "goroutine" profile, debug=2 means to print the goroutine stacks in the same form that a Go program uses when dying due to an unrecovered panic.
>
> ## Bugs
>
> - [☞](https://golang.org/src/runtime/pprof/pprof.go?s=469:590#L13) Profiles are incomplete and inaccurate on NetBSD and OS X. See https://golang.org/issue/6047 for details.
>
> Build version go1.5.1.
> Except as [noted](https://developers.google.com/site-policies#restrictions), the content of this page is licensed under the Creative Commons Attribution 3.0 License, and code is licensed under a [BSD license](https://golang.org/LICENSE).
> [Terms of Service](https://golang.org/doc/tos.html) | [Privacy Policy](http://www.google.com/intl/en/policies/privacy/)
>
> 来源： <<https://golang.org/pkg/runtime/pprof/>>
>
>  

