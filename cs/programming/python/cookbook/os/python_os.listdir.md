
```python
Signature: os.listdir(path=None)
Docstring:
Return a list containing the names of the files in the directory.

path can be specified as either str, bytes, or a path-like object.  If path is bytes,
  the filenames returned will also be bytes; in all other circumstances
  the filenames returned will be str.
If path is None, uses the path='.'.
On some platforms, path may also be specified as an open file descriptor;\
  the file descriptor must refer to a directory.
  If this functionality is unavailable, using it raises NotImplementedError.

The list is in arbitrary order.  It does not include the special
entries '.' and '..' even if they are present in the directory.
Type:      builtin_function_or_method
```

* 估计默认（传入str类型path时）用系统默认编码去解码，返回 `[]str`
* 传入bytes类型path时，返回 `[]bytes`
  如： `os.listdir(b".")`
  可用于解决一些场景： 比如默认编码为utf-8而该文件名实际是gbk的，那么就会显示为 `gbk -> utf-8` 的乱码，而这个乱码基本不可能反解