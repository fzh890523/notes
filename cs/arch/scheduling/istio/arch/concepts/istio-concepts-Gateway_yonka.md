

解释



* 通过`selector`来选择一些workloads（上面的proxies）作为gateway

  一样的，通过labels方式

* 通过`servers`指定gateway提供哪些服务(server，not service) or 以哪些形式对外提供服务

  * `Port` 指定端口、协议等

    协议目前支持： `HTTP|HTTPS|GRPC|HTTP2|MONGO|TCP|TLS`，对于TLS，会根据SNI来做路由


### yaml example



```yaml
apiVersion: networking.istio.io/v1alpha3
kind: Gateway
metadata:
  name: ext-host-gwy
spec:
  selector:
    app: my-gateway-controller
  servers:
  - port:
      number: 443
      name: https
      protocol: HTTPS
    hosts:
    - ext-host.example.com
    tls:
      mode: SIMPLE
      serverCertificate: /tmp/tls.crt
      privateKey: /tmp/tls.key
```



```yaml
apiVersion: networking.istio.io/v1alpha3
kind: VirtualService
metadata:
  name: virtual-svc
spec:
  hosts:
  - ext-host.example.com
  gateways:
    - ext-host-gwy
```



### pb



```protobuf
message Gateway {
  // A list of server specifications.
  repeated Server servers = 1 [(google.api.field_behavior) = REQUIRED];

  // One or more labels that indicate a specific set of pods/VMs
  // on which this gateway configuration should be applied. The scope of
  // label search is restricted to the configuration namespace in which the
  // the resource is present. In other words, the Gateway resource must
  // reside in the same namespace as the gateway workload instance.
  map<string, string> selector = 2 [(google.api.field_behavior) = REQUIRED];
}
```



