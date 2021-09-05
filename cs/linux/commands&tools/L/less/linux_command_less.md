

## 搜索

### 关闭正则： `/`+`ctrl+r`

* 输入`/`进入搜索
* 输入`ctrl+r`进入 `Regex-off /`





### 大小写不敏感搜索

```
-i or --ignore-case

Causes searches to ignore case; that is, uppercase and lowercase are considered identical.

This option is ignored if any uppercase letters appear in the search pattern; in other words, if a pattern contains uppercase letters, then that search does not ignore case.

-I or --IGNORE-CASE

Like -i, but searches ignore case even if the pattern contains uppercase letters.
```

* `-i` 可以“运行时”通过search pattern里包含大写字母来关闭 - 此时倒回到大小写敏感
* `-I` 不可以被如上方式“运行时”关闭，也即永远ignore



### follow-name模式（跟进文件变化）



* `+F`
* `--follow-name`



