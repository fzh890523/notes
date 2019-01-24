

https://stackoverflow.com/questions/25965584/separating-unit-tests-and-integration-tests-in-go



主要是单元测试和集成测试。





# ref1

@Ainar-G suggests several great patterns to separate tests.

[This set of Go practices from SoundCloud](http://peter.bourgon.org/go-in-production/#testing-and-validation) recommends using build tags ([described in the "Build Constraints" section of the build package](http://golang.org/pkg/go/build/)) to select which tests to run:

> Write an integration_test.go, and give it a build tag of integration. Define (global) flags for things like service addresses and connect strings, and use them in your tests.
>
> ```golang
> // +build integration
> 
> var fooAddr = flag.String(...)
> 
> func TestToo(t *testing.T) {
>     f, err := foo.Connect(*fooAddr)
>     // ...
> }
> ```
>
> go test takes build tags just like go build, so you can call `go test -tags=integration`. It also synthesizes a package main which calls flag.Parse, so any flags declared and visible will be processed and available to your tests.

As a similar option, you could also have integration tests run by default by using a build condition `// +build !unit`, and then disable them on demand by running `go test -tags=unit`.

@adamc comments:

For anyone else attempting to use build tags, it's important that the `// +build test` comment is the first line in your file, and that you include a blank line after the comment, otherwise the `-tags` command will ignore the directive.

Also, the tag used in the build comment cannot have a dash, although underscores are allowed. For example, `// +build unit-tests` will not work, whereas `// +build unit_tests`will.











# ref2

I see three possible solutions. The first is to use the [short mode](http://golang.org/pkg/testing/#Short) for unit tests. So you would use `go test -short` with unit tests and the same but without the `-short` flag to run your integration tests as well. The standard library uses the short mode to either skip long-running tests, or make them run faster by providing simpler data.

The second is to use a convention and call your tests either `TestUnitFoo` or `TestIntegrationFoo` and then use the [`-run` testing flag](http://golang.org/cmd/go/#hdr-Description_of_testing_flags) to denote which tests to run. So you would use `go test -run 'Unit'` for unit tests and `go test -run 'Integration'` for integration tests.

The third option is to use an environment variable, and get it in your tests setup with [`os.Getenv`](http://golang.org/pkg/os/#Getenv). Then you would use simple `go test` for unit tests and `FOO_TEST_INTEGRATION=true go test` for integration tests.

I personally would prefer the `-short` solution since it's simpler and is used in the standard library, so it seems like it's a de facto way of separating/simplifying long-running tests. But the `-run` and `os.Getenv` solutions offer more flexibility (more caution is required as well, since regexps are involved with `-run`).











# ref3

To elaborate on my comment to @Ainar-G's excellent answer, over the past year I have been using the combination of `-short` with `Integration` naming convention to achieve the best of both worlds.

## Unit and Integration tests harmony, in the same file

Build flags previously forced me to have multiple files (`services_test.go`, `services_integration_test.go`, etc).

Instead, take this example below where the first two are unit tests and I have an integration test at the end:

```golang
package services

import "testing"

func TestServiceFunc(t *testing.T) {
    t.Parallel()
    ...
}

func TestInvalidServiceFunc3(t *testing.T) {
    t.Parallel()
    ...
}

func TestPostgresVersionIntegration(t *testing.T) {
    if testing.Short() {
        t.Skip("skipping integration test")
    }
    ...
}
```

Notice the last test has the convention of:

1. using `Integration` in the test name.
2. checking if running under `-short` flag directive.

Basically, the spec goes: "write all tests normally. if it is a long-running tests, or an integration test, follow this naming convention and check for `-short` to be nice to your peers."

## Run only Unit tests:

```golang
go test -v -short
```

this provides you with a nice set of messages like:

```golang
=== RUN   TestPostgresVersionIntegration
--- SKIP: TestPostgresVersionIntegration (0.00s)
        service_test.go:138: skipping integration test
```

## Run Integration Tests only:

```golang
go test -run Integration
```

This runs only the integration tests. Useful for smoke testing canaries in production.

Obviously the downside to this approach is if anyone runs `go test`, without the `-short` flag, it will default to run all tests - unit and integration tests.

In reality, if your project is large enough to have unit and integration tests, then you most likely are using a `Makefile` where you can have simple directives to use `go test -short` in it. Or, just put it in your `README.md` file and call it the day.



# 小结



* 通过testcase命名来区分，然后go test加上匹配来筛选
* 通过编译选项来筛选

* 通过环境变量来主动选择













