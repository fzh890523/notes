



# map



## key要求： hashable



如惯例，py的key要求hashable，list、普通对象等都不行。



# enum



## items



### 获取所有的值



```python
for a in Action:
    pass

for a in list(Action):
    pass

[a.name for a in Action]
```

