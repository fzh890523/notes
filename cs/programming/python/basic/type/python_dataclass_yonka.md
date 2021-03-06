

```python
import dataclasses

@dataclasses.dataclass
class A:
    a: int = None
        
@dataclasses.dataclass
class B:
    b: int = None  # 一样也是需要有默认值才能免去每个传入
        
dataclasses.asdict(B(a=1))  # {'a': 1, 'b': None}

@dataclasses.dataclass
class C:
    def __init__(self):  # 问题是： 加了init，pycharm就无法正确的做param info的提示了
        self.a = 10
        
C()  # C(a=10, b=None)        
```



## 默认值

如上，对于没有默认值的field，必须在调用构造器时传入。

此外，**因为众所周知的坑，mutable类型不允许直接给出默认值**，如：

```python
@dataclass
class A:
    l: List =[]
# 会报错： ValueError: mutable default <class 'list'> for field l is not allowed: use default_factory

# 需要这样写，用factory模式来解决这个问题：
from dataclasses import field
@dataclass
class A:
    l: List = field(default_factory=lambda: [])
        
In [73]: A()
Out[73]: A(l=[])

In [74]: a1 = A()
In [76]: a1.l.append(1)

In [77]: a1
Out[77]: A(l=[1])

In [79]: a2
Out[79]: A(l=[])        
```





## init=True

默认会给类生成完整`__init__`，可以通过设为`False`来改变该行为



## class var

```python
from typing import ClassVar

@dataclass
class A:
    a: int = None
    values: ClassVar[List[int]] = []
```





## 其他格式转换



### to_dict支持： dataclasses.asdict

支持嵌套，如：

```python
In [13]: @dataclass
    ...: class A:
    ...:     a: int = 0
    ...:
In [15]: @dataclass
    ...: class B(A):
    ...:     b: int = 0
    ...:
In [18]: @dataclass
    ...: class C:
    ...:     bb: B = None
    ...:     c: int = 0     
            
In [22]: asdict(C(bb=B()))
Out[22]: {'bb': {'a': 0, 'b': 0}, 'c': 0}            
```



不过，如果有非dataclass的类的实例，会保留原实例，如：

```python
In [24]: @dataclass
    ...: class C:
    ...:     bb: B = None
    ...:     c: int = 0
    ...:     d: D = None
    ...:
    ...:

In [25]: asdict(C(bb=B()))
Out[25]: {'bb': {'a': 0, 'b': 0}, 'c': 0, 'd': None}

In [26]: asdict(C(bb=B(), d=D()))
Out[26]: {'bb': {'a': 0, 'b': 0}, 'c': 0, 'd': <__main__.D at 0x7f5ed04d74f0>}
```





### from_dict 支持： 第三方

ref： https://stackoverflow.com/questions/53376099/python-dataclass-from-a-nested-dict



* 单独实现

  ```python
  from dataclasses import dataclass, fields, is_dataclass
  
  # thanks to Patrick Haugh
  from typing import *
  
  # deepcopy 
  import copy
  
  
  class TypeDict(dict):
      def __init__(self, t, *args, **kwargs):
          super(TypeDict, self).__init__(*args, **kwargs)
  
          if not isinstance(t, type):
              raise TypeError("t must be a type")
  
          self._type = t
  
      @property
      def type(self):
          return self._type
      
  
  # copy of the internal function _is_dataclass_instance
  def is_dataclass_instance(obj):
      return is_dataclass(obj) and not is_dataclass(obj.type)
  
  # the adapted version of asdict
  def todict(obj):
      if not is_dataclass_instance(obj):
           raise TypeError("todict() should be called on dataclass instances")
      return _todict_inner(obj)    
  
  
  def _fromdict_inner(obj):
      # reconstruct the dataclass using the type tag
      if is_dataclass_dict(obj):
          result = {}
          for name, data in obj.items():
              result[name] = _fromdict_inner(data)
          return obj.type(**result)
  
      # exactly the same as before (without the tuple clause)
      elif isinstance(obj, (list, tuple)):
          return type(obj)(_fromdict_inner(v) for v in obj)
      elif isinstance(obj, dict):
          return type(obj)((_fromdict_inner(k), _fromdict_inner(v))
                           for k, v in obj.items())
      else:
          return copy.deepcopy(obj)   
      
      
  def is_dataclass_dict(obj):
      return isinstance(obj, TypeDict)
  
  def fromdict(obj):
      if not is_dataclass_dict(obj):
          raise TypeError("fromdict() should be called on TypeDict instances")
      return _fromdict_inner(obj)    
  ```

  

* [`dacite`](https://github.com/konradhalas/dacite)

  ```python
  from dataclasses import dataclass
  from dacite import from_dict
  
  @dataclass
  class User:
      name: str
      age: int
      is_active: bool
  
  data = {
      'name': 'john',
      'age': 30,
      'is_active': True,
  }
  
  user = from_dict(data_class=User, data=data)
  ```

  