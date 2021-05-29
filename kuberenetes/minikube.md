

## Deploy

### Check nodes

```
$ kubectl get nodes
NAME       STATUS   ROLES    AGE     VERSION
minikube   Ready    master   6m32s   v1.14.1
```

### Create App

```
$ kubectl run kubernetes-bootcamp --image=gcr.io/google-samples/kubernetes-bootcamp:v1 --port=8080
kubectl run --generator=deployment/apps.v1 is DEPRECATED and will be removed in a future version. Use kubectl run --generator=run-pod/v1 or kubectl create instead.
deployment.apps/kubernetes-bootcamp created
```

### View App

```
$ kubectl get deployments
NAME                  READY   UP-TO-DATE   AVAILABLE   AGE
kubernetes-bootcamp   0/1     1            0           48s
```

### Proxy

```
$ Starting to serve on 127.0.0.1:8001

$ curl http://127.0.0.1:8001/version
{
  "major": "1",
  "minor": "14",
  "gitVersion": "v1.14.1",
  "gitCommit": "b7394102d6ef778017f2ca4046abbaa23b88c290",
  "gitTreeState": "clean",
  "buildDate": "2019-04-08T17:02:58Z",
  "goVersion": "go1.12.1",
  "compiler": "gc",
  "platform": "linux/amd64"
}
```

### Proxy for POD

```
$ export POD_NAME=$(kubectl get pods -o go-template --template '{{range .items}}{{.metadata.name}}{{"\n"}}{{end}}')
$ echo Name of the Pod: $POD_NAME
Name of the Pod: kubernetes-bootcamp-b94cb9bff-jh54n
```

```
curl http://localhost:8001/api/v1/namespaces/default/pods/$POD_NAME/proxy/
Hello Kubernetes bootcamp! | Running on: kubernetes-bootcamp-b94cb9bff-jh54n | v=1
```

## Explore

### Pods

