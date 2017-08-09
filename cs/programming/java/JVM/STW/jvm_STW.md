

## STW的目的

ensure what initiator have **exclusive access to JVM data structures** and can do crazy things like moving objects in heap or replacing code of method which is currently running (On-Stack-Replacement).



具体的见下面的时机。



## STW发生的时机



 Oracle Hotspot implementation will perform a request to stop all threads when: 

* Garbage collection pauses (depends on the GC algorithm being used)
* Code deoptimization (in response to new information from newly loaded classes, null pointer exceptions, division by zero traps and so forth)
* Flushing code cacheClass 
* redefinition (e.g. hot swap or instrumentation)
* Biased lock revocation (and as we learned in this post, turning the BiasedLock mechanism on)
* Various debug operation (e.g. deadlock check or stacktrace dump)



## STW的实现

> STW也要实现？ 惊人问号...

> sure...



### Hotspot实现

主要见safepoint部分。

* VM notify STW
  * how？
* threads poll STW when in safe-points
  * safe-point如何到达？
    * 会不会有迟迟不能到达的情况？
    * 部分到达个别迟迟到达的结果是？ hang...
  * 如何poll？

