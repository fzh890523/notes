
# shell 语法

## 控制语句

```shell
while [ 1 ]; do sleep 1; ll; done # 无限循环
while [ $i -lt 10 ]; do echo $i;let "i=$i+1"; done # 有限循环
cat raw.txt | while read line; do echo $line; done # readline
until [ 1 = 0 ]; do sleep 1; ll; done # 无限循环，另一中语义
for i in /media/m* ; do ls -l $i; done  # 与目录资源结合
if [ 1 -eq 1 ]; then ll ;fi # test常用判断
if [[ 0 -eq 0 && 1 -eq 0 ]]; then ll ;fi  ## || or  多条件test的写法1  && and  (注意2层中括号)
if [ 0 -eq 0 -a 1 -eq 0 ]; then ll ;fi    ## -o or  多条件test的写法2  -a and
if [ ! -e /tmp/111 -a -z "$a" ]; then ll ;fi   ## 不存在111文件 且a变量长度为0 则执行ll（对应的否定是）
```

