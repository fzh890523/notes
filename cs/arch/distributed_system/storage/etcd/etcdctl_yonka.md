

# 使用



ref:

* https://chromium.googlesource.com/external/github.com/coreos/etcd/+/release-3.0/etcdctl/README.md

* https://etcd.io/docs/v3.4/demo/#access-etcd

* https://etcd.io/docs/v3.4/dev-guide/interacting_v3/





## server连接



* `--endpoint=`



## 一些参数



* `-w`输出格式
  * `-w json`

* `--prefix`

* `--rev`

* `--limit`

* `--endpoints` 

  `etcdctl --endpoints=$ENDPOINTS put foo "Hello World!"`



## 一些env



```sh
export ETCDCTL_API=3
export ETCDCTL_ENDPOINTS=https://portal1130-24.brilliant-etcd-21.compose-34.composedb.com:20773,https://portal1142-25.brilliant-etcd-21.compose-34.composedb.com:20773
export ETCDCTL_USER=root:password
```





## get

`GET [options] <key> [range_end]`



* `get /key1 --hex` 输出hex格式，一般用于查看binary

* `etcdctl get --prefix /testdir/testkey`
* `etcdctl get --prefix --limit=2 /testdir/testkey`
* `etcdctl get --prefix --rev=4 foo`



## put

`PUT [options] <key> <value>`



## watch

`watch [options] <key or prefix>\n`



## del



`DEL [options] <key> [range_end]`



## txn



这个是个比较复杂的命令

```sh
<Txn> ::= <CMP>* "\n" <THEN> "\n" <ELSE> "\n"
<CMP> ::= (<CMPCREATE>|<CMPMOD>|<CMPVAL>|<CMPVER>) "\n"
<CMPOP> ::= "<" | "=" | ">"
<CMPCREATE> := ("c"|"create")"("<KEY>")" <REVISION>
<CMPMOD> ::= ("m"|"mod")"("<KEY>")" <CMPOP> <REVISION>
<CMPVAL> ::= ("val"|"value")"("<KEY>")" <CMPOP> <VALUE>
<CMPVER> ::= ("ver"|"version")"("<KEY>")" <CMPOP> <VERSION>
<THEN> ::= <OP>*
<ELSE> ::= <OP>*
<OP> ::= ((see put, get, del etcdctl command syntax)) "\n"
<KEY> ::= (%q formatted string)
<VALUE> ::= (%q formatted string)
<REVISION> ::= "\""[0-9]+"\""
<VERSION> ::= "\""[0-9]+"\""
```



样例：

* `-i`交互模式

  ```sh
  ./etcdctl txn -i
  mod("key1") > "0"
  
  put key1 "overwrote-key1"
  
  put key1 "created-key1"
  put key2 "some extra key"
  
  FAILURE
  
  OK
  
  OK
  ```

* 非交互模式

  ```sh
  ./etcdctl txn <<<'mod("key1") > "0"
  
  put key1 "overwrote-key1"
  
  put key1 "created-key1"
  put key2 "some extra key"
  
  '
  FAILURE
  
  OK
  
  OK
  ```



```sh
//先设置上值flag = 1
$ etcdctl put flag 1
OK
//txn事务开始
$ etcdctl txn -i
compares:
value("flag") = "1"
//输入空格确认 我输入了两次
//设置上边验证成功了该怎么办？
success requests (get, put, delete):
//成功了就put设置值
put result true
//设置上边失败了怎么办?
failure requests (get, put, delete):
//失败了也是put值
put result false
SUCCESS
OK
//然后验证
$ etcdctl get result
result
true

————————————————
原文作者：huxiaobai_001
转自链接：https://learnku.com/articles/48767
版权声明：著作权归作者所有。商业转载请联系作者获得授权，非商业转载请保留以上作者信息和原文链接。
```





## lease

`LEASE <subcommand>`

* `LEASE GRANT <ttl>`
* `LEASE REVOKE <leaseID>`
* `LEASE REVOKE <leaseID>`



```sh
$ etcdctl lease grant 100
lease 38015a3c00490513 granted with TTL(100s)
$ etcdctl put k1 v1 --lease=38015a3c00490513
OK
$ etcdctl lease timetolive 38015a3c00490513
lease 38015a3c00490513 granted with TTL(100s), remaining(67s)
$ etcdctl lease timetolive 38015a3c00490513
lease 38015a3c00490513 granted with TTL(100s), remaining(64s)
$ etcdctl lease timetolive 38015a3c00490513 --keys
lease 38015a3c00490513 granted with TTL(100s), remaining(59s), attached keys([k1])
$ etcdctl put k2 v2 --lease=38015a3c00490513
OK
$ etcdctl lease timetolive 38015a3c00490513 --keys
lease 38015a3c00490513 granted with TTL(100s), remaining(46s), attached keys([k1 k2])
$ etcdctl lease revoke 38015a3c00490513 
lease 38015a3c00490513 revoked
$ etcdctl get k1
$ etcdctl get k2
$ 
$ etcdctl lease grant 10
lease 38015a3c0049051d granted with TTL(10s)
$ etcdctl lease keep-alive 38015a3c0049051d
lease 38015a3c0049051d keepalived with TTL(10)
lease 38015a3c0049051d keepalived with TTL(10)
lease 38015a3c0049051d keepalived with TTL(10)

————————————————
原文作者：huxiaobai_001
转自链接：https://learnku.com/articles/48767
版权声明：著作权归作者所有。商业转载请联系作者获得授权，非商业转载请保留以上作者信息和原文链接。
```





## member



`MEMBER <subcommand>`

* `MEMBER ADD <memberName>`
* `MEMBER UPDATE <memberID>`
* `MEMBER REMOVE <memberID>`
* `MEMBER LIST`



## 其他工具类命令



### endpoint

`ENDPOINT <subcommand>`

* `ENDPOINT HEALTH`
* `ENDPOINT STATUS`



### lock

`LOCK <lockname>`



### elect

`ELECT [options] <election-name> [proposal]`



### compaction

`COMPACTION <revision>`



### defrag



### make-mirror

`MAKE-MIRROR [options] <destination>`



### snapshot

`SNAPSHOT <subcommand>`

* `SNAPSHOT SAVE <filename>`
* `SNAPSHOT RESTORE [options] <filename>`
* `SNAPSHOT STATUS <filename>`



### migrate

`MIGRATE [options]`



### auth

`AUTH <enable or disable>`



### role

* `ROLE ADD <role name>`
* `ROLE DELETE <role name>`
* `ROLE GET <role name>`
* `ROLE GRANT-PERMISSION <role name> <permission type> <key> [endkey]`
* `ROLE REVOKE-PERMISSION <role name> <permission type> <key> [endkey]`



### user

* `USER ADD <user name>`
* `USER DELETE <user name>`
* `USER GET <user name>`
* `USER PASSWD <user name>`
* `USER GRANT-ROLE <user name> <role name>`
* `USER REVOKE-ROLE <user name> <role name>`



