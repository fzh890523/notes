

* https://helm.sh/

* https://helm.sh/docs/
* 



# basic



## values



* 子chart读不到父chart values，除非：

  * 父chart中定义在子chart name中的内容

    ```yaml
    # 父chart values
    subchart1:
      k1: v0  # 会merge到子chart（subchart1） values 并覆盖其 k1 （如果有）
    ```

  * 父chart中定义在`global`中的内容

    ```yaml
    # 父chart values
    global:
      k1: v0  # 会merge到子chart（subchart1） values.global 并覆盖其 k1 （如果有）
    ```

* 默认使用chart下`values.yaml`，可以`--values`/`-f`指定其他文件，可以多次传入来指定多个，还能`--set`来覆盖指定值

* `show values .`/`show values charts/subchart1` 可以查看父chart、指定子chart（自己）的values，**但不支持完整子chart values**

  可以子chart中加入：

  ```yaml
  # templates/print_values.yaml
  subchart1_values:
  {{ toYaml .Values | nindent 2 }}
  ```

  然后 `helm template .`来查看





## command

* `helm repo add stable https://charts.helm.sh/stable`

* `helm search repo stable`

  > searches the repositories that you have added to your local helm client (with helm repo add). This search is done over local data, and no public network connection is needed.

* `helm search hub xxx`

  > searches the [Artifact Hub](https://artifacthub.io/), which lists helm charts from dozens of different repositories

* `helm repo update`

* `helm ls`

* `helm uninstall smiling-penguin`

* `helm status smiling-penguin`

* `install`

  * `helm install happy-panda bitnami/wordpress`

  * `helm install -f values.yaml bitnami/wordpress --generate-name`

    > There are two ways to pass configuration data during install:
    >
    > * `--values` (or `-f`): Specify a YAML file with overrides. This can be specified multiple times and the rightmost file will take precedence
    >
    > * `--set`: Specify overrides on the command line.
    >
    >   * 优先于或者说会覆盖`-f`
    >
    >   * `--set name={a, b, c}`效果如下：
    >
    >     ```yaml
    >     name:
    >       - a
    >       - b
    >       - c
    >     ```
    >
    >   * `--set servers[0].port=80`
    >
    >     ```yaml
    >     servers:
    >       - port: 80
    >     ```
    >
    >   * `--set servers[0].port=80,servers[0].host=example`
    >
    >     ```yaml
    >     servers:
    >       - port: 80
    >         host: example
    >     ```
    >
    >   * `--set name=value1\,value2` -> `name: "value1,value2"`
    >
    >   * `--set nodeSelector."kubernetes\.io/role"=master` -> 
    >
    >     ```yaml
    >     nodeSelector:
    >       kubernetes.io/role: master
    >     ```
    >
    >   > 太深层/复杂结构的values可能难以用`--set`来设置值，所以建议在设计values时考虑这点

  * `helm install foo foo-0.1.1.tgz`

  * `helm install foo path/to/foo`

  * `helm install foo https://example.com/charts/foo-1.2.3.tgz`

  * `helm install test . --dry-run --debug -n testns`

    > `test`只是给一个release name

    模拟install，会执行对应的校验，甚至包括对k8s已有的资源...

* `upgrade`

  * `helm upgrade -f panda.yaml happy-panda bitnami/wordpress`
  * `helm upgrade --install <release name> --values <values file> <chart directory>`

* `rollback`

  > `helm rollback [RELEASE] [REVISION]`

  * `helm rollback happy-panda 1`

* `history`

  > `helm history [RELEASE]`

* `get`

  > a useful tool for looking at a release in the cluster

  * `helm get values happy-panda`

* `helm show values bitnami/wordpress`

* `create`
  
  * `helm create deis-workflow`
  
* `lint`

* `package`
  * `helm package deis-workflow` 得到一个tgz包
  * 
  
* `template`

  * `helm template test . -n testns --debug`

    输出生成的yamls内容



### auto completion

```sh
# 临时生效
source <(helm completion zsh) 

# 持久化
helm completion zsh > "${fpath[1]}/_helm"
```



### install

> `-name`参数已经废弃

* `helm install -n <ns> <name> .`

  指定ns安装当前目录下的...



### fetch/pull

`helm fetch xxx --untar`



## 一些约定

https://helm.sh/docs/chart_best_practices/

https://helm.sh/docs/chart_best_practices/conventions/



* chart name

  Chart names must be lower case letters and numbers. Words may be separated with dashes (-)

* version number: SemVer 2
* format: 2-space indent, never tab
* 





# ref



## 在线渲染



### helm-playground.com

实时渲染、三窗口（模板、value、结果），**挺好用**





# develop







## template

* 基于go tmpl

* 增强

  * 添加了[Sprig library](https://masterminds.github.io/sprig/) 中的所有函数

  * `include`

    ```yaml
    value: {{ include "mytpl" . | lower | quote }}
    
    {{ include "toYaml" $value | indent 2 }}
    ```

  * `required`

    ```yaml
    value: {{ required "A valid .Values.who entry required!" .Values.who }}
    ```

    



```
{{/* a comment */}}
{{- /* a comment with white space trimmed from preceding and following text */ -}}
	A comment; discarded. May contain newlines.
	Comments do not nest and must start and end at the
	delimiters, as shown here.

{{pipeline}}
	The default textual representation (the same as would be
	printed by fmt.Print) of the value of the pipeline is copied
	to the output.

{{if pipeline}} T1 {{end}}
	If the value of the pipeline is empty, no output is generated;
	otherwise, T1 is executed. The empty values are false, 0, any
	nil pointer or interface value, and any array, slice, map, or
	string of length zero.
	Dot is unaffected.

{{if pipeline}} T1 {{else}} T0 {{end}}
	If the value of the pipeline is empty, T0 is executed;
	otherwise, T1 is executed. Dot is unaffected.

{{if pipeline}} T1 {{else if pipeline}} T0 {{end}}
	To simplify the appearance of if-else chains, the else action
	of an if may include another if directly; the effect is exactly
	the same as writing
		{{if pipeline}} T1 {{else}}{{if pipeline}} T0 {{end}}{{end}}

{{range pipeline}} T1 {{end}}
	The value of the pipeline must be an array, slice, map, or channel.
	If the value of the pipeline has length zero, nothing is output;
	otherwise, dot is set to the successive elements of the array,
	slice, or map and T1 is executed. If the value is a map and the
	keys are of basic type with a defined order, the elements will be
	visited in sorted key order.

{{range pipeline}} T1 {{else}} T0 {{end}}
	The value of the pipeline must be an array, slice, map, or channel.
	If the value of the pipeline has length zero, dot is unaffected and
	T0 is executed; otherwise, dot is set to the successive elements
	of the array, slice, or map and T1 is executed.

{{template "name"}}
	The template with the specified name is executed with nil data.

{{template "name" pipeline}}
	The template with the specified name is executed with dot set
	to the value of the pipeline.

{{block "name" pipeline}} T1 {{end}}
	A block is shorthand for defining a template
		{{define "name"}} T1 {{end}}
	and then executing it in place
		{{template "name" pipeline}}
	The typical use is to define a set of root templates that are
	then customized by redefining the block templates within.

{{with pipeline}} T1 {{end}}
	If the value of the pipeline is empty, no output is generated;
	otherwise, dot is set to the value of the pipeline and T1 is
	executed.

{{with pipeline}} T1 {{else}} T0 {{end}}
	If the value of the pipeline is empty, dot is unaffected and T0
	is executed; otherwise, dot is set to the value of the pipeline
	and T1 is executed.
```

### if

https://helm.sh/docs/chart_template_guide/control_structures/#ifelse

似乎没办法判空和判nil？
比如显式传入空字符串、空列表，没办法区分？

> For templates, the operators (`eq`, `ne`, `lt`, `gt`, `and`, `or` and so on) are all implemented as functions. In pipelines, operations can be grouped with parentheses (`(`, and `)`).

* `{{ if .a }}`
* `{{ if not .a }}`
* `{{- if or (eq .Values.isCar true) (eq .Values.isBus true) }}`
* `{{- if or .Values.isCar .Values.isBus }}`



### range



### 数组（list）

https://helm.sh/docs/chart_template_guide/function_list/#lists-and-list-functions



* new
  * `{{ $myList := list 1 2 3 4 5 }}`
  * `{{ $myList := list }}` 初始化一个空的

* 支持的操作函数： 见链接

  > Helm provides the following list functions: append (mustAppend), compact (mustCompact), concat, first (mustFirst), has (mustHas), initial (mustInitial), last (mustLast), prepend (mustPrepend), rest (mustRest), reverse (mustReverse), seq, index, slice (mustSlice), uniq (mustUniq), until, untilStep, and without (mustWithout).



#### join



`{{- join "," .Values.xxlist | quote }}`





#### 取最后一个元素



`image: {{ $image := "" }}{{ range (split ”、“ .Values.image_xx) }}{{ $image = . }}{{ end }}{{ $image }}`



### dict（map）

https://helm.sh/docs/chart_template_guide/function_list/#dictionaries-and-dict-functions



* key需要是string
* value可以是任意类型，包括list、dict
* mutable



* new
  * `{{ $mydict := dict }}` new一个空的
  * `{{ $mydict := dict "key1" "value1" "key2" "value2" }}`

* 支持的操作函数： 见链接

  > Helm provides the following functions to support working with dicts: deepCopy (mustDeepCopy), dict, get, hasKey, keys, merge (mustMerge), mergeOverwrite (mustMergeOverwrite), omit, pick, pluck, set, unset, and values.



### 特殊case处理



#### nil处理

* nil无法和string比较，会报错，可以的做法

  * `{{- if eq (default "prod" $.Values.global.environment) "dev" }}`

    注意不是 <del>`{{- if eq (default $.Values.global.environment "prod") "dev" }}`</del>>

    差不多的写法

    `{{- if eq ($.Values.global.environment | default "prod") "dev" }}`

* <del>`{{- if (.Values.app.configMap.DEMO_MODE) and eq .Values.app.configMap.DEMO_MODE "1" }}`</del>

  > Returns the boolean AND of its arguments by returning the
  > 	first empty argument or the last argument, that is,
  > 	"and x y" behaves as "if x then y else x". All the
  > 	arguments are evaluated.

  helm里的`AND`没有短路效益...



#### `-` （前后换行处理）



* `{{ }}` 默认行为似乎是： 前后都不额外加空白

  但：

  ```yaml
  
  ```

  

* `{{- }}`  删除前面空白（包括空格、回车符）
* `{{ -}}` 删除后面...





## function



### nindent/indent



* nindent适合多行文本，会每行都... 
* indent则...只...



> 实测好像不是
>
> ```yaml
> example: 
>   - a
> {{ .Values.items | toYaml  | indent 2 }}
> 
> ---
> # values
> items: ["first", "second"]
> ```
>
> 
>
> * 无indent
>
>   ```yaml
>   ---
>   example: 
>     - a
>   - first
>   - second
>   ```
>
> * indent
>
>   ```yaml
>   ---
>   example: 
>     - a
>     - first
>     - second
>   ```
>
>   > `indent` 函数不会在文本块之前添加空行，是因为 `indent` 函数只对文本块的后续行应用缩进，而不是对整个文本块应用缩进。因此，它不会将换行符视为文本块的第一行，也就不会在文本块之前添加空行。相反，它只是对文本块的每一行应用缩进。
>   >
>   > > 在 Helm 模板中，文本块指的是一个或多个连续的文本行，它们由换行符分隔。在一个文本块中，第一行被称为文本块的第一行，而后续的行被称为文本块的后续行。
>
> * nindent
>
>   ```yaml
>   ---
>   example: 
>     - a
>   
>     - first
>     - second
>   ```
>
>   > `nindent` 函数在缩进文本时，可能会在文本前面添加一个空行，这是因为 Helm 在渲染模板时，会将模板中的多行文本块转换为一个单独的字符串，并在每行文本块之间添加一个换行符。这个换行符会被视为文本块的第一行，因此，在使用 `nindent` 函数缩进文本块时，第一行之前会出现一个空行。
>   >
>   > ```yaml
>   >   {{ "- xx" | nindent 3 }}
>   >   
>   > --- 
>   > # 输出
>   >   
>   >    - xx
>   > ```
>   >
>   > 
>   >
>   > ```yaml
>   >   {{ "- xx" | indent 3 }}
>   >   
>   > ---
>   > # 输出
>   >      - xx
>   > ```
>
> ```yaml
>   {{ "aa\nbb\ncc" }}
> ```
>
> * 无indent
>
>   ```yaml
>     aa
>   bb
>   cc
>   ```
>
> * indent
>
>   ```yaml
>        aa
>      bb
>      cc
>   ```
>
>   
>
> * nindent
>
>   ```yaml
>     
>      aa
>      bb
>      cc
>   ```
>
>   







