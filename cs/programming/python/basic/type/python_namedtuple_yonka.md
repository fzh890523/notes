



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



尝试单独指定metaclass方式，也不行：

```python
In [38]: class NTB1(NTA, metaclass=NamedTupleMeta):
    ...:     b: int = None
    ...: 

In [39]: NTB1()
Out[39]: NTB1(b=None)

In [40]: NTB1().a
---------------------------------------------------------------------------
AttributeError                            Traceback (most recent call last)
<ipython-input-40-b95d0874ea24> in <module>
----> 1 NTB1().a

AttributeError: 'NTB1' object has no attribute 'a'
```





### 定义的字段必须全部传 - 不是，配好默认值即可

否则会类似报错： `TypeError: __new__() missing 1 required positional argument: 'a'`



这个应该也能通过metaclass解，但再改下去麻烦，而且依赖比较重。

弃疗。。。 <del>打算用新版pycharm+typedDict</del>

> typeddict实际就是dict，子类定义的方法无效。 ide warn，define运行不报错，access时报错： `AttributeError: 'dict' object has no attribute 't'`



**更新** 配好默认值即可

```python
In [27]: import options

In [28]: class OA(options.Options):
    ...:     a: int = None  # 这个默认值没有才会报上面的错
    ...: 

In [29]: oa = OA()

In [30]: oa._asdict()
Out[30]: {'a': None}

In [31]: class OB(OA):
    ...:     b: int = None
    ...: 

In [32]: ob = OB()

In [33]: ob
Out[33]: OB(a=None, b=None)

In [34]: ob._asdict()
Out[34]: {'a': None, 'b': None}
```



## 关于更好的作为pojo的结论

* 原生NamedTuple有一些局限，不能多级子类等，可以用上面提到的增强的版本： [options.py](_code/options.py)

* 用dataclasses.dataclass，参见 [python_dataclass_yonka.md](python_dataclass_yonka.md)

  这个方案好像唯一不方便的就是每个子类都需要注解，比较啰嗦且有侵入性（不好替换实现）