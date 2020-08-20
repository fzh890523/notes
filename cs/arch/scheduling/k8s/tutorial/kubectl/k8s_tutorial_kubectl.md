



# tools



```sh
function kubeconf() {
  local cluster=$1
  
  if [[ "$cluster" == "/l" ]]; then
  	ls ${HOME}/.kube/config.* | awk -F '.' '{print $NF}'
  	return
  fi
  
  local f=${HOME}/.kube/config.${cluster}
  if [ ! -f "$f" ]; then
  	echo "can not find config file: $f"
  	return 1
  fi
  
  shift
  
  kubectl --kubeconfig=$f $@
}
```


```sh
kubectl -n nmspc exec "$POD" -- env curIP=123 script01

runScript() {
  kubectl -n nmspc exec "$POD" -- bash -c 'export curIP=123 && script01 "$@"' _ "$@"
}

runScript --command "do stuff"
```



















