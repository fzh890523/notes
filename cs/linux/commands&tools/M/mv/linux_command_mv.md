

* `mv`不会merge目录

  > mv cannot merge or overwrite directories, it will fail with the message "mv: cannot move 'a' to 'b': Directory not empty", even when you're using the --force option.

  可以用 `rsync`, `find`+`args`等方案



