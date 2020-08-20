





### build with debug info




```sh
-gcflags "all=-N -l"
# idea上还有bug，提示里错误写为： -gcflags='all -N -l'
```


```sh
# ~/.go/bin/dlv
${dlvBinaryPath} \
--listen=0.0.0.0:57584 \
--headless=true \
--api-version=2 \
--backend=default \
exec ${binaryPath} \
-- \
${args}

# go get -u github.com/derekparker/delve/cmd/dlv
```





### print stacktrace



* 开了debug接口另说

* 没开的话： `kill -SIGQUIT ${go_pid}`。 会输出在标准（错误？）输出里。



