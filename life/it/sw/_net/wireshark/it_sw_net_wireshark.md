



# Tutorial





### column



#### interface

* 在包上选择`Frame`的`Interface Id`然后应用为列，即可。
* 添加类型为Custom的列，字段内容为`frame.interface_id`







### filter



#### tcp data containers



```sh
tcp.port == 80 && !(tcp.segment_data contains "HeartBeat")
tcp.port == 80 && not (tcp.segment_data contains "HeartBeat")

tcp.port == 80 && not (tcp.payload contains "HeartBeat")
```









## cases



### capture on multiple interfaces



捕获-选项 (mac上cmd+k) 里可以通过 cmd+点击 的方式进行复选（这个交互真的。。。）




