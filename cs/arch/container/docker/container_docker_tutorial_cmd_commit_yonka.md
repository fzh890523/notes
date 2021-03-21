

```sh
docker exec -it <container_id> /bin/sh
touch test.txt  # 文件变更
exit

docker commit <container_id> username/helloworld:v2  # 新建的文件被提交到新的版本
```

