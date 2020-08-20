

# INDEX

- [Introduction](https://note.wiz.cn/web?cmd=ca%2C%2FProgramming%2FGo%2F%E9%AB%98%E7%BA%A7%2F%E5%8F%8D%E5%B0%84%2F&dc=94305d75-0e89-4ac2-8820-4a8bece4ad92&kb=89077880-eff4-11e0-a402-00237def97cc#Introduction)
- [Types and interfaces](#Types and interfaces)
- [The representation of an interface](#The representation of an interface)
- [The laws of reflection](#The laws of reflection)
  - [The first law of reflection](#The first law of reflection)
  - [The second law of reflection](#The second law of reflection)
  - [The third law of reflection](#The third law of reflection)
- [Structs](https://note.wiz.cn/web?cmd=ca%2C%2FProgramming%2FGo%2F%E9%AB%98%E7%BA%A7%2F%E5%8F%8D%E5%B0%84%2F&dc=94305d75-0e89-4ac2-8820-4a8bece4ad92&kb=89077880-eff4-11e0-a402-00237def97cc#Structs)
- [Conclusion](https://note.wiz.cn/web?cmd=ca%2C%2FProgramming%2FGo%2F%E9%AB%98%E7%BA%A7%2F%E5%8F%8D%E5%B0%84%2F&dc=94305d75-0e89-4ac2-8820-4a8bece4ad92&kb=89077880-eff4-11e0-a402-00237def97cc#Conclusion)

# Introduction

反射： 程序检查自己的结构的能力，尤其是通过类型。 
反射是元编程的一种形式。 也带来很多困扰。

本文中尝试澄清go中的反射是如何工作的。 各种语言的反射模型都有所不同（很多语言甚至不支持），本文是关于go的，所以 *反射* 要理解为 *go中的反射*。

# Types and interfaces

先关注一下go中的类型。 
go是静态类型语言，每个变量都有一个静态类型，也即在编译期间知道/确定的（一个）类型： int、float64、*MyType、[]byte 等。 主要通过 声明、类型推导 等来确定。

```
type MyInt intvar i intvar j MyInt
```

i和j的（静态）类型不同，虽然有相同的底层类型。 不通过类型转换的话两者不能互相赋值。

还有一类重要的类型是 接口类型，表示固定的方法集。 接口类型的变量可以保存任何具体（非接口）类型的值，只要该值（具体类型）实现了该接口的方法。

```
// Reader is the interface that wraps the basic Read method.type Reader interface {    Read(p []byte) (n int, err error)}// Writer is the interface that wraps the basic Write method.type Writer interface {    Write(p []byte) (n int, err error)}// 使用var r io.Readerr = os.Stdinr = bufio.NewReader(r)r = new(bytes.Buffer)// and so on
```

注意这里： 无论r的实际值（具体类型）是什么，r这个变量的（静态）类型都是 io.Reader。

一个接口类型的极其重要的例子是 空接口：

```
interface{}

```

它表示空的方法集，所以任何值（具体类型）都可以满足它，也即任何值都可以赋给空接口类型的变量。

**一种误解**： **go的接口是动态类型** 
实际上接口也是静态类型，一种接口类型（I）的变量永远都是这个静态类型（I），即使运行时存储在该变量的值可能改变（其具体）类型。

# The representation of an interface

接口类型的变量保存了一个 类型&值对： 赋给该变量的实际值 和 该值的类型描述。 更准确的说，该值是底层的具体（类型）值，也即实现了该接口的那个具体类型的值，该类型则是… 
**注意： 并不会保存接口类型信息**

```
var r io.Readertty, err := os.OpenFile("/dev/tty", os.O_RDWR, 0)  // 返回 *os.Fileif err != nil {    return nil, err}r = tty  // 此时r的静态类型为 io.Reader，保存的值为tty，保存的类型信息为 *os.File - 也即 类型&值对为 (tty, *os.File)var w io.Writerw = r.(io.Writer)  // 正是因为r的底层类型为*os.File，此处才能转为...var empty interface{}empty = w  // 类型&值对仍为 (tty, *os.File)
```

*接口的静态类型决定了接口变量能调用什么方法*，即使底层具体类型有很多其他方法。

# The laws of reflection

## The first law of reflection

**反射： 从 接口值 到 反射对象** 
通过reflect包中的**Type**和**Value**这两个类型，以及**TypeOf**和**ValueOf**这两个函数，可以访问接口变量中的类型。

```
// TypeOf returns the reflection Type of the value in the interface{}.func TypeOf(i interface{}) Typevar t reflect.Type = reflect.TypeOf(o)var v reflect.Value = reflect.ValueOf(o)
```

```
package mainimport (    "fmt"    "reflect")func main() {    var x float64 = 3.4    fmt.Println("type:", reflect.TypeOf(x))  // 输出： type: float64    fmt.Println("value:", reflect.ValueOf(x))  // 输出： value: <float64 Value>    v := reflect.ValueOf(x)    fmt.Println("kind is float64:", v.Kind() == reflect.Float64)  // kind is float64: true}
```

Value的**SetInt**、**SetFloat**等方法，不过需要可设值属性，见第三节关于可设值的讨论。 
Value的**Type**、**Kind**方法，前者返回静态类型（？），后者返回实际类型。

```
type MyInt intvar x MyInt = 7v := reflect.ValueOf(x)  // v.Type()得到的是MyInt，而v.Kind()得到的是int
```

**yonka**

```
// 接前一段var i interface{} = x  // 此时i保存的值为7，而类型信息为MyInt
```

## The second law of reflection

**反射： 从 反射对象 到 接口值**

Value的**Interface**方法： ValueOf方法的反向…

```
// Interface returns v's value as an interface{}.
func (v Value) Interface() interface{}

```

可以通过Interface方法从Value恢复到接口类型，然后再…

```
y := v.Interface().(float64) // y will have type float64.fmt.Println(y)fmt.Println(v.Interface())fmt.Printf("value is %7.1e\n", v.Interface())
```

## The third law of reflection

**要修改反射对象，则该值必须是可以设值的（settable）**

```
var x float64 = 3.4v := reflect.ValueOf(x)v.SetFloat(7.1) // Error: will panic.// 输出： panic: reflect.Value.SetFloat using unaddressable valuevar x float64 = 3.4v := reflect.ValueOf(x)fmt.Println("settability of v:", v.CanSet())// 输出： settability of v: false
```

not addressable ---> not settable 
settability是Value的属性，不是所有的Value都有该属性。 可以通过 Value.CanSet()来判断。 调用不能set的Value的Set会导致panic。 
settability有点像addressability，但更严格。 它表示一个反射对象是否能修改*用来创建该反射对象的实际存储*的内容。 它是由反射对象是否hold原始item决定的。 如：

```
var x float64 = 3.4v := reflect.ValueOf(x)  // v得到的是一份拷贝，而不是x自己// 如果这里v.SetFloat(7.1)能执行成功的话，则表明可以影响到x的值，但这是不成立的，所以...var x float64 = 3.4p := reflect.ValueOf(&x) // Note: take the address of x.fmt.Println("type of p:", p.Type())  // 输出： type of p: *float64fmt.Println("settability of p:", p.CanSet())  // 输出： settability of p: false// 这里可以看到指针仍然不是settable的（set一个指针进去也对原来的指针内容无影响），而p.Elem()则是v := p.Elem()fmt.Println("settability of v:", v.CanSet())  // 输出： settability of v: truev.SetFloat(7.1)fmt.Println(v.Interface())  // 输出： 7.1fmt.Println(x)  // 输出： 7.1
```

# Structs

从上面一节可以得知要通过反射改变结构体的内容（字段值），方式如下：

```
type T struct {    A int    B string}t := T{23, "skidoo"}s := reflect.ValueOf(&t).Elem()typeOfT := s.Type()for i := 0; i < s.NumField(); i++ {    f := s.Field(i)    fmt.Printf("%d: %s %s = %v\n", i,        typeOfT.Field(i).Name, f.Type(), f.Interface())}// 输出：// 0: A int = 23// 1: B string = skidoo
```

Type/Value类型的 **NumField()** 和 **Field(i int)**方法。 取field，Type取到的是**StructField**类型，Value取到的是**Value**类型。

> *只有导出字段才是可设值的*

```
s.Field(0).SetInt(77)s.Field(1).SetString("Sunset Strip")fmt.Println("t is now", t)  // 输出： t is now {77 Sunset Strip}
```

> 如果把t传入reflect.VlaueOf而不是&t的话，设值时会失败，因为t的字段是不可设值的 = =

# Conclusion

- 可以从接口值到反射对象做反射
- 也可以从反射对象到接口值做反射
- 要修改反射对象，需要对应的值是可设值的

还有一些反射场景没有涉及：

- 从channel收发
- 分配内存
- 使用slice和map
- 调用方法和函数





