

## python -c




```sh
python -c 'import foo; print foo.hello()'
# 注意，如果存在相对import的话，会报错。这时候： cd .. && 
python -c 'import parent.foo; print parent.foo.hello()'
```



## python -m



