



* umask = ~fileMode

  ```sh
  stat -c "%a" ${file}  # 可以获取到fileMode，如 644。 对应umask为 777-644=133
  ```

  不过还没找到直接根据fileMode算出对应的umask的方式

* 注意，umask中的0和1是等效，因为linux不允许create的文件带`x`位，只能后面再`chmod +x`

  也即`644`对应`133`也对应`022`。

* 有些程序因为实现原因或者其他规范导致不遵循umask，比如`mktemp`，得到的文件是`600`



