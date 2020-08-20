

# Package testing




## Overview ▾

Package testing provides support for automated testing of Go packages. It is intended to be used in concert with the “go test” command, which automates execution of any function of the form

```
func TestXxx(*testing.T)

```

where Xxx can be any alphanumeric string (but the first letter must not be in [a-z]) and serves to identify the test routine.

Within these functions, use the Error, Fail or related methods to signal failure.

To write a new test suite, create a file whose name ends _test.go that contains the TestXxx functions as described here. Put the file in the same package as the one being tested. The file will be excluded from regular package builds but will be included when the “go test” command is run. For more detail, run “go help test” and “go help testflag”.

Tests and benchmarks may be skipped if not applicable with a call to the Skip method of *T and *B:

```
func TestTimeConsuming(t *testing.T) {
    if testing.Short() {
        t.Skip("skipping test in short mode.")
    }
    ...
}

```

### Benchmarks

Functions of the form

```
func BenchmarkXxx(*testing.B)

```

are considered benchmarks, and are executed by the "go test" command when its -bench flag is provided. Benchmarks are run sequentially.

For a description of the testing flags, see <http://golang.org/cmd/go/#hdr-Description_of_testing_flags>.

A sample benchmark function looks like this:

```
func BenchmarkHello(b *testing.B) {
    for i := 0; i < b.N; i++ {
        fmt.Sprintf("hello")
    }
}

```

The benchmark function must run the target code b.N times. During benchark execution, b.N is adjusted until the benchmark function lasts long enough to be timed reliably. The output

```
BenchmarkHello    10000000    282 ns/op

```

means that the loop ran 10000000 times at a speed of 282 ns per loop.

If a benchmark needs some expensive setup before running, the timer may be reset:

```
func BenchmarkBigLen(b *testing.B) {
    big := NewBig()
    b.ResetTimer()
    for i := 0; i < b.N; i++ {
        big.Len()
    }
}

```

If a benchmark needs to test performance in a parallel setting, it may use the RunParallel helper function; such benchmarks are intended to be used with the go test -cpu flag:

```
func BenchmarkTemplateParallel(b *testing.B) {
    templ := template.Must(template.New("test").Parse("Hello, {{.}}!"))
    b.RunParallel(func(pb *testing.PB) {
        var buf bytes.Buffer
        for pb.Next() {
            buf.Reset()
            templ.Execute(&buf, "World")
        }
    })
}

```

### Examples

The package also runs and verifies example code. Example functions may include a concluding line comment that begins with "Output:" and is compared with the standard output of the function when the tests are run. (The comparison ignores leading and trailing space.) These are examples of an example:

```
func ExampleHello() {
        fmt.Println("hello")
        // Output: hello
}

func ExampleSalutations() {
        fmt.Println("hello, and")
        fmt.Println("goodbye")
        // Output:
        // hello, and
        // goodbye
}

```

Example functions without output comments are compiled but not executed.

The naming convention to declare examples for the package, a function F, a type T and method M on type T are:

```
func Example() { ... }
func ExampleF() { ... }
func ExampleT() { ... }
func ExampleT_M() { ... }

```

Multiple example functions for a package/type/function/method may be provided by appending a distinct suffix to the name. The suffix must start with a lower-case letter.

```
func Example_suffix() { ... }
func ExampleF_suffix() { ... }
func ExampleT_suffix() { ... }
func ExampleT_M_suffix() { ... }

```

The entire test file is presented as the example when it contains a single example function, at least one other function, type, variable, or constant declaration, and no test or benchmark functions.

### Main

It is sometimes necessary for a test program to do extra setup or teardown before or after testing. It is also sometimes necessary for a test to control which code runs on the main thread. To support these and other cases, if a test file contains a function:

```
func TestMain(m *testing.M)

```

then the generated test will call TestMain(m) instead of running the tests directly. TestMain runs in the main goroutine and can do whatever setup and teardown is necessary around a call to m.Run. It should then call os.Exit with the result of m.Run.

The minimal implementation of TestMain is:

```
func TestMain(m *testing.M) { os.Exit(m.Run()) }

```

In effect, that is the implementation used when no TestMain is explicitly defined.

## Index ▾


#### Examples


#### Package files

