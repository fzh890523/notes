# go语言的flag包简单使用教程

看了一下go语言的flag包,然后写了点代码试了试

直接看代码

```Go
// TestFlag project main.go
package main

import (
	"flag""fmt"
)

func main() {
	// golang的flag包的一些基本使用方法

	// 待使用的变量
	var id int
	var name string
	var male bool

	// 是否已经解析
	fmt.Println("parsed? = ", flag.Parsed())

	// 设置flag参数 (变量指针，参数名，默认值，帮助信息)
	// 也可以用以下带返回值的方法代替，不过他们返回的是指针，比较麻烦点
	// Int(name string, value int, usage string) *int
	// String(name string, value string, usage string) *string
	// Bool(name string, value bool, usage string) *bool
	flag.IntVar(&id, "id", 123, "help msg for id")
	flag.StringVar(&name, "name", "default name", "help msg for name")
	flag.BoolVar(&male, "male", false, "help msg for male")

	// 解析
	flag.Parse()

	// 是否已经解析
	fmt.Println("parsed? = ", flag.Parsed())

	// 获取非flag参数
	fmt.Println("------ Args start ------")
	for i, v := range flag.Args() {
		fmt.Printf("arg[%d] = (%s).\n", i, v)
	}
	fmt.Println("------ Args end ------")

	// visit只包含已经设置了的flag
	fmt.Println("------ visit flag start ------")
	flag.Visit(func(f *flag.Flag) {
		fmt.Println(f.Name, f.Value, f.Usage, f.DefValue)
	})
	fmt.Println("------ visit flag end ------")

	// visitAll只包含所有的flag(包括未设置的)
	fmt.Println("------ visitAll flag start ------")
	flag.VisitAll(func(f *flag.Flag) {
		fmt.Println(f.Name, f.Value, f.Usage, f.DefValue)

	})
	fmt.Println("------ visitAll flag end ------")

	// flag参数
	fmt.Printf("id = %d\n", id)
	fmt.Printf("name = %s\n", name)
	fmt.Printf("male = %t\n", male)

	// flag参数默认值
	fmt.Println("------ PrintDefaults start ------")
	flag.PrintDefaults()
	fmt.Println("------ PrintDefaults end ------")

	// 非flag参数个数
	fmt.Printf("NArg = %d\n", flag.NArg())
	// 已设置的flag参数个数
	fmt.Printf("NFlag = %d\n", flag.NFlag())
}

```

**输入**

TestFlag.exe -id=123 -name="zhangsan" -male=true p1 p2 p3 p4

**输出**

```
parsed? =  false
parsed? =  true
------ Args start ------
arg[0] = (p1).
arg[1] = (p2).
arg[2] = (p3).
arg[3] = (p4).
------ Args end ------
------ visit flag start ------
id 123 help msg for id 123
male true help msg for male false
name zhangsan help msg for name default name
------ visit flag end ------
------ visitAll flag start ------
id 123 help msg for id 123
male true help msg for male false
name zhangsan help msg for name default name
------ visitAll flag end ------
id = 123
name = zhangsan
male = true
------ PrintDefaults start ------
  -id=123: help msg for id
  -male=false: help msg for male
  -name="default name": help msg for name
------ PrintDefaults end ------
NArg = 4
NFlag = 3
```

来源： <<http://qefee.com/2014/02/02/go%E8%AF%AD%E8%A8%80%E7%9A%84flag%E5%8C%85%E7%AE%80%E5%8D%95%E4%BD%BF%E7%94%A8%E6%95%99%E7%A8%8B/>>

 

------

```Go
    package main
    import (
        "fmt"
        "flag"
        )   
    func main(){
        var infile *string = flag.String("i", "infile", "File contains values for sorting")
        var outfile *string = flag.String("o", "outfile", "File to receive sorted values")
        var algorithm *string = flag.String("a", "qsort", "Sort algorithm")
        flag.Parse()
        if infile != nil {
            fmt.Println("infile =", *infile, "outfile =", *outfile, "algorithm =", *algorithm)
        }   
    }
    $$ ./flagdemo -h
    Usage of ./flagdemo:
      -a="qsort": Sort algorithm
      -i="infile": File contains values for sorting
      -o="outfile": File to receive sorted values
```