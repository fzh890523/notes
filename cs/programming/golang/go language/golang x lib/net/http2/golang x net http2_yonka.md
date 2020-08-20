

# impl



## conn management



```go
// ClientConnPool manages a pool of HTTP/2 client connections.
type ClientConnPool interface {
	GetClientConn(req *http.Request, addr string) (*ClientConn, error)
	MarkDead(*ClientConn)
}
```





* 可以指定conn pool

* 默认`clientConnPool`







### 默认`clientConnPool`实现



- 从`GetClientConn`往里看

- conn数没有限制，按需创建

  - 但这个按需，对于http2来说，似乎不然

    ```go
    	if cc.singleUse && cc.nextStreamID > 1 {
    		return
    	}
    	st.canTakeNewRequest = cc.goAway == nil && !cc.closed && !cc.closing &&
    		int64(cc.nextStreamID)+int64(cc.pendingRequests) < math.MaxInt32
    	st.freshConn = cc.nextStreamID == 1 && st.canTakeNewRequest
    ```

    可以看到，stream没用完都是ok的 = =

    那么也就是说只有一条连接。。。	



### ClientConn

```go
// ClientConn is the state of a single HTTP/2 client connection to an
// HTTP/2 server.
type ClientConn struct {
	t         *Transport
	tconn     net.Conn             // usually *tls.Conn, except specialized impls
	tlsState  *tls.ConnectionState // nil only for specialized impls
	singleUse bool                 // whether being used for a single http.Request

	// readLoop goroutine fields:
	readerDone chan struct{} // closed on error
	readerErr  error         // set before readerDone is closed

	idleTimeout time.Duration // or 0 for never
	idleTimer   *time.Timer

	mu              sync.Mutex // guards following
	cond            *sync.Cond // hold mu; broadcast on flow/closed changes
	flow            flow       // our conn-level flow control quota (cs.flow is per stream)
	inflow          flow       // peer's conn-level flow control
	closing         bool
	closed          bool
	wantSettingsAck bool                     // we sent a SETTINGS frame and haven't heard back
	goAway          *GoAwayFrame             // if non-nil, the GoAwayFrame we received
	goAwayDebug     string                   // goAway frame's debug data, retained as a string
	streams         map[uint32]*clientStream // client-initiated
	nextStreamID    uint32
	pendingRequests int                       // requests blocked and waiting to be sent because len(streams) == maxConcurrentStreams
	pings           map[[8]byte]chan struct{} // in flight ping data to notification channel
	bw              *bufio.Writer
	br              *bufio.Reader
	fr              *Framer
	lastActive      time.Time
	// Settings from peer: (also guarded by mu)
	maxFrameSize          uint32
	maxConcurrentStreams  uint32
	peerMaxHeaderListSize uint64
	initialWindowSize     uint32

	hbuf    bytes.Buffer // HPACK encoder writes into this
	henc    *hpack.Encoder
	freeBuf [][]byte

	wmu  sync.Mutex // held while writing; acquire AFTER mu if holding both
	werr error      // first write error that has occurred
}
```









## stream management



### maxConcurrentStreams



* 在client侧会被设置，但没有给设置方式，而是用的默认的1000
* 会被peer的协议消息设置





## request handling



* (`net/http`)`Client.Do`

  * `send(req *Request, deadline time.Time)`

    `c.transport()` -> RoundTripper

    * `send(ireq *Request, rt RoundTripper, deadline time.Time)`

      `rt.RoundTrip(req)`

      * (`RoundTripper`)`RoundTrip(*Request)`

        `(t *Transport) RoundTrip(req *http.Request)`
        * `(cc *ClientConn) roundTrip(req *http.Request) `





## arch



### net/http 和 x/net/http2结合的地方

`Transport`



```go
func (c *Client) transport() RoundTripper {
	if c.Transport != nil {
		return c.Transport
	}
	return DefaultTransport
}
```



```go
// RoundTripper is an interface representing the ability to execute a
// single HTTP transaction, obtaining the Response for a given Request.
//
// A RoundTripper must be safe for concurrent use by multiple
// goroutines.
type RoundTripper interface {
	// RoundTrip executes a single HTTP transaction, returning
	// a Response for the provided Request.
	//
	// RoundTrip should not attempt to interpret the response. In
	// particular, RoundTrip must return err == nil if it obtained
	// a response, regardless of the response's HTTP status code.
	// A non-nil err should be reserved for failure to obtain a
	// response. Similarly, RoundTrip should not attempt to
	// handle higher-level protocol details such as redirects,
	// authentication, or cookies.
	//
	// RoundTrip should not modify the request, except for
	// consuming and closing the Request's Body. RoundTrip may
	// read fields of the request in a separate goroutine. Callers
	// should not mutate the request until the Response's Body has
	// been closed.
	//
	// RoundTrip must always close the body, including on errors,
	// but depending on the implementation may do so in a separate
	// goroutine even after RoundTrip returns. This means that
	// callers wanting to reuse the body for subsequent requests
	// must arrange to wait for the Close call before doing so.
	//
	// The Request's URL and Header fields must be initialized.
	RoundTrip(*Request) (*Response, error)
}
```



```go
func (t *Transport) RoundTrip(req *http.Request) (*http.Response, error) {
	return t.RoundTripOpt(req, RoundTripOpt{})
}


// RoundTripOpt is like RoundTrip, but takes options.
func (t *Transport) RoundTripOpt(req *http.Request, opt RoundTripOpt) (*http.Response, error) {
	if !(req.URL.Scheme == "https" || (req.URL.Scheme == "http" && t.AllowHTTP)) {
		return nil, errors.New("http2: unsupported scheme")
	}

	addr := authorityAddr(req.URL.Scheme, req.URL.Host)
	for retry := 0; ; retry++ {
		cc, err := t.connPool().GetClientConn(req, addr)
		if err != nil {
			t.vlogf("http2: Transport failed to get client conn for %s: %v", addr, err)
			return nil, err
		}
		traceGotConn(req, cc)
		res, gotErrAfterReqBodyWrite, err := cc.roundTrip(req)
...
        

func (cc *ClientConn) RoundTrip(req *http.Request) (*http.Response, error) {
	resp, _, err := cc.roundTrip(req)
	return resp, err
}
```





