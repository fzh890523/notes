



```sh
# goland用的测试方式

# -c 先编译出binary，不执行
go test -c -o ${output_file} ${full_package} #gosetup
# 实际执行
go tool test2json -t ${output_file} -test.v -test.run ^${test_case_func_name}$ #gosetup

# 如：
go test -c -mod=vendor -o /tmp/xxx.exe istio.io/istio/pilot/pkg/proxy/envoy/v2
go tool test2json -t /tmp/xxx.exe -test.v -test.run ^\QTestAdsPushScoping\E$
# 这个 \Q \E 似乎多余
go tool test2json -t /tmp/xxx.exe -test.v -test.run ^TestAdsPushScoping$
```





执行 `go test -x -cover -coverprofile=./size.out` 命令，运行测试并统计测试覆盖率。其中，`-x` 参数打印上述命令的执行过程（需注意：打印的执行步骤信息不完整，如果手动执行输出的步骤，则会运行失败，这是因为 go test 的一些执行步骤并没有打印信息），`-cover` 参数开启测试覆盖率统计功能，`-coverprofile` 参数指定存储测试覆盖率文件



