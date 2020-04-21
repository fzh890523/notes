

### domain

目前了解到的唯一作用： 作为`search`没配置时的默认内容



### search

解析规则大致如下：

* `.`结尾的主机名视为FQDN，直接解析

  如`baidu.com.`

* 不包含`.`的主机名，会遍历`search`中的域名，以其作为后缀进行解析

  如`iMac`一般会拼接为`iMac.localdomain`

* 包含`.`（但不是末尾）的主机名，会先视为FQDN进行解析，解析失败的话再fallback为非FQDN重复前一步骤

  如：`notexist.com`解析失败后再尝试`notexist.com.localdomain`...



可见： search中配置的域名条目的数量，会决定解析（如果一直失败的话）的耗时

常见的search内容有 `local` `localdomain` 等

y



