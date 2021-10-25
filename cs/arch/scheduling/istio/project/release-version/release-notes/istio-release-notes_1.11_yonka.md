

## 1.11.x Releases

Announcements for the 1.11 release and its associated patch releases.

| [Istio 1.11.4](https://istio.io/latest/news/releases/1.11.x/announcing-1.11.4/) | October 14, 2021   |
| ------------------------------------------------------------ | ------------------ |
| [Istio 1.11.3](https://istio.io/latest/news/releases/1.11.x/announcing-1.11.3/) | September 23, 2021 |
| [Istio 1.11.2](https://istio.io/latest/news/releases/1.11.x/announcing-1.11.2/) | September 2, 2021  |
| [Istio 1.11.1](https://istio.io/latest/news/releases/1.11.x/announcing-1.11.1/) | August 24, 2021    |
| [Istio 1.11](https://istio.io/latest/news/releases/1.11.x/announcing-1.11/) | August 12, 2021    |



Istio 1.11.0 is officially supported on Kubernetes versions `1.18.0` to `1.22.x`.

Here are some highlights for this release:

### CNI plugin (Beta)

By default Istio injects an [init container](https://kubernetes.io/docs/concepts/workloads/pods/init-containers/) in pods deployed in the mesh. The `istio-init` container sets up the pod network traffic redirection to/from the Istio sidecar proxy using iptables. This requires the user or service account deploying pods in the mesh to have sufficient permissions to deploy [containers with the `NET_ADMIN` and `NET_RAW` capabilities](https://kubernetes.io/docs/tasks/configure-pod-container/security-context/#set-capabilities-for-a-container). Requiring Istio users to have elevated Kubernetes privileges can be problematic for security compliance within an organization. The Istio CNI plugin is a replacement for the `istio-init` container that performs the same networking functionality, but without requiring Istio users to enable elevated Kubernetes permissions.

The CNI plugin can be chained with other plugins, and supports most hosted Kubernetes implementations.

In this release, we have promoted the CNI plugin functionality to Beta by improving our documentation and testing to ensure users can enable this feature safely in production. [Learn how to install Istio with the CNI plugin.](https://istio.io/latest/docs/setup/additional-setup/cni/)

### External control plane (Beta)

Last year we introduced a [new deployment model for Istio](https://istio.io/latest/blog/2020/new-deployment-model/) where the control plane for a cluster was managed outside of that cluster. This allows for separation of concerns between a mesh owner, who administers the control plane, and the mesh users, who deploy and configure services in the mesh. An external control plane, running in a separate cluster, can control a single data plane cluster or more than one cluster of a multicluster mesh.

In 1.11, this feature has been promoted to Beta. [Learn how you can set up a mesh with an external control plane](https://istio.io/latest/docs/setup/install/external-controlplane/).

### Gateway injection

Istio provides gateways as a way to interface with the outside world. You can deploy [ingress gateways](https://istio.io/latest/docs/tasks/traffic-management/ingress/ingress-control/), for incoming traffic originating outside your cluster, and [egress gateways](https://istio.io/latest/docs/tasks/traffic-management/egress/egress-gateway/), for outgoing traffic from your applications to services deployed outside your cluster.

In the past, an Istio version would deploy a gateway as a Deployment which had a completely separate proxy configuration to all the rest of the sidecar proxies in the cluster. This made management and upgrade of the gateway complex, especially when multiple gateways were deployed in the cluster. One common issue was that settings from the control plane passed down to sidecar proxies and the gateways could drift, causing unexpected issues.

Gateway injection moves the management of gateways to the same method as sidecar proxies. Configuration that you set on your proxies globally will apply to your gateways, and complex configurations that weren’t possible (for example, running a gateway as a DaemonSet) are now easy. You can also update your gateways to the latest version after a cluster upgrade simply by restarting the pods.

In addition to these changes, we have released new [Installing Gateways](https://istio.io/latest/docs/setup/additional-setup/gateway/) documentation, which covers best practices for installation, management, and upgrade of gateways.

### Updates to revision and tag deployments

In Istio 1.6 we added support for running multiple control planes simultaneously, which allows you to do a [canary deployment of a new Istio version](https://istio.io/latest/blog/2020/multiple-control-planes/). In 1.10, we introduced [revision tags](https://istio.io/latest/blog/2021/revision-tags/), which lets you mark a revision as “production” or “testing” and minimizes the chance of error when upgrading.

The `istioctl tag` command has graduated out of experimental in 1.11. You can also now specify a default revision for the control plane. This helps further simplify the canary upgrade from a non-revisioned control plane to a new version.

We also fixed an [outstanding issue](https://github.com/istio/istio/issues/28880) with upgrades - you can safely perform a canary upgrade of your control plane regardless of whether or not it was installed using a revision.

To improve the sidecar injection experience, `istio-injection` and `sidecar.istio.io/inject` labels were introduced. We recommend you to switch to using injection labels, as they perform better than injection annotations. We intend to deprecate the injection annotations in a future release.

### Kubernetes Multi-cluster Services (MCS) support (Experimental)

The Kubernetes project is building an [multi-cluster services API](https://github.com/kubernetes/enhancements/tree/master/keps/sig-multicluster/1645-multi-cluster-services-api) that allows service owners or mesh admins to control the export of services and their endpoints across the mesh.

Istio 1.11 adds experimental support for multi-cluster services. Once enabled, the discoverability of service endpoints is determined by client location and whether the service has been exported. Endpoints residing within the same cluster as the client will always be discoverable. Endpoints within a different cluster, however, will only be discoverable by the client if they were exported to the mesh.

Note that Istio does not yet support the behavior for the `cluster.local` and `clusterset.local` hosts as defined by the MCS spec. Clients should continue to address services using either `cluster.local` or `svc.namespace`.

This is the first phase in [our plan](https://docs.google.com/document/d/1K8hvQ83UcJ9a7U8oqXIefwr6pFJn-VBEi40Ak-fwQtk/edit) to support MCS. Stay tuned!

### Sneak peek: new APIs

A number of Istio features can only be configured by [`EnvoyFilter`](https://istio.io/latest/docs/reference/config/networking/envoy-filter/), which allows you to set proxy configuration. We’re working on new APIs for common use cases - such as configuring telemetry settings and WebAssembly (Wasm) extension deployment, and you can expect to see these become available to users in the 1.12 release. If you’re interested in helping us test the implementations as they are built, [please join the appropriate working group meeting](https://github.com/istio/community/blob/master/WORKING-GROUPS.md).

### Join the Istio community

You can also join the conversation at [Discuss Istio](https://discuss.istio.io/), or join our [Slack workspace](https://slack.istio.io/).

Would you like to get involved? Find and join one of our [Working Groups](https://github.com/istio/community/blob/master/WORKING-GROUPS.md) and help improve Istio.

### Istio 1.11 Upgrade Survey

If you have completed your upgrade to Istio 1.11, we would like to hear from you! Please take a few minutes to respond to our brief [survey](https://forms.gle/pquMQs4Qxujus6jB9) to tell us how we’re doing.

### See also

- [Istio 1.11 Change Notes](https://istio.io/latest/news/releases/1.11.x/announcing-1.11/change-notes/)
- [Istio 1.11 Upgrade Notes](https://istio.io/latest/news/releases/1.11.x/announcing-1.11/upgrade-notes/)

### 

## versions



### [1.11.1](https://istio.io/latest/news/releases/1.11.x/announcing-1.11.1/)

**Security updates**

- **[CVE-2021-39155](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2CVE-2021-39155])** **([CVE-2021-32779](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-32779))**: Istio authorization policies incorrectly compare the host header in a case-sensitive manner against RFC 4343 with states it should be case-insensitive. Envoy routes the request hostname in a case-insensitive way which means the authorization policy could be bypassed.
  - **CVSS Score**: 8.3 [CVSS:3.1/AV:N/AC:L/PR:N/UI:R/S:U/C:H/I:H/A:L](https://www.first.org/cvss/calculator/3.1#CVSS:3.1/AV:N/AC:L/PR:N/UI:R/S:U/C:H/I:H/A:L)
- **[CVE-2021-39156](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2CVE-2021-39156])**: Istio contains a remotely exploitable vulnerability where an HTTP request with a fragment (e.g. #Section) in the path may bypass Istio’s URI path based authorization policies.
  - **CVSS Score**: 8.1 [CVSS:3.1/AV:N/AC:L/PR:N/UI:R/S:U/C:H/I:H/A:N](https://www.first.org/cvss/calculator/3.1#CVSS:3.1/AV:N/AC:L/PR:N/UI:R/S:U/C:H/I:H/A:N)

**Envoy Security updates**

- [CVE-2021-32777](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-32777) (CVSS score 8.6, High): Envoy contains a remotely exploitable vulnerability where an HTTP request with multiple value headers may bypass authorization policies when using the `ext_authz` extension.
- [CVE-2021-32778](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-32778) (CVSS score 8.6, High): Envoy contains a remotely exploitable vulnerability where an Envoy client opening and then resetting a large number of HTTP/2 requests may lead to excessive CPU consumption.
- [CVE-2021-32780](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-32780) (CVSS score 8.6, High): Envoy contains a remotely exploitable vulnerability where an untrusted upstream service may cause Envoy to terminate abnormally by sending the GOAWAY frame followed by the SETTINGS frame with the `SETTINGS_MAX_CONCURRENT_STREAMS` parameter set to 0. Note: this vulnerability does not impact downstream client connections.
- [CVE-2021-32781](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-32781) (CVSS score 8.6, High): Envoy contains a remotely exploitable vulnerability that affects Envoy’s decompressor, json-transcoder or grpc-web extensions or proprietary extensions that modify and increase the size of request or response bodies. Modifying and increasing the size of the body in an Envoy’s extension beyond internal buffer size may lead to Envoy accessing deallocated memory and terminating abnormally.



### [1.11.2](https://istio.io/latest/news/releases/1.11.x/announcing-1.11.2/)

**Changes**

- **Improved** `istioctl install` to give more details during installation failures.
- **Added** support for gRPC configuring workloads via xDS without an Envoy proxy.
- **Added** two mutually-exclusive flags to `istioctl x workload entry configure`
  - **`--internal-ip`** configures the VM workload with a private IP address used for workload auto registration and health probes.
  - **`--external-ip`** configures the VM workload with a public IP address used for workload auto registration. Meanwhile, it configures health probes to be performed through localhost by setting the environment variable `REWRITE_PROBE_LEGACY_LOCALHOST_DESTINATION` to true. ([Issue #34411](https://github.com/istio/istio/issues/34411))
- **Added** topology label `topology.istio.io/network` to `IstioEndpoint` if it does not exist in pod/workload label.
- **Added** a configuration `FILE_DEBOUNCE_DURATION` that allows users to configure the duration SDS server should wait after it sees first file change event. This is useful in File mounted certificate flows to ensure key and cert are fully written before they are pushed to Envoy. Default is `100ms`.
- **Fixed** unexpected info logs for Istio when using command line tool `istioctl profile diff` and `istioctl profile dump`.
- **Fixed** issue of deployment analyzer ignoring service namespaces during the analysis process.
- **Fixed** `DestinationRule` updates not triggering an update for `AUTO_PASSTHROUGH` listeners on gateways. ([Issue #34944](https://github.com/istio/istio/issues/34944))



### [1.11.3](https://istio.io/latest/news/releases/1.11.x/announcing-1.11.3/)

**Changes**

- **Updated** to allow specifying NICs that bypassing traffic capture in Istio iptables. ([Issue #34753](https://github.com/istio/istio/issues/34753))
- **Added** values to the Istio Gateway Helm charts for configuring annotations on the `ServiceAccount`. Can be used to enable [IAM Roles for Service Accounts](https://docs.aws.amazon.com/eks/latest/userguide/iam-roles-for-service-accounts.html) on AWS EKS.
- **Fixed** `istioctl analyze` command to not output [IST0132] message when analyzing the gateway of the virtual service. ([Issue #34653](https://github.com/istio/istio/issues/34653))
- **Fixed** a bug using a Service’s pointer address to get its instances in the case where a sidecar’s egress listener has a port.
- **Fixed** a bug in the “image: auto” analyzer causing it to fail to take into account the Deployment namespace. ([Issue #34929](https://github.com/istio/istio/issues/34929))
- **Fixed** `istioctl x workload` command output to set the correct `discoveryAddress` for revisioned control-planes. ([Issue #34058](https://github.com/istio/istio/issues/34058))
- **Fixed** gateway analyzer message reporting if there is no selector in the gateway spec. ([Issue #35093](https://github.com/istio/istio/issues/35093))
- **Fixed** an issue causing memory to not be freed after XDS clients disconnect.
- **Fixed** an issue occurring when multiple `VirtualServices` with the same name exist in different namespaces. ([Issue #35127](https://github.com/istio/istio/issues/35127))



### [1.11.4](https://istio.io/latest/news/releases/1.11.x/announcing-1.11.4/)

**Changes**

- **Fixed** VMs are able to use a revisioned control plane specified by `--revision` on the `istioctl x workload entry` command.
- **Fixed** an issue when creating a Service and Gateway at the same time, causing the Service to be ignored. ([Issue #35172](https://github.com/istio/istio/issues/35172))
- **Fixed** an issue causing stale endpoints for service entry selecting pods ([Issue #35404](https://github.com/istio/istio/issues/35404))





