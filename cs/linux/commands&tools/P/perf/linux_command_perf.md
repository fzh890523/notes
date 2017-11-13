

# 火焰图



Ref: 

* [FlameGraph-github](https://github.com/brendangregg/FlameGraph)



# perf命令



## perf record



* `-p ${pid}` 
* `-s` per thread

## perf report



* `--pid=${pid1},${pid2}`



# tutorial



```shell
perf record -F 99 -a -g -s
cp perf.data perf.data.201710261257
perf report --pid=19712
perf script > out.perf.201710261257
perf script --pid=19712 > out.perf.201710261257.pid19712
perl FlameGraph-master/stackcollapse-perf.pl out.perf.201710261257 > out.folded.201710261257
perl FlameGraph-master/stackcollapse-perf.pl out.perf.201710261257.pid19712 > out.folded.201710261257.pid19712
perl FlameGraph-master/flamegraph.pl out.folded.201710261257 > kernel.svg.201710261257
perl FlameGraph-master/flamegraph.pl out.folded.201710261257.pid19712 > kernel.svg.201710261257.pid19712
```

