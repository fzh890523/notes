
ref:

http://rocketmq.apache.org/docs/quick-start/

* jdk版本： 似乎要求1.8 （至少1.11开始是不行的）

  看报错是后面版本jvm选项有变化导致起不来

* 测试环境可能因`-Xms`等参数过大而无法启动

  nameserver和broker的，分别修改即可


简单的操作：

* 启动nameserver

  `nohup sh bin/mqnamesrv &`

* 关闭nameserver

  `sh bin/mqshutdown namesrv`

* 启动brokder

  `nohup sh bin/mqbroker -n localhost:9876 &`

* 关闭broker

  `sh bin/mqshutdown broker`