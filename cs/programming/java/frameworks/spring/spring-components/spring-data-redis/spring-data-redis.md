# 代码结构

## 类关系

- interface **RedisConnection**
  - *extends*
    - interface **RedisCommands**
      - *extends*
        - interface **RedisKeyCommands**
        - interface **RedisStringCommands**
        - interface **RedisListCommands**
        - interface **RedisSetCommands**
        - interface **RedisZSetCommands**
        - interface **RedisHashCommands**
        - interface **RedisTxCommands**
        - interface **RedisPubSubCommands**
        - interface **RedisConnectionCommands**
        - interface **RedisServerCommands**
        - interface **RedisScriptingCommands**
        - interface **HyperLogLogCommands**
      - Object execute(String command, byte[]… args)
  - void close()
  - boolean isClosed()
  - Object getNativeConnection()
  - boolean isQueueing()
  - boolean isPipelined()
  - void openPipeline()
  - List\<Object\> closePipeline()
  - RedisSentinelConnection getSentinelConnection()
  - *implementations*
    - interface **StringRedisConnection**
      - *implementations*
        - class **DefaultStringRedisConnection**
    - interface **RedisClusterConnection**
      - *implementations*
        - class **JedisClusterConnection**
        - class **LettuceClusterConnection**
    - class **AbstractRedisConnection**
      - *subclasses*
        - class **LettuceConnection**
          - *subclasses*
            - class **LettuceClusterConnection**
        - class **JedisConnection**
        - class **SrpConnection**
        - class **JredisConnection**
- interface **RedisOperations\<K, V\>**
  - *implementations*
    - class **RedisTemplate**
      - *subclasses*
        - class **StringRedisTemplate**
  - execute\*方法
  - executePipelined\*方法
  - key操作相关入口方法
  - 事务操作相关入口方法
  - pub/sub操作相关入口方法
  - opsFor\*方法
  - \*Ops方法
  - 其他杂项



## pub/sub

### 命令接口 RedisPubSubCommands

实现类在porting代码中

### 使用 - pub

`RedisTemplate.convertAndSend`中，以redis client的形式使用。

样例：

```java
    String channel = "user:topic";  
    //其中channel必须为string，而且“序列化”策略也是StringSerializer  
    //消息内容，将会根据配置文件中指定的valueSerializer进行序列化  
    //本例中，默认全部采用StringSerializer  
    //那么在消息的subscribe端也要对“发序列化”保持一致。  
    redisTemplate.convertAndSend(channel, "from app 1");  
```



### 使用 - subscribe/notify

`RedisMessageListenerContainer`和（以jedis为例）`JedisConnection`中为主。

1. 根据sub需求确定topic和listeners
2. 构建listener-container，传入executor、connectionFactory
3. 把 `listener:topics`对加入container，container会进行初始化然后（在container启动时，异步）做sub操作
   - 会使用（独占）一个连接，连接subscribed之前第一次sub会占用当前线程（反正不是start那个了）在BinaryJedisPubSub.psubscribe/subscribe中循环处理消息。 后面的sub都是使用conn.getSubscription().subscribe/psubscribe来…，只做sub而不负责处理消息
   - container这个层面的sub其实是向porting代码（conn）注册topics（channels or patterns）然后handler为一个dispatch handler，dispatch handler（会在conn里被调用）里会根据...做dispatch到原先sub的listeners
   - 最后dispatch到特定listener执行其回调是异步的（不能阻塞dispatch线程也即循环处理消息的线程），提交给taskExecutor处理

样例：

```xml
     <bean id="topicMessageListener" class="com.sample.redis.sdr.TopicMessageListener">  
        <property name="redisTemplate" ref="jedisTemplate"></property>  
    </bean>  
    <bean id="topicContainer" class="org.springframework.data.redis.listener.RedisMessageListenerContainer" destroy-method="destroy">  
        <property name="connectionFactory" ref="jedisConnectionFactory"/>  
        <property name="taskExecutor"><!-- 此处有个奇怪的问题，无法正确使用其他类型的Executor -->  
            <bean class="org.springframework.scheduling.concurrent.ThreadPoolTaskScheduler">  
                <property name="poolSize" value="3"></property>  
            </bean>  
        </property>  
        <property name="messageListeners">  
            <map>  
                <entry key-ref="topicMessageListener">  
                    <bean class="org.springframework.data.redis.listener.ChannelTopic">  
                        <constructor-arg value="user:topic"/>  
                    </bean>  
                </entry>  
            </map>  
        </property>  
    </bean>  
```

