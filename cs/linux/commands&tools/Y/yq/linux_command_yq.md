
* [项目github-kislyuk/yq](https://github.com/kislyuk/yq)
* [项目github-mikefarah/yq](https://github.com/mikefarah/yq) 这个好像功能更完整，star人更多。 但比较啰嗦
* 安装： `pip3 install yq`



# kisyuk/yq


使用：
* `cat input.yml | yq .foo.bar`  yaml转为json、提取字段
  
  > 转为json竟然是默认行为...
* `cat input.yml | yq -y .foo.bar` yaml提取字段
* 语法似乎跟jq差不多  



# mikefarah/yq

* [gitbook](https://mikefarah.gitbook.io/yq/)



```sh
Usage:
  yq [flags]
  yq [command]

Available Commands:
  eval             Apply the expression to each document in each yaml file in sequence
  eval-all         Loads _all_ yaml documents of _all_ yaml files and runs expression once
  help             Help about any command
  shell-completion Generate completion script

Flags:
  -C, --colors                force print with colors
  -e, --exit-status           set exit status if there are no matches or null or false is returned
  -f, --front-matter string   (extract|process) first input as yaml front-matter. Extract will pull out the yaml content, process will run the expression against the yaml content, leaving the remaining data intact
  -h, --help                  help for yq
  -I, --indent int            sets indent level for output (default 2)
  -i, --inplace               update the yaml file inplace of first yaml file given.
  -M, --no-colors             force print with no colors
  -N, --no-doc                Don't print document separators (---)
  -n, --null-input            Don't read input, simply evaluate the expression given. Useful for creating yaml docs from scratch.
  -P, --prettyPrint           pretty print, shorthand for '... style = ""'
  -j, --tojson                output as json. Set indent to 0 to print json in one line.
      --unwrapScalar          unwrap scalar, print the value with no quotes, colors or comments (default true)
  -v, --verbose               verbose mode
  -V, --version               Print version information and quit

Use "yq [command] --help" for more information about a command.
```





* `cat input.yml | yq e '.a' -P`

  * `-`表示从stdin，否则需要显式传入filepath

  * `-P`表示转为yaml、`-j`表示转为json

    



# tutorial



## document



* https://mikefarah.gitbook.io/yq/operators/document-index



* document index



## file



* https://mikefarah.gitbook.io/yq/operators/file-operators



* merge

  * `yq eval-all x.yaml y.yaml`
  * `yq eval-all 'select(fi == 0) * select(filename == "file2.yaml")' file1.yaml file2.yaml`

* file name

  `yq 'filename' sample.yml`

* file index

  ```sh
  yq eval-all 'file_index' sample.yml another.yml
  
  0
  ---
  1
  ```

  













