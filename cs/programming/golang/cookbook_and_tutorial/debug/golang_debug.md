




```sh
-gcflags "all=-N -l"
# idea上还有bug，提示里错误写为： -gcflags='all -N -l'
```


```sh
~/.go/bin/dlv \
--listen=0.0.0.0:57584 \
--headless=true \
--api-version=2 \
--backend=default \
exec ${binaryPath} \
-- \
${args}
```
