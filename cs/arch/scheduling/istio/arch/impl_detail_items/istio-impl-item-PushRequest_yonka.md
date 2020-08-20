

### merge PushRequest

合并pushReq是1.4版本的较大亮点（虽然可能是之前就出来了），因为这样可以：

* 保证不丢push

* 不用做推送重试

目前merge的逻辑如下：

* `full = first.Full || other.Full`

* `full ? nil : first.EdsUpdates ∪ other.EdsUpdates`

* `features.ScopePushes.Get() ? (mergeNamespaceUpdated; mergeConfigTypesUpdated)` 

  * mergeNamespaceUpdated: `NamespacesUpdated = first.NamespacesUpdated ∪ other.NamespacesUpdated`

  * mergeConfigTypesUpdated: `ConfigTypesUpdated = first.ConfigTypesUpdated ∪ other.ConfigTypesUpdated`

