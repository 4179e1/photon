 curl -L https://git.io/getLatestIstio | ISTIO_VERSION=1.3.0 sh -

 cd istio-1.3.0
 export PATH=$PWD/bin:$PATH

 # kubectl api-resources | grep istio
meshpolicies                                   authentication.istio.io        false        MeshPolicy
policies                                       authentication.istio.io        true         Policy
adapters                                       config.istio.io                true         adapter
attributemanifests                             config.istio.io                true         attributemanifest
handlers                                       config.istio.io                true         handler
httpapispecbindings                            config.istio.io                true         HTTPAPISpecBinding
httpapispecs                                   config.istio.io                true         HTTPAPISpec
instances                                      config.istio.io                true         instance
quotaspecbindings                              config.istio.io                true         QuotaSpecBinding
quotaspecs                                     config.istio.io                true         QuotaSpec
rules                                          config.istio.io                true         rule
templates                                      config.istio.io                true         template
destinationrules                  dr           networking.istio.io            true         DestinationRule
envoyfilters                                   networking.istio.io            true         EnvoyFilter
gateways                          gw           networking.istio.io            true         Gateway
serviceentries                    se           networking.istio.io            true         ServiceEntry
sidecars                                       networking.istio.io            true         Sidecar
virtualservices                   vs           networking.istio.io            true         VirtualService
authorizationpolicies                          rbac.istio.io                  true         AuthorizationPolicy
clusterrbacconfigs                             rbac.istio.io                  false        ClusterRbacConfig
rbacconfigs                                    rbac.istio.io                  true         RbacConfig
servicerolebindings                            rbac.istio.io                  true         ServiceRoleBinding
serviceroles                                   rbac.istio.io                  true         ServiceRole

# kubectl api-versions | grep istio
authentication.istio.io/v1alpha1
config.istio.io/v1alpha2
networking.istio.io/v1alpha3
rbac.istio.io/v1alpha1


```
kubectl apply -f install/kubernetes/istio-demo.yaml
```

```
# kubectl get svc -n istio-system
NAME                     TYPE           CLUSTER-IP      EXTERNAL-IP     PORT(S)                                                                                                                                      AGE
grafana                  ClusterIP      172.19.63.63    <none>          3000/TCP                                                                                                                                     5m45s
istio-citadel            ClusterIP      172.19.63.134   <none>          8060/TCP,15014/TCP                                                                                                                           5m45s
istio-egressgateway      ClusterIP      172.19.63.167   <none>          80/TCP,443/TCP,15443/TCP                                                                                                                     5m45s
istio-galley             ClusterIP      172.19.63.242   <none>          443/TCP,15014/TCP,9901/TCP                                                                                                                   5m45s
istio-ingressgateway     LoadBalancer   172.19.63.42    111.230.0.236   15020:31098/TCP,80:31380/TCP,443:31390/TCP,31400:31400/TCP,15029:31145/TCP,15030:30386/TCP,15031:30093/TCP,15032:32132/TCP,15443:30576/TCP   5m45s
istio-pilot              ClusterIP      172.19.63.113   <none>          15010/TCP,15011/TCP,8080/TCP,15014/TCP                                                                                                       5m45s
istio-policy             ClusterIP      172.19.63.25    <none>          9091/TCP,15004/TCP,15014/TCP                                                                                                                 5m45s
istio-sidecar-injector   ClusterIP      172.19.63.219   <none>          443/TCP,15014/TCP                                                                                                                            5m45s
istio-telemetry          ClusterIP      172.19.63.163   <none>          9091/TCP,15004/TCP,15014/TCP,42422/TCP                                                                                                       5m45s
jaeger-agent             ClusterIP      None            <none>          5775/UDP,6831/UDP,6832/UDP                                                                                                                   5m44s
jaeger-collector         ClusterIP      172.19.63.194   <none>          14267/TCP,14268/TCP                                                                                                                          5m44s
jaeger-query             ClusterIP      172.19.63.95    <none>          16686/TCP                                                                                                                                    5m44s
kiali                    ClusterIP      172.19.63.143   <none>          20001/TCP                                                                                                                                    5m45s
prometheus               ClusterIP      172.19.63.156   <none>          9090/TCP                                                                                                                                     5m45s
tracing                  ClusterIP      172.19.63.222   <none>          80/TCP                                                                                                                                       5m44s
zipkin                   ClusterIP      172.19.63.118   <none>          9411/TCP                                                                                                                                     5m44s
```

vim install/kubernetes/istio-demo.yaml

add annotation


