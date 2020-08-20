## JVM源码分析之Attach机制实现完全解读 - ref ATA

### 背景

#### 线程

- Attach Listener
  - 顾名思义是用来…的
  - AttachListener::init()中创建，但启动时默认不会执行这个，也即默认该线程不启动
- Signal Dispatcher
  - 启动时启动
  - 默认jvm各线程会屏蔽SIGBREAK（define为SIGQUIT），只有此线程会处理
  - 处理方式是：

### attach过程中做了什么

1. 尝试找socket file，没找到的话看1.x
   1. 创建attach文件（<del>/proc//cwd/.Attach_pid</del>，应该是文中表述错误，从下问看这里应该是进程当前目录下的**.Attach_pid${pid}**文件），似乎是做锁用，看下文
   2. 给signal dispatcher线程发信号，但jvm进程可能有很多线程，所以采用“饱和攻击”法，给每个线程发送（反正只有...处理）
   3. 等待一段时间直到找到socket文件或者超时
   4. 删除attach文件
2. 确认socket文件权限
3. 然后尝试连接到该socket

### jvm进程对每一步的响应

1. 对于信号，如果是触发启动attach listener的话则… 否则执行默认行为： 输出栈

   判断的依据是：

   1. 如果已经启动了，自然不是，return false
   2. 查看进程当前目录下是否有 **.Attach_pid${pid}**，没有的话则查看系统temp（一般为/tmp）下是否有该文件，没有则return false
   3. 如果2中有该文件，则判断uid是否与jvm进程euid同，是则做init然后return true

2. 如前文提到的，init里会创建attach listener线程

3. 该线程init方法里会创建socket文件并且listen，文件为： `${系统temp}/.java_pid${pid}.tmp`，然后设置权限… 最后rename为去掉tmp后缀的...

4. 之后就在该socket上接受请求（AttachOperation）、处理、响应。这里没有用…模型，所以是读完一个请求、处理、响应后才会读下一个请求

