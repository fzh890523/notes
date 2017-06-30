# 介绍



ref：

* [wikipedia - Monitor](https://en.wikipedia.org/wiki/Monitor_(synchronization))



## 同步场景

* fork-and-join

  **等待**、**协调**、**面向任务**

* producer-and-consumer

  **等待**、**协调**、**面向任务**

* 排他方式使用资源

  **互斥**、**面向资源**





# 概念

## 共享资源、竞争、临界区、串行化

1. 共享资源导致竞争

2. 解决竞争的思路： 临界区 - 一次最多一个

3. 临界区的结果： 串行化

   也可以说串行化是思路



## lock, mutex, monitor, semaphore, condition, event 等概念

下面尽量按语义层级来

* monitor - 管程

* condition (viarable) - 条件变量

  > is basically a container of threads that are waiting for a certain condition. 

* lock

  比较形象的词，不够抽象，所以其实语义不明显。

  并没有限制实现的方式，比如可以：

  * mutex lock

    mutex本身字面上语义和lock同，只是典型实现里mutex为 block-lock，所以就mutex就成为...的代名词。

    而且有些场景下，mutex就是lock的含义而非指阻塞锁。如：

    > A [mutex](https://en.wikipedia.org/wiki/Mutex) is essentially the same thing as a binary semaphore and sometimes uses the same basic implementation. 

  * spin lock

  > **锁不必是互斥**

* semaphore



### 重要： 语义关系

其实语义是逻辑层面的概念，并不意味着实现，所以：

* lock - semaphore

  lock + count 可以表示semaphore

* lock - condition

  lock + 状态 可以表示 condition，比如

  * lock + isEmpty -> emptyCond
  * lock + isFull -> fullCond

  不过同一个lock关联的多个状态（得到的多个cond）应该是互斥的（毕竟lock本身互斥）

  > 不过语义操作上，lock比较简单，不能区分notify和notifyAll（当然lock实现上可以；lock加上一些控制逻辑和数据结构也能实现完整condition语义操作）

* semaphore - condition

  数量为1的semaphore 可以表示 condition（1和0表示ok、nok）

* semaphore - lock

  数量为1的semaphore 可以表示 lock


* condition - lock

  把临界区ready视为条件ok的话，condition 则表示 lock

* condition - semaphore

  condition + count 可以表示 semaphore

# monitor - 管程

* 协调 而不是 互斥
* 面向任务 而不是 （共享）资源



> a synchronization construct that allows [threads](https://en.wikipedia.org/wiki/Thread_(computing)) to have both [mutual exclusion](https://en.wikipedia.org/wiki/Mutual_exclusion) and the ability to wait (block) for a certain condition to become true

>  also have a mechanism for signalling other threads that their condition has been met.

- consists of a [mutex (lock)](https://en.wikipedia.org/wiki/Lock_(computer_science)) object and **condition variables**.
- provide a mechanism for threads to temporarily give up exclusive access in order to wait for some condition to be met, before regaining exclusive access and resuming their task.

> 不只是互斥，而是 **等待（条件） -> 互斥争抢**
>
> 当然了，实际上，加入等待队列需要一个互斥量，而条件达到时的执行可能是不用争抢而是按（队列）序的



**注意**：**java中的monitor有不同的含义**

>  a **thread-safe** [class](https://en.wikipedia.org/wiki/Class_(computer_science)), [object](https://en.wikipedia.org/wiki/Object_(computer_science)), or [module](https://en.wikipedia.org/wiki/Module_(programming)) that uses wrapped [mutual exclusion](https://en.wikipedia.org/wiki/Mutual_exclusion) in order to safely allow access to a method or variable by more than one [thread](https://en.wikipedia.org/wiki/Thread_(computer_science)).
>
> The defining characteristic of a monitor is that its methods are executed with [mutual exclusion](https://en.wikipedia.org/wiki/Mutual_exclusion): At each point in time, at most one thread may be executing any of its [methods](https://en.wikipedia.org/wiki/Method_(computer_science)).
>
> > 也就是说 **java里的monitor只有互斥的语义**

> By using one or more condition variables it can also provide the ability for threads to wait on a certain condition (thus using the above definition of a "monitor"). 
>
> > 也就说 **本来可能没有** 或者 **本不必有** **等待条件**的语义

> wikipedia里把这种monitor干脆称为**syntactic sugar “monitor class”**

```java
class Account {
  private lock myLock

  private int balance := 0
  invariant balance >= 0

  public method boolean withdraw(int amount)
     precondition amount >= 0
  {
    myLock.acquire()
    try {
      if balance < amount {
        return false
      } else {
        balance := balance - amount
        return true
      }
    } finally {
      myLock.release()
    }
  }

  public method deposit(int amount)
     precondition amount >= 0
  {
    myLock.acquire()
    try {
      balance := balance + amount
    } finally {
      myLock.release()
    }
  }
}
```





# lock - 锁

锁应该只有等待的语义

语义操作只有：

* acquire（lock）
* release（unlock）



至于是互斥还是spin得方式的等待，则是实现选择了。

# mutex - 互斥量



## 实现



### 伪码实现 - wikipedia

TAS + 



```java
class Mutex {
    protected volatile bool held=false;
    private volatile ThreadQueue blockingThreads; // Thread-unsafe queue of blocked threads.  Elements are (Thread*).
    
    public method acquire(){
        // Internal spin-lock while other threads on any core are accessing this object's
        // "held" and "threadQueue", or "readyQueue".
        while (testAndSet(threadingSystemBusy)){}
        // N.B.: "threadingSystemBusy" is now true.
        
        // System call to disable interrupts on this core so that threadSleep() doesn't get interrupted by
        // the thread-switching timer on this core which would call contextSwitchISR().
        // Done outside threadSleep() for more efficiency so that this thread will be sleeped
        // right after going on the lock queue.
        systemCall_disableInterrupts();

        assert !blockingThreads.contains(currentThread);

        if (held){
            // Put "currentThread" on this lock's queue so that it will be
            // considered "sleeping" on this lock.
            // Note that "currentThread" still needs to be handled by threadSleep().
            readyQueue.remove(currentThread);
            blockingThreads.enqueue(currentThread);
            threadSleep();
            
            // Now we are woken up, which must be because "held" became false.
            assert !held;
            assert !blockingThreads.contains(currentThread);
        }
        
        held=true;
        
        threadingSystemBusy=false; // Must be an atomic assignment.
        systemCall_enableInterrupts(); // Turn pre-emptive switching back on on this core.

    }        
        
    public method release(){
        // Internal spin-lock while other threads on any core are accessing this object's
        // "held" and "threadQueue", or "readyQueue".
        while (testAndSet(threadingSystemBusy)){}
        // N.B.: "threadingSystemBusy" is now true.
        
        // System call to disable interrupts on this core for efficiency.
        systemCall_disableInterrupts();
        
        assert held; // (Release should only be performed while the lock is held.)

        held=false;
        
        if (!blockingThreads.isEmpty()){
            Thread* unblockedThread=blockingThreads.dequeue();
            readyQueue.enqueue(unblockedThread);
        }
        
        threadingSystemBusy=false; // Must be an atomic assignment.
        systemCall_enableInterrupts(); // Turn pre-emptive switching back on on this core.
        
    }

}
```







# condition variable条件变量



## why need variable?

### 演示

如果没有条件变量，那么类似的代码会变成：

```python
while True:
    with lock:
        if ok:
            break
    time.sleep(some_time)
```

问题在于，`some_time`如果太大，则会响应的不够及时，而如果太小，会带来大量的锁争抢和上下文切换（当然，usr time也会大量增加）。



典型场景是： 有界的**queue** 或 **ringbuffer**



#### “直接”代码：

```Java
global RingBuffer queue; // A thread-unsafe ring-buffer of tasks.

// Method representing each producer thread's behavior:
public method producer(){
    while(true){
        task myTask=...; // Producer makes some new task to be added.
        while(queue.isFull()){} // Busy-wait until the queue is non-full.
        queue.enqueue(myTask); // Add the task to the queue.
    }
}

// Method representing each consumer thread's behavior:
public method consumer(){
    while(true){
        while (queue.isEmpty()){} // Busy-wait until the queue is non-empty.
        myTask=queue.dequeue(); // Take a task off of the queue.
        doStuff(myTask); // Go off and do something with the task.
    }
}
```

> 很明显，并发不安全，即使 `isXX` 能准确获得条件信息，在 `判断后-action前`是不安全的（更别说action自己也不安全）



#### lock + busy-wait

```java
global RingBuffer queue; // A thread-unsafe ring-buffer of tasks.
global Lock queueLock; // A mutex for the ring-buffer of tasks.

// Method representing each producer thread's behavior:
public method producer(){
    while(true){
        task myTask=...; // Producer makes some new task to be added.

        queueLock.acquire(); // Acquire lock for initial busy-wait check.
        while(queue.isFull()){ // Busy-wait until the queue is non-full.
            queueLock.release();
            // Drop the lock temporarily to allow a chance for other threads
            // needing queueLock to run so that a consumer might take a task.
            queueLock.acquire(); // Re-acquire the lock for the next call to "queue.isFull()".
        }

        queue.enqueue(myTask); // Add the task to the queue.
        queueLock.release(); // Drop the queue lock until we need it again to add the next task.
    }
}

// Method representing each consumer thread's behavior:
public method consumer(){
    while(true){
        queueLock.acquire(); // Acquire lock for initial busy-wait check.
        while (queue.isEmpty()){ // Busy-wait until the queue is non-empty.
            queueLock.release();
            // Drop the lock temporarily to allow a chance for other threads
            // needing queueLock to run so that a producer might add a task.
            queueLock.acquire(); // Re-acquire the lock for the next call to "queue.isEmpty()".
        }
        myTask=queue.dequeue(); // Take a task off of the queue.
        queueLock.release(); // Drop the queue lock until we need it again to take off the next task.
        doStuff(myTask); // Go off and do something with the task.
    }
}
```

这是一个可行方案，重点是：

1. 在lock保护范围内检查条件
2. 如条件达成则不释放lock继续在lock保护范围内操作，完成（或失败）后才释放
3. 用busy-wait模型释放lock以使得其他task能

问题：

1. 仍然是busy-wait模型，开销太大（cs、usr）



#### 条件变量

```java
global volatile RingBuffer queue; // A thread-unsafe ring-buffer of tasks.
global Lock queueLock;  // A mutex for the ring-buffer of tasks.  (Not a spin-lock.)
global CV queueEmptyCV; // A condition variable for consumer threads waiting for the queue to become non-empty.
                        // Its associated lock is "queueLock".
global CV queueFullCV; // A condition variable for producer threads waiting for the queue to become non-full.
                       //Its associated lock is also "queueLock".

// Method representing each producer thread's behavior:
public method producer(){
    while(true){
        task myTask=...; // Producer makes some new task to be added.

        queueLock.acquire(); // Acquire lock for initial predicate check.
        while(queue.isFull()){ // Check if the queue is non-full.
            // Make the threading system atomically release queueLock,
            // enqueue this thread onto queueFullCV, and sleep this thread.
            wait(queueLock, queueFullCV);
            // Then, "wait" automatically re-acquires "queueLock" for re-checking
            // the predicate condition.
        }
        
        // Critical section that requires the queue to be non-full.
        // N.B.: We are holding queueLock.
        queue.enqueue(myTask); // Add the task to the queue.

        // Now the queue is guaranteed to be non-empty, so signal a consumer thread
        // or all consumer threads that might be blocked waiting for the queue to be non-empty:
        signal(queueEmptyCV); -- OR -- notifyAll(queueEmptyCV);
        
        // End of critical sections related to the queue.
        queueLock.release(); // Drop the queue lock until we need it again to add the next task.
    }
}

// Method representing each consumer thread's behavior:
public method consumer(){
    while(true){

        queueLock.acquire(); // Acquire lock for initial predicate check.
        while (queue.isEmpty()){ // Check if the queue is non-empty.
            // Make the threading system atomically release queueLock,
            // enqueue this thread onto queueEmptyCV, and sleep this thread.
            wait(queueLock, queueEmptyCV);
            // Then, "wait" automatically re-acquires "queueLock" for re-checking
            // the predicate condition.
        }
        // Critical section that requires the queue to be non-empty.
        // N.B.: We are holding queueLock.
        myTask=queue.dequeue(); // Take a task off of the queue.
        // Now the queue is guaranteed to be non-full, so signal a producer thread
        // or all producer threads that might be blocked waiting for the queue to be non-full:
        signal(queueFullCV); -- OR -- notifyAll(queueFullCV);

        // End of critical sections related to the queue.
        queueLock.release(); // Drop the queue lock until we need it again to take off the next task.

        doStuff(myTask); // Go off and do something with the task.
    }
}
```



也可以使用单CV版本：

```java
global volatile RingBuffer queue; // A thread-unsafe ring-buffer of tasks.
global Lock queueLock; // A mutex for the ring-buffer of tasks.  (Not a spin-lock.)
global CV queueFullOrEmptyCV; // A single condition variable for when the queue is not ready for any thread
                              // -- i.e., for producer threads waiting for the queue to become non-full 
                              // and consumer threads waiting for the queue to become non-empty.
                              // Its associated lock is "queueLock".
                              // Not safe to use regular "signal" because it is associated with
                              // multiple predicate conditions (assertions).

// Method representing each producer thread's behavior:
public method producer(){
    while(true){
        task myTask=...; // Producer makes some new task to be added.

        queueLock.acquire(); // Acquire lock for initial predicate check.
        while(queue.isFull()){ // Check if the queue is non-full.
            // Make the threading system atomically release queueLock,
            // enqueue this thread onto the CV, and sleep this thread.
            wait(queueLock, queueFullOrEmptyCV);
            // Then, "wait" automatically re-acquires "queueLock" for re-checking
            // the predicate condition.
        }
        
        // Critical section that requires the queue to be non-full.
        // N.B.: We are holding queueLock.
        queue.enqueue(myTask); // Add the task to the queue.

        // Now the queue is guaranteed to be non-empty, so signal all blocked threads
        // so that a consumer thread will take a task:
        notifyAll(queueFullOrEmptyCV); // Do not use "signal" (as it might wake up another producer instead).
        
        // End of critical sections related to the queue.
        queueLock.release(); // Drop the queue lock until we need it again to add the next task.
    }
}

// Method representing each consumer thread's behavior:
public method consumer(){
    while(true){

        queueLock.acquire(); // Acquire lock for initial predicate check.
        while (queue.isEmpty()){ // Check if the queue is non-empty.
            // Make the threading system atomically release queueLock,
            // enqueue this thread onto the CV, and sleep this thread.
            wait(queueLock, queueFullOrEmptyCV);
            // Then, "wait" automatically re-acquires "queueLock" for re-checking
            // the predicate condition.
        }
        // Critical section that requires the queue to be non-full.
        // N.B.: We are holding queueLock.
        myTask=queue.dequeue(); // Take a task off of the queue.

        // Now the queue is guaranteed to be non-full, so signal all blocked threads
        // so that a producer thread will take a task:
        notifyAll(queueFullOrEmptyCV); // Do not use "signal" (as it might wake up another consumer instead).

        // End of critical sections related to the queue.
        queueLock.release(); // Drop the queue lock until we need it again to take off the next task.

        doStuff(myTask); // Go off and do something with the task.
    }
}
```

这里就要如前面说的notifyAll了。

但这样性能还是偏低啊，比如 notEmpty 条件达到时，无谓的把所有 wait-for-notFull（也就是producer）全部唤醒来争抢锁了...





### 小结

* 有多（**其实只是2**）种状态才称为条件
* `wait for (one of) condition` 并不等于 简单的互斥，因为**等待在不同状态上tasks并不是互斥关系**，甚至**等待在同一状态上的tasks也可能不是互斥关系**，只有 **对状态的改变才是临界区才需要互斥**

## 概念

> Conceptually a condition variable is **a queue of threads**, associated with **a monitor**, on which a thread may wait for some condition to become true. 

* Thus each condition variable c is associated with an [assertion](https://en.wikipedia.org/wiki/Assertion_(computing)) Pc. 

* While a thread is waiting on a condition variable, that thread is not considered to occupy the monitor, and so other threads may enter the monitor to change the monitor's state. In most types of monitors, these other threads may signal the condition variable c to indicate that assertion Pc is true in the current state.

  把**等待**和**持有**分开了，这样就允许多个tasks进入等待。



### 组成

* （等待）任务队列



CV关联到

* monitor
  * a [mutex (lock)](https://en.wikipedia.org/wiki/Lock_(computer_science)) associated with the monitor



从下文可以知道，在单核环境下可以不使用monitor只是禁用中断的方式实现并发安全的CV，此时CV内容就只是一个**等待任务队列**。

而多核环境下，**需要在多核间同步（协调），所以引入monitor**。



### 语义操作

* **wait** c, m

  **wait会导致 task OK -> wait 状态**（当然也不一定，如刚add to queue就被notify选中或notifyAll了）

  会做如下事情：

  1. 释放m

  2. 将task从running -> c的wait-queue

     wikipedia里说1、2可以是任意顺序，但每种情况需要考虑其后果，比如如果 2-1 的话失去m的保护，c.wait_queue有并发访问问题

  3. thread sleep

* **signal** c / **notify** c

  **notify/notifyAll会导致 选中task wait ->  block 状态**，然后**抢到_lock的那个状态 block -> OK**

  > 这里 _lock 是指保护condition queue的那个lock，参考下面py实现

  会做如下事情：

  1. 将task/tasks从c.sleep_queue移出（到“ready queue”或者其他类似）

  2. 通知这些tasks（signal/notify）

  3. 释放m

     一般认为 2-3 是最佳实践，但如果充分设计好并发情况的话，其他顺序也可以。

* **broadcast** c / **notifyAll** c

  清空c.sleep_queue，唤醒其中所有tasks

  一般，如果条件变量关联了多个predicate condition的话，应该用notifyAll而不是notify。 因为如果用notify的话，可能一个被错误唤醒（唤醒后发现自己期望的条件并没达成）的task可能直接继续sleep而没有唤醒下一个，这样就GG了。

  > 见前面示例

  除此以外，notify一般会更高效。



#### notify和notifyAll区别

假定场景：

* 已有10个task为wait状态

* 有1个task执行完，需要做notify

  * 调notify

    * 另一个task -> block，然后随着该task释放`_lock`而获得`_lock`而 -> OK，其余9个仍为wait状态
    * 新task执行完，做notify，重复上面的流程

  * 调notifyAll

    * 所有task -> block，然后随着该task释放而挣抢`_lock`...

      > 这个看lock实现吧，释放锁时是唤醒所有来争抢还是...
      >
      > java里： The highest priority thread will run first in most of the situation, though not guaranteed.

    * 之后每次释放`_lock`都会面对so many block tasks的情况

      > 无论如何开销还是比notify大一些吧，只是看lock实现确定大多少
      >
      > 而且还有个问题是： 
      >
      > * `_lock`这把锁的主要是控制wait_queue访问的，也就是控制**尝试wait**的，所以这把锁往往粒度小也即获取/释放频繁
      >
      > 如果大量的tasks是在block而不是wait的话，有一些**本以为短暂加锁**的逻辑会因为锁竞争而耗时超过预期，比如之后再执行单个notify时，最后获得锁的不是wait的而是众多block的tasks之一的概率较大



## 实现



### 伪码实现 - wikipedia



```java
struct ConditionVariable {
    volatile ThreadQueue waitingThreads;
}

public method wait(ConditionVariable c){
    // Internal spin-lock while other threads on any core are accessing this object's
    // "held" and "threadQueue", or "readyQueue".
    while (testAndSet(threadingSystemBusy)){}
    // N.B.: "threadingSystemBusy" is now true.
    
    // System call to disable interrupts on this core so that threadSleep() doesn't get interrupted by
    // the thread-switching timer on this core which would call contextSwitchISR().
    // Done outside threadSleep() for more efficiency so that this thread will be sleeped
    // right after going on the condition-variable queue.
    systemCall_disableInterrupts();
 
    c.waitingThreads.enqueue(currentThread);
    
    threadSleep();
    
    // Thread sleeps ... Thread gets woken up from a signal/broadcast.
    
    threadingSystemBusy=false; // Must be an atomic assignment.
    systemCall_enableInterrupts(); // Turn pre-emptive switching back on on this core.
}

public method signal(ConditionVariable c){

    // Internal spin-lock while other threads on any core are accessing this object's
    // "held" and "threadQueue", or "readyQueue".
    while (testAndSet(threadingSystemBusy)){}
    // N.B.: "threadingSystemBusy" is now true.
    
    // System call to disable interrupts on this core so that threadSleep() doesn't get interrupted by
    // the thread-switching timer on this core which would call contextSwitchISR().
    // Done outside threadSleep() for more efficiency so that this thread will be sleeped
    // right after going on the condition-variable queue.
    systemCall_disableInterrupts();
    
    if (!c.waitingThreads.isEmpty()){
        wokenThread=c.waitingThreads.dequeue();
        readyQueue.enqueue(wokenThread);
    }
    
    threadingSystemBusy=false; // Must be an atomic assignment.
    systemCall_enableInterrupts(); // Turn pre-emptive switching back on on this core.
    
    // Mesa style:
    // The woken thread is not given any priority.
    
}

public method broadcast(ConditionVariable c){

    // Internal spin-lock while other threads on any core are accessing this object's
    // "held" and "threadQueue", or "readyQueue".
    while (testAndSet(threadingSystemBusy)){}
    // N.B.: "threadingSystemBusy" is now true.
    
    // System call to disable interrupts on this core so that threadSleep() doesn't get interrupted by
    // the thread-switching timer on this core which would call contextSwitchISR().
    // Done outside threadSleep() for more efficiency so that this thread will be sleeped
    // right after going on the condition-variable queue.
    systemCall_disableInterrupts();
    
    while (!c.waitingThreads.isEmpty()){
        wokenThread=c.waitingThreads.dequeue();
        readyQueue.enqueue(wokenThread);
    }
    
    threadingSystemBusy=false; // Must be an atomic assignment.
    systemCall_enableInterrupts(); // Turn pre-emptive switching back on on this core.
    
    // Mesa style:
    // The woken threads are not given any priority.
    
}
```

然而这个在多核环境下不是线程安全的，可以选择用**monitor**（如前面实现的**Mutex**）来保护多核环境下线程安全。

***？？？ 怎么看起来不是这样啊… = =***

> 既然threadingSystemBusy是global的，也就等于多核都生效的spin lock了，虽然这把锁超级大 - kernel级别
>
> 



下面是**多核安全版本**

```java
struct ConditionVariable {
    volatile ThreadQueue waitingThreads;
}

public method wait(Mutex m, ConditionVariable c){
    // Internal spin-lock while other threads on any core are accessing this object's
    // "held" and "threadQueue", or "readyQueue".
    while (testAndSet(threadingSystemBusy)){}
    // N.B.: "threadingSystemBusy" is now true.
    
    // System call to disable interrupts on this core so that threadSleep() doesn't get interrupted by
    // the thread-switching timer on this core which would call contextSwitchISR().
    // Done outside threadSleep() for more efficiency so that this thread will be sleeped
    // right after going on the condition-variable queue.
    systemCall_disableInterrupts();
 
    assert m.held; // (Specifically, this thread must be the one holding it.)
    
    m.release();
    c.waitingThreads.enqueue(currentThread);
    
    threadSleep();
    
    // Thread sleeps ... Thread gets woken up from a signal/broadcast.
    
    threadingSystemBusy=false; // Must be an atomic assignment.
    systemCall_enableInterrupts(); // Turn pre-emptive switching back on on this core.
    
    // Mesa style:
    // Context switches may now occur here, making the client caller's predicate false.
    
    m.acquire();
}

public method signal(ConditionVariable c){

    // Internal spin-lock while other threads on any core are accessing this object's
    // "held" and "threadQueue", or "readyQueue".
    while (testAndSet(threadingSystemBusy)){}
    // N.B.: "threadingSystemBusy" is now true.
    
    // System call to disable interrupts on this core so that threadSleep() doesn't get interrupted by
    // the thread-switching timer on this core which would call contextSwitchISR().
    // Done outside threadSleep() for more efficiency so that this thread will be sleeped
    // right after going on the condition-variable queue.
    systemCall_disableInterrupts();
    
    if (!c.waitingThreads.isEmpty()){
        wokenThread=c.waitingThreads.dequeue();
        readyQueue.enqueue(wokenThread);
    }
    
    threadingSystemBusy=false; // Must be an atomic assignment.
    systemCall_enableInterrupts(); // Turn pre-emptive switching back on on this core.
    
    // Mesa style:
    // The woken thread is not given any priority.
    
}

public method broadcast(ConditionVariable c){

    // Internal spin-lock while other threads on any core are accessing this object's
    // "held" and "threadQueue", or "readyQueue".
    while (testAndSet(threadingSystemBusy)){}
    // N.B.: "threadingSystemBusy" is now true.
    
    // System call to disable interrupts on this core so that threadSleep() doesn't get interrupted by
    // the thread-switching timer on this core which would call contextSwitchISR().
    // Done outside threadSleep() for more efficiency so that this thread will be sleeped
    // right after going on the condition-variable queue.
    systemCall_disableInterrupts();
    
    while (!c.waitingThreads.isEmpty()){
        wokenThread=c.waitingThreads.dequeue();
        readyQueue.enqueue(wokenThread);
    }
    
    threadingSystemBusy=false; // Must be an atomic assignment.
    systemCall_enableInterrupts(); // Turn pre-emptive switching back on on this core.
    
    // Mesa style:
    // The woken threads are not given any priority.
    
}
```







下面是更进一步用mutex、sem来实现condition

```java
class ConditionVariable {
 
    protected int numWaiters=0; // Roughly tracks the number of waiters blocked in sem.
                                // (The semaphore's internal state is necessarily private.)
    protected Semaphore sem=Semaphore(0); // Provides the wait queue.
    protected Mutex internalMutex; // (Really another Semaphore.  Protects "numWaiters".)
 
}

public method wait(Mutex m, ConditionVariable c){

    assert m.held;

    c.internalMutex.acquire();
    
    c.numWaiters++;
    m.release(); // Can go before/after the neighboring lines.
    c.internalMutex.release();

    // Another thread could signal here, but that's OK because of how
    // semaphores count.  If c.sem's number becomes 1, we'll have no
    // waiting time.
    c.sem.Proberen(); // Block on the CV.
    // Woken
    m.acquire(); // Re-acquire the mutex.
}

public method signal(ConditionVariable c){

    c.internalMutex.acquire();
    if (c.numWaiters > 0){
        c.numWaiters--;
        c.sem.Verhogen(); // (Doesn't need to be protected by c.internalMutex.)
    }
    c.internalMutex.release();

}

public method broadcast(ConditionVariable c){

    c.internalMutex.acquire();
    while (c.numWaiters > 0){
        c.numWaiters--;
        c.sem.Verhogen(); // (Doesn't need to be protected by c.internalMutex.)
    }
    c.internalMutex.release();

}
```



### py中Condition实现示例



> ```python
> def __init__(self, lock=None):
>     if lock is None:
>         lock = RLock()
>     self._lock = lock
>     # Export the lock's acquire() and release() methods
>     self.acquire = lock.acquire
>     self.release = lock.release
>     # ... 一些实现上的优化
>     self._waiters = _deque()
>     
> def wait(self, timeout=None):
>     if not self._is_owned():
>         raise RuntimeError("cannot wait on un-acquired lock")
>     waiter = _allocate_lock()
>     waiter.acquire()
>     self._waiters.append(waiter)
>     saved_state = self._release_save()  # 这个是超出这里的额外设计了，acquire需要传入一个凭据/状态 而 release时会将其return
>     gotit = False
>     try:    # restore state no matter what (e.g., KeyboardInterrupt)
>         if timeout is None:
>             waiter.acquire()
>             gotit = True
>         else:
>             if timeout > 0:
>                 gotit = waiter.acquire(True, timeout)
>             else:
>                 gotit = waiter.acquire(False)
>         return gotit
>     finally:
>         self._acquire_restore(saved_state)
>         if not gotit:
>             try:
>                 self._waiters.remove(waiter)
>             except ValueError:
>                 pass
>
> def notify(self, n=1):
>     if not self._is_owned():
>         raise RuntimeError("cannot notify on un-acquired lock")
>     all_waiters = self._waiters
>     waiters_to_notify = _deque(_islice(all_waiters, n))
>     if not waiters_to_notify:
>         return
>     for waiter in waiters_to_notify:
>         waiter.release()
>         try:
>             all_waiters.remove(waiter)
>         except ValueError:
>             pass
>
> # 简化版本
> def wait(self):  
>     if not self._is_owned():
>         raise RuntimeError("cannot wait on un-acquired lock")
>     waiter = _allocate_lock()
>     waiter.acquire()
>     self._waiters.append(waiter)
>     try:
>         waiter.acquire()
>     finally:
>         self._lock.acquire()
> def notify(self):
>     if not self._is_owned():
>         raise RuntimeError("cannot wait on un-acquired lock")
>     waiter = _deque(self._waiters)
>     if waiter is None:
>         return
>     waiter.release()
>     try:
>         self._waiters.remove(waiter)
>     except ValueError:
>         pass
>         
> def notify_all(self):
>     self.notify(len(self._waiters))
>     
> def __enter__(self):
>     return self._lock.__enter__()
>
> def __exit__(self, *args):
>     return self._lock.__exit__(*args)
>
> # 实例的使用方式，如semaphore，简化版本
> def acquire(self):
>     with self._cond:
>         if self.value == 0:
>             self._cond.wait()
>         self.value -= 1
> def release(self):
>     with self._cond:
>         self.value += 1
>         self._cond.notify()
>         
> ```
>
> 这个_lock保护对queue的操作 - 将自己（表示自己的waiter（这里实现是lock））入队、出队； 还保护了对value的访问
>
> 实际上如果只notify的话线程是阻塞在自己的waiter上；notifyAll的话会阻塞在_lock上，
>
> > 实际**wait会导致 task OK -> wait 状态**（当然也不一定，如刚add to queue就被notify选中或notifyAll了）； 而**notify/notifyAll会导致 选中task wait ->  block 状态**，然后抢到_lock的那个状态OK。
>
> > **可以看到wait到（返回）不只表示状态达到了，还持有了`_lock`； notify则从头到尾在持有`_lock`中 （通过with cond方式最后会释放）**



### C实现

manual，参考：

* [pthread_cond_wait](https://linux.die.net/man/3/pthread_cond_wait)
* [pthread_cond_broadcast](https://linux.die.net/man/3/pthread_cond_broadcast)
* [pthread_cond_signal](https://linux.die.net/man/3/pthread_cond_signal)
* [pthread_cond_init](https://linux.die.net/man/3/pthread_cond_init)



# semaphore



```java
monitor class Semaphore
{
  private int s := 0
  invariant s >= 0
  private Condition sIsPositive /* associated with s > 0 */

  public method P()
  {
    while s = 0:
      wait sIsPositive
    assert s > 0
    s := s - 1
  }

  public method V()
  {
    s := s + 1
    assert s > 0
    signal sIsPositive
  }
}
```





## 实现



### 伪码实现 - wikipedia



文中叫**thread-safe版semaphore**，借用前面java monitor的概念，用**monitor class**表示该类是线程安全的。

> 无需额外的措施可以保证线程安全
>
> 类似 java的 synchronized 方法，只是java里没夸张到整个类都… - 等于类的每个方法都加synchronized（属性访问直接不暴露/通过getter/setter）

```java
monitor class Semaphore
{
  private int s := 0
  invariant s >= 0
  private Condition sIsPositive /* associated with s > 0 */

  public method P()
  {
    while s = 0:
      wait sIsPositive  // 这里是 无保护的使用CV 的方式
    assert s > 0
    s := s - 1
  }

  public method V()
  {
    s := s + 1
    assert s > 0
    signal sIsPositive  // 这里是 无保护的使用CV 的方式
  }
}
```



下面是常见版本，去掉生造的**monitor**关键字，通过`Mutex`来保护线程安全：

```java
class Semaphore
{
  private volatile int s := 0
  invariant s >= 0
  private ConditionVariable sIsPositive /* associated with s > 0 */
  private Mutex myLock /* Lock on "s" */

  public method P()
  {
    myLock.acquire()
    while s = 0:
      wait(myLock, sIsPositive)
    assert s > 0
    s := s - 1
    myLock.release()
  }

  public method V()
  {
    myLock.acquire()
    s := s + 1
    assert s > 0
    signal sIsPositive
    myLock.release()
  }
}
```



相反的，也可以用monitor（Mutex）来实现semaphore：

```java
class Mutex {

    protected boolean held=false; // For assertions only, to make sure sem's number never goes > 1.
    protected Semaphore sem=Semaphore(1); // The number shall always be at most 1.
                                          // Not held <--> 1; held <--> 0.

    public method acquire(){
    
        sem.Proberen();
        assert !held;
        held=true;
    
    }
    
    public method release(){
    
        assert held; // Make sure we never Verhogen sem above 1.  That would be bad.
        held=false;
        sem.Verhogen();
    
    }

}
```





### py中Semaphore实现示例

> 比如python的Semaphore
>
> ```python
> def __init__(self, value=1):
>     if value < 0:
>         raise ValueError("semaphore initial value must be >= 0")
>         self._cond = Condition(Lock())
>         self._value = value
>         
> # 简化版本
> def acquire(self):
>     with self._cond:
>         if self.value == 0:
>             self._cond.wait()
>         self.value -= 1
> def release(self):
>     with self._cond:
>         self.value += 1
>         self._cond.notify()
> ```
>
> _cond表示`notEmptyCond`



# 同步原语

mutex、condition variable的实现基础： 硬件层面提供的同步原语，保证（部分操作的）原子性。 - 前者是后者的高层抽象。



单核情况下：

* 禁用中断就可以保证临界区 = =

  不再做CS了



多核情况下：

* 使用TAS, CAS这种read-modify-write原子指令

  具体使用的指令要看底层指令集

  >当然也有些硬件就不支持，于是GG。

  ​

  这些指令可能会（根据实现而定）：

  * 锁总线
  * 禁止重排



## 实现



### 基于TAS的线程调度实现样例

```java
// Basic parts of threading system:
// Assume "ThreadQueue" supports random access.
public volatile ThreadQueue readyQueue; // Thread-unsafe queue of ready threads.  Elements are (Thread*).
public volatile global Thread* currentThread; // Assume this variable is per-core.  (Others are shared.)

// Implements a spin-lock on just the synchronized state of the threading system itself.
// This is used with test-and-set as the synchronization primitive.
public volatile global bool threadingSystemBusy=false;

// Context-switch interrupt service routine (ISR):
// On the current CPU core, preemptively switch to another thread.
public method contextSwitchISR(){
    if (testAndSet(threadingSystemBusy)){
        return; // Can't switch context right now.
    }

    // Ensure this interrupt can't happen again which would foul up the context switch:
    systemCall_disableInterrupts();

    // Get all of the registers of the currently-running process.
    // For Program Counter (PC), we will need the instruction location of
    // the "resume" label below.  Getting the register values is platform-dependent and may involve
    // reading the current stack frame, JMP/CALL instructions, etc.  (The details are beyond this scope.)
    currentThread->registers = getAllRegisters(); // Store the registers in the "currentThread" object in memory.
    currentThread->registers.PC = resume; // Set the next PC to the "resume" label below in this method.

    readyQueue.enqueue(currentThread); // Put this thread back onto the ready queue for later execution.
    
    Thread* otherThread=readyQueue.dequeue(); // Remove and get the next thread to run from the ready queue.
    
    currentThread=otherThread; // Replace the global current-thread pointer value so it is ready for the next thread.

    // Restore the registers from currentThread/otherThread, including a jump to the stored PC of the other thread
    // (at "resume" below).  Again, the details of how this is done are beyond this scope.
    restoreRegisters(otherThread.registers);

    // *** Now running "otherThread" (which is now "currentThread")!  The original thread is now "sleeping". ***

    resume: // This is where another contextSwitch() call needs to set PC to when switching context back here.

    // Return to where otherThread left off.

    threadingSystemBusy=false; // Must be an atomic assignment.
    systemCall_enableInterrupts(); // Turn pre-emptive switching back on on this core.

}

// Thread sleep method:
// On current CPU core, a synchronous context switch to another thread without putting
// the current thread on the ready queue.
// Must be holding "threadingSystemBusy" and disabled interrupts so that this method
// doesn't get interrupted by the thread-switching timer which would call contextSwitchISR().
// After returning from this method, must clear "threadingSystemBusy".
public method threadSleep(){
    // Get all of the registers of the currently-running process.
    // For Program Counter (PC), we will need the instruction location of
    // the "resume" label below.  Getting the register values is platform-dependent and may involve
    // reading the current stack frame, JMP/CALL instructions, etc.  (The details are beyond this scope.)
    currentThread->registers = getAllRegisters(); // Store the registers in the "currentThread" object in memory.
    currentThread->registers.PC = resume; // Set the next PC to the "resume" label below in this method.

    // Unlike contextSwitchISR(), we will not place currentThread back into readyQueue.
    // Instead, it has already been placed onto a mutex's or condition variable's queue.
    
    Thread* otherThread=readyQueue.dequeue(); // Remove and get the next thread to run from the ready queue.
    
    currentThread=otherThread; // Replace the global current-thread pointer value so it is ready for the next thread.

    // Restore the registers from currentThread/otherThread, including a jump to the stored PC of the other thread
    // (at "resume" below).  Again, the details of how this is done are beyond this scope.
    restoreRegisters(otherThread.registers);

    // *** Now running "otherThread" (which is now "currentThread")!  The original thread is now "sleeping". ***

    resume: // This is where another contextSwitch() call needs to set PC to when switching context back here.

    // Return to where otherThread left off.
    
}
```





