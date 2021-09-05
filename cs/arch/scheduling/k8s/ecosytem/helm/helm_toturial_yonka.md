

* https://helm.sh/

* https://helm.sh/docs/
* 



## basic

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

似乎没办法判空和判nil？
比如显式传入空字符串、空列表，没办法区分？





