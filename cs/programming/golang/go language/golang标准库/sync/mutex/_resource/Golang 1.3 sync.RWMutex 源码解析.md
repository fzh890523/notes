# Golang 1.3 sync.RWMutex 源码解析

Posted on [2014年10月20日](http://dev.cmcm.com/archives/228)  by  [毛, 剑](http://dev.cmcm.com/)		 | [暂无评论](http://dev.cmcm.com/archives/228#comments)		

在《[Golang 1.3 sync.Mutex 源码解析](http://dev.cmcm.com/archives/22)》中讲到了Mutex的实现，那么RWMutex对于读多写少的业务模型来说，可以提升业务的整体并发性，类似shared lock，读无锁，写有锁。简单来说就是读取操作无锁，但是写操作是一把互斥锁，当写操作进行时，其他任何锁争用者（读和写）都不能获取锁。
这sync.RWMutex（**golang 1.3版本**）涉及到的相关代码主要有：

> /usr/local/go/src/pkg/sync/rwmutex.go
>   /usr/local/go/src/pkg/sync/runtime.go
>   /usr/local/go/src/pkg/runtime/sema.goc

首先是rwmutex.go:

```Go
// An RWMutex is a reader/writer mutual exclusion lock.                            // The lock can be held by an arbitrary number of readers                          // or a single writer.                                                             // RWMutexes can be created as part of other                                       // structures; the zero value for a RWMutex is                                     // an unlocked mutex.                                                              
type RWMutex struct {                                                              
    w           Mutex  // held if there are pending writers                        
    writerSem   uint32 // semaphore for writers to wait for completing readers  
    readerSem   uint32 // semaphore for readers to wait for completing writers  
    readerCount int32  // number of pending readers                                
    readerWait  int32  // number of departing readers                              } 
```

我们看到RWMutex主要由w（Mutex对象）来提供互斥锁的，writerSem以及readerSem类似Mutex源码解析中提到的是用于信号量调度goroutine用的，另外就是readerCount和readerWait注释说是当前持有读锁的个数和当前等待读锁的个数（提供给写锁获取锁使用，因为写锁是互斥锁，读写都不兼容的锁）。

先看到RLock获取读锁：

```Go
// RLock locks rw for reading.                                                  
func (rw *RWMutex) RLock() {if raceenabled {                                                            
        _ = rw.w.state                                                          
        raceDisable()}if atomic.AddInt32(&rw.readerCount, 1) < 0 {// A writer is pending, wait for it.                                    
        runtime_Semacquire(&rw.readerSem)}if raceenabled {                                                            
        raceEnable()                                                            
        raceAcquire(unsafe.Pointer(&rw.readerSem))}}
```

raceenabled代码忽略（如果你认真看过我的Mutex源码解析），我们发现代码只有一个原子指令操作，非常的高效，意思就是把当前持有锁的人加一，如果小于零说明写锁正被持有，因此进入等待runtime_Semacquire(&rw.readerSem)，如果>0那么持有锁成功。那么疑问来了，为什么小于零表示写锁被持有呢？

请看Lock写锁代码：

```Go
// Lock locks rw for writing.                                                   // If the lock is already locked for reading or writing,                        // Lock blocks until the lock is available.                                     // To ensure that the lock eventually becomes available,                        // a blocked Lock call excludes new readers from acquiring                      // the lock.                                                                    
func (rw *RWMutex) Lock() {if raceenabled {                                                            
        _ = rw.w.state                                                          
        raceDisable()}// First, resolve competition with other writers. // 先使用一把互斥锁，保证没有其他的写锁争用者            
    rw.w.Lock()// Announce to readers there is a pending writer.     // 核心来了，首先用原子指令把readerCount变成绝对的负数，因为rwmutexMaxReaders ＝ 1<<30// 但是为什么作者又把结果加上rwmutexMaxReaders，其实这是一种推断做法，意思就是如果当前// 可能存在读锁刚好被持有，那么r这时候就不等于0了（持读锁的话会变成正数，释放读锁会变成负数）                    
    r := atomic.AddInt32(&rw.readerCount, -rwmutexMaxReaders) + rwmutexMaxReaders
    // Wait for active readers.// 如果r == 0，表示没有任何读锁持有者，直接成功// 如果r != 0，表示这时候读写锁存在竞赛，需要使用信号量来等待goroutine唤醒// 而当r == 0，但是如果当前readerWait等待读锁的个数（持有锁的人 + 释放锁的） != 0 ，那么表示还有读写争用，需要使用信号量等待其他读锁持有者，进行release// 因此还加了这行判断 atomic.AddInt32(&rw.readerWait, r) != 0                                         if r != 0 && atomic.AddInt32(&rw.readerWait, r) != 0 {                      
        runtime_Semacquire(&rw.writerSem)}if raceenabled {                                                            
        raceEnable()                                                            
        raceAcquire(unsafe.Pointer(&rw.readerSem))                              
        raceAcquire(unsafe.Pointer(&rw.writerSem))}}
```

atomic.AddInt32(&rw.readerWait, r) != 0的意义在于，这个时候RUnlock是可以被调用的！看完上面我的注释，可能比较晕，当看完读锁的释放代码就明白了：

```Go
// RUnlock undoes a single RLock call;                                          // it does not affect other simultaneous readers.                               // It is a run-time error if rw is not locked for reading                       // on entry to RUnlock.                                                         
func (rw *RWMutex) RUnlock() {if raceenabled {                                                            
        _ = rw.w.state                                                          
        raceReleaseMerge(unsafe.Pointer(&rw.writerSem))                         
        raceDisable()}// 首先把读锁持有者计数-1，如果>0表示，锁释放成功，直接返回// 当写锁被持有的时候，我们回看一下代码：// r := atomic.AddInt32(&rw.readerCount, -rwmutexMaxReaders) + rwmutexMaxReaders// 所以可能存在负数的情况，那么这个时候是存在读写争用的需要使用信号量来处理                                                                          if atomic.AddInt32(&rw.readerCount, -1) < 0 {// A writer is pending.// 把读锁等待个数-1，如果等于0，那么所有等待都释放了，因此信号量唤醒写锁的goroutine                                                 if atomic.AddInt32(&rw.readerWait, -1) == 0 {// The last reader unblocks the writer.                             
            runtime_Semrelease(&rw.writerSem)}}if raceenabled {                                                            
        raceEnable()}}
```

最后我们再看到写锁的释放就简单了：

```Go
// Unlock unlocks rw for writing.  It is a run-time error if rw is              // not locked for writing on entry to Unlock.                                   //                                                                              // As with Mutexes, a locked RWMutex is not associated with a particular        // goroutine.  One goroutine may RLock (Lock) an RWMutex and then               // arrange for another goroutine to RUnlock (Unlock) it.                        
func (rw *RWMutex) Unlock() {if raceenabled {                                                            
        _ = rw.w.state                                                          
        raceRelease(unsafe.Pointer(&rw.readerSem))                              
        raceRelease(unsafe.Pointer(&rw.writerSem))                              
        raceDisable()}// Announce to readers there is no active writer.// 先把计数还原成正数或者0，表示写锁被释放，其他读锁争用者，可以进入了！                           
    r := atomic.AddInt32(&rw.readerCount, rwmutexMaxReaders)// Unblock blocked readers, if any.// 如果还剩余reader，全部唤醒。我们看到RLock在XADD指令失败时候的代码：// runtime_Semacquire(&rw.readerSem)，因此RLock全部需要信号量来唤醒                                       for i := 0; i < int(r); i++ {                                               
        runtime_Semrelease(&rw.readerSem)}// Allow other writers to proceed.  // 让写锁争用者可以进来啦！                                        
    rw.w.Unlock()if raceenabled {                                                            
        raceEnable()}} 
```

关于读写锁的实现就到此结束，多谢各位纠正和给出意见！

Posted in [Golang](http://dev.cmcm.com/archives/category/golang).| Tagged [Golang](http://dev.cmcm.com/archives/tag/golang), [rwlock](http://dev.cmcm.com/archives/tag/rwlock).		| 288 views

来源： <<http://dev.cmcm.com/archives/228>>