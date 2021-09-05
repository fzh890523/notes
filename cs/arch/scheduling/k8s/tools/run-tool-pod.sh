name=$(kubectl get pod --all-namespaces --field-selector 'metadata.name=bb'  | grep -v ^NAMESPACE | awk '{printf("%s/%s", $1, $2)}')
test -z "$name" && { echo "run bb in current ns: "; kubectl run bb --image yonka-ubuntu-180413-1:5000/yonka/ubuntu:18.04-server-addTelnet -- /bin/sh -c "sleep infinity" } || echo "found bb: $name"
