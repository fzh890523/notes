

# 概念



## 半开、半闭



ref： [detection-of-half-open-dropped](https://blog.stephencleary.com/2009/05/detection-of-half-open-dropped.html)



两个截然不同的概念。

* 半开

  ```
  This leads to a scenario known as a “half-open connection”. At any given point in most protocols, one side is expected to send a message and the other side is expecting to receive it. Consider what happens if an intermediate router is suddenly rebooted at that point: the receiving side will continue waiting for the message to arrive; the sending side will send its data, and receive an error indicating the connection was lost. Since broken connections can only be detected by sending data, the receiving side will wait forever. This scenario is called a “half-open connection” because one side realizes the connection was lost but the other side believes it is still active.
  ```

* 半闭

  ```
  “half-open” is completely different than “half-closed”. Half-closed connections are when one side performs a Shutdown operation on its socket, shutting down only the sending (outgoing) stream. 
  ```

  ​







# 流程



## 正常流程



* 正常流程客户端TCP状态迁移：

  * client发起连接
  * client主动关闭连接

  `CLOSED->SYN_SENT->ESTABLISHED->FIN_WAIT_1->FIN_WAIT_2->TIME_WAIT->CLOSED`

  * CLOSED

    -SYN

    * SYNC_SENT

      +SYN-ACK -ACK

      * ESTABLISHED

        -FIN

        * FIN_WAIT_1

          +ACK

          * FIN_WAIT_2

            +FIN -ACK

            * TIME_WAIT
              * CLOSED

          +FIN -ACK

          * CLOSING

            +ACK

            * TIME_WAIT
              * CLOSED

* 正常流程服务器TCP状态迁移：

  * 服务端接受连接（被动建立）
  * 服务端被动关闭连接

  `CLOSED->LISTEN->SYN_RCVD->ESTABLISHED->CLOSE_WAIT->LAST_ACK->CLOSED`

  * CLOSED

    do listen

    * LISTEN

      +SYN -SYN-ACK

      * SYN_RCVD

        +ACK

        * ESTABLISHED

          +FIN -ACK

          * CLOSE_WAIT

            -FIN

            * LAST_ACK

              +ACK

              > 没收到ACK然后超时？ 重传FIN？

              * CLOSED





## 异常流程





# 状态



## 特殊状态



- LAST-ACK

  应该比较少见，毕竟ACK是协议栈完成

  如果多的话，client不回最后一个ACK，可能是恶意或者协议实现缺陷

- FIN-WAIT-1

  如果多的话，从client端上看，可能server hang住，因为ACK都回不了 = = （FIN会延迟ACK吗）

- FIN-WAIT-2

  如果多的话，从client端上看，可能server端应用hang住，无法close

  对应server端应有较多CLOSE-WAIT

- CLOSE-WAIT

  多的话，从server端上看，server端应用可能hang住无法close

  对应...

- TIME-WAIT

- CLOSING

  这个状态应该比较少见，server端都能回FIN说明更能回ACK

  瞬态的话可能是ACK丢包； 比较多的话可能是协议栈实现缺陷



## 状态迁移



![](../resource/[原创]结合案例深入解析orphan socket产生与消亡（一）-博客-云栖社区-阿里云_files/10e3ea2de912329ffa09fb478e0d7a63.png)



从图里可以看到一点：

* 如果应用层一直不close的话，会产生/积累 CLOSE_WAIT 状态的连接

  框架是不是可以尝试帮忙做类似的收尾事情？



