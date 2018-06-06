

# codis-notes

Permalink: [2015-05-01 07:27:51](http://idning.github.io/codis.html) by [ning](http://idning.github.io/author/ning.html) in [redis](http://idning.github.io/category/redis.html)

Table of Contents

- 1   概况
  - [1.1   设计特点](http://idning.github.io/codis.html#id2)
  - [1.2   和redis-clueter对比](http://idning.github.io/codis.html#redis-clueter)
  - [1.3   Why not Twemproxy & 我的反驳](http://idning.github.io/codis.html#why-not-twemproxy)
  - [1.4   迁移](http://idning.github.io/codis.html#id3)
  - [1.5   优点](http://idning.github.io/codis.html#id4)
  - [1.6   性能](http://idning.github.io/codis.html#id5)
  - [1.7   问题](http://idning.github.io/codis.html#id6)
- [2   使用&部署](http://idning.github.io/codis.html#id7)
- [3   zk 设计](http://idning.github.io/codis.html#zk)
- 4   代码
  - 4.1   redis改动
    - [4.1.1   增加hash_slots](http://idning.github.io/codis.html#hash-slots)
    - [4.1.2   阻塞迁移](http://idning.github.io/codis.html#id9)
    - [4.1.3   问题](http://idning.github.io/codis.html#id10)
  - 4.2   proxy
    - [4.2.1   actions](http://idning.github.io/codis.html#actions)
    - [4.2.2   router.go](http://idning.github.io/codis.html#router-go)
    - [4.2.3   更新路由](http://idning.github.io/codis.html#id11)
    - [4.2.4   pipeline](http://idning.github.io/codis.html#pipeline)
    - [4.2.5   mget](http://idning.github.io/codis.html#mget)
- 5   几个问题
  - [5.1   slot内存](http://idning.github.io/codis.html#slot)
  - [5.2   阻塞迁移大value](http://idning.github.io/codis.html#value)
  - 5.3   pipeline 性能
    - [5.3.1   性能问题](http://idning.github.io/codis.html#id14)
  - [5.4   使用时遇到的其它小问题](http://idning.github.io/codis.html#id15)
- [6   redis-port](http://idning.github.io/codis.html#redis-port)
- [7   参考](http://idning.github.io/codis.html#id16)
- [8   总结](http://idning.github.io/codis.html#id17)

## [1   概况](http://idning.github.io/codis.html#id18)

### [1.1   设计特点](http://idning.github.io/codis.html#id19)

- 分1024个slot

- zk保存拓扑.

- go实现proxy, 无状态.

- 预分配: 1024个slot (单实例5G, 大约可以支撑5-10T规模)

- 平滑扩容.

- - 5个组件:

    ZooKeeperCodis Redis (codis-server) 修改后的redis.Codis Proxy (codis-proxy) 实现proxy逻辑Dashboard 需要作为服务运行Codis Manager (codis-config) 作为工具运行, 调用dashboard api修改配置.

### [1.2   和redis-clueter对比](http://idning.github.io/codis.html#id20)

- - 在以下方面很像

    分slotredis单机引擎知道slot逻辑.阻塞迁移

- - 不同:

    使用中心存储 保存路由, 而不是goisp协议.使用proxy而不是客户端lib

### [1.3   Why not Twemproxy & 我的反驳](http://idning.github.io/codis.html#id21)

- - 最大痛点：无法平滑的扩/缩容（Scale!!!!）

    实际上, 可以通过迁移redis实例来做( **不过确实很痛苦, 我们一般直接迁移整个集群** )

- - 没有HA机制，没有容错能力

    实际上, 通过外部工具来做, 效果不错.

- - 修改配置需要重启服务

    实际上, 在作出修改配置决定前, 服务已经出问题至少30s了, 重启花1s并没有问题.

### [1.4   迁移](http://idning.github.io/codis.html#id22)

1. 把一个slot标记为pre-migrate
2. 等待所有proxy确认
3. 标记slot状态为migrating
4. 外部工具不断发送slotmigrate给源redis, 每次一个key, 把这个slot中所有key迁移走
5. 标记slot状态为online

notes:

- - 一次迁移一个key, 原子, 阻塞(有的文档说: 我们每次只原子的迁走一个 key，不会把主线程 block 住, 这是不对的)

    这里为了实现一致性, 降低了可用性(根据CAP, 在分布式系统中, 选择了C, A就会降低)为什么说放弃了可用性呢? 假设我们在迁移, 迁移1个key需要0.1ms(同机柜), 那么这个分片的qps就会降到10000qps以下了.如果是不同机房, rtt在1ms 左右, 迁移1个key需要1ms, 那么这个qps就降到1000qps以下了.

- - proxy 也可能发起迁移命令.

    迁移某个slot的过程中, proxy 会提前要求迁移这个key到目标分片.

- 必须保证不能同时有多个 slots 处于迁移状态 => 决定了很慢, 不能扩展到很大的集群(不过够用了)

### [1.5   优点](http://idning.github.io/codis.html#id23)

- 给redis一个扩容方案.
- proxy能利用多核, 单机性能比Twemproxy好.

### [1.6   性能](http://idning.github.io/codis.html#id24)

redis-benchmark -p 10000 -r 10000 -t set -n 10000000

单核4w 多核 + pipeline(-P 10): 4-5w

多核6w 多核 + pipeline(-P 10): 20w cpu占用700%, 每个核剩下10%左右.

很不错!

### [1.7   问题](http://idning.github.io/codis.html#id25)

- redis中每个key会多存一份(slots hash表), 如果key比较大, 很浪费redis内存( **大约1.5倍** ).

- - 阻塞式迁移, 为了一致性降低可用性.

    对大set, 大list等大value不友好, 可能导致redis **阻塞1-2s**

- 主从切换不是自动, 需要手动操作(建议自动操作, 操作后人工后验检查)

- 不能并发迁移，扩容会很慢, 影响集群规模.

- 直接把redis代码包进来了, 对redis的后续升级, bugfix等难以merge.

- 使用crc32做sharding, 不是很均匀, 不过分了slot, 所以没关系了.

- - proxy

    pipeline 性能差(后面优化了, 尚未测试)mget/mset 都是串行执行, 性能较差.

- 需要一个部署工具.

## [2   使用&部署](http://idning.github.io/codis.html#id26)

```
DATE=`date +'%Y%m%d%H%M'`
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# clean & start zk
cd ~/xredis/zookeeper-3.4.6 && ./bin/zkServer.sh stop

rm /tmp/zookeeper -rf
pkill -f codis-config

cd ~/xredis/zookeeper-3.4.6 && ./bin/zkServer.sh start

cd $DIR

nohup ./bin/codis-config -c sample/config.ini dashboard &

sleep 3

echo 'dash running'

./bin/codis-config -c sample/config.ini server add 0 localhost:2000 master
./bin/codis-config -c sample/config.ini server add 0 localhost:3000 slave

./bin/codis-config -c sample/config.ini server add 1 localhost:2001 master
./bin/codis-config -c sample/config.ini server add 1 localhost:3001 slave

./bin/codis-config -c sample/config.ini server add 2 localhost:2002 master
./bin/codis-config -c sample/config.ini server add 2 localhost:3002 slave

./bin/codis-config -c sample/config.ini server add 3 localhost:2003 master
./bin/codis-config -c sample/config.ini server add 3 localhost:3003 slave

./bin/codis-config -c sample/config.ini slot init
./bin/codis-config -c sample/config.ini slot range-set 0 511    0 online
./bin/codis-config -c sample/config.ini slot range-set 512 1023 1 online

./bin/codis-proxy -c sample/config.ini -L ./log/proxy.log  --cpu=8 --addr=0.0.0.0:19000 --http-addr=0.0.0.0:11000

```

## [3   zk 设计](http://idning.github.io/codis.html#id27)

zk 设计:

```
/codis/db_{xx}
{xx} means 产品名， 如: /codis/db_sync , /codis/db_applist
/codis/db_{xx}/servers/group_{N}/{ server addr (e.g. 127.0.0.1:6379) }
存储真实的 redis 组 (主master、从slave), N为一个自定义的整数编号, in JSON, 内容包括服务器地址，
角色（master or slave）等信息

/codis/db_{xx}/slots/slot_{N}

```

举例, zk中存储的数据如下:

```
$ zk-shell 127.1:2181
Welcome to zk-shell (1.0.05)
(CONNECTING) />
(CONNECTED) /> tree
├── zk
│   ├── codis
│   │   ├── db_test
│   │   │   ├── migrate_manager
│   │   │   ├── fence
│   │   │   ├── servers
│   │   │   │   ├── group_0
│   │   │   │   │   ├── localhost:2000
│   │   │   │   │   ├── localhost:3000
│   │   │   │   ├── group_1
│   │   │   │   │   ├── localhost:2001
│   │   │   │   │   ├── localhost:3001
│   │   │   ├── slots
│   │   │   │   ├── slot_0
│   │   │   │   ├── slot_1
│   │   │   │   ├── slot_2
│   │   │   │   ├── slot_3
│   │   │   │   ├── ...
│   │   │   │   ├── ...
│   │   │   │   ├── slot_1023
│   │   │   ├── proxy
|   │   │   │   ├── proxy_1
│   │   │   ├── migrate_tasks
│   │   │   ├── LOCK
│   │   │   ├── actions
│   │   │   │   ├── 0000000004
│   │   │   │   ├── 0000000010
│   │   │   │   ├── 0000000006
│   │   │   │   ├── 0000000008
│   │   │   │   ├── 0000000000
│   │   │   │   ├── 0000000002
│   │   │   ├── ActionResponse
│   │   │   │   ├── 0000000004
│   │   │   │   ├── 0000000010
│   │   │   │   ├── 0000000006
│   │   │   │   ├── 0000000008
│   │   │   │   ├── 0000000000
│   │   │   │   ├── 0000000002

```

其中几种节点数据:

```
(CONNECTED) /> get zk/codis/db_test/servers/group_0/localhost:2000
{"type":"master","group_id":0,"addr":"localhost:2000"}

(CONNECTED) /> get zk/codis/db_test/slots/slot_0
{"product_name":"test","id":0,"group_id":1,"state":{"status":"online","migrate_status":{"from":-1,"to":-1},"last_op_ts":"0"}}

(CONNECTED) /> get zk/codis/db_test/proxy/proxy_1
{"id":"proxy_1","addr":"127.1:19000","last_event":"","last_event_ts":0,"state":"offline","description":"","debug_var_addr":"127.1:11000","pid":12438,"start_at":"2015-04-28 15:20:23.739459751 +0800 CST"}

```

## [4   代码](http://idning.github.io/codis.html#id28)

### [4.1   redis改动](http://idning.github.io/codis.html#id29)

ext/redis-2.8.13/

- - 每个db增加N个slot. 每个slot里面是一个hash, 用于保存每个slot有哪些个key(导致每个key多存一份)

    key是raw_key, val 是crc(key).

- dictadd/dictDel/dictResize的时候都要在每个slot里面操作

- 增加一系列命令(slotsxxx) slots.c

#### [4.1.1   增加hash_slots](http://idning.github.io/codis.html#id30)

```
typedef struct redisDb {
    dict *dict;                 /* The keyspace for this DB */
    dict *expires;              /* Timeout of keys with a timeout set */
    ...
    dict *hash_slots[HASH_SLOTS_SIZE];
} redisDb;

initServer() {
    for (i = 0; i < HASH_SLOTS_SIZE; i ++) {
        server.db[j].hash_slots[i] = dictCreate(&hashSlotType, NULL);
    }

```

```
void dbAdd(redisDb *db, robj *key, robj *val) {
    sds copy = sdsdup(key->ptr);

    int retval = dictAdd(db->dict, copy, val);

    do {
        uint32_t crc;
        int slot = slots_num(key->ptr, &crc);
        dictAdd(db->hash_slots[slot], sdsdup(key->ptr), (void *)(long)crc);
    } while (0);
    ...
}

```

增加了一些命令:

```
{"slotsinfo",slotsinfoCommand,-1,"rF",0,NULL,0,0,0,0,0},
{"slotsdel",slotsdelCommand,-2,"w",0,NULL,1,-1,1,0,0},
{"slotsmgrtslot",slotsmgrtslotCommand,5,"aw",0,NULL,0,0,0,0,0},
{"slotsmgrtone",slotsmgrtoneCommand,5,"aw",0,NULL,0,0,0,0,0},
{"slotsmgrttagslot",slotsmgrttagslotCommand,5,"aw",0,NULL,0,0,0,0,0},
{"slotsmgrttagone",slotsmgrttagoneCommand,5,"aw",0,NULL,0,0,0,0,0},
{"slotshashkey",slotshashkeyCommand,-1,"rF",0,NULL,0,0,0,0,0},
{"slotscheck",slotscheckCommand,0,"r",0,NULL,0,0,0,0,0},
{"slotsrestore",slotsrestoreCommand,-4,"awm",0,NULL,1,1,1,0,0},

```

#### [4.1.2   阻塞迁移](http://idning.github.io/codis.html#id31)

```
static int
slotsmgrt(redisClient *c, sds host, sds port, int fd, int dbid, int timeout, robj *keys[], robj *vals[], int n) {
    ...
    syncWrite(fd, buf + pos, towrite, timeout);
    syncReadLine(fd, buf1, sizeof(buf1), timeout)
}

```

#### [4.1.3   问题](http://idning.github.io/codis.html#id32)

1. - 它会记录每个slot有哪些key, 这就会导致key多存一份, 这在某些情况下带来的内存消耗大约是1.5倍.(详见后面测试)

     而对redis来说, 空间很珍贵, 这里可以用时间换空间, 会更好.

2. 阻塞迁移造成的短时拒绝服务(详见后面测试).

### [4.2   proxy](http://idning.github.io/codis.html#id33)

代码量大约6000:

```
-------------------------------------------------------------------------------
Language                     files          blank        comment           code
-------------------------------------------------------------------------------
Go                              42            975            171           5113
Javascript                       7             84             99            596
HTML                             4             48             24            588
JSON                             3              0              0             52
CSS                              3              2              4             11
Bourne Shell                     2              3              0              6
-------------------------------------------------------------------------------
SUM:                            61           1112            298           6366
-------------------------------------------------------------------------------

```

代码结构:

```
▾ pkg/
  ▾ models/                 # 对应zk中的几个结构.
      server_group.go
      slot.go
      action.go
      proxy.go
  ▾ proxy/
    ▾ parser/
        parser.go           # 解析请求.
    ▾ redispool/
        conn.go             # 连接
        redispool.go        # 连接池
    ▾ cachepool/
        cachepool.go        # 由后端名字到连接池的map. [127.0.0.1:2000] => redispool {conn1, conn2, conn3}
    ▾ group/
        group.go            # 简单包装.
    ▾ router/
      ▸ topology/           # InitZkConn, watch
        helper.go           # redis命令黑名单, isMulOp, PING, SELECT 几个命令的处理.
        mapper.go           # mapKey2Slot (crc32 % 1024)
        session.go          # 记录当前实例的ops, starttime.
        router.go           # 主要proxy逻辑.
        multioperator.go    # mget/mset/del 的实现.

```

- pkg/models定义几种角色, slot, server_group, proxy 和几种动作, action, 它是zk的的数据模型.
- pkg/proxy 是proxy逻辑的实现.

看一个获得所有ServerGroups的代码:

```
func ServerGroups(zkConn zkhelper.Conn, productName string) ([]ServerGroup, error) {
    var ret []ServerGroup
    root := fmt.Sprintf("/zk/codis/db_%s/servers", productName)
    groups, _, err := zkConn.Children(root)

    for _, group := range groups {
        groupId, err := strconv.Atoi(strings.Split(group, "_")[1])
        g, err := GetGroup(zkConn, productName, groupId)
        ret = append(ret, *g)
    }
    return ret, nil
}

```

#### [4.2.1   actions](http://idning.github.io/codis.html#id34)

- - codis 对每个topo变化的操作, 都会记录到actions, 同时对于某些action, 会要求每个proxy ack确保更新到最新拓扑.

    TODO: 如何实现确保ack

#### [4.2.2   router.go](http://idning.github.io/codis.html#id35)

加载拓扑信息(一个slot指向那个group):

```
//use it in lock
func (s *Server) fillSlot(i int, force bool) {
    slotInfo, groupInfo, err := s.top.GetSlotByIndex(i)             # 从zk中获取slot信息

    slot := &Slot{
        slotInfo:  slotInfo,
        dst:       group.NewGroup(*groupInfo),
        groupInfo: groupInfo,
    }

    s.pools.AddPool(slot.dst.Master())                              # 准备连接池

    if slot.slotInfo.State.Status == models.SLOT_STATUS_MIGRATE {   # 处理MIGRATE状态.
        //get migrate src group and fill it
        from, err := s.top.GetGroup(slot.slotInfo.State.MigrateStatus.From)
        slot.migrateFrom = group.NewGroup(*from)
        s.pools.AddPool(slot.migrateFrom.Master())
    }

    s.slots[i] = slot
}

#如果状态是migrate 的slot, 发migrate命令
func (s *Server) handleMigrateState(slotIndex int, key []byte) error {
    ...
    err = WriteMigrateKeyCmd(redisConn.(*redispool.PooledConn), shd.dst.Master(), 30*1000, key)
    ...
}

```

转发逻辑, 读一个请求, 向后端转发一个:

```
func (s *Server) redisTunnel(c *session) error {
    resp, err := parser.Parse(c.r) // read client request

    op, k, err := getOpKey(resp)
    i := mapKey2Slot(k)

check_state:        # 这里是一个循环来检查, 等待SLOT_STATUS_PRE_MIGRATE结束
    s.mu.RLock()
    if s.slots[i] == nil {
        s.mu.Unlock()
        return errors.Errorf("should never happend, slot %d is empty", i)
    }
    //wait for state change, should be soon
    if s.slots[i].slotInfo.State.Status == models.SLOT_STATUS_PRE_MIGRATE {
        s.mu.RUnlock()
        time.Sleep(10 * time.Millisecond)
        goto check_state
    }

    s.handleMigrateState(i, k);

    //get redis connection
    redisConn, err := s.pools.GetConn(s.slots[i].dst.Master())
    redisErr, clientErr := forward(c, redisConn.(*redispool.PooledConn), resp)
}

func (s *Server) handleConn(c net.Conn) {
    for {
        err = s.redisTunnel(client)
        client.Ops++
    }
}

func (s *Server) Run() {
    log.Info("listening on", s.addr)
    listener, err := net.Listen("tcp", s.addr)
    for {
        conn, err := listener.Accept()
        go s.handleConn(conn)       #起一个
    }
}

```

#### [4.2.3   更新路由](http://idning.github.io/codis.html#id36)

proxy 会watch action 树下的变更, 有变化时 重新加载路由:

```
func (s *Server) OnGroupChange(groupId int) {
    log.Warning("group changed", groupId)

    for i, slot := range s.slots {
        if slot.slotInfo.GroupId == groupId {
            s.fillSlot(i, true)
        }
    }
}

```

#### [4.2.4   pipeline](http://idning.github.io/codis.html#id37)

上面代码是@ngaut同学pipeline优化前的代码,

一般来说, 实现pipeline可能存在下面两个问题, 不过测试发现: **codis都没有问题** .

1. 返回乱序:

   ```
   get     k1 k2 k3 k4
   return  v2 v1 v3 vj

   ```

原因是k1,k2发到不同后端, 如果其中一个后端很慢, 而先返回的后端就先写客户端, 就是这个错误.

测试发现codis没有这个问题, 但是看代码没有看懂为什么. 涉及到多个channel中传递消息(TODO).

1. 乱序执行:

   ```
   lpush   lst 1
   lpush   lst 2
   lpush   lst 3
   lpush   lst 4

   lpop return 2 1 3 4

   ```

第二种情况是发到同一个后端, 但是如果向同一个后端有多个连接, 就可能出这个问题. 某个连接上的请求先执行.

codis一个后端只有一个TaskRunner(一个连接), 所以应该不会出这个问题.

#### [4.2.5   mget](http://idning.github.io/codis.html#id38)

逐一访问:

```
func (oper *MultiOperator) mgetResults(mop *MulOp) ([]byte, error) {
    results := make([]interface{}, len(mop.keys))
    conn := oper.pool.Get()
    defer conn.Close()
    for i, key := range mop.keys {
        replys, err := redis.Values(conn.Do("mget", key))

        for _, reply := range replys {
            if reply != nil {
                results[i] = reply
            } else {
                results[i] = nil
            }
        }
    }

    b, err := respcoding.Marshal(results)
    return b, errors.Trace(err)
}

```

这是为了保证迁移过程中的一致性, 必须一个一个处理.

性能较差.

## [5   几个问题](http://idning.github.io/codis.html#id39)

### [5.1   slot内存](http://idning.github.io/codis.html#id40)

对于简单value(value大小1字节) slots内存占用:

```
keys         0          1000      10000     100000      1000000
---------------------------------------------------------------
codis-server 2519112    2678920   4073224   17449032    176095304
redis-server 908688     1019856   2078736   12356240    120496272

```

- 可以看出codis 内存占用大约是 原生redis 的 **1.5倍** .
- 越长的key, 浪费的内存越多.
- 对于大value(复杂的hash, set结构), 浪费的内存会较少.

测试代码: <https://gist.github.com/idning/b23c4d4fe76da5b00ae3>

### [5.2   阻塞迁移大value](http://idning.github.io/codis.html#id41)

为了保证迁移的一致性, codis选择牺牲可用性, 迁移单个key是通过阻塞当前实例来实现的.

一个100w 字段的hset(内存中大约占70M), 迁移耗时:

```
本机:           1.85 s
同机房不同机器: 2.06s

```

1-2s的不响应对大多数业务来说, 还是可以接受的, 所以这个问题不是很严重.

但是一定要注意:

1. 业务中不要出现1000w字段的hset, len=1000w的list之类.
2. 如果一个集群跨机房部署, 数据传输时间会更长, 迁移时间也会更长.

测试代码: <https://gist.github.com/idning/03f43b6789f14e1fe878>

在proxy代码中, 给迁移一个key设置的超时是30s:

```
func (s *Server) handleMigrateState(slotIndex int, key []byte) error {
    ...
    err = WriteMigrateKeyCmd(redisConn.(*redispool.PooledConn), shd.dst.Master(), 30*1000, key)
    ...
}

```

### [5.3   pipeline 性能](http://idning.github.io/codis.html#id42)

codis proxy对每个请求都是解析 => 找个连接发到后端 => 等待响应 => 发给客户端

这样就相当于不能使用pipeline, 而pipeline对要求高性能的case是非常重要的,

开pipeline的情况下, 单redis, 单线程client做简单set可以达到 **100w qps**.

#### [5.3.1   性能问题](http://idning.github.io/codis.html#id43)

spinlock同学的测试:

<https://github.com/wandoulabs/codis/issues/63>

```
CONC        PIPELINE        CODIS-LATENCY   REDIS-LATENCY
50      10          3.17            0.60
50      20          5.88            0.89
50      75          21.78           2.40p

```

@ngaut 同学在15年2月实现了pipeline的支持: <https://github.com/wandoulabs/codis/pull/110>

- - 用go来实现pipeline的问题是:

    pipeline并不是一个命令, pipeline实际上是异步处理所带来的一个好处, 只是一种使用方法.理论上, proxy/redis都不需要做任何事情来支持pipeline.但是codis采用了逐一转发的方法来处理请求, 就需要对pipeline专门处理.需要把go写成异步处理的形式, go 带来的编程模型简单的好处就没有了.优化后, 对所有的请求统一使用同样的方法(proxy并不知道自己处在一个pipeline中)

pipeline 遇到slot迁移状态性能又是一个严重的问题.

### [5.4   使用时遇到的其它小问题](http://idning.github.io/codis.html#id44)

1. 命令行支持:

   ```
   只支持 ./bin/codis-config  -c sample/config.ini dashboard
   不支持 ./bin/codis-config  dashboard -c sample/config.ini, 这个命令报一个非常奇怪的错误.

   ```

2. 努棒性:

配置写错, 比如:

```
dashboard_addr=:8087

```

zk节点也能建成功, 而且不删, 需要等一段时间.

1. 很多地方没有检查, 如果 add group 时写错参数.
2. proxy 启动后为啥不直接是online呢?
3. 代码上有些同名的函数, 比如NewServer一个Server是backendServer, 一个是proxy自己, 读的时候要小心.

## [6   redis-port](http://idning.github.io/codis.html#id45)

这是整个系统一个极大的亮点, 用于数据迁移, 比我们的redis-replay-aof好的是, 不需要到目标机器读aof文件.

实现原理: 作为一个假的 slave，挂在一个redis后面，然后将master的数据同步回来，sync 到 codis 集群上，

## [7   参考](http://idning.github.io/codis.html#id46)

- 设计文章: <http://0xffff.me/blog/2014/11/11/codis-de-she-ji-yu-shi-xian-part-2/>

- 代码: <https://github.com/wandoulabs/codis>

- codis design pdf

- - 豌豆荚分布式REDIS设计与实现－刘奇

    <http://blog.qiniu.com/?p=871>

## [8   总结](http://idning.github.io/codis.html#id47)

- 非常赞的人&非常赞的项目

- - 几个比较重要的问题:

    slot内存占用 `1.5倍`redis阻塞迁移, 为了一致性降低可用性.不是自动failoverpipeline/mget性能

- - 迁移状态是瞬态(在一个集群运行过程中, 迁移时间只占1/1000不到)

    但是为了保持迁移状态, mget 性能做了牺牲, redis内存占用做了牺牲, 这不划算.

- redis-port很赞.

每种方案都是适用的地方.

## Comments

来源： <http://idning.github.io/codis.html>

