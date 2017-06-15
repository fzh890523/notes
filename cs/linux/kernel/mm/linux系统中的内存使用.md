

# Java进程内存

* 应用程序角度
  * 堆内存
  * 栈上内存
  * 堆外内存
    * off-heap allocation
    * JNI
* JVM-MM
  * 虚拟机栈
  * 本地方法栈
  * PC寄存器
  * 方法区（hotspot PermGen）
  * 堆
* JVM（自身开销）
  * GC算法追踪对象
  * 统计数据（JIT）
  * ...



# 进程内存

* 进程内存
  * TEXT segment
  * DATA segment
  * shared lib
  * mmap files
  * shared memory
  * malloc库预分配/未及时归还的内存
  * …
* 进程用到的内核空间内存
  * page cache（cache）
  * block cache（buf）
  * kernel stack
  * task_struct



# 内核使用内存

* 内核
  * page tables
  * slab
  * …





