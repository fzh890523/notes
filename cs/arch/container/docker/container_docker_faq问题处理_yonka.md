

### Docker: Got permission denied while trying to connect to the Docker daemon socket at unix:///var/run/docker.sock

```sh
sudo usermod -a -G docker xxx_user
# 然后重新进入shell 或者 newgrp docker
```


