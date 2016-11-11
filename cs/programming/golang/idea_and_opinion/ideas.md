
# 简洁易用可读

## vs 特性

```
Go was always intended for programming at large scale, which includes working in large software environments, with large teams, and in the presence of complicating factors such as interoperating with other languages. The alias feature is directed at solving a problem that arises in that area, which is refactoring a package in an environment too large or complex to update all the dependents of a package at once. This problem is often seen as a versioning problem, but in monorepos versioning doesn't help, while aliases do.

Now, some of Go's popularity is also because of its clarity and readability, but it is worth observing that those properties, which were also goals of the language, are sometimes in conflict with the need for programming at scale. There are other features of the language that are there for industrial-level work but clearly at odds with simplicity. Goto is the most obvious (it's present for machine-generated code). The verbosity of struct literals is another. The way interface embedding works, allowing redundancy if it's not used, is a third. There are more.

What's really different in this conversation is that, for the first time, the community is seeing the difficulty of trying to decide what to do when a new issue raises such a conflict. If we today decided to start enforcing the need to put types on the fields of struct literals, there would be an outcry, but we would do it anyway, because the benefits outweigh the cost, high though the cost is.

The alias feature is important, but it is also ugly. We know that. It is open to abuse. We know that. But we have been listening: the feature is very heavily restricted because of the many good points raised in the issue discussion. The possibility for abuse is greatly restricted, and the conversation that got us here was invaluable. But the conversation is behind us; now the time has come to try it out. If it turns out to be a real problem in practice, there are three months left to roll it back.

But I don't think it will be a problem, because I believe the community will help make sure the abuses don't happen. If you see an alias appear in code where none is necessary, complain about it, publicly. Update golint to yell whenever the feature is used.

Most important, explain to people what it is for: that, like goto, it is a necessary evil for program development in the modern world.

-rob
```

> 来自： [google groups - golang-dev - Please do not add aliases to the language](https://groups.google.com/d/msg/golang-dev/OmjsXkyOQpQ/xn6YP-IHAwAJ)


```
@h12@Shanghai 我看了下那段rob的评论。 正好可以用来说前面泛型的问题。
https://groups.google.com/d/msg/golang-dev/OmjsXkyOQpQ/xn6YP-IHAwAJ
我理解的他的态度是： 如果确实有助于工程化/大规模编程的话，一些和简洁易用的原则冲突的甚至容易被滥用的特性，也是可以考虑的。
```


