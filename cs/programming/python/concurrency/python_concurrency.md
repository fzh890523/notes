



# 多线程



## 线程同步



ref： [Thread Synchronization Mechanisms in Python](http://effbot.org/zone/thread-synchronization.htm) or [local](resource/Thread Synchronization Mechanisms in Python.html)



* 对共享资源访问的同步（不要争，慢慢来）
* 线程间（执行）的同步（一二一，齐步走）



### 对共享资源访问的同步（不要争，慢慢来）

#### 语言规范里的原子操作

比较让人意外的，**python里没有atomic API**，但是**很多语言规范里原生支持的线程安全特性覆盖了大多数atomic场景**

The simplest way to synchronize access to shared variables or other resources is to rely on atomic operations in the interpreter. An atomic operation is an operation that is carried out in a single execution step, without any chance that another thread gets control.

In general, this approach only works if the shared resource consists of a single instance of a core data type, such as a string variable, a number, or a list or dictionary. 

- reading or replacing a single instance attribute
- reading or replacing a single global variable
- fetching an item from a list
- modifying a list in place (e.g. adding an item using **append**)
- fetching an item from a dictionary
- modifying a dictionary in place (e.g. adding an item, or calling the **clear** method)



**线程同步方式**

* lock
* semaphore



#### lock

Locks are the most fundamental synchronization mechanism provided by the **threading** module. At any time, a lock can be held by a single thread, or by no thread at all. If a thread attempts to hold a lock that’s already held by some other thread, execution of the first thread is halted until the lock is released.

Locks are typically used to synchronize access to a shared resource. For each shared resource, create a **Lock** object. When you need to access the resource, call **acquire** to hold the lock (this will wait for the lock to be released, if necessary), and call **release** to release it

* Lock
* RLock 可重入



#### semaphore

A semaphore is a more advanced lock mechanism. A semaphore has an internal counter rather than a lock flag, and it only blocks if more than a given number of threads have attempted to hold the semaphore. Depending on how the semaphore is initialized, this allows multiple threads to access the same code section simultaneously.



* Semaphore
* BoundedSemaphore



### 线程间（执行）的同步（一二一，齐步走）

和前面对共享资源的同步方式相比，区别主要不在实现细节，而在“语义”。

这几种方式不面向资源，而面向线程本身关心的**事件**、**条件**、**状态**等。





#### event

An event is a simple synchronization object; the event represents an internal flag, and threads can wait for the flag to be set, or set or clear the flag themselves.





```python
event = threading.Event()

# a client thread can wait for the flag to be set
event.wait()

# a server thread can set or reset it
event.set()
event.clear()
```





#### condition

A condition is a more advanced version of the event object. A condition represents some kind of state change in the application, and a thread can wait for a given condition, or signal that the condition has happened. Here’s a simple consumer/producer example. First, you need a condition object

```python
# represents the addition of an item to a resource
condition = threading.Condition()
# The producing thread needs to acquire the condition before it can notify the consumers that a new item is available:

# producer thread
# ... generate item
condition.acquire()
# ... add item to resource
condition.notify() # signal that a new item is available
condition.release()
# The consumers must acquire the condition (and thus the related lock), and can then attempt to fetch items from the resource:

# consumer thread
condition.acquire()
while True:
    ... get item from resource
    if item:
        break
    condition.wait() # sleep until item becomes available
condition.release()
# ... process item
```

