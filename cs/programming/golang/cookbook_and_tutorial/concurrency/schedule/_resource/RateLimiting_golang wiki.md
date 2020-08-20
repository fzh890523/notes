

# RateLimiting

Pero edited this page on 24 Apr · [6 revisions](https://github.com/golang/go/wiki/RateLimiting/_history)

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

# Rate Limiting

To limit the rate of operations per unit time, use a [time.Ticker](http://golang.org/pkg/time/#NewTicker). This works well for rates up to 10s per second. For higher rates, prefer a token bucket rate limiter (search godoc.org for [rate limit](http://godoc.org/?q=rate+limit)).

```
import "time"rate := time.Second / 10throttle := time.Tick(rate)
for req := range requests {
  <-throttle  // rate limit our Service.Method RPCsgo client.Call("Service.Method", req, ...)
}
```

To allow some bursts, add a buffer to the throttle:

```
import "time"rate := time.Second / 10burstLimit := 100tick := time.NewTicker(rate)
defer tick.Stop()
throttle := make(chan time.Time, burstLimit)
go func() {
  for t := range tick.C {
    select {
      case throttle <- t:
      default:
    }
  }  // exits after tick.Stop()
}()
for req := range requests {
  <-throttle  // rate limit our Service.Method RPCsgo client.Call("Service.Method", req, ...)
}
```

来源： <<https://github.com/golang/go/wiki/RateLimiting>>

 

