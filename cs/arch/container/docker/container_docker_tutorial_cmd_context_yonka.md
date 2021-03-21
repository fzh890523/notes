

```sh
docker context create home-dev --description "home dev node via tunnel" --docker "host=tcp://localhost:2376"

docker context update home-dev --docker "host=tcp://ubuntu-20041-dev.localdomain:2375"
```



