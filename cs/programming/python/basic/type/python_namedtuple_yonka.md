



## 一些问题



###  添加字段特性只支持直接子类

如下：

> 至少在3.8.5是这样。 经测试，TypedDict没问题。 但是，起码2020.02版本的pycharm不支持对TypedDict的field的“理解” --- 参见 [TypedDict arguments for functions don't present keys for autocomplete](https://youtrack.jetbrains.com/issue/PY-39703)，可以看到`202.7109`版本里支持了 --- 但只支持key访问而不是field访问，而且只支持一级... 勉强能用，差评

```python
class A(NamedTuple):
    a: int
        
class B(A):
    b: int
        
A(a=1)  # ok
B(a=1)  # ok
B(b=2)  # TypeError: __new__() got an unexpected keyword argument 'b'
```



详见： [Two issues with subclassing namedtuple-generated class and overriding __new__ #1279](https://github.com/python/mypy/issues/1279) 和 [NamedTuple subclasses do not pick up __new__'s signature #5194](https://github.com/python/mypy/issues/5194)



[options.py](_code/options.py) 里通过metaclass修复了该问题并且支持了多继承。



### 定义的字段必须全部传

否则会类似报错： `TypeError: __new__() missing 1 required positional argument: 'a'`



这个应该也能通过metaclass解，但再改下去麻烦，而且依赖比较重。

弃疗。。。 打算用新版pycharm+typedDict