```
kubectl apply -f install/kubernetes/istio-demo.yaml
```

```
# kubectl get svc -n istio-system
NAME                     TYPE           CLUSTER-IP      EXTERNAL-IP     PORT(S)                                                                                                                                      AGE
grafana                  ClusterIP      172.19.63.63    <none>          3000/TCP                                                                                                                                     12m
istio-citadel            ClusterIP      172.19.63.134   <none>          8060/TCP,15014/TCP                                                                                                                           12m
istio-egressgateway      ClusterIP      172.19.63.167   <none>          80/TCP,443/TCP,15443/TCP                                                                                                                     12m
istio-galley             ClusterIP      172.19.63.242   <none>          443/TCP,15014/TCP,9901/TCP                                                                                                                   12m
istio-ingressgateway     LoadBalancer   172.19.63.164   9.138.221.176   15020:32276/TCP,80:31380/TCP,443:31390/TCP,31400:31400/TCP,15029:30824/TCP,15030:31844/TCP,15031:32188/TCP,15032:31863/TCP,15443:31540/TCP   4m44s
istio-pilot              ClusterIP      172.19.63.113   <none>          15010/TCP,15011/TCP,8080/TCP,15014/TCP                                                                                                       12m
istio-policy             ClusterIP      172.19.63.25    <none>          9091/TCP,15004/TCP,15014/TCP                                                                                                                 12m
istio-sidecar-injector   ClusterIP      172.19.63.219   <none>          443/TCP,15014/TCP                                                                                                                            12m
istio-telemetry          ClusterIP      172.19.63.163   <none>          9091/TCP,15004/TCP,15014/TCP,42422/TCP                                                                                                       12m
jaeger-agent             ClusterIP      None            <none>          5775/UDP,6831/UDP,6832/UDP                                                                                                                   12m
jaeger-collector         ClusterIP      172.19.63.194   <none>          14267/TCP,14268/TCP                                                                                                                          12m
jaeger-query             ClusterIP      172.19.63.95    <none>          16686/TCP                                                                                                                                    12m
kiali                    ClusterIP      172.19.63.143   <none>          20001/TCP                                                                                                                                    12m
prometheus               ClusterIP      172.19.63.156   <none>          9090/TCP                                                                                                                                     12m
tracing                  ClusterIP      172.19.63.222   <none>          80/TCP                                                                                                                                       12m
zipkin                   ClusterIP      172.19.63.118   <none>          9411/TCP                                                                                                                                     12m
```

```
# kubectl get pods -n istio-system
NAME                                      READY   STATUS      RESTARTS   AGE
grafana-686db98dbc-7l62j                  1/1     Running     0          13m
istio-citadel-6465cbf6f4-nm8kl            1/1     Running     0          13m
istio-cleanup-secrets-1.3.0-dnv9w         0/1     Completed   0          13m
istio-egressgateway-7df9c9cb88-lr8f8      1/1     Running     0          13m
istio-galley-f8b55cd5d-nvvph              1/1     Running     0          13m
istio-grafana-post-install-1.3.0-qphk4    0/1     Completed   0          13m
istio-ingressgateway-54f6c8db59-tp8jl     1/1     Running     0          13m
istio-pilot-7b44d85b9d-wdswk              2/2     Running     1          13m
istio-policy-5c445966b5-6d9rr             2/2     Running     8          13m
istio-security-post-install-1.3.0-xrbrk   0/1     Completed   0          13m
istio-sidecar-injector-6b7676f447-bvpj8   1/1     Running     0          13m
istio-telemetry-85f855bf47-h9fjv          2/2     Running     8          13m
istio-tracing-7666b8d688-2dhpb            1/1     Running     0          13m
kiali-679b495b5-hqrts                     1/1     Running     0          13m
prometheus-6d757cd447-qwqjh               1/1     Running     0          13m
```

$ kubectl label namespace <namespace> istio-injection=enabled
$ kubectl create -n <namespace> -f <your-app-spec>.yaml

```
[root@VM_221_117_centos ~/istio-1.3.0]# kubectl label namespace default istio-injection=enabled
namespace/default labeled
[root@VM_221_117_centos ~/istio-1.3.0]# kubectl get namespace --show-labels
NAME           STATUS   AGE   LABELS
default        Active   90d   istio-injection=enabled
istio-system   Active   23m   istio-injection=disabled
kube-public    Active   90d   <none>
kube-system    Active   90d   <none>
```

alternatively:
$ istioctl kube-inject -f <your-app-spec>.yaml | kubectl apply -f -


