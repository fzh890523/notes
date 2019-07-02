

### docker vm

```sh
screen ~/Library/Containers/com.docker.docker/Data/vms/0/tty
```

### docker image path

`~/Library/Containers/com.docker.docker/Data/vms/0/Docker.[qcow2, raw]`

ref: https://stackoverflow.com/questions/19234831/where-are-docker-images-stored-on-the-host-machine

If you keep in mind that Docker is still running in a VM, the system paths are relative to the VM and not from the Mac Osx system. As it says all is contained in a VM file :

```sh
/Users/MyUserName/Library/Containers/com.docker.docker/Data/com.docker.driver.amd64-linux/Docker.qcow2
```

Try to run Alpine image with this volume option and the ls command you are able to list the VM host:

```sh
docker run --rm -it -v /:/vm-root alpine:edge ls -l /vm-root
```

After this just try :

```sh
docker run --rm -it -v /:/vm-root alpine:edge ls -l /vm-root/var/lib/docker
```

Now, you are able to list the docker folder from the WM host


mac -> vm -> docker

