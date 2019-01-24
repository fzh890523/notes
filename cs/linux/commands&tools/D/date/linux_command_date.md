











# 显示



## 格式





### 显示更精确的时间（毫秒微秒纳秒）：`%N`



```sh
date +"%T.%N" # returns the current time with nanoseconds.
06:46:41.431857000

date +"%T.%6N" # returns the current time with nanoseconds rounded to the first 6 digits, which is microseconds.
06:47:07.183172

date +"%T.%3N" # returns the current time with nanoseconds rounded to the first 3 digits, which is milliseconds.
06:47:42.773
```





### unix timestamp

Ref: https://unix.stackexchange.com/questions/69322/how-to-get-milliseconds-since-unix-epoch



`date +%s.%N` will give you, eg., `1364391019.877418748`. The %N is the number of nanoseconds elapsed in the current second. Notice it is 9 digits, and by default date will pad this with zeros if it is less than 100000000. This is actually a problem if we want to do math with the number, because [bash treats numbers with a leading zero as octal](http://tldp.org/LDP/abs/html/numerical-constants.html). This padding can be disabled by using a hyphen in the field spec, so:

```bsh
echo $((`date +%s`*1000+`date +%-N`/1000000))
```

would naively give you milliseconds since the epoch.

**However**, as Stephane Chazelas points out in comment below, that's two different `date` calls which will yield two slightly different times. If the second has rolled over in between them, the calculation will be an entire second off. So:

```bsh
echo $(($(date +'%s * 1000 + %-N / 1000000')))
```



* `date +%s` 秒
* `echo $(($(date +'%s * 1000 + %-N / 1000000')))` 毫秒



