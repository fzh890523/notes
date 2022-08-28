

* http

  * `buildSidecarOutboundListeners`

    ```go
    if egressListener.IstioListener != nil &&
    			egressListener.IstioListener.Port != nil {
    // ...
          bind := egressListener.IstioListener.Bind
    			if bind == "" {
    				if bindToPort {
    					bind = actualLocalHostAddress
    				} else {
    					bind = actualWildcard
    				}
    			}
    
    else {        
    // ...
    			bind := ""
    			if egressListener.IstioListener != nil && egressListener.IstioListener.Bind != "" {
    				bind = egressListener.IstioListener.Bind
    			}
    			if bindToPort && bind == "" {
    				bind = actualLocalHostAddress
    			}
      
          // ...
      
          // headless服务 且有效svc addr（这个可能来自智能dns分配的“假” svc addr）为0.0.0.0（说明是无...选的兜底地址） 并且是tcp or unsupported（HTTP的话估计直接聚合到wildcard）， 则...会取instances addr来...，生成多个listener
    					// Support statefulsets/headless services with TCP ports, and empty service address field.
    					// Instead of generating a single 0.0.0.0:Port listener, generate a listener
    					// for each instance. HTTP services can happily reside on 0.0.0.0:PORT and use the
    					// wildcard route match to get to the appropriate IP through original dst clusters.
    					if features.EnableHeadlessService && bind == "" && service.Resolution == model.Passthrough &&
    						saddress == constants.UnspecifiedIP && (servicePort.Protocol.IsTCP() || servicePort.Protocol.IsUnsupported())   
          // ...
    ```

    

  * `buildSidecarOutboundHTTPListenerOptsForPortOrUDS`

    ```go
    	// first identify the bind if its not set. Then construct the key
    	// used to lookup the listener in the conflict map.
    	if len(listenerOpts.bind) == 0 { // no user specified bind. Use 0.0.0.0:Port
    		listenerOpts.bind = actualWildcard
    	}
    ```

  > 可以看到**http svc基本没用svc addr**

* tcp

  * `buildSidecarOutboundListeners`

    > 见http部分

  * `buildSidecarOutboundTCPListenerOptsForPortOrUDS`

    ```go
    	// Determine the listener address if bind is empty
    	// we listen on the service VIP if and only
    	// if the address is an IP address. If its a CIDR, we listen on
    	// 0.0.0.0, and setup a filter chain match for the CIDR range.
    	// As a small optimization, CIDRs with /32 prefix will be converted
    	// into listener address so that there is a dedicated listener for this
    	// ip:port. This will reduce the impact of a listener reload
    
    	if len(listenerOpts.bind) == 0 {
    		svcListenAddress := listenerOpts.service.GetAddressForProxy(listenerOpts.proxy)
    		// We should never get an empty address.
    		// This is a safety guard, in case some platform adapter isn't doing things
    		// properly
    		if len(svcListenAddress) > 0 {
    			if !strings.Contains(svcListenAddress, "/") {
    				listenerOpts.bind = svcListenAddress
    			} else {
    				// Address is a CIDR. Fall back to 0.0.0.0 and
    				// filter chain match
    				*destinationCIDR = svcListenAddress
    				listenerOpts.bind = actualWildcard
    			}
    		}
    	}
    ```

  > 可以看到，tcp还是会用svc addr的

  > 对于k8s svc，是在`ClusterVIPs`中（每个cluster一个，所以实际还是取出一个来用），而对于serviceentry，是在`DefaultAddress string`中。 而多地址场景实际会转为多个`Service`实例（虽然此时怎么共存是另一个问题）

  > 此外，因为k8s svc一般有vip（headless另说，等等），所以不会分配ip。 不会出现业务用的ip1而实际下发listener是精确ip2的情况
  >
  > > 差不多headless（没vip） -> resolution Passthrough -> 从代码上看也还是不分配地址，符合预期。
  > >
  > > 这时候：
  > >
  > > * 如果启用了headless支持，会根据instance （addr）生成大量listener
  > > * 否则走到默认分支，最后用的会是0.0.0.0
  >
  > **但对于serviceentry中的tcp服务，似乎有点问题**： 假设定义serviceentry把ep addr指向了某个能访问的ip（但没vip也即se.addresses），那... **也即istio默认服务都是基于dns的，如果不是，那就设置vip/addresses**

  

  