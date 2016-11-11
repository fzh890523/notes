
# 文件操作

## 判断是否存在
```golang
    cpPath = path.Join(s.baseDir, cpFileName)
    if cpFileInfo, err = os.Stat(cpPath); err != nil {
        if os.IsNotExist(err) {
            err = nil
        } 
        return err
    }
    if cpFileInfo.IsDir() {
        return errors.New(fmt.Sprintf("cpFilePath %s is dir", cpPath))
    }
```

# path操作

## join
```golang
import path
var filePath := path.Join(baseDir, fileName)
```