```
# kubectl apply -f samples/bookinfo/platform/kube/bookinfo.yaml
service/details created
serviceaccount/bookinfo-details created
deployment.apps/details-v1 created
service/ratings created
serviceaccount/bookinfo-ratings created
deployment.apps/ratings-v1 created
service/reviews created
serviceaccount/bookinfo-reviews created
deployment.apps/reviews-v1 created
deployment.apps/reviews-v2 created
deployment.apps/reviews-v3 created
service/productpage created
serviceaccount/bookinfo-productpage created
deployment.apps/productpage-v1 created
```


```
[root@VM_221_117_centos ~/istio-1.3.0]# export INGRESS_HOST=$(kubectl -n istio-system get service istio-ingressgateway -o jsonpath='{.status.loadBalancer.ingress[0].ip}')
[root@VM_221_117_centos ~/istio-1.3.0]# export INGRESS_PORT=$(kubectl -n istio-system get service istio-ingressgateway -o jsonpath='{.spec.ports[?(@.name=="http2")].port}')
[root@VM_221_117_centos ~/istio-1.3.0]# export SECURE_INGRESS_PORT=$(kubectl -n istio-system get service istio-ingressgateway -o jsonpath='{.spec.ports[?(@.name=="https")].port}')
[root@VM_221_117_centos ~/istio-1.3.0]# export GATEWAY_URL=$INGRESS_HOST:$INGRESS_PORT
[root@VM_221_117_centos ~/istio-1.3.0]# echo $GATEWAY_URL
9.138.221.176:80
```

```
[root@VM_221_117_centos ~/istio-1.3.0]# cat samples/bookinfo/networking/bookinfo-gateway.yaml 
apiVersion: networking.istio.io/v1alpha3
kind: Gateway
metadata:
  name: bookinfo-gateway
spec:
  selector:
    istio: ingressgateway # use istio default controller , apply to pods with this label: kubectl get pods -n istio-system -l istio=ingressgateway
  servers:
  - port:
      number: 80    # port in the service
      name: http
      protocol: HTTP
    hosts:
    - "*"
---
apiVersion: networking.istio.io/v1alpha3
kind: VirtualService
metadata:
  name: bookinfo
spec:
  hosts:
  - "*"     # <=== 网关匹配的Virtual Host
  gateways:
  - bookinfo-gateway
  http:
  - match:
    - uri:
        exact: /productpage
    - uri:
        prefix: /static
    - uri:
        exact: /login
    - uri:
        exact: /logout
    - uri:
        prefix: /api/v1/products
    route:
    - destination:
        host: productpage   # <==== Kubernetes 的 service
        port:
          number: 9080
[root@VM_221_117_centos ~/istio-1.3.0]# kubectl apply -f samples/bookinfo/networking/bookinfo-gateway.yaml
gateway.networking.istio.io/bookinfo-gateway created
virtualservice.networking.istio.io/bookinfo created
```

```
# cat samples/bookinfo/networking/destination-rule-all.yaml
apiVersion: networking.istio.io/v1alpha3
kind: DestinationRule
metadata:
  name: productpage
spec:
  host: productpage
  subsets:
  - name: v1
    labels:
      version: v1
---
apiVersion: networking.istio.io/v1alpha3
kind: DestinationRule
metadata:
  name: reviews
spec:
  host: reviews
  subsets:
  - name: v1
    labels:
      version: v1
  - name: v2
    labels:
      version: v2
  - name: v3
    labels:
      version: v3
---
apiVersion: networking.istio.io/v1alpha3
kind: DestinationRule
metadata:
  name: ratings
spec:
  host: ratings
  subsets:
  - name: v1
    labels:
      version: v1
  - name: v2
    labels:
      version: v2
  - name: v2-mysql
    labels:
      version: v2-mysql
  - name: v2-mysql-vm
    labels:
      version: v2-mysql-vm
---
apiVersion: networking.istio.io/v1alpha3
kind: DestinationRule
metadata:
  name: details
spec:
  host: details
  subsets:
  - name: v1
    labels:
      version: v1
  - name: v2
    labels:
      version: v2
---
```

```
# kubectl apply -f samples/bookinfo/networking/destination-rule-all.yaml
destinationrule.networking.istio.io/productpage created
destinationrule.networking.istio.io/reviews created
destinationrule.networking.istio.io/ratings created
destinationrule.networking.istio.io/details created
```

```
# kubectl get destinationrules 
NAME          HOST          AGE
details       details       1m
productpage   productpage   1m
ratings       ratings       1m
reviews       reviews       1m
```