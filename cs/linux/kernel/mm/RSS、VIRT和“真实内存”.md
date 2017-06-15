

# RSS“不准”问题

* 常见RSS = private mem + shared mem，该计算方式存在问题，应该把shared部分除以进程数


* fork-COW机制带来一些奇怪的结果，尤其是使用了shm之后

