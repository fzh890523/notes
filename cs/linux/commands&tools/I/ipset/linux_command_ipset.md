

```sh
ipset help
ipset v7.3

Usage: ipset [options] COMMAND

Commands:
create SETNAME TYPENAME [type-specific-options]
        Create a new set
add SETNAME ENTRY
        Add entry to the named set
del SETNAME ENTRY
        Delete entry from the named set
test SETNAME ENTRY
        Test entry in the named set
destroy [SETNAME]
        Destroy a named set or all sets
list [SETNAME]
        List the entries of a named set or all sets
save [SETNAME]
        Save the named set or all sets to stdout
restore
        Restore a saved state
flush [SETNAME]
        Flush a named set or all sets
rename FROM-SETNAME TO-SETNAME
        Rename two sets
swap FROM-SETNAME TO-SETNAME
        Swap the contect of two existing sets
help [TYPENAME]
        Print help, and settype specific help
version
        Print version information
quit
        Quit interactive mode

Options:
-o plain|save|xml
       Specify output mode for listing sets.
       Default value for "list" command is mode "plain"
       and for "save" command is mode "save".
-s
        Print elements sorted (if supported by the set type).
-q
        Suppress any notice or warning message.
-r
        Try to resolve IP addresses in the output (slow!)
-!
        Ignore errors when creating or adding sets or
        elements that do exist or when deleting elements
        that don't exist.
-n
        When listing, just list setnames from the kernel.

-t
        When listing, list setnames and set headers
        from kernel only.
-f
        Read from the given file instead of standard
        input (restore) or write to given file instead
        of standard output (list/save).

Supported set types:
    list:set            3       skbinfo support
    list:set            2       comment support
    list:set            1       counters support
    list:set            0       Initial revision
    hash:mac            0       Initial revision
    hash:ip,mac         0       Initial revision
    hash:net,iface      6       skbinfo support
    hash:net,iface      5       forceadd support
    hash:net,iface      4       comment support
    hash:net,iface      3       counters support
    hash:net,iface      2       /0 network support
    hash:net,iface      1       nomatch flag support
    hash:net,iface      0       Initial revision
    hash:net,port       7       skbinfo support
    hash:net,port       6       forceadd support
    hash:net,port       5       comment support
    hash:net,port       4       counters support
    hash:net,port       3       nomatch flag support
    hash:net,port       2       Add/del range support
    hash:net,port       1       SCTP and UDPLITE support
    hash:net,port,net   2       skbinfo support
    hash:net,port,net   1       forceadd support
    hash:net,port,net   0       initial revision
    hash:net,net        2       skbinfo support
    hash:net,net        1       forceadd support
    hash:net,net        0       initial revision
    hash:net            6       skbinfo support
    hash:net            5       forceadd support
    hash:net            4       comment support
    hash:net            3       counters support
    hash:net            2       nomatch flag support
    hash:net            1       Add/del range support
    hash:net            0       Initial revision
    hash:ip,port,net    7       skbinfo support
    hash:ip,port,net    6       forceadd support
    hash:ip,port,net    5       comment support
    hash:ip,port,net    4       counters support
    hash:ip,port,net    3       nomatch flag support
    hash:ip,port,net    2       Add/del range support
    hash:ip,port,net    1       SCTP and UDPLITE support
    hash:ip,port,ip     5       skbinfo support
    hash:ip,port,ip     4       forceadd support
    hash:ip,port,ip     3       comment support
    hash:ip,port,ip     2       counters support
    hash:ip,port,ip     1       SCTP and UDPLITE support
    hash:ip,mark        2       skbinfo support
    hash:ip,mark        1       forceadd support
    hash:ip,mark        0       initial revision
    hash:ip,port        5       skbinfo support
    hash:ip,port        4       forceadd support
    hash:ip,port        3       comment support
    hash:ip,port        2       counters support
    hash:ip,port        1       SCTP and UDPLITE support
    hash:ip             4       skbinfo support
    hash:ip             3       forceadd support
    hash:ip             2       comment support
    hash:ip             1       counters support
    hash:ip             0       Initial revision
    bitmap:port         3       skbinfo support
    bitmap:port         2       comment support
    bitmap:port         1       counters support
    bitmap:port         0       Initial revision
    bitmap:ip,mac       3       skbinfo support
    bitmap:ip,mac       2       comment support
    bitmap:ip,mac       1       counters support
    bitmap:ip,mac       0       Initial revision
    bitmap:ip           3       skbinfo support
    bitmap:ip           2       comment support
    bitmap:ip           1       counters support
    bitmap:ip           0       Initial revision
```





## 子命令


* `list <set>`

  列出指定set内容（不传入则列出所有sets内容）
  
  如：
  
  ```sh
  Name: xxx
  Type: hash:net
  Revision: 6
  Header: family inet hashsize 2048 maxelem 65536 timeout 604800
  Size in memory: 280664
  References: 1
  Number of entries: 8625
  Members:
  43.248.104.0/22 timeout 0
  ```

* `test `

  ```sh
  ipset test xxx 183.60.93.249
  ```

  

## 样例



ref: [ipset详解](https://blog.csdn.net/gymaisyl/article/details/101695697)



```sh
ipset list

ipset create aa hash:ip

ipset list aa
ipset test aa 1.1.1.1

ipset add aa 1.1.1.1
ipset list aa
ipset test aa 1.1.1.1

ipset del aa 1.1.1.1
ipset list aa
pset test aa 1.1.1.1

ipset add aa 1.1.1.1
ipset flush aa  # 清空set aa的条目（不含sets本身）
ipset destroy aa  # 删掉这个set
ipset list aa
```

