

# tools



## mac版本ldd： otool



```shell
otool -L `which vim`                                              1
/usr/local/bin/vim:
	/usr/lib/libncurses.5.4.dylib (compatibility version 5.4.0, current version 5.4.0)
	/usr/lib/libiconv.2.dylib (compatibility version 7.0.0, current version 7.0.0)
	/System/Library/Frameworks/Cocoa.framework/Versions/A/Cocoa (compatibility version 1.0.0, current version 22.0.0)
	/usr/local/opt/perl/lib/perl5/5.26.0/darwin-thread-multi-2level/CORE/libperl.dylib (compatibility version 5.26.0, current version 5.26.0)
	/usr/lib/libSystem.B.dylib (compatibility version 1.0.0, current version 1238.60.2)
	/usr/lib/libutil.dylib (compatibility version 1.0.0, current version 1.0.0)
	/usr/local/opt/python/Frameworks/Python.framework/Versions/2.7/Python (compatibility version 2.7.0, current version 2.7.0)
	/usr/local/opt/ruby/lib/libruby.2.4.1.dylib (compatibility version 2.4.0, current version 2.4.1)
	/usr/lib/libobjc.A.dylib (compatibility version 1.0.0, current version 228.0.0)
	/System/Library/Frameworks/AppKit.framework/Versions/C/AppKit (compatibility version 45.0.0, current version 1504.83.101)
	/System/Library/Frameworks/CoreFoundation.framework/Versions/A/CoreFoundation (compatibility version 150.0.0, current version 1349.8.0)
	/System/Library/Frameworks/CoreServices.framework/Versions/A/CoreServices (compatibility version 1.0.0, current version 775.19.0)
	/System/Library/Frameworks/Foundation.framework/Versions/C/Foundation (compatibility version 300.0.0, current version 1349.81.0)
```















