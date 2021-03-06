

## 介绍、语法

https://goessner.net/articles/JsonPath/



| **XPath** | **JSONPath**     | **Description**                                              |
| --------- | ---------------- | ------------------------------------------------------------ |
| /         | $                | the root object/element                                      |
| .         | @                | the current object/element                                   |
| /         | . or []          | child operator                                               |
| ..        | n/a              | parent operator                                              |
| //        | ..               | recursive descent. JSONPath borrows this syntax from E4X.    |
| *         | *                | wildcard. All objects/elements regardless their names.       |
| @         | n/a              | attribute access. JSON structures don't have attributes.     |
| []        | []               | subscript operator. XPath uses it to iterate over element collections and for [predicates](http://www.w3.org/TR/xpath#predicates). In Javascript and JSON it is the native array operator. |
| \|        | [,]              | Union operator in XPath results in a combination of node sets. JSONPath allows alternate names or array indices as a set. |
| n/a       | `[start:end:step]` | array slice operator borrowed from ES4.                      |
| []        | ?()              | applies a filter (script) expression.                        |
| n/a       | ()               | script expression, using the underlying script engine.       |
| ()        | n/a              | grouping in Xpath                                          |



```json
{ "store": {
    "book": [ 
      { "category": "reference",
        "author": "Nigel Rees",
        "title": "Sayings of the Century",
        "price": 8.95
      },
      { "category": "fiction",
        "author": "Evelyn Waugh",
        "title": "Sword of Honour",
        "price": 12.99
      },
      { "category": "fiction",
        "author": "Herman Melville",
        "title": "Moby Dick",
        "isbn": "0-553-21311-3",
        "price": 8.99
      },
      { "category": "fiction",
        "author": "J. R. R. Tolkien",
        "title": "The Lord of the Rings",
        "isbn": "0-395-19395-8",
        "price": 22.99
      }
    ],
    "bicycle": {
      "color": "red",
      "price": 19.95
    }
  }
}
```



| **XPath**              | **JSONPath**                           | **Result**                                                   |
| ---------------------- | -------------------------------------- | ------------------------------------------------------------ |
| `/store/book/author`   | `$.store.book[*].author`               | the authors of all books in the store                        |
| `//author`             | `$..author`                            | all authors                                                  |
| `/store/*`             | `$.store.*`                            | all things in store, which are some books and a red bicycle. |
| `/store//price`        | `$.store..price`                       | the price of everything in the store.                        |
| `//book[3]`            | `$..book[2]`                           | the third book                                               |
| `//book[last()]`       | `$..book[(@.length-1)]` `$..book[-1:]` | the last book in order.                                      |
| `//book[position()<3]` | `$..book[0,1]` `$..book[:2]`           | the first two books                                          |
| `//book[isbn]`         | `$..book[?(@.isbn)]`                   | filter all books with isbn number                            |
| `//book[price<10]`     | `$..book[?(@.price<10)]`               | filter all books cheapier than 10                            |
| `//*`                  | `$..*`                                 | all Elements in XML document. All members of JSON structure. |



### Operators

| Operator                  | Description                                                  |
| ------------------------- | ------------------------------------------------------------ |
| `$`                       | The root element to query. This starts all path expressions. |
| `@`                       | The current node being processed by a filter predicate.      |
| `*`                       | Wildcard. Available anywhere a name or numeric are required. |
| `..`                      | Deep scan. Available anywhere a name is required.            |
| `.<name>`                 | Dot-notated child                                            |
| `['<name>' (, '<name>')]` | Bracket-notated child or children                            |
| `[<number> (, <number>)]` | Array index or indexes                                       |
| `[start:end]`             | Array slice operator                                         |
| `[?(<expression>)]`       | Filter expression. Expression must evaluate to a boolean value. |

### Functions

Functions can be invoked at the tail end of a path - the input to a function is the output of the path expression. The function output is dictated by the function itself.

| Function | Description                                                  | Output   |
| -------- | ------------------------------------------------------------ | -------- |
| min()    | Provides the min value of an array of numbers                | Double   |
| max()    | Provides the max value of an array of numbers                | Double   |
| avg()    | Provides the average value of an array of numbers            | Double   |
| stddev() | Provides the standard deviation value of an array of numbers | Double   |
| length() | Provides the length of an array                              | Integer  |
| sum()    | Provides the sum value of an array of numbers                | Double   |
| keys()   | Provides the property keys (An alternative for terminal tilde `~`) | `Set<E>` |

### Filter Operators

Filters are logical expressions used to filter arrays. A typical filter would be `[?(@.age > 18)]` where `@` represents the current item being processed. More complex filters can be created with logical operators `&&` and `||`. String literals must be enclosed by single or double quotes (`[?(@.color == 'blue')]` or `[?(@.color == "blue")]`).

| Operator | Description                                                  |
| -------- | ------------------------------------------------------------ |
| ==       | left is equal to right (note that 1 is not equal to '1')     |
| !=       | left is not equal to right                                   |
| <        | left is less than right                                      |
| <=       | left is less or equal to right                               |
| >        | left is greater than right                                   |
| >=       | left is greater than or equal to right                       |
| =~       | left matches regular expression [?(@.name =~ /foo.*?/i)]     |
| in       | left exists in right [?(@.size in ['S', 'M'])]               |
| nin      | left does not exists in right                                |
| subsetof | left is a subset of right [?(@.sizes subsetof ['S', 'M', 'L'])] |
| anyof    | left has an intersection with right [?(@.sizes anyof ['M', 'L'])] |
| noneof   | left has no intersection with right [?(@.sizes noneof ['M', 'L'])] |
| size     | size of left (array or string) should match right            |
| empty    | left (array or string) should be empty                       |



## 库



* java
  * [json-path/JsonPath](https://github.com/json-path/JsonPath)

* python
  * [jsonpath-ng](https://pypi.org/project/jsonpath-ng/#description)



### python



#### jsonpath-ng

https://pypi.org/project/jsonpath-ng/



也做了一些扩展：

> **注意，以下要看source code**，因为如：
>
> ```python
> $.objects.`len`
> ```
>
> 的表达，变成如下显示了

| name              | Example                                                      |
| :---------------- | :----------------------------------------------------------- |
| len               | $.objects.`len`                                              |
| sub               | $.field.`sub(/foo\\+(.*)/, \\1)`                             |
| split             | $.field.`split(+, 2, -1)`$.field.`split(sep, segement, maxsplit)` |
| sorted            | $.objects.`sorted`$.objects[\some_field]$.objects[\some_field,/other_field] |
| filter            | $.objects[?(@some_field > 5)]<br />$.objects[?some_field = “foobar”)]<br />$.objects[?some_field =~ “foobar”)]<br />$.objects[?some_field > 5 & other < 2)] |
| arithmetic (-+*/) | $.foo + “_” + $.bar$.foo * 12$.objects[*].cow + $.objects[*].cat |



## 命令行

> yonka： 目前为止（2020/01/27）还没找到一个可用的 jsonpath cmd



### jq

可以apt安装

额这个好像就是单纯的jquery支持，把stdout解析成js对象然后访问，而不是jsonpath语法。

```sh
$ echo '{"arr":[1,2,3]}' |jq '.arr'
[
  1,
  2,
  3
]
$ echo '{"arr":[1,2,3]}' |jq '.arr[1]'
2
$ echo '{"arr":[1,2,3]}' |jq '.arr[*]'
jq: error: syntax error, unexpected '*' (Unix shell quoting issues?) at <top-level>, line 1:
.arr[*]
jq: 1 compile error
```



### [github-trentm-json](https://github.com/trentm/json)

这个也不是jsonpath，而是自己支持了一系列json数据的处理操作



