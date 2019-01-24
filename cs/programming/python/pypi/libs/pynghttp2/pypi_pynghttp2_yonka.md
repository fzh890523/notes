

* 不支持mac

  ```sh
  /usr/local/Cellar/python3/3.6.1/Frameworks/Python.framework/Versions/3.6/lib/python3.6/ctypes/__init__.py in __init__(self, name, mode, handle, use_errno, use_last_error)
      346
      347         if handle is None:
  --> 348             self._handle = _dlopen(self._name, mode)
      349         else:
      350             self._handle = handle
  
  OSError: dlopen(libnghttp2.so, 6): image not found
  ```

* 不支持py2

  ```python
  import pynghttp2
    File "/usr/local/lib/python2.7/dist-packages/pynghttp2/sessions.py", line 277
      async def wait_for_window_update(self, stream_id):
              ^
  SyntaxError: invalid syntax
  ```

* 好像不支持低版本的py3(3.6以下？)

  ```python
  /usr/local/lib/python3.5/dist-packages/pynghttp2/typedefs.py in <module>
      253
      254
  --> 255 class data_flag(enum.IntFlag):
      256     """The flags used to set in data_flags output parameter in
      257     data_source_read_callback.
  
  AttributeError: module 'enum' has no attribute 'IntFlag'
  ```

* 对nghttp2的版本好像有要求

  ```sh
  /usr/lib/python3.6/ctypes/__init__.py in __getitem__(self, name_or_ordinal)
      364
      365     def __getitem__(self, name_or_ordinal):
  --> 366         func = self._FuncPtr((name_or_ordinal, self))
      367         if not isinstance(name_or_ordinal, int):
      368             func.__name__ = name_or_ordinal
  
  AttributeError: /usr/lib/x86_64-linux-gnu/libnghttp2.so: undefined symbol: nghttp2_option_set_builtin_recv_extension_type
  ```




