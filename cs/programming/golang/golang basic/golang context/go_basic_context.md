



# basic



```go
// A Context carries a deadline, a cancelation signal, and other values across
// API boundaries.
//
// Context's methods may be called by multiple goroutines simultaneously.
type Context interface {
	// Deadline returns the time when work done on behalf of this context
	// should be canceled. Deadline returns ok==false when no deadline is
	// set. Successive calls to Deadline return the same results.
	Deadline() (deadline time.Time, ok bool)

	// Done returns a channel that's closed when work done on behalf of this
	// context should be canceled. Done may return nil if this context can
	// never be canceled. Successive calls to Done return the same value.
	//
	// WithCancel arranges for Done to be closed when cancel is called;
	// WithDeadline arranges for Done to be closed when the deadline
	// expires; WithTimeout arranges for Done to be closed when the timeout
	// elapses.
	//
	// Done is provided for use in select statements:
	//
	//  // Stream generates values with DoSomething and sends them to out
	//  // until DoSomething returns an error or ctx.Done is closed.
	//  func Stream(ctx context.Context, out chan<- Value) error {
	//  	for {
	//  		v, err := DoSomething(ctx)
	//  		if err != nil {
	//  			return err
	//  		}
	//  		select {
	//  		case <-ctx.Done():
	//  			return ctx.Err()
	//  		case out <- v:
	//  		}
	//  	}
	//  }
	//
	// See https://blog.golang.org/pipelines for more examples of how to use
	// a Done channel for cancelation.
	Done() <-chan struct{}

	// If Done is not yet closed, Err returns nil.
	// If Done is closed, Err returns a non-nil error explaining why:
	// Canceled if the context was canceled
	// or DeadlineExceeded if the context's deadline passed.
	// After Err returns a non-nil error, successive calls to Err return the same error.
	Err() error

	// Value returns the value associated with this context for key, or nil
	// if no value is associated with key. Successive calls to Value with
	// the same key returns the same result.
	//
	// Use context values only for request-scoped data that transits
	// processes and API boundaries, not for passing optional parameters to
	// functions.
	//
	// A key identifies a specific value in a Context. Functions that wish
	// to store values in Context typically allocate a key in a global
	// variable then use that key as the argument to context.WithValue and
	// Context.Value. A key can be any type that supports equality;
	// packages should define keys as an unexported type to avoid
	// collisions.
	//
	// Packages that define a Context key should provide type-safe accessors
	// for the values stored using that key:
	//
	// 	// Package user defines a User type that's stored in Contexts.
	// 	package user
	//
	// 	import "context"
	//
	// 	// User is the type of value stored in the Contexts.
	// 	type User struct {...}
	//
	// 	// key is an unexported type for keys defined in this package.
	// 	// This prevents collisions with keys defined in other packages.
	// 	type key int
	//
	// 	// userKey is the key for user.User values in Contexts. It is
	// 	// unexported; clients use user.NewContext and user.FromContext
	// 	// instead of using this key directly.
	// 	var userKey key
	//
	// 	// NewContext returns a new Context that carries value u.
	// 	func NewContext(ctx context.Context, u *User) context.Context {
	// 		return context.WithValue(ctx, userKey, u)
	// 	}
	//
	// 	// FromContext returns the User value stored in ctx, if any.
	// 	func FromContext(ctx context.Context) (*User, bool) {
	// 		u, ok := ctx.Value(userKey).(*User)
	// 		return u, ok
	// 	}
	Value(key interface{}) interface{}
}
```



