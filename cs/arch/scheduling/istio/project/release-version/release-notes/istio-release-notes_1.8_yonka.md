




## release-1.8



参见： 

* [Istio_Change Notes_1.8.html](_resource/Istio_Change Notes_1.8.html)

* [Istio_Upgrade Notes_1.8.html](_resource/Istio_Upgrade Notes_1.8.html)



### 1.8.1



**Changes**

- **Fixed** an issue showing unnecessary warnings when downgrading to a lower version of Istio. ([Issue #29183](https://github.com/istio/istio/issues/29183))
- **Fixed** an issue where a delegate `VirtualService` change would not trigger an xDS push. ([Issue #29123](https://github.com/istio/istio/issues/29123))
- **Fixed** a regression in Istio 1.8.0 causing workloads with multiple Services with overlapping ports to send traffic to the wrong port. ([Issue #29199](https://github.com/istio/istio/issues/29199))
- **Fixed** a bug causing Istio to attempt to validate resource types it no longer supports.