```protobuf
// `Server` describes the properties of the proxy on a given load balancer
// port. For example,
//
// ```yaml
// apiVersion: networking.istio.io/v1alpha3
// kind: Gateway
// metadata:
//   name: my-ingress
// spec:
//   selector:
//     app: my-ingress-gateway
//   servers:
//   - port:
//       number: 80
//       name: http2
//       protocol: HTTP2
//     hosts:
//     - "*"
// ```
//
// Another example
//
// ```yaml
// apiVersion: networking.istio.io/v1alpha3
// kind: Gateway
// metadata:
//   name: my-tcp-ingress
// spec:
//   selector:
//     app: my-tcp-ingress-gateway
//   servers:
//   - port:
//       number: 27018
//       name: mongo
//       protocol: MONGO
//     hosts:
//     - "*"
// ```
//
// The following is an example of TLS configuration for port 443
//
// ```yaml
// apiVersion: networking.istio.io/v1alpha3
// kind: Gateway
// metadata:
//   name: my-tls-ingress
// spec:
//   selector:
//     app: my-tls-ingress-gateway
//   servers:
//   - port:
//       number: 443
//       name: https
//       protocol: HTTPS
//     hosts:
//     - "*"
//     tls:
//       mode: SIMPLE
//       serverCertificate: /etc/certs/server.pem
//       privateKey: /etc/certs/privatekey.pem
// ```
message Server {
  // The Port on which the proxy should listen for incoming
  // connections.
  Port port = 1 [(google.api.field_behavior) = REQUIRED];

  // $hide_from_docs
  // The ip or the Unix domain socket to which the listener should be bound
  // to. Format: `x.x.x.x` or `unix:///path/to/uds` or `unix://@foobar`
  // (Linux abstract namespace). When using Unix domain sockets, the port
  // number should be 0.
  string bind = 4;

  // One or more hosts exposed by this gateway.
  // While typically applicable to
  // HTTP services, it can also be used for TCP services using TLS with SNI.
  // A host is specified as a `dnsName` with an optional `namespace/` prefix.
  // The `dnsName` should be specified using FQDN format, optionally including
  // a wildcard character in the left-most component (e.g., `prod/*.example.com`).
  // Set the `dnsName` to `*` to select all `VirtualService` hosts from the
  // specified namespace (e.g.,`prod/*`).
  //
  // The `namespace` can be set to `*` or `.`, representing any or the current
  // namespace, respectively. For example, `*/foo.example.com` selects the
  // service from any available namespace while `./foo.example.com` only selects
  // the service from the namespace of the sidecar. The default, if no `namespace/`
  // is specified, is `*/`, that is, select services from any namespace.
  // Any associated `DestinationRule` in the selected namespace will also be used.
  //
  // A `VirtualService` must be bound to the gateway and must have one or
  // more hosts that match the hosts specified in a server. The match
  // could be an exact match or a suffix match with the server's hosts. For
  // example, if the server's hosts specifies `*.example.com`, a
  // `VirtualService` with hosts `dev.example.com` or `prod.example.com` will
  // match. However, a `VirtualService` with host `example.com` or
  // `newexample.com` will not match.
  //
  // NOTE: Only virtual services exported to the gateway's namespace
  // (e.g., `exportTo` value of `*`) can be referenced.
  // Private configurations (e.g., `exportTo` set to `.`) will not be
  // available. Refer to the `exportTo` setting in `VirtualService`,
  // `DestinationRule`, and `ServiceEntry` configurations for details.
  repeated string hosts = 2 [(google.api.field_behavior) = REQUIRED];

  message TLSOptions {
    // If set to true, the load balancer will send a 301 redirect for all
    // http connections, asking the clients to use HTTPS.
    bool https_redirect = 1;

    // TLS modes enforced by the proxy
    enum TLSmode {
      // The SNI string presented by the client will be used as the match
      // criterion in a VirtualService TLS route to determine the
      // destination service from the service registry.
      PASSTHROUGH = 0;

      // Secure connections with standard TLS semantics.
      SIMPLE = 1;

      // Secure connections to the downstream using mutual TLS by presenting
      // server certificates for authentication.
      MUTUAL = 2;

      // Similar to the passthrough mode, except servers with this TLS mode
      // do not require an associated VirtualService to map from the SNI
      // value to service in the registry. The destination details such as
      // the service/subset/port are encoded in the SNI value. The proxy
      // will forward to the upstream (Envoy) cluster (a group of
      // endpoints) specified by the SNI value. This server is typically
      // used to provide connectivity between services in disparate L3
      // networks that otherwise do not have direct connectivity between
      // their respective endpoints. Use of this mode assumes that both the
      // source and the destination are using Istio mTLS to secure traffic.
      AUTO_PASSTHROUGH = 3;

      // Secure connections from the downstream using mutual TLS by presenting
      // server certificates for authentication.
      // Compared to Mutual mode, this mode uses certificates, representing
      // gateway workload identity, generated automatically by Istio for
      // mTLS authentication. When this mode is used, all other fields in
      // `TLSOptions` should be empty.
      ISTIO_MUTUAL = 4;
    };

    // Optional: Indicates whether connections to this port should be
    // secured using TLS. The value of this field determines how TLS is
    // enforced.
    TLSmode mode = 2;

    // REQUIRED if mode is `SIMPLE` or `MUTUAL`. The path to the file
    // holding the server-side TLS certificate to use.
    string server_certificate = 3;

    // REQUIRED if mode is `SIMPLE` or `MUTUAL`. The path to the file
    // holding the server's private key.
    string private_key = 4;

    // REQUIRED if mode is `MUTUAL`. The path to a file containing
    // certificate authority certificates to use in verifying a presented
    // client side certificate.
    string ca_certificates = 5;

    // The credentialName stands for a unique identifier that can be used
    // to identify the serverCertificate and the privateKey. The
    // credentialName appended with suffix "-cacert" is used to identify
    // the CaCertificates associated with this server. Gateway workloads
    // capable of fetching credentials from a remote credential store such
    // as Kubernetes secrets, will be configured to retrieve the
    // serverCertificate and the privateKey using credentialName, instead
    // of using the file system paths specified above. If using mutual TLS,
    // gateway workload instances will retrieve the CaCertificates using
    // credentialName-cacert. The semantics of the name are platform
    // dependent.  In Kubernetes, the default Istio supplied credential
    // server expects the credentialName to match the name of the
    // Kubernetes secret that holds the server certificate, the private
    // key, and the CA certificate (if using mutual TLS). Set the
    // `ISTIO_META_USER_SDS` metadata variable in the gateway's proxy to
    // enable the dynamic credential fetching feature.
    string credential_name = 10;

    // A list of alternate names to verify the subject identity in the
    // certificate presented by the client.
    repeated string subject_alt_names = 6;
    
    // An optional list of base64-encoded SHA-256 hashes of the SKPIs of
    // authorized client certificates.
    // Note: When both verify_certificate_hash and verify_certificate_spki
    // are specified, a hash matching either value will result in the
    // certificate being accepted.
    repeated string verify_certificate_spki = 11;

    // An optional list of hex-encoded SHA-256 hashes of the
    // authorized client certificates. Both simple and colon separated
    // formats are acceptable.
    // Note: When both verify_certificate_hash and verify_certificate_spki
    // are specified, a hash matching either value will result in the
    // certificate being accepted.
    repeated string verify_certificate_hash = 12;

    // TLS protocol versions.
    enum TLSProtocol {
      // Automatically choose the optimal TLS version.
      TLS_AUTO = 0;

      // TLS version 1.0
      TLSV1_0 = 1;

      // TLS version 1.1
      TLSV1_1 = 2;

      // TLS version 1.2
      TLSV1_2 = 3;

      // TLS version 1.3
      TLSV1_3 = 4;
    }

    // Optional: Minimum TLS protocol version.
    TLSProtocol min_protocol_version = 7;

    // Optional: Maximum TLS protocol version.
    TLSProtocol max_protocol_version = 8;

    // Optional: If specified, only support the specified cipher list.
    // Otherwise default to the default cipher list supported by Envoy.
    repeated string cipher_suites = 9;
  }

  // Set of TLS related options that govern the server's behavior. Use
  // these options to control if all http requests should be redirected to
  // https, and the TLS modes to use.
  TLSOptions tls = 3;

  // The loopback IP endpoint or Unix domain socket to which traffic should
  // be forwarded to by default. Format should be `127.0.0.1:PORT` or
  // `unix:///path/to/socket` or `unix://@foobar` (Linux abstract namespace).
  string default_endpoint = 5;
}
```




