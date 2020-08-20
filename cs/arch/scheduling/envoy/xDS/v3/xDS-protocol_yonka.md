
ref: 

* [envoy api-docs xds_protocol](https://www.envoyproxy.io/docs/envoy/latest/api-docs/xds_protocol)


# 协议交互

## **xDS**

* `DiscoveryRequest`

* `DiscoveryResponse`

## **incremental xDS**

* `DeltaDiscoveryRequest`

* `DeltaDiscoveryResponse`

# 配置内容

## 启动配置: istio template

```json
  "dynamic_resources": {
    "lds_config": {
      "ads": {}
    },
    "cds_config": {
      "ads": {}
    },
    "ads_config": {
      "api_type": "GRPC",
      "grpc_services": [
        {
          "envoy_grpc": {
            "cluster_name": "xds-grpc"
          }
        }
      ]
    }
  },
  "static_resources": {
    "clusters": [
      {
        "name": "prometheus_stats",
        "type": "STATIC",
        "connect_timeout": "0.250s",
        "lb_policy": "ROUND_ROBIN",
        "hosts": [
          {
            "socket_address": {
              "protocol": "TCP",
              "address": "127.0.0.1",
              "port_value": 15000
            }
          }
        ]
      },
      {
        "name": "xds-grpc",
        "type": "STRICT_DNS",
        "dns_lookup_family": "V4_ONLY",
        "connect_timeout": "1s",
        "lb_policy": "ROUND_ROBIN",

        "hosts": [
          {
            "socket_address": {"address": "localhost", "port_value": 15010}
          }
        ],
        "circuit_breakers": {
          "thresholds": [
            {
              "priority": "DEFAULT",
              "max_connections": 100000,
              "max_pending_requests": 100000,
              "max_requests": 100000
            },
            {
              "priority": "HIGH",
              "max_connections": 100000,
              "max_pending_requests": 100000,
              "max_requests": 100000
            }
          ]
        },
        "upstream_connection_options": {
          "tcp_keepalive": {
            "keepalive_time": 300
          }
        },
        "http2_protocol_options": { }
      }
    ],
    "listeners":[
      {
        "address": {
          "socket_address": {
            "protocol": "TCP",
            "address": "0.0.0.0",
            "port_value": 15090
          }
        },
        "filter_chains": [
          {
            "filters": [
              {
                "name": "envoy.http_connection_manager",
                "config": {
                  "codec_type": "AUTO",
                  "stat_prefix": "stats",
                  "route_config": {
                    "virtual_hosts": [
                      {
                        "name": "backend",
                        "domains": [
                          "*"
                        ],
                        "routes": [
                          {
                            "match": {
                              "prefix": "/stats/prometheus"
                            },
                            "route": {
                              "cluster": "prometheus_stats"
                            }
                          }
                        ]
                      }
                    ]
                  },
                  "http_filters": {
                    "name": "envoy.router"
                  }
                }
              }
            ]
          }
        ]
      }
    ]
  }              
```

## 动态静态配置

从启动配置可以看到，启动静态配置只有：

* listener

  * prometheus

* cluster

  * xDS grpc

  * prometheus

此外，envoy本身会为prometheus生成相应的路由配置，所以：

* route

  * prometheus
  
### 动态配置引用关系



* CDS

  * deprecated `repeated core.Address hosts = 7;`   可以直接带上地址

  * `ClusterLoadAssignment load_assignment = 33;`

  以上两个都需要 `DiscoveryType` 为 `STATIC`/`STRICT_DNS`/`LOGICAL_DNS`
  > 后两者估计是下发hostname

  而如果是`EDS`则为。。。

* EDS - `ClusterLoadAssignment`

  这一层是最底了，所以暂时没有。。。

  不过，当EGDS加上以后，这里可以是类似的。 `EDS -> groups -> exact group endpoints / EGDS`。

## config_dump配置内容

config_dump出的内容，会把静态配置和动态（via `*DS`协议）获取的分别组织呈现:

* `{"config": [${config}]}`

* `${config}`

  以下分块介绍

### BootstrapConfigDump

 `"@type": "type.googleapis.com/envoy.admin.v2alpha.BootstrapConfigDump"`

 * `bootstrap: {...}`

 * `static_resources: {...}`

   * `listeners: [${listener}]`

   * `clusters: [${cluster}]`

 * `dynamic_resources: [...]`

   `{l,c}ds_config: {ads: {}}`

   * `ads_config`

     ```json
     {
       "api_type": "GRPC",
       "grpc_services": [
        {
         "envoy_grpc": {
          "cluster_name": "xds-grpc"
         }
        }
       ]
     }      
     ```

### ClustersConfigDump

`"@type": "type.googleapis.com/envoy.admin.v2alpha.ClustersConfigDump"`

* `static_clusters: [...]`

  ```json
  {
   "cluster": {
    "name": "prometheus_stats",
    "type": "STATIC",
    "connect_timeout": "0.250s",
    "hosts": [
     {
      "socket_address": {
       "address": "127.0.0.1",
       "port_value": 15000
      }
     }
    ]
   },
   "last_updated": "2020-01-03T08:46:13.037Z"
  }
  ```

* `dynamic_active_clusters: [...]`

  ```json
  {
   "version_info": "2020-01-03T16:22:08+08:00/5",
   "cluster": {
    "name": "BlackHoleCluster",
    "type": "STATIC",
    "connect_timeout": "1s"
   },
   "last_updated": "2020-01-03T09:25:50.474Z"
  },
  {
   "version_info": "2020-01-03T16:22:08+08:00/5",
   "cluster": {
    "name": "InboundPassthroughClusterIpv4",
    "type": "ORIGINAL_DST",
    "connect_timeout": "1s",
    "lb_policy": "CLUSTER_PROVIDED",
    "circuit_breakers": {
     "thresholds": [
      {
       "max_connections": 4294967295,
       "max_pending_requests": 4294967295,
       "max_requests": 4294967295,
       "max_retries": 4294967295
      }
     ]
    },
    "upstream_bind_config": {
     "source_address": {
      "address": "127.0.0.6",
      "port_value": 0
     }
    }
   },
   "last_updated": "2020-01-03T09:25:50.477Z"
  },
  {
   "version_info": "2020-01-03T16:22:08+08:00/5",
   "cluster": {
    "name": "InboundPassthroughClusterIpv6",
    "type": "ORIGINAL_DST",
    "connect_timeout": "1s",
    "lb_policy": "CLUSTER_PROVIDED",
    "circuit_breakers": {
     "thresholds": [
      {
       "max_connections": 4294967295,
       "max_pending_requests": 4294967295,
       "max_requests": 4294967295,
       "max_retries": 4294967295
      }
     ]
    },
    "upstream_bind_config": {
     "source_address": {
      "address": "::6",
      "port_value": 0
     }
    }
   },
   "last_updated": "2020-01-03T09:25:50.477Z"
  },
  {
   "version_info": "2020-01-03T16:22:08+08:00/5",
   "cluster": {
    "name": "PassthroughCluster",
    "type": "ORIGINAL_DST",
    "connect_timeout": "1s",
    "lb_policy": "CLUSTER_PROVIDED",
    "circuit_breakers": {
     "thresholds": [
      {
       "max_connections": 4294967295,
       "max_pending_requests": 4294967295,
       "max_requests": 4294967295,
       "max_retries": 4294967295
      }
     ]
    }
   },
   "last_updated": "2020-01-03T09:25:50.476Z"
  },
  {
   "version_info": "2020-01-03T16:22:08+08:00/5",
   "cluster": {
    "name": "outbound|8080||xxx.com",
    "type": "EDS",
    "eds_cluster_config": {
     "eds_config": {
      "ads": {}
     },
     "service_name": "outbound|8080||xxx.com"
    },
    "connect_timeout": "1s",
    "circuit_breakers": {
     "thresholds": [
      {
       "max_connections": 4294967295,
       "max_pending_requests": 4294967295,
       "max_requests": 4294967295,
       "max_retries": 4294967295
      }
     ]
    },
    "lb_subset_config": {
     "fallback_policy": "DEFAULT_SUBSET",
     "default_subset": {
      "xxx": "default"
     },
     "subset_selectors": [
      {
       "keys": [
        "xxx"
       ]
      }
     ]
    }
   },
   "last_updated": "2020-01-03T09:25:50.404Z"
  }
  ```

### ListenersConfigDump

`"@type": "type.googleapis.com/envoy.admin.v2alpha.ListenersConfigDump"`

* `static_listeners: [...]`

  ```json
  {
   "listener": {
    "address": {
     "socket_address": {
      "address": "0.0.0.0",
      "port_value": 15090
     }
    },
    "filter_chains": [
     {
      "filters": [
       {
        "name": "envoy.http_connection_manager",
        "config": {
         "codec_type": "AUTO",
         "http_filters": {
          "name": "envoy.router"
         },
         "stat_prefix": "stats",
         "route_config": {
          "virtual_hosts": [
           {
            "name": "backend",
            "routes": [
             {
              "route": {
               "cluster": "prometheus_stats"
              },
              "match": {
               "prefix": "/stats/prometheus"
              }
             }
            ],
            "domains": [
             "*"
            ]
           }
          ]
         }
        }
       }
      ]
     }
    ]
   },
   "last_updated": "2020-01-03T08:46:13.041Z"
  }
  ```

* `dynamic_active_listeners: [...]`

  ```json
  {
   "version_info": "2020-01-03T16:22:08+08:00/5",
   "listener": {
    "name": "0.0.0.0_8080",
    "address": {
     "socket_address": {
      "address": "0.0.0.0",
      "port_value": 8080 
     }
    },
    "filter_chains": [
     {
      "filters": [
       {
        "name": "envoy.http_connection_manager",
        "typed_config": {
         "@type": "type.googleapis.com/envoy.config.filter.network.http_connection_manager.v2.HttpConnectionManager",
         "stat_prefix": "outbound_0.0.0.0_8080",
         "http_filters": [
          {
           "name": "envoy.cors"
          },
          {
           "name": "envoy.fault"
          },
          {
           "name": "envoy.router"
          }
         ],
         "tracing": {
          "operation_name": "EGRESS",
          "client_sampling": {
           "value": 100
          },
          "random_sampling": {
           "value": 100
          },
          "overall_sampling": {
           "value": 100
          }
         },
         "use_remote_address": false,
         "generate_request_id": true,
         "upgrade_configs": [
          {
           "upgrade_type": "websocket"
          }
         ],
         "stream_idle_timeout": "0s",
         "normalize_path": true,
         "rds": {
          "config_source": {
           "ads": {}
          },
          "route_config_name": "8080"
         }
        }
       }
      ]
     }
    ],
    "deprecated_v1": {
     "bind_to_port": true
    }
   },
   "last_updated": "2020-01-03T09:25:50.482Z"
  },
  {
   "version_info": "2020-01-03T16:22:08+08:00/5",
   "listener": {
    "name": "virtualOutbound",
    "address": {
     "socket_address": {
      "address": "0.0.0.0",
      "port_value": 15001
     }
    },
    "filter_chains": [
     {
      "filters": [
       {
        "name": "envoy.tcp_proxy",
        "typed_config": {
         "@type": "type.googleapis.com/envoy.config.filter.network.tcp_proxy.v2.TcpProxy",
         "stat_prefix": "PassthroughCluster",
         "cluster": "PassthroughCluster"
        }
       }
      ]
     }
    ],
    "use_original_dst": true,
    "transparent": true
   },
   "last_updated": "2020-01-03T09:25:50.485Z"
  },
  {
   "version_info": "2020-01-03T16:22:08+08:00/5",
   "listener": {
    "name": "virtualInbound",
    "address": {
     "socket_address": {
      "address": "0.0.0.0",
      "port_value": 15006
     }
    },
    "filter_chains": [
     {
      "filter_chain_match": {
       "prefix_ranges": [
        {
         "address_prefix": "0.0.0.0",
         "prefix_len": 0
        }
       ]
      },
      "filters": [
       {
        "name": "envoy.tcp_proxy",
        "typed_config": {
         "@type": "type.googleapis.com/envoy.config.filter.network.tcp_proxy.v2.TcpProxy",
         "stat_prefix": "InboundPassthroughClusterIpv4",
         "cluster": "InboundPassthroughClusterIpv4"
        }
       }
      ],
      "metadata": {
       "filter_metadata": {
        "pilot_meta": {
         "original_listener_name": "virtualInbound"
        }
       }
      }
     },
     {
      "filter_chain_match": {
       "prefix_ranges": [
        {
         "address_prefix": "::0",
         "prefix_len": 0
        }
       ]
      },
      "filters": [
       {
        "name": "envoy.tcp_proxy",
        "typed_config": {
         "@type": "type.googleapis.com/envoy.config.filter.network.tcp_proxy.v2.TcpProxy",
         "stat_prefix": "InboundPassthroughClusterIpv6",
         "cluster": "InboundPassthroughClusterIpv6"
        }
       }
      ],
      "metadata": {
       "filter_metadata": {
        "pilot_meta": {
         "original_listener_name": "virtualInbound"
        }
       }
      }
     }
    ],
    "listener_filters": [
     {
      "name": "envoy.listener.original_dst"
     }
    ],
    "transparent": true,
    "listener_filters_timeout": "1s",
    "continue_on_listener_filters_timeout": true
   },
   "last_updated": "2020-01-03T09:25:50.486Z"
  },
  ```

### ScopedRoutesConfigDump

`"@type": "type.googleapis.com/envoy.admin.v2alpha.ScopedRoutesConfigDump"`

### RoutesConfigDump

`"@type": "type.googleapis.com/envoy.admin.v2alpha.RoutesConfigDump"`

* `static_route_configs: [...]`

  ```json
  {
   "route_config": {
    "virtual_hosts": [
     {
      "name": "backend",
      "domains": [
       "*"
      ],
      "routes": [
       {
        "match": {
         "prefix": "/stats/prometheus"
        },
        "route": {
         "cluster": "prometheus_stats"
        }
       }
      ]
     }
    ]
   },
   "last_updated": "2020-01-03T08:46:13.040Z"
  }
  ```

* `dynamic_route_configs: [...]`

  ```json
  {
   "version_info": "2020-01-03T16:22:08+08:00/5",
   "route_config": {
    "name": "12220",
    "virtual_hosts": [
     {
      "name": "allow_any",
      "domains": [
       "*"
      ],
      "routes": [
       {
        "match": {
         "prefix": "/"
        },
        "route": {
         "cluster": "PassthroughCluster"
        }
       }
      ]
     },
     {
      "name": "xxx",
      "domains": [
       "xxx.com",
       "xxx.com:8080"
      ],
      "routes": [
       {
        "match": {
         "prefix": "/yyy/",
         "headers": [
          {
           "name": "header1",
           "exact_match": "for_test"
          },
          {
           "name": "header2",
           "exact_match": "for_test"
          }
         ]
        },
        "route": {
         "cluster": "outbound|8080||xxx.com",
         "metadata_match": {
          "filter_metadata": {
           "envoy.lb": {
            "xxx": "for_test"
           }
          }
         },
         "timeout": "0s"
        },
        "decorator": {
         "operation": "xxx/*"
        }
       },      
  ```

### SecretsConfigDump

  * `"@type": "type.googleapis.com/envoy.admin.v2alpha.SecretsConfigDump"`

# 版本演进

## v2 -> v3

基本没改动，都是deprecated -> reserved

* cds

  * `enum Cluster.LbPolicy`
    
    * `ORIGINAL_DST_LB = 4 [deprecated = true];` -> `ORIGINAL_DST_LB = 4 [deprecated = true];`

  * `message Cluster.LoadBalancingPolicy.Policy`

    * `google.protobuf.Struct config = 2 [deprecated = true];` -> `reserved "config";`

  * `message Cluster`

    * `auth.UpstreamTlsContext tls_context = 11 [deprecated = true];`/`map<string, google.protobuf.Struct> extension_protocol_options = 35 [deprecated = true];` -> `reserved "tls_context", "extension_protocol_options";`

* lds

  * `message Listener`

    * `google.protobuf.BoolValue use_original_dst = 4 [deprecated = true];` -> `reserved "use_original_dst";`