![](https://d33wubrfki0l68.cloudfront.net/fe03f68d8ede9815184852ca2a4fd30325e5d15a/98064/docs/tutorials/kubernetes-basics/public/images/module_03_pods.svg)

### Nodes

![](https://d33wubrfki0l68.cloudfront.net/5cb72d407cbe2755e581b6de757e0d81760d5b86/a9df9/docs/tutorials/kubernetes-basics/public/images/module_03_nodes.svg)


### kubectl

- `kubectl get` - list resources
- `kubectl describe` - show detailed information about a resource
- `kubectl logs` - print the logs from a container in a pod
- `kubectl exec` - execute a command on a container in a pod

```
lyre@yonlli-LC2:~$ kubectl get deployments
NAME                  READY   UP-TO-DATE   AVAILABLE   AGE
kubernetes-bootcamp   1/1     1            1           22m
lyre@yonlli-LC2:~$ kubectl get pods
NAME                                  READY   STATUS    RESTARTS   AGE
kubernetes-bootcamp-b94cb9bff-jh54n   1/1     Running   0          22m
lyre@yonlli-LC2:~$ kubectl logs kubernetes-bootcamp-b94cb9bff-jh54n
Kubernetes Bootcamp App Started At: 2019-05-22T11:42:08.171Z | Running On:  kubernetes-bootcamp-b94cb9bff-jh54n 

Running On: kubernetes-bootcamp-b94cb9bff-jh54n | Total Requests: 1 | App Uptime: 676.704 seconds | Log Time: 2019-05-22T11:53:24.876Z
Running On: kubernetes-bootcamp-b94cb9bff-jh54n | Total Requests: 2 | App Uptime: 698.897 seconds | Log Time: 2019-05-22T11:53:47.068Z
Running On: kubernetes-bootcamp-b94cb9bff-jh54n | Total Requests: 3 | App Uptime: 740.147 seconds | Log Time: 2019-05-22T11:54:28.318Z
Running On: kubernetes-bootcamp-b94cb9bff-jh54n | Total Requests: 4 | App Uptime: 784.476 seconds | Log Time: 2019-05-22T11:55:12.647Z
Running On: kubernetes-bootcamp-b94cb9bff-jh54n | Total Requests: 5 | App Uptime: 824.022 seconds | Log Time: 2019-05-22T11:55:52.193Z
```

### Check Pods

```
lyre@yonlli-LC2:~$ kubectl get pods
NAME                                  READY   STATUS    RESTARTS   AGE
kubernetes-bootcamp-b94cb9bff-jh54n   1/1     Running   0          25m
lyre@yonlli-LC2:~$ kubectl describe pods
Name:               kubernetes-bootcamp-b94cb9bff-jh54n
Namespace:          default
Priority:           0
PriorityClassName:  <none>
Node:               minikube/10.0.2.15
Start Time:         Wed, 22 May 2019 19:41:21 +0800
Labels:             pod-template-hash=b94cb9bff
                    run=kubernetes-bootcamp
Annotations:        <none>
Status:             Running
IP:                 172.17.0.5
Controlled By:      ReplicaSet/kubernetes-bootcamp-b94cb9bff
Containers:
  kubernetes-bootcamp:
    Container ID:   docker://997a1462737a846b34899e21473a612473b17e77dbe1ac1c2b4f30fa4d9a9816
    Image:          gcr.io/google-samples/kubernetes-bootcamp:v1
    Image ID:       docker-pullable://gcr.io/google-samples/kubernetes-bootcamp@sha256:0d6b8ee63bb57c5f5b6156f446b3bc3b3c143d233037f3a2f00e279c8fcc64af
    Port:           8080/TCP
    Host Port:      0/TCP
    State:          Running
      Started:      Wed, 22 May 2019 19:42:08 +0800
    Ready:          True
    Restart Count:  0
    Environment:    <none>
    Mounts:
      /var/run/secrets/kubernetes.io/serviceaccount from default-token-zc9jg (ro)
Conditions:
  Type              Status
  Initialized       True 
  Ready             True 
  ContainersReady   True 
  PodScheduled      True 
Volumes:
  default-token-zc9jg:
    Type:        Secret (a volume populated by a Secret)
    SecretName:  default-token-zc9jg
    Optional:    false
QoS Class:       BestEffort
Node-Selectors:  <none>
Tolerations:     node.kubernetes.io/not-ready:NoExecute for 300s
                 node.kubernetes.io/unreachable:NoExecute for 300s
Events:
  Type    Reason     Age   From               Message
  ----    ------     ----  ----               -------
  Normal  Scheduled  25m   default-scheduler  Successfully assigned default/kubernetes-bootcamp-b94cb9bff-jh54n to minikube
  Normal  Pulling    25m   kubelet, minikube  Pulling image "gcr.io/google-samples/kubernetes-bootcamp:v1"
  Normal  Pulled     24m   kubelet, minikube  Successfully pulled image "gcr.io/google-samples/kubernetes-bootcamp:v1"
  Normal  Created    24m   kubelet, minikube  Created container kubernetes-bootcamp
  Normal  Started    24m   kubelet, minikube  Started container kubernetes-bootcamp
```

### Logs

```
$ kubectl logs $POD_NAME
Kubernetes Bootcamp App Started At: 2019-05-22T11:42:08.171Z | Running On:  kubernetes-bootcamp-b94cb9bff-jh54n 

Running On: kubernetes-bootcamp-b94cb9bff-jh54n | Total Requests: 1 | App Uptime: 676.704 seconds | Log Time: 2019-05-22T11:53:24.876Z
Running On: kubernetes-bootcamp-b94cb9bff-jh54n | Total Requests: 2 | App Uptime: 698.897 seconds | Log Time: 2019-05-22T11:53:47.068Z
Running On: kubernetes-bootcamp-b94cb9bff-jh54n | Total Requests: 3 | App Uptime: 740.147 seconds | Log Time: 2019-05-22T11:54:28.318Z
Running On: kubernetes-bootcamp-b94cb9bff-jh54n | Total Requests: 4 | App Uptime: 784.476 seconds | Log Time: 2019-05-22T11:55:12.647Z
Running On: kubernetes-bootcamp-b94cb9bff-jh54n | Total Requests: 5 | App Uptime: 824.022 seconds | Log Time: 2019-05-22T11:55:52.193Z
```

### Exec

```
$ kubectl exec $POD_NAME env
PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
HOSTNAME=kubernetes-bootcamp-b94cb9bff-jh54n
KUBERNETES_PORT_443_TCP_PROTO=tcp
KUBERNETES_PORT_443_TCP_PORT=443
KUBERNETES_PORT_443_TCP_ADDR=10.96.0.1
KUBERNETES_SERVICE_HOST=10.96.0.1
KUBERNETES_SERVICE_PORT=443
KUBERNETES_SERVICE_PORT_HTTPS=443
KUBERNETES_PORT=tcp://10.96.0.1:443
KUBERNETES_PORT_443_TCP=tcp://10.96.0.1:443
NPM_CONFIG_LOGLEVEL=info
NODE_VERSION=6.3.1
HOME=/root
```

Bash

```
kubectl exec -ti $POD_NAME bash
```


## Expose

> A Service in Kubernetes is an abstraction which defines a logical set of Pods and a policy by which to access them

 - `ClusterIP (default)` - Exposes the Service on an internal IP in the cluster. This type makes the Service only reachable from within the cluster.
 - `NodePort` - Exposes the Service on the same port of each selected Node in the cluster using NAT. Makes a Service accessible from outside the cluster using <NodeIP>:<NodePort>. Superset of ClusterIP.
 - `LoadBalancer` - Creates an external load balancer in the current cloud (if supported) and assigns a fixed, external IP to the Service. Superset of NodePort.
 - `ExternalName` - Exposes the Service using an arbitrary name (specified by externalName in the spec) by returning a CNAME record with the name. No proxy is used. This type requires v1.7 or higher of kube-dns.


### service 

```
lyre@yonlli-LC2:~$ kubectl expose deployment/kubernetes-bootcamp --type="NodePort" --port 8080
service/kubernetes-bootcamp exposed

lyre@yonlli-LC2:~$ kubectl get services
NAME                  TYPE        CLUSTER-IP      EXTERNAL-IP   PORT(S)          AGE
kubernetes            ClusterIP   10.96.0.1       <none>        443/TCP          67m
kubernetes-bootcamp   NodePort    10.104.224.53   <none>        8080:30754/TCP   5s

lyre@yonlli-LC2:~$ kubectl describe services/kubernetes-bootcamp
Name:                     kubernetes-bootcamp
Namespace:                default
Labels:                   run=kubernetes-bootcamp
Annotations:              <none>
Selector:                 run=kubernetes-bootcamp
Type:                     NodePort
IP:                       10.104.224.53
Port:                     <unset>  8080/TCP
TargetPort:               8080/TCP
NodePort:                 <unset>  30754/TCP
Endpoints:                172.17.0.5:8080
Session Affinity:         None
External Traffic Policy:  Cluster
Events:                   <none>
```

Test

```
lyre@yonlli-LC2:~$ export NODE_PORT=$(kubectl get services/kubernetes-bootcamp -o go-template='{{(index .spec.ports 0).nodePort}}')
lyre@yonlli-LC2:~$ echo NODE_PORT=$NODE_PORT
NODE_PORT=30754
lyre@yonlli-LC2:~$ curl $(minikube ip):$NODE_PORT
Hello Kubernetes bootcamp! | Running on: kubernetes-bootcamp-b94cb9bff-jh54n | v=1
```

### Labels

```
$ kubectl describe deployment
Name:                   kubernetes-bootcamp
Namespace:              default
CreationTimestamp:      Wed, 22 May 2019 19:41:21 +0800
Labels:                 run=kubernetes-bootcamp
Annotations:            deployment.kubernetes.io/revision: 1
Selector:               run=kubernetes-bootcamp
Replicas:               1 desired | 1 updated | 1 total | 1 available | 0 unavailable
StrategyType:           RollingUpdate
MinReadySeconds:        0
RollingUpdateStrategy:  25% max unavailable, 25% max surge
Pod Template:
  Labels:  run=kubernetes-bootcamp                              # <---- here
  Containers:
   kubernetes-bootcamp:
    Image:        gcr.io/google-samples/kubernetes-bootcamp:v1
    Port:         8080/TCP
    Host Port:    0/TCP
    Environment:  <none>
    Mounts:       <none>
  Volumes:        <none>
Conditions:
  Type           Status  Reason
  ----           ------  ------
  Available      True    MinimumReplicasAvailable
  Progressing    True    NewReplicaSetAvailable
OldReplicaSets:  <none>
NewReplicaSet:   kubernetes-bootcamp-b94cb9bff (1/1 replicas created)
Events:          <none>
```

```
lyre@yonlli-LC2:~$ kubectl get pods -l run=kubernetes-bootcamp
NAME                                  READY   STATUS    RESTARTS   AGE
kubernetes-bootcamp-b94cb9bff-jh54n   1/1     Running   1          72m
lyre@yonlli-LC2:~$ kubectl get services -l run=kubernetes-bootcamp
NAME                  TYPE       CLUSTER-IP      EXTERNAL-IP   PORT(S)          AGE
kubernetes-bootcamp   NodePort   10.104.224.53   <none>        8080:30754/TCP   13m
```

```
lyre@yonlli-LC2:~$ kubectl label pod $POD_NAME app=v1
pod/kubernetes-bootcamp-b94cb9bff-jh54n labeled
lyre@yonlli-LC2:~$ kubectl describe pods $POD_NAME
Name:               kubernetes-bootcamp-b94cb9bff-jh54n
Namespace:          default
Priority:           0
PriorityClassName:  <none>
Node:               minikube/10.0.2.15
Start Time:         Wed, 22 May 2019 19:41:21 +0800
Labels:             app=v1                                          # <--- here
                    pod-template-hash=b94cb9bff
                    run=kubernetes-bootcamp
Annotations:        <none>
Status:             Running
IP:                 172.17.0.4
Controlled By:      ReplicaSet/kubernetes-bootcamp-b94cb9bff
Containers:
  kubernetes-bootcamp:
    Container ID:   docker://376b922ace328067e442351fb40326c1390d1c07f372e87124cabf57a4371112
    Image:          gcr.io/google-samples/kubernetes-bootcamp:v1
    Image ID:       docker-pullable://gcr.io/google-samples/kubernetes-bootcamp@sha256:0d6b8ee63bb57c5f5b6156f446b3bc3b3c143d233037f3a2f00e279c8fcc64af
    Port:           8080/TCP
    Host Port:      0/TCP
    State:          Running
      Started:      Wed, 22 May 2019 20:48:54 +0800
    Ready:          True
    Restart Count:  1
    Environment:    <none>
    Mounts:
      /var/run/secrets/kubernetes.io/serviceaccount from default-token-zc9jg (ro)
Conditions:
  Type              Status
  Initialized       True 
  Ready             True 
  ContainersReady   True 
  PodScheduled      True 
Volumes:
  default-token-zc9jg:
    Type:        Secret (a volume populated by a Secret)
    SecretName:  default-token-zc9jg
    Optional:    false
QoS Class:       BestEffort
Node-Selectors:  <none>
Tolerations:     node.kubernetes.io/not-ready:NoExecute for 300s
                 node.kubernetes.io/unreachable:NoExecute for 300s
Events:
  Type    Reason          Age    From               Message
  ----    ------          ----   ----               -------
  Normal  SandboxChanged  6m15s  kubelet, minikube  Pod sandbox changed, it will be killed and re-created.
  Normal  Pulled          5m48s  kubelet, minikube  Container image "gcr.io/google-samples/kubernetes-bootcamp:v1" already present on machine
  Normal  Created         5m41s  kubelet, minikube  Created container kubernetes-bootcamp
  Normal  Started         5m37s  kubelet, minikube  Started container kubernetes-bootcamp
lyre@yonlli-LC2:~$ kubectl get pods -l app=v1
NAME                                  READY   STATUS    RESTARTS   AGE
kubernetes-bootcamp-b94cb9bff-jh54n   1/1     Running   1          73m
```

### Delete

```
lyre@yonlli-LC2:~$ kubectl get services
NAME         TYPE        CLUSTER-IP   EXTERNAL-IP   PORT(S)   AGE
kubernetes   ClusterIP   10.96.0.1    <none>        443/TCP   87m
lyre@yonlli-LC2:~$ curl $(minikube ip):$NODE_PORT
curl: (7) Failed to connect to 192.168.99.101 port 30754: Connection refused
lyre@yonlli-LC2:~$ kubectl exec -ti $POD_NAME curl localhost:8080
Hello Kubernetes bootcamp! | Running on: kubernetes-bootcamp-b94cb9bff-jh54n | v=1
```

## Scaling

![](https://d33wubrfki0l68.cloudfront.net/043eb67914e9474e30a303553d5a4c6c7301f378/0d8f6/docs/tutorials/kubernetes-basics/public/images/module_05_scaling1.svg)
![](https://d33wubrfki0l68.cloudfront.net/30f75140a581110443397192d70a4cdb37df7bfc/b5f56/docs/tutorials/kubernetes-basics/public/images/module_05_scaling2.svg)


### Scale up
```
lyre@yonlli-LC2:~$ kubectl scale deployments/kubernetes-bootcamp --replicas=4
deployment.extensions/kubernetes-bootcamp scaled

lyre@yonlli-LC2:~$ kubectl get deployments
NAME                  READY   UP-TO-DATE   AVAILABLE   AGE
kubernetes-bootcamp   1/4     4            1           85m

lyre@yonlli-LC2:~$ kubectl get pods -o wide
NAME                                  READY   STATUS    RESTARTS   AGE   IP           NODE       NOMINATED NODE   READINESS GATES
kubernetes-bootcamp-b94cb9bff-fm5wv   1/1     Running   0          47s   172.17.0.7   minikube   <none>           <none>
kubernetes-bootcamp-b94cb9bff-jh54n   1/1     Running   1          86m   172.17.0.4   minikube   <none>           <none>
kubernetes-bootcamp-b94cb9bff-p77ct   1/1     Running   0          48s   172.17.0.6   minikube   <none>           <none>
kubernetes-bootcamp-b94cb9bff-v29bj   1/1     Running   0          47s   172.17.0.8   minikube   <none>           <none>
```

### Load balancing

```
lyre@yonlli-LC2:~$ kubectl expose deployment/kubernetes-bootcamp --type="NodePort" --port 8080
service/kubernetes-bootcamp exposed
lyre@yonlli-LC2:~$ kubectl describe services/kubernetes-bootcamp
Name:                     kubernetes-bootcamp
Namespace:                default
Labels:                   run=kubernetes-bootcamp
Annotations:              <none>
Selector:                 run=kubernetes-bootcamp
Type:                     NodePort
IP:                       10.100.48.47
Port:                     <unset>  8080/TCP
TargetPort:               8080/TCP
NodePort:                 <unset>  31996/TCP
Endpoints:                172.17.0.4:8080,172.17.0.6:8080,172.17.0.7:8080 + 1 more...
Session Affinity:         None
External Traffic Policy:  Cluster
Events:                   <none>
lyre@yonlli-LC2:~$ export NODE_PORT=$(kubectl get services/kubernetes-bootcamp -o go-template='{{(index .spec.ports 0).nodePort}}')
lyre@yonlli-LC2:~$ echo NODE_PORT=$NODE_PORT
NODE_PORT=31996
lyre@yonlli-LC2:~$ curl $(minikube ip):$NODE_PORT
Hello Kubernetes bootcamp! | Running on: kubernetes-bootcamp-b94cb9bff-v29bj | v=1
lyre@yonlli-LC2:~$ curl $(minikube ip):$NODE_PORT
Hello Kubernetes bootcamp! | Running on: kubernetes-bootcamp-b94cb9bff-fm5wv | v=1
```

### Scale Down

```
lyre@yonlli-LC2:~$ kubectl scale deployments/kubernetes-bootcamp --replicas=2
deployment.extensions/kubernetes-bootcamp scaled
lyre@yonlli-LC2:~$ kubectl get deployments
NAME                  READY   UP-TO-DATE   AVAILABLE   AGE
kubernetes-bootcamp   2/2     2            2           94m
lyre@yonlli-LC2:~$ kubectl get pods -o wide
NAME                                  READY   STATUS        RESTARTS   AGE    IP           NODE       NOMINATED NODE   READINESS GATES
kubernetes-bootcamp-b94cb9bff-fm5wv   1/1     Terminating   0          9m7s   172.17.0.7   minikube   <none>           <none>
kubernetes-bootcamp-b94cb9bff-jh54n   1/1     Running       1          94m    172.17.0.4   minikube   <none>           <none>
kubernetes-bootcamp-b94cb9bff-p77ct   1/1     Running       0          9m8s   172.17.0.6   minikube   <none>           <none>
kubernetes-bootcamp-b94cb9bff-v29bj   1/1     Terminating   0          9m7s   172.17.0.8   minikube   <none>           <none>
```

## Pre Update

```
$ kubectl describe pods
Name:               kubernetes-bootcamp-b94cb9bff-jh54n
Namespace:          default
Priority:           0
PriorityClassName:  <none>
Node:               minikube/10.0.2.15
Start Time:         Wed, 22 May 2019 19:41:21 +0800
Labels:             app=v1
                    pod-template-hash=b94cb9bff
                    run=kubernetes-bootcamp
Annotations:        <none>
Status:             Running
IP:                 172.17.0.4
Controlled By:      ReplicaSet/kubernetes-bootcamp-b94cb9bff
Containers:
  kubernetes-bootcamp:
    Container ID:   docker://376b922ace328067e442351fb40326c1390d1c07f372e87124cabf57a4371112
    Image:          gcr.io/google-samples/kubernetes-bootcamp:v1 <----- here
...
```

### Update

```
lyre@yonlli-LC2:~$ kubectl set image deployments/kubernetes-bootcamp kubernetes-bootcamp=jocatalin/kubernetes-bootcamp:v2
deployment.extensions/kubernetes-bootcamp image updated
lyre@yonlli-LC2:~$ kubectl get pods
NAME                                   READY   STATUS              RESTARTS   AGE
kubernetes-bootcamp-64bfc9b489-tplxl   0/1     ContainerCreating   0          10s
kubernetes-bootcamp-b94cb9bff-jh54n    1/1     Running             1          102m
kubernetes-bootcamp-b94cb9bff-p77ct    1/1     Running             0          17m
```

Both old and new: old images were terminating

```
lyre@yonlli-LC2:~$ kubectl describe pods | grep Image:
    Image:          jocatalin/kubernetes-bootcamp:v2
    Image:          jocatalin/kubernetes-bootcamp:v2
    Image:          gcr.io/google-samples/kubernetes-bootcamp:v1
    Image:          gcr.io/google-samples/kubernetes-bootcamp:v1
```

```
lyre@yonlli-LC2:~$ curl $(minikube ip):$NODE_PORT
Hello Kubernetes bootcamp! | Running on: kubernetes-bootcamp-64bfc9b489-c8znb | v=2
```

### History

```
lyre@yonlli-LC2:~$ kubectl rollout status deployments/kubernetes-bootcamp
deployment "kubernetes-bootcamp" successfully rolled out


lyre@yonlli-LC2:~$ kubectl describe pods
Name:               kubernetes-bootcamp-64bfc9b489-c8znb
Namespace:          default
Priority:           0
PriorityClassName:  <none>
Node:               minikube/10.0.2.15
Start Time:         Wed, 22 May 2019 21:24:15 +0800
Labels:             pod-template-hash=64bfc9b489
                    run=kubernetes-bootcamp
Annotations:        <none>
Status:             Running
IP:                 172.17.0.8
Controlled By:      ReplicaSet/kubernetes-bootcamp-64bfc9b489
Containers:
  kubernetes-bootcamp:
    Container ID:   docker://216d0bafde0669c4e261811c42f5d70636c71c58c106bdb6b10017eb14c82541
    Image:          jocatalin/kubernetes-bootcamp:v2
    Image ID:       docker-pullable://jocatalin/kubernetes-bootcamp@sha256:fb1a3ced00cecfc1f83f18ab5cd14199e30adc1b49aa4244f5d65ad3f5feb2a5
    Port:           8080/TCP
    Host Port:      0/TCP
    State:          Running
      Started:      Wed, 22 May 2019 21:24:25 +0800
    Ready:          True
    Restart Count:  0
    Environment:    <none>
    Mounts:
      /var/run/secrets/kubernetes.io/serviceaccount from default-token-zc9jg (ro)
Conditions:
  Type              Status
  Initialized       True 
  Ready             True 
  ContainersReady   True 
  PodScheduled      True 
Volumes:
  default-token-zc9jg:
    Type:        Secret (a volume populated by a Secret)
    SecretName:  default-token-zc9jg
    Optional:    false
QoS Class:       BestEffort
Node-Selectors:  <none>
Tolerations:     node.kubernetes.io/not-ready:NoExecute for 300s
                 node.kubernetes.io/unreachable:NoExecute for 300s
Events:
  Type    Reason     Age    From               Message
  ----    ------     ----   ----               -------
  Normal  Scheduled  2m51s  default-scheduler  Successfully assigned default/kubernetes-bootcamp-64bfc9b489-c8znb to minikube
  Normal  Pulled     2m45s  kubelet, minikube  Container image "jocatalin/kubernetes-bootcamp:v2" already present on machine
  Normal  Created    2m42s  kubelet, minikube  Created container kubernetes-bootcamp
  Normal  Started    2m41s  kubelet, minikube  Started container kubernetes-bootcamp


Name:               kubernetes-bootcamp-64bfc9b489-tplxl
Namespace:          default
Priority:           0
PriorityClassName:  <none>
Node:               minikube/10.0.2.15
Start Time:         Wed, 22 May 2019 21:23:54 +0800
Labels:             pod-template-hash=64bfc9b489
                    run=kubernetes-bootcamp
Annotations:        <none>
Status:             Running
IP:                 172.17.0.7
Controlled By:      ReplicaSet/kubernetes-bootcamp-64bfc9b489
Containers:
  kubernetes-bootcamp:
    Container ID:   docker://940b6162f68996d0dd6faa17120887933d66b1ba9f9e1daf4bf4ae3f869134ab
    Image:          jocatalin/kubernetes-bootcamp:v2
    Image ID:       docker-pullable://jocatalin/kubernetes-bootcamp@sha256:fb1a3ced00cecfc1f83f18ab5cd14199e30adc1b49aa4244f5d65ad3f5feb2a5
    Port:           8080/TCP
    Host Port:      0/TCP
    State:          Running
      Started:      Wed, 22 May 2019 21:24:12 +0800
    Ready:          True
    Restart Count:  0
    Environment:    <none>
    Mounts:
      /var/run/secrets/kubernetes.io/serviceaccount from default-token-zc9jg (ro)
Conditions:
  Type              Status
  Initialized       True 
  Ready             True 
  ContainersReady   True 
  PodScheduled      True 
Volumes:
  default-token-zc9jg:
    Type:        Secret (a volume populated by a Secret)
    SecretName:  default-token-zc9jg
    Optional:    false
QoS Class:       BestEffort
Node-Selectors:  <none>
Tolerations:     node.kubernetes.io/not-ready:NoExecute for 300s
                 node.kubernetes.io/unreachable:NoExecute for 300s
Events:
  Type    Reason     Age    From               Message
  ----    ------     ----   ----               -------
  Normal  Scheduled  3m12s  default-scheduler  Successfully assigned default/kubernetes-bootcamp-64bfc9b489-tplxl to minikube
  Normal  Pulling    3m6s   kubelet, minikube  Pulling image "jocatalin/kubernetes-bootcamp:v2"
  Normal  Pulled     2m57s  kubelet, minikube  Successfully pulled image "jocatalin/kubernetes-bootcamp:v2"
  Normal  Created    2m55s  kubelet, minikube  Created container kubernetes-bootcamp
  Normal  Started    2m53s  kubelet, minikube  Started container kubernetes-bootcamp
```


### Rollback

Wrong image

```
lyre@yonlli-LC2:~$ kubectl set image deployments/kubernetes-bootcamp kubernetes-bootcamp=gcr.io/google-samples/kubernetes-bootcamp:v10
deployment.extensions/kubernetes-bootcamp image updated
lyre@yonlli-LC2:~$ kubectl get deployments
NAME                  READY   UP-TO-DATE   AVAILABLE   AGE
kubernetes-bootcamp   2/2     1            2           107m
lyre@yonlli-LC2:~$ kubectl get deployments
NAME                  READY   UP-TO-DATE   AVAILABLE   AGE
kubernetes-bootcamp   2/2     1            2           107m
lyre@yonlli-LC2:~$ kubectl get pods
NAME                                   READY   STATUS         RESTARTS   AGE
kubernetes-bootcamp-64bfc9b489-c8znb   1/1     Running        0          4m37s
kubernetes-bootcamp-64bfc9b489-tplxl   1/1     Running        0          4m57s
kubernetes-bootcamp-66ffd96587-88h8l   0/1     ErrImagePull   0          19s
```

```
lyre@yonlli-LC2:~$ kubectl rollout undo deployments/kubernetes-bootcamp
deployment.extensions/kubernetes-bootcamp rolled back
lyre@yonlli-LC2:~$ kubectl get pods
NAME                                   READY   STATUS    RESTARTS   AGE
kubernetes-bootcamp-64bfc9b489-c8znb   1/1     Running   0          6m10s
kubernetes-bootcamp-64bfc9b489-tplxl   1/1     Running   0          6m30s
```