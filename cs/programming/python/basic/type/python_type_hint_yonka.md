

### *args **kwargs

只需要对元素做声明即可，类似：

```python
def a(*args: int, **kwargs: str):
    pass
```



### 函数签名、lambda

ref： https://stackoverflow.com/questions/33833881/is-it-possible-to-type-hint-a-lambda-function

You can, sort of, in Python 3.6 and up using [PEP 526 variable annotations](https://www.python.org/dev/peps/pep-0526/). You can annotate the variable you assign the `lambda` result to with the [`typing.Callable` generic](https://docs.python.org/3/library/typing.html#typing.Callable):

```python
from typing import Callable

func: Callable[[str, str], int] = lambda var1, var2: var1.index(var2)
```

不过这样，在写lambda内容时还是不能hint，但能满足func使用处的hint。



### file-like object: `TextIO`等











