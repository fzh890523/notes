# tutorial



## install



### 一次安装多个

```shell
# 安装多个类似软件时
yum -y install httpd*

# 安装多个非类似软件时
yum -y install httpd php php-gd mysql
```





### 搜索/安装指定版本

* 搜索

  ```shell
  yum list ${name}
  ```

  如：

  ```shell
  yum list prce

  输出：
  pcre-7.8-6.el6.i686:Perl-compatibleregularexpressionlibrary
  pcre-7.8-6.el6.x86_64:Perl-compatibleregularexpressionlibrary
  pcre-7.8-6.el6.x86_64:Perl-compatibleregularexpressionlibrary
  ```

* 安装

  ```shell
  yum install ${full_package_name}  # with version
  ```

  如：

  ```shell
  yum install pcre-7.8-6.el6.i686 -y
  ```

  ​

## remove



```shell
# 卸载一个软件时
yum -y remove httpd
```





### 一次卸载多个



```shell
# 卸载多个相类似的软件
yum -y remove httpd*
# 卸载多个非类似软件时
yum -y remove httpd php php-gd mysql
```











