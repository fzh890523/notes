

```Sh
docker cp foo.txt mycontainer:/foo.txt
docker cp mycontainer:/foo.txt foo.txt
```

> 对于image，见下面 image tutorial 部分



* `-L` follow symbol link

  默认会copy回link文件