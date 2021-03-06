
## relase-1.0



参见： 

* [Istio_Announcing Istio 1.0.html](_resource/Istio_Announcing Istio 1.0.html)



### 1.0.1

We’re pleased to announce the availability of Istio 1.0.1. Please see below for what’s changed.



**Networking**

- Improved Pilot scalability and Envoy startup time.
- Fixed virtual service host mismatch issue when adding a port.
- Added limited support for [merging multiple virtual service or destination rule definitions](https://istio.io/latest/docs/ops/best-practices/traffic-management/#split-virtual-services) for the same host.
- Allow [outlier](https://www.envoyproxy.io/docs/envoy/latest/api-v2/api/v2/cluster/outlier_detection.proto) consecutive gateway failures when using HTTP.

**Environment**

- Made it possible to use Pilot standalone, for those users who want to only leverage Istio’s traffic management functionality.
- Introduced the convenient `values-istio-gateway.yaml` configuration that enables users to run standalone gateways.
- Fixed a variety of Helm installation issues, including an issue with the `istio-sidecar-injector` configmap not being found.
- Fixed the Istio installation error with Galley not being ready.
- Fixed a variety of issues around mesh expansion.

**Policy and telemetry**

- Added an experimental metrics expiration configuration to the Mixer Prometheus adapter.
- Updated Grafana to 5.2.2.

**Adapters**

- Ability to specify sink options for the Stackdriver adapter.

**Galley**

- Improved configuration validation for health checks.



### 1.0.2

We’re pleased to announce the availability of Istio 1.0.2. Please see below for what’s changed.



**General**

- Fixed bug in Envoy where the sidecar would crash if receiving normal traffic on the mutual TLS port.
- Fixed bug with Pilot propagating incomplete updates to Envoy in a multicluster environment.
- Added a few more Helm options for Grafana.
- Improved Kubernetes service registry queue performance.
- Fixed bug where `istioctl proxy-status` was not showing the patch version.
- Add validation of virtual service SNI hosts.



### 1.0.3



We’re pleased to announce the availability of Istio 1.0.3. Please see below for what’s changed.



**Behavior changes**

- [Validating webhook](https://istio.io/latest/docs/ops/common-problems/validation) is now mandatory. Disabling it may result in Pilot crashes.
- [Service entry](https://istio.io/latest/docs/reference/config/networking/service-entry/) validation now rejects the wildcard hostname (`*`) when configuring DNS resolution. The API has never allowed this, however `ServiceEntry` was erroneously excluded from validation in the previous release. Use of wildcards as part of a hostname, e.g. `*.bar.com`, remains unchanged.
- The core dump path for `istio-proxy` has changed to `/var/lib/istio`.

**Networking**

- Mutual TLS Permissive mode is enabled by default.
- Pilot performance and scalability has been greatly enhanced. Pilot now delivers endpoint updates to 500 sidecars in under 1 second.
- Default [trace sampling](https://istio.io/latest/docs/tasks/observability/distributed-tracing/configurability/#trace-sampling) is set to 1%.

**Policy and telemetry**

- Mixer (`istio-telemetry`) now supports load shedding based on request rate and expected latency.
- Mixer client (`istio-policy`) now supports `FAIL_OPEN` setting.
- Istio Performance dashboard added to Grafana.
- Reduced `istio-telemetry` CPU usage by 10%.
- Eliminated `statsd-to-prometheus` deployment. Prometheus now directly scrapes from `istio-proxy`.





### 1.0.4



We’re pleased to announce the availability of Istio 1.0.4. Please see below for what’s changed.



**Known issues**

- Pilot may deadlock when using [`istioctl proxy-status`](https://istio.io/latest/docs/reference/commands/istioctl/#istioctl-proxy-status) to get proxy synchronization status. The work around is to *not use* `istioctl proxy-status`. Once Pilot enters a deadlock, it exhibits continuous memory growth eventually running out of memory.

**Networking**

- Fixed the lack of removal of stale endpoints causing `503` errors.
- Fixed sidecar injection when a pod label contains a `/`.

**Policy and telemetry**

- Fixed occasional data corruption problem with out-of-process Mixer adapters leading to incorrect behavior.
- Fixed excessive CPU usage by Mixer when waiting for missing CRDs.



### 1.0.5

We’re pleased to announce the availability of Istio 1.0.5. Please see below for what’s changed.



**General**

- Disabled the precondition cache in the `istio-policy` service as it lead to invalid results. The cache will be reintroduced in a later release.
- Mixer now only generates spans when there is an enabled `tracespan` adapter, resulting in lower CPU overhead in normal cases.
- Fixed a problem that could lead Pilot to hang.



### 1.0.6

We’re pleased to announce the availability of Istio 1.0.6. Please see below for what’s changed.



**Bug fixes**

- Fix Galley Helm charts so that the `validatingwebhookconfiguration` object can now deployed to a namespace other than `istio-system` ([Issue 13625](https://github.com/istio/istio/issues/13625)).

- Additional Helm chart fixes for anti-affinity support: fix `gatewaypodAntiAffinityRequiredDuringScheduling` and `podAntiAffinityLabelSelector` match expressions and fix the default value for `podAntiAffinityLabelSelector` ([Issue 13892](https://github.com/istio/istio/issues/13892)).

- Make Pilot handle a condition where Envoy continues to request routes for a deleted gateway while listeners are still draining ([Issue 13739](https://github.com/istio/istio/issues/13739)).

  > yonka: `buildGatewayHTTPRouteConfig` 里原逻辑是 如果请求的route（name）没有对应的路由（比如此case中的被删除），那么就会报错返回err，这样会导致push失败。
  >
  > 修复方式是，增加`DisablePartialRouteResponse`参数允许调整行为为 跳过不存在的路由继续build其他 。

  

**Small enhancements**

- If access logs are enabled, `passthrough` listener requests will be logged.

- Make Pilot tolerate unknown JSON fields to make it easier to rollback to older versions during upgrade.

- Add support for fallback secrets to `SDS` which Envoy can use instead of waiting indefinitely for late or non-existent secrets during startup ([Issue 13853](https://github.com/istio/istio/issues/13853)).

  > yonka: 改动较多，没看出来





### 1.0.7

We’re announcing immediate availability of Istio 1.0.7 which contains some important security updates. Please see below for details.

**Security update**

Two security vulnerabilities have recently been identified in the Envoy proxy ([CVE 2019-9900](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-9900) and [CVE 2019-9901](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-9901)). The vulnerabilities have now been patched in Envoy version 1.9.1, and correspondingly in the Envoy builds embedded in Istio 1.1.2 and Istio 1.0.7. Since Envoy is an integral part of Istio, users are advised to update Istio immediately to mitigate security risks arising from these vulnerabilities.

The vulnerabilities are centered on the fact that Envoy did not normalize HTTP URI paths and did not fully validate HTTP/1.1 header values. These vulnerabilities impact Istio features that rely on Envoy to enforce any of authorization, routing, or rate limiting.

> yonka: 怀疑就是没对入口header做检查，导致入口流量可以自行制定特殊header于是绕过系统内限制...

**Affected Istio releases**

The following Istio releases are vulnerable:

- 1.1, 1.1.1
  - These releases can be patched to Istio 1.1.2.
  - 1.1.2 is built from the same source as 1.1.1 with the addition of Envoy patches minimally sufficient to address the CVEs.
- 1.0, 1.0.1, 1.0.2, 1.0.3, 1.0.4, 1.0.5, 1.0.6
  - These releases can be patched to Istio 1.0.7
  - 1.0.7 is built from the same source as 1.0.6 with the addition of Envoy patches minimally sufficient to address the CVEs.
- 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8
  - These releases are no longer supported and will not be patched. Please upgrade to a supported release with the necessary fixes.

**Vulnerability impact**

[CVE 2019-9900](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-9900) and [CVE 2019-9901](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-9901) allow remote attackers access to unauthorized resources by using specially crafted request URI paths (9901) and NUL bytes in HTTP/1.1 headers (9900), potentially circumventing DoS prevention systems such as rate limiting, or routing to a unexposed upstream system. Refer to [issue 6434](https://github.com/envoyproxy/envoy/issues/6434) and [issue 6435](https://github.com/envoyproxy/envoy/issues/6435) for more information.

As Istio is based on Envoy, Istio customers can be affected by these vulnerabilities based on whether paths and request headers are used within Istio policies or routing rules and how the backend HTTP implementation resolves them. If prefix path matching rules are used by Mixer or by Istio authorization policies or the routing rules, an attacker could exploit these vulnerabilities to gain access to unauthorized paths on certain HTTP backends.

**Mitigation**

Eliminating the vulnerabilities requires updating to a corrected version of Envoy. We’ve incorporated the necessary updates in the latest Istio patch releases.

For Istio 1.1.x deployments: update to a minimum of [Istio 1.1.2](https://istio.io/latest/news/releases/1.1.x/announcing-1.1.2)

For Istio 1.0.x deployments: update to a minimum of [Istio 1.0.7](https://istio.io/latest/news/releases/1.0.x/announcing-1.0.7)

While Envoy 1.9.1 requires opting in to path normalization to address CVE 2019-9901, the version of Envoy embedded in Istio 1.1.2 and 1.0.7 enables path normalization by default.

**Detection of NUL header exploit**

Based on current information, this only affects HTTP/1.1 traffic. If this is not structurally possible in your network or configuration, then it is unlikely that this vulnerability applies.

File-based access logging uses the `c_str()` representation for header values, as does gRPC access logging, so there will be no trivial detection via Envoy’s access logs by scanning for NUL. Instead, operators might look for inconsistencies in logs between the routing that Envoy performs and the logic intended in the `RouteConfiguration`.

External authorization and rate limit services can check for NULs in headers. Backend servers might have sufficient logging to detect NULs or unintended access; it’s likely that many will simply reject NULs in this scenario via 400 Bad Request, as per RFC 7230.

**Detection of path traversal exploit**

Envoy’s access logs (whether file-based or gRPC) will contain the unnormalized path, so it is possible to examine these logs to detect suspicious patterns and requests that are incongruous with the intended operator configuration intent. In addition, unnormalized paths are available at `ext_authz`, rate limiting and backend servers for log inspection.



### 1.0.8



We’re pleased to announce the availability of Istio 1.0.8. Please see below for what’s changed.



**Bug fixes**

- Fix issue where Citadel could generate a new root CA if it cannot contact the Kubernetes API server, causing mutual TLS verification to incorrectly fail ([Issue 14512](https://github.com/istio/istio/issues/14512)).

**Small enhancements**

- Update Citadel’s default root CA certificate TTL from 1 year to 10 years.



### 1.0.9

We’re pleased to announce the availability of Istio 1.0.9. Please see below for what’s changed.



**Bug fixes**

- Fix crash in Istio’s JWT Envoy filter caused by malformed JWT ([Issue 15084](https://github.com/istio/istio/issues/15084)).

  > yonka: https://github.com/istio/proxy/pull/2291/files




## relase-1.1



参见： 

* [Istio_Change Notes_1.1.html](_resource/Istio_Change Notes_1.1.html)

* [Istio_Upgrade Notes_1.1.html](_resource/Istio_Upgrade Notes_1.1.html)

* [Istio_Helm Changes_1.1.html](_resource/Istio_Helm Changes_1.1.html)



### 1.1.1

We’re pleased to announce the availability of Istio 1.1.1. Please see below for what’s changed.

**Bug fixes and minor enhancements**

- Configure Prometheus to monitor Citadel ([Issue 12175](https://github.com/istio/istio/pull/12175))

  > yonka: prometheus的configmap里加了一个job
  >
  > ```yaml
  >     - job_name: 'citadel'
  >       kubernetes_sd_configs:
  >       - role: endpoints
  >         namespaces:
  >           names:
  >           - {{ .Release.Namespace }}
  >       relabel_configs:
  >       - source_labels: [__meta_kubernetes_service_name, __meta_kubernetes_endpoint_port_name]
  >         action: keep
  >         regex: istio-citadel;http-monitoring
  > ```

- Improve output of [`istioctl verify-install`](https://istio.io/latest/docs/reference/commands/istioctl/#istioctl-verify-install) command ([Issue 12174](https://github.com/istio/istio/pull/12174))

- Reduce log level for missing service account messages for a SPIFFE URI ([Issue 12108](https://github.com/istio/istio/issues/12108))

- Fix broken path on the opt-in SDS feature’s Unix domain socket ([Issue 12688](https://github.com/istio/istio/pull/12688))

- Fix Envoy tracing that was preventing a child span from being created if the parent span was propagated with an empty string ([Envoy Issue 6263](https://github.com/envoyproxy/envoy/pull/6263))

- Add namespace scoping to the Gateway ‘port’ names. This fixes two issues:
  - `IngressGateway` only respects first port 443 Gateway definition ([Issue 11509](https://github.com/istio/istio/issues/11509))
  - Istio `IngressGateway` routing broken with two different gateways with same port name (SDS) ([Issue 12500](https://github.com/istio/istio/issues/12500))
  
  > yonka: https://github.com/istio/istio/pull/12556
  >
  > 背景： 
  >
  > * 有时网关会作为https终结，那么可能多个（gateway）server在一个端口上
  >
  > * 原来的设计： routeName的格式是 `https.<portnumber>.portname` ，这样如果有多条就会导致routeName一样，那么在后面的处理中就会覆盖、不一致
  > * 新的设计： 格式改为`https.<portNumber>.<portName>.<gatewayName>.<namespace>`，这样后续就不会...
  >
  > 有一些计算流程上的相关改动，原先只需要传server对象，现在需要多传一个routeName，等等
  
- Five bug fixes for locality weighted load balancing:
  - Fix bug causing empty endpoints per locality ([Issue 12610](https://github.com/istio/istio/issues/12610))
  
    > yonka: 就是笔误
  
  - Apply locality weighted load balancing configuration correctly ([Issue 12587](https://github.com/istio/istio/issues/12587))
  
  - Locality label `istio-locality` in Kubernetes should not contain `/`, use `.` ([Issue 12582](https://github.com/istio/istio/issues/12582))
  
  - Fix crash in locality load balancing ([Issue 12649](https://github.com/istio/istio/pull/12649))
  
    > yonka: 判nil
  
  - Fix bug in locality load balancing normalization ([Issue 12579](https://github.com/istio/istio/pull/12579))
  
- Propagate Envoy Metrics Service configuration ([Issue 12569](https://github.com/istio/istio/issues/12569))

- Do not apply `VirtualService` rule to the wrong gateway ([Issue 10313](https://github.com/istio/istio/issues/10313))

  > yonka: https://github.com/istio/istio/pull/12663
  >
  > * `createGatewayTCPFilterChainOpts` 中 `buildGatewayNetworkFiltersFromTCPRoutes`/`buildGatewayNetworkFiltersFromTLSRoutes` 中调`PushContext.VirtualServices`，传入了gateways，指定获取绑定到这些gateways的vs
  >
  > * `buildGatewayListeners` 会先 `node.ServiceInstances` -> instances的workloadLabels聚合成一个 -> 根据这个总workloadLabels过滤适用的gateways -> 把这些gateways按port聚合（要求单port只能一个协议） -> 进行迭代，对每个port的servers也进行迭代 -> `buildListenerOpts` -> build listener with opts
  >
  > * 原来代码，`buildGatewayListeners`中使用`mergeGateway.Names`，也就是说对于merged gateway的某一个server，会取到所有的names的vs，就出问题了
  >
  >   这里的修正方式是： 只传入这个server对应的gateway config的name
  >
  > * `buildGatewayHTTPRouteConfig` 同理

### 1.1.2

We’re announcing immediate availability of Istio 1.1.2 which contains some important security updates. Please see below for details.



**Security update**

Two security vulnerabilities have recently been identified in the Envoy proxy ([CVE 2019-9900](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-9900) and [CVE 2019-9901](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-9901)). The vulnerabilities have now been patched in Envoy version 1.9.1, and correspondingly in the Envoy builds embedded in Istio 1.1.2 and Istio 1.0.7. Since Envoy is an integral part of Istio, users are advised to update Istio immediately to mitigate security risks arising from these vulnerabilities.

The vulnerabilities are centered on the fact that Envoy did not normalize HTTP URI paths and did not fully validate HTTP/1.1 header values. These vulnerabilities impact Istio features that rely on Envoy to enforce any of authorization, routing, or rate limiting.

**Affected Istio releases**

The following Istio releases are vulnerable:

- 1.1, 1.1.1
  - These releases can be patched to Istio 1.1.2.
  - 1.1.2 is built from the same source as 1.1.1 with the addition of Envoy patches minimally sufficient to address the CVEs.
- 1.0, 1.0.1, 1.0.2, 1.0.3, 1.0.4, 1.0.5, 1.0.6
  - These releases can be patched to Istio 1.0.7
  - 1.0.7 is built from the same source as 1.0.6 with the addition of Envoy patches minimally sufficient to address the CVEs.
- 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8
  - These releases are no longer supported and will not be patched. Please upgrade to a supported release with the necessary fixes.

**Vulnerability impact**

[CVE 2019-9900](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-9900) and [CVE 2019-9901](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-9901) allow remote attackers access to unauthorized resources by using specially crafted request URI paths (9901) and NUL bytes in HTTP/1.1 headers (9900), potentially circumventing DoS prevention systems such as rate limiting, or routing to a unexposed upstream system. Refer to [issue 6434](https://github.com/envoyproxy/envoy/issues/6434) and [issue 6435](https://github.com/envoyproxy/envoy/issues/6435) for more information.

As Istio is based on Envoy, Istio customers can be affected by these vulnerabilities based on whether paths and request headers are used within Istio policies or routing rules and how the backend HTTP implementation resolves them. If prefix path matching rules are used by Mixer or by Istio authorization policies or the routing rules, an attacker could exploit these vulnerabilities to gain access to unauthorized paths on certain HTTP backends.

**Mitigation**

Eliminating the vulnerabilities requires updating to a corrected version of Envoy. We’ve incorporated the necessary updates in the latest Istio patch releases.

For Istio 1.1.x deployments: update to a minimum of [Istio 1.1.2](https://istio.io/latest/news/releases/1.1.x/announcing-1.1.2)

For Istio 1.0.x deployments: update to a minimum of [Istio 1.0.7](https://istio.io/latest/news/releases/1.0.x/announcing-1.0.7)

While Envoy 1.9.1 requires opting in to path normalization to address CVE 2019-9901, the version of Envoy embedded in Istio 1.1.2 and 1.0.7 enables path normalization by default.

**Detection of NUL header exploit**

Based on current information, this only affects HTTP/1.1 traffic. If this is not structurally possible in your network or configuration, then it is unlikely that this vulnerability applies.

File-based access logging uses the `c_str()` representation for header values, as does gRPC access logging, so there will be no trivial detection via Envoy’s access logs by scanning for NUL. Instead, operators might look for inconsistencies in logs between the routing that Envoy performs and the logic intended in the `RouteConfiguration`.

External authorization and rate limit services can check for NULs in headers. Backend servers might have sufficient logging to detect NULs or unintended access; it’s likely that many will simply reject NULs in this scenario via 400 Bad Request, as per RFC 7230.

**Detection of path traversal exploit**

Envoy’s access logs (whether file-based or gRPC) will contain the unnormalized path, so it is possible to examine these logs to detect suspicious patterns and requests that are incongruous with the intended operator configuration intent. In addition, unnormalized paths are available at `ext_authz`, rate limiting and backend servers for log inspection.



### 1.1.3

**Known issues with 1.1.3**

- A [panic in the Node Agent](https://github.com/istio/istio/issues/13325) was discovered late in the 1.1.3 qualification process. The panic only occurs in clusters with the alpha-quality SDS certificate rotation feature enabled. Since this is the first time we have included SDS certificate rotation in our long-running release tests, we don’t know whether this is a latent bug or a new regression. Considering SDS certificate rotation is in alpha, we have decided to release 1.1.3 with this issue and target a fix for the 1.1.4 release.

  > yonka: 只是goroutine workers并发访问了普通map。 改为sync map即可

**Bug fixes**

- Istio-specific back-ports of Envoy patches for [`CVE-2019-9900`](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-9900) and [`CVE-2019-9901`](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-9901) included in Istio 1.1.2 have been dropped in favor of an Envoy update which contains the final version of the patches.
- Fix load balancer weight setting for split horizon `EDS`.
- Fix typo in the default Envoy `JSON` log format ([Issue 12232](https://github.com/istio/istio/issues/12232)).
- Correctly reload out-of-process adapter address upon configuration change ([Issue 12488](https://github.com/istio/istio/issues/12488)).
- Restore Kiali settings that were accidentally deleted ([Issue 3660](https://github.com/istio/istio/issues/3660)).
- Prevent services with same target port resulting in duplicate inbound listeners ([Issue 9504](https://github.com/istio/istio/issues/9504)).
- Fix issue with configuring `Sidecar egress` ports for namespaces other than `istio-system` resulting in a `envoy.tcp_proxy` filter of `BlackHoleCluster` by auto binding to services for `Sidecar` listeners ([Issue 12536](https://github.com/istio/istio/issues/12536)).
- Fix gateway `vhost` configuration generation issue by favoring more specific host matches ([Issue 12655](https://github.com/istio/istio/issues/12655)).
- Fix `ALLOW_ANY` so it now allows external traffic if there is already an http service present on a port.
- Fix validation logic so that `port.name` is no longer a valid `PortSelection`.
- Fix [`istioctl proxy-config cluster`](https://istio.io/latest/docs/reference/commands/istioctl/#istioctl-proxy-config-cluster) cluster type column rendering ([Issue 12455](https://github.com/istio/istio/issues/12455)).
- Fix SDS secret mount configuration.
- Fix incorrect Istio version in the Helm charts.
- Fix partial DNS failures in the presence of overlapping ports ([Issue 11658](https://github.com/istio/istio/issues/11658)).
- Fix Helm `podAntiAffinity` template error ([Issue 12790](https://github.com/istio/istio/issues/12790)).
- Fix bug with the original destination service discovery not using the original destination load balancer.
- Fix SDS memory leak in the presence of invalid or missing keying materials ([Issue 13197](https://github.com/istio/istio/issues/13197)).

**Small enhancements**

- Hide `ServiceAccounts` from `PushContext` log to reduce log volume.
- Configure `localityLbSetting` in `values.yaml` by passing it through to the mesh configuration.
- Remove the soon-to-be deprecated `critical-pod` annotation from Helm charts ([Issue 12650](https://github.com/istio/istio/issues/12650)).
- Support pod anti-affinity annotations to improve control plane availability ([Issue 11333](https://github.com/istio/istio/issues/11333)).
- Pretty print `IP` addresses in access logs.
- Remove redundant write header to further reduce log volume.
- Improve destination host validation in Pilot.
- Explicitly configure `istio-init` to run as root so use of pod-level `securityContext.runAsUser` doesn’t break it ([Issue 5453](https://github.com/istio/istio/issues/5453)).
- Add configuration samples for Vault integration.
- Respect locality load balancing weight settings from `ServiceEntry`.
- Make the TLS certificate location watched by Pilot Agent configurable ([Issue 11984](https://github.com/istio/istio/issues/11984)).
- Add support for Datadog tracing.
- Add alias to `istioctl` so ‘x’ can be used instead of ‘experimental’.
- Provide improved distribution of sidecar certificate by adding jitter to their CSR requests.
- Allow weighted load balancing registry locality to be configured.
- Add support for standard CRDs for compiled-in Mixer adapters.
- Reduce Pilot resource requirements for demo configuration.
- Fully populate Galley dashboard by adding data source ([Issue 13040](https://github.com/istio/istio/issues/13040)).
- Propagate Istio 1.1 `sidecar` performance tuning to the `istio-gateway`.
- Improve destination host validation by rejecting `*` hosts ([Issue 12794](https://github.com/istio/istio/issues/12794)).
- Expose upstream `idle_timeout` in cluster definition so dead connections can sometimes be removed from connection pools before they are used ([Issue 9113](https://github.com/istio/istio/issues/9113)).
- When registering a `Sidecar` resource to restrict what a pod can see, the restrictions are now applied if the spec contains a `workloadSelector` ([Issue 11818](https://github.com/istio/istio/issues/11818)).
- Update the Bookinfo example to use port 80 for TLS origination.
- Add liveness probe for Citadel.
- Improve AWS ELB interoperability by making 15020 the first port listed in the `ingressgateway` service ([Issue 12502](https://github.com/istio/istio/issues/12503)).
- Use outlier detection for failover mode but not for distribute mode for locality weighted load balancing ([Issues 12965](https://github.com/istio/istio/issues/12961)).
- Replace generation of Envoy’s deprecated `enabled` field in `CorsPolicy` with the replacement `filter_enabled` field for 1.1.0+ sidecars only.
- Standardize labels on Mixer’s Helm charts.



### 1.1.4

**Behavior change**

- Changed the default behavior for Pilot to allow traffic to outside the mesh, even if it is on the same port as an internal service. This behavior can be controlled by the `PILOT_ENABLE_FALLTHROUGH_ROUTE` environment variable.

**Bug fixes**

- Fixed egress route generation for services of type `ExternalName`.
- Added support for configuring Envoy’s idle connection timeout, which prevents running out of memory or IP ports over time ([Issue 13355](https://github.com/istio/istio/issues/13355)).
- Fixed a crashing bug in Pilot in failover handling of locality-based load balancing.
- Fixed a crashing bug in Pilot when it was given custom certificate paths.
- Fixed a bug in Pilot where it was ignoring short names used as service entry hosts ([Issue 13436](https://github.com/istio/istio/issues/13436)).
- Added missing `https_protocol_options` to the envoy-metrics-service cluster configuration.
- Fixed a bug in Pilot where it didn’t handle https traffic correctly in the fall through route case ([Issue 13386](https://github.com/istio/istio/issues/13386)).
- Fixed a bug where Pilot didn’t remove endpoints from Envoy after they were removed from Kubernetes ([Issue 13402](https://github.com/istio/istio/issues/13402)).
- Fixed a crashing bug in the node agent ([Issue 13325](https://github.com/istio/istio/issues/13325)).
- Added missing validation to prevent gateway names from containing dots ([Issue 13211](https://github.com/istio/istio/issues/13211)).
- Fixed bug where [`ConsistentHashLB.minimumRingSize`](https://istio.io/latest/docs/reference/config/networking/destination-rule#LoadBalancerSettings-ConsistentHashLB) was defaulting to 0 instead of the documented 1024 ([Issue 13261](https://github.com/istio/istio/issues/13261)).

**Small enhancements**

- Updated to the latest version of the [Kiali](https://www.kiali.io/) add-on.
- Updated to the latest version of [Grafana](https://grafana.com/).
- Added validation to ensure Citadel is only deployed with a single replica ([Issue 13383](https://github.com/istio/istio/issues/13383)).
- Added support to configure the logging level of the proxy and Istio control plane (([Issue 11847](https://github.com/istio/istio/issues/11847)).
- Allow sidecars to bind to any loopback address and not just 127.0.0.1 ([Issue 13201](https://github.com/istio/istio/issues/13201)).



### 1.1.5

**Bug fixes**

- Add additional validation to Pilot to reject gateway configuration with overlapping hosts matches ([Issue 13717](https://github.com/istio/istio/issues/13717)).
- Build against the latest stable version of `istio-cni` instead of the latest daily build ([Issue 13171](https://github.com/istio/istio/issues/13171)).

**Small enhancements**

- Add additional logging to help diagnose hostname resolution failures ([Issue 13581](https://github.com/istio/istio/issues/13581)).
- Improve ease of installing `prometheus` by removing unnecessary use of `busybox` image ([Issue 13501](https://github.com/istio/istio/issues/13501)).
- Make Pilot Agent’s certificate paths configurable ([Issue 11984](https://github.com/istio/istio/issues/11984)).



### 1.1.6

**Bug fixes**

- Fix Galley Helm charts so that the `validatingwebhookconfiguration` object can now deployed to a namespace other than `istio-system` ([Issue 13625](https://github.com/istio/istio/issues/13625)).
- Additional Helm chart fixes for anti-affinity support: fix `gatewaypodAntiAffinityRequiredDuringScheduling` and `podAntiAffinityLabelSelector` match expressions and fix the default value for `podAntiAffinityLabelSelector` ([Issue 13892](https://github.com/istio/istio/issues/13892)).
- Make Pilot handle a condition where Envoy continues to request routes for a deleted gateway while listeners are still draining ([Issue 13739](https://github.com/istio/istio/issues/13739)).

**Small enhancements**

- If access logs are enabled, `passthrough` listener requests will be logged.
- Make Pilot tolerate unknown JSON fields to make it easier to rollback to older versions during upgrade.
- Add support for fallback secrets to `SDS` which Envoy can use instead of waiting indefinitely for late or non-existent secrets during startup ([Issue 13853](https://github.com/istio/istio/issues/13853)).



### 1.1.7

**Security update**

This release fixes [CVE 2019-12243](https://istio.io/latest/news/security/istio-security-2019-001).

**Bug fixes**

- Fix issue where two gateways with overlapping hosts, created at the same second, can cause Pilot to fail to generate routes correctly and lead to Envoy listeners stuck indefinitely at startup in a warming state.
- Improve the robustness of the SDS node agent: if Envoy sends a SDS request with an empty `ResourceNames`, ignore it and wait for the next request instead of closing the connection ([Issue 13853](https://github.com/istio/istio/issues/13853)).
- In prior releases Pilot automatically injected the experimental `envoy.filters.network.mysql_proxy` filter into the outbound filter chain if the service port name is `mysql`. This was surprising and caused issues for some operators, so Pilot will now automatically inject the `envoy.filters.network.mysql_proxy` filter only if the `PILOT_ENABLE_MYSQL_FILTER` environment variable is set to `1` ([Issue 13998](https://github.com/istio/istio/issues/13998)).
- Fix issue where Mixer policy checks were incorrectly disabled for TCP ([Issue 13868](https://github.com/istio/istio/issues/13868)).

**Small enhancements**

- Add `--applicationPorts` option to the `ingressgateway` Helm charts. When set to a comma-delimited list of ports, readiness checks will fail until all the ports become active. When configured, traffic will not be sent to Envoys stuck in the warming state.
- Increase memory limit in the `ingressgateway` Helm chart to 1GB and add resource `request` and `limits` to the SDS node agent container to support HPA autoscaling.



### 1.1.8

**Bug fixes**

- Fix `PASSTHROUGH DestinationRules` for CDS clusters ([Issue 13744](https://github.com/istio/istio/issues/13744)).
- Make the `appVersion` and `version` fields in the Helm charts display the correct Istio version ([Issue 14290](https://github.com/istio/istio/issues/14290)).
- Fix Mixer crash affecting both policy and telemetry servers ([Issue 14235](https://github.com/istio/istio/issues/14235)).
- Fix multicluster issue where two pods in different clusters could not share the same IP address ([Issue 14066](https://github.com/istio/istio/issues/14066)).
- Fix issue where Citadel could generate a new root CA if it cannot contact the Kubernetes API server, causing mutual TLS verification to incorrectly fail ([Issue 14512](https://github.com/istio/istio/issues/14512)).
- Improve Pilot validation to reject different `VirtualServices` with the same domain since Envoy will not accept them ([Issue 13267](https://github.com/istio/istio/issues/13267)).
- Fix locality load balancing issue where only one replica in a locality would receive traffic ([13994](https://github.com/istio/istio/issues/13994)).
- Fix issue where Pilot Agent might not notice a TLS certificate rotation ([Issue 14539](https://github.com/istio/istio/issues/14539)).
- Fix a `LuaJIT` panic in Envoy ([Envoy Issue 6994](https://github.com/envoyproxy/envoy/pull/6994)).
- Fix a race condition where Envoy might reuse a HTTP/1.1 connection after the downstream peer had already closed the TCP connection, causing 503 errors and retries ([Issue 14037](https://github.com/istio/istio/issues/14037)).
- Fix a tracing issue in Mixer’s Zipkin adapter causing missing spans ([Issue 13391](https://github.com/istio/istio/issues/13391)).

**Small enhancements**

- Reduce Pilot log spam by logging the `the endpoints within network ... will be ignored for no network configured` message at `DEBUG`.
- Make it easier to rollback by making pilot-agent ignore unknown flags.
- Update Citadel’s default root CA certificate TTL from 1 year to 10 years.



### 1.1.9

**Bug fixes**

- Prevent overly large strings from being sent to Prometheus ([Issue 14642](https://github.com/istio/istio/issues/14642)).
- Reuse previously cached JWT public keys if transport errors are encountered during renewal ([Issue 14638](https://github.com/istio/istio/issues/14638)).
- Bypass JWT authentication for HTTP OPTIONS methods to support CORS requests.
- Fix Envoy crash caused by the Mixer filter ([Issue 14707](https://github.com/istio/istio/issues/14707)).

**Small enhancements**

- Expose cryptographic signature verification functions to `Lua` Envoy filters ([Envoy Issue 7009](https://github.com/envoyproxy/envoy/issues/7009)).



### 1.1.10

**Bug fixes**

- Eliminate 503 errors caused by Envoy not being able to talk to the SDS Node Agent after a restart ([Issue 14853](https://github.com/istio/istio/issues/14853)).
- Fix cause of ‘TLS error: Secret is not supplied by SDS’ errors during upgrade ([Issue 15020](https://github.com/istio/istio/issues/15020)).
- Fix crash in Istio’s JWT Envoy filter caused by malformed JWT ([Issue 15084](https://github.com/istio/istio/issues/15084)).



### 1.1.11

**Small enhancements**

- Add ability to enable `HTTP/1.0` support in ingress gateway ([Issue 13085](https://github.com/istio/istio/issues/13085)).



### 1.1.12

**Bug fixes**

- Fix a bug where the sidecar could infinitely forward requests to itself when a `Pod` resource defines a port that isn’t defined for a service ([Issue 14443](https://github.com/istio/istio/issues/14443)) and ([Issue 14242](https://github.com/istio/istio/issues/14242))



### 1.1.13

**Security update**

This release contains fixes for the security vulnerabilities described in [ISTIO-SECURITY-2019-003](https://istio.io/latest/news/security/istio-security-2019-003/) and [ISTIO-SECURITY-2019-004](https://istio.io/latest/news/security/istio-security-2019-004/). Specifically:

**ISTIO-SECURITY-2019-003**: An Envoy user reported publicly an issue (c.f. [Envoy Issue 7728](https://github.com/envoyproxy/envoy/issues/7728)) about regular expressions matching that crashes Envoy with very large URIs. * **[CVE-2019-14993](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-14993)**: After investigation, the Istio team has found that this issue could be leveraged for a DoS attack in Istio, if users are employing regular expressions in some of the Istio APIs: `JWT`, `VirtualService`, `HTTPAPISpecBinding`, `QuotaSpecBinding`.

**ISTIO-SECURITY-2019-004**: Envoy, and subsequently Istio are vulnerable to a series of trivial HTTP/2-based DoS attacks: * **[CVE-2019-9512](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-9512)**: HTTP/2 flood using `PING` frames and queuing of response `PING` ACK frames that results in unbounded memory growth (which can lead to out of memory conditions). * **[CVE-2019-9513](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-9513)**: HTTP/2 flood using PRIORITY frames that results in excessive CPU usage and starvation of other clients. * **[CVE-2019-9514](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-9514)**: HTTP/2 flood using `HEADERS` frames with invalid HTTP headers and queuing of response `RST_STREAM` frames that results in unbounded memory growth (which can lead to out of memory conditions). * **[CVE-2019-9515](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-9515)**: HTTP/2 flood using `SETTINGS` frames and queuing of `SETTINGS` ACK frames that results in unbounded memory growth (which can lead to out of memory conditions). * **[CVE-2019-9518](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-9518)**: HTTP/2 flood using frames with an empty payload that results in excessive CPU usage and starvation of other clients.

Nothing else is included in this release except for the above security fixes.





### 1.1.14



**Security update**

Following the previous fixes for the security vulnerabilities described in [ISTIO-SECURITY-2019-003](https://istio.io/latest/news/security/istio-security-2019-003/) and [ISTIO-SECURITY-2019-004](https://istio.io/latest/news/security/istio-security-2019-004/), we are now addressing the internal control plane communication surface. These security fixes were not available at the time of our previous security release, and we considered the control plane gRPC surface to be harder to exploit.

You can find the gRPC vulnerability fix description on their mailing list (c.f. [HTTP/2 Security Vulnerabilities](https://groups.google.com/forum/#!topic/grpc-io/w5jPamxdda4)).

**Bug fix**

- Fix an Envoy bug that breaks `java.net.http.HttpClient` and other clients that attempt to upgrade from `HTTP/1.1` to `HTTP/2` using the `Upgrade: h2c` header ([Issue 16391](https://github.com/istio/istio/issues/16391)).



### 1.1.15

**Bug fixes**

- Fix an Envoy crash introduced in Istio 1.1.14 ([Issue 16357](https://github.com/istio/istio/issues/16357)).

**Small enhancements**

- Expose `HTTP/2` window size settings as Pilot environment variables ([Issue 17117](https://github.com/istio/istio/issues/17117)).



### 1.1.16

**Security update**

This release contains fixes for the security vulnerability described in [our October 8th, 2019 news post](https://istio.io/latest/news/security/istio-security-2019-005). Specifically:

**ISTIO-SECURITY-2019-005**: A DoS vulnerability has been discovered by the Envoy community. * **[CVE-2019-15226](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-15226)**: After investigation, the Istio team has found that this issue could be leveraged for a DoS attack in Istio if an attacker uses a high quantity of very small headers.

Nothing else is included in this release except for the above security fix.



### 1.1.17

We’re pleased to announce the availability of Istio 1.1.17. This will be the last 1.1.x patch release. Please see below for what’s changed.



**Bug fixes**

- Fix a bug introduced by [our October 8th security release](https://istio.io/latest/news/security/istio-security-2019-005) which incorrectly calculated HTTP header and body sizes ([Issue 17735](https://github.com/istio/istio/issues/17735).



## relase-1.2



参见： 

* [Istio_Change Notes_1.2.html](_resource/Istio_Change Notes_1.2.html)

* [Istio_Upgrade Notes_1.2.html](_resource/Istio_Upgrade Notes_1.2.html)

* [Istio_Helm Changes_1.2.html](_resource/Istio_Helm Changes_1.2.html)



### 1.2.1

**Bug fixes**

- Fix duplicate CRD being generated in the install ([Issue 14976](https://github.com/istio/istio/issues/14976))
- Fix Mixer unable to start when Galley is disabled ([Issue 14841](https://github.com/istio/istio/issues/14841))
- Fix environment variable shadowing (NAMESPACE is used for listened namespaces and overwrites Citadel storage namespace (istio-system))
- Fix cause of ‘TLS error: Secret is not supplied by SDS’ errors during upgrade ([Issue 15020](https://github.com/istio/istio/issues/15020))

**Minor enhancements**

- Allow users to disable Istio default retries by setting retries to 0 ([Issue 14900](https://github.com/istio/istio/issues/14900))
- Introduction of a Redis filter (this feature is guarded with the environment feature flag `PILOT_ENABLE_REDIS_FILTER`, disabled by default)
- Add HTTP/1.0 support to gateway configuration generation ([Issue 13085](https://github.com/istio/istio/issues/13085))
- Add [toleration](https://kubernetes.io/docs/concepts/configuration/taint-and-toleration/) for Istio components ([Pull Request 15081](https://github.com/istio/istio/pull/15081))



### 1.2.2

**Bug fixes**

- Fix crash in Istio’s JWT Envoy filter caused by malformed JWT ([Issue 15084](https://github.com/istio/istio/issues/15084))
- Fix incorrect overwrite of x-forwarded-proto header ([Issue 15124](https://github.com/istio/istio/issues/15124))





### 1.2.3



**Bug fixes**

- Fix a bug where the sidecar could infinitely forward requests to itself when pod defines a port undefined for service ([Issue 14443](https://github.com/istio/istio/issues/14443)) and ([Issue 14242](https://github.com/istio/istio/issues/14242))
- Fix a bug where Stackdriver adapter shuts down after telemetry is started.
- Fix Redis connectivity issues.
- Fix case-sensitivity in regex-based HTTP URI matching for Virtual Service ([Issue 14983](https://github.com/istio/istio/issues/14983))
- Fix HPA and CPU settings for demo profile ([Issue 15338](https://github.com/istio/istio/issues/15338))
- Relax Keep-Alive enforcement policy to avoid dropping connections under load ([Issue 15088](https://github.com/istio/istio/issues/15088))
- When SDS is not used, skip Kubernetes JWT authentication to mitigate the risk of compromised (untrustworthy) JWTs being used.

**Tests upgrade**

- Update base image version for Bookinfo reviews sample app ([Issue 15477](https://github.com/istio/istio/issues/15477))
- Bookinfo samples image qualification ([Issue 14237](https://github.com/istio/istio/issues/14237))



### 1.2.4

**Security update**

This release contains fixes for the security vulnerabilities described in [ISTIO-SECURITY-2019-003](https://istio.io/latest/news/security/istio-security-2019-003/)] [ISTIO-SECURITY-2019-004](https://istio.io/latest/news/security/istio-security-2019-004/). Specifically:

**ISTIO-SECURITY-2019-003**: An Envoy user reported publicly an issue (c.f. [Envoy Issue 7728](https://github.com/envoyproxy/envoy/issues/7728)) about regular expressions matching that crashes Envoy with very large URIs. * **[CVE-2019-14993](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-14993)**: After investigation, the Istio team has found that this issue could be leveraged for a DoS attack in Istio, if users are employing regular expressions in some of the Istio APIs: `JWT`, `VirtualService`, `HTTPAPISpecBinding`, `QuotaSpecBinding`.

**ISTIO-SECURITY-2019-004**: Envoy, and subsequently Istio are vulnerable to a series of trivial HTTP/2-based DoS attacks: * **[CVE-2019-9512](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-9512)**: HTTP/2 flood using `PING` frames and queuing of response `PING` ACK frames that results in unbounded memory growth (which can lead to out of memory conditions). * **[CVE-2019-9513](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-9513)**: HTTP/2 flood using PRIORITY frames that results in excessive CPU usage and starvation of other clients. * **[CVE-2019-9514](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-9514)**: HTTP/2 flood using `HEADERS` frames with invalid HTTP headers and queuing of response `RST_STREAM` frames that results in unbounded memory growth (which can lead to out of memory conditions). * **[CVE-2019-9515](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-9515)**: HTTP/2 flood using `SETTINGS` frames and queuing of `SETTINGS` ACK frames that results in unbounded memory growth (which can lead to out of memory conditions). * **[CVE-2019-9518](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-9518)**: HTTP/2 flood using frames with an empty payload that results in excessive CPU usage and starvation of other clients.

Nothing else is included in this release except for the above security fixes.



### 1.2.5



**Security update**

Following the previous fixes for the security vulnerabilities described in [ISTIO-SECURITY-2019-003](https://istio.io/latest/news/security/istio-security-2019-003/) and [ISTIO-SECURITY-2019-004](https://istio.io/latest/news/security/istio-security-2019-004), we are now addressing the internal control plane communication surface. These security fixes were not available at the time of our previous security release, and we considered the control plane gRPC surface to be harder to exploit.

You can find the gRPC vulnerability fix description on their mailing list (c.f. [HTTP/2 Security Vulnerabilities](https://groups.google.com/forum/#!topic/grpc-io/w5jPamxdda4)).

**Bug fixes**

- Fix an Envoy bug that breaks `java.net.http.HttpClient` and other clients that attempt to upgrade from `HTTP/1.1` to `HTTP/2` using the `Upgrade: h2c` header ([Issue 16391](https://github.com/istio/istio/issues/16391)).
- Fix a memory leak on send timeout ([Issue 15876](https://github.com/istio/istio/issues/15876)).



### 1.2.6



**Bug fixes**

- Fix `redisquota` inconsistency in regards to `memquota` counting ([Issue 15543](https://github.com/istio/istio/issues/15543)).
- Fix an Envoy crash introduced in Istio 1.2.5 ([Issue 16357](https://github.com/istio/istio/issues/16357)).
- Fix Citadel health check broken in the context of plugin certs (with intermediate certs) ([Issue 16593](https://github.com/istio/istio/issues/16593)).
- Fix Stackdriver Mixer Adapter error log verbosity ([Issue 16782](https://github.com/istio/istio/issues/16782)).
- Fix a bug where the service account map would be erased for service hostnames with more than one port.
- Fix incorrect `filterChainMatch` wildcard hosts duplication produced by Pilot ([Issue 16573](https://github.com/istio/istio/issues/16573)).

**Small enhancements**

- Expose `sidecarToTelemetrySessionAffinity` (required for Mixer V1) when it talks to services like Stackdriver. ([Issue 16862](https://github.com/istio/istio/issues/16862)).
- Expose `HTTP/2` window size settings as Pilot environment variables ([Issue 17117](https://github.com/istio/istio/issues/17117)).



### 1.2.7



**Security update**

This release contains fixes for the security vulnerability described in [our October 8th, 2019 news post](https://istio.io/latest/news/security/istio-security-2019-005). Specifically:

**ISTIO-SECURITY-2019-005**: A DoS vulnerability has been discovered by the Envoy community. * **[CVE-2019-15226](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-15226)**: After investigation, the Istio team has found that this issue could be leveraged for a DoS attack in Istio if an attacker uses a high quantity of very small headers.

**Bug fix**

- Fix a bug where `nodeagent` was failing to start when using citadel ([Issue 15876](https://github.com/istio/istio/issues/17108))



### 1.2.8

**Bug fixes**

- Fix a bug introduced by [our October 8th security release](https://istio.io/latest/news/security/istio-security-2019-005) which incorrectly calculated HTTP header and body sizes ([Issue 17735](https://github.com/istio/istio/issues/17735)).
- Fix a minor bug where endpoints still remained in /clusters while scaling a deployment to 0 replica ([Issue 14336](https://github.com/istio/istio/issues/14336)).
- Fix Helm upgrade process to correctly update mesh policy for mutual TLS ([Issue 16170](https://github.com/istio/istio/issues/16170)).
- Fix inconsistencies in the destination service label for TCP connection opened/closed metrics ([Issue 17234](https://github.com/istio/istio/issues/17234)).
- Fix the Istio secret cleanup mechanism ([Issue 17122](https://github.com/istio/istio/issues/17122)).
- Fix the Mixer Stackdriver adapter encoding process to handle invalid UTF-8 ([Issue 16966](https://github.com/istio/istio/issues/16966)).

**Features**

- Add `pilot` support for the new failure domain labels: `zone` and `region`.



### 1.2.9



**Bug fixes**

- Fix a proxy startup race condition.

**Features**

- Adding support for Citadel automatic root certificate rotation ([Issue 17059](https://github.com/istio/istio/issues/17059)).



### 1.2.10

This release contains fixes for the security vulnerability described in [our December 10th, 2019 news post](https://istio.io/latest/news/security/istio-security-2019-007). This release note describes what’s different between Istio 1.2.9 and Istio 1.2.10.



**Security update**

- **ISTIO-SECURITY-2019-007** A heap overflow and improper input validation have been discovered in Envoy.

**[CVE-2019-18801](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-18801)**: Fix a vulnerability affecting Envoy’s processing of large HTTP/2 request headers. A successful exploitation of this vulnerability could lead to a denial of service, escalation of privileges, or information disclosure. **[CVE-2019-18802](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-18802)**: Fix a vulnerability resulting from whitespace after HTTP/1 header values which could allow an attacker to bypass Istio’s policy checks, potentially resulting in information disclosure or escalation of privileges. **[CVE-2019-18838](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-18838)**: Fix a vulnerability resulting from malformed HTTP request missing the “Host” header. An encoder filter that invokes Envoy’s route manager APIs that access request’s “Host” header will cause a NULL pointer to be dereferenced and result in abnormal termination of the Envoy process.

**Bug fix**

- Add support for Citadel to automatically rotate root cert. ([Issue 17059](https://github.com/istio/istio/issues/17059))

## relase-1.3



参见： 

* [Istio_Change Notes_1.3.html](_resource/Istio_Change Notes_1.3.html)

* [Istio_Upgrade Notes_1.3.html](_resource/Istio_Upgrade Notes_1.3.html)

* [Istio_Helm Changes_1.3.html](_resource/Istio_Helm Changes_1.3.html)



### 1.3.1

**Bug fixes**

- **Fixed** an issue which caused the secret cleanup job to erroneously run during upgrades ([Issue 16873](https://github.com/istio/istio/issues/16873)).
- **Fixed** an issue where the default configuration disabled Kubernetes Ingress support ([Issue 17148](https://github.com/istio/istio/issues/17148))
- **Fixed** an issue with handling invalid `UTF-8` characters in the Stackdriver logging adapter ([Issue 16966](https://github.com/istio/istio/issues/16966)).
- **Fixed** an issue which caused the `destination_service` label in HTTP metrics not to be set for `BlackHoleCluster` and `PassThroughCluster` ([Issue 16629](https://github.com/istio/istio/issues/16629)).
- **Fixed** an issue with the `destination_service` label in the `istio_tcp_connections_closed_total` and `istio_tcp_connections_opened_total` metrics which caused them to not be set correctly ([Issue 17234](https://github.com/istio/istio/issues/17234)).
- **Fixed** an Envoy crash introduced in Istio 1.2.4 ([Issue 16357](https://github.com/istio/istio/issues/16357)).
- **Fixed** Istio CNI sidecar initialization when IPv6 is disabled on the node ([Issue 15895](https://github.com/istio/istio/issues/15895)).
- **Fixed** a regression affecting support of RS384 and RS512 algorithms in JWTs ([Issue 15380](https://github.com/istio/istio/issues/15380)).

**Minor enhancements**

- **Added** support for `.Values.global.priorityClassName` to the telemetry deployment.
- **Added** annotations for Datadog tracing that controls extra features in sidecars.
- **Added** the `pilot_xds_push_time` metric to report Pilot xDS push time.
- **Added** `istioctl experimental analyze` to support multi-resource analysis and validation.
- **Added** support for running metadata exchange and stats extensions in a WebAssembly sandbox.
- **Removed** time diff info in the proxy-status command.

### 1.3.2

**Security update**

This release contains fixes for the security vulnerability described in [our October 8th, 2019 news post](https://istio.io/latest/news/security/istio-security-2019-005). Specifically:

**ISTIO-SECURITY-2019-005**: A DoS vulnerability has been discovered by the Envoy community. * **[CVE-2019-15226](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-15226)**: After investigation, the Istio team has found that this issue could be leveraged for a DoS attack in Istio if an attacker uses a high quantity of very small headers.

Nothing else is included in this release except for the above security fix. Distroless images will be available in a few days.

### 1.3.3

This release includes bug fixes to improve robustness. This release note describes what’s different between Istio 1.3.2 and Istio 1.3.3.



**Bug fixes**

- **Fixed** an issue which caused Prometheus to install improperly when using `istioctl x manifest apply`. ([Issue 16970](https://github.com/istio/istio/issues/16970))
- **Fixed** a bug where locality load balancing can not read locality information from the node. ([Issue 17337](https://github.com/istio/istio/issues/17337))
- **Fixed** a bug where long-lived connections were getting dropped by the Envoy proxy as the listeners were getting reconfigured without any user configuration changes. ([Issue 17383](https://github.com/istio/istio/issues/17383), [Issue 17139](https://github.com/istio/istio/issues/17139))
- **Fixed** a crash in `istioctl x analyze` command. ([Issue 17449](https://github.com/istio/istio/issues/17449))
- **Fixed** `istioctl x manifest diff` to diff text blocks in ConfigMaps. ([Issue 16828](https://github.com/istio/istio/issues/16828))
- **Fixed** a segmentation fault crash in the Envoy proxy. ([Issue 17699](https://github.com/istio/istio/issues/17699))



### 1.3.4

This release includes bug fixes to improve robustness. This release note describes what’s different between Istio 1.3.3 and Istio 1.3.4.

**Bug fixes**

- **Fixed** a crashing bug in the Google node agent provider. ([Pull Request #18296](https://github.com/istio/istio/pull/18260))
- **Fixed** Prometheus annotations and updated Jaeger to 1.14. ([Pull Request #18274](https://github.com/istio/istio/pull/18274))
- **Fixed** in-bound listener reloads that occur on 5 minute intervals. ([Issue #18138](https://github.com/istio/istio/issues/18088))
- **Fixed** validation of key and certificate rotation. ([Issue #17718](https://github.com/istio/istio/issues/17718))
- **Fixed** invalid internal resource garbage collection. ([Issue #16818](https://github.com/istio/istio/issues/16818))
- **Fixed** webhooks that were not updated on a failure. ([Pull Request #17820](https://github.com/istio/istio/pull/17820)
- **Improved** performance of OpenCensus tracing adapter. ([Issue #18042](https://github.com/istio/istio/issues/18042))

**Minor enhancements**

- **Improved** reliability of the SDS service. ([Issue #17409](https://github.com/istio/istio/issues/17409), [Issue #17905](https://github.com/istio/istio/issues/17905))
- **Added** stable versions of failure domain labels. ([Pull Request #17755](https://github.com/istio/istio/pull/17755))
- **Added** update of the global mesh policy on upgrades. ([Pull Request #17033](https://github.com/istio/istio/pull/17033))



### 1.3.5

This release contains fixes for the security vulnerability described in [our November 11, 2019 news post](https://istio.io/latest/news/security/istio-security-2019-006) as well as bug fixes to improve robustness. This release note describes what’s different between Istio 1.3.4 and Istio 1.3.5.



**Security update**

- **ISTIO-SECURITY-2019-006** A DoS vulnerability has been discovered in Envoy.

**[CVE-2019-18817](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-18817)**: An infinite loop can be triggered in Envoy if the option `continue_on_listener_filters_timeout` is set to True, which is the case in Istio. This vulnerability could be leveraged for a DoS attack. If you applied the mitigation mentioned in [our November 11, 2019 news post](https://istio.io/latest/news/security/istio-security-2019-006), you can remove the mitigation once you upgrade to Istio 1.3.5 or newer.

**Bug fixes**

- **Fixed** Envoy listener configuration for TCP headless services. ([Issue #17748](https://github.com/istio/istio/issues/17748))
- **Fixed** an issue which caused stale endpoints to remain even when a deployment was scaled to 0 replicas. ([Issue #14436](https://github.com/istio/istio/issues/14336))
- **Fixed** Pilot to no longer crash when an invalid Envoy configuration is generated. ([Issue 17266](https://github.com/istio/istio/issues/17266))
- **Fixed** an issue with the `destination_service_name` label not getting populated for TCP metrics related to BlackHole/Passthrough clusters. ([Issue 17271](https://github.com/istio/istio/issues/17271))
- **Fixed** an issue with telemetry not reporting metrics for BlackHole/Passthrough clusters when fall through filter chains were invoked. This occurred when explicit `ServiceEntries` were configured for external services. ([Issue 17759](https://github.com/istio/istio/issues/17759))

**Minor enhancements**

- **Added** support for Citadel to periodically check the root certificate remaining lifetime and rotate expiring root certificates. ([Issue 17059](https://github.com/istio/istio/issues/17059))
- **Added** `PILOT_BLOCK_HTTP_ON_443` boolean environment variable to Pilot. If enabled, this flag prevents HTTP services from running on port 443 in order to prevent conflicts with external HTTP services. This is disabled by default. ([Issue 16458](https://github.com/istio/istio/issues/16458))



### 1.3.6

This release contains fixes for the security vulnerability described in [our December 10th, 2019 news post](https://istio.io/latest/news/security/istio-security-2019-007) as well as bug fixes to improve robustness. This release note describes what’s different between Istio 1.3.5 and Istio 1.3.6.



**Security update**

- **ISTIO-SECURITY-2019-007** A heap overflow and improper input validation have been discovered in Envoy.

**[CVE-2019-18801](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-18801)**: Fix a vulnerability affecting Envoy’s processing of large HTTP/2 request headers. A successful exploitation of this vulnerability could lead to a denial of service, escalation of privileges, or information disclosure. **[CVE-2019-18802](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-18802)**: Fix a vulnerability resulting from whitespace after HTTP/1 header values which could allow an attacker to bypass Istio’s policy checks, potentially resulting in information disclosure or escalation of privileges. **[CVE-2019-18838](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-18838)**: Fix a vulnerability resulting from malformed HTTP request missing the “Host” header. An encoder filter that invokes Envoy’s route manager APIs that access request’s “Host” header will cause a NULL pointer to be dereferenced and result in abnormal termination of the Envoy process.

**Bug fixes**

- **Fixed** an issue where a duplicate listener was generated for a proxy’s IP address when using a headless `TCP` service. ([Issue 17748](https://github.com/istio/istio/issues/17748))
- **Fixed** an issue with the `destination_service` label in HTTP related metrics incorrectly falling back to `request.host` which can cause a metric cardinality explosion for ingress traffic. ([Issue 18818](https://github.com/istio/istio/issues/18818))

**Minor enhancements**

- **Improved** load-shedding options for Mixer. Added support for a `requests-per-second` threshold for load-shedding enforcement. This allows operators to turn off load-shedding for Mixer in low traffic scenarios.



### 1.3.7

This release includes bug fixes to improve robustness. This release note describes what’s different between Istio 1.3.6 and Istio 1.3.7.





**Bug fixes**

- **Fixed** root certificate rotation in Citadel to reuse values from the expiring root certificate into the new root certificate ([Issue 19644](https://github.com/istio/istio/issues/19644)).
- **Fixed** telemetry to ignore forwarded attributes at the gateway.
- **Fixed** sidecar injection into pods with containers that export no port ([Issue 18594](https://github.com/istio/istio/issues/18594)).
- **Added** telemetry support for pod names containing periods ([Issue 19015](https://github.com/istio/istio/issues/19015)).
- **Added** support for generating `PKCS#8` private keys in Citadel agent ([Issue 19948](https://github.com/istio/istio/issues/19948)).

**Minor enhancements**

- **Improved** injection template to fully specify `securityContext`, allowing `PodSecurityPolicies` to properly validate injected deployments ([Issue 17318](https://github.com/istio/istio/issues/17318)).
- **Added** support for setting the `lifecycle` for proxy containers.
- **Added** support for setting the Mesh UID in the Stackdriver Mixer adapter ([Issue 17952](https://github.com/istio/istio/issues/17952)).

**Security update**

- [**ISTIO-SECURITY-2020-002**](https://istio.io/latest/news/security/istio-security-2020-002) Mixer policy check bypass caused by improperly accepting certain request headers.

**[CVE-2020-8843](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-8843)**: Under certain circumstances it is possible to bypass a specifically configured Mixer policy. Istio-proxy accepts `x-istio-attributes` header at ingress that can be used to affect policy decisions when Mixer policy selectively applies to source equal to ingress. Istio 1.3 to 1.3.6 is vulnerable.





### 1.3.8

This release contains a fix for the security vulnerability described in [our February 11th, 2020 news post](https://istio.io/latest/news/security/istio-security-2020-001). This release note describes what’s different between Istio 1.3.7 and Istio 1.3.8.



**Security update**

- **ISTIO-SECURITY-2020-001** Improper input validation have been discovered in `AuthenticationPolicy`.

**[CVE-2020-8595](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-8595)**: A bug in Istio’s [Authentication Policy](https://archive.istio.io/1.3/docs/reference/config/security/istio.authentication.v1alpha1/#Policy) exact path matching logic allows unauthorized access to resources without a valid JWT token.



## relase-1.4



参见： 

* [Istio_Change Notes_1.4.html](_resource/Istio_Change Notes_1.4.html)
* [Istio_Upgrade Notes_1.4.html](_resource/Istio_Upgrade Notes_1.4.html)



### 1.4.1

This release includes bug fixes to improve robustness. This release note describes what’s different between Istio 1.4.0 and Istio 1.4.1.



**Bug fixes**

- **Fixed** `istioctl` installation on Windows ([Issue 19020](https://github.com/istio/istio/pull/19020)).
- **Fixed** an issue with route matching order when using cert-manager with Kubernetes Ingress ([Issue 19000](https://github.com/istio/istio/pull/19000)).
- **Fixed** Mixer source namespace attribute when the pod name contains a period ([Issue 19015](https://github.com/istio/istio/issues/19015)).
- **Fixed** excessive metrics generated by Galley ([Issue 19165](https://github.com/istio/istio/issues/19165)).
- **Fixed** tracing Service port to correctly listen on port 80 ([Issue 19227](https://github.com/istio/istio/issues/19227)).
- **Fixed** missing `istioctl` auto-completion files ([Issue 19297](https://github.com/istio/istio/issues/19297)).

### 1.4.2

This release contains fixes for the security vulnerability described in [our December 10th, 2019 news post](https://istio.io/latest/news/security/istio-security-2019-007). This release note describes what’s different between Istio 1.4.1 and Istio 1.4.2.



**Security update**

- **ISTIO-SECURITY-2019-007** A heap overflow and improper input validation have been discovered in Envoy.

**[CVE-2019-18801](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-18801)**: Fix a vulnerability affecting Envoy’s processing of large HTTP/2 request headers. A successful exploitation of this vulnerability could lead to a denial of service, escalation of privileges, or information disclosure. **[CVE-2019-18802](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-18802)**: Fix a vulnerability resulting from whitespace after HTTP/1 header values which could allow an attacker to bypass Istio’s policy checks, potentially resulting in information disclosure or escalation of privileges. **[CVE-2019-18838](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-18838)**: Fix a vulnerability resulting from malformed HTTP request missing the “Host” header. An encoder filter that invokes Envoy’s route manager APIs that access request’s “Host” header will cause a NULL pointer to be dereferenced and result in abnormal termination of the Envoy process.

### 1.4.3

This release includes bug fixes to improve robustness and user experience. This release note describes what’s different between Istio 1.4.2 and Istio 1.4.3.



**Bug fixes**

- **Fixed** an issue where Mixer creates too many watches, overloading `kube-apiserver` ([Issue 19481](https://github.com/istio/istio/issues/19481)).
- **Fixed** an issue with injection when pod has multiple containers without exposed ports ([Issue 18594](https://github.com/istio/istio/issues/18594)).
- **Fixed** overly restrictive validation of `regex` field ([Issue 19212](https://github.com/istio/istio/pull/19212)).
- **Fixed** an upgrade issue with `regex` field ([Issue 19665](https://github.com/istio/istio/pull/19665)).
- **Fixed** `istioctl` install to properly send logs to `stderr` ([Issue 17743](https://github.com/istio/istio/issues/17743)).
- **Fixed** an issue where a file and profile could not be specified for `istioctl` installs ([Issue 19503](https://github.com/istio/istio/issues/19503)).
- **Fixed** an issue preventing certain objects from being installed for `istioctl` installs ([Issue 19371](https://github.com/istio/istio/issues/19371)).
- **Fixed** an issue preventing using certain JWKS with EC keys in JWT policy ([Issue 19424](https://github.com/istio/istio/issues/19424)).

**Improvements**

- **Improved** injection template to fully specify `securityContext`, allowing `PodSecurityPolicies` to properly validate injected deployments ([Issue 17318](https://github.com/istio/istio/issues/17318)).
- **Improved** telemetry v2 configuration to support Stackdriver and forward compatibility ([Issue 591](https://github.com/istio/installer/pull/591)).
- **Improved** output of `istioctl` installation ([Issue 19451](https://github.com/istio/istio/issues/19451)).
- **Improved** `istioctl` installation to set exit code upon failure ([Issue 19747](https://github.com/istio/istio/issues/19747)).

### 1.4.4

This release includes bug fixes to improve robustness and user experience as well as a fix for the security vulnerability described in [our February 11th, 2020 news post](https://istio.io/latest/news/security/istio-security-2020-001). This release note describes what’s different between Istio 1.4.3 and Istio 1.4.4.



**Security update**

- **ISTIO-SECURITY-2020-001** An improper input validation has been discovered in `AuthenticationPolicy`.

**[CVE-2020-8595](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-8595)**: A bug in Istio’s [Authentication Policy](https://archive.istio.io/1.4/docs/reference/config/security/istio.authentication.v1alpha1/#Policy) exact path matching logic allows unauthorized access to resources without a valid JWT token.

**Bug fixes**

- **Fixed** Debian packaging of `iptables` scripts ([Issue 19615](https://github.com/istio/istio/issues/19615)).
- **Fixed** an issue where Pilot generated a wrong Envoy configuration when the same port was used more than once ([Issue 19935](https://github.com/istio/istio/issues/19935)).
- **Fixed** an issue where running multiple instances of Pilot could lead to a crash ([Issue 20047](https://github.com/istio/istio/issues/20047)).
- **Fixed** a potential flood of configuration pushes from Pilot to Envoy when scaling the deployment to zero ([Issue 17957](https://github.com/istio/istio/issues/17957)).
- **Fixed** an issue where Mixer could not fetch the correct information from the request/response when pod contains a dot in its name ([Issue 20028](https://github.com/istio/istio/issues/20028)).
- **Fixed** an issue where Pilot sometimes would not send a correct pod configuration to Envoy ([Issue 19025](https://github.com/istio/istio/issues/19025)).
- **Fixed** an issue where sidecar injector with SDS enabled was overwriting pod `securityContext` section, instead of just patching it ([Issue 20409](https://github.com/istio/istio/issues/20409)).

**Improvements**

- **Improved** Better compatibility with Google CA. (Issues [20530](https://github.com/istio/istio/issues/20530), [20560](https://github.com/istio/istio/issues/20560)).
- **Improved** Added analyzer error message when Policies using JWT are not configured properly (Issues [20884](https://github.com/istio/istio/issues/20884), [20767](https://github.com/istio/istio/issues/20767)).

### 1.4.5

This release includes bug fixes to improve robustness. This release note describes what’s different between Istio 1.4.4 and Istio 1.4.5.

The fixes below focus on various bugs occurring during node restarts. If you use Istio CNI, or have nodes that restart, you are highly encouraged to upgrade.



**Improvements**

- **Fixed** a bug triggered by node restart causing Pods to receive incorrect configuration ([Issue 20676](https://github.com/istio/istio/issues/20676)).
- **Improved** [Istio CNI](https://istio.io/latest/docs/setup/additional-setup/cni/) robustness. Previously, when a node restarted, new pods may be created before the CNI was setup, causing pods to be created without `iptables` rules configured ([Issue 14327](https://github.com/istio/istio/issues/14327)).
- **Fixed** MCP metrics to include the size of the MCP responses, rather than just requests ([Issue 21049](https://github.com/istio/istio/issues/21049)).



### 1.4.6

This release contains fixes for the security vulnerabilities described in [our March 3rd, 2020 news post](https://istio.io/latest/news/security/istio-security-2020-003). This release note describes what’s different between Istio 1.4.5 and Istio 1.4.6.



**Security update**

- **ISTIO-SECURITY-2020-003** Two Uncontrolled Resource Consumption and Two Incorrect Access Control Vulnerabilities in Envoy.

**[CVE-2020-8659](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-8659)**: The Envoy proxy may consume excessive memory when proxying HTTP/1.1 requests or responses with many small (i.e. 1 byte) chunks. Envoy allocates a separate buffer fragment for each incoming or outgoing chunk with the size rounded to the nearest 4Kb and does not release empty chunks after committing data. Processing requests or responses with a lot of small chunks may result in extremely high memory overhead if the peer is slow or unable to read proxied data. The memory overhead could be two to three orders of magnitude more than configured buffer limits.

**[CVE-2020-8660](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-8660)**: The Envoy proxy contains a TLS inspector that can be bypassed (not recognized as a TLS client) by a client using only TLS 1.3. Because TLS extensions (SNI, ALPN) are not inspected, those connections may be matched to a wrong filter chain, possibly bypassing some security restrictions.

**[CVE-2020-8661](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-8661)**: The Envoy proxy may consume excessive amounts of memory when responding to pipelined HTTP/1.1 requests. In the case of illegally formed requests, Envoy sends an internally generated 400 error, which is sent to the `Network::Connection` buffer. If the client reads these responses slowly, it is possible to build up a large number of responses, and consume functionally unlimited memory. This bypasses Envoy’s overload manager, which will itself send an internally generated response when Envoy approaches configured memory thresholds, exacerbating the problem.

**[CVE-2020-8664](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-8664)**: For the SDS TLS validation context in the Envoy proxy, the update callback is called only when the secret is received for the first time or when its value changes. This leads to a race condition where other resources referencing the same secret (e.g,. trusted CA) remain unconfigured until the secret’s value changes, creating a potentially sizable window where a complete bypass of security checks from the static (“default”) section can occur.

- This vulnerability only affects the SDS implementation of Istio’s certificate rotation mechanism for Istio 1.4.5 and earlier which is only when SDS and mutual TLS are enabled. SDS is off by default and must be explicitly enabled by the operator in all versions of Istio prior to Istio 1.5. Istio’s default secret distribution implementation based on Kubernetes secret mounts is not affected by this vulnerability.

### 1.4.7

This release contains fixes for the security vulnerabilities described in [our March 25th, 2020 news post](https://istio.io/latest/news/security/istio-security-2020-004). This release note describes what’s different between Istio 1.4.6 and Istio 1.4.7.



**Security Update**

- **ISTIO-SECURITY-2020-004** Istio uses a hard coded `signing_key` for Kiali.

**[CVE-2020-1764](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-1764)**: Istio uses a default `signing key` to install Kiali. This can allow an attacker with access to Kiali to bypass authentication and gain administrative privileges over Istio. In addition, another CVE is fixed in this release, described in the Kiali 1.15.1 [release](https://kiali.io/news/security-bulletins/kiali-security-001/).

**Changes**

- **Fixed** an issue causing protocol detection to break HTTP2 traffic to gateways ([Issue 21230](https://github.com/istio/istio/issues/21230)).

### 1.4.8

This release includes bug fixes to improve robustness. This release note describes what’s different between Istio 1.4.7 and Istio 1.4.8.

The fixes below focus on various issues related to installing Istio on OpenShift with CNI. Instructions for installing Istio on OpenShift with CNI can be found [here](https://istio.io/latest/docs/setup/additional-setup/cni/#instructions-for-istio-1-4-x-and-openshift).



**Bug fixes**

- **Fixed** Fixed CNI installation on OpenShift ([Issue 21421](https://github.com/istio/istio/pull/21421)) ([Issue 22449](https://github.com/istio/istio/issues/22449)).
- **Fixed** Not all inbound ports are redirected when CNI is enabled ([Issue 22448](https://github.com/istio/istio/issues/22498)).
- **Fixed** Syntax errors in gateway templates with GoLang 1.14 ([Issue 22366](https://github.com/istio/istio/issues/22366)).
- **Fixed** Remove namespace from `clusterrole` and `clusterrolebinding` ([PR 297](https://github.com/istio/cni/pull/297)).

### 1.4.9

This release contains bug fixes to improve robustness and fixes for the security vulnerabilities described in [our May 12th, 2020 news post](https://istio.io/latest/news/security/istio-security-2020-005). This release note describes what’s different between Istio 1.4.9 and Istio 1.4.8.



**Security update**

- **ISTIO-SECURITY-2020-005** Denial of Service with Telemetry V2 enabled.

**[CVE-2020-10739](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-10739)**: By sending a specially crafted packet, an attacker could trigger a Null Pointer Exception resulting in a Denial of Service. This could be sent to the ingress gateway or a sidecar.

**Bug Fixes**

- **Fixed** the Helm installer to install Kiali using an dynamically generated signing key.
- **Fixed** Citadel to ignore namespaces that are not part of the mesh.
- **Fixed** the Istio operator installer to print the name of any resources that are not ready when an installation timeout occurs.

### 1.4.10

This is the final release for Istio 1.4.

This release fixes the security vulnerability described in [our June 11th, 2020 news post](https://istio.io/latest/news/security/istio-security-2020-006) as well as bug fixes to improve robustness.

This release note describes what’s different between Istio 1.4.9 and Istio 1.4.10.



**Security update**

- **ISTIO-SECURITY-2020-006** Excessive CPU usage when processing HTTP/2 SETTINGS frames with too many parameters, potentially leading to a denial of service.

**[CVE-2020-11080](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-11080)**: By sending a specially crafted packet, an attacker could cause the CPU to spike at 100%. This could be sent to the ingress gateway or a sidecar.

**Bug fixes**

- **Fixed** `istio-cni-node` crash when `COS_CONTAINERD` and Istio CNI are enabled when running on Google Kubernetes Engine ([Issue 23643](https://github.com/istio/istio/issues/23643))
- **Fixed** Istio CNI causes pod initialization to experience a 30-40 second delay on startup when DNS is unreachable ([Issue 23770](https://github.com/istio/istio/issues/23770))

**Bookinfo sample application security fixes**

We’ve updated the versions of Node.js and jQuery used in the Bookinfo sample application. Node.js has been upgraded from version 12.9 to 12.18. jQuery has been updated from version 2.1.4 to version 3.5.0. The highest rated vulnerability fixed: *HTTP request smuggling using malformed Transfer-Encoding header (Critical) (CVE-2019-15605)*



## relase-1.5



参见： 

* [Istio_Change Notes_1.5.html](_resource/Istio_Change Notes_1.5.html)

* [Istio_Upgrade Notes_1.5.html](_resource/Istio_Upgrade Notes_1.5.html)



### 1.5.1

This release contains bug fixes to improve robustness and fixes for the security vulnerabilities described in [our March 25th, 2020 news post](https://istio.io/latest/news/security/istio-security-2020-004). This release note describes what’s different between Istio 1.5.0 and Istio 1.5.1.



**Security update**

- **ISTIO-SECURITY-2020-004** Istio uses a hard coded `signing_key` for Kiali.

**[CVE-2020-1764](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-1764)**: Istio uses a default `signing key` to install Kiali. This can allow an attacker with access to Kiali to bypass authentication and gain administrative privileges over Istio. In addition, another CVE is fixed in this release, described in the Kiali 1.15.1 [release](https://kiali.io/news/security-bulletins/kiali-security-001/).

**Changes**

- **Fixed** an issue where Istio Operator instance deletion hangs for in-cluster operator ([Issue 22280](https://github.com/istio/istio/issues/22280))
- **Fixed** istioctl proxy-status should not list differences if just the order of the routes have changed ([Issue 21709](https://github.com/istio/istio/issues/21709))
- **Fixed** Incomplete support for array notation in “istioctl manifest apply —set” ([Issue 20950](https://github.com/istio/istio/issues/20950))
- **Fixed** Add possibility to add annotations to services in Kubernetes service spec ([Issue 21995](https://github.com/istio/istio/issues/21995))
- **Fixed** Enable setting ILB Gateway using istioctl ([Issue 20033](https://github.com/istio/istio/issues/20033))
- **Fixed** istioctl does not correctly set names on gateways ([Issue 21938](https://github.com/istio/istio/issues/21938))
- **Fixed** OpenID discovery does not work with beta request authentication policy ([Issue 21954](https://github.com/istio/istio/issues/21954))
- **Fixed** Issues related to shared control plane multicluster ([Issue 22173](https://github.com/istio/istio/pull/22173))
- **Fixed** Ingress port displaying target port instead of actual port ([Issue 22125](https://github.com/istio/istio/issues/22125))
- **Fixed** Issue where endpoints were being pruned automatically when installing the Istio Controller ([Issue 21495](https://github.com/istio/istio/issues/21495))
- **Fixed** Add istiod port to gateways for mesh expansion([Issue 22027](https://github.com/istio/istio/issues/22027))
- **Fixed** Multicluster secret controller silently ignoring updates to secrets ([Issue 18708](https://github.com/istio/istio/issues/18708))
- **Fixed** Autoscaler for mixer-telemetry always being generated when deploying with istioctl or Helm ([Issue 20935](https://github.com/istio/istio/issues/20935))
- **Fixed** Prometheus certificate provisioning is broken ([Issue 21843](https://github.com/istio/istio/issues/21843))
- **Fixed** Segmentation fault in Pilot with beta mutual TLS ([Issue 21816](https://github.com/istio/istio/issues/21816))
- **Fixed** Operator status enumeration not being rendered as a string ([Issue 21554](https://github.com/istio/istio/issues/21554))
- **Fixed** in-cluster operator fails to install control plane after having deleted a prior control plane ([Issue 21467](https://github.com/istio/istio/issues/21467))
- **Fixed** TCP metrics for BlackHole clusters does not work with Telemetry v2 ([Issue 21566](https://github.com/istio/istio/issues/21566))
- **Improved** Add option to enable V8 runtime for telemetry V2 ([Issue 21846](https://github.com/istio/istio/pull/21846))
- **Improved** compatibility of Helm gateway chart ([Issue 22295](https://github.com/istio/istio/pull/22295))
- **Improved** operator by adding a Helm installation chart ([Issue 21861](https://github.com/istio/istio/issues/21861))
- **Improved** Support custom CA on istio-agent ([Issue 22113](https://github.com/istio/istio/pull/22113))
- **Improved** Add a flag that supports passing GCP metadata to STS ([Issue 21904](https://github.com/istio/istio/issues/21904))



### 1.5.2

This release contains bug fixes to improve robustness. This release note describes what’s different between Istio 1.5.1 and Istio 1.5.2.



**Changes**

- **Fixed** Istiod deployment lacking label used by the matching `PodDisruptionBudget` ([Issue 22267](https://github.com/istio/istio/issues/22267))
- **Fixed** Custom Istio installation with istioctl not working using external charts ([Issue 22368](https://github.com/istio/istio/issues/22368))
- **Fixed** Panic in `istio-init` with GKE+COS and `interceptionMode`: TPROXY ([Issue 22500](https://github.com/istio/istio/issues/22500))
- **Fixed** Logging for validation by sending warnings to `stdErr` ([Issue 22496](https://github.com/istio/istio/issues/22496))
- **Fixed** Kiali not working when external Prometheus link used for the IstioOperator API ([Issue 22510](https://github.com/istio/istio/issues/22510))
- **Fixed** Istio agent should calculate grace period based on the cert TTL, not client-side settings ([Issue 22226](https://github.com/istio/istio/issues/22226)]
- **Fixed** Incorrect error message referring to incorrect CLI option for the `istioctl kube-inject` command ([Issue 22501](https://github.com/istio/istio/issues/22501))
- **Fixed** IstioOperator validation of slice ([Issue 21915](https://github.com/istio/istio/issues/21915))
- **Fixed** Race condition caused by read/write of `rootCert` and `rootCertExpireTime` not always being protected ([Issue 22627](https://github.com/istio/istio/issues/22627))
- **Fixed** BlackHoleCluster HTTP metrics broken with Telemetry v2 ([Issue 21385](https://github.com/istio/istio/issues/21385))
- **Fixed** `istio-init` container failing when Istio CNI is enabled ([Issue 22695](https://github.com/istio/istio/issues/22695))
- **Fixed** istioctl does not set gateway name for multiple gateways ([Issue 22703](https://github.com/istio/istio/issues/22703))
- **Fixed** Unstable inbound bind address when configuring a sidecar ingress listener without bind address ([Issue 22830](https://github.com/istio/istio/issues/22830))
- **Fixed** Proxy pods for Istio 1.4 not showing up when upgrading from Istio 1.4 to 1.5 using default profile ([Issue 22841](https://github.com/istio/istio/issues/22841))
- **Fixed** `PersistentVolumeClaim` for Grafana not being created in the namespace specified in the IstioOperator spec ([Issue 22835](https://github.com/istio/istio/issues/22835))
- **Fixed** `istio-sidecar-injector` and istiod related pods crashing when applying new manifest through istioctl because `alwaysInjectSelector` and `neverInjectSelector` are not correctly indented in the `istio-sidecar-injector` config map ([Issue 23027](https://github.com/istio/istio/issues/23027))
- **Fixed** Prometheus scraping failing in CNI injected pods because the default `excludeInboundPort` configuration does not include port 15090 ([Issue 23038](https://github.com/istio/istio/issues/23038))
- **Fixed** `Lightstep` secret volume issue causing the bundled Prometheus to not install correctly with Istio operator ([Issue 23078](https://github.com/istio/istio/issues/23078))
- **Fixed** Avoid using host header to extract destination service name at gateway in default Telemetry V2 configuration.
- **Fixed** Zipkin: Fix wrongly rendered timestamp value ([Issue 22968](https://github.com/istio/istio/issues/22968))
- **Improved** Add annotations for setting CPU/memory limits on sidecar ([Issue 16126](https://github.com/istio/istio/issues/16126))
- **Improved** Enable `rewriteAppHTTPProbe` annotation for liveness probe rewrite by default([Issue 10357](https://github.com/istio/istio/issues/10357))

### 1.5.3

> **DO NOT USE this release. USE release 1.5.4 instead.**

Due to a publishing error, the 1.5.3 images do not contain the fix for CVE-2020-10739 as claimed in the original announcement.

This release contains bug fixes to improve robustness. This release note describes what’s different between Istio 1.5.3 and Istio 1.5.2.



**Changes**

- **Fixed** the Helm installer to install Kiali using a dynamically generated signing key.
- **Fixed** overlaying the generated Kubernetes resources for addon components with user-defined overlays [(Issue 23048)](https://github.com/istio/istio/issues/23048)
- **Fixed** `istio-sidecar.deb` failing to start on Debian buster with `iptables` default `nftables` setting [(Issue 23279)](https://github.com/istio/istio/issues/23279)
- **Fixed** the corresponding hash policy not being updated after the header name specified in `DestinationRule.trafficPolicy.loadBalancer.consistentHash.httpHeaderName` is changed [(Issue 23434)](https://github.com/istio/istio/issues/23434)
- **Fixed** traffic routing when deployed in a namespace other than istio-system [(Issue 23401)](https://github.com/istio/istio/issues/23401)



### 1.5.4

This release fixes the security vulnerability described in [our May 12th, 2020 news post](https://istio.io/latest/news/security/istio-security-2020-005).

This release note describes what’s different between Istio 1.5.4 and Istio 1.5.3.



**Security update**

- **ISTIO-SECURITY-2020-005** Denial of Service with Telemetry V2 enabled.

**[CVE-2020-10739](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-10739)**: By sending a specially crafted packet, an attacker could trigger a Null Pointer Exception resulting in a Denial of Service. This could be sent to the ingress gateway or a sidecar.

### 1.5.5

This release fixes the security vulnerability described in [our June 11th, 2020 news post](https://istio.io/latest/news/security/istio-security-2020-006).

This release note describes what’s different between Istio 1.5.5 and Istio 1.5.4.



**Security update**

- **ISTIO-SECURITY-2020-006** Excessive CPU usage when processing HTTP/2 SETTINGS frames with too many parameters, potentially leading to a denial of service.

**[CVE-2020-11080](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-11080)**: By sending a specially crafted packet, an attacker could cause the CPU to spike at 100%. This could be sent to the ingress gateway or a sidecar.

### 1.5.6

This release contains bug fixes to improve robustness. This release note describes what’s different between Istio 1.5.5 and Istio 1.5.6.



**Security**

- **Updated** Node.js and jQuery versions used in bookinfo.

**Changes**

- **Fixed** Transfer-Encoding value case-sensitivity in Envoy ([Envoy’s issue 10041](https://github.com/envoyproxy/envoy/issues/10041))
- **Fixed** handling of user defined ingress gateway configuration ([Issue 23303](https://github.com/istio/istio/issues/23303))
- **Fixed** Add `TCP MX ALPN` in `UpstreamTlsContext` for clusters that specify `http2_protocol_options` ([Issue 23907](https://github.com/istio/istio/issues/23907))
- **Fixed** election lock for namespace configmap controller.
- **Fixed** `istioctl validate -f` for `networking.istio.io/v1beta1` rules ([Issue 24064](https://github.com/istio/istio/issues/24064))
- **Fixed** aggregate clusters configuration ([Issue 23909](https://github.com/istio/istio/issues/23909))
- **Fixed** Prometheus mTLS poods scraping ([Issue 22391](https://github.com/istio/istio/issues/22391))
- **Fixed** ingress crash for overlapping hosts without match ([Issue 22910](https://github.com/istio/istio/issues/22910))
- **Fixed** Istio telemetry Pod crashes ([Issue 23813](https://github.com/istio/istio/issues/23813))
- **Removed** hard-coded operator namespace ([Issue 24073](https://github.com/istio/istio/issues/24073))

### 1.5.7

This release fixes the security vulnerability described in [our June 30th, 2020 news post](https://istio.io/latest/news/security/istio-security-2020-007).

This release note describes what’s different between Istio 1.5.7 and Istio 1.5.6.



**Security update**

- **[CVE-2020-12603](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-12603)**: By sending a specially crafted packet, an attacker could cause Envoy to consume excessive amounts of memory when proxying HTTP/2 requests or responses.
  - CVSS Score: 7.0 [AV:N/AC:L/PR:N/UI:N/S:U/C:N/I:N/A:H](https://nvd.nist.gov/vuln-metrics/cvss/v3-calculator?vector=AV:N/AC:L/PR:N/UI:N/S:U/C:N/I:N/A:H&version=3.1)
- **[CVE-2020-12605](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-12605)**: An attacker could cause Envoy to consume excessive amounts of memory when processing specially crafted HTTP/1.1 packets.
  - CVSS Score: 7.0 [AV:N/AC:L/PR:N/UI:N/S:U/C:N/I:N/A:H](https://nvd.nist.gov/vuln-metrics/cvss/v3-calculator?vector=AV:N/AC:L/PR:N/UI:N/S:U/C:N/I:N/A:H&version=3.1)
- **[CVE-2020-8663](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-8663)**: An attacker could cause Envoy to exhaust file descriptors when accepting too many connections.
  - CVSS Score: 7.0 [AV:N/AC:L/PR:N/UI:N/S:U/C:N/I:N/A:H](https://nvd.nist.gov/vuln-metrics/cvss/v3-calculator?vector=AV:N/AC:L/PR:N/UI:N/S:U/C:N/I:N/A:H&version=3.1)
- **[CVE-2020-12604](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-12604)**: An attacker could cause increased memory usage when processing specially crafted packets.
  - CVSS Score: 5.3 [AV:N/AC:L/PR:N/UI:N/S:U/C:N/I:N/A:L](https://nvd.nist.gov/vuln-metrics/cvss/v3-calculator?vector=AV:N/AC:L/PR:N/UI:N/S:U/C:N/I:N/A:H&version=3.1)

### 1.5.8

This release fixes the security vulnerability described in [our July 9th, 2020 news post](https://istio.io/latest/news/security/istio-security-2020-008).

These release notes describe what’s different between Istio 1.5.8 and Istio 1.5.7.



**Security update**

- [CVE-2020-15104](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-15104): When validating TLS certificates, Envoy incorrectly allows wildcards in DNS Subject Alternative Name (SAN) to apply to multiple subdomains. For example, with a SAN of `*.example.com`, Envoy incorrectly allows `nested.subdomain.example.com`, when it should only allow `subdomain.example.com`.
- CVSS Score: 6.6 [AV:N/AC:H/PR:H/UI:N/S:C/C:H/I:L/A:N/E:F/RL:O/RC:C](https://nvd.nist.gov/vuln-metrics/cvss/v3-calculator?vector=AV:N/AC:H/PR:H/UI:N/S:C/C:H/I:L/A:N/E:F/RL:O/RC:C&version=3.1)

**Changes**

- **Allowed** setting `status.sidecar.istio.io/port` to zero ([Issue 24722](https://github.com/istio/istio/issues/24722))
- **Improved** `istioctl validate` to disallow unknown fields not included in the Open API specification ([Issue 24860](https://github.com/istio/istio/issues/24860))
- **Fixed** a bug in Mixer where it would incorrectly return source names when it did lookup by IP.

### 1.5.9

This release fixes the security vulnerability described in [our August 11th, 2020 news post](https://istio.io/latest/news/security/istio-security-2020-009).

These release notes describe what’s different between Istio 1.5.8 and Istio 1.5.9.



**Security update**

- [CVE-2020-16844](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-16844): Callers to TCP services that have a defined Authorization Policies with `DENY` actions using wildcard suffixes (e.g. `*-some-suffix` ) for source principals or namespace fields will never be denied access.

  - CVSS Score: 6.8 [AV:N/AC:H/PR:L/UI:N/S:U/C:H/I:H/A:N](https://nvd.nist.gov/vuln-metrics/cvss/v3-calculator?vector=AV:N/AC:H/PR:L/UI:N/S:U/C:H/I:H/A:N&version=3.1)



### 1.5.10


This release includes bug fixes to improve robustness. These release notes describe what’s different between Istio 1.5.9 and Istio 1.5.10.



**Bug fixes**

- **Fixed** container name as `app_container` in telemetry v2.
- **Fixed** ingress SDS not getting secret updates. ([Issue 23715](https://github.com/istio/istio/issues/23715)).

## release-1.6



参见： 

* [Istio_Change Notes_1.6.html](_resource/Istio_Change Notes_1.6.html)

* [Istio_Upgrade Notes_1.6.html](_resource/Istio_Upgrade Notes_1.6.html)



### 1.6.1

This release contains bug fixes to improve robustness. This release note describes what’s different between Istio 1.6.0 and Istio 1.6.1.



**Changes**

- **Fixed** support for pod annotations to override mesh-wide proxy settings
- **Updated** `EnvoyFilter` to register all filter types in order to support `typed_config` attributes ([Issue 23909](https://github.com/istio/istio/issues/23909))
- **Fixed** handling of custom resource names for Gateways ([Issue 23303](https://github.com/istio/istio/issues/23303))
- **Fixed** an issue where `istiod` fails to issue certificates to a remote cluster. `Istiod` now has support for the cluster name and certificate to generate the `injectionURL` ([Issue 23879](https://github.com/istio/istio/issues/23879))
- **Fixed** remote cluster’s validation controller to check `istiod`’s ready status endpoint ([Issue 23945](https://github.com/istio/istio/issues/23945))
- **Improved** `regexp` fields validation to match Envoy’s validation ([Issue 23436](https://github.com/istio/istio/issues/23436))
- **Fixed** `istioctl analyze` to validate `networking.istio.io/v1beta1` resources ([Issue 24064](https://github.com/istio/istio/issues/24064))
- **Fixed** typo of `istio` in `ControlZ` dashboard log ([Issue 24039](https://github.com/istio/istio/issues/24039))
- **Fixed** tar name to directory translation ([Issue 23635](https://github.com/istio/istio/issues/23635))
- **Improved** certificate management for multi-cluster and virtual machine setup from `samples/certs` directory to `install/tools/certs` directory
- **Improved** `pilot-agent`’s handling of client certificates when only a CA client certificate is present
- **Improved** `istiocl upgrade` to direct users to the `istio.io` website to migrate from `v1alpha1` security policies to `v1beta1` security policies
- **Fixed** release URL name for `istioctl upgrade`
- **Fixed** `k8s.overlays` for cluster resources
- **Fixed** `HTTP/HTTP2` conflict at Gateway ([Issue 24061](https://github.com/istio/istio/issues/24061) and [Issue 19690](https://github.com/istio/istio/issues/19690))
- **Fixed** Istio operator to respect the `--operatorNamespace` argument ([Issue 24073](https://github.com/istio/istio/issues/24073))
- **Fixed** Istio operator hanging when uninstalling Istio ([Issue 24038](https://github.com/istio/istio/issues/24038))
- **Fixed** TCP metadata exchange for upstream clusters that specify `http2_protocol_options` ([Issue 23907](https://github.com/istio/istio/issues/23907))
- **Added** `sideEffects` field to `MutatingWebhookConfiguration` for `istio-sidecar-injector` ([Issue 23485](https://github.com/istio/istio/issues/23485))
- **Improved** installation for replicated control planes ([Issue 23871](https://github.com/istio/istio/issues/23871))
- **Fixed** `istioctl experimental precheck` to report compatible versions of Kubernetes (1.14-1.18) ([Issue 24132](https://github.com/istio/istio/issues/24132))
- **Fixed** Istio operator namespace mismatches that caused a resource leak when pruning resources ([Issue 24222](https://github.com/istio/istio/issues/24222))
- **Fixed** SDS Agent failing to start when proxy uses file mounted certs for Gateways ([Issue 23646](https://github.com/istio/istio/issues/23646))
- **Fixed** TCP over HTTP conflicts that caused invalid configuration to be generated ([Issue 24084](https://github.com/istio/istio/issues/24084))
- **Fixed** the use of external name when remote Pilot address is a hostname ([Issue 24155](https://github.com/istio/istio/issues/24155))
- **Fixed** Istio CNI node `DaemonSet` starting when Istio CNI and `cos_containerd` are enabled on Google Kubernetes Engine (GKE) ([Issue 23643](https://github.com/istio/istio/issues/23643))
- **Fixed** Istio CNI causing pod initialization to experience a 30-40 second delay on startup when DNS unreachable ([Issue 23770](https://github.com/istio/istio/issues/23770))
- **Improved** Google Stackdriver telemetry use of UIDs with GCE VMs
- **Improved** telemetry plugins to not crash due invalid configuration ([Issue 23865](https://github.com/istio/istio/issues/23865))
- **Fixed** a proxy sidecar segfault when the response to HTTP calls by WASM filters are empty ([Issue 23890](https://github.com/istio/istio/issues/23890))
- **Fixed** a proxy sidecar segfault while parsing CEL expressions ([Issue 497](https://github.com/envoyproxy/envoy-wasm/issues/497))

**Bookinfo sample application security fixes**

We’ve updated the versions of Node.js and jQuery used in the Bookinfo sample application. Node.js has been upgraded from version 12.9 to 12.18. jQuery has been updated from version 2.1.4 to version 3.5.0. The highest rated vulnerability fixed: *HTTP request smuggling using malformed Transfer-Encoding header (Critical) (CVE-2019-15605)*



### 1.6.2

This release fixes the security vulnerability described in [our June 11th, 2020 news post](https://istio.io/latest/news/security/istio-security-2020-006).

This release note describes what’s different between Istio 1.6.2 and Istio 1.6.1.



**Security update**

- **ISTIO-SECURITY-2020-006** Excessive CPU usage when processing HTTP/2 SETTINGS frames with too many parameters, potentially leading to a denial of service.

**[CVE-2020-11080](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-11080)**: By sending a specially crafted packet, an attacker could cause the CPU to spike at 100%. This could be sent to the ingress gateway or a sidecar.





### 1.6.3

This release contains bug fixes to improve robustness. This release note describes what’s different between Istio 1.6.2 and Istio 1.6.3.



**Changes**

- **Fixed** an issue preventing the operator from recreating watched resources if they are deleted ([Issue 23238](https://github.com/istio/istio/issues/23238)).
- **Fixed** an issue where Istio crashed with the message: `proto.Message is *client.QuotaSpecBinding, not *client.QuotaSpecBinding`([Issue 24624](https://github.com/istio/istio/issues/24264)).
- **Fixed** an issue preventing operator reconciliation due to improper labels on watched resources ([Issue 23603](https://github.com/istio/istio/issues/23603)).
- **Added** support for the `k8s.v1.cni.cncf.io/networks` annotation ([Issue 24425](https://github.com/istio/istio/issues/24425)).
- **Updated** the `SidecarInjectionSpec` CRD to read the `imagePullSecret` from `.Values.global` ([Pull 24365](https://github.com/istio/istio/pull/24365)).
- **Updated** split horizon to skip gateways that resolve hostnames.
- **Fixed** `istioctl experimental metrics` to only flag error response codes as errors ([Issue 24322](https://github.com/istio/istio/issues/24322))
- **Updated** `istioctl analyze` to sort output formats.
- **Updated** gateways to use `proxyMetadata`
- **Updated** the Prometheus sidecar to use `proxyMetadata`([Issue 24415](https://github.com/istio/istio/pull/24415)).
- **Removed** invalid configuration from `PodSecurityContext` when `gateway.runAsRoot` is enabled ([Issue 24469](https://github.com/istio/istio/issues/24469)).

**Grafana addon security fixes**

We’ve updated the version of Grafana shipped with Istio from 6.5.2 to 6.7.4. This addresses a Grafana security issue, rated high, that can allow access to internal cluster resources using the Grafana avatar feature. [(CVE-2020-13379)](https://grafana.com/blog/2020/06/03/grafana-6.7.4-and-7.0.2-released-with-important-security-fix/)



### 1.6.4

This release fixes the security vulnerability described in [our June 30th, 2020 news post](https://istio.io/latest/news/security/istio-security-2020-007).

This release note describes what’s different between Istio 1.6.4 and Istio 1.6.3.



**Security update**

- **[CVE-2020-12603](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-12603)**: By sending a specially crafted packet, an attacker could cause Envoy to consume excessive amounts of memory when proxying HTTP/2 requests or responses.
  - CVSS Score: 7.0 [AV:N/AC:L/PR:N/UI:N/S:U/C:N/I:N/A:H](https://nvd.nist.gov/vuln-metrics/cvss/v3-calculator?vector=AV:N/AC:L/PR:N/UI:N/S:U/C:N/I:N/A:H&version=3.1)
- **[CVE-2020-12605](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-12605)**: An attacker could cause Envoy to consume excessive amounts of memory when processing specially crafted HTTP/1.1 packets.
  - CVSS Score: 7.0 [AV:N/AC:L/PR:N/UI:N/S:U/C:N/I:N/A:H](https://nvd.nist.gov/vuln-metrics/cvss/v3-calculator?vector=AV:N/AC:L/PR:N/UI:N/S:U/C:N/I:N/A:H&version=3.1)
- **[CVE-2020-8663](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-8663)**: An attacker could cause Envoy to exhaust file descriptors when accepting too many connections.
  - CVSS Score: 7.0 [AV:N/AC:L/PR:N/UI:N/S:U/C:N/I:N/A:H](https://nvd.nist.gov/vuln-metrics/cvss/v3-calculator?vector=AV:N/AC:L/PR:N/UI:N/S:U/C:N/I:N/A:H&version=3.1)
- **[CVE-2020-12604](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-12604)**: An attacker could cause increased memory usage when processing specially crafted packets.
  - CVSS Score: 5.3 [AV:N/AC:L/PR:N/UI:N/S:U/C:N/I:N/A:L](https://nvd.nist.gov/vuln-metrics/cvss/v3-calculator?vector=AV:N/AC:L/PR:N/UI:N/S:U/C:N/I:N/A:H&version=3.1)



### 1.6.5



This release fixes the security vulnerability described in [our July 9th, 2020 news post](https://istio.io/latest/news/security/istio-security-2020-008).

This release contains bug fixes to improve robustness. These release notes describe what’s different between Istio 1.6.5 and Istio 1.6.4.



**Security update**

- [CVE-2020-15104](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-15104)

  : When validating TLS certificates, Envoy incorrectly allows a wildcard DNS Subject Alternative Name to apply to multiple subdomains. For example, with a SAN of

   

  ```
  *.example.com
  ```

  , Envoy incorrectly allows

   

  ```
  nested.subdomain.example.com
  ```

  , when it should only allow

   

  ```
  subdomain.example.com
  ```

  .

  - CVSS Score: 6.6 [AV:N/AC:H/PR:H/UI:N/S:C/C:H/I:L/A:N/E:F/RL:O/RC:C](https://nvd.nist.gov/vuln-metrics/cvss/v3-calculator?vector=AV:N/AC:H/PR:H/UI:N/S:C/C:H/I:L/A:N/E:F/RL:O/RC:C&version=3.1)

**Changes**

- **Fixed** return the proper source name after Mixer does a lookup by IP if multiple pods have the same IP.
- **Improved** the sidecar injection control based on revision at a per-pod level ([Issue 24801](https://github.com/istio/istio/issues/24801))
- **Improved** `istioctl validate` to disallow unknown fields not included in the Open API specification ([Issue 24860](https://github.com/istio/istio/issues/24860))
- **Changed** `stsPort` to `sts_port` in Envoy’s bootstrap file.
- **Preserved** existing WASM state schema for state objects to reference it later as needed.
- **Added** `targetUri` to `stackdriver_grpc_service`.
- **Updated** WASM state to log for Access Log Service.
- **Increased** default protocol detection timeout from 100 ms to 5 s ([Issue 24379](https://github.com/istio/istio/issues/24379))
- **Removed** UDP port 53 from Istiod.
- **Allowed** setting `status.sidecar.istio.io/port` to zero ([Issue 24722](https://github.com/istio/istio/issues/24722))
- **Fixed** EDS endpoint selection for subsets with no or empty label selector. ([Issue 24969](https://github.com/istio/istio/issues/24969))
- **Allowed** `k8s.overlays` on `BaseComponentSpec`. ([Issue 24476](https://github.com/istio/istio/issues/24476))
- **Fixed** `istio-agent` to create *elliptical* curve CSRs when `ECC_SIGNATURE_ALGORITHM` is set.
- **Improved** mapping of gRPC status codes into HTTP domain for telemetry.
- **Fixed** `scaleTargetRef` naming in `HorizontalPodAutoscaler` for Istiod ([Issue 24809](https://github.com/istio/istio/issues/24809))



### 1.6.6

> This release contains a regression from 1.6.5 that prevents endpoints not associated with pods from working. Please upgrade to 1.6.7 when it is available.

This release contains bug fixes to improve robustness. This release note describes what’s different between Istio 1.6.5 and Istio 1.6.6.



**Changes**

- **Optimized** performance in scenarios with large numbers of gateways. ([Issue 25116](https://github.com/istio/istio/issues/25116))
- **Fixed** an issue where out of order events may cause the Istiod update queue to get stuck. This resulted in proxies with stale configuration.
- **Fixed** `istioctl upgrade` so that it no longer checks remote component versions when using `--dry-run`. ([Issue 24865](https://github.com/istio/istio/issues/24865))
- **Fixed** long log messages for clusters with many gateways.
- **Fixed** outlier detection to only fire on user configured errors and not depend on success rate. ([Issue 25220](https://github.com/istio/istio/issues/25220))
- **Fixed** demo profile to use port 15021 as the status port. ([Issue #25626](https://github.com/istio/istio/issues/25626))
- **Fixed** Galley to properly handle errors from Kubernetes tombstones.
- **Fixed** an issue where manually enabling TLS/mTLS for communication between a sidecar and an egress gateway did not work. ([Issue 23910](https://github.com/istio/istio/issues/23910))
- **Fixed** Bookinfo demo application to verify if a specified namespace exists and if not, use the default namespace.
- **Added** a label to the `pilot_xds` metric in order to give more information on data plane versions without scraping the data plane.
- **Added** `CA_ADDR` field to allow configuring the certificate authority address on the egress gateway configuration and fixed the `istio-certs` mount secret name.
- **Updated** Bookinfo demo application to latest versions of libraries.
- **Updated** Istio to disable auto mTLS when sending traffic to headless services without a sidecar.



### 1.6.7

This release contains bug fixes to improve robustness. This release note describes what’s different between Istio 1.6.6 and Istio 1.6.7.

**Changes**

- **Fixed** an issue which prevented endpoints not associated with pods from working. ([Issue #25974](https://github.com/istio/istio/issues/25974))



### 1.6.8



This release fixes the security vulnerability described in [our August 11th, 2020 news post](https://istio.io/latest/news/security/istio-security-2020-009).

This release contains bug fixes to improve robustness. These release notes describe what’s different between Istio 1.6.7 and Istio 1.6.8.





**Security update**

- [CVE-2020-16844](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-16844): Callers to TCP services that have a defined Authorization Policies with `DENY` actions using wildcard suffixes (e.g. `*-some-suffix`) for source principals or namespace fields will never be denied access.
- CVSS Score: 6.8 [AV:N/AC:H/PR:L/UI:N/S:U/C:H/I:H/A:N](https://nvd.nist.gov/vuln-metrics/cvss/v3-calculator?vector=AV:N/AC:H/PR:L/UI:N/S:U/C:H/I:H/A:N&version=3.1)





### 1.6.9

This release contains bug fixes to improve robustness. This release note describes what’s different between Istio 1.6.8 and Istio 1.6.9.



**Changes**

- **Added** istioctl analyzer to detect when Destination Rules do not specify `caCertificates` ([Istio #25652](https://github.com/istio/istio/issues/25652))
- **Added** missing `telemetry.loadshedding.*` options to mixer container arguments
- **Fixed** HTTP match request without headers conflict
- **Fixed** Istio operator to watch multiple namespaces ([Istio #26317](https://github.com/istio/istio/issues/26317))
- **Fixed** `EDS` cache when an endpoint appears after its service resource ([Istio #26983](https://github.com/istio/istio/issues/26983))
- **Fixed** `istioctl remove-from-mesh` not removing init containers on CNI installations.
- **Fixed** `istioctl` `add-to-mesh` and `remove-from-mesh` commands from affecting `OwnerReferences` ([Istio #26720](https://github.com/istio/istio/issues/26720))
- **Fixed** cleaning up of service information when the cluster secret is deleted
- **Fixed** egress gateway ports binding to 80⁄443 due to user permissions
- **Fixed** gateway listeners created with traffic direction outbound to be drained properly on exit
- **Fixed** headless services not updating listeners ([Istio #26617](https://github.com/istio/istio/issues/26617))
- **Fixed** inaccurate `endpointsPendingPodUpdate` metric
- **Fixed** ingress SDS from not getting secret update ([Istio #18912](https://github.com/istio/istio/issues/18912))
- **Fixed** ledger capacity size
- **Fixed** operator to update service monitor due to invalid permissions ([Istio #26961](https://github.com/istio/istio/issues/26961))
- **Fixed** regression in gateway name resolution ([Istio 26264](https://github.com/istio/istio/issues/26264))
- **Fixed** rotated certificates not being stored to `/etc/istio-certs` `VolumeMount` ([Istio #26821](https://github.com/istio/istio/issues/26821))
- **Fixed** trust domain validation in transport socket level ([Istio #26435](https://github.com/istio/istio/issues/26435))
- **Improved** specifying network for a cluster without `meshNetworks` also being configured
- **Improved** the cache readiness state with TTL ([Istio #26418](https://github.com/istio/istio/issues/26418))
- **Updated** SDS timeout to fetch workload certificates to `0s`
- **Updated** `app_containers` to use comma separated values for container specification
- **Updated** default protocol sniffing timeout to `5s` ([Istio #24379](https://github.com/istio/istio/issues/24379))





### 1.6.10

This release contains bug fixes to improve robustness. This release note describes what’s different between Istio 1.6.9 and Istio 1.6.10.



**Changes**

- **Added** quotes in log sampling config and Stackdriver test
- **Fixed** gateways missing endpoint instances of headless services ([Istio #27041](https://github.com/istio/istio/issues/27041))
- **Fixed** locality load balancer settings were applied to inbound clusters unnecessarily ([Istio #27293](https://github.com/istio/istio/issues/27293))
- **Fixed** unbounded cardinality of Istio metrics for `CronJob` workloads ([Istio #24058](https://github.com/istio/istio/issues/24058))
- **Improved** envoy to cache readiness value
- **Removed** deprecated help message for `istioctl manifest migrate` ([Istio #26230](https://github.com/istio/istio/issues/26230))





### 1.6.11

This release fixes the security vulnerability described in [our September 29 post](https://istio.io/latest/news/security/istio-security-2020-010).



**Security update**

- [CVE-2020-25017](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-25017): In some cases, Envoy only considers the first value when multiple headers are present. Also, Envoy does not replace all existing occurrences of a non-inline header.
  - **CVSS Score**: 8.3 [AV:N/AC:L/PR:N/UI:N/S:C/C:L/I:L/A:L](https://nvd.nist.gov/vuln-metrics/cvss/v3-calculator?vector=AV:N/AC:L/PR:N/UI:N/S:C/C:L/I:L/A:L&version=3.1)





### 1.6.12

This release contains bug fixes to improve robustness. This release note describes what’s different between Istio 1.6.11 and Istio 1.6.12



**Changes**

- **Added** ability to configure domain suffix for multicluster installation ([Issue #27300](https://github.com/istio/istio/issues/27300))
- **Added** support for `securityContext` in the Kubernetes settings for the operator API. ([Issue #26275](https://github.com/istio/istio/issues/26275))
- **Fixed** an issue preventing calls to wildcard (such as `*.example.com`) domains when a port is set in the `Host` header. ([Issue #25350](https://github.com/istio/istio/issues/25350))





### 1.6.13

This release contains bug fixes to improve robustness. This release note describes what’s different between Istio 1.6.12 and Istio 1.6.13



**Changes**

- **Fixed** an issue where Istiod’s `cacert.pem` was under the `testdata` directory ([Issue #27574](https://github.com/istio/istio/issues/27574))
- **Fixed** Pilot agent app probe connection leak. ([Issue #27726](https://github.com/istio/istio/issues/27726))





### 1.6.14

This release contains bug fixes to improve robustness. This release note describes what’s different between Istio 1.6.13 and Istio 1.6.14



**Changes**

- **Fixed** HPA settings for telemetry being overridden by the inline replicas. ([Issue #28916](https://github.com/istio/istio/issues/28916))
- **Fixed** an issue that caused very high memory usage with a large number of `ServiceEntries`. ([Issue #25531](https://github.com/istio/istio/issues/25531))




## release-1.7



参见： 

* [Istio_Change Notes_1.7.html](_resource/Istio_Change Notes_1.7.html)

* [Istio_Upgrade Notes_1.7.html](_resource/Istio_Upgrade Notes_1.7.html)



### 1.7.1

**Changes**

- **Added** Envoy [ext `authz` and gRPC access log API support](https://github.com/istio/istio/wiki/Enabling-Envoy-Authorization-Service-and-gRPC-Access-Log-Service-With-Mixer) in Mixer, which makes Mixer based configuration and out of process adapter still work after upgrading to future versions of Istio. ([Issue #23580](https://github.com/istio/istio/issues/23580))
- **Fixed** the `istioctl x authz check` command to work properly with the v1beta1 AuthorizationPolicy. ([PR #26625](https://github.com/istio/istio/pull/26625))
- **Fixed** unreachable endpoints for non-injected workloads across networks by removing them. ([Issue #26517](https://github.com/istio/istio/issues/26517))
- **Fixed** enabling hold application until proxy starts feature flag breaking rewriting application probe logic. ([Issue #26873](https://github.com/istio/istio/issues/26873))
- **Fixed** deleting the remote-secret for multicluster installation removes remote endpoints. ([Issue #27187](https://github.com/istio/istio/issues/27187))
- **Fixed** missing endpoints when Service is populated later than Endpoints.
- **Fixed** an issue causing headless Service updates to be missed ([Issue #26617](https://github.com/istio/istio/issues/26617)). ([Issue #26617](https://github.com/istio/istio/issues/26617))
- **Fixed** an issue with Kiali RBAC permissions which prevented its deployment from working properly. ([Issue #27109](https://github.com/istio/istio/issues/27109))
- **Fixed** an issue where `remove-from-mesh` did not remove the init containers when using Istio CNI ([Issue #26938](https://github.com/istio/istio/issues/26938))
- **Fixed** Kiali to use anonymous authentication strategy since newer versions have removed the login authentication strategy.



### 1.7.2

**Changes**

- **Fixed** locality load balancer settings being applied to inbound clusters unnecessarily. ([Issue #27293](https://github.com/istio/istio/issues/27293))
- **Fixed** unbounded cardinality of Istio metrics for `CronJob` job workloads. ([Issue #24058](https://github.com/istio/istio/issues/24058))
- **Fixed** setting the `ISTIO_META_REQUESTED_NETWORK_VIEW` environment variable for a proxy will filter out endpoints that aren’t part of the comma-separated list of networks. This should be set to the local-network on the ingress-gateway used for cross-network traffic to prevent odd load balancing behavior. ([Issue #26293](https://github.com/istio/istio/issues/26293))
- **Fixed** issues with `WorkloadEntry` when the Service or `WorkloadEntry` is updated after creation. ([Issue #27183](https://github.com/istio/istio/issues/27183)),([Issue #27151](https://github.com/istio/istio/issues/27151)),([Issue #27185](https://github.com/istio/istio/issues/27185))



### 1.7.3



**Security update**

- [CVE-2020-25017](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-25017)

  : In some cases, Envoy only considers the first value when multiple headers are present. Also, Envoy does not replace all existing occurrences of a non-inline header.

  - **CVSS Score**: 8.3 [AV:N/AC:L/PR:N/UI:N/S:C/C:L/I:L/A:L](https://nvd.nist.gov/vuln-metrics/cvss/v3-calculator?vector=AV:N/AC:L/PR:N/UI:N/S:C/C:L/I:L/A:L&version=3.1)



### 1.7.4



**Changes**

- **Improved** TLS configuration on sidecar server-side inbound paths to enforce TLS 2.0 version along with recommended cipher suites. This is disabled by default and can enabled by setting the environment variable `PILOT_SIDECAR_ENABLE_INBOUND_TLS_V2` to true.
- **Added** ability to configure domain suffix for multicluster installation. ([Issue #27300](https://github.com/istio/istio/issues/27300))
- **Added** `istioctl proxy-status` and other commands will attempt to contact the control plane using both port-forwarding and exec before giving up, restoring functionality on clusters that do not offer port-forwarding to the control plane. ([Issue #27421](https://github.com/istio/istio/issues/27421))
- **Added** support for `securityContext` in the Kubernetes settings for the operator API. ([Issue #26275](https://github.com/istio/istio/issues/26275))
- **Added** support for revision based istiod to istioctl version. ([Issue #27756](https://github.com/istio/istio/issues/27756))
- **Fixed** deleting the remote-secret for multicluster installation removes remote endpoints.
- **Fixed** an issue that Istiod’s `cacert.pem` is under the `testdata` directory. ([Issue #27574](https://github.com/istio/istio/issues/27574))
- **Fixed** `PodDisruptionBudget` of `istio-egressgateway` does not match any pods. ([Issue #27730](https://github.com/istio/istio/issues/27730))
- **Fixed** an issue preventing calls to wildcard (such as *.example.com) domains when a port is set in the Host header.
- **Fixed** an issue periodically causing a deadlock in Pilot’s `syncz` debug endpoint.
- **Removed** deprecated `outboundTrafficPolicy` from global values. ([Issue #27494](https://github.com/istio/istio/issues/27494))



### 1.7.5



**Changes**

- **Fixed** pilot agent app probe connection leak. ([Issue #27726](https://github.com/istio/istio/issues/27726))
- **Fixed** how `install-cni` applies `istio-cni` plugin configuration. Previously, new configurations would be appended to the list. This has been changed to remove existing `istio-cni` plugins from the CNI config before inserting new plugins. ([Issue #27771](https://github.com/istio/istio/issues/27771))
- **Fixed** when a node has multiple IP addresses (e.g., a VM in the mesh expansion scenario). Istio Proxy will now bind inbound listeners to the first applicable address in the list rather than to the last one. ([Issue #28269](https://github.com/istio/istio/issues/28269))
- **Fixed** Istio to not run gateway secret fetcher when proxy is configured with `FILE_MOUNTED_CERTS`.
- **Fixed** multicluster `EnvoyFilter` to have valid configuration following the underlying changes in Envoy’s API. ([Issue #27909](https://github.com/istio/istio/issues/27909))
- **Fixed** an issue causing a short spike in errors during in place upgrades from Istio 1.6 to 1.7. Previously, the xDS version would be upgraded automatically from xDS v2 to xDS v3. This caused downtime with upgrades from Istio 1.6 to Istio 1.7. This has been fixed so that these upgrades no longer cause downtime. Note that, as a trade off, upgrading from Istio 1.7.x to Istio 1.7.5 still causes downtime in any existing 1.6 proxies; if you are in this scenario you may set the `PILOT_ENABLE_TLS_XDS_DYNAMIC_TYPES` environment variable to false in Istiod to retain the previous behavior. ([Issue #28120](https://github.com/istio/istio/issues/28120))
- **Fixed** missing listeners on a VM when the VM sidecar is connected to `istiod` but a `WorkloadEntry` is registered later. ([Issue #28743](https://github.com/istio/istio/issues/28743))

**Upgrade Notice**

When upgrading your Istio data plane from 1.7.x (where x < 5) to 1.7.5 or newer, you may observe connectivity issues between your gateway and your sidecars or among your sidecars with 503 errors in the log. This happens when 1.7.5+ proxies send HTTP 1xx or 204 response codes with headers that 1.7.x proxies reject. To fix this, upgrade all your proxies (gateways and sidecars) to 1.7.5+ as soon as possible. ([Issue 29427](https://github.com/istio/istio/issues/29427), [More information](https://github.com/istio/istio/pull/28450))



### 1.7.6



**Changes**

- **Fixed** an issue causing telemetry HPA settings to be overridden by the inline replicas. ([Issue #28916](https://github.com/istio/istio/issues/28916))
- **Fixed** an issue where a delegate `VirtualService` change would not trigger an xDS push. ([Issue #29123](https://github.com/istio/istio/issues/29123))
- **Fixed** an issue that caused a very high memory usage with a large number of `ServiceEntry`s. ([Issue #25531](https://github.com/istio/istio/issues/25531))