```go
// WithCancel returns a copy of parent with a new Done channel. The returned
// context's Done channel is closed when the returned cancel function is called
// or when the parent context's Done channel is closed, whichever happens first.
//
// Canceling this context releases resources associated with it, so code should
// call cancel as soon as the operations running in this Context complete.
func WithCancel(parent Context) (ctx Context, cancel CancelFunc) 

// WithDeadline returns a copy of the parent context with the deadline adjusted
// to be no later than d. If the parent's deadline is already earlier than d,
// WithDeadline(parent, d) is semantically equivalent to parent. The returned
// context's Done channel is closed when the deadline expires, when the returned
// cancel function is called, or when the parent context's Done channel is
// closed, whichever happens first.
//
// Canceling this context releases resources associated with it, so code should
// call cancel as soon as the operations running in this Context complete.
func WithDeadline(parent Context, d time.Time) (Context, CancelFunc)

// WithTimeout returns WithDeadline(parent, time.Now().Add(timeout)).
//
// Canceling this context releases resources associated with it, so code should
// call cancel as soon as the operations running in this Context complete:
//
// 	func slowOperationWithTimeout(ctx context.Context) (Result, error) {
// 		ctx, cancel := context.WithTimeout(ctx, 100*time.Millisecond)
// 		defer cancel()  // releases resources if slowOperation completes before timeout elapses
// 		return slowOperation(ctx)
// 	}
func WithTimeout(parent Context, timeout time.Duration) (Context, CancelFunc) 

// WithValue returns a copy of parent in which the value associated with key is
// val.
//
// Use context Values only for request-scoped data that transits processes and
// APIs, not for passing optional parameters to functions.
//
// The provided key must be comparable and should not be of type
// string or any other built-in type to avoid collisions between
// packages using context. Users of WithValue should define their own
// types for keys. To avoid allocating when assigning to an
// interface{}, context keys often have concrete type
// struct{}. Alternatively, exported context key variables' static
// type should be a pointer or interface.
func WithValue(parent Context, key, val interface{}) Context
```





## 构造

go把主要的几种context的实现都屏蔽了，构造也统一由它（`context.WithXXX`）来完成。



## 几种context

* emptyCtx

  ```go
  // An emptyCtx is never canceled, has no values, and has no deadline. It is not
  // struct{}, since vars of this type must have distinct addresses.
  type emptyCtx int
  ```

  * TODO()
  * Background()

* cancelCtx

  ```go
  // A cancelCtx can be canceled. When canceled, it also cancels any children
  // that implement canceler.
  type cancelCtx struct {
  	Context

  	mu       sync.Mutex            // protects following fields
  	done     chan struct{}         // created lazily, closed by first cancel call
  	children map[canceler]struct{} // set to nil by the first cancel call
  	err      error                 // set to non-nil by the first cancel call
  }
  ```

  * WithCancel()

* timerCtx

  ```Go
  // A timerCtx carries a timer and a deadline. It embeds a cancelCtx to
  // implement Done and Err. It implements cancel by stopping its timer then
  // delegating to cancelCtx.cancel.
  type timerCtx struct {
  	cancelCtx
  	timer *time.Timer // Under cancelCtx.mu.

  	deadline time.Time
  }
  ```

  * WithTimeout()

    ```go
    return WithDeadline(parent, time.Now().Add(timeout))
    ```

  * WithDeadline()

* valueCtx

  ```go
  // A valueCtx carries a key-value pair. It implements Value for that key and
  // delegates all other calls to the embedded Context.
  type valueCtx struct {
  	Context
  	key, val interface{}
  }
  ```

  * WithValue



## Done



* 可能返回nil： 对于不能cancel的context

  > yonka: 囧，那干脆叫 Cancelled 得了


* 这个只有 **读** 没有 **写**： **写**由`CancelFunc`来完成



## cancel

cancel/cancelFunc



* cancellable context才有

* 典型的，在构造时获取。 go没有在Context接口里给出获取cancelFunc

* **！！！** go给context设计的cancel，是用于 **前向cancel** 的，也即cancel当前操作衍生的操作； 而无法（不方便）做后向cancel，也即用于类似`Future`的场景比较麻烦

  类似的还有篇博文： [Context isn’t for cancellation](https://dave.cheney.net/2017/08/20/context-isnt-for-cancellation)

  如：

  ```go
  func handleReq(ctx context.Context) {
    newCtx, cancelFunc := context.WithCancel(ctx)
    go func(ctx context.Context){
      select {
        case <- ctx.Done():
        // cancelled by ...
        case <- doSth().C:
        // can not notify backwards
      }
    }(newCtx)
    doSth()
    // done
    cancelFunc()  // cancel forwards: cancel ongoing actions rather than wait for them
  }
  ```

  ​

## Err



* emptyCtx 没有（`return nil`）

* cancelCtx 有，在第一次cancel时置为non-nil

  **特别蠢的设计** ctx没有stop/done等方法，导致`timerCtx`必然会被cancel，也即 `ctx.Err()` 迟早会non-nil。

* timerCtx 的来自于“继承”的cancelCtx







# case



## case: async









## Case: future



```go
func a(ctx context.Context, arg1 string) {
  doSth(args)

  // 
}
```







