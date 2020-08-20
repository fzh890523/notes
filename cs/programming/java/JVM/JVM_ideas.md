

# GC



## 对象直接分配在老年代



* 已有： 根据大小来选择是否分配在老年代

* 待： 业务场景“明知”对象生存期比较长的话，直接分配在老年代可以减少youngGC无谓的copy、升代的开销

  AJDK支持`-XX:+DirectTenuredAlloc -XX:+UseZenGC`，基于多租户场景/API：特定租户的对象分配都...

















