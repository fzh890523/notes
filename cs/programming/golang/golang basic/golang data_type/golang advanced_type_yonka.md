
# 嵌入类型 - embed type - embedding

## ref

### documentation

* [effective_go.html#embedding](https://golang.org/doc/effective_go.html#embedding) 里用专门一节讲embedding
* [golang spec](https://golang.org/ref/spec)里则是分布在[struct types](https://golang.org/ref/spec#Struct_types)和[interface types](https://golang.org/ref/spec#Interface_types)里

## name conflict
```
Embedding types introduces the problem of name conflicts but the rules to resolve them are simple. First, a field or method X hides any other item X in a more deeply nested part of the type. If log.Logger contained a field or method called Command, the Command field of Job would dominate it.

Second, if the same name appears at the same nesting level, it is usually an error; it would be erroneous to embed log.Logger if the Job struct contained another field or method called Logger. However, if the duplicate name is never mentioned in the program outside the type definition, it is OK. This qualification provides some protection against changes made to types embedded from outside; there is no problem if a field is added that conflicts with another field in another subtype if neither field is ever used.
```

几个原则：
* 浅 > 深
* 同样深度/层级的冲突则报错。 冲突的name没有被**外部**使用的情况除外

