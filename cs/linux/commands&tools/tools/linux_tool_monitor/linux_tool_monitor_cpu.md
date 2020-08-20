

```sh
function watch_cpu() {
	local target_pid=$1
	local gap=${2:-1}

    local pids=()
    local stat_files=()

    for pid_item in `ps -ef | awk '{if($2 == '${target_pid}' || $3 == '${target_pid}'){print $2}}'`; do
        pids+=(${pid_item})
        stat_files+=("/proc/"${pid_item}"/stat")
    done

    local v=0
    local tickets=`getconf CLK_TCK`

    while true; do
        # this is equivalent to: second foo bar baz bat
        cat ${stat_files[@]} | awk '{
            res+=$14; 
            res+=$15; 
            res+=$16; 
            res+=$17;
        }END{
            t='${tickets}'; 
            total_secs=res/t; 
            mins=int(total_secs/60); 
            secs=int(total_secs) - mins*60; 
            left=int(total_secs*100)-int(total_secs)*100; 
            printf("%.2f %d:%d:%d\n", total_secs, mins, secs, left)
        }'
        sleep ${gap}
    done    
}
```

```sh
watch_cpu ${pid} ${gap} | awk '{
    if(NR > 1) {
        printf("%s %.3f\n", strftime("%m/%d/%Y %H:%M:%S", systime()), $1 - last)
    }
    last=$1;
}'
```


```sh
while true; do
  ps -a -x -o pid,lstart,etime,cputime,rss | grep `pidof envoy` >> ./a.log
  sleep 60
done
# save as a.sh


# nohup bash a.sh > a.log 

# a.log格式类似： 
#   1298 Fri Feb 28 15:01:40 2020  6-03:09:53 1-14:40:51 9280076
#   1298 Fri Feb 28 15:01:40 2020  6-03:09:53 1-15:40:51 9280076

# 算每个cpu累计值
cat a.log | awk '{print $8}' | awk -F'[-:]' '{for(i=NF; i>0; i--){if((NF-i)==3){sum+=24*3600*$i; break} else {sum+=$i*60^(NF-i)}}; print sum}'
# 139251
# 282102

# 算每个cpu差值（也即这一分钟消耗的cpu）
cat a.log | awk '{print $8}' | awk -F'[-:]' '{for(i=NF; i>0; i--){if((NF-i)==3){sum+=24*3600*$i; break} else {sum+=$i*60^(NF-i)}}; print sum}' | awk '{if(NR>1){print $1-last}; last=$1}'
# 142851
```


