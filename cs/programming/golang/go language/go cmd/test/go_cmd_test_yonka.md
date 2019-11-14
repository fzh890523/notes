



```sh
go test -c -o ${output_file} ${full_package} #gosetup
go tool test2json -t ${output_file} -test.v -test.run ^${test_case_func_name}$ #gosetup
```





执行 `go test -x -cover -coverprofile=./size.out` 命令，运行测试并统计测试覆盖率。其中，`-x` 参数打印上述命令的执行过程（需注意：打印的执行步骤信息不完整，如果手动执行输出的步骤，则会运行失败，这是因为 go test 的一些执行步骤并没有打印信息），`-cover` 参数开启测试覆盖率统计功能，`-coverprofile` 参数指定存储测试覆盖率文件



