

> `index`函数是Helm中的一个模板函数，它用于从一个映射类型的值中检索一个键的值。`index`函数接受两个参数：要检索的映射类型的值和要检索的键名称。
>
> 
>
> `index`函数的语法如下：
>
> 
>
> ```
> {{ index MAP KEY }}
> ```
>
> 
>
> 其中，`MAP`是要检索的映射类型的值，`KEY`是要检索的键名称。
>
> 
>
> 例如，假设您有一个名为`myConfig`的映射类型的值，其中包含了一些配置信息，您可以使用以下代码从该映射中检索一个键的值：
>
> 
>
> ```
> {{ index .Values.myConfig "myKey" }}
> ```
>
> 
>
> 该代码将返回`myConfig`映射中名为`myKey`的键的值。如果该键不存在，则返回零值。
>
> 
>
> 在管道符中，您可以使用`index`函数来检索一个映射类型的值中的一个键的值。例如，考虑以下模板代码：
>
> 
>
> ```
> {{ .Values.myConfig | index "myKey" }}
> ```
>
> 
>
> 在这个代码中，`.Values.myConfig`是要传递的值，`index`是要应用于该值的函数名称，而`"myKey"`是要传递给`index`函数的参数。因此，这个代码将从`.Values.myConfig`映射中检索键名称为`"myKey"`的值。
>
> 
>
> 需要注意的是，在使用管道符时，您可以使用`index`函数作为管道中的任何一个函数的参数，以便从另一个函数的输出中检索一个值。例如，考虑以下模板代码：
>
> 
>
> ```
> {{ .Values.myConfig | myFunction | index "myKey" }}
> ```
>
> 
>
> 在这个代码中，`.Values.myConfig`是要传递的值，`myFunction`是要应用于该值的函数名称，而`"myKey"`是要传递给`index`函数的参数。`myFunction`函数的输出将作为`index`函数的输入，并从中检索键名称为`"myKey"`的



> yonka: 不知为何，实际 `v | index "key"` 用不了，效果等同于 `index "key" v` 而正确的应该是 `index v "key"`
>
> 考虑到 `default "defaultv" v` == `v | default "defaultv"`，所以helm中管道符应该是把传值放到最后一个参数，所以index无法直接把值通过管道传入，而要 `(index (dosth "xxx") "key")`