[allocs.go](http://golang.org/src/testing/allocs.go) [benchmark.go](http://golang.org/src/testing/benchmark.go) [cover.go](http://golang.org/src/testing/cover.go) [example.go](http://golang.org/src/testing/example.go) [testing.go](http://golang.org/src/testing/testing.go)

## func [AllocsPerRun](http://golang.org/src/testing/allocs.go?s=671:722#L10)

```
func AllocsPerRun(runs int, f func()) (avg float64)
```

AllocsPerRun returns the average number of allocations during calls to f. Although the return value has type float64, it will always be an integral value.

To compute the number of allocations, the function will first be run once as a warm-up. The average number of allocations over the specified number of runs will then be measured and returned.

AllocsPerRun sets GOMAXPROCS to 1 during its measurement and will restore it before returning.

## func [Coverage](http://golang.org/src/testing/cover.go?s=1319:1342#L35)

```
func Coverage() float64
```

Coverage reports the current code coverage as a fraction in the range [0, 1]. If coverage is not enabled, Coverage returns 0.

When running a large set of sequential test cases, checking Coverage after each one can be useful for identifying which test cases exercise new code paths. It is not a replacement for the reports generated by 'go test -cover' and 'go tool cover'.

## func [Main](http://golang.org/src/testing/testing.go?s=15530:15670#L443)

```
func Main(matchString func(pat, str string) (bool, error), tests []InternalTest, benchmarks []InternalBenchmark, examples []InternalExample)
```

An internal function but exported because it is cross-package; part of the implementation of the "go test" command.

## func [RegisterCover](http://golang.org/src/testing/cover.go?s=1782:1809#L54)

```
func RegisterCover(c Cover)
```

RegisterCover records the coverage data accumulators for the tests. NOTE: This function is internal to the testing infrastructure and may change. It is not covered (yet) by the Go 1 compatibility guidelines.

## func [RunBenchmarks](http://golang.org/src/testing/benchmark.go?s=7475:7574#L275)

```
func RunBenchmarks(matchString func(pat, str string) (bool, error), benchmarks []InternalBenchmark)
```

An internal function but exported because it is cross-package; part of the implementation of the "go test" command.

## func [RunExamples](http://golang.org/src/testing/example.go?s=314:417#L12)

```
func RunExamples(matchString func(pat, str string) (bool, error), examples []InternalExample) (ok bool)
```

## func [RunTests](http://golang.org/src/testing/testing.go?s=17237:17331#L503)

```
func RunTests(matchString func(pat, str string) (bool, error), tests []InternalTest) (ok bool)
```

## func [Short](http://golang.org/src/testing/testing.go?s=8093:8110#L193)

```
func Short() bool
```

Short reports whether the -test.short flag is set.

## func [Verbose](http://golang.org/src/testing/testing.go?s=8183:8202#L198)

```
func Verbose() bool
```

Verbose reports whether the -test.v flag is set.

## type [B](http://golang.org/src/testing/benchmark.go?s=1037:1641#L26)

```
type B struct {
        N int// contains filtered or unexported fields
}
```

B is a type passed to Benchmark functions to manage benchmark timing and to specify the number of iterations to run.

### func (*B) [Error](http://golang.org/src/testing/testing.go?s=12685:12728#L340)

```
func (c *B) Error(args ...interface{})
```

Error is equivalent to Log followed by Fail.

### func (*B) [Errorf](http://golang.org/src/testing/testing.go?s=12824:12883#L346)

```
func (c *B) Errorf(format string, args ...interface{})
```

Errorf is equivalent to Logf followed by Fail.

### func (*B) [Fail](http://golang.org/src/testing/testing.go?s=10377:10400#L277)

```
func (c *B) Fail()
```

Fail marks the function as having failed but continues execution.

### func (*B) [FailNow](http://golang.org/src/testing/testing.go?s=10926:10952#L296)

```
func (c *B) FailNow()
```

FailNow marks the function as having failed and stops its execution. Execution will continue at the next test or benchmark. FailNow must be called from the goroutine running the test or benchmark function, not from other goroutines created during the test. Calling FailNow does not stop those other goroutines.

### func (*B) [Failed](http://golang.org/src/testing/testing.go?s=10508:10538#L284)

```
func (c *B) Failed() bool
```

Failed reports whether the function has failed.

### func (*B) [Fatal](http://golang.org/src/testing/testing.go?s=12987:13030#L352)

```
func (c *B) Fatal(args ...interface{})
```

Fatal is equivalent to Log followed by FailNow.

### func (*B) [Fatalf](http://golang.org/src/testing/testing.go?s=13132:13191#L358)

```
func (c *B) Fatalf(format string, args ...interface{})
```

Fatalf is equivalent to Logf followed by FailNow.

### func (*B) [Log](http://golang.org/src/testing/testing.go?s=12265:12306#L332)

```
func (c *B) Log(args ...interface{})
```

Log formats its arguments using default formatting, analogous to Println, and records the text in the error log. The text will be printed only if the test fails or the -test.v flag is set.

### func (*B) [Logf](http://golang.org/src/testing/testing.go?s=12538:12595#L337)

```
func (c *B) Logf(format string, args ...interface{})
```

Logf formats its arguments according to the format, analogous to Printf, and records the text in the error log. The text will be printed only if the test fails or the -test.v flag is set.

### func (*B) [ReportAllocs](http://golang.org/src/testing/benchmark.go?s=3128:3154#L92)

```
func (b *B) ReportAllocs()
```

ReportAllocs enables malloc statistics for this benchmark. It is equivalent to setting -test.benchmem, but it only affects the benchmark function that calls ReportAllocs.

### func (*B) [ResetTimer](http://golang.org/src/testing/benchmark.go?s=2537:2561#L73)

```
func (b *B) ResetTimer()
```

ResetTimer zeros the elapsed benchmark time and memory allocation counters. It does not affect whether the timer is running.

### func (*B) [RunParallel](http://golang.org/src/testing/benchmark.go?s=10904:10943#L379)

```
func (b *B) RunParallel(body func(*PB))
```

RunParallel runs a benchmark in parallel. It creates multiple goroutines and distributes b.N iterations among them. The number of goroutines defaults to GOMAXPROCS. To increase parallelism for non-CPU-bound benchmarks, call SetParallelism before RunParallel. RunParallel is usually used with the go test -cpu flag.

The body function will be run in each goroutine. It should set up any goroutine-local state and then iterate until pb.Next returns false. It should not use the StartTimer, StopTimer, or ResetTimer functions, because they have global effect.

▹ Example

### func (*B) [SetBytes](http://golang.org/src/testing/benchmark.go?s=2901:2930#L87)

```
func (b *B) SetBytes(n int64)
```

SetBytes records the number of bytes processed in a single operation. If this is called, the benchmark will report ns/op and MB/s.

### func (*B) [SetParallelism](http://golang.org/src/testing/benchmark.go?s=12053:12086#L420)

```
func (b *B) SetParallelism(p int)
```

SetParallelism sets the number of goroutines used by RunParallel to p*GOMAXPROCS. There is usually no need to call SetParallelism for CPU-bound benchmarks. If p is less than 1, this call will have no effect.

### func (*B) [Skip](http://golang.org/src/testing/testing.go?s=13297:13339#L364)

```
func (c *B) Skip(args ...interface{})
```

Skip is equivalent to Log followed by SkipNow.

### func (*B) [SkipNow](http://golang.org/src/testing/testing.go?s=13878:13904#L380)

```
func (c *B) SkipNow()
```

SkipNow marks the test as having been skipped and stops its execution. Execution will continue at the next test or benchmark. See also FailNow. SkipNow must be called from the goroutine running the test, not from other goroutines created during the test. Calling SkipNow does not stop those other goroutines.

### func (*B) [Skipf](http://golang.org/src/testing/testing.go?s=13440:13498#L370)

```
func (c *B) Skipf(format string, args ...interface{})
```

Skipf is equivalent to Logf followed by SkipNow.

### func (*B) [Skipped](http://golang.org/src/testing/testing.go?s=14087:14118#L393)

```
func (c *B) Skipped() bool
```

Skipped reports whether the test was skipped.

### func (*B) [StartTimer](http://golang.org/src/testing/benchmark.go?s=1816:1840#L48)

```
func (b *B) StartTimer()
```

StartTimer starts timing a test. This function is called automatically before a benchmark starts, but it can also used to resume timing after a call to StopTimer.

### func (*B) [StopTimer](http://golang.org/src/testing/benchmark.go?s=2163:2186#L61)

```
func (b *B) StopTimer()
```

StopTimer stops timing a test. This can be used to pause the timer while performing complex initialization that you don't want to measure.

## type [BenchmarkResult](http://golang.org/src/testing/benchmark.go?s=5790:6120#L212)

```
type BenchmarkResult struct {
        N         int           // The number of iterations.
        T         time.Duration // The total time taken.
        Bytes     int64         // Bytes processed in one iteration.
        MemAllocs uint64        // The total number of memory allocations.
        MemBytes  uint64        // The total number of bytes allocated.
}
```

The results of a benchmark run.

### func [Benchmark](http://golang.org/src/testing/benchmark.go?s=12251:12295#L428)

```
func Benchmark(f func(b *B)) BenchmarkResult
```

Benchmark benchmarks a single function. Useful for creating custom benchmarks that do not use the "go test" command.

### func (BenchmarkResult) [AllocedBytesPerOp](http://golang.org/src/testing/benchmark.go?s=6525:6575#L241)

```
func (r BenchmarkResult) AllocedBytesPerOp() int64
```

### func (BenchmarkResult) [AllocsPerOp](http://golang.org/src/testing/benchmark.go?s=6406:6450#L234)

```
func (r BenchmarkResult) AllocsPerOp() int64
```

### func (BenchmarkResult) [MemString](http://golang.org/src/testing/benchmark.go?s=7215:7258#L268)

```
func (r BenchmarkResult) MemString() string
```

### func (BenchmarkResult) [NsPerOp](http://golang.org/src/testing/benchmark.go?s=6122:6162#L220)

```
func (r BenchmarkResult) NsPerOp() int64
```

### func (BenchmarkResult) [String](http://golang.org/src/testing/benchmark.go?s=6649:6689#L248)

```
func (r BenchmarkResult) String() string
```

## type [Cover](http://golang.org/src/testing/cover.go?s=776:923#L21)

```
type Cover struct {
        Mode            string
        Counters        map[string][]uint32
        Blocks          map[string][]CoverBlock
        CoveredPackages string
}
```

Cover records information about test coverage checking. NOTE: This struct is internal to the testing infrastructure and may change. It is not covered (yet) by the Go 1 compatibility guidelines.

## type [CoverBlock](http://golang.org/src/testing/cover.go?s=458:554#L8)

```
type CoverBlock struct {
        Line0 uint32
        Col0  uint16
        Line1 uint32
        Col1  uint16
        Stmts uint16
}
```

CoverBlock records the coverage data for a single basic block. NOTE: This struct is internal to the testing infrastructure and may change. It is not covered (yet) by the Go 1 compatibility guidelines.

## type [InternalBenchmark](http://golang.org/src/testing/benchmark.go?s=849:912#L19)

```
type InternalBenchmark struct {
        Name string
        F    func(b *B)
}
```

An internal type but exported because it is cross-package; part of the implementation of the "go test" command.

## type [InternalExample](http://golang.org/src/testing/example.go?s=236:312#L6)

```
type InternalExample struct {
        Name   string
        F      func()
        Output string
}
```

## type [InternalTest](http://golang.org/src/testing/testing.go?s=14712:14768#L411)

```
type InternalTest struct {
        Name string
        F    func(*T)
}
```

An internal type but exported because it is cross-package; part of the implementation of the "go test" command.

## type [M](http://golang.org/src/testing/testing.go?s=15814:15972#L448)

```
type M struct {
        // contains filtered or unexported fields
}
```

M is a type passed to a TestMain function to run the actual tests.

### func [MainStart](http://golang.org/src/testing/testing.go?s=16185:16333#L458)

```
func MainStart(matchString func(pat, str string) (bool, error), tests []InternalTest, benchmarks []InternalBenchmark, examples []InternalExample) *M
```

MainStart is meant for use by tests generated by 'go test'. It is not meant to be called directly and is not subject to the Go 1 compatibility document. It may change signature from release to release.

### func (*M) [Run](http://golang.org/src/testing/testing.go?s=16523:16544#L468)

```
func (m *M) Run() int
```

Run runs the tests. It returns an exit code to pass to os.Exit.

## type [PB](http://golang.org/src/testing/benchmark.go?s=9707:9989#L346)

```
type PB struct {
        // contains filtered or unexported fields
}
```

A PB is used by RunParallel for running parallel benchmarks.

### func (*PB) [Next](http://golang.org/src/testing/benchmark.go?s=10053:10078#L354)

```
func (pb *PB) Next() bool
```

Next reports whether there are more iterations to execute.

## type [T](http://golang.org/src/testing/testing.go?s=10146:10275#L268)

```
type T struct {
        // contains filtered or unexported fields
}
```

T is a type passed to Test functions to manage test state and support formatted test logs. Logs are accumulated during execution and dumped to standard error when done.

### func (*T) [Error](http://golang.org/src/testing/testing.go?s=12685:12728#L340)

```
func (c *T) Error(args ...interface{})
```

Error is equivalent to Log followed by Fail.

### func (*T) [Errorf](http://golang.org/src/testing/testing.go?s=12824:12883#L346)

```
func (c *T) Errorf(format string, args ...interface{})
```

Errorf is equivalent to Logf followed by Fail.

### func (*T) [Fail](http://golang.org/src/testing/testing.go?s=10377:10400#L277)

```
func (c *T) Fail()
```

Fail marks the function as having failed but continues execution.

### func (*T) [FailNow](http://golang.org/src/testing/testing.go?s=10926:10952#L296)

```
func (c *T) FailNow()
```

FailNow marks the function as having failed and stops its execution. Execution will continue at the next test or benchmark. FailNow must be called from the goroutine running the test or benchmark function, not from other goroutines created during the test. Calling FailNow does not stop those other goroutines.

### func (*T) [Failed](http://golang.org/src/testing/testing.go?s=10508:10538#L284)

```
func (c *T) Failed() bool
```

Failed reports whether the function has failed.

### func (*T) [Fatal](http://golang.org/src/testing/testing.go?s=12987:13030#L352)

```
func (c *T) Fatal(args ...interface{})
```

Fatal is equivalent to Log followed by FailNow.

### func (*T) [Fatalf](http://golang.org/src/testing/testing.go?s=13132:13191#L358)

```
func (c *T) Fatalf(format string, args ...interface{})
```

Fatalf is equivalent to Logf followed by FailNow.

### func (*T) [Log](http://golang.org/src/testing/testing.go?s=12265:12306#L332)

```
func (c *T) Log(args ...interface{})
```

Log formats its arguments using default formatting, analogous to Println, and records the text in the error log. The text will be printed only if the test fails or the -test.v flag is set.

### func (*T) [Logf](http://golang.org/src/testing/testing.go?s=12538:12595#L337)

```
func (c *T) Logf(format string, args ...interface{})
```

Logf formats its arguments according to the format, analogous to Printf, and records the text in the error log. The text will be printed only if the test fails or the -test.v flag is set.

### func (*T) [Parallel](http://golang.org/src/testing/testing.go?s=14284:14306#L401)

```
func (t *T) Parallel()
```

Parallel signals that this test is to be run in parallel with (and only with) other parallel tests.

### func (*T) [Skip](http://golang.org/src/testing/testing.go?s=13297:13339#L364)

```
func (c *T) Skip(args ...interface{})
```

Skip is equivalent to Log followed by SkipNow.

### func (*T) [SkipNow](http://golang.org/src/testing/testing.go?s=13878:13904#L380)

```
func (c *T) SkipNow()
```

SkipNow marks the test as having been skipped and stops its execution. Execution will continue at the next test or benchmark. See also FailNow. SkipNow must be called from the goroutine running the test, not from other goroutines created during the test. Calling SkipNow does not stop those other goroutines.

### func (*T) [Skipf](http://golang.org/src/testing/testing.go?s=13440:13498#L370)

```
func (c *T) Skipf(format string, args ...interface{})
```

Skipf is equivalent to Logf followed by SkipNow.

### func (*T) [Skipped](http://golang.org/src/testing/testing.go?s=14087:14118#L393)

```
func (c *T) Skipped() bool
```

Skipped reports whether the test was skipped.

## type [TB](http://golang.org/src/testing/testing.go?s=9410:9926#L242)

```
type TB interface {
        Error(args ...interface{})
        Errorf(format string, args ...interface{})
        Fail()
        FailNow()
        Failed() bool
        Fatal(args ...interface{})
        Fatalf(format string, args ...interface{})
        Log(args ...interface{})
        Logf(format string, args ...interface{})
        Skip(args ...interface{})
        SkipNow()
        Skipf(format string, args ...interface{})
        Skipped() bool// contains filtered or unexported methods
}
```

TB is the interface common to T and B.

## Subdirectories

| Name                                     | Synopsis                                 |
| ---------------------------------------- | ---------------------------------------- |
| [..](http://golang.org/pkg/)             |                                          |
| [iotest](http://golang.org/pkg/testing/iotest/) | Package iotest implements Readers and Writers useful mainly for testing. |
| [quick](http://golang.org/pkg/testing/quick/) | Package quick implements utility functions to help with black box testing. |

Build version go1.4.2.
Except as [noted](https://developers.google.com/site-policies#restrictions), the content of this page is licensed under the Creative Commons Attribution 3.0 License, and code is licensed under a [BSD license](http://golang.org/LICENSE).
[Terms of Service](http://golang.org/doc/tos.html) | [Privacy Policy](http://www.google.com/intl/en/policies/privacy/)

来源： <<http://golang.org/pkg/testing/>>

