

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
