## 过滤

### 文件类型 



`find ./ -type f`



### 文件名



`find ./ "*.go"`



### 路径



* `find ./ -not -path "./.git/*" `



### 多条件



* `find ./ -not -path "./.git/*" -not -path "./vendor/*"`

* `-and`
* `-or`



