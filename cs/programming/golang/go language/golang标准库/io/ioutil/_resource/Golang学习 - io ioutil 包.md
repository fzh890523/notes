# [Golang学习 - io/ioutil 包](http://www.cnblogs.com/golove/p/3278444.html)

**io/ioutil 包中的函数和方法**

 

```Go
// ioutil.go

------------------------------------------------------------

// ReadAll 读取 r 中的所有数据// 返回读取的数据和读取过程中遇到的任何错误// 如果读取成功，则 err 返回 nil，而不是 EOFfunc ReadAll(r io.Reader) ([]byte, error)

func main() {
	s := strings.NewReader("Hello World!")
	ra, _ := ioutil.ReadAll(s)
	fmt.Printf("%s", ra)
	// Hello World!
}

------------------------------------------------------------

// ReadFile 读取文件中的所有数据// 返回读取的数据和读取过程中遇到的任何错误// 如果读取成功，则 err 返回 nil，而不是 EOFfunc ReadFile(filename string) ([]byte, error)

func main() {
	ra, _ := ioutil.ReadFile("C:\\Windows\\win.ini")
	fmt.Printf("%s", ra)
}

------------------------------------------------------------

// WriteFile 向文件 filename 中写入数据 data// 如果文件不存在，则以 perm 权限创建该文件// 如果文件存在，则先清空文件，然后再写入// 返回写入过程中遇到的任何错误func WriteFile(filename string, data []byte, perm os.FileMode) errorfunc main() {
	fn := "C:\\Test.txt"
	s := []byte("Hello World!")
	ioutil.WriteFile(fn, s, os.ModeAppend)
	rf, _ := ioutil.ReadFile(fn)
	fmt.Printf("%s", rf)
	// Hello World!
}

------------------------------------------------------------

// ReadDir 读取目录 dirmane 中的所有目录和文件（不包括子目录）// 返回读取到的文件的信息列表和读取过程中遇到的任何错误// 返回的文件列表是经过排序的func ReadDir(dirname string) ([]os.FileInfo, error)

func main() {
	rd, err := ioutil.ReadDir("C:\\Windows")
	for _, fi := range rd {
		fmt.Println("")
		fmt.Println(fi.Name())
		fmt.Println(fi.IsDir())
		fmt.Println(fi.Size())
		fmt.Println(fi.ModTime())
		fmt.Println(fi.Mode())
	}
	fmt.Println("")
	fmt.Println(err)
}

------------------------------------------------------------

// NopCloser 将 r 封装为一个 ReadCloser 类型// 其 Close 方法不做任何事情func NopCloser(r io.Reader) io.ReadCloser

------------------------------------------------------------

// tempfile.go

------------------------------------------------------------

// TempFile 在目录 dir 中创建一个临时文件并将其打开// 文件名以 prefix 为前缀// 返回创建的文件的对象和创建过程中遇到的任何错误// 如果 dir 为空，则在系统的临时目录中创建临时文件// 如果环境变量中没有设置系统临时目录，则在 /tmp 中创建临时文件// 调用者可以通过 f.Name() 方法获取临时文件的完整路径// 调用 TempFile 所创建的临时文件，应该由调用者自己移除func TempFile(dir, prefix string) (f *os.File, err error) 

func main() {
	dn := "C:\\"
	f, _ := ioutil.TempFile(dn, "Test")
	fmt.Printf("%s", f.Name())
}

------------------------------------------------------------

// TempDir 功能同 TempFile，只不过创建的是目录// 返回值也只返目录的完整路径func TempDir(dir, prefix string) (name string, err error)

func main() {
	dn := "C:\\"
	f, _ := ioutil.TempDir(dn, "Test")
	fmt.Printf("%s", f.Name())
}





```

标签: [Golang包](http://www.cnblogs.com/golove/tag/Golang%E5%8C%85/)

来源： <<http://www.cnblogs.com/golove/p/3278444.html>>