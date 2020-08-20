Ref: https://github.com/golang/mock





# tutorial



## install



```sh
# install go

go get github.com/golang/mock/gomock
go install github.com/golang/mock/mockgen
```





## usage



1. 为需要mock的接口生成实现： mockgen

   因为go不支持动态生成。。 = =







## mockgen

* 通过指定文件读取接口

  ```sh
  ./mockgen -source=./pkg/client/xx.go -destination=./pkg/client/xx_mock_test.go -package=client TestInterface1,TestInterface2
  
  # -source 指定输入文件
  # -destination 指定输出文件，默认stdout
  # -package 指明生成代码的包名，默认 mock_${原来包名}
  ```




