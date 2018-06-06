

# Golang 1.4 net/rpc client源码解析

Posted on [2015年3月19日](http://dev.cmcm.com/archives/320)  by  [毛, 剑](http://dev.cmcm.com/)		 | [4条评论](http://dev.cmcm.com/archives/320#comments)		

[net/rpc](http://godoc.org/net/rpc)是golang标准库提供的rpc框架，下面我们重点看下net/rpc是如何实现的。 我本机源码安装路径在/usr/local/go，这net/rpc（golang 1.4版本）涉及到的相关代码主要有：

> client.go
>
> server.go

首先我们先从client.go，客户端入手看：

```
type ClientCodec interface {// WriteRequest must be safe for concurrent use by multiple goroutines.     WriteRequest(*Request, interface{}) error                                   
    ReadResponseHeader(*Response) error                                         
    ReadResponseBody(interface{}) error                                         

    Close() error                                                               
}   

type Call struct {ServiceMethod string      // The name of the service and method to call.    Args          interface{} // The argument to the function (*struct).           Reply         interface{} // The reply from the function (*struct).            Error         error       // After completion, the error status.               Done          chan *Call  // Strobes when call is complete.                    Tracer        *Trace      // tracer                                            }  

type Client struct {                                                               
    codec ClientCodec                                                              

    reqMutex sync.Mutex // protects following                                      
    request  Request                                                               

    mutex    sync.Mutex // protects following                                      
    seq      uint64                                                                
    pending  map[uint64]*Call                                                      
    closing  bool // user has called Close                                         
    shutdown bool // server has told us to stop                                    } 

func (client *Client) send(call *Call) {// client要想复用，保证线程安全，加上请求锁reqMutex是必须的。                                
    client.reqMutex.Lock()                                                      
    defer client.reqMutex.Unlock()// 这其实是针对map的另外一把锁，这样可以更细粒度的操作                                                    
    client.mutex.Lock()// client如果外部调用关闭，那么call也是结束状态，之后我们再分析call.done()                                                         if client.shutdown || client.closing {                                      
        call.Error = ErrShutdown                                                
        client.mutex.Unlock()                                                   
        call.done()return}// 重点来了！seq序号自增在把call请求暂存在pennding的map中，锁释放                                                                  
    seq := client.seq                                                           
    client.seq++                                                                
    client.pending[seq] = call                                                  
    client.mutex.Unlock()// 这一块代码属于编码请求了，因为rpc涉及到调用具体是谁，所以需要把method传给rpc server// 这里的Seq是用于当server response的时候，seq从client->server，再从server->client，然后反查map，定位call对象使用的。                                         
    client.request.Seq = seq                                                    
    client.request.ServiceMethod = call.ServiceMethod// inject tracer，这个请忽视。。。                                                         
    client.request.Tracer = call.Tracer                                         
    err := client.codec.WriteRequest(&client.request, call.Args)if err != nil {                                                             
        client.mutex.Lock()                                                     
        call = client.pending[seq]delete(client.pending, seq)                                             
        client.mutex.Unlock()if call != nil {                                                        
            call.Error = err                                                    
            call.done()}}} 
```

我们使用rpc的时候，都知道client是线程安全的，client其实是基于单个socket连接来，依赖channel来实现复用连接以及并行的。而临时的调用对象Call都是保存在Client的map中的，至于每个call怎么查找，也是根据seq序列号在请求server时候转发过去，之后response的时候，client根据返回的seq再反查结果的。不难看出，实现了ClientCodec之后就可以自定义rpc协议请求头和内容了。那么send函数中的Call对象是从哪里来的？

```
// 我们rpc请求的时候，调用就是这个方法，传入方法名，参数，获取返回等
func (client *Client) Call(serviceMethod string, args interface{}, reply interface{}) error {// Call里面调用了client.Go，然后返回一个chan，之后阻塞等待，这是基本的同步调用
    call := <-client.Go(serviceMethod, args, reply, make(chan *Call, 1)).Donereturn call.Error} 

func (client *Client) Go(serviceMethod string, args interface{}, reply interface{}, done chan *Call) *Call {// 构建call对象
    call := new(Call)                                                           
    call.ServiceMethod = serviceMethod                                          
    call.Args = args                                                            
    call.Reply = reply   
    // 如果非外部传入call，自己构建                                                       if done == nil {done = make(chan *Call, 10) // buffered.                                } else {// If caller passes done != nil, it must arrange that                   // done has enough buffer for the number of simultaneous                // RPCs that will be using that channel.  If the channel                // is totally unbuffered, it's best not to run at all.                  if cap(done) == 0 {                                                     
            log.Panic("rpc: done channel is unbuffered")}}                                                                           
    call.Done = done// 发送请求                                                            
    client.send(call)return call                                                                 
} 
```

在初始化client的时候，我们会指定ip，port等

```
// Dial connects to an RPC server at the specified network address.             
func Dial(network, address string) (*Client, error) {                           
    conn, err := net.Dial(network, address)if err != nil {return nil, err                                                         
    }return NewClient(conn), nil}// 我们看到其实NewClient内部使用的默认的gob编码，gobClientCodes实现了Codec的接口                              
func NewClient(conn io.ReadWriteCloser) *Client {                               
    encBuf := bufio.NewWriter(conn)                                             
    client := &gobClientCodec{conn, gob.NewDecoder(conn), gob.NewEncoder(encBuf), encBuf}return NewClientWithCodec(client)}// 当然也提供自定义的codec，你可以使用thrift协议、messagepack等来扩展                 // codec to encode requests and decode responses.                               
func NewClientWithCodec(codec ClientCodec) *Client {                            
    client := &Client{                                                          
        codec:   codec,                                                         
        pending: make(map[uint64]*Call),}                                                                           
    go client.input()return client                                                               
}

type gobClientCodec struct {                                                    
    rwc    io.ReadWriteCloser                                                   
    dec    *gob.Decoder                                                         
    enc    *gob.Encoder                                                         
    encBuf *bufio.Writer} 
```

最后，NewClient会后台开启一枚goroutine，就是接受server返回然后转发具体调用者了。

```
func (client *Client) input() {var err error                                                             
    var response Responsefor err == nil {// 二话不说先获取Response的头                                                    
        response = Response{}                                                   
        err = client.codec.ReadResponseHeader(&response)if err != nil {break}// 头部中包含了序列号，用于定位pending map使用的                                                               
        seq := response.Seq// 小粒度锁删除map，获取call对象                                                
        client.mutex.Lock()                                                     
        call := client.pending[seq]delete(client.pending, seq)                                             
        client.mutex.Unlock()switch {// 如果pending找不到，那么肯定是异常了                                                              case call == nil:// We've got no pending call. That usually means that               // WriteRequest partially failed, and call was already              // removed; response is a server telling us about an                // error reading request body. We should still attempt              // to read error body, but there's no one to give it to.            
            err = client.codec.ReadResponseBody(nil)if err != nil {                                                     
                err = errors.New("reading error body: " + err.Error())}// rpc 报错了，解不开什么的都有可能                                                          case response.Error != "":// We've got an error response. Give this to the request;           // any subsequent requests will get the ReadResponseBody            // error if there is one.                                           
            call.Error = ServerError(response.Error)                            
            err = client.codec.ReadResponseBody(nil)if err != nil {                                                     
                err = errors.New("reading error body: " + err.Error())}                                                                   
            call.done()default:// 默认还是正常的处理，获取Body给Reply，让调用者可见                                                               
            err = client.codec.ReadResponseBody(call.Reply)if err != nil {                                                     
                call.Error = errors.New("reading body " + err.Error())}                                                                   
            call.done()}}// 如果有啥不可逆的异常，那么只能shutdown client了。全部退出吧                                                                         // Terminate pending calls.                                                 
    client.reqMutex.Lock()                                                      
    client.mutex.Lock()                                                         
    client.shutdown = true                                                      
    closing := client.closing                                                   
    if err == io.EOF {if closing {                                                            
            err = ErrShutdown} else {                                                                
            err = io.ErrUnexpectedEOF}}// 之前pending的也一个个结束吧，避免调用者都等待                                                                           for _, call := range client.pending {                                       
        call.Error = err                                                        
        call.done()}                                                                           
    client.mutex.Unlock()                                                       
    client.reqMutex.Unlock()if debugLog && err != io.EOF && !closing {                                  
        log.Println("rpc: client protocol error:", err)}}
```

最后call.done做了什么了，相比你也猜到：

```
// 把call对象传递给调用者，主要是获取内部的Error
func (call *Call) done() {select {case call.Done <- call:// ok                                                                   default:// We don't want to block here.  It is the caller's responsibility to make// sure the channel has enough buffer space. See comment in Go().       if debugLog {                                                           
            log.Println("rpc: discarding Call reply due to insufficient Done chan capacity")}}}
```

大致的分析就结束了，但是完整的rpc框架，还应该包括，服务发现，服务降级，服务追踪，服务容错等， 服务发现：可以使用zk，以及配合client定制的方式实现

服务降级：可以在zk中指定服务质量，以及根据回馈系统来drop request

服务追踪：最近我在看Twitter的Zipkin和Google的Dapper，对核心rpc库修改的方式避免大量植入代码，但是golang要做到这点有点困难，一是AOP不好支持，所以现在只能考虑用侵入代码，有更好思路的可以联系我！

服务容错：因为input本身单连接请求获取server，有可能<-call一直不返回，导致业务大量hang，这个可以考虑加上一些channel的timeout特性来实现，只不过浪费了一些内存。

总体来说net/rpc还是一个不错的框架，但是几个地方需要考虑，一个是全局大锁reqMutex，另外是call对象会大量创建（可否考虑call pool等）

Posted in [Golang](http://dev.cmcm.com/archives/category/golang).| Tagged [Golang](http://dev.cmcm.com/archives/tag/golang), [rpc](http://dev.cmcm.com/archives/tag/rpc).		| 42 views

来源： <<http://dev.cmcm.com/archives/320>>

 

