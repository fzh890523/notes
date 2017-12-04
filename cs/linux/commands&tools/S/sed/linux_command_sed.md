



# tutorial



### 替换多个pattern



```shell
's/pattern1\|pattern2\|pattern3/replacement/g'
```



如：

```shell
echo 123haha456 | sed -e 's/123\|456/nn/g'
# output:
#   nnhahann
```



但`\|`限于GNU sed，**mac sed**不支持。 不过可以通过brew手动安装`gnu-sed`，最后binary为`gsed`。











