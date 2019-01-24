

```sh
dtruss -h
USAGE: dtruss [-acdefholLs] [-t syscall] { -p PID | -n name | command }

          -p PID          # examine this PID
          -n name         # examine this process name
          -t syscall      # examine this syscall only
          -a              # print all details
          -c              # print syscall counts
          -d              # print relative times (us)
          -e              # print elapsed times (us)
          -f              # follow children
          -l              # force printing pid/lwpid
          -o              # print on cpu times
          -s              # print stack backtraces
          -L              # don't print pid/lwpid
          -b bufsize      # dynamic variable buf size
   eg,
       dtruss df -h       # run and examine "df -h"
       dtruss -p 1871     # examine PID 1871
       dtruss -n tar      # examine all processes called "tar"
       dtruss -f test.sh  # run test.sh and follow children
```





