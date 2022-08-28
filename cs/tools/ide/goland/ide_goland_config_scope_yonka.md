
1. 新增一个scope： non-test-go-file
2. 直接编辑pattern内容为 `!file:*_test.go`
3. 支持与条件，如 `!file:*_test.go && !file:*.generated.*.go`

ref： 

* https://www.jetbrains.com/help/idea/configuring-scopes-and-file-colors.html

* https://www.jetbrains.com/help/idea/scope-language-syntax-reference.html
