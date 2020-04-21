

### 内存页大小： `getconf PAGESIZE`

输出类似：

```shell
4096
```



单位为B，也即这里为 `4KB`





## 内存统计分类



* `(total)VM`： the size of the virtual memory that a process uses

  if you allocate a lot of space (with malloc()), but nevers use it, the total-vm would be higher, but no real memory would be used (due to the memory overcommit), so, the rss values would be low

  * `RSS`: really mapped into the RAM itself (allocated and used)

    * `anon-rss`: allocated in real memory blocks (other than mapped into a file or device)

      if you malloc() a lot of memory and really use it, your anon-rss would be high also

    * `file-rss`: memory blocks that are mapped into devices and files

      if you open a huge file in vim, the file-rss would be high

  

