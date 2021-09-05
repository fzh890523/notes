

# argparse



ref:

* [[Python命令行参数解析模块argparse](https://www.cnblogs.com/cheyunhua/p/11002421.html)](https://www.cnblogs.com/cheyunhua/p/11002421.html)



### 创建解析器 `ArgumentParser` 

使用 `argparse` 的第一步是创建一个 `ArgumentParser` 对象，如下：

```
import argparse

parser = argparse.ArgumentParser(prog=None, usage=None,description=None, 
epilog=None, parents=[], formatter_class=argparse.HelpFormatter, prefix_chars='-', fromfile_prefix_chars=None, 
argument_default=None, conflict_handler='error', add_help=True, allow_abbrev=True)
```

创建一个新的 `ArgumentParser` 对象。所有的参数都应当作为关键字参数传入。部分参数在下面都有它更详细的描述，但简而言之，它们是：

- `prog` - 程序的名称（默认：sys.argv[0]）
- `usage` - 描述程序用途的字符串（默认值：从添加到解析器的参数生成）
- `description` - 在参数帮助文档之前显示的文本（默认值：无）
- `epilog` - 在参数帮助文档之后显示的文本（默认值：无）
- `parents` - 一个 ArgumentParser 对象的列表，它们的参数也应包含在内
- `formatter_class` - 用于自定义帮助文档输出格式的类
- `prefix_chars` - 可选参数的前缀字符集合（默认值：'-'）
- `fromfile_prefix_chars` - 当需要从文件中读取其他参数时，用于标识文件名的前缀字符集合（默认值：None）
- `argument_default` - 参数的全局默认值（默认值： None）
- `conflict_handler` - 解决冲突选项的策略（通常是不必要的）
- `add_help` - 为解析器添加一个 -h/--help 选项（默认值： True）
- `allow_abbrev` - 如果缩写是无歧义的，则允许缩写长选项 （默认值：True）



### 添加参数 `add_argument`

使用 `add_argument()` 方法添加参数。其中，部分参数在下面都有它更详细的描述，但简而言之，它们是：

- `name or flags` - 一个命名或者一个选项字符串的列表，例如 foo 或 -f, --foo。
- `action` - 当参数在命令行中出现时使用的动作基本类型。
- `nargs` - 命令行参数应当消耗的数目。
- `const` - 被一些 action 和 nargs 选择所需求的常数。
- `default` - 当参数未在命令行中出现时使用的值。
- `type` - 命令行参数应当被转换成的类型。
- `choices` - 可用的参数的容器。
- `required` - 此命令行选项是否可省略 （仅选项可用）。
- `help` - 一个此选项作用的简单描述。
- `metavar` - 在使用方法消息中使用的参数值示例。
- `dest` - 被添加到 parse_args() 所返回对象上的属性名。

#### name or flags

第一个传递给 `add_argument()` 的参数必须是一系列 flags 或者是一个简单的参数名，如下：

```
>>> parser.add_argument('-f', '--foo')

or

>>> parser.add_argument('bar')
```

#### action

- `store` - 存储参数的值。这是默认的动作。如下:

```
>>> parser = argparse.ArgumentParser()
>>> parser.add_argument('--foo')
>>> parser.parse_args('--foo 1'.split())
Namespace(foo='1')
```

- `store_const` - 存储被 const 命名参数指定的值。 `store_const` 动作通常用在选项中来指定一些标志。如下：

```
>>> parser = argparse.ArgumentParser()
>>> parser.add_argument('--foo', action='store_const', const=42)
>>> parser.parse_args(['--foo'])
Namespace(foo=42)
```

- `store_true` 和 `store_false` - 这些是 `store_const` 分别用作存储 `True` 和 `False` 值的特殊用例。另外，它们的默认值分别为 `False` 和 `True`。如下：

```
>>> parser = argparse.ArgumentParser()
>>> parser.add_argument('--foo', action='store_true')
>>> parser.add_argument('--bar', action='store_false')
>>> parser.add_argument('--baz', action='store_false')
>>> parser.parse_args('--foo --bar'.split())
Namespace(foo=True, bar=False, baz=True)
```

- `append` - 存储一个列表，并且将每个参数值追加到列表中。在允许多次使用选项时很有用。如下：

```
>>> parser = argparse.ArgumentParser()
>>> parser.add_argument('--foo', action='append')
>>> parser.parse_args('--foo 1 --foo 2'.split())
Namespace(foo=['1', '2'])
```

- `count` - 计算一个关键字参数出现的数目或次数。例如，对于一个增长的详情等级来说有用。如下：

```
>>> parser = argparse.ArgumentParser()
>>> parser.add_argument('--verbose', '-v', action='count')
>>> parser.parse_args(['-vvv'])
Namespace(verbose=3)
```

- `version` - 期望有一个 `version=` 命名参数在 `add_argument()` 调用中，并打印版本信息并在调用后退出。如下：

```
>>> import argparse
>>> parser = argparse.ArgumentParser(prog='PROG')
>>> parser.add_argument('--version', action='version', version='%(prog)s 2.0')
>>> parser.parse_args(['--version'])
PROG 2.0
```



#### 关键字参数名： dest

类似：

```python
parser.add_argument('--from', dest='from_xx')
```

否则`parser.from`肯定会报错，而parser又没提供`['from']`的访问接口。

可以：`parser.from_xx`



### 一些样例

```python
import argparse

parser = argparse.ArgumentParser(description='Process some integers.')
parser.add_argument('integers', metavar='N', type=int, nargs='+',
                    help='an integer for the accumulator')
parser.add_argument('--sum', dest='accumulate', action='store_const',
                    const=sum, default=max,
                    help='sum the integers (default: find the max)')

args = parser.parse_args()
print(args.accumulate(args.integers))
```