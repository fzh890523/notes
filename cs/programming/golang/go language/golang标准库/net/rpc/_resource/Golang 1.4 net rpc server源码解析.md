

# Golang 1.4 net/rpc server源码解析

Posted on [2015年3月23日](http://dev.cmcm.com/archives/324)  by  [毛, 剑](http://dev.cmcm.com/)		 | [2条评论](http://dev.cmcm.com/archives/324#comments)		

[上一篇](http://godoc.org/net/rpc)文章我们讲了net/rpc中client部分的实现，我本机源码安装路径在/usr/local/go，这net/rpc（golang 1.4版本）涉及到的相关代码主要有：

> server.go

**方法注册：**

因为从client我们知道是复用的socket来实现并发调用rpc方法，我们先从方法注册来看源码部分：

```
// Server对象大都是保存方法存根，保证对象互斥的
type Server struct {                                                            
    mu         sync.RWMutex // protects the serviceMap                          
    serviceMap map[string]*service                                              
    reqLock    sync.Mutex // protects freeReq                                   
    freeReq    *Request                                                         
    respLock   sync.Mutex // protects freeResp                                  
    freeResp   *Response}

func NewServer() *Server {return &Server{serviceMap: make(map[string]*service)}}// rpc.Register默认使用了一个Server，只对serviceMap进行了初始化                          var DefaultServer = NewServer()// rpc的service包括方法名、方法反射，类型等
type service struct {                                                           
    name   string                 // name of service                            
    rcvr   reflect.Value          // receiver of methods for the service        
    typ    reflect.Type           // type of the receiver                       
    method map[string]*methodType // registered methods                         }// 无论是RegisterName、Register最终都调用了register的内部方法
func (server *Server) register(rcvr interface{}, name string, useName bool) error {// 保证注册服务安全，先加锁
    server.mu.Lock()                                                               
    defer server.mu.Unlock()// 如果服务为空，默认注册一个                                                      if server.serviceMap == nil {                                                  
        server.serviceMap = make(map[string]*service)}// 获取注册服务的反射信息                                                                              
    s := new(service)                                                              
    s.typ = reflect.TypeOf(rcvr)                                                   
    s.rcvr = reflect.ValueOf(rcvr)// 可以使用自定义名称                                               
    sname := reflect.Indirect(s.rcvr).Type().Name()if useName {                                                                   
        sname = name                                                               
    }if sname == "" {                                                               
        s := "rpc.Register: no service name for type " + s.typ.String()            
        log.Print(s)return errors.New(s)}// 方法必须是暴露的，既服务名首字符大写                                                                             if !isExported(sname) && !useName {                                            
        s := "rpc.Register: type " + sname + " is not exported"                    
        log.Print(s)return errors.New(s)}// 不允许重复注册                                                                         if _, present := server.serviceMap[sname]; present {return errors.New("rpc: service already defined: " + sname)}                                                                           
    s.name = sname                                                              

    // 开始注册rpc struct内部的方法存根                                                      
    s.method = suitableMethods(s.typ, true)// 如果struct内部一个方法也没，那么直接报错，错误信息还非常详细                                                                            if len(s.method) == 0 {                                                     
        str := ""// To help the user, see if a pointer receiver would work.              
        method := suitableMethods(reflect.PtrTo(s.typ), false)if len(method) != 0 {                                                   
            str = "rpc.Register: type " + sname + " has no exported methods of suitable type (hint: pass a pointer to value of that type)"} else {                                                                
            str = "rpc.Register: type " + sname + " has no exported methods of suitable type"}                                                                       
        log.Print(str)return errors.New(str)}// 保存在server的serviceMap中                                                                           
    server.serviceMap[s.name] = s                                               
    return nil}// 上文提到了服务还需要方法存根的注册
func suitableMethods(typ reflect.Type, reportErr bool) map[string]*methodType {// 根据方法名创建保存内部方法map
    methods := make(map[string]*methodType)// 获取rpc struct内部的方法                                     for m := 0; m < typ.NumMethod(); m++ {                                      
        method := typ.Method(m)                                                 
        mtype := method.Type                                                    
        mname := method.Name// 之前对这行代码觉得比较奇葩，方法是否是暴露，是看是否有PkgPath的，如果是私有方法，PkgPath显示包名                       if method.PkgPath != "" {continue}// 判断是否是三个参数：第一个是结构本身，第二个是参数，第三个是返回值                                                    // Method needs three ins: receiver, *args, *reply.                     if mtype.NumIn() != 3 {if reportErr {                                                      
                log.Println("method", mname, "has wrong number of ins:", mtype.NumIn())}continue}// args是指针类型                                                                     // First arg need not be a pointer.                                     
        argType := mtype.In(1)if !isExportedOrBuiltinType(argType) {if reportErr {                                                      
                log.Println(mname, "argument type not exported:", argType)}continue}// reply是指针类型                                                                   // Second arg must be a pointer.                                        
        replyType := mtype.In(2)if replyType.Kind() != reflect.Ptr {if reportErr {                                                      
                log.Println("method", mname, "reply type not a pointer:", replyType)}continue}// Reply type must be exported.       // reply必须是可暴露的                                  if !isExportedOrBuiltinType(replyType) {if reportErr {                                                      
                log.Println("method", mname, "reply type not exported:", replyType)}continue}// Method needs one out.  // 必须有一个返回值，而且要是error                                              if mtype.NumOut() != 1 {if reportErr {                                                      
                log.Println("method", mname, "has wrong number of outs:", mtype.NumOut())}continue}// The return type of the method must be error.                         if returnType := mtype.Out(0); returnType != typeOfError {if reportErr {                                                      
                log.Println("method", mname, "returns", returnType.String(), "not error")}continue}                                                                       
        methods[mname] = &methodType{method: method, ArgType: argType, ReplyType: replyType}}return methods                                                              
}
```

**请求调用：**

方法已经被注册成功，接下来我们看看是如何客户端发送请求调用的：

```
func (server *Server) Accept(lis net.Listener) {for {                                                                       
        conn, err := lis.Accept()if err != nil {                                                         
            log.Fatal("rpc.Serve: accept:", err.Error()) // TODO(r): exit?      }// accept连接以后，打开一个goroutine处理请求                                                                       
        go server.ServeConn(conn)}} 

func (server *Server) ServeConn(conn io.ReadWriteCloser) {                      
    buf := bufio.NewWriter(conn)                                                
    srv := &gobServerCodec{                                                     
        rwc:    conn,                                                           
        dec:    gob.NewDecoder(conn),                                           
        enc:    gob.NewEncoder(buf),                                            
        encBuf: buf,}// 根据指定的codec进行协议解析                                                                          
    server.ServeCodec(srv)} 

func (server *Server) ServeCodec(codec ServerCodec) {                           
    sending := new(sync.Mutex)for {// 解析请求                                                                     
        service, mtype, req, argv, replyv, keepReading, err := server.readRequest(codec)if err != nil {if debugLog && err != io.EOF {                                      
                log.Println("rpc:", err)}if !keepReading {break}// send a response if we actually managed to read a header. // 如果当前请求错误了，我们应该返回信息，然后继续处理        if req != nil {                                                     
                server.sendResponse(sending, req, invalidRequest, codec, err.Error())
                server.freeRequest(req)}continue}// 因为需要继续处理后续请求，所以开一个gorutine处理rpc方法                                                                      
        go service.call(server, sending, mtype, req, argv, replyv, codec)}// 如果连接关闭了需要释放资源                                                                           
    codec.Close()} 

func (server *Server) readRequestHeader(codec ServerCodec) (service *service, mtype *methodType, req *Request, keepReading bool, err error) {// 解析头部，如果失败，直接返回了                                                 
    req = server.getRequest()                                                   
    err = codec.ReadRequestHeader(req)if err != nil {                                                             
        req = nilif err == io.EOF || err == io.ErrUnexpectedEOF {return}                                                                       
        err = errors.New("rpc: server cannot decode request: " + err.Error())return}if debugLog {                                                               
        log.Printf("rpc: [trace:%v]\n", req.Tracer)}// We read the header successfully.  If we see an error now,                // we can still recover and move on to the next request.                    
    keepReading = true// 获取请求中xxx.xxx中.的位置                                                                        
    dot := strings.LastIndex(req.ServiceMethod, ".")if dot < 0 {                                                                
        err = errors.New("rpc: service/method request ill-formed: " + req.ServiceMethod)return}// 拿到struct名字和方法名字                                                                 
    serviceName := req.ServiceMethod[:dot]                                      
    methodName := req.ServiceMethod[dot+1:]// Look up the request.// 加读锁，获取对象                                                     
    server.mu.RLock()                                                           
    service = server.serviceMap[serviceName]                                    
    server.mu.RUnlock()if service == nil {                                                         
        err = errors.New("rpc: can't find service " + req.ServiceMethod)return}// 获取反射类型，看见rpc中的发射其实是预先放入map中的                                                                         
    mtype = service.method[methodName]if mtype == nil {                                                           
        err = errors.New("rpc: can't find method " + req.ServiceMethod)}return}

func (server *Server) readRequest(codec ServerCodec) (service *service, mtype *methodType, req *Request, argv, replyv reflect.Value, keepReading bool, err error) {
    service, mtype, req, keepReading, err = server.readRequestHeader(codec)if err != nil {if !keepReading {return}// discard body                                                         
        codec.ReadRequestBody(nil)return}// 解析请求中的args                                               
    argIsValue := false // if true, need to indirect before calling.            if mtype.ArgType.Kind() == reflect.Ptr {                                    
        argv = reflect.New(mtype.ArgType.Elem())} else {                                                                    
        argv = reflect.New(mtype.ArgType)                                       
        argIsValue = true}// argv guaranteed to be a pointer now.                                     if err = codec.ReadRequestBody(argv.Interface()); err != nil {return}if argIsValue {                                                             
        argv = argv.Elem()}// 初始化reply类型                                                                            
    replyv = reflect.New(mtype.ReplyType.Elem())return}

func (s *service) call(server *Server, sending *sync.Mutex, mtype *methodType, req *Request, argv, replyv reflect.Value, codec ServerCodec) {
    mtype.Lock()                                                                
    mtype.numCalls++                                                            
    mtype.Unlock()function := mtype.method.Func// Invoke the method, providing a new value for the reply.  // 这里是真正调用rpc方法的地方                
    returnValues := function.Call([]reflect.Value{s.rcvr, argv, replyv})// The return value for the method is an error.                             
    errInter := returnValues[0].Interface()                                     
    errmsg := ""if errInter != nil {                                                        
        errmsg = errInter.(error).Error()}// 处理返回请求了                                                                 
    server.sendResponse(sending, req, replyv.Interface(), codec, errmsg)        
    server.freeRequest(req)} 

func (server *Server) sendResponse(sending *sync.Mutex, req *Request, reply interface{}, codec ServerCodec, errmsg string) {
    resp := server.getResponse()// Encode the response header                                               
    resp.ServiceMethod = req.ServiceMethodif errmsg != "" {                                                           
        resp.Error = errmsg                                                     
        reply = invalidRequest                                                  
    }// 上一文提到，客户端是根据序号来定位请求的，所以需要原样返回                                                                     
    resp.Seq = req.Seq                                                          
    sending.Lock()                                                              
    err := codec.WriteResponse(resp, reply)if debugLog && err != nil {                                                 
        log.Println("rpc: writing response:", err)}                                                                           
    sending.Unlock()                                                            
    server.freeResponse(resp)} 
```

资源重用：

上面把大致的rpc请求都说明了，server有一个技巧是重用对象，这里使用的是链表方式处理的：

```
// 可以看出使用一个free list链表，来避免Request以及Response对象频繁创建，导致GC压力
func (server *Server) getRequest() *Request {                                   
    server.reqLock.Lock()                                                       
    req := server.freeReq                                                       
    if req == nil {                                                             
        req = new(Request)} else {                                                                    
        server.freeReq = req.next*req = Request{}}                                                                           
    server.reqLock.Unlock()return req                                                                  
}                                                                               

func (server *Server) freeRequest(req *Request) {                               
    server.reqLock.Lock()                                                       
    req.next = server.freeReq                                                   
    server.freeReq = req                                                        
    server.reqLock.Unlock()}                                                                               

func (server *Server) getResponse() *Response {                                 
    server.respLock.Lock()                                                      
    resp := server.freeResp                                                     
    if resp == nil {                                                            
        resp = new(Response)} else {                                                                    
        server.freeResp = resp.next*resp = Response{}}                                                                           
    server.respLock.Unlock()return resp                                                                 
}                                                                               

func (server *Server) freeResponse(resp *Response) {                            
    server.respLock.Lock()                                                      
    resp.next = server.freeResp                                                 
    server.freeResp = resp                                                      
    server.respLock.Unlock()} 
```

最后，sending这把锁的目的是避免同一个套接字快速请求中避免返回包写入乱序，因此避免一个包完整写入完毕才允许下一个返回写入套接字。通过rpc包源码解析，可以看到标准库中的核心思想还是channel+mutex实现复用对象，以及各种方式的复用，避免GC压力，在我们以后写高性能服务端可以借鉴的地方。

Posted in [Golang](http://dev.cmcm.com/archives/category/golang).| Tagged [Golang](http://dev.cmcm.com/archives/tag/golang), [rpc](http://dev.cmcm.com/archives/tag/rpc).		| 28 views

来源： <<http://dev.cmcm.com/archives/324>>

 

