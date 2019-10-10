# 理解Istio Gateway 

## Service Mesh 

在《API 网关 kong 实战》(https://cloud.tencent.com/developer/article/1477672)一文中，我们介绍了为什么要采用API网关“ 如果让每一个后台系统都实现鉴权、限流、负载均衡、审计等基础功能是不合适的，通用的做法是把这些功能抽离出来放到网关层。”这解决了外部请求访问服务的问题，但是微服务之间的内部调用也需要进行鉴权、限流、负载均衡，一种简单的解决方案是让微服务之间的调用也通过网关进行，然而这种方案有有很多问题：

1. 网关成为了一个单点，虽然我们可以通过横向扩展解决
2. 微服务之间的调用加重了网关的负载，这些流量*本来*可以不经过网关
3. 内部调用和外部请求的鉴权、限流等策略是相同的，无法根据内外部调用来区别对待

正如API网关是在流量入口处设置一个反向代理，service mesh的做法是为每一个微服务部署一个代理，这些代理通过iptables劫持所有进入服务的流量，完成鉴权、限流等步骤后再把请求发送到后端服务中，因此service mesh对应用程序来说是透明的。在Istio的实现中，这个代理采用了Envoy，通过Sidecar方式实现，如下图所示：


> ![](img/6-b.png)
> 
> 图片来自[Pattern: Service Mesh](https://philcalcado.com/2017/08/03/pattern_service_mesh.html)

多个微服务之间相互调用的时候，形成了服务网格(Service Mesh)，sidecar proxy和service之间的业务流量是数据面，而每个sidecar proxy还跟Control Plane相连，这是控制面，用于下发配置和规则。

> ![](img/mesh3.png)
> 
> 图片来自[Pattern: Service Mesh](https://philcalcado.com/2017/08/03/pattern_service_mesh.html)