```java
    public class TopicMessageListener implements MessageListener {  
      
        private RedisTemplate redisTemplate;  
          
        public void setRedisTemplate(RedisTemplate redisTemplate) {  
            this.redisTemplate = redisTemplate;  
        }  
      
        @Override  
        public void onMessage(Message message, byte[] pattern) {  
            byte[] body = message.getBody();//请使用valueSerializer  
            byte[] channel = message.getChannel();  
            //请参考配置文件，本例中key，value的序列化方式均为string。  
            //其中key必须为stringSerializer。和redisTemplate.convertAndSend对应  
            String itemValue = (String)redisTemplate.getValueSerializer().deserialize(body);  
            String topic = (String)redisTemplate.getStringSerializer().deserialize(channel);  
            //...  
        }  
    }  
```





## script



## transaction

### 命令接口 RedisTxCommands

实现类在porting代码中

## connection

### 命令接口 RedisConnectionCommands

实现类在porting代码中

## GEO



## 数据

### key



### string



### hash



### list



### set



### sortedset



### hyperloglog



### GEO



# 信息汇总

## 方法&命令对应



- Hash
  - delete
    - hdel
  - entries
    - hgetall
  - get
    - hget
  - hasKey
    - hexists
  - increment（double、long）
    - hincrby
  - keys
    - hkeys
  - multiGet
    - hmget
  - put
    - hset
  - putAll
    - hmset
  - putIfAbsent
    - hsetnx
  - scan
    - hscan
  - size
    - hlen
  - values
    - hvals
- HyperLogLog
  - add
    - pfadd
  - size
    - pfcount
  - union
    - pfmerge+pfcount
- List
  - index
    - lindex
  - leftPop（普通、带超时）
    - lpop/blpop
  - leftPush（普通、指定元素之前）
    - lpush/linsert
  - leftPushAll
    - lpush
  - leftPushIfPresent
    - lpushx
  - range
    - lrange
  - remove
    - lrem
  - rightPop
    - rpop/brpop
  - rightPush（普通、指定元素之后）
    - rpush/linsert
  - rightPushAll
    - rpush
  - rightPushIfPresent
    - rpushx
  - set
    - lset
  - size
    - llen
  - trim
    - ltrim
- Set
  - add
    - sadd
  - difference
    - sdiff
  - differenceAndStore
    - sdiffstore
  - distinctRandomMembers
    - srandmember
  - intersect
    - sinter
  - intersectAndStore
    - sinterstore
  - isMember
    - sismember
  - members
    - smembers
  - move
    - smove
  - pop
    - spop
  - randomMember
    - srandmember
  - randomMembers（指定数量）
    - srandmember
  - remove（values）
    - srem
  - scan
    - sscan
  - size
    - scard
  - union
    - sunion
  - unionAndStore
    - sunionstore
- Value
  - append
    - append
  - get
    - get/getrange
  - getAndSet
    - getset
  - getbit
    - getbit
  - increment
    - incrby
  - multiGet
    - mget
  - multiSet
    - mset
  - multiSetIfAbsent
    - msetnx
  - set
    - set/setrange/setex/psetex
  - setBit
    - setbit
  - setIfAbsent
    - setnx
  - size
    - strlen
- ZSet
  - add（单个、批量）
    - zadd
  - count（min-max range）
    - zcount
  - incrementScore
    - zincrby
  - incrementAndStore
    - zinterstore
  - range
    - zrange
  - rangeByLex
    - zrangebylex
  - rangeByScore（min、max、offset、cout）
    - zrangebyscore
  - rangeByScoreWithScores（min、max、offset、cout）
    - zrangebyscorewithscores
  - rangeWithScores（min、max）
    - zrangewithscores
  - rank
    - zrank
  - remove
    - zrem
  - removeRange
    - zremrange
  - remveRangeByScore
    - zremrangebyscore
  - reverseRange
    - zrevrange
  - reverseRangeByScore
    - zrevrangebyscore
  - reverseRangeByScoreWithScores
    - zrevrangebyscorewithscores
  - reverseRangeWithScores
    - zrevrangewithscores
  - reverseRanke
    - revrank
  - scan
    - zscan
  - score
    - score
  - unionAndStore
    - zunionstore
  - zcard
    - zcard



