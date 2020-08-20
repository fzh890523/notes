







# link



```sh
dlopen(libnghttp2.so, 6): image not found
```





```sh
otool -L libnghttp2.so             1   master
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/objdump: 'libnghttp2.so': No such file or directory.
fatal error: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/otool: internal objdump command failed
```



```
Then, where does Mac OS X store the default search path for libraries?

It's simply hard-coded into the dynamic loader (dyld). Search for "sLibraryFallbackPaths" in this code. By the way, it shows that the man page is incorrect to include "/lib". That's not actually part of the default search path.
```



```cpp
#if __MAC_OS_X_VERSION_MIN_REQUIRED
static const char*					sFrameworkFallbackPaths[] = { "$HOME/Library/Frameworks", "/Library/Frameworks", "/Network/Library/Frameworks", "/System/Library/Frameworks", NULL };
static const char*					sLibraryFallbackPaths[] = { "$HOME/lib", "/usr/local/lib", "/usr/lib", NULL };
static const char*					sRestrictedFrameworkFallbackPaths[] = { "/System/Library/Frameworks", NULL };
static const char*					sRestrictedLibraryFallbackPaths[] = { "/usr/lib", NULL };
#else
static const char*					sFrameworkFallbackPaths[] = { "/System/Library/Frameworks", NULL };
static const char*					sLibraryFallbackPaths[] = { "/usr/local/lib", "/usr/lib", NULL };
#endif
```



















