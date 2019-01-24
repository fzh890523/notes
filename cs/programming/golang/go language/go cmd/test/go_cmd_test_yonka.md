



```sh
go test -c -o ${output_file} ${full_package} #gosetup
go tool test2json -t ${output_file} -test.v -test.run ^${test_case_func_name}$ #gosetup
```





