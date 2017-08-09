

## 线程安全



* Pattern - YES
* Matcher - NOT



```
Yes, from the Java API documentation for the Pattern class

Instances of this (Pattern) class are immutable and are safe for use by multiple concurrent threads. Instances of the Matcher class are not safe for such use.
If you are looking at performance centric code, attempt to reset the Matcher instance using the reset() method, instead of creating new instances. This would reset the state of the Matcher instance, making it usable for the next regex operation. In fact, it is the state maintained in the Matcher instance that is responsible for it to be unsafe for concurrent access.


http://stackoverflow.com/questions/1360113/is-java-regex-thread-safe
```







