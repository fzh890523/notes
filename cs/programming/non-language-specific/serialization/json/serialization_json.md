

# 数据类型









## binary - 不支持

JSON原生不支持binary，所以对于binary类型（如byte[]）的序列化，需要以”迂回“的方式。

fastjson默认行为是*以base64编码*，在接收端无法区分，所以要手动再转回来：

```java
com.alibaba.fastjson.JSONObject.getBytes(String)
```





```
There are 94 Unicode characters which can be represented as one byte according to the JSON spec (if your JSON is transmitted as UTF-8). With that in mind, I think the best you can do space-wise is base85 which represents four bytes as five characters. However, this is only a 7% improvement over base64, it's more expensive to compute, and implementations are less common than for base64 so it's probably not a win.

You could also simply map every input byte to the corresponding character in U+0000-U+00FF, then do the minimum encoding required by the JSON standard to pass those characters; the advantage here is that the required decoding is nil beyond builtin functions, but the space efficiency is bad -- a 105% expansion (if all input bytes are equally likely) vs. 25% for base85 or 33% for base64.

Final verdict: base64 wins, in my opinion, on the grounds that it's common, easy, and not bad enough to warrant replacement.
```











