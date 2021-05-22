# 利用公有云上的Kubernetes集群为单点应用提供高可用

## 前言

Kubernetes作为一个容器编排系统，为我们提供了异常丰富的功能：服务发现、负载均衡、CI/CD、配置管理、故障自愈。作为曾经的高可用性（HA）集群的开发人员，笔者在研究Kubernets的调度策略时忽然意识到，Kubernetes同时也是一个HA集群，配合公有云提供的基础设施后，可以满足大部分可用性要求不是特别高的应用。

## 传统高可用集群

以一个最基本的双机部署为例，HA软件把Virutal IP和共享存储（SAN/NAS/iSCSI等)挂载到其中一台机器上，在这个机器上启动应用，这台机器称为Master。另一台称为Slave，上面的HA软件持续进行监控，当发现Master不正常时，通过STONITH (https://en.m.wikipedia.org/wiki/STONITH) 强制关闭Master，然后把Virtual IP和共享存储挂载到本地并启动应用，成为新的Master。一些变种包括：

- 不使用共享存储，而是通过DRBD之类的软件把数据从Master复制到Slave
- Slave上的应用也保持运行状态，但是不对外提供服务或者仅提供只读查询，在Master失败时进行状态切换，对外提供服务

## 云端高可用

虚拟化兴起之后，VMware 和 Oracle VM等虚拟化管理软件提供了代替方案：如果一个虚拟机或者它的宿主机故障了，直接在另一个节点重启这台虚拟机。

在云计算的时代，虚拟机已经下沉为一种基础设施，代替了原来的物理机，而它的位置则为容器所取代。同时分布式存储取代了传统的存储。最后，Load Balancer某种程度上代替了Virtual IP，但是这两者的功能是不太一样的，Load Balancer要求后端所有节点都能响应请求，而使用Virtual IP的场景是后端只有一个节点能响应请求。当后端有且只有一个节点的情况下，两者就没什么区别了。

下文以腾讯云为例，简要介绍如何通过公有云为一个单点的MariaDB提供高可用，使用的组件包括：

- 腾讯云容器服务（Tencent Kubernetes Engine ，TKE）
- 负载均衡（Cloud Load Balancer，CLB）
- 云硬盘（Cloud Block Storage，CBS）

## 创建PresistentVolumeClaim (PVC)

可以看到TKE提供了cbs的StorageClass：

```
# kubectl get sc
NAME            PROVISIONER                    AGE
cbs (default)   cloud.tencent.com/qcloud-cbs   57d
# kubectl describe sc/cbs
Name:                  cbs
IsDefaultClass:        Yes
Annotations:           storageclass.beta.kubernetes.io/is-default-class=true
Provisioner:           cloud.tencent.com/qcloud-cbs
Parameters:            type=cbs
AllowVolumeExpansion:  <unset>
MountOptions:          <none>
ReclaimPolicy:         Delete
VolumeBindingMode:     Immediate
Events:                <none>
```

cbs-pvc.yaml:
```
apiVersion: v1
kind: PersistentVolumeClaim
metadata:
  name: cbs-pvc1
spec:
  accessModes:
    - ReadWriteOnce
  volumeMode: Filesystem
  resources:
    requests:
      storage: 10Gi
  storageClassName: cbs
```

使用这个yaml文件创建PVC

```
# kubectl apply -f cbs-pvc.yaml 
persistentvolumeclaim/cbs-pvc1 created
# kubectl get pvc
NAME       STATUS    VOLUME   CAPACITY   ACCESS MODES   STORAGECLASS   AGE
cbs-pvc1   Pending  
# kubectl get pvc
NAME           STATUS    VOLUME                                     CAPACITY   ACCESS MODES   STORAGECLASS   AGE
cbs-pvc1       Bound     pvc-4bc5fe62-c32f-11e9-84d3-6afa4b99576d   50Gi       RWO            cbs            19s
```

注意，yaml文件中指定的大小是10Gi，我们实际得到的实际是50Gi，估计是个最小值。后续测试中笔者尝试创建一个51Gi的PVC，实际得到60Gi，猜测其分配策略按照10Gi为单位做了向上取整。另外`accessModes`设置为`ReadWriteOnce`，只允许一个节点以读写的方式挂载这个卷。

## 创建Deployment

mariadb-deployment.yaml
```
apiVersion: apps/v1
kind: Deployment
metadata:
  name: mariadb
  labels:
    app: mariadb
spec:
  selector:
    matchLabels:
      app: mariadb
  replicas: 1
  template:
    metadata:
      labels:
        app: mariadb
    spec:
      securityContext:
        fsGroup: 27
      containers:
      - name: mariadb-pod
        image: centos/mariadb
        ports:
        - containerPort: 3306
          protocol: TCP
        env:
        - name: MYSQL_ROOT_PASSWORD
          value: "123456"
        volumeMounts:
        - name: cbs-volume1
          mountPath:  /var/lib/mysql
          readOnly: false
      volumes:
      - name: cbs-volume1
        persistentVolumeClaim:
          claimName: cbs-pvc1
```

这个yaml文件中几个值得注意的点：

- 把我们刚创建的PVC声明为cbs-volume1，并挂载到container的/var/lib/mysql
- fsGroup：默认情况下docker在挂载卷时，mount point目录（如上文的/var/lib/mysql）的用户和组都是root，而`centos/mariadb`这个image默认是以`mysql`用户的身份运行的，会导致`MariaDB`初始化失败。这里把fsGroup设置为27，即mariab的group id，这样/var/lib/mysql的用户和组即为`root:mysql`.
- replicas = 1：只运行一个副本，并且前文中`accessModes:ReadWriteOnce`也限制了只有单个Pod能以读写方式挂载。
- env里面明文配置了数据库root密码，这**不是**一个好的做法，实践中请使用secret。

下面创建这个deployment： 

```
# kubectl apply -f mariadb-deployment.yaml 
deployment.apps/mariadb created
# kubectl get deployment mariadb
NAME      DESIRED   CURRENT   UP-TO-DATE   AVAILABLE   AGE
mariadb   1         1         1            1           42s
# kubectl get pods -o wide --selector=app=mariadb
NAME                       READY   STATUS    RESTARTS   AGE     IP             NODE            NOMINATED NODE
mariadb-788bbb8697-zlvf7   1/1     Running   0          6m11s   172.19.34.63   <NODE ID>       <none>
```

可以看到这个Deployment仅有一个Pod，其IP是172.19.34.63，并且/var/lib/msyql这个目录确实以mysql group创建：

```
# kubectl exec mariadb-788bbb8697-zlvf7 -- ls -ld /var/lib/mysql
drwxrwsr-x 6 root mysql 4096 Aug 23 14:45 /var/lib/mysql
```

## 创建Load Balancer

mariadb-service.yaml:

```
apiVersion: v1
kind: Service
metadata:
  name: mariadb
  annotations:
    service.kubernetes.io/qcloud-loadbalancer-internal-subnetid: <SUBNET-ID>
  labels:
    app: mariadb
spec:
  type: LoadBalancer
  selector:
    app: mariadb
  ports:
  - port: 3306
    targetPort: 3306
```

笔者使用的网络是腾讯云的VPC，通过annotations中的`service.kubernetes.io/qcloud-loadbalancer-internal-subnetid`，在指定的VPC内网中创建Load Balancer。在不指定该选项的情况下，会创建公网的Load Balancer。详情请参考TKE的Service管理 (https://cloud.tencent.com/document/product/457/31710)。

```
# kubectl apply -f mariadb-service.yaml 
service/mariadb created
# kubectl get service --selector=app=mariadb
NAME      TYPE           CLUSTER-IP     EXTERNAL-IP     PORT(S)          AGE
mariadb   LoadBalancer   172.19.63.54   10.0.0.1   3306:30929/TCP   22s
```

这里创建了一个EXTERNAL-IP，为保密起见，我已经替换成10.0.0.1；同时Endpoint指向172.19.34.63，刚创建的Node的IP。

```
# kubectl describe service mariadb
Name:                     mariadb
Namespace:                default
Labels:                   app=mariadb
Annotations:              kubectl.kubernetes.io/last-applied-configuration:
                            {"apiVersion":"v1","kind":"Service","metadata":{"annotations":{"service.kubernetes.io/qcloud-loadbalancer-internal-subnetid":"subnet-<SUBNET>...
                          service.kubernetes.io/qcloud-loadbalancer-internal-subnetid: subnet-<SUBNET>
Selector:                 app=mariadb
Type:                     LoadBalancer
IP:                       172.19.63.54
LoadBalancer Ingress:     10.0.0.1
Port:                     <unset>  3306/TCP
TargetPort:               3306/TCP
NodePort:                 <unset>  30929/TCP
Endpoints:                172.19.34.63:3306
Session Affinity:         None
External Traffic Policy:  Cluster
Events:
  Type    Reason                Age   From                           Message
  ----    ------                ----  ----                           -------
  Normal  EnsuringLoadBalancer  11m   service-controller-yunapi_clb  Ensuring load balancer
  Normal  EnsuredLoadBalancer   10m   service-controller-yunapi_clb  Ensured load balancer
```

## 写入测试数据

```
# mysql -h 10.0.0.1 -u root -p
Enter password: 
Welcome to the MariaDB monitor.  Commands end with ; or \g.
Your MariaDB connection id is 3
Server version: 5.5.60-MariaDB MariaDB Server

Copyright (c) 2000, 2018, Oracle, MariaDB Corporation Ab and others.

Type 'help;' or '\h' for help. Type '\c' to clear the current input statement.

MariaDB [(none)]> create database pet;
Query OK, 1 row affected (0.00 sec)

MariaDB [(none)]> use pet;
Database changed
MariaDB [pet]> CREATE TABLE test (name VARCHAR(10), owner VARCHAR(10),  species VARCHAR(10), birth DATE, death DATE);
Query OK, 0 rows affected (0.01 sec)

MariaDB [pet]> INSERT INTO test VALUES('dove', 'me', 'cat', now(), now());
Query OK, 1 row affected, 2 warnings (0.00 sec)

MariaDB [pet]> select * from test;
+------+-------+---------+------------+------------+
| name | owner | species | birth      | death      |
+------+-------+---------+------------+------------+
| dove | me    | cat     | 2019-08-20 | 2019-08-20 |
+------+-------+---------+------------+------------+
1 row in set (0.00 sec)

MariaDB [pet]> Bye
```

## Failover测试


测试很简单，直接把原来的mariadb pod删掉，k8s会重新创建这个pod，在这个过程中观察数据库服务中断的时间。

打开一个新的终端，执行下列命令观察：

```
while true; do date; echo 'select * from test' | mysql -h 10.0.0.1 -u root -p123456 pet; sleep 1; done
```

在另一个终端中删除这个pod：
 
```
# date; kubectl delete pods mariadb-788bbb8697-zlvf7; date
Fri Aug 23 23:12:57 CST 2019
pod "mariadb-788bbb8697-zlvf7" deleted
Fri Aug 23 23:13:28 CST 2019
```

可以看到从发起删除到实际删除，大约用了30秒，刚好是kubernetes的grace period，观察另一个终端的输出可以发现这段时间mariadb是不可用的：

```
name	owner	species	birth	death
dove	me	cat	2019-08-20	2019-08-20
Fri Aug 23 23:12:58 CST 2019
ERROR 2003 (HY000): Can't connect to MySQL server on '10.0.0.1' (111)

...

Fri Aug 23 23:13:28 CST 2019
ERROR 2003 (HY000): Can't connect to MySQL server on '10.0.0.1' (111)
Fri Aug 23 23:13:29 CST 2019
name	owner	species	birth	death
dove	me	cat	2019-08-20	2019-08-20
```

检查ReplicaSet的日志会发现它自动重新创建了一个Pod

```
# kubectl get rs --selector=app=mariadb
NAME                 DESIRED   CURRENT   READY   AGE
mariadb-788bbb8697   1         1         1       37m
# kubectl describe rs mariadb-788bbb8697
Name:           mariadb-788bbb8697
Namespace:      default
Selector:       app=mariadb,pod-template-hash=788bbb8697
Labels:         app=mariadb
                pod-template-hash=788bbb8697
Annotations:    deployment.kubernetes.io/desired-replicas: 1
                deployment.kubernetes.io/max-replicas: 2
                deployment.kubernetes.io/revision: 1
Controlled By:  Deployment/mariadb
Replicas:       1 current / 1 desired
Pods Status:    1 Running / 0 Waiting / 0 Succeeded / 0 Failed
Pod Template:
  Labels:  app=mariadb
           pod-template-hash=788bbb8697
  Containers:
   mariadb-pod:
    Image:      centos/mariadb
    Port:       3306/TCP
    Host Port:  0/TCP
    Environment:
      MYSQL_ROOT_PASSWORD:  123456
    Mounts:
      /var/lib/mysql from cbs-volume1 (rw)
  Volumes:
   cbs-volume1:
    Type:       PersistentVolumeClaim (a reference to a PersistentVolumeClaim in the same namespace)
    ClaimName:  cbs-pvc1
    ReadOnly:   false
Events:
  Type    Reason            Age    From                   Message
  ----    ------            ----   ----                   -------
  Normal  SuccessfulCreate  38m    replicaset-controller  Created pod: mariadb-788bbb8697-zlvf7
  Normal  SuccessfulCreate  7m47s  replicaset-controller  Created pod: mariadb-788bbb8697-5z597
```


## Failover测试2

上面的测试未必真实体现了实际的场景，因为我们手工命令Kubernetes删除一个Pod，可能影响ReplicaSet内部的逻辑，一种可能是删除Pod的过程中不自动创建。这次我们尝试直接用docker kill掉MariaDB的容器：

```
# docker ps | grep -i mariadb
945486909f9f        centos/mariadb                                           "/docker-entrypoint.…"   10 minutes ago      Up 10 minutes                           k8s_mariadb-pod_mariadb-788bbb8697-5z597_default_7d08caab-c5b8-11e9-b5a9-6afa4b99576d_0
28caeadb798c        ccr.ccs.tencentyun.com/library/pause:latest              "/pause"                 10 minutes ago      Up 10 minutes                           k8s_POD_mariadb-788bbb8697-5z597_default_7d08caab-c5b8-11e9-b5a9-6afa4b99576d_0
# docker kill 945486909f9f
```

这次我们看到通过Load Balancer的请求没有失败，只是有一个14秒的延迟

```
Fri Aug 23 23:23:43 CST 2019
name	owner	species	birth	death
dove	me	cat	2019-08-20	2019-08-20
Fri Aug 23 23:23:44 CST 2019
name	owner	species	birth	death
dove	me	cat	2019-08-20	2019-08-20
Fri Aug 23 23:23:58 CST 2019
name	owner	species	birth	death
dove	me	cat	2019-08-20	2019-08-20
Fri Aug 23 23:23:59 CST 2019
name	owner	species	birth	death
dove	me	cat	2019-08-20	2019-08-20
```

## 关于作者

李勇，Skybility HA 开发、Oracle Linux和Oracle VM技术支持、Oracle 公有云 SRE、腾讯云分布式存储CBS运维、运营系统开发、BUG制造者、CPU0杀手。