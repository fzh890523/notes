# index

- 使用测试程序进行profile
  - [下载代码](#下载代码)
  - [尝试build运行](#尝试build运行)
  - [修改内容进行cpu profile](#修改内容进行cpu profile)
  - [修改内容进行mem profile](#修改内容进行mem profile)
- 通过网络进行profile
  - [profile](#profile)
  - [heap](#heap)
  - [block](#block)


i# 使用测试程序进行profile

## 下载代码













 

```
cd /root/sw
svn checkout http://multi-language-bench.googlecode.com/svn/trunk/ multi-language-bench
# 如果下不下来就去git找找

yum install graphviz.x86_64
yum install xdg-utils
```





## 尝试build运行













 

```
cd /root/sw/multi-language-bench/src/havlak/go

/usr/local/go_1.4.2/pkg/tool/linux_amd64/6g basicblock.go
/usr/local/go_1.4.2/pkg/tool/linux_amd64/6g lsg.go
/usr/local/go_1.4.2/pkg/tool/linux_amd64/6g havlakloopfinder.go
/usr/local/go_1.4.2/pkg/tool/linux_amd64/6g looptesterapp.go
/usr/local/go_1.4.2/pkg/tool/linux_amd64/6l looptesterapp
# 把path加好以后，make就行了
/usr/bin/time -f '%Uu %Ss %er %MkB %C' ./6.out
```





## pprof命令













 

```
(pprof) help

Commands:
   cmd [n] [--cum] [focus_regex]* [-ignore_regex]*
       Produce a text report with the top n entries.
       Include samples matching focus_regex, and exclude ignore_regex.
       Add --cum to sort using cumulative data.
       Available commands:
         callgrind    Outputs a graph in callgrind format
         disasm       Output annotated assembly for functions matching regexp or address
         dot          Outputs a graph in DOT format
         eog          Visualize graph through eog
         evince       Visualize graph through evince
         gif          Outputs a graph image in GIF format
         gv           Visualize graph through gv
         list         Output annotated source for functions matching regexp
         pdf          Outputs a graph in PDF format
         peek         Output callers/callees of functions matching regexp
         png          Outputs a graph image in PNG format
         proto        Outputs the profile in compressed protobuf format
         ps           Outputs a graph in PS format
         raw          Outputs a text representation of the raw profile
         svg          Outputs a graph in SVG format
         tags         Outputs all tags in the profile
         text         Outputs top entries in text form
         top          Outputs top entries in text form
         tree         Outputs a text rendering of call graph
         web          Visualize graph through web browser
         weblist      Output annotated source in HTML for functions matching regexp or address
   peek func_regex
       Display callers and callees of functions matching func_regex.

   dot [n] [focus_regex]* [-ignore_regex]* [>file]
       Produce an annotated callgraph with the top n entries.
       Include samples matching focus_regex, and exclude ignore_regex.
       For other outputs, replace dot with:
       - Graphic formats: dot, svg, pdf, ps, gif, png (use > to name output file)
       - Graph viewer:    gv, web, evince, eog

   callgrind [n] [focus_regex]* [-ignore_regex]* [>file]
       Produce a file in callgrind-compatible format.
       Include samples matching focus_regex, and exclude ignore_regex.

   weblist func_regex [-ignore_regex]*
       Show annotated source with interspersed assembly in a web browser.

   list func_regex [-ignore_regex]*
       Print source for routines matching func_regex, and exclude ignore_regex.

   disasm func_regex [-ignore_regex]*
       Disassemble routines matching func_regex, and exclude ignore_regex.

   tags tag_regex [-ignore_regex]*
       List tags with key:value matching tag_regex and exclude ignore_regex.

   quit/exit/^D
             Exit pprof.

   option=value
       The following options can be set individually:
           cum/flat:           Sort entries based on cumulative or flat data
           call_tree:          Build context-sensitive call trees
           nodecount:          Max number of entries to display
           nodefraction:       Min frequency ratio of nodes to display
           edgefraction:       Min frequency ratio of edges to display
           focus/ignore:       Regexp to include/exclude samples by name/file
           tagfocus/tagignore: Regexp or value range to filter samples by tag
                               eg "1mb", "1mb:2mb", ":64kb"

           functions:          Level of aggregation for sample data
           files:
           lines:
           addresses:

           unit:               Measurement unit to use on reports

           Sample value selection by index:
            sample_index:      Index of sample value to display
            mean:              Average sample value over first value

           Sample value selection by name:
            alloc_space        for heap profiles
            alloc_objects
            inuse_space
            inuse_objects

            total_delay        for contention profiles
            mean_delay
            contentions

   :   Clear focus/ignore/hide/tagfocus/tagignore
```





## 修改内容进行cpu profile













 

```
vim looptesterapp.go  # 加上打pprof的部分代码
/usr/local/go_1.4.2/pkg/tool/linux_amd64/6g looptesterapp.go
/usr/local/go_1.4.2/pkg/tool/linux_amd64/6l looptesterapp
./6.out -cpuprofile=havlak1.prof

go tool pprof 6.out havlak1.prof 

(pprof) top10
43690ms of 52610ms total (83.05%)
Dropped 71 nodes (cum <= 263.05ms)
Showing top 10 nodes out of 37 (cum >= 1310ms)
      flat  flat%   sum%        cum   cum%
   15940ms 30.30% 30.30%    15940ms 30.30%  scanblock
    8390ms 15.95% 46.25%    26920ms 51.17%  /root/sw/multi-language-bench/src/havlak/go/havlakloopfinder.FindLoops
    4600ms  8.74% 54.99%     4920ms  9.35%  runtime.mapaccess1_fast64
    3510ms  6.67% 61.66%     6590ms 12.53%  runtime.mapassign1
    3330ms  6.33% 67.99%     3960ms  7.53%  runtime.mallocgc
    2150ms  4.09% 72.08%     2150ms  4.09%  runtime.MSpan_Sweep
    2040ms  3.88% 75.96%     5330ms 10.13%  /root/sw/multi-language-bench/src/havlak/go/havlakloopfinder.DFS
    1540ms  2.93% 78.88%     1830ms  3.48%  runtime.mapiternext
    1260ms  2.39% 81.28%     1260ms  2.39%  runtime.writebarrierptr
     930ms  1.77% 83.05%     1310ms  2.49%  runtime.evacuate
(pprof) top10 -cum
(pprof) top10 -cum
34.48s of 52.61s total (65.54%)
Dropped 71 nodes (cum <= 0.26s)
Showing top 10 nodes out of 37 (cum >= 4.92s)
      flat  flat%   sum%        cum   cum%
         0     0%     0%     27.11s 51.53%  runtime.goexit
         0     0%     0%     27.09s 51.49%  main.main
         0     0%     0%     27.09s 51.49%  runtime.main
         0     0%     0%     26.92s 51.17%  /root/sw/multi-language-bench/src/havlak/go/havlakloopfinder.FindHavlakLoops
     8.39s 15.95% 15.95%     26.92s 51.17%  /root/sw/multi-language-bench/src/havlak/go/havlakloopfinder.FindLoops
         0     0% 15.95%     16.33s 31.04%  GC
    15.94s 30.30% 46.25%     15.94s 30.30%  scanblock
     3.51s  6.67% 52.92%      6.59s 12.53%  runtime.mapassign1
     2.04s  3.88% 56.80%      5.33s 10.13%  /root/sw/multi-language-bench/src/havlak/go/havlakloopfinder.DFS
     4.60s  8.74% 65.54%      4.92s  9.35%  runtime.mapaccess1_fast64
(pprof) web
(pprof) web mapaccess1
(pprof) list DFS
Total: 52.61s
ROUTINE ======================== /root/sw/multi-language-bench/src/havlak/go/havlakloopfinder.DFS in /root/sw/multi-language-bench/src/havlak/go/havlakloopfinder.go
     2.04s     10.60s (flat, cum) 20.15% of Total
         .          .    153:   return false
         .          .    154:}
         .          .    155:
         .          .    156:// DFS - Depth-First-Search and node numbering.
         .          .    157://
      10ms       10ms    158:func DFS(currentNode *cfg.BasicBlock, nodes []*UnionFindNode, number map[*cfg.BasicBlock]int, last []int, current int) int {
      90ms      150ms    159:   nodes[current].Init(currentNode, current)
         .      340ms    160:   number[currentNode] = current
         .          .    161:
         .          .    162:   lastid := current
     830ms      830ms    163:   for ll := currentNode.OutEdges().Front(); ll != nil; ll = ll.Next() {
     830ms      2.98s    164:           if target := ll.Value.(*cfg.BasicBlock); number[target] == unvisited {
      60ms      5.33s    165:                   lastid = DFS(target, nodes, number, last, lastid+1)
         .          .    166:           }
         .          .    167:   }
     200ms      940ms    168:   last[number[currentNode]] = lastid
      20ms       20ms    169:   return lastid
         .          .    170:}
         .          .    171:
         .          .    172:// FindLoops
         .          .    173://
         .          .    174:// Find loops and build loop forest using Havlak's algorithm, which
```





## 修改内容进行mem profile













 

```
vim looptesterapp.go  # 加入memprofile部分代码
/usr/local/go_1.4.2/pkg/tool/linux_amd64/6g looptesterapp.go
/usr/local/go_1.4.2/pkg/tool/linux_amd64/6l looptesterapp.6
./6.out -memprofile=6.mprof -cpuprofile=6.cprof

go tool pprof 6.out 6.mprof
(pprof) top5
77317.58kB of 77317.58kB total (  100%)
Dropped 9 nodes (cum <= 386.59kB)
Showing top 5 nodes out of 16 (cum >= 14336.66kB)
      flat  flat%   sum%        cum   cum%
45059.98kB 58.28% 58.28% 45059.98kB 58.28%  /root/sw/multi-language-bench/src/havlak/go/basicblock.(*CFG).CreateNode
31745.45kB 41.06% 99.34% 31745.45kB 41.06%  container/list.(*List).insertValue
  512.14kB  0.66%   100%   512.14kB  0.66%  mcommoninit
         0     0%   100% 17408.80kB 22.52%  /root/sw/multi-language-bench/src/havlak/go/basicblock.(*BasicBlock).AddInEdge
         0     0%   100% 14336.66kB 18.54%  /root/sw/multi-language-bench/src/havlak/go/basicblock.(*BasicBlock).AddOutEdge
(pprof) list CreateNode
Total: 75.51MB
ROUTINE ======================== /root/sw/multi-language-bench/src/havlak/go/basicblock.(*CFG).CreateNode in /root/sw/multi-language-bench/src/havlak/go/basicblock.go
      44MB       44MB (flat, cum) 58.28% of Total
         .          .     99:
         .          .    100:func (cfg *CFG) CreateNode(node int) *BasicBlock {
         .          .    101:   if bblock := cfg.bb[node]; bblock != nil {
         .          .    102:           return bblock
         .          .    103:   }
      28MB       28MB    104:   bblock := NewBasicBlock(node)
      16MB       16MB    105:   cfg.bb[node] = bblock
         .          .    106:
         .          .    107:   if cfg.NumNodes() == 1 {
         .          .    108:           cfg.startNode = bblock
         .          .    109:   }
         .          .    110:

go tool pprof --inuse_objects 6.out 6.mprof
(pprof) top5
957601 of 959421 total (99.81%)
Dropped 12 nodes (cum <= 4797)
Showing top 5 nodes out of 13 (cum >= 957601)
      flat  flat%   sum%        cum   cum%
    677226 70.59% 70.59%     677226 70.59%  container/list.(*List).insertValue
    280375 29.22% 99.81%     280375 29.22%  /root/sw/multi-language-bench/src/havlak/go/basicblock.(*CFG).CreateNode
         0     0% 99.81%     371382 38.71%  /root/sw/multi-language-bench/src/havlak/go/basicblock.(*BasicBlock).AddInEdge
         0     0% 99.81%     305844 31.88%  /root/sw/multi-language-bench/src/havlak/go/basicblock.(*BasicBlock).AddOutEdge
         0     0% 99.81%     957601 99.81%  /root/sw/multi-language-bench/src/havlak/go/basicblock.NewBasicBlockEdge
```







# 通过网络进行profile

## profile













 

```
localhost:~:# go tool pprof http://127.0.0.1:5099/debug/pprof/profile   
Fetching profile from http://127.0.0.1:5099/debug/pprof/profile
Please wait... (30s)
Saved profile in /root/pprof/pprof.127.0.0.1:5099.samples.cpu.001.pb.gz
Entering interactive mode (type "help" for commands)
(pprof) top5
10ms of 10ms total (  100%)
      flat  flat%   sum%        cum   cum%
      10ms   100%   100%       10ms   100%  runtime.scanobject
         0     0%   100%       10ms   100%  runtime.gcBgMarkWorker
         0     0%   100%       10ms   100%  runtime.gcDrainUntilPreempt
         0     0%   100%       10ms   100%  runtime.goexit

(pprof) tree
10ms of 10ms total (  100%)
----------------------------------------------------------+-------------
      flat  flat%   sum%        cum   cum%   calls calls% + context             
----------------------------------------------------------+-------------
                                              10ms   100% |   runtime.gcDrainUntilPreempt
      10ms   100%   100%       10ms   100%                | runtime.scanobject
----------------------------------------------------------+-------------
                                              10ms   100% |   runtime.goexit
         0     0%   100%       10ms   100%                | runtime.gcBgMarkWorker
                                              10ms   100% |   runtime.gcDrainUntilPreempt
----------------------------------------------------------+-------------
                                              10ms   100% |   runtime.gcBgMarkWorker
         0     0%   100%       10ms   100%                | runtime.gcDrainUntilPreempt
                                              10ms   100% |   runtime.scanobject
----------------------------------------------------------+-------------
         0     0%   100%       10ms   100%                | runtime.goexit
                                              10ms   100% |   runtime.gcBgMarkWorker
----------------------------------------------------------+-------------

(pprof) peek
10ms of 10ms total (  100%)
----------------------------------------------------------+-------------
      flat  flat%   sum%        cum   cum%   calls calls% + context             
----------------------------------------------------------+-------------
                                              10ms   100% |   runtime.gcDrainUntilPreempt
      10ms   100%   100%       10ms   100%                | runtime.scanobject
----------------------------------------------------------+-------------
                                              10ms   100% |   runtime.goexit
         0     0%   100%       10ms   100%                | runtime.gcBgMarkWorker
                                              10ms   100% |   runtime.gcDrainUntilPreempt
----------------------------------------------------------+-------------
                                              10ms   100% |   runtime.gcBgMarkWorker
         0     0%   100%       10ms   100%                | runtime.gcDrainUntilPreempt
                                              10ms   100% |   runtime.scanobject
----------------------------------------------------------+-------------
         0     0%   100%       10ms   100%                | runtime.goexit
                                              10ms   100% |   runtime.gcBgMarkWorker
----------------------------------------------------------+-------------

(pprof) raw
PeriodType: cpu nanoseconds
Period: 10000000
Time: 2015-11-12 00:02:52.213542837 +0800 CST
Duration: 30s
Samples:
samples/count cpu/nanoseconds
          1   10000000: 1 2 3 4
Locations
     1: 0x41e22b runtime.scanobject :0 s=0
     2: 0x41dd71 runtime.gcDrainUntilPreempt :0 s=0
     3: 0x41a863 runtime.gcBgMarkWorker :0 s=0
     4: 0x460510 runtime.goexit :0 s=0
Mappings
```





## heap













 

```
localhost:~:# go tool pprof http://127.0.0.1:5099/debug/pprof/heap  
Fetching profile from http://127.0.0.1:5099/debug/pprof/heap
Saved profile in /root/pprof/pprof.127.0.0.1:5099.inuse_objects.inuse_space.001.pb.gz
Entering interactive mode (type "help" for commands)
(pprof) top5
5414.55kB of 7462.86kB total (72.55%)
Dropped 32 nodes (cum <= 37.31kB)
Showing top 5 nodes out of 26 (cum >= 565.76kB)
      flat  flat%   sum%        cum   cum%
 1616.48kB 21.66% 21.66%  1616.48kB 21.66%  github.com/samuel/go-zookeeper/zk.(*Conn).recvLoop
 1616.48kB 21.66% 43.32%  1616.48kB 21.66%  github.com/samuel/go-zookeeper/zk.(*Conn).sendLoop
 1024.08kB 13.72% 57.04%  2693.62kB 36.09%  database/sql.(*DB).prepare
  591.75kB  7.93% 64.97%  1103.78kB 14.79%  database/sql.(*driverConn).prepareLocked
  565.76kB  7.58% 72.55%   565.76kB  7.58%  database/sql.(*DB).addDep

(pprof) tree
7462.86kB of 7462.86kB total (  100%)
Dropped 32 nodes (cum <= 37.31kB)
----------------------------------------------------------+-------------
      flat  flat%   sum%        cum   cum%   calls calls% + context             
----------------------------------------------------------+-------------
                                         1616.48kB   100% |   github.com/samuel/go-zookeeper/zk.(*Conn).loop.func2
 1616.48kB 21.66% 21.66%  1616.48kB 21.66%                | github.com/samuel/go-zookeeper/zk.(*Conn).recvLoop
----------------------------------------------------------+-------------
                                         1616.48kB   100% |   github.com/samuel/go-zookeeper/zk.(*Conn).loop.func1
 1616.48kB 21.66% 43.32%  1616.48kB 21.66%                | github.com/samuel/go-zookeeper/zk.(*Conn).sendLoop
----------------------------------------------------------+-------------
                                         2693.62kB   100% |   database/sql.(*DB).Prepare
 1024.08kB 13.72% 57.04%  2693.62kB 36.09%                | database/sql.(*DB).prepare
                                         1103.78kB 66.11% |   database/sql.(*driverConn).prepareLocked
                                          565.76kB 33.89% |   database/sql.(*DB).addDep
----------------------------------------------------------+-------------
                                         1103.78kB   100% |   database/sql.(*DB).prepare
  591.75kB  7.93% 64.97%  1103.78kB 14.79%                | database/sql.(*driverConn).prepareLocked
                                          512.02kB   100% |   github.com/go-sql-driver/mysql.(*mysqlConn).Prepare
----------------------------------------------------------+-------------
                                          565.76kB   100% |   database/sql.(*DB).prepare
  565.76kB  7.58% 72.55%   565.76kB  7.58%                | database/sql.(*DB).addDep
----------------------------------------------------------+-------------
                                          512.19kB   100% |   runtime.mpreinit
  512.19kB  6.86% 79.42%   512.19kB  6.86%                | runtime.malg
----------------------------------------------------------+-------------
                                          512.07kB   100% |   common/define/logformat.LoadConfig
  512.07kB  6.86% 86.28%   512.07kB  6.86%                | github.com/felixhao/log4go.NewFileLogWriter
----------------------------------------------------------+-------------
                                          512.03kB   100% |   bilizone/dao/reply.NewReplyReportDao
  512.03kB  6.86% 93.14%   512.03kB  6.86%                | fmt.Sprintf
----------------------------------------------------------+-------------
                                          512.02kB   100% |   database/sql.(*driverConn).prepareLocked
  512.02kB  6.86%   100%   512.02kB  6.86%                | github.com/go-sql-driver/mysql.(*mysqlConn).Prepare
----------------------------------------------------------+-------------
                                          512.02kB   100% |   bilizone/service.Init
         0     0%   100%   512.02kB  6.86%                | bilizone/dao/favourite.NewVideoDao
                                          512.02kB   100% |   common/define/utils.Stmt
----------------------------------------------------------+-------------
                                         1024.08kB   100% |   bilizone/service.Init
         0     0%   100%  1024.08kB 13.72%                | bilizone/dao/reply.NewReplyActionDao
                                         1024.08kB   100% |   common/define/utils.Stmt
----------------------------------------------------------+-------------
                                          565.76kB   100% |   bilizone/service.Init
         0     0%   100%   565.76kB  7.58%                | bilizone/dao/reply.NewReplyDao
                                          565.76kB   100% |   common/define/utils.Stmt
----------------------------------------------------------+-------------
                                         1103.79kB   100% |   bilizone/service.Init
         0     0%   100%  1103.79kB 14.79%                | bilizone/dao/reply.NewReplyReportDao
                                          591.75kB 53.61% |   common/define/utils.Stmt
                                          512.03kB 46.39% |   fmt.Sprintf
----------------------------------------------------------+-------------
                                         3205.65kB   100% |   main.main
         0     0%   100%  3205.65kB 42.95%                | bilizone/service.Init
                                         1103.79kB 34.43% |   bilizone/dao/reply.NewReplyReportDao
                                         1024.08kB 31.95% |   bilizone/dao/reply.NewReplyActionDao
                                          565.76kB 17.65% |   bilizone/dao/reply.NewReplyDao
                                          512.02kB 15.97% |   bilizone/dao/favourite.NewVideoDao
----------------------------------------------------------+-------------
                                          512.07kB   100% |   main.main
         0     0%   100%   512.07kB  6.86%                | common/define/logformat.LoadConfig
                                          512.07kB   100% |   github.com/felixhao/log4go.NewFileLogWriter
----------------------------------------------------------+-------------
                                         1024.08kB 38.02% |   bilizone/dao/reply.NewReplyActionDao
                                          591.75kB 21.97% |   bilizone/dao/reply.NewReplyReportDao
                                          565.76kB 21.00% |   bilizone/dao/reply.NewReplyDao
                                          512.02kB 19.01% |   bilizone/dao/favourite.NewVideoDao
         0     0%   100%  2693.62kB 36.09%                | common/define/utils.Stmt
                                         2693.62kB   100% |   database/sql.(*DB).Prepare
----------------------------------------------------------+-------------
                                         2693.62kB   100% |   common/define/utils.Stmt
         0     0%   100%  2693.62kB 36.09%                | database/sql.(*DB).Prepare
                                         2693.62kB   100% |   database/sql.(*DB).prepare
----------------------------------------------------------+-------------
                                         1616.48kB   100% |   runtime.goexit
         0     0%   100%  1616.48kB 21.66%                | github.com/samuel/go-zookeeper/zk.(*Conn).loop.func1
                                         1616.48kB   100% |   github.com/samuel/go-zookeeper/zk.(*Conn).sendLoop
----------------------------------------------------------+-------------
                                         1616.48kB   100% |   runtime.goexit
         0     0%   100%  1616.48kB 21.66%                | github.com/samuel/go-zookeeper/zk.(*Conn).loop.func2
                                         1616.48kB   100% |   github.com/samuel/go-zookeeper/zk.(*Conn).recvLoop
----------------------------------------------------------+-------------
                                         3717.72kB   100% |   runtime.main
         0     0%   100%  3717.72kB 49.82%                | main.main
                                         3205.65kB 86.23% |   bilizone/service.Init
                                          512.07kB 13.77% |   common/define/logformat.LoadConfig
----------------------------------------------------------+-------------
         0     0%   100%  6950.68kB 93.14%                | runtime.goexit
                                         3717.72kB 53.49% |   runtime.main
                                         1616.48kB 23.26% |   github.com/samuel/go-zookeeper/zk.(*Conn).loop.func1
                                         1616.48kB 23.26% |   github.com/samuel/go-zookeeper/zk.(*Conn).loop.func2
----------------------------------------------------------+-------------
                                         3717.72kB   100% |   runtime.goexit
         0     0%   100%  3717.72kB 49.82%                | runtime.main
                                         3717.72kB   100% |   main.main
----------------------------------------------------------+-------------
                                          512.19kB   100% |   runtime.schedinit
         0     0%   100%   512.19kB  6.86%                | runtime.mcommoninit
                                          512.19kB   100% |   runtime.mpreinit
----------------------------------------------------------+-------------
                                          512.19kB   100% |   runtime.mcommoninit
         0     0%   100%   512.19kB  6.86%                | runtime.mpreinit
                                          512.19kB   100% |   runtime.malg
----------------------------------------------------------+-------------
         0     0%   100%   512.19kB  6.86%                | runtime.rt0_go
                                          512.19kB   100% |   runtime.schedinit
----------------------------------------------------------+-------------
                                          512.19kB   100% |   runtime.rt0_go
         0     0%   100%   512.19kB  6.86%                | runtime.schedinit
                                          512.19kB   100% |   runtime.mcommoninit
----------------------------------------------------------+-------------

(pprof) peek
7462.86kB of 7462.86kB total (  100%)
Dropped 32 nodes (cum <= 37.31kB)
----------------------------------------------------------+-------------
      flat  flat%   sum%        cum   cum%   calls calls% + context             
----------------------------------------------------------+-------------
                                         1616.48kB   100% |   github.com/samuel/go-zookeeper/zk.(*Conn).loop.func2
 1616.48kB 21.66% 21.66%  1616.48kB 21.66%                | github.com/samuel/go-zookeeper/zk.(*Conn).recvLoop
----------------------------------------------------------+-------------
                                         1616.48kB   100% |   github.com/samuel/go-zookeeper/zk.(*Conn).loop.func1
 1616.48kB 21.66% 43.32%  1616.48kB 21.66%                | github.com/samuel/go-zookeeper/zk.(*Conn).sendLoop
----------------------------------------------------------+-------------
                                         2693.62kB   100% |   database/sql.(*DB).Prepare
 1024.08kB 13.72% 57.04%  2693.62kB 36.09%                | database/sql.(*DB).prepare
                                         1103.78kB 66.11% |   database/sql.(*driverConn).prepareLocked
                                          565.76kB 33.89% |   database/sql.(*DB).addDep
----------------------------------------------------------+-------------
                                         1103.78kB   100% |   database/sql.(*DB).prepare
  591.75kB  7.93% 64.97%  1103.78kB 14.79%                | database/sql.(*driverConn).prepareLocked
                                          512.02kB   100% |   github.com/go-sql-driver/mysql.(*mysqlConn).Prepare
----------------------------------------------------------+-------------
                                          565.76kB   100% |   database/sql.(*DB).prepare
  565.76kB  7.58% 72.55%   565.76kB  7.58%                | database/sql.(*DB).addDep
----------------------------------------------------------+-------------
                                          512.19kB   100% |   runtime.mpreinit
  512.19kB  6.86% 79.42%   512.19kB  6.86%                | runtime.malg
----------------------------------------------------------+-------------
                                          512.07kB   100% |   common/define/logformat.LoadConfig
  512.07kB  6.86% 86.28%   512.07kB  6.86%                | github.com/felixhao/log4go.NewFileLogWriter
----------------------------------------------------------+-------------
                                          512.03kB   100% |   bilizone/dao/reply.NewReplyReportDao
  512.03kB  6.86% 93.14%   512.03kB  6.86%                | fmt.Sprintf
----------------------------------------------------------+-------------
                                          512.02kB   100% |   database/sql.(*driverConn).prepareLocked
  512.02kB  6.86%   100%   512.02kB  6.86%                | github.com/go-sql-driver/mysql.(*mysqlConn).Prepare
----------------------------------------------------------+-------------
                                          512.02kB   100% |   bilizone/service.Init
         0     0%   100%   512.02kB  6.86%                | bilizone/dao/favourite.NewVideoDao
                                          512.02kB   100% |   common/define/utils.Stmt
----------------------------------------------------------+-------------
                                         1024.08kB   100% |   bilizone/service.Init
         0     0%   100%  1024.08kB 13.72%                | bilizone/dao/reply.NewReplyActionDao
                                         1024.08kB   100% |   common/define/utils.Stmt
----------------------------------------------------------+-------------
                                          565.76kB   100% |   bilizone/service.Init
         0     0%   100%   565.76kB  7.58%                | bilizone/dao/reply.NewReplyDao
                                          565.76kB   100% |   common/define/utils.Stmt
----------------------------------------------------------+-------------
                                         1103.79kB   100% |   bilizone/service.Init
         0     0%   100%  1103.79kB 14.79%                | bilizone/dao/reply.NewReplyReportDao
                                          591.75kB 53.61% |   common/define/utils.Stmt
                                          512.03kB 46.39% |   fmt.Sprintf
----------------------------------------------------------+-------------
                                         3205.65kB   100% |   main.main
         0     0%   100%  3205.65kB 42.95%                | bilizone/service.Init
                                         1103.79kB 34.43% |   bilizone/dao/reply.NewReplyReportDao
                                         1024.08kB 31.95% |   bilizone/dao/reply.NewReplyActionDao
                                          565.76kB 17.65% |   bilizone/dao/reply.NewReplyDao
                                          512.02kB 15.97% |   bilizone/dao/favourite.NewVideoDao
----------------------------------------------------------+-------------
                                          512.07kB   100% |   main.main
         0     0%   100%   512.07kB  6.86%                | common/define/logformat.LoadConfig
                                          512.07kB   100% |   github.com/felixhao/log4go.NewFileLogWriter
----------------------------------------------------------+-------------
                                         1024.08kB 38.02% |   bilizone/dao/reply.NewReplyActionDao
                                          591.75kB 21.97% |   bilizone/dao/reply.NewReplyReportDao
                                          565.76kB 21.00% |   bilizone/dao/reply.NewReplyDao
                                          512.02kB 19.01% |   bilizone/dao/favourite.NewVideoDao
         0     0%   100%  2693.62kB 36.09%                | common/define/utils.Stmt
                                         2693.62kB   100% |   database/sql.(*DB).Prepare
----------------------------------------------------------+-------------
                                         2693.62kB   100% |   common/define/utils.Stmt
         0     0%   100%  2693.62kB 36.09%                | database/sql.(*DB).Prepare
                                         2693.62kB   100% |   database/sql.(*DB).prepare
----------------------------------------------------------+-------------
                                         1616.48kB   100% |   runtime.goexit
         0     0%   100%  1616.48kB 21.66%                | github.com/samuel/go-zookeeper/zk.(*Conn).loop.func1
                                         1616.48kB   100% |   github.com/samuel/go-zookeeper/zk.(*Conn).sendLoop
----------------------------------------------------------+-------------
                                         1616.48kB   100% |   runtime.goexit
         0     0%   100%  1616.48kB 21.66%                | github.com/samuel/go-zookeeper/zk.(*Conn).loop.func2
                                         1616.48kB   100% |   github.com/samuel/go-zookeeper/zk.(*Conn).recvLoop
----------------------------------------------------------+-------------
                                         3717.72kB   100% |   runtime.main
         0     0%   100%  3717.72kB 49.82%                | main.main
                                         3205.65kB 86.23% |   bilizone/service.Init
                                          512.07kB 13.77% |   common/define/logformat.LoadConfig
----------------------------------------------------------+-------------
         0     0%   100%  6950.68kB 93.14%                | runtime.goexit
                                         3717.72kB 53.49% |   runtime.main
                                         1616.48kB 23.26% |   github.com/samuel/go-zookeeper/zk.(*Conn).loop.func1
                                         1616.48kB 23.26% |   github.com/samuel/go-zookeeper/zk.(*Conn).loop.func2
----------------------------------------------------------+-------------
                                         3717.72kB   100% |   runtime.goexit
         0     0%   100%  3717.72kB 49.82%                | runtime.main
                                         3717.72kB   100% |   main.main
----------------------------------------------------------+-------------
                                          512.19kB   100% |   runtime.schedinit
         0     0%   100%   512.19kB  6.86%                | runtime.mcommoninit
                                          512.19kB   100% |   runtime.mpreinit
----------------------------------------------------------+-------------
                                          512.19kB   100% |   runtime.mcommoninit
         0     0%   100%   512.19kB  6.86%                | runtime.mpreinit
                                          512.19kB   100% |   runtime.malg
----------------------------------------------------------+-------------
         0     0%   100%   512.19kB  6.86%                | runtime.rt0_go
                                          512.19kB   100% |   runtime.schedinit
----------------------------------------------------------+-------------
                                          512.19kB   100% |   runtime.rt0_go
         0     0%   100%   512.19kB  6.86%                | runtime.schedinit
                                          512.19kB   100% |   runtime.mcommoninit
----------------------------------------------------------+-------------

(pprof) peek NewReplyActionDao
7462.86kB of 7462.86kB total (  100%)
Dropped 32 nodes (cum <= 37.31kB)
----------------------------------------------------------+-------------
      flat  flat%   sum%        cum   cum%   calls calls% + context             
----------------------------------------------------------+-------------
                                         1024.08kB   100% |   bilizone/service.Init
         0     0%     0%  1024.08kB 13.72%                | bilizone/dao/reply.NewReplyActionDao
                                         1024.08kB   100% |   common/define/utils.Stmt
----------------------------------------------------------+-------------



(pprof) raw
PeriodType: objects bytes
Period: 524288
Time: 2015-11-11 23:23:22.792968036 +0800 CST
Samples:
inuse_objects/count inuse_space/bytes
          1    1655275: 2 3 4
                bytes:[1572864]
          1    1655275: 5 6 4
                bytes:[1572864]
          3     605957: 9 10 11 12 13 14 15 16 4
                bytes:[155648]
          5     579337: 17 18 11 12 19 20 15 16 4
                bytes:[106496]
       1365     524480: 21 22 23 24 25
                bytes:[384]
       3641     524360: 26 27 28 16 4
                bytes:[144]
       4096     524352: 29 11 12 30 31 15 16 4
                bytes:[128]
       8192     524320: 34 35 14 15 16 4
                bytes:[64]
      10923     524312: 36 37 10 11 12 38 39 15 16 4
                bytes:[48]
      16384     524304: 40 11 12 41 31 15 16 4
                bytes:[32]
          0          0: 43 4
                bytes:[0]
          0          0: 44 45 46 47 48 49 50 4
                bytes:[0]
          0          0: 9 10 11 12 51 20 15 16 4
                bytes:[0]
          0          0: 17 18 11 12 52 20 15 16 4
                bytes:[0]
          0          0: 53 54 55 56 57 58 59 60 61 62 4
                bytes:[0]
          0          0: 63 3 4
                bytes:[0]
          0          0: 64 65 66 67 68 69 70 71 72 73 4
                bytes:[0]
          0          0: 74 55 56 57 58 59 60 61 62 4
                bytes:[0]
          0          0: 75 76 4
                bytes:[0]
          0          0: 77 78 79 4
                bytes:[0]
          0          0: 80 81 62 4
                bytes:[0]
          0          0: 83 79 4
                bytes:[0]
          0          0: 84 54 55 56 57 58 59 60 61 62 4
                bytes:[0]
Locations
     1: 0x4423f5 runtime.makeslice :0 s=0
     2: 0x8d056d github.com/samuel/go-zookeeper/zk.(*Conn).sendLoop :0 s=0
     3: 0x8d96a7 github.com/samuel/go-zookeeper/zk.(*Conn).loop.func1 :0 s=0
     4: 0x460510 runtime.goexit :0 s=0
     5: 0x8d12d5 github.com/samuel/go-zookeeper/zk.(*Conn).recvLoop :0 s=0
     6: 0x8d9745 github.com/samuel/go-zookeeper/zk.(*Conn).loop.func2 :0 s=0
     7: 0x40b942 runtime.hashGrow :0 s=0
     8: 0x40a88a runtime.mapassign1 :0 s=0
     9: 0x73114b database/sql.(*driverConn).prepareLocked :0 s=0
    10: 0x734a76 database/sql.(*DB).prepare :0 s=0
    11: 0x734855 database/sql.(*DB).Prepare :0 s=0
    12: 0x564ba9 common/define/utils.Stmt :0 s=0
    13: 0x629b0c bilizone/dao/reply.NewReplyReportDao :0 s=0
    14: 0x4c6a24 bilizone/service.Init :0 s=0
    15: 0x401381 main.main :0 s=0
    16: 0x42efef runtime.main :0 s=0
    17: 0x731b97 database/sql.(*DB).addDep :0 s=0
    18: 0x734ce2 database/sql.(*DB).prepare :0 s=0
    19: 0x61f245 bilizone/dao/reply.NewReplyDao :0 s=0
    20: 0x4c69b0 bilizone/service.Init :0 s=0
    21: 0x435e27 runtime.malg :0 s=0
    22: 0x42a64e runtime.mpreinit :0 s=0
    23: 0x43055f runtime.mcommoninit :0 s=0
    24: 0x430108 runtime.schedinit :0 s=0
    25: 0x45dcf1 runtime.rt0_go :0 s=0
    26: 0x4d8248 github.com/felixhao/log4go.NewFileLogWriter :0 s=0
    27: 0x4cc0bb common/define/logformat.LoadConfig :0 s=0
    28: 0x40120a main.main :0 s=0
    29: 0x734b53 database/sql.(*DB).prepare :0 s=0
    30: 0x60efbc bilizone/dao/reply.NewReplyActionDao :0 s=0
    31: 0x4c6a07 bilizone/service.Init :0 s=0
    32: 0x446cb7 runtime.rawstringtmp :0 s=0
    33: 0x446b3e runtime.slicebytetostring :0 s=0
    34: 0x571f25 fmt.Sprintf :0 s=0
    35: 0x629ad8 bilizone/dao/reply.NewReplyReportDao :0 s=0
    36: 0x79ae87 github.com/go-sql-driver/mysql.(*mysqlConn).Prepare :0 s=0
    37: 0x731065 database/sql.(*driverConn).prepareLocked :0 s=0
    38: 0x5b99c1 bilizone/dao/favourite.NewVideoDao :0 s=0
    39: 0x4c6598 bilizone/service.Init :0 s=0
    40: 0x734bc8 database/sql.(*DB).prepare :0 s=0
    41: 0x60ee47 bilizone/dao/reply.NewReplyActionDao :0 s=0
    42: 0x40e509 runtime.convT2E :0 s=0
    43: 0x7b8555 github.com/felixhao/gosnowflake/client.(*Client).pingAndRetry :0 s=0
    44: 0x8f8862 encoding/gob.decString :0 s=0
    45: 0x8f917f encoding/gob.(*Decoder).decodeStruct :0 s=0
    46: 0x90139f encoding/gob.(*Decoder).decodeValue :0 s=0
    47: 0x902a36 encoding/gob.(*Decoder).DecodeValue :0 s=0
    48: 0x902858 encoding/gob.(*Decoder).Decode :0 s=0
    49: 0x8dfb80 net/rpc.(*gobClientCodec).ReadResponseHeader :0 s=0
    50: 0x8de31e net/rpc.(*Client).input :0 s=0
    51: 0x61f0b5 bilizone/dao/reply.NewReplyDao :0 s=0
    52: 0x61f565 bilizone/dao/reply.NewReplyDao :0 s=0
    53: 0x756a4f github.com/Shopify/sarama.(*TopicMetadata).decode :0 s=0
    54: 0x75710b github.com/Shopify/sarama.(*MetadataResponse).decode :0 s=0
    55: 0x751a74 github.com/Shopify/sarama.decode :0 s=0
    56: 0x7477aa github.com/Shopify/sarama.(*Broker).sendAndReceive :0 s=0
    57: 0x746480 github.com/Shopify/sarama.(*Broker).GetMetadata :0 s=0
    58: 0x74d2a8 github.com/Shopify/sarama.(*client).tryRefreshMetadata :0 s=0
    59: 0x74a70b github.com/Shopify/sarama.(*client).RefreshMetadata :0 s=0
    60: 0x74cded github.com/Shopify/sarama.(*client).backgroundMetadataUpdater :0 s=0
    61: 0x76425f github.com/Shopify/sarama.(*client).(github.com/Shopify/sarama.backgroundMetadataUpdater)-fm :0 s=0
    62: 0x763129 github.com/Shopify/sarama.withRecover :0 s=0
    63: 0x8d0e28 github.com/samuel/go-zookeeper/zk.(*Conn).sendLoop :0 s=0
    64: 0x8ea0d7 html.init :0 s=0
    65: 0x85a3df html/template.init :0 s=0
    66: 0x8e6cde net/rpc.init :0 s=0
    67: 0x7b901a github.com/felixhao/gosnowflake/client.init :0 s=0
    68: 0x6313ef bilizone/dao/snowflake.init :0 s=0
    69: 0x5dfddd bilizone/dao.init :0 s=0
    70: 0x4cba6a bilizone/service.init :0 s=0
    71: 0x4a4db0 bilizone/http.init :0 s=0
    72: 0x4029bf main.init :0 s=0
    73: 0x42efbe runtime.main :0 s=0
    74: 0x757053 github.com/Shopify/sarama.(*MetadataResponse).decode :0 s=0
    75: 0x44a107 time.Sleep :0 s=0
    76: 0x7b87c3 github.com/felixhao/gosnowflake/client.(*Client).pingAndRetry :0 s=0
    77: 0x8dfee3 net/rpc.(*Client).Go :0 s=0
    78: 0x8e01eb net/rpc.(*Client).Call :0 s=0
    79: 0x7b85b9 github.com/felixhao/gosnowflake/client.(*Client).pingAndRetry :0 s=0
    80: 0x74843f github.com/Shopify/sarama.(*Broker).responseReceiver :0 s=0
    81: 0x7638bf github.com/Shopify/sarama.(*Broker).(github.com/Shopify/sarama.responseReceiver)-fm :0 s=0
    82: 0x4047d3 runtime.makechan :0 s=0
    83: 0x8e0191 net/rpc.(*Client).Call :0 s=0
    84: 0x7569e3 github.com/Shopify/sarama.(*TopicMetadata).decode :0 s=0
Mappings

(pprof) tags
bytes: Total 44611
     16384 (36.73%): 32B
     10923 (24.48%): 48B
      8192 (18.36%): 64B
      4096 ( 9.18%): 128B
      3641 ( 8.16%): 144B
      1365 ( 3.06%): 384B
         5 (0.011%): 104kB
         3 (0.0067%): 152kB
         2 (0.0045%): 1.50MB
         0 (    0%): 0
```





## block













 

```
localhost:~:# go tool pprof http://127.0.0.1:5099/debug/pprof/block 
Fetching profile from http://127.0.0.1:5099/debug/pprof/block
parsing profile: unrecognized profile format
# 额...
```

