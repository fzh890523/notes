# 零碎



## 坑： annotation实例是proxy类导致无访问权限



```java
class com.sun.proxy.$Proxy5
```



```
packageA
	Annotation1
		在其中访问field1、enum2会导致报错，无访问权限。 改为public就好了。
	Class1
		protect field1
		default enum2
```





